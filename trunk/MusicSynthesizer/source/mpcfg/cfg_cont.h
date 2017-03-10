/** @file
 *  @brief ϵͳ�����Ľṹ������
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  1�����ļ��������ļ�����Ҫ�û�����
 *  2��ϵͳ��Ķ�����ǰ�棬�û�����Ҫ�Ķ����û���Ķ���������
 *  3�������ڴ��ʹ�����ܿصģ����û���������������Ŀռ�
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
 *  @brief ϵͳ�����Ķ���
 *  @remarks ����ֻ�������Բ����巽��
 */
struct S_CONTEXT
{
	struct S_MEMSIGN m_MemSign;///< ϵͳ�����ı�ʶ
	struct S_SERVINFO m_ServerInfo[MP_PROCESS_CNT];///<������Ϣ����
	struct S_TraceSet m_TraceSet[MP_PROCESS_CNT];///<���ٿ�����Ϣ����
	struct S_SystemInfo m_SystemInfo;///<ϵͳ��Ϣ

//user define follow me
	struct S_ConnectStatus m_ConnectStatus; ///<��վͨѶ״̬
	struct S_HttxStatus m_HttxStatus[HTTX_PROCESS_MAX]; ///< ��̨ͨѶ״̬
	struct S_PppStatus m_PppStatus; ///< PPP״̬
};

#endif//__MP_CFG_CONTEXT_H__

