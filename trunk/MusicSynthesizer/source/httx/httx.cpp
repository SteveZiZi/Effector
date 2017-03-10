/** @file
 *  @brief ��̨ͨѶ�����ܶ���
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#include "httx.h"
#include "sysipc.h"
#include "htrace.h"
#include "context.h"
#include "status.h"
#include "connfact.h"
#include "flowctrl.h"

C_HttxServer::C_HttxServer(void)
{
	m_tickRecvMsData = 0;
	m_HttxSN = MP_HTTX_1;
	m_pChannel = NULL;
	m_pMStationConnect = NULL;
	m_pConnectStatus = NULL;
	m_pHttxStatus = NULL;
	
}

C_HttxServer::~C_HttxServer()
{

}

int C_HttxServer::BeforeService(void)
{
	m_pConnectStatus = CONTEXT.GetConnectStatus();
	m_pHttxStatus = CONTEXT.GetHttxStatus(m_HttxSN);
	m_pDspDevice = new C_DspDevice();
	m_pDspDevice->StartUp();

	return 0;
}

int C_HttxServer::AfterService(void)
{
	return 0;
}

int C_HttxServer::OnTickService(void)
{
	for (int i = HTTX_CHANNEL_1; i < HTTX_CHANNL_MAX; i += 1)
	{
		/** @brief ��������ͨ�� */
		m_pChannel = STATUS.GetHttxChannel(m_HttxSN, (S_ChannelSN)i);
	
		HeartBeat();
		if (m_pChannel == NULL)
		{
			loget<<"����ͨ��("<<(int)i<<")ʧ��!!!"<<endl;
			SleepSecond(5);
			continue;
		}

		if ((m_pMStationConnect = C_ConnFact::CreateConnection(*m_pChannel)) == NULL)
		{
			loget<<"����ͨ��("<<m_pChannel->m_Name<<")ʧ��!!!"<<endl;
			SleepSecond(5);
			continue;
		}
		
				
		ChannelServer();

		C_ConnFact::DestroyConnection(m_pMStationConnect);
		SleepMS(20);
	}

	return 0;
}

#define CONNECT_RETRY_TIMES_MAX	3

#define KGESG_TC_MAGIC        'T'
#define KGESG_TC_READ_USB_MODE                   _IOR(KGESG_TC_MAGIC, 2, int)

int C_HttxServer::ChannelServer(void)
{
#ifndef WIN32
//	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	int usbDetect;
	
	if (m_pHttxStatus->m_fMasterTerm == true && m_HttxSN == MP_HTTX_1)
		m_pConnectStatus->m_Httx1Logon = false,m_pConnectStatus->m_Httx1LogonTime = 0;	
	m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;

	if (m_pChannel->NeedPPP() == true)
	{
		if (WaitPppDial() < 0)
			return -1;
	}

	int fd = open("/dev/kgetc", O_RDWR);
	if (fd < 0)
	{
		loget<<"open /dev/kgetc fail"<<endl;
		return -1;
	}
	
	ioctl(fd, KGESG_TC_READ_USB_MODE, &usbDetect);
	if (usbDetect == 0)//USB�������Ƿ�γ���usbDetect=0Ϊ�γ���1Ϊ����
	{
		logwt<<"USB��δ����"<<endl;
		close(fd);
		return -1;
	}
	
	if (ConnectMasterStation() < 0)
		return -1;
	logwt<<"���ӳɹ�!!!"<<endl;

	m_tickRecvMsData = CONTEXT.GetSystemTick();
	//clear cmd buffer
	m_strRecvMsBuffer.clear();
	m_strResponseCmds.clear();

	m_pConnectStatus->m_Reconnect = false; ///< ��ֹ�ڱ��ز�������λ

	while (m_pMStationConnect->IsConnected())
	{
		CmdProcess(); // ���ղ��������վ��������
		OffLineCheck();
		DataTransfer();
		HeartBeat();
		SleepMS(1);

		ioctl(fd, KGESG_TC_READ_USB_MODE, &usbDetect);

		if (usbDetect == 0)// USB�߰γ����˳�����״̬
		{
			close(fd);
			logwt<<"USB���Ѱγ�"<<endl;
			break;
		}
	}
	m_pMStationConnect->Disconnect();
	logwt<<"���ӶϿ�!!!"<<endl;
	
	if (m_pHttxStatus->m_fMasterTerm == true && m_HttxSN == MP_HTTX_1)
	{
		m_pConnectStatus->m_Httx1Logon = false;
		m_pConnectStatus->m_Httx1LogonTime = 0;	
	}
#endif
	return 0;
}

