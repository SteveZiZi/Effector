/** @file
 *  @brief ���ز������Ͳ˵����ڶ���
 *  @author MSZ
 *  @date 2015/05/30
 *  @version 0.1
 */
#ifndef __LCD_SUBWND_H__
#define __LCD_SUBWND_H__
#include "uiwnd.h"

class C_SUBWND:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};

#endif//__LCD_SUBWND_H__
