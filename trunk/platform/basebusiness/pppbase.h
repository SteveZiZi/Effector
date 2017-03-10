/**
 *  @file
 *  @brief PPP拨号基础类
 *  @author Steven Wan
 *  @date 2014-01-05
 *  @version v1.0
 *  @note
 *  PPP拨号基础业务库，各具体PPP拨号服务需要重写控制功能
 *  请在具体应用中设置MODEM通道, PPP拨号信息(S_PppInfo)等相关信息
 */
#ifndef __BASEB_PPP_BASE_H__
#define __BASEB_PPP_BASE_H__
#include "basedef.h"
#include "server.h"
#include "modem.h"
#include "basetype.h"

class C_PppBase:public C_SERVER
{
public:
	enum{PPP_REFRESH_SIGNAL_PERIOD = 300};
	enum{PPP_VPORT_GPRS = 11, PPP_VPORT_ATCMD = 12, PPP_VPORT_SMS = 13};
	enum{PPP_TRY_GET_SIGNAL_MAX = 12, PPP_CHECK_MODEM_MAX = 3, PPP_CHECK_MUXD_READY_MAX = 12};
	enum{PPP_STATUS_RESET, PPP_STATUS_INIT, PPP_STATUS_SIGNAL, PPP_STATUS_DIAL, PPP_STATUS_ONLINE, PPP_STATUS_OFFLINE};

public:
	C_PppBase(void);
	virtual ~C_PppBase();

	/** @brief 启动服务前调用
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int BeforeService(void);
	/** @brief 启动服务后调用
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int AfterService(void){return 0;}
	/** @brief 周期服务
	 *  @return 0 成功；-1 失败
	 *  @remarks 应用程序可以重载
	 */
	virtual int OnTickService(void);

protected:
	int ResetModem(void);
	bool PppInfoClear(void);
	int CheckModem(void);
	int CheckModemEx(void);
	int GetModemSimInfo(void);
	bool DetectSignal(void);

	int StartMuxd(void);
	int StopMuxd(void);
	int WaitMuxd(void);
	bool MuxdIsReady(void);

	void ClearRecvSendStat();
	bool DialSuccessfully(void);

	int OnLine();
	int OffLine(bool bForce = false);
	int WaitPPP();
	int PrintWaitInfo(char *pPrompt, PID_T pid, int status);

	int RefreshStatus(void);
	int RefreshSignal(void);
	int GetDevInfo(void);

	virtual bool PppEnable(void);
	virtual int CheckNotify(void);
	virtual int CheckModemType(void);
	virtual int AdaptModem(void);
	virtual int OnPPPOffLine(void);		//拨号失败后调用，统计拨号失败次数
	virtual int OnPPPOnLine(void);	//拨号成功后调用，复位拨号失败次数计数器
	virtual bool NeedPpp(void);
	virtual bool NeedOnLine(void);		//拨号前调用，判断是否进行连接
	virtual bool NeedOffLine(void);
	virtual bool InitSignalVal(void);
	virtual bool SetSignalVal(int signal);

	virtual int GetModemPowerOffDelayS(void);
	virtual int GetModemPowerOnDelayS(void);
	virtual S_WORD GetRefreshSignalInterS(void);

protected:
	bool m_OnLine;
	int m_PppStatus;
	int m_DetectSignalFailedCount;
	S_TIME m_LastOffLineTime;
	S_TIME m_LastOnlineTime;
	PID_T m_PidPPP;
	PID_T m_PidMuxd;
	int m_WirelessModemType;
	INT64U m_lastRecvPackages;		//上次接收的包总数，仅此次拨号
	INT64U m_lastSendPackages;		//上次发送的包总数，仅此次拨号
	string m_strModemFactory;
	string m_strModemModel;
	S_DialInfo *m_pDialInfo;
	S_PppPackage *m_pPppPackage;
	C_MODEM *m_pModem;
	C_CONNECT *m_pModemConnect;
	C_CONNECT *m_pGprsConnect;
	C_CONNECT *m_pAtCmdConnect;
	C_CONNECT *m_pSmsConnect;
};

#endif//__BASEB_PPP_BASE_H__

