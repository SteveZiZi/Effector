/**
 *  @file
 *  @brief PPP���Ż�����
 *  @author Steven Wan
 *  @date 2014-01-05
 *  @version v1.0
 *  @note
 *  PPP���Ż���ҵ��⣬������PPP���ŷ�����Ҫ��д���ƹ���
 *  ���ھ���Ӧ��������MODEMͨ��, PPP������Ϣ(S_PppInfo)�������Ϣ
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

	/** @brief ��������ǰ����
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int BeforeService(void);
	/** @brief ������������
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
	 */
	virtual int AfterService(void){return 0;}
	/** @brief ���ڷ���
	 *  @return 0 �ɹ���-1 ʧ��
	 *  @remarks Ӧ�ó����������
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
	virtual int OnPPPOffLine(void);		//����ʧ�ܺ���ã�ͳ�Ʋ���ʧ�ܴ���
	virtual int OnPPPOnLine(void);	//���ųɹ�����ã���λ����ʧ�ܴ���������
	virtual bool NeedPpp(void);
	virtual bool NeedOnLine(void);		//����ǰ���ã��ж��Ƿ��������
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
	INT64U m_lastRecvPackages;		//�ϴν��յİ����������˴β���
	INT64U m_lastSendPackages;		//�ϴη��͵İ����������˴β���
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

