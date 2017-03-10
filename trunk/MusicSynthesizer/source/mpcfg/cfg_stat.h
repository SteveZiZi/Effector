/** @file
 *  @brief 系统状态结构定义类
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  1、本文件是配置文件，需要用户定义
 *  2、系统类的定义在前面，用户不需要改动。用户类的定义紧跟其后
 *  3、共享内存的使用是受控的，即用户不得随意分配过大的空间
 */
#ifndef __MP_CFG_STATUS_H__
#define __MP_CFG_STATUS_H__
#include "sharemem.h"
#include "sysbase.h"


/** @class
 *  @brief 系统状态定义
 *  @remarks 本类只定义属性不定义方法
 */
struct S_STATUS
{
	struct S_MEMSIGN m_MemSign;///<系统状态标识

//config area
	struct S_SYSCFG m_SysConfig; ///< 系统配置
	struct S_SYSRUN m_SysRun; ///< 系统运行信息
	struct S_IpConfig m_IpCofig; ///< IP INFO
	struct S_MenuConfig m_MenuConfig; /// < Menu info

	struct S_UpChannel m_UpChannels; ///< 上行通道信息
	struct S_DnChannel m_DnChannels; ///< 下行通道信息
	struct S_HttxInfo m_HttxInfo[HTTX_PROCESS_MAX]; ///< HTTX信息
	struct S_PppInfo m_PppInfo; ///< PPP信息
	struct S_LcdInfo m_LcdInfo; ///< lcd配置信息
	struct S_RegItems m_RegItems;

	
	
};

#endif//__MP_CFG_STATUS_H__