int C_HttxServer::CmdProcess(void)
{
	if (m_strRecvMsBuffer.size() > 0)
	{
		string strCmd;
		string oStrData;
	
		if (m_protocol.Handle(m_pDspDevice,m_strRecvMsBuffer, oStrData) == 0)
		{
			m_pMStationConnect->SendData(oStrData);
		}
	}
	return 0;
}

int C_HttxServer::DataTransfer(void)
{
	/** @brief Send Data */
	if (!m_strResponseCmds.empty())
	{
		do{
			if (MStationSend(*m_strResponseCmds.begin()) <= 0)
				break;
			SleepMS(100);
			m_strResponseCmds.erase(m_strResponseCmds.begin());
		}while (!m_strResponseCmds.empty());
		MsCmdCount();
	}
	
	/** @brief Recv Data */
	if (MStationRecv(m_strRecvMsBuffer) > 0)
		m_tickRecvMsData = CONTEXT.GetSystemTick();
	return 0;
}






int C_HttxServer::OffLineCheck(void)
{
	if (NeedOffLine() == true)
		m_pMStationConnect->Disconnect();
	return 0;
}




bool C_HttxServer::NeedOffLine(void)
{
	if (m_HttxSN == MP_HTTX_1 && m_pConnectStatus->m_Reconnect == true)
	{
		logwt<<"�յ���������֪ͨ,�Ͽ�����..."<<endl;
		m_pConnectStatus->m_Reconnect = false;

		/** @brief confirm forward info */

		return true;
	}
	if (m_pChannel->m_IdleTimeoutS > 0 && CONTEXT.GetSystemTick() > m_tickRecvMsData+m_pChannel->m_IdleTimeoutS)
	{
		logwt<<"ͨ�����г�ʱ"<<m_pChannel->m_IdleTimeoutS<<"��,�Ͽ�����..."<<endl;
		return true;
	}

	if (m_pChannel->NeedPPP() == true)
	{
		if (m_pConnectStatus->m_PppOnline == false)
		{
			logwt<<"PPP������,�Ͽ�����..."<<endl;
			return true;
		}

		if (FLOWCTRL.FlowCtrlEnable() == true && FLOWCTRL.GetLogonFailedTimes() >= FLOWCTRL.GetLogonRetryTimes())
		{
			FLOWCTRL.LogonRecord(true);
			FLOWCTRL.SocketConnectRecord(false);//��½ʧ��,����ʧ��
			return true;
		}
		
		if (FLOWCTRL.FlowCtrlEnable() == false && FLOWCTRL.GetLogonFailedTimes() > 3)
		{
			FLOWCTRL.LogonRecord(true);
			return true;
		}
	}
	else
	{// LAN
		if (FLOWCTRL.GetLogonFailedTimes() > 3)
		{
			FLOWCTRL.LogonRecord(true);
			return true;
		}
	}
	S_SYSCFG *pSysCfg = STATUS.GetSystemConfig();
	if (FLOWCTRL.GetHeartBeatFailedTimes() >= pSysCfg->m_HeartBeatRetryTimes)
	{
		FLOWCTRL.HeartBeatRecord(true);
		return true;
	}
	return false;
}

int C_HttxServer::WaitPppDial(void)
{
	m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = true;
	logit<<"�ȴ�PPP����......"<<endl;
	INT32U timeoutS = 0;
	while (m_pConnectStatus->m_PppOnline == false)
	{
		if (m_pChannel->NeedPPP() == false)///��ֹ�ڱ��ز�������Ϊno ppp
			return 0;
		HeartBeat(), SleepSecond(1);
		if ((++timeoutS % 120) == 0)
			logit<<"�����ȴ�......"<<endl;
		if (timeoutS > 600) //	logit<<"�л�ͨ������..."<<endl;
			return -1;
	}
	logit<<"PPP ���ųɹ�!"<<endl;
	return 0;
}

