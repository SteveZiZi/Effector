//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguictrl.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUI_CTRL_H__
#define __SGUI_CTRL_H__
#include "sguiwnd.h"
#include "strings.h"
#include "lcddataiface.h"
#include <sstream>  
//using namespace std; 

#define NUMEDIT_STYLE_DEC		0x00000000UL
#define NUMEDIT_STYLE_OCT		0x00000100UL
#define NUMEDIT_STYLE_HEX		0x00000200UL

#define GM_CMD_BTN_PUSHED	0x00000000UL
#define GM_CMD_CTRL_UPDATE	0x00000000UL
//
//控件父类
//
class C_GUICTRL:public C_GUIWNDB
{
public:
	C_GUICTRL(void);
	virtual ~C_GUICTRL();

public:
	S_DWORD GetCtrlStyle(void){return m_flag;}
	S_DWORD GetCtrlID(void){return m_ID;}

protected:
	bool AddCtrl2ParentWnd(C_SGUIWND *pWnd);

	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_ID;
};

//
//数字编辑框，支持八进制、十进制、十六进制
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT）消息
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_CTRL_UPDATE(表示框内容有变化)
//
class C_NUMEDIT:public C_GUICTRL
{
public:
	C_NUMEDIT(void);
	virtual ~C_NUMEDIT();

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	bool IsOctChar(S_CHAR ch){return ch>='0'&&ch<='7';}
	bool IsDecChar(S_CHAR ch){return ch>='0'&&ch<='9';}
	bool IsHexChar(S_CHAR ch){return IsDecChar(ch) || ch>='A'&&ch<='F';}

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//简易ASCII码编辑框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT）消息
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_CTRL_UPDATE(表示框内容有变化)
//
class C_ASCEDIT:public C_GUICTRL
{
public:
	C_ASCEDIT(void);
	virtual ~C_ASCEDIT();

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	S_CHAR IncreaseChar(S_CHAR ch);
	S_CHAR DecreaseChar(S_CHAR ch);

protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};


struct S_VKBTN
{
	const S_CHAR *m_pName;
	const S_CHAR m_Val;
};
//
//虚拟键盘控件
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT,GVK_OK）消息
//
class C_VKBOARD:public C_GUICTRL
{
public:
	C_VKBOARD(void);
	virtual ~C_VKBOARD();

public:
	bool Create(S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	virtual int GetHighlightRect(S_RECT &rect, const C_SGUIDC *pdc);
	virtual S_WORD GetHighlightChar(void);

protected:
	int m_RowCharCnt;
	int m_RowIdx;
	int m_ColIdx;

protected:
	static const int m_VkBoardW = 139;
	static const int m_VkBoardH = 64;
	static const S_CHAR *m_VkAsciiSet[5];
	static const S_VKBTN m_VkSpecialKey[5];
};

//
//文本编辑框
//响应GM_KEYUP（GVK_LEFT,GVK_RIGHT）消息
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_CTRL_UPDATE(表示框内容有变化)
//
class C_TEXTEDIT:public C_GUICTRL
{
public:
	C_TEXTEDIT(void);
	virtual ~C_TEXTEDIT();

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	int m_CaretIdx;
	
protected:
	static const char m_SideW = 2;
	static const char m_SideH = 1;
};

//
//按钮控件
//响应GM_KEYUP（GVK_OK）消息
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_BTN_PUSHED(表示按钮按下了)
//
class C_BUTTON:public C_GUICTRL
{
public:
	C_BUTTON(void);
	virtual ~C_BUTTON();

public:
	bool Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

protected:
	S_DWORD m_state;
};

//
//选择框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_OK）消息
//选择文本用;间隔
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_CTRL_UPDATE(表示框内容有变化)
//lParam 为其它值为当前选择框确认的项
//

class C_SELECTBOX:public C_GUICTRL
{
public:
	C_SELECTBOX(void);
	C_SELECTBOX(S_RegSec regSec);
	virtual ~C_SELECTBOX();

public:
	bool Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 
	/** @brief 设置缺省项
	 * @param[in] idx 索引值 [0-(项总数-1)]
	 * @return true 设置成功; false 设置失败
	 */
	bool SetDefaultItem(S_DWORD idx);

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	int DrawFocusDirection(C_SGUIDC *pdc);

protected:
	S_WORD m_Index;
	strings m_SelectText;
	static const char m_Select_SideW = 90;
	static const char m_Select_SideH = 16;
	static const char m_Direction_High = 16;//固定高度为16（SYM16X16符号高度）
	static const char m_Direction_Width = 16;//固定宽度为16（SYM16X16符号高度）
	bool m_Flag; // 如果数据已被修改，则为true，反正为false
	double m_Value;
	bool m_Type; // 如果初始化时选项只有一个的话，则为true，如果初始化时选项有两个以上，则为false
	S_RegSec m_sRegSec; // 存储寄存器相关段落信息

private:
	string convertToString(double d);
	double convertFromString(string str);
};

//
//菜单选择框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_OK）消息
//选择文本用;间隔
//用户处理GM_CMD要点
//GM_CMD消息参数:wParam=控件ID,lParam=GM_CMD_CTRL_UPDATE(表示框内容有变化)
//lParam 为其它值为当前选择框确认的项
//

class C_MENUSELECTBOX:public C_GUICTRL
{
public:
	C_MENUSELECTBOX(void);
	C_MENUSELECTBOX(S_RegSec regSec);
	virtual ~C_MENUSELECTBOX();

public:
	bool Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID); 
	/** @brief 设置缺省项
	 * @param[in] idx 索引值 [0-(项总数-1)]
	 * @return true 设置成功; false 设置失败
	 */
	bool SetDefaultItem(S_DWORD idx);

protected:
	virtual int WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);
	virtual int DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam);

	int DrawFocusDirection(C_SGUIDC *pdc);

protected:
	S_WORD m_Index;
	strings m_SelectText;
	static const char m_Select_SideW = 90;
	static const char m_Select_SideH = 16;
	static const char m_Direction_High = 16;//固定高度为16（SYM16X16符号高度）
	static const char m_Direction_Width = 16;//固定宽度为16（SYM16X16符号高度）
	bool m_Flag; // 如果数据已被修改，则为true，反正为false
	double m_Value;
	bool m_Type; // 如果初始化时选项只有一个的话，则为true，如果初始化时选项有两个以上，则为false
	S_RegSec m_sRegSec; // 存储寄存器相关段落信息

private:
	string convertToString(double d);
	double convertFromString(string str);
};
#endif //__SGUI_CTRL_H__

