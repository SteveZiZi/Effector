/** @file
 *  @brief 本地操作环绕菜单窗口定义
 *  @author MSZ
 *  @date 2015/05/30
 *  @version 0.1
 */
#ifndef __LCD_SURWND_H__
#define __LCD_SURWND_H__
#include "uiwnd.h"

class C_SURWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__LCD_SURWND_H__
