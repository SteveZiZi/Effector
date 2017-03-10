/** @file
 *  @brief ���������
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1����������̳��Զ���̿��
 *  2�����ļ�����������ɫ���������
 */
#ifndef __NW_APP_CMD_H__
#define __NW_APP_CMD_H__
#include "appcmd.h"


/** @class 
 *  @brief �����������
 *  @remarks 
 */
class C_NwAppCmd:public C_AppCmd
{
public:
	C_NwAppCmd(int argc, char *argv[]);
	virtual ~C_NwAppCmd();

protected:
	virtual C_SERVER *GetServer(const char *pServerName);
	virtual int ListVersion(void);
};

#endif//__NW_APP_CMD_H__