int C_HttxServer::ConnectMasterStation(void)
{
	if (FLOWCTRL.FlowCtrlEnable() == true && m_pChannel->NeedPPP() == true)
	{
		while (FLOWCTRL.GetSocketConnectFailedTimes() < FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"FLOWCTRL:��"<<FLOWCTRL.GetSocketConnectFailedTimes()+1<<"������......"<<endl;
			if (m_pMStationConnect->Connect() == 0)
				return 0;
			FLOWCTRL.SocketConnectRecord(false);
			logwt<<"FLOWCTRL:������վʧ��,˯��"<<FLOWCTRL.GetSocketConnectInterSecond()<<"��......"<<endl;
			for (INT32U Second = 0; Second < FLOWCTRL.GetSocketConnectInterSecond(); Second++)
				HeartBeat(), SleepSecond(1);
		}
		if (FLOWCTRL.GetSocketConnectFailedTimes() >= FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"����ʧ�ܴ���="<<FLOWCTRL.GetSocketConnectFailedTimes()<<",֪ͨPPP���²���......"<<endl;
			m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
			FLOWCTRL.SocketConnectRecord(true);
			SleepSecond(5);//make sure PPP redial
		}
	}
	else
	{
		for (int i = 0; i < CONNECT_RETRY_TIMES_MAX;)
		{
			logit<<"��"<<i+1<<"������......"<<endl;
			int ok = m_pMStationConnect->Connect();
			if (ok == 0)
			{
				return 0;
			}
			else if (ok > 0)
			{
				logit<<"δ����,��������ʱ�仹��"<<ok<<"��..."<<endl;
				for (int j = 0; j < ok; j++)
					HeartBeat(), SleepSecond(1);
			}
			else
			{
				i++;
				logit<<"����ʧ��..."<<endl;
			}
		}
		if (m_pMStationConnect->IsConnected() == false)
		{
			if (m_pChannel->NeedPPP() == true)
			{
				logwt<<"����ʧ�ܴ���="<<CONNECT_RETRY_TIMES_MAX<<",֪ͨPPP���²���......"<<endl;
				m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
				SleepSecond(5);//make sure PPP redial
			}
		}
	}
	return -1;
}

int C_HttxServer::MStationSend(const string &strCmd)
{
	if (m_pHttxStatus->m_fForwardEnable == false)//��ֹ������վ����
		return 0;//�����ն˵�HTTX1�п���Ϊfalse

	int Cnt = m_pMStationConnect->SendData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":��������:"<<HEX<<strCmd<<"ʱ���ӳ���!!!"<<endl;
		m_pMStationConnect->Disconnect();
	}
	else if (Cnt > 0)
	{
		if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
			m_pHttxStatus->m_fForwardEnable = false;//���ն˵�HTTX1������һ����false,�ȼ�������֡
		m_pConnectStatus->m_CommunicationTime = C_TIME::TimeNow();
	}

	return Cnt;
}

int C_HttxServer::MStationRecv(string &strCmd)
{
	int Cnt = m_pMStationConnect->RecvData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":��������ʱ���ӳ���!!!"<<endl;
		m_pMStationConnect->Disconnect();
	}
	else if (Cnt > 0)
	{
		m_pConnectStatus->m_CommunicationTime = C_TIME::TimeNow();
	}

	return Cnt;
}

int C_HttxServer::MsCmdCount(void)
{
	if (m_pHttxStatus->m_ResponseCmdCnt != m_strResponseCmds.size())
		m_pHttxStatus->m_ResponseCmdCnt = (S_WORD)m_strResponseCmds.size();
	return 0;
}

//
//
//
C_Httx1Server::C_Httx1Server(void)
{
	m_HttxSN = MP_HTTX_1;
}

C_Httx1Server::~C_Httx1Server()
{
}

C_Httx2Server::C_Httx2Server(void)
{
	m_HttxSN = MP_HTTX_2;
}

C_Httx2Server::~C_Httx2Server()
{
};

C_Httx3Server::C_Httx3Server(void)
{
	m_HttxSN = MP_HTTX_3;
}

C_Httx3Server::~C_Httx3Server()
{
};

