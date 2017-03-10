/** @file
 *  @brief PPP拨号服务功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#include "ppp.h"
#include "sysipc.h"
#include "htrace.h"
#include "context.h"
#include "status.h"
#include "connfact.h"
#include "timeop.h"


int C_PppServer::BeforeService(void)
{
	return 0;
}


