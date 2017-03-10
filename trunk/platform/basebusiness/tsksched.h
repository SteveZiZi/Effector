/**
 *  @file
 *  @brief �ɼ����������
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
 *  @brief ���������
 *  @note 
 *  ʹ��˳�� 1:����init(); 2:�����û�����CreateTaskClient; 3:����ʼ����Schedule
 *  �����п�����������,��ʵʱ����
 *  �û���Ҫ��дGetTaskExec��GetTaskFreezeStatus
 */
class C_TaskSchedule
{
public:
	C_TaskSchedule(void);
	virtual ~C_TaskSchedule();

public:
	/** @brief �����������ʼ�� */
	bool Init(void);
	/** @brief �����û�����
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return true�ɹ�;falseʧ��
	 */
	bool CreateTaskClient(const S_TaskInfo &TaskInfo);
	/** @brief ������� 
	 *  @return �ݷ���0
	 */
	int Schedule(void);

protected:
	/** @brief ɾ���������� */
	bool DeleteTaskClient(void);
	/** @brief �����û��������û������б�
	 *  @param[in] pTaskClient �û�����
	 *  @return true�ɹ�;falseʧ��
	 */
	bool AddTask(C_TaskCtrl *pTaskClient);

	/** @brief ��ʼ���������� */
	int InitIdleTask(void);
	/** @brief �������������� */
	int CheckBlockList(void);
	/** @brief ������������� */
	int CheckReadyList(void);
	/** @brief �������� */
	int TaskRunning(void);

	/** @brief ��������
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return NULL����ʧ�� ��NULL�ɹ�
	 */
	C_TaskCtrl *NewTask(const S_TaskInfo &TaskInfo);
	/** @brief ɾ������ */
	void DeleteTask(C_TaskCtrl *pTaskClient);

	/** @brief ��ȡ����ִ����
	 *  @param[in] pTaskTypeName ����������
	 *  @return NULLʧ��;!NULL�ɹ�
	 */
	virtual C_TaskExec *GetTaskExec(const char *pTaskTypeName);
	/** @brief ��ȡ����־û�״̬
	 *  @param[in] TaskInfo ������Ϣ
	 *  @return NULLʧ��;!NULL�ɹ�
	 */
	virtual S_TaskFreezeStatus *GetTaskFreezeStatus(const S_TaskInfo &TaskInfo);

protected:
	C_TaskCtrl m_SysTask[SYS_TASK_MAX];///< ϵͳ����
	C_TaskCtrl *m_pUserTask[USER_TASK_MAX]; ///< �û�����

	C_TaskList m_BlockList;///< �����������
	C_TaskList m_ReadyList;///< �����������
	C_TaskCtrl *m_pRunTask;///< ��������
};

#endif //__BASEB_TASK_SCHEDULE_H__


