/** @file
 *  @brief �ն˹�����ά�����ڶ���
 *  @author Steven Wan yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#ifndef __LCD_MANAGE_WND_H__
#define __LCD_MANAGE_WND_H__
#include "uiwnd.h"
#include "strings.h"

//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
class C_ScreenParamViewWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};


//Һ������	�ɵ����ն�Һ����ʾ�Աȶȡ��������ȵ�
class C_ScreenParamSetWnd:public C_UIMWND
{
public:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
};
	
#endif //__LCD_MANAGE_WND_H__


