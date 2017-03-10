/** @file
 *  @brief PPP拨号服务功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#ifndef __PPP_SERVER_H__
#define __PPP_SERVER_H__
#include "pppbase.h"
#include "sysbase.h"

class C_PppServer:public C_PppBase
{
public:
	C_PppServer(void){}
	virtual ~C_PppServer(){}

	virtual int BeforeService(void);

};

#endif//__PPP_SERVER_H__

