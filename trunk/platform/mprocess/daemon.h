/** @file
 *  @brief �ػ����̷���
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  @note
 *  1��ι���Ź������ϵͳ
 *  2�������������ӹ��ܽ���
 */
#ifndef __MP_DAEMON_H__
#define __MP_DAEMON_H__
#include "basedef.h"
#include "server.h"

/** @class
 *  @brief �ػ����̷�����
 */
class C_DaemonServer:public C_SERVER
{
public:
	C_DaemonServer(void);
	~C_DaemonServer();

	/** @brief ���������ڷ������ */
	virtual int BeforeService(void);
	/** @brief ����ִ�к��� */
	virtual int OnTickService(void);	

protected:
	/** @brief Alarm�жϺ��� */
	virtual int OnSigAlrm(void);
	/** @brief TERM�ź��жϺ��� */
	virtual int OnSigTerm(void);
	/** @brief ȡALARM�������Ϊ0����ALARM������ */
	virtual S_DWORD GetAlarmInterUS(void){return 1000000;}
	/** @brief ���5������ */
	virtual int CheckTask5S(void);
	/** @brief ���2������ */
	virtual int CheckTask2S(void);

	/** @brief ���״̬ */
	bool CheckStatus(void);
	/** @brief ������״̬ */
	bool ScanServers(void);
	/** @brief ɱ�����з��� */
	bool KillServers(void);
	/** @brief �������з��� */
	bool SuspendServers(bool bSyspend);
	/** @brief ����ָ������ */
	bool StartServer(S_SERVINFO &ServInfo);
	/** @brief ɱ��ָ������ */
	bool KillServer(S_SERVINFO &ServInfo);
	/** @brief ���շ��� */
	bool WaitServer(PID_T pid=-1);
	/** @brief ι���Ź� */
	bool WatchDog();

	/** @brief ִ���˳��ű� */
	int ExecStopScript();

private:
	/** @brief ι���Ź� */
	bool doWatchDog(void);
	/** @brief ��鸴λ��Ϣ */
	bool CheckResetInfo(void);
	/** @brief ���������Ϣ */
	bool CheckUpdate(void);

private:
	int m_ProcessCnt; ///< ��������
	bool m_StopWatchDog; ///< ���Ź���־
	S_SystemInfo *m_pSysInfo; ///< ϵͳ��Ϣָ��
};

#endif//__MP_DAEMON_H__

