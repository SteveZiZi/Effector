/** @file
 *  @brief 守护进程服务
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  @note
 *  1、喂看门狗，监测系统
 *  2、启动及监测各子功能进程
 */
#ifndef __MP_DAEMON_H__
#define __MP_DAEMON_H__
#include "basedef.h"
#include "server.h"

/** @class
 *  @brief 守护进程服务类
 */
class C_DaemonServer:public C_SERVER
{
public:
	C_DaemonServer(void);
	~C_DaemonServer();

	/** @brief 本函数先于服务调用 */
	virtual int BeforeService(void);
	/** @brief 服务执行函数 */
	virtual int OnTickService(void);	

protected:
	/** @brief Alarm中断函数 */
	virtual int OnSigAlrm(void);
	/** @brief TERM信号中断函数 */
	virtual int OnSigTerm(void);
	/** @brief 取ALARM间隔，如为0，则ALARM不启动 */
	virtual S_DWORD GetAlarmInterUS(void){return 1000000;}
	/** @brief 检查5秒任务 */
	virtual int CheckTask5S(void);
	/** @brief 检查2秒任务 */
	virtual int CheckTask2S(void);

	/** @brief 检查状态 */
	bool CheckStatus(void);
	/** @brief 检查服务状态 */
	bool ScanServers(void);
	/** @brief 杀死所有服务 */
	bool KillServers(void);
	/** @brief 挂起所有服务 */
	bool SuspendServers(bool bSyspend);
	/** @brief 启动指定服务 */
	bool StartServer(S_SERVINFO &ServInfo);
	/** @brief 杀死指定服务 */
	bool KillServer(S_SERVINFO &ServInfo);
	/** @brief 回收服务 */
	bool WaitServer(PID_T pid=-1);
	/** @brief 喂看门狗 */
	bool WatchDog();

	/** @brief 执行退出脚本 */
	int ExecStopScript();

private:
	/** @brief 喂看门狗 */
	bool doWatchDog(void);
	/** @brief 检查复位信息 */
	bool CheckResetInfo(void);
	/** @brief 检查升级信息 */
	bool CheckUpdate(void);

private:
	int m_ProcessCnt; ///< 进程总数
	bool m_StopWatchDog; ///< 看门狗标志
	S_SystemInfo *m_pSysInfo; ///< 系统信息指针
};

#endif//__MP_DAEMON_H__

