/** @file
 *  @brief 本地操作音乐菜单窗口定义
 *  @author MSZ
 *  @date 2015/06/01
 *  @version 0.1
 */
#ifndef __LCD_MUSICWND_H__
#define __LCD_MUSICWND_H__
#include "uiwnd.h"

class C_MUSICWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__LCD_MUSICWND_H__
