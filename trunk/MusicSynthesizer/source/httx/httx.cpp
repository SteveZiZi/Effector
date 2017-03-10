/** @file
 *  @brief 后台通讯服务功能定义
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
		/** @brief 查找上行通道 */
		m_pChannel = STATUS.GetHttxChannel(m_HttxSN, (S_ChannelSN)i);
	
		HeartBeat();
		if (m_pChannel == NULL)
		{
			loget<<"查找通道("<<(int)i<<")失败!!!"<<endl;
			SleepSecond(5);
			continue;
		}

		if ((m_pMStationConnect = C_ConnFact::CreateConnection(*m_pChannel)) == NULL)
		{
			loget<<"创建通道("<<m_pChannel->m_Name<<")失败!!!"<<endl;
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
	if (usbDetect == 0)//USB串口线是否拔出，usbDetect=0为拔出，1为插入
	{
		logwt<<"USB线未插入"<<endl;
		close(fd);
		return -1;
	}
	
	if (ConnectMasterStation() < 0)
		return -1;
	logwt<<"连接成功!!!"<<endl;

	m_tickRecvMsData = CONTEXT.GetSystemTick();
	//clear cmd buffer
	m_strRecvMsBuffer.clear();
	m_strResponseCmds.clear();

	m_pConnectStatus->m_Reconnect = false; ///< 防止在本地操作中置位

	while (m_pMStationConnect->IsConnected())
	{
		CmdProcess(); // 接收并处理从主站来的数据
		OffLineCheck();
		DataTransfer();
		HeartBeat();
		SleepMS(1);

		ioctl(fd, KGESG_TC_READ_USB_MODE, &usbDetect);

		if (usbDetect == 0)// USB线拔出，退出连接状态
		{
			close(fd);
			logwt<<"USB线已拔出"<<endl;
			break;
		}
	}
	m_pMStationConnect->Disconnect();
	logwt<<"连接断开!!!"<<endl;
	
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
		logwt<<"收到重新连接通知,断开连接..."<<endl;
		m_pConnectStatus->m_Reconnect = false;

		/** @brief confirm forward info */

		return true;
	}
	if (m_pChannel->m_IdleTimeoutS > 0 && CONTEXT.GetSystemTick() > m_tickRecvMsData+m_pChannel->m_IdleTimeoutS)
	{
		logwt<<"通道空闲超时"<<m_pChannel->m_IdleTimeoutS<<"秒,断开连接..."<<endl;
		return true;
	}

	if (m_pChannel->NeedPPP() == true)
	{
		if (m_pConnectStatus->m_PppOnline == false)
		{
			logwt<<"PPP不在线,断开连接..."<<endl;
			return true;
		}

		if (FLOWCTRL.FlowCtrlEnable() == true && FLOWCTRL.GetLogonFailedTimes() >= FLOWCTRL.GetLogonRetryTimes())
		{
			FLOWCTRL.LogonRecord(true);
			FLOWCTRL.SocketConnectRecord(false);//登陆失败,建链失败
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
	logit<<"等待PPP拨号......"<<endl;
	INT32U timeoutS = 0;
	while (m_pConnectStatus->m_PppOnline == false)
	{
		if (m_pChannel->NeedPPP() == false)///防止在本地操作中置为no ppp
			return 0;
		HeartBeat(), SleepSecond(1);
		if ((++timeoutS % 120) == 0)
			logit<<"继续等待......"<<endl;
		if (timeoutS > 600) //	logit<<"切换通道尝试..."<<endl;
			return -1;
	}
	logit<<"PPP 拨号成功!"<<endl;
	return 0;
}

int C_HttxServer::ConnectMasterStation(void)
{
	if (FLOWCTRL.FlowCtrlEnable() == true && m_pChannel->NeedPPP() == true)
	{
		while (FLOWCTRL.GetSocketConnectFailedTimes() < FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"FLOWCTRL:第"<<FLOWCTRL.GetSocketConnectFailedTimes()+1<<"次连接......"<<endl;
			if (m_pMStationConnect->Connect() == 0)
				return 0;
			FLOWCTRL.SocketConnectRecord(false);
			logwt<<"FLOWCTRL:连接主站失败,睡眠"<<FLOWCTRL.GetSocketConnectInterSecond()<<"秒......"<<endl;
			for (INT32U Second = 0; Second < FLOWCTRL.GetSocketConnectInterSecond(); Second++)
				HeartBeat(), SleepSecond(1);
		}
		if (FLOWCTRL.GetSocketConnectFailedTimes() >= FLOWCTRL.GetSocketConnectRetryTimes())
		{
			logwt<<"连接失败次数="<<FLOWCTRL.GetSocketConnectFailedTimes()<<",通知PPP重新拨号......"<<endl;
			m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
			FLOWCTRL.SocketConnectRecord(true);
			SleepSecond(5);//make sure PPP redial
		}
	}
	else
	{
		for (int i = 0; i < CONNECT_RETRY_TIMES_MAX;)
		{
			logit<<"第"<<i+1<<"次连接......"<<endl;
			int ok = m_pMStationConnect->Connect();
			if (ok == 0)
			{
				return 0;
			}
			else if (ok > 0)
			{
				logit<<"未连接,距离连接时间还有"<<ok<<"秒..."<<endl;
				for (int j = 0; j < ok; j++)
					HeartBeat(), SleepSecond(1);
			}
			else
			{
				i++;
				logit<<"连接失败..."<<endl;
			}
		}
		if (m_pMStationConnect->IsConnected() == false)
		{
			if (m_pChannel->NeedPPP() == true)
			{
				logwt<<"连接失败次数="<<CONNECT_RETRY_TIMES_MAX<<",通知PPP重新拨号......"<<endl;
				m_pConnectStatus->m_HttxNeedPPP[m_HttxSN] = false;
				SleepSecond(5);//make sure PPP redial
			}
		}
	}
	return -1;
}

int C_HttxServer::MStationSend(const string &strCmd)
{
	if (m_pHttxStatus->m_fForwardEnable == false)//禁止发送主站命令
		return 0;//仅从终端的HTTX1有可能为false

	int Cnt = m_pMStationConnect->SendData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":发送命令:"<<HEX<<strCmd<<"时连接出错!!!"<<endl;
		m_pMStationConnect->Disconnect();
	}
	else if (Cnt > 0)
	{
		if (m_pHttxStatus->m_fMasterTerm == false && m_HttxSN == MP_HTTX_1)
			m_pHttxStatus->m_fForwardEnable = false;//从终端的HTTX1发送完一次置false,等级联控制帧
		m_pConnectStatus->m_CommunicationTime = C_TIME::TimeNow();
	}

	return Cnt;
}

int C_HttxServer::MStationRecv(string &strCmd)
{
	int Cnt = m_pMStationConnect->RecvData(strCmd);
	if (Cnt < 0)
	{
		logwt<<m_pChannel->m_Name<<":接收命令时连接出错!!!"<<endl;
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

