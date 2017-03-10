#include "dspdevice.h"
#include "basealgo.h"
S_CHANNEL* C_DspCmdChannel::m_pChannel=NULL;
C_CONNECT * C_DspCmdChannel::m_pConnect=NULL;
/*
brief: DSPͨѶ��ʼ��
param [in]void
return void
author:Steven Wan
date: 2015-04-23
*/ 

void C_DspCmdChannel::Init(void)
{
	S_DnChannel* pDnChannel  = STATUS.GetDnChannelInfo();
	if (pDnChannel == NULL)
	{
		loget<<"�޷���ȡ����ͨ����Ϣ"<<endl;
		return;
	}
	
	if (m_pConnect == NULL)
	{
		m_pChannel = pDnChannel->FindChannel(DBCJ_CHANNL_485I);
		if (m_pChannel == NULL)
		{
			loget<<"RS485_1ͨ��δ�ҵ�"<<endl;
			return;
		}
		if ((m_pConnect = C_ConnFact::CreateConnection(*m_pChannel)) == NULL)
		{
			loget<<"����ͨ��("<<m_pChannel->m_Name<<")ʧ��!!!"<<endl;
		}
		else
		{
			if (m_pConnect->IsConnected() == false)
			{
				if (m_pConnect->Connect() == -1)
				{
					loget<<"dsp ͨѶ������ʧ��"<<endl;
				}
				else
				{
					loget<<"dsp ͨѶ�����ӳɹ�"<<endl;
				}	
			}
			else
			{
				logvt<<"dsp ͨѶ��������"<<endl;
			}
		}
	}
}

/*
brief: ��DSPģ�鷢������
param [in]const string& iStrData:�������ݴ�
return int: >=0,�����ֽ���.-1,����ʧ�ܡ�
author:Steven Wan
date: 2015-04-23
*/ 
int C_DspCmdChannel::SendData(const string& iStrData)
{
	return m_pConnect->SendData(iStrData);
}
/*
brief: ����DSPģ�鷢������
param [out] string& oStrData:���յ����ݴ�
return int: >=0,�����ֽ���.-1,����ʧ�ܡ�
author:Steven Wan
date: 2015-04-23
*/ 
int C_DspCmdChannel::RecvData(string& oStrData)
{
	return m_pConnect->RecvData(oStrData);
}
/*
brief: ��DSPģ��д�������
param [in]di:���ݱ�ʶ
param [in]const string& iStrData:д�������ݴ�
param [out]string& oStrData:Dsp�����������ݴ�
return bool: true,д�ɹ�.false,дʧ�ܡ�
author:Steven Wan
date: 2015-04-23
*/ 
bool C_DspDevice::Write(const string& iStrData,string& oStrData)
{
#ifndef WIN32
	string retStrData;
	int cnt;
	C_DspCmdChannel::SendData(iStrData);
	cnt = C_DspCmdChannel::RecvData(retStrData);
	if (cnt <= 0)
	{
		loget<<"DSP ����ͨ���޽��շ�������"<<endl;
		return false;
	}

	if (FramePrase(retStrData) == true)
	{
		oStrData = retStrData;
		return true;
	}
	else
	{
		return false;
	}
#else
	oStrData.clear();
	oStrData.push_back((char)0xeb);
	oStrData.push_back((char)0x90);
	oStrData.push_back((char)0x84);
	oStrData.push_back((char)0x00);
	oStrData.push_back((char)0x00);
	oStrData.push_back((char)0xff);
	oStrData.push_back((char)0x16);
#endif	
	return true;
}

