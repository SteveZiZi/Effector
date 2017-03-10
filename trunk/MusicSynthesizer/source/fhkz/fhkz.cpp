/** @file
 *  @brief 负荷控制功能定义
 *  @author 万青松
 *  @date 2012/03/01
 *  @version 0.1
 */
#include "fhkz.h"

int C_FhkzServer::BeforeService(void)
{
	return 0;
}

int C_FhkzServer::AfterService(void)
{
	return 0;
}

int C_FhkzServer::OnTickService(void)
{
	HeartBeat();
	SleepMS(50);
	return 0;
}



