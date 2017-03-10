/** @file
 *  @brief 后台通讯服务功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#ifndef __HTTX_SERVER_H__
#define __HTTX_SERVER_H__
#include "server.h"
#include "basetype.h"
#include "connect.h"
#include "sysbase.h"
#include "dspdevice.h"
#include "protocol.h"

class C_HttxServer:public C_SERVER
{
public:
	C_HttxServer(void);
	virtual ~C_HttxServer();

public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);

protected:
	int ChannelServer(void);

	int CmdProcess(void);
	int OffLineCheck(void);
	bool NeedOffLine(void);
	int WaitPppDial(void);
	int ConnectMasterStation(void);

	int MStationSend(const string &strCmd);
	int MStationRecv(string &strCmd);
	int MsCmdCount(void);
	int DataTransfer(void);

protected:
	S_TIME m_tickRecvMsData;
	S_HttxSN m_HttxSN;
	S_CHANNEL *m_pChannel;
	C_CONNECT *m_pMStationConnect;
	S_ConnectStatus *m_pConnectStatus;
	S_HttxStatus *m_pHttxStatus;

	string m_strRecvMsBuffer;
	strings m_strResponseCmds;
	C_DspDevice* m_pDspDevice;
	C_Protocol m_protocol;
};

class C_Httx1Server:public C_HttxServer
{
public:
	C_Httx1Server(void);
	virtual ~C_Httx1Server();
};

class C_Httx2Server:public C_HttxServer
{
public:
	C_Httx2Server(void);
	virtual ~C_Httx2Server();
};

class C_Httx3Server:public C_HttxServer
{
public:
	C_Httx3Server(void);
	virtual ~C_Httx3Server();
};

#endif//__HTTX_SERVER_H__

