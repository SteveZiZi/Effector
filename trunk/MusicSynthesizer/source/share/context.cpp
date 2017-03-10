/** @file
 *  @brief �û���ϵͳ������������
 *  @author Steven Wan
 *  @datedate 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_ContextBase�࣬ע�����еĳ��󷽷������ء�
 */
#include "context.h"
#include "ini_sys.h"





C_CONTEXT &C_CONTEXT::Instance(void)
{
	static C_CONTEXT s_MyContext;
	return s_MyContext;
}

/** @brief ���ó�Ա��Ϣ��
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
 */
bool C_CONTEXT::SetInformation(void)
{
	if (C_ContextBase::SetInformation() == false)
		return false;
	m_pMyContext = (S_CONTEXT*)m_pContext;
	return true;
}

/** @brief ��ʼ��ϵͳ������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::InitContext(void)
{
	if (C_ContextBase::InitContext() != true)
		return false;

	m_pMyContext->m_ConnectStatus.Init();
//	m_pMyContext->m_PowerStatus.Init();

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
		m_pMyContext->m_HttxStatus[i].Init();

	m_pMyContext->m_PppStatus.Init(); ///< PPP״̬

	return true;
}

/** @brief �������õ�ϵͳ������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::LoadContext(void)
{
	if (C_ContextBase::LoadContext() != true)
		return false;

	return true;
}

/** @brief ��������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_CONTEXT::ParamParse(void)
{
	if (C_ContextBase::ParamParse() != true)
		return false;

	return true;
}
/** @brief ����LCDָʾ��
 *  @param[in] pProcessName ������
 *  @param[in] Indicator �μ�LCD_INDICATOR_
 */
bool C_CONTEXT::SetLcdBottomInfoIndicator(const char *pProcessName, S_BYTE Indicator)
{
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		if (Indicator >= LCD_INDICATOR_PPP_MAX)
			return false;
		m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator = Indicator;
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ) == 0)
	{
		if (Indicator >= LCD_INDICATOR_DBCJ_MAX)
			return false;
		//m_pMyContext-> TODO:
	}
	return true;
}

const char *C_CONTEXT::GetLcdBottomInfo(const char *pProcessName)
{
	static const char *LcdBottomInfoPpp[] = 
	{
		"MODEM�����ϵ�...",
		"���MODEM...    ",
		"��������ź�... ",
		"PPP���ڲ���...  ",
		"PPP���ӳɹ�...  ",
		"PPP����ʧ��     "
	};
	static const char *LcdBottomInfoDbcj[] = 
	{
		"�������        ",
		"���ڳ�����...   ",
		"����·����...   ",
		"���ڱ���ϱ���..",
		"��Ӧʵʱ����... ",
		"����ʶ������ģ��",
		"δ֪������ģ��  ",
		"����ģ��ʶ��ɹ�"
	};
	if (strcmp(pProcessName, MP_PROCESS_PPP) == 0)
	{
		return LcdBottomInfoPpp[m_pMyContext->m_PppStatus.m_LcdBottomInfoIndicator];
	}
	else if (strcmp(pProcessName, MP_PROCESS_DBCJ) == 0)
	{
		return LcdBottomInfoDbcj[0];
	}
	return NULL;
}



