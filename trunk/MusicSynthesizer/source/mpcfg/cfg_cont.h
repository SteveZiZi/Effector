/** @file
 *  @brief 系统上下文结构定义类
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  1、本文件是配置文件，需要用户定义
 *  2、系统类的定义在前面，用户不需要改动。用户类的定义紧跟其后
 *  3、共享内存的使用是受控的，即用户不得随意分配过大的空间
 */
#ifndef __MP_CFG_CONTEXT_H__
#define __MP_CFG_CONTEXT_H__
#include "traceset.h"
#include "sharemem.h"
#include "cfg_proc.h"
#include "server.h"
#include "basetype.h"
#include "sysbase.h"


/** @class
 *  @brief 系统上下文定义
 *  @remarks 本类只定义属性不定义方法
 */
struct S_CONTEXT
{
	struct S_MEMSIGN m_MemSign;///< 系统上下文标识
	struct S_SERVINFO m_ServerInfo[MP_PROCESS_CNT];///<进程信息数组
	struct S_TraceSet m_TraceSet[MP_PROCESS_CNT];///<跟踪控制信息数组
	struct S_SystemInfo m_SystemInfo;///<系统信息

//user define follow me
	struct S_ConnectStatus m_ConnectStatus; ///<主站通讯状态
	struct S_HttxStatus m_HttxStatus[HTTX_PROCESS_MAX]; ///< 后台通讯状态
	struct S_PppStatus m_PppStatus; ///< PPP状态
};

#endif//__MP_CFG_CONTEXT_H__

