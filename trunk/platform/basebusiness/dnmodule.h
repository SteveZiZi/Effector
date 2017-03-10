/**
 *  @file
 *  @brief ����ģ�����
 *  @author Steven Wan
 *  @date 2014-05-31
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_DN_MODULE_H__
#define __BASEB_DN_MODULE_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "connect.h"

class C_DnModule
{
public:
	/** @fn
	 *  @brief ��ʼ���������õı�ƹ�Լ
	 *  @param[in] pRS485I ����RS485I����,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pRS485II ����RS485II����,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pZBWX �ز���������������,��ΪNULL,�򲻳�ʼ��
	 *  @param[in] pJLCY ����������������,��ΪNULL,�򲻳�ʼ��
	 *  @return 0:�ɹ�; <0:ʧ��
	 *  @note ���ӱ���Ϊ�Ǿֲ�����
	 */
	int InitFiledBus(C_CONNECT *pRS485I, C_CONNECT *pRS485II, C_CONNECT *pZBWX, C_CONNECT *pJLCY);

	/** @brief ����ģ�鸴λ
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	int Reset(C_CONNECT *pZBWX);
	/** @brief �ֳ�����״̬ͬ�������ϵ�����������Ҫ״̬ͬ�� 
	 *  @param[in] strAddress ģ���ַ����û�У���ͬ����ַ
	 *  @param[out] strFactory ���ģ�鹤������Ͱ汾��
	 *  @return 0:�ɹ�; -1:ʧ��
	 *  @note ��ͬ��ʧ�ܣ������C_DnModule::Reset()����ٽ����ֳ����߹�����ʼ��
	 *  @see fbfact.h
	 */
	int SyncStatus(const string &strAddress, string &strFactory);
	/** @brief �ֳ�����·����������ѯ���� 
	 *  @param[in] timeKeeping ·��ʱ�䣬0:�˳�·��;0xFFFFFFFF:��ѯ·��״̬;����������·��
	 *  @param[in] Meter485 �ɼ�����485���ַ
	 *  @param[in] MeterZB �ز����ַ
	 *  @param[in] Relay �ɼ�����ַ
	 *  @return <0:ʧ�� 0:�ɹ���·�ɽ��� >0:ʣ��·��ʱ��,��λ��
	 */
	int Router(S_TIME timeKeeping, const S_TERMINALS &Meter485, const S_TERMINALS &MeterZB, const S_TERMINALS &Relay);
};

#endif //__BASEB_DN_MODULE_H__

