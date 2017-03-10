/** @file
 *  @brief ����������ļ�
 *  @author Steven Wan
 *  @date 2015/03/15
 *  @version 0.1
 *  1�����̺�������
 *  2��ע�⣺�������̺�һ��Ҫ�޸ġ����������ꡱ�Ķ���
 */
#ifndef __MP_CFG_PROCESS_H__
#define __MP_CFG_PROCESS_H__

/** @brief ���̷������ƶ��� */
#define MP_PROCESS_HTTX1	"httx1"
#define MP_PROCESS_HTTX2	"httx2"
#define MP_PROCESS_HTTX3	"httx3"
#define MP_PROCESS_DBCJ		"dbcj"
#define MP_PROCESS_PPP		"ppp"
#define MP_PROCESS_LCD		"lcd"
#define MP_PROCESS_JLCY		"jlcy"
#define MP_PROCESS_FHKZ		"fhkz"

/** @brief ����������
 *  @remarks �������̺�һ��Ҫ�޸ı���Ķ��壬
 *  ����ֵ������gMP_ProcessAll�����н��̸�����ȣ����򽫳���PLEASE BE CAREFUL.
 */
#define MP_PROCESS_CNT	(1/*daemon*/+1/*httx1*/+1/*httx2*/+1/*httx3*/+1/*dbcj*/+1/*ppp*/+1/*lcd*/+1/*jlcy*/+1/*fhkz*/)


#endif//__MP_CFG_PROCESS_H__

