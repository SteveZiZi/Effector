/** @file
 *  @brief �����ػ����̷�����չ
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
 *  @brief �����ػ����̷�����
 */
class C_NwDaemonServer:public C_DaemonServer
{
public:
	C_NwDaemonServer(void);
	virtual ~C_NwDaemonServer();

	/** @brief ���������ڷ������ */
	virtual int BeforeService(void);
};

#endif//__NW_DAEMON_H__


