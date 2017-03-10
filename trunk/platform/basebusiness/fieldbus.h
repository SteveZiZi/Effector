/**
 *  @file
 *  @brief 定义现场总线抽象接口
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_FIELDBUS_H__
#define __BASEB_FIELDBUS_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "connect.h"
#include "strings.h"
#include "cmdfilt.h"

/**
 *  @class C_FieldBus
 *  @brief 现场总线控制器
 *  @author Steven Wan
 *  @note
 *  1、现场总线抽象接口；
 *  2、现场总线父类，所有的现场总线都须继承该类，如485总线，载波总线，微功率无线总线；
 *  3、定义了现场总线必须提供的功能。
 */
class C_FieldBus
{
public:
	/** @brief 现场总线的状态集 */
	enum{FB_STATUS_NORMAL,FB_STATUS_REPORT_DATA,FB_STATUS_REPORT_METER,FB_STATUS_ROUTER};
public:
	C_FieldBus(void);
	virtual ~C_FieldBus();

public:
	const char *GetName(void);
	C_CONNECT *GetConnection(void);
	C_CONNECT *SetConnection(C_CONNECT *pConnection);
	C_CmdFilter *SetCommandFilter(C_CmdFilter *pCmdFilter);
	
	/** @brief 现场总线硬复位（操作复位引脚） 
	 *  @return 0:成功; -1:失败
	 */
	virtual int Reset(void);
	/** @brief 更改连接属性 
	 *  @return 0:成功; -1:失败
	 */
	virtual int ChangeConnectionProperty(const S_COMSET &ComSet);
	/** @brief 现场总线状态同步，刚上电或进程重启需要状态同步 
	 *  @param[in] strModuleAddr 模块地址，如没有，则不同步地址
	 *  @param[out] strFactory 输出模块工厂代码和版本号
	 *  @return 0:成功; -1:失败
	 */
	virtual int SyncStatus(const string &strModuleAddr, string &strFactory);
	/** @brief 现场总线路由启动、查询函数 
	 *  @param[in] timeKeeping 路由时间，0:退出路由;0xFFFFFFFF:查询路由状态;其它：进入路由
	 *  @param[in] Meter485 采集器下485表地址
	 *  @param[in] MeterZB 载波表地址
	 *  @param[in] Relay 采集器地址
	 *  @return <0:失败 0:成功，路由结束 >0:剩余路由时间,单位秒
	 */
	virtual int Router(S_TIME timeKeeping, const S_TERMINALS &Meter485, const S_TERMINALS &MeterZB, const S_TERMINALS &Relay);
	/** @brief 现场总线的数据主动上报函数 
	 *  @param[out] CtlCmd 控制命令
	 *  @param[out] DataGram 命令参数
	 *  @param[in] timeKeeping 上报时间;0:退出上报;0xFFFFFFFF:查询;其它:进入上报
	 *  @return <0:出错 0:结束上报 >0:上报剩余时间
	 */
	virtual int ReportData(int &CtlCmd, S_DATAGRAM &DataGram, S_TIME timeKeeping);
	/** @brief 现场总线的新增表上报函数 
	 *  @param[out] strMeterAddr 表计地址
	 *  @param[in] timeKeeping 上报持续时间
	 *  @return -1:上报出错 0:上报结束 >0:剩余上报时间
	 */
	virtual int ReportMeter(string &strMeterAddr, S_TIME timeKeeping);
	/** @brief 现场总线的数据抄读函数 
	 *  @param[in] strRelay 中继地址+目标地址（目标地址在最后，每个地址占6字节）
	 *  @param[in] DataGram 命令参数
	 *  @param[out] strData 输出数据
	 *  @return <0:出错 >=0:成功
	 */
	virtual int RequestData(const string &strRelay, const S_DATAGRAM &DataGram, string &strData);
	/** @brief 广播命令 
	 *  @param[in] strRelay 中继地址+目标地址（目标地址在最后，每个地址占6字节）
	 *  @param[in] DataGram 命令参数
	 *  @return <0:出错 >=0:成功
	 */
	virtual int Broadcast(const string &strRelay, const S_DATAGRAM &DataGram);
	/** @brief 现场总线的命令操作函数 
	 *  @param[in] CmdID 命令ID
	 *  @param[in] strRelay 中继地址+目标地址（目标地址在最后，每个地址占6字节）
	 *  @param[in] DataGram 命令参数
	 *  @param[out] strData 输出数据
	 *  @return <0:出错 >=0:成功
	 */
	virtual int RequestCmd(int CmdID, const string &strRelay, const S_DATAGRAM &DataGram, string &strData);

protected:
	int ClearConnectionBuffer(void);
	int SendCmd(const string &strCmd);
	int RecvCmd(string &strCmd);

protected:
	const char *m_pFieldBusName;///< 现场总线控制器名字
	C_CONNECT *m_pConnection;///< 现场总线控制器串口连接
	C_CmdFilter *m_pCmdFilter;
};

#endif//__BASEB_FIELDBUS_H__

