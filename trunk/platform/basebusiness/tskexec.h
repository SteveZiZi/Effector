/**
 *  @file
 *  @brief 任务执行者
 *  @author Steven Wan
 *  @date 2014-06-19
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_TASK_EXEC_H__
#define __BASEB_TASK_EXEC_H__
#include "basetype.h"
#include "dnmodule.h"
#include "metercom.h"

enum
{
	TASK_EXEC_STATUS_CONTINUE,
	TASK_EXEC_STATUS_BLOCK,
	TASK_EXEC_STATUS_DESTROY,

	TASK_EXEC_STATUS_MAX
};

/** @class
 *  @brief 任务执行器
 *  @note 轻量级对象，不得有属性
 */
class C_TaskExec
{
public:
	C_TaskExec(void);
	virtual ~C_TaskExec();

public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务运行态前准备
	 *  @return 0:成功；>0:失败
	 */
	virtual int BeforeExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief 空闲任务
 */
class C_TaskIdle:public C_TaskExec
{
};

/** @class
 *  @brief 广播校表任务
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskBroadcast:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
protected:
	/** @brief 获取表读通信组件 */
	virtual C_MeterCom* GetMeterCom(void);
};
	
/** @class
 *  @brief 路由任务基类
 *  @note GetMeterAll需要重写；GetDnModule依具体情况决定是否重写
 */
class C_TaskRouteBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取下行模块管理组件 */
	virtual C_DnModule* GetDnModule(void);
	/** @brief 获取当前表计
	 *  @param[out] Meter485 采集器下的485表
	 *  @param[out] MeterZB 载波表或无线表
	 *  @param[out] Relay 采集器
	 *  @return 0:成功；>0:失败
	 */
	virtual int GetMeterAll(S_TERMINALS &Meter485, S_TERMINALS &MeterZB, S_TERMINALS &Relay) = 0;
	/** @brief 路由模块操作失败消息 */
	virtual int SendRouterErrorMessage(int ErrorInfo) = 0;
};

struct S_GatherInfo
{
	S_DWORD m_MP; ///< 测量点
	S_DWORD m_MSDI; ///< 主站数据标识
	S_MeterBase m_MeterBase; ///< 表计信息
	S_REGISTERS m_CommRegs; ///< 通用寄存器信息
	S_DIBS m_DisableReg;
	strings m_strDatas; ///< 采集数据信息
};
struct S_GatherInfoS:public vector<S_GatherInfo>
{
};

/** @class
 *  @brief 实时任务基类
 *  @note GetMeterGatherInfo和SaveMeterGatherData需要重写；
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskRealtimeBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_DESTROY
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取表读通信组件 */
	virtual C_MeterCom* GetMeterCom(void);
	/** @brief 获取采集信息
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(const S_TaskInfo *pTaskInfo, S_GatherInfoS &GatherInfos) = 0;
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] GatherInfos 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_GatherInfoS &GatherInfos) = 0;
};

/** @class
 *  @brief 轮询式采表基类
 *  @note GetMeterGatherInfo和HandleMeterGatherData需要重写；
 *  GetMeterCom依具体情况决定是否重写
 */
class C_TaskPollBase:public C_TaskExec
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务运行态前准备
	 *  @return 0:成功；<0:失败
	 */
	virtual int BeforeExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取表读通信组件 */
	virtual C_MeterCom* GetMeterCom(void);

	/** @brief 获取采集信息
	 *  @param[in] MP 测量点或表计序号
	 *  @param[out] MeterBase 表计信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int GetMeterGatherInfo(S_WORD idxMeter, S_MeterBase &MeterBase) = 0;
	/** @brief 处理采集数据
	 *  @param[in] pTaskInfo 任务信息
	 *  @param[out] pTaskCtrlStatus 采集信息
	 *  @return 0:成功; <0:失败
	 */
	virtual int HandleMeterGatherData(const S_TaskInfo *pTaskInfo, const S_TaskCtrlStatus *pTaskCtrlStatus) = 0;

	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int DirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int IndirectMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

	/** @brief 判断测量点是否需要采集
	 *  @return true:需要；false:不需要
	 */
	bool NeedGather(S_WORD idxMeter, const S_TaskInfo *pTaskInfo, const S_TaskFreezeStatus *pTaskFreezeStatus);

	bool InitNextMeterInfo_485(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
	bool InitNextMeterInfo_ZBWX(S_TaskCtrlStatus *pTaskCtrlStatus, const S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief 上报式采表基类
 *  @note GetMeterIndex需要重写；
 */
class C_TaskReportBase:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务运行态后处理
	 *  @return >=0:成功；<0:失败
	 */
	virtual int AfterExecute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);

protected:
	/** @brief 获取表计索引号
	 *  @return 表计序号或测量点号;无效:0xFFFF
	 */
	virtual S_WORD GetMeterIndex(const string &strMeterAddress) = 0;

	/** @brief 任务处于运行态
	 *  @return 0:采集结束；<0:继续采集
	 */
	int ReportMeterGather(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

/** @class
 *  @brief 直抄表采表基类
 */
class C_TaskDirectBase:public C_TaskPollBase
{
public:
	/** @brief 取任务执行器的名称 */
	virtual const char *GetName(void);
	/** @brief 任务处于运行态
	 *  @return TASK_EXEC_STATUS_
	 */
	virtual int Execute(const S_TaskInfo *pTaskInfo, S_TaskCtrlStatus *pTaskCtrlStatus, S_TaskFreezeStatus *pTaskFreezeStatus);
};

#endif //__BASEB_TASK_EXEC_H__



