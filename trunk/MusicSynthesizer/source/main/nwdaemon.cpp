/** @file
 *  @brief �����ػ����̷�����չ
 *  @author Steven Wan
 *  @date  2015/01/06
 *  @version 0.1
 *  @see mprocess/daemon.h
 */
#include "nwdaemon.h"
#include "context.h"
#include "baseio.h"
#include "htrace.h"
#include "status.h"

//
C_NwDaemonServer::C_NwDaemonServer(void)
{
}

C_NwDaemonServer::~C_NwDaemonServer()
{
}

/** @brief ���������ڷ������ */
int C_NwDaemonServer::BeforeService(void)
{
	if (C_DaemonServer::BeforeService() != 0)
		return -1;
	return 0;
}

