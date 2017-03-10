/** @file
 *  @brief 表计能量采集功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#ifndef __DBCJ_SERVER_H__
#define __DBCJ_SERVER_H__
#include "server.h"

class C_DbcjServer:public C_SERVER
{
public:
	virtual int BeforeService(void);
	virtual int AfterService(void);
	virtual int OnTickService(void);
	
private:
		
private:
};

#endif//__DBCJ_SERVER_H__

