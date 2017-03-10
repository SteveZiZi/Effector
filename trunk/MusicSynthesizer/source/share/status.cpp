/** @file
 *  @brief 用户层系统状态配置类
 *  @author Steven Wan
 *  @date 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_StatusBase类，注意其中的抽象方法的重载。
 */
#include "status.h"
#include "ini_sys.h"
#include "ini_serv.h"
#include "timeop.h"
#include "regitem.h"
#include "parammode.h"

#include"Arithmetic.h"

C_STATUS &C_STATUS::Instance(void)
{
	static C_STATUS s_MyStatus;
	return s_MyStatus;
}

/** @brief 设置成员信息等
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
 */
bool C_STATUS::SetInformation(void)
{
	if (C_StatusBase::SetInformation() == false)
		return false;
	m_pMyStatus = (S_STATUS *)m_pStatus;
	return true;
}

/** @brief 初始化系统状态
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_STATUS::InitStatus(void)
{
	if (C_StatusBase::InitStatus() != true)
		return false;


	m_pMyStatus->m_SysConfig.Init();
//	m_pMyStatus->m_SysRun.Init();
	m_pMyStatus->m_IpCofig.Init();
	m_pMyStatus->m_UpChannels.Init(); ///< 上行通道信息
	m_pMyStatus->m_DnChannels.Init(); ///< 下行通道信息

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
	m_pMyStatus->m_HttxInfo[i].Init();
	m_pMyStatus->m_PppInfo.Init();
	m_pMyStatus->m_LcdInfo.Init();
	m_pMyStatus->m_MenuConfig.Init(); ///< 菜单页配置信息初始化

	/* 数据表示加载*/
	//m_pMyStatus->m_RegItems.Init();


	return true;
}

/** @brief 加载配置到系统状态
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_STATUS::LoadStatus(void)
{
	if (C_StatusBase::LoadStatus() != true)
		return false;
	C_IniSys::LoadRunInformation(m_pMyStatus);
	C_IniSys::LoadPortMap(m_pMyStatus);
	C_IniSys::LoadTermIPConfig(m_pMyStatus);
	C_IniServer::LoadUpChannl(m_pMyStatus);
	C_IniServer::LoadDnChannl(m_pMyStatus);
	C_IniServer::LoadHttxInfo(m_pMyStatus);
	C_IniServer::LoadPppInfo(m_pMyStatus);
	C_IniServer::LoadLcdInfo(m_pMyStatus);
	C_ParamMode::LoadTermMenuConfig(m_pMyStatus); // 加载模式配置信息
	C_RegitemCfg::LoadRegItems();


	return true;
}

/** @brief 参数分析
 *  @return true 成功；false 失败
 *  @remarks 子类重载时请先回调本函数
 */
bool C_STATUS::ParamParse(void)
{
	if (C_StatusBase::ParamParse() != true)
		return false;


	//定义负控硬件状态的重置
	return true;
}

S_CHANNEL *C_STATUS::GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN)
{
	if (HttxSN > HTTX_PROCESS_MAX || ChannelSN > HTTX_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_HttxInfo[HttxSN].m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetPppChannel(int ChannelSN)
{
	if (ChannelSN < 0 || ChannelSN > PPP_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_PppInfo.m_ChannlName[ChannelSN]);
}




