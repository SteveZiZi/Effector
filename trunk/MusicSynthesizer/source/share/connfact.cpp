/** @file
 *  @brief 连接工厂定义
 *  @author Steven Wan
 *  @date 2015/01/06
 *  @version 0.1
 */
#include "connfact.h"
#include "htrace.h"
#include "conn_com.h"
#include "conn_net.h"
#include "sysbase.h"
#include "status.h"

/** 钩子函数 */
void Connect_ReadHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" recv:"<<HEX<<strData<<endl;
}
void Connect_WritHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" send:"<<HEX<<strData<<endl;
}
void Connect_ErrorOut(bool fWarnLevel, const string &strErrorInfo)
{
	if (!strErrorInfo.empty())
	{
		if (fWarnLevel == true) logwt<<strErrorInfo<<endl;
		else logit<<strErrorInfo<<endl;
	}
}

C_CONNECT* C_ConnFact::CreateConnection(S_CHANNEL& channel)
{
	S_SYSCFG *pSysCfg = NULL;
	C_COMCONN *pCom = NULL;
	C_NETCONN *pNet = NULL;
	switch(channel.m_Type)
	{
	case CHANNEL_TYPE_COM:
		pCom = new C_COMCONN(channel);
		pCom->SetReadHook(Connect_ReadHOOK);
		pCom->SetWritHook(Connect_WritHOOK);
		pCom->SetErrorHook(Connect_ErrorOut);
		pSysCfg = STATUS.GetSystemConfig();
		pCom->SetPortMap(pSysCfg->m_PortMap);
		return pCom;
	case CHANNEL_TYPE_NET:
		pNet = new C_NETCONN(channel);
		pNet->SetReadHook(Connect_ReadHOOK);
		pNet->SetWritHook(Connect_WritHOOK);
		pNet->SetErrorHook(Connect_ErrorOut);
		return pNet;
	}
	return NULL;
}

void C_ConnFact::DestroyConnection(C_CONNECT *pConnect)
{
	delete pConnect;
}

//
//
//
S_DWORD C_IP::GetNetDeviceIP(const char *pNetDev)
{
	S_CHANNEL channel;
	channel.m_Type = CHANNEL_TYPE_NET;
	C_CONNECT* pConnect = C_ConnFact::CreateConnection(channel);
	if (pConnect->GetProperty(pNetDev, &channel, sizeof(channel)) == 0)
	{
		S_DWORD ip;
		memcpy(&ip, channel.m_PortSet.m_NetSet.m_IP, 4);
		return ip;
	}
	return 0;
}



