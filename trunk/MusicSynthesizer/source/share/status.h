/** @file
 *  @brief 用户层系统状态配置类
 *  @author Steven Wan
 *  @date 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  注意需要继承C_StatusBase类，注意其中的抽象方法的重载。
 */
#ifndef __NW_STATUS_H__
#define __NW_STATUS_H__
#include "cfg_stat.h"
#include "sysipc.h"
#include "basetype.h"
#include "ini_serv.h"
#include "ini_sys.h"

class C_STATUS:public C_StatusBase
{
public:
	virtual ~C_STATUS(){}
	static C_STATUS &Instance(void);

	/** @brief 设置成员信息等
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数，共享内存创建成功时调用本函数
	 */
	virtual bool SetInformation(void);
	/** @brief 初始化系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool InitStatus(void);
	/** @brief 加载配置到系统状态
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool LoadStatus(void);
	/** @brief 参数分析
	 *  @return true 成功；false 失败
	 *  @remarks 子类重载时请先回调本函数
	 */
	virtual bool ParamParse(void);

public:
	/** @brief Get函数区*/
	S_SYSCFG *GetSystemConfig(void){return &m_pMyStatus->m_SysConfig;}
	S_SYSRUN *GetSysRunInfo(void){return &m_pMyStatus->m_SysRun;}
	S_CHANNEL *GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN);
	S_CHANNEL *GetPppChannel(int ChannelSN);
	S_HttxInfo *GetHttxInfo(S_HttxSN httxSN){return &m_pMyStatus->m_HttxInfo[httxSN];}
	S_PppInfo *GetPppInfo(void){return &m_pMyStatus->m_PppInfo;}
	S_IpConfig *GetIpConfig(void){return &m_pMyStatus->m_IpCofig;}
	S_LcdInfo *GetLcdInfo(void){return &m_pMyStatus->m_LcdInfo;}
	S_DnChannel *GetDnChannelInfo(void){return &m_pMyStatus->m_DnChannels;}
	
public:
	S_RegItems *GetRegitemsArea(void){return &m_pMyStatus->m_RegItems;}

public:
	/** @brief 参数保存区 */
	int SaveUpChannel(void){return C_IniServer::SaveUpChannl(m_pMyStatus);}
	int SaveHttxInfo(void){return C_IniServer::SaveHttxInfo(m_pMyStatus);}
	int SaveTermIPConfig(void){return C_IniSys::SaveTermIPConfig(m_pMyStatus);}
	int SavePppInfo(void){return C_IniServer::SavePppInfo(m_pMyStatus);}
	int SaveSysRunInfo(void){return C_IniSys::SaveRunInformation(m_pMyStatus);}
	

protected:
	C_STATUS(void){m_pMyStatus = NULL;}


protected:
	S_STATUS *m_pMyStatus;
};

#define STATUS C_STATUS::Instance()

#endif//__NW_STATUS_H__

