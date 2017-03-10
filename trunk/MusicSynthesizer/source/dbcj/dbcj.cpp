/** @file
 *  @brief 表计能量采集功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#include "dbcj.h"
#include "sysipc.h"
#include "htrace.h"

int C_DbcjServer::BeforeService(void)
{
	return 0;
}

int C_DbcjServer::AfterService(void)
{
	return 0;
}

int C_DbcjServer::OnTickService(void)
{
	HeartBeat();

	return 0;
}



