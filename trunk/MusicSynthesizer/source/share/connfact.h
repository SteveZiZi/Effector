/** @file
 *  @brief 连接工厂定义
 *  @author Steven Wan
 *  @date date 2015/01/06
 *  @version 0.1
 */
#ifndef __NW_CONNECT_FACTORY_H__
#define __NW_CONNECT_FACTORY_H__
#include "connect.h"

class C_ConnFact
{
public:
	static C_CONNECT* CreateConnection(S_CHANNEL& channel);
	static void DestroyConnection(C_CONNECT *pConnect);
};

class C_IP
{
public:
	static S_DWORD GetNetDeviceIP(const char *pNetDev);
	static S_DWORD GetSimIP(void){return GetNetDeviceIP(NET_PPP0_DEV_NAME);}
	static S_DWORD GetEthIP(void){return GetNetDeviceIP(NET_ETH0_DEV_NAME);}
};

#endif//__NW_CONNECT_FACTORY_H__

