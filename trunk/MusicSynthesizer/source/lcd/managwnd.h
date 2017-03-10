/** @file
 *  @brief 终端管理与维护窗口定义
 *  @author Steven Wan yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_MANAGE_WND_H__
#define __LCD_MANAGE_WND_H__
#include "uiwnd.h"
#include "strings.h"

//液晶调节	可调节终端液晶显示对比度、背光亮度等
class C_ScreenParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};


//液晶调节	可调节终端液晶显示对比度、背光亮度等
class C_ScreenParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
	
#endif //__LCD_MANAGE_WND_H__


