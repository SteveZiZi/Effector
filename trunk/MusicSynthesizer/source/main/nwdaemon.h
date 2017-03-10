/** @file
 *  @brief 南网守护进程服务扩展
 *  @author Steven Wan
 *  @date 2015/01/07
 *  @version 0.1
 *  @see mprocess/daemon.h
 */
#ifndef __NW_DAEMON_H__
#define __NW_DAEMON_H__
#include "daemon.h"
#include "batteryb.h"


/** @class
 *  @brief 南网守护进程服务类
 */
class C_NwDaemonServer:public C_DaemonServer
{
public:
	C_NwDaemonServer(void);
	virtual ~C_NwDaemonServer();

	/** @brief 本函数先于服务调用 */
	virtual int BeforeService(void);
};

#endif//__NW_DAEMON_H__


