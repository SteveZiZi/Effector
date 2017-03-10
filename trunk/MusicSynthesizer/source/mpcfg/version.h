/** @file
 *  @brief ��Ʒ�汾����
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#ifndef __NW_VERSION_H__
#define __NW_VERSION_H__

/** @brief ������汾�ż��������ں궨��
 *  @note
 *  ���¸��汾�ŵ�������󲻵ó��� 9
 */
#define CONS_MAJOR_VERSION	"0"						//���汾��
#define CONS_MINOR_VERSION	"0"						//�ΰ汾��
#define CONS_FIX_VERSION	"3"						//�����汾��
#define CONS_SW_REAL_DATE	"2012-06-15"		//�������ڣ���ʽ: YYYY-MM-DD

#define CONS_MAIN_VERSION CONS_MAJOR_VERSION"."CONS_MINOR_VERSION CONS_FIX_VERSION

/** @brief ��Լ�汾�ţ�����Լ�涨Ϊ0100
 */
#define CONS_MSRULE_VER  0x0100

/** @brief Ӳ���汾�� */
#define CONS_HARD_VERSION	"0.01"
#define CONS_HW_REAL_DATE	"2010-08-25"		//�������ڣ���ʽ: YYYY-MM-DD

/** @brief �û��汾��
 */
#define CONS_SOFT_VERSION CONS_MAIN_VERSION
#define CONS_SW_RELEASE_DATE CONS_SW_REAL_DATE

#endif//__NW_VERSION_H__