/*
brief: ��DSPģ��д�������
param [in]di:���ݱ�ʶ
param [in]S_CHAR *pData:���ݱ�ʶ����
param [in]S_WORD len:���ݱ�ʶ���ݳ���
return bool: true,д�ɹ�.false,дʧ�ܡ�
author:Steven Wan
date: 2015-04-23
*/ 
bool C_DspDevice::Write(S_WORD di,S_CHAR *pData,S_WORD len)
{
#ifndef WIN32
	string oFrame;
	string retStrData;
	int cnt;

	MakeFrame(CTL_WRITE,di,pData,len,oFrame);
	
	C_DspCmdChannel::SendData(oFrame);
	cnt = C_DspCmdChannel::RecvData(retStrData);
	if (cnt <= 0)
	{
		loget<<"DSP ����ͨ���޽��շ�������"<<endl;
		return false;
	}

	if (FramePrase(retStrData) == true)
	{
		if (retStrData.at(2) == CTL_WRITE_ACK)//DSP�ظ�дȷ��
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
#else
	return true;
#endif	
}
/*
brief: ��Լ��֡
param [in]S_CHAR ctlcode:������
param [in]S_WORD di:���ݱ�ʶ
param [in]S_CHAR *pData:������ָ��
param [out]S_WORD len:���������ݳ���
param [out]string& oFrame:��֡����
return 0:��֡�ɹ���-1:��֡ʧ��
author:Steven Wan
date: 2015-04-23
*/ 
S_WORD C_DspDevice::MakeFrame(S_CHAR ctlcode,S_WORD di,S_CHAR *pData,S_WORD len,string& oFrame)
{
	string oPackage;

	oPackage.clear();
	oPackage.push_back((char)0xEB);
	oPackage.push_back((char)0x90);

	oPackage.push_back(ctlcode);
       int datalen = len+2;
	oPackage.push_back((char)(datalen&0xff));
	oPackage.push_back((char)((datalen>>8)&0xff));

	oPackage.push_back((char)(di&0xff));
	oPackage.push_back((char)((di>>8)&0xff));

	for (S_WORD i=0; i<len; i++)
	{
		oPackage.push_back(pData[i]);
	}

	INT8U checkSum=0;
	for (S_WORD i=0;i<oPackage.size();i++)
	{
		checkSum+=oPackage[i];
	}

	oPackage.push_back(checkSum);
	oPackage.push_back((char)0x16);
	oFrame = oPackage;
	return 0;
}

/*
brief: Э��֡����
param [in]iStrData:������֡����
return bool: true,�����ɹ�.false,����ʧ�ܡ�
author:Steven Wan
date: 2015-04-23
*/ 
bool C_DspDevice::FramePrase(const string& iStrData)
{
	if (iStrData.empty())
	{
		return false;
	}
	string::const_iterator iter = find(iStrData.begin(), iStrData.end(), (char)HEAD_GUIDE_FLAG_FIRST);

	if (distance(iter, iStrData.end()) < FRAME_LEN_MIN)
	{
		return false;
	}

	S_BYTE Start1 = (S_BYTE)*iter; 
	S_BYTE Start2 = (S_BYTE)*(iter+1);

	if (Start1 != HEAD_GUIDE_FLAG_FIRST || Start2 != HEAD_GUIDE_FLAG_SECOND)
	{
		loget<<"ǰ��������"<<endl;
		return false;
	}

	S_WORD len = (S_WORD)(*(iter + 3) |(*(iter + 4)<<8));

	string::const_iterator end_iter = iter + 6 + len; 

	if ( (S_BYTE)(*(end_iter)) != (S_BYTE)END_FLAG)
	{
		loget<<"��֡������"<<endl;
		return false;
	}

	return true;

}
/*
brief: ��ʼ��DSP����
param [in]None
return None
author:Steven Wan
date: 2015-04-23
*/ 
void C_DspDevice::InitialParam(void)
{

	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	string regdata;

	logvt<<"DSP������ʼ��"<<endl;

	for (int i=0; i < pRegItemsArea->GetItemNum(); i++)
	{
		pRegItem  = pRegItemsArea->GetDataItemByIndex(i);
		if (pRegItem->m_bDataBlock == true)
		{
			continue;
		}

		if (pRegItem->IsValidData() == true)
		{
			if (Write(pRegItem->m_regid,pRegItem->m_data,pRegItem->m_Len) == false)
			{
				loget<<"������"<<HEX<<pRegItem->m_regid<<":��ʼ��DSPʧ��!"<<endl;
			}
			else
			{
				regdata.clear();
				regdata.append(pRegItem->m_data,pRegItem->m_Len);
				logvt<<"������"<<HEX<<pRegItem->m_regid<<",data:["<<regdata<<"]:��ʼ���ɹ�!"<<endl;
			}
		}
		else
		{
			loget<<"������"<<HEX<<pRegItem->m_regid<<":������Ч!"<<endl;
		}
	}
	return;
}

/*
brief: DSP����
param [in]None
return None
author:Steven Wan
date: 2015-04-23
*/ 
bool C_DspDevice::StartUp()
{
//	C_DspCmdChannel::Init();
	InitialParam();
	return true;
}

