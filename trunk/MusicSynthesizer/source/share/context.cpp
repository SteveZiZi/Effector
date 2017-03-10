/** @file
 *  @brief 用户层系统上下文配置类
 *  @author Steven Wan
 *  @datedate 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_ContextBase类，注意其中的抽象方法的重载。
 */
#include "context.h"
#include "ini_sys.h"





C_CONTEXT &C_CONTEXT::Instance(void)
{
	static C_CONTEXT s_MyContext;
	return s_MyContext;
}

/** @brief 设置成员信息等
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
 */
bool C_CONTEXT::SetInformation(void)
{
	if (C_ContextBase::SetInformation() == false)
		return false;
	m_pMyContext = (S_CONTEXT*)m_pContext;
	return true;
}

/** @brief 初始化系统上下文
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::InitContext(void)
{
	if (C_ContextBase::InitContext() != true)
		return false;

	m_pMyContext->m_ConnectStatus.Init();
//	m_pMyContext->m_PowerStatus.Init();

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
		m_pMyContext->m_HttxStatus[i].Init();

	m_pMyContext->m_PppStatus.Init(); ///< PPP状态

	return true;
}

/** @brief 加载配置到系统上下文
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::LoadContext(void)
{
	if (C_ContextBase::LoadContext() != true)
		return false;

	return true;
}

/** @brief 参数分析
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_CONTEXT::ParamParse(void)
{
	if (C_ContextBase::ParamParse() != true)
		return false;

	return true;
}
/** @brief 设置LCD指示器
 *  @param[in] pProcessName 进程名
 *  @param[in] Indicator 参见LCD_INDICATOR_
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
		"MODEM正在上电...",
		"检查MODEM...    ",
		"检查无线信号... ",
		"PPP正在拨号...  ",
		"PPP连接成功...  ",
		"PPP连接失败     "
	};
	static const char *LcdBottomInfoDbcj[] = 
	{
		"抄表结束        ",
		"正在抄表中...   ",
		"正在路由中...   ",
		"正在表号上报中..",
		"响应实时命令... ",
		"正在识别下行模块",
		"未知的下行模块  ",
		"下行模块识别成功"
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



