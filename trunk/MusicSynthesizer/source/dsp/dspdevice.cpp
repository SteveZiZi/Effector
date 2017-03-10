#include "dspdevice.h"
#include "basealgo.h"
S_CHANNEL* C_DspCmdChannel::m_pChannel=NULL;
C_CONNECT * C_DspCmdChannel::m_pConnect=NULL;
/*
brief: DSP通讯初始化
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
		loget<<"无法获取下行通道信息"<<endl;
		return;
	}
	
	if (m_pConnect == NULL)
	{
		m_pChannel = pDnChannel->FindChannel(DBCJ_CHANNL_485I);
		if (m_pChannel == NULL)
		{
			loget<<"RS485_1通道未找到"<<endl;
			return;
		}
		if ((m_pConnect = C_ConnFact::CreateConnection(*m_pChannel)) == NULL)
		{
			loget<<"创建通道("<<m_pChannel->m_Name<<")失败!!!"<<endl;
		}
		else
		{
			if (m_pConnect->IsConnected() == false)
			{
				if (m_pConnect->Connect() == -1)
				{
					loget<<"dsp 通讯口连接失败"<<endl;
				}
				else
				{
					loget<<"dsp 通讯口连接成功"<<endl;
				}	
			}
			else
			{
				logvt<<"dsp 通讯口已连接"<<endl;
			}
		}
	}
}

/*
brief: 对DSP模块发送数据
param [in]const string& iStrData:发送数据串
return int: >=0,发送字节数.-1,发送失败。
author:Steven Wan
date: 2015-04-23
*/ 
int C_DspCmdChannel::SendData(const string& iStrData)
{
	return m_pConnect->SendData(iStrData);
}
/*
brief: 接收DSP模块发送数据
param [out] string& oStrData:接收的数据串
return int: >=0,接收字节数.-1,接收失败。
author:Steven Wan
date: 2015-04-23
*/ 
int C_DspCmdChannel::RecvData(string& oStrData)
{
	return m_pConnect->RecvData(oStrData);
}
/*
brief: 对DSP模块写命令操作
param [in]di:数据标识
param [in]const string& iStrData:写命令数据串
param [out]string& oStrData:Dsp返回命令数据串
return bool: true,写成功.false,写失败。
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
		loget<<"DSP 命令通道无接收返回命令"<<endl;
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
brief: 对DSP模块写命令操作
param [in]di:数据标识
param [in]S_CHAR *pData:数据标识内容
param [in]S_WORD len:数据标识内容长度
return bool: true,写成功.false,写失败。
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
		loget<<"DSP 命令通道无接收返回命令"<<endl;
		return false;
	}

	if (FramePrase(retStrData) == true)
	{
		if (retStrData.at(2) == CTL_WRITE_ACK)//DSP回复写确认
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
brief: 规约组帧
param [in]S_CHAR ctlcode:控制码
param [in]S_WORD di:数据标识
param [in]S_CHAR *pData:数据区指针
param [out]S_WORD len:数据区内容长度
param [out]string& oFrame:组帧命令
return 0:组帧成功，-1:组帧失败
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
brief: 协议帧解析
param [in]iStrData:带解析帧数据
return bool: true,解析成功.false,解析失败。
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
		loget<<"前导符出错"<<endl;
		return false;
	}

	S_WORD len = (S_WORD)(*(iter + 3) |(*(iter + 4)<<8));

	string::const_iterator end_iter = iter + 6 + len; 

	if ( (S_BYTE)(*(end_iter)) != (S_BYTE)END_FLAG)
	{
		loget<<"无帧结束符"<<endl;
		return false;
	}

	return true;

}
/*
brief: 初始化DSP参数
param [in]None
return None
author:Steven Wan
date: 2015-04-23
*/ 
void C_DspDevice::InitialParam(void)
{

	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	string regdata;

	logvt<<"DSP参数初始化"<<endl;

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
				loget<<"数据项"<<HEX<<pRegItem->m_regid<<":初始化DSP失败!"<<endl;
			}
			else
			{
				regdata.clear();
				regdata.append(pRegItem->m_data,pRegItem->m_Len);
				logvt<<"数据项"<<HEX<<pRegItem->m_regid<<",data:["<<regdata<<"]:初始化成功!"<<endl;
			}
		}
		else
		{
			loget<<"数据项"<<HEX<<pRegItem->m_regid<<":数据无效!"<<endl;
		}
	}
	return;
}

/*
brief: DSP启动
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

