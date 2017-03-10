/** @file
 *  @brief 本地操作中置菜单窗口定义
 *  @author MSZ
 *  @date 2015/05/30
 *  @version 0.1
 */
#ifndef __LCD_CENWND_H__
#define __LCD_CENWND_H__
#include "uiwnd.h"

class C_CENWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__LCD_CENWND_H__

