/** @file
 *  @brief 本地操作效果菜单窗口定义
 *  @author MSZ
 *  @date 2015/06/01
 *  @version 0.1
 */
#ifndef __LCD_EFFECTWND_H__
#define __LCD_EFFECTWND_H__
#include "uiwnd.h"

class C_EFFECTWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__LCD_EFFECTWND_H__

