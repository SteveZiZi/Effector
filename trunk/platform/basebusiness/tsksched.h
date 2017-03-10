/**
 *  @file
 *  @brief 采集任务调度器
 *  @author Steven Wan
 *  @date 2014-06-19
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_TASK_SCHEDULE_H__
#define __BASEB_TASK_SCHEDULE_H__
#include "tskctrl.h"
#include "tskexec.h"
#include "basetype.h"

/** @class
 *  @brief 任务调度器
 *  @note 
 *  使用顺序 1:调用init(); 2:创建用户任务CreateTaskClient; 3:任务开始调度Schedule
 *  运行中可以增加任务,如实时任务
 *  用户需要重写GetTaskExec和GetTaskFreezeStatus
 */
class C_TaskSchedule
{
public:
	C_TaskSchedule(void);
	virtual ~C_TaskSchedule();

public:
	/** @brief 任务调度器初始化 */
	bool Init(void);
	/** @brief 创建用户任务
	 *  @param[in] TaskInfo 任务信息
	 *  @return true成功;false失败
	 */
	bool CreateTaskClient(const S_TaskInfo &TaskInfo);
	/** @brief 任务调度 
	 *  @return 暂返回0
	 */
	int Schedule(void);

protected:
	/** @brief 删除所有任务 */
	bool DeleteTaskClient(void);
	/** @brief 加入用户任务至用户任务列表
	 *  @param[in] pTaskClient 用户任务
	 *  @return true成功;false失败
	 */
	bool AddTask(C_TaskCtrl *pTaskClient);

	/** @brief 初始化空闲任务 */
	int InitIdleTask(void);
	/** @brief 检查阻塞任务队列 */
	int CheckBlockList(void);
	/** @brief 检查就绪任务队列 */
	int CheckReadyList(void);
	/** @brief 任务运行 */
	int TaskRunning(void);

	/** @brief 创建任务
	 *  @param[in] TaskInfo 任务信息
	 *  @return NULL创建失败 ！NULL成功
	 */
	C_TaskCtrl *NewTask(const S_TaskInfo &TaskInfo);
	/** @brief 删除任务 */
	void DeleteTask(C_TaskCtrl *pTaskClient);

	/** @brief 获取任务执行器
	 *  @param[in] pTaskTypeName 任务类型名
	 *  @return NULL失败;!NULL成功
	 */
	virtual C_TaskExec *GetTaskExec(const char *pTaskTypeName);
	/** @brief 获取任务持久化状态
	 *  @param[in] TaskInfo 任务信息
	 *  @return NULL失败;!NULL成功
	 */
	virtual S_TaskFreezeStatus *GetTaskFreezeStatus(const S_TaskInfo &TaskInfo);

protected:
	C_TaskCtrl m_SysTask[SYS_TASK_MAX];///< 系统任务
	C_TaskCtrl *m_pUserTask[USER_TASK_MAX]; ///< 用户任务

	C_TaskList m_BlockList;///< 阻塞任务队列
	C_TaskList m_ReadyList;///< 就绪任务队列
	C_TaskCtrl *m_pRunTask;///< 运行任务
};

#endif //__BASEB_TASK_SCHEDULE_H__


