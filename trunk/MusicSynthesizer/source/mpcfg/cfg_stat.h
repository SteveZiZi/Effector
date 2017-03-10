/** @file
 *  @brief ϵͳ״̬�ṹ������
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  1�����ļ��������ļ�����Ҫ�û�����
 *  2��ϵͳ��Ķ�����ǰ�棬�û�����Ҫ�Ķ����û���Ķ���������
 *  3�������ڴ��ʹ�����ܿصģ����û���������������Ŀռ�
 */
#ifndef __MP_CFG_STATUS_H__
#define __MP_CFG_STATUS_H__
#include "sharemem.h"
#include "sysbase.h"


/** @class
 *  @brief ϵͳ״̬����
 *  @remarks ����ֻ�������Բ����巽��
 */
struct S_STATUS
{
	struct S_MEMSIGN m_MemSign;///<ϵͳ״̬��ʶ

//config area
	struct S_SYSCFG m_SysConfig; ///< ϵͳ����
	struct S_SYSRUN m_SysRun; ///< ϵͳ������Ϣ
	struct S_IpConfig m_IpCofig; ///< IP INFO
	struct S_MenuConfig m_MenuConfig; /// < Menu info

	struct S_UpChannel m_UpChannels; ///< ����ͨ����Ϣ
	struct S_DnChannel m_DnChannels; ///< ����ͨ����Ϣ
	struct S_HttxInfo m_HttxInfo[HTTX_PROCESS_MAX]; ///< HTTX��Ϣ
	struct S_PppInfo m_PppInfo; ///< PPP��Ϣ
	struct S_LcdInfo m_LcdInfo; ///< lcd������Ϣ
	struct S_RegItems m_RegItems;

	
	
};

#endif//__MP_CFG_STATUS_H__

