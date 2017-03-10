/**
 *  @file
 *  @brief ��ƹ�Լ������
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MR_FACTORY_H__
#define __BASEB_MR_FACTORY_H__
#include "mrule.h"

/** @brief ��ƹ�Լ����ӳ�� */
struct S_RuleNameMap
{
	int m_RuleID;
	char m_RuleName[32];

public:
	S_RuleNameMap(void){memset(this, 0x00, sizeof(*this));}
};
struct S_RuleNameMaps:public vector<S_RuleNameMap>
{
};

class C_MRuleFact
{
public:
	/** @fn
	 *  @brief ��ʼ���������õı�ƹ�Լ
	 *  @return 0:�ɹ�; -1:ʧ��
	 *  @see /mnt/dyjc/registor/mrule.ini
	 *  @note ��mrule.ini�����ñ�ƹ�Լ
	 */
	static int InitMeterRules(void);
	/** @fn
	 *  @brief ȡ���Լ
	 *  @param[in] pRuleName ��Լ����
	 *  @return NULL:û����Ӧ�ı�ƹ�Լ; !NULL:�Ϸ���ƹ�Լ
	 */
	static C_MRULE *GetMeterRule(const char *pRuleName);
	/** @fn
	 *  @brief ȡ���Լ
	 *  @param[in] RuleID ����Ҫ���ԼID
	 *  @return NULL:û����Ӧ�ı�ƹ�Լ; !NULL:�Ϸ���ƹ�Լ
	 */
	static C_MRULE *GetMeterRule(int RuleID);
};

class C_CommReg
{
public:
	static int LoadCommonRegister(S_REGISTERS &Registers);
};

class C_MeterRuleMap
{
public:
	static int LoadMeterRuleMap(S_RuleNameMaps &NameMaps);
};

#endif //__BASEB_MR_FACTORY_H__

