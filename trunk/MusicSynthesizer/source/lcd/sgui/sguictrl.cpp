//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguictrl.cpp
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-06-06  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "sguictrl.h"
#include <math.h>
#include "htrace.h"
#include "parammode.h"
#include "status.h"

#define MAXFRE	22000.0
#define MINFRE	20.0
#define STEPFRE   pow(10.0,log10(MAXFRE / MINFRE) / 100)
//
//控件父类
//
C_GUICTRL::C_GUICTRL(void)
{
}

C_GUICTRL::~C_GUICTRL()
{
}

bool C_GUICTRL::AddCtrl2ParentWnd(C_SGUIWND *pWnd)
{
	if (pWnd == NULL)
		return false;
	return pWnd->AddControl(this);
}

int C_GUICTRL::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUIWNDB::WndProcess(msg, wParam, lParam);
}

int C_GUICTRL::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUIWNDB::DefWndProcess(msg, wParam, lParam);
}

//
//数字编辑框，支持八进制、十进制、十六进制
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT）消息
//
C_NUMEDIT::C_NUMEDIT(void)
{
}
C_NUMEDIT::~C_NUMEDIT()
{
}

bool C_NUMEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;

	for (size_t i = 0; i < m_WndText.size(); i++)
	{
		if (m_WndText[i] >= 'a' && m_WndText[i] <= 'f')
			m_WndText[i] -= 'a' - 'A';
		if ((GetCtrlStyle()&NUMEDIT_STYLE_OCT)==NUMEDIT_STYLE_OCT)
		{
			if (!IsOctChar(m_WndText[i])) m_WndText[i] = '0';
		}
		else if ((GetCtrlStyle()&NUMEDIT_STYLE_HEX)==NUMEDIT_STYLE_HEX)
		{
			if (!IsHexChar(m_WndText[i])) m_WndText[i] = '0';
		}
		else
		{
			if (!IsDecChar(m_WndText[i])) m_WndText[i] = '0';
		}
	}
	if (m_WndText.empty())
		m_WndText.push_back('0');

	SendWndMsg(GM_CREATE, 0x00, 0x00);
	return true;
}

int C_NUMEDIT::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+0*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case GM_KEYUP:{
		S_WORD x, y;
		C_SGUIDC *pdc = new C_SGUIDC(this);
		GetCaretPos(x, y);
		x = (x - 2) / pdc->GetFontWidth('0');
		if (wParam == GVK_UP)
		{
			m_WndText[x] = IncreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
		}
		else if(wParam == GVK_DN)
		{
			m_WndText[x] = DecreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
		}
		else if (wParam == GVK_LEFT)
		{
			if (x == 0)
				ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x-1) * pdc->GetFontWidth('0'), y);
		}
		else if (wParam == GVK_RIGHT)
		{
			if (x == m_WndText.size() - 1)
				ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x+1) * pdc->GetFontWidth('0'), y);
		}
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		delete pdc;}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_NUMEDIT::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

S_CHAR C_NUMEDIT::IncreaseChar(S_CHAR ch)
{
	if ((GetCtrlStyle()&NUMEDIT_STYLE_OCT)==NUMEDIT_STYLE_OCT)
	{
		return (ch >= '7')? '0':(ch+1);
	}
	else if ((GetCtrlStyle()&NUMEDIT_STYLE_HEX)==NUMEDIT_STYLE_HEX)
	{
		if (ch == '9') return 'A';
		else if (ch == 'F' || ch == 'f') return '0';
		else return ch+1;
	}

	return (ch >= '9')? '0':(ch+1);
}

S_CHAR C_NUMEDIT::DecreaseChar(S_CHAR ch)
{
	if ((GetCtrlStyle()&NUMEDIT_STYLE_OCT)==NUMEDIT_STYLE_OCT)
	{
		return (ch == '0')? '7':(ch-1);
	}
	else if ((GetCtrlStyle()&NUMEDIT_STYLE_HEX)==NUMEDIT_STYLE_HEX)
	{
		if (ch == '0') return 'F';
		else if (ch == 'A' || ch == 'a') return '9';
		else return ch-1;
	}

	return (ch == '0')? '9':(ch-1);
}


//
//简易ASCII码编辑框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT）消息
//
C_ASCEDIT::C_ASCEDIT(void)
{
}
C_ASCEDIT::~C_ASCEDIT()
{
}

bool C_ASCEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;

	for (size_t i = 0; i < m_WndText.size(); i++)
	{
		if (!(m_WndText[i] >= ' ' && m_WndText[i] <= '~'))
			m_WndText[i] = ' ';
	}
	if (m_WndText.empty())
		m_WndText.push_back('0');

	SendWndMsg(GM_CREATE, 0x00, 0x00);
	return true;
}

int C_ASCEDIT::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+0*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case GM_KEYUP:{
		S_WORD x, y;
		C_SGUIDC *pdc = new C_SGUIDC(this);
		GetCaretPos(x, y);
		x = (x - 2) / pdc->GetFontWidth('0');
		if (wParam == GVK_UP)
		{
			m_WndText[x] = IncreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
		}
		else if(wParam == GVK_DN)
		{
			m_WndText[x] = DecreaseChar(m_WndText[x]);
			pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
			m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
		}
		else if (wParam == GVK_LEFT)
		{
			if (x == 0)
				ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x-1) * pdc->GetFontWidth('0'), y);
		}
		else if (wParam == GVK_RIGHT)
		{
			if (x == m_WndText.size() - 1)
				ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
			else
				SetCaretPos(m_SideW + (x+1) * pdc->GetFontWidth('0'), y);
		}
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		delete pdc;}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_ASCEDIT::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

S_CHAR C_ASCEDIT::IncreaseChar(S_CHAR ch)
{
	return (ch >= '~')? ' ':(ch+1);
}

S_CHAR C_ASCEDIT::DecreaseChar(S_CHAR ch)
{
	return (ch <= ' ')? '~':(ch-1);
}


//
//虚拟键盘控件
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_LEFT,GVK_RIGHT,GVK_OK）消息
//
const S_CHAR *C_VKBOARD::m_VkAsciiSet[5] =
{
	"ABCDEFGHIJKLMNOPQRS",
	"TUVWXYZabcdefghijkl",
	"mnopqrstuvwxyz@#$%&",
	"1234567890 <>[](){}",
	"*^~_+-=|!:;,.?'\"\\ /"
};
const S_VKBTN C_VKBOARD::m_VkSpecialKey[5] =
{
	{"ESC", GVK_ESC}, {"Del", GVK_BACKSPACE}, {"Ent", GVK_ENTER}, {"Tab", GVK_TAB}, {"F1 ", GVK_F1}
};

C_VKBOARD::C_VKBOARD(void)
{
	m_RowCharCnt = 0;
	m_RowIdx = 0;
	m_ColIdx = 0;
}

C_VKBOARD::~C_VKBOARD()
{
}

bool C_VKBOARD::Create(S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndRect = rect;
	m_WndRect.w = m_VkBoardW;
	m_WndRect.h = m_VkBoardH;
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;

	SendWndMsg(GM_CREATE, 0x00, 0x00);
	return true;
}

int C_VKBOARD::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:
		m_RowIdx = 0;
		m_ColIdx = 0;
		m_RowCharCnt = (int)strlen(m_VkAsciiSet[0]);
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		for (int i = 0; i < 5; i++)
			pdc->DrawString(2, 2+i*pdc->GetFontHeight('0'), m_VkAsciiSet[i]);
		for (int i = 0; i < 5; i++)
			pdc->DrawString(2+3+m_RowCharCnt*pdc->GetFontWidth('0'), 2+i*pdc->GetFontHeight('0'), m_VkSpecialKey[i].m_pName);
		pdc->DrawVLine(2+1+m_RowCharCnt*pdc->GetFontWidth('0'), 1+0*pdc->GetFontHeight('0'), 3+5*pdc->GetFontHeight('0'));
		for (int i = 1; i < 5; i++)
			pdc->DrawHLine(1+3+m_RowCharCnt*pdc->GetFontWidth('0'), 2+i*pdc->GetFontHeight('0'), 2+3*pdc->GetFontWidth('0'));
		if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
		{
			GetHighlightRect(WndRect, pdc);
			pdc->ReverseRect(WndRect);
		}
		delete pdc;
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_DN || wParam == GVK_UP || wParam == GVK_LEFT || wParam == GVK_RIGHT)
		{
			S_RECT Rect;
			C_SGUIDC *pdc = new C_SGUIDC(this);
			if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
			{
				GetHighlightRect(Rect, pdc);
				pdc->ReverseRect(Rect);
			}
			if (wParam == GVK_DN)
				m_ColIdx = (m_ColIdx>=4)?0:m_ColIdx+1;
			else if (wParam == GVK_UP)
				m_ColIdx = (m_ColIdx<=0)?4:m_ColIdx-1;
			else if (wParam == GVK_LEFT)
				m_RowIdx = (m_RowIdx<=0)?m_RowCharCnt:m_RowIdx-1;
			else if (wParam == GVK_RIGHT)
				m_RowIdx = (m_RowIdx>=m_RowCharCnt)?0:m_RowIdx+1;
			if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
			{
				GetHighlightRect(Rect, pdc);
				pdc->ReverseRect(Rect);
			}
			delete pdc;
		}
		else if (wParam == GVK_OK)
		{
			ok = m_pParent->SendWndMsg(GM_CHAR, GetHighlightChar(), lParam);
		}
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_VKBOARD::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

int C_VKBOARD::GetHighlightRect(S_RECT &rect, const C_SGUIDC *pdc)
{
	if (m_RowIdx == (m_RowCharCnt + 1) - 1)
	{
		rect.x = 2+3+m_RowCharCnt*pdc->GetFontWidth('0');
		rect.y = 3+m_ColIdx*pdc->GetFontHeight('0');
		rect.w = 3*pdc->GetFontWidth('0');
		rect.h = pdc->GetFontHeight('0')-1;
	}
	else
	{
		rect.x = 2+m_RowIdx*pdc->GetFontWidth('0');
		rect.y = 2+m_ColIdx*pdc->GetFontHeight('0');
		rect.w = pdc->GetFontWidth('0');
		rect.h = pdc->GetFontHeight('0');
	}
	return 0;
}

S_WORD C_VKBOARD::GetHighlightChar(void)
{
	if (m_RowIdx == (m_RowCharCnt + 1) - 1)
		return (S_WORD)m_VkSpecialKey[m_ColIdx].m_Val;
	return (S_WORD)m_VkAsciiSet[m_ColIdx][m_RowIdx];
}

//
//文本编辑框
//响应GM_KEYUP（GVK_LEFT,GVK_RIGHT）消息
//
C_TEXTEDIT::C_TEXTEDIT(void)
{
	m_CaretIdx = 0;
}

C_TEXTEDIT::~C_TEXTEDIT()
{
}

bool C_TEXTEDIT::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;

	if (m_WndText.empty())
		m_WndText.append("  ");
	SendWndMsg(GM_CREATE, 0x00, 0x00);
	return true;
}

int C_TEXTEDIT::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		m_CaretIdx = 0;
		CreateCaret(pdc->GetFontWidth('0'), 1);
		SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
		ShowCaret();
		delete pdc;
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
		delete pdc;
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_LEFT)
		{
			C_SGUIDC *pdc = new C_SGUIDC(this);
			if (m_CaretIdx != 0)
			{
				m_CaretIdx -= 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
			}
			delete pdc;
		}
		else if (wParam == GVK_RIGHT)
		{
			C_SGUIDC *pdc = new C_SGUIDC(this);
			if (m_CaretIdx < (int)m_WndText.size())
			{
				m_CaretIdx += 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
			}
			delete pdc;
		}
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		break;
	case GM_CHAR:
		if (wParam == GVK_BACKSPACE)
		{
			C_SGUIDC *pdc = new C_SGUIDC(this);
			if (m_CaretIdx != 0)
			{
				size_t i = m_CaretIdx - 1;
				for (; i < m_WndText.size()-1; i++)
					m_WndText[i] = m_WndText[i+1];
				m_WndText[i] = ' ';
				pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
				m_CaretIdx -= 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
				m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
			}
			delete pdc;
		}
		else if (wParam == GVK_ENTER || wParam == GVK_ESC || wParam == GVK_F1)
		{
			ok = m_pParent->SendWndMsg(GM_CHAR, wParam, lParam);
		}
		else
		{
			if (wParam == GVK_TAB) wParam = ' ';
			C_SGUIDC *pdc = new C_SGUIDC(this);
			if (m_CaretIdx < (int)m_WndText.size() )
			{
				m_WndText[m_CaretIdx] = (S_CHAR)wParam;
				pdc->DrawString(m_SideW, m_SideH, m_WndText.c_str());
				m_CaretIdx += 1;
				SetCaretPos(m_SideW+m_CaretIdx*pdc->GetFontWidth('0'), m_SideH+pdc->GetFontHeight('0'));
				m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
			}
			delete pdc;
		}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_TEXTEDIT::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}


//
//按钮控件
//响应GM_KEYUP（GVK_OK）消息
//
C_BUTTON::C_BUTTON(void)
{
	m_state = 0;
}

C_BUTTON::~C_BUTTON()
{
}

bool C_BUTTON::Create(const S_CHAR *pszCaption, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	m_WndText = pszCaption;
	m_WndRect = rect;
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;

	SendWndMsg(GM_CREATE, 0x00, 0x00);
	return true;
}

int C_BUTTON::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_WPEN));
		pdc->FillRect(WndRect);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_BPEN));
		pdc->DrawRect(WndRect);
		pdc->DrawString(4, 2, m_WndText.c_str());
		if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
		{
			WndRect.x = 1, WndRect.y = 1;
			WndRect.w = WndRect.w - 2, WndRect.h = WndRect.h - 2;
			pdc->ReverseRect(WndRect);
		}
		delete pdc;
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			ok = m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_BTN_PUSHED);
		}
		else
		{
			ok = m_pParent->SendWndMsg(msg, wParam, lParam);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_BUTTON::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

//
//选择框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_OK）消息
//选择文本用;间隔
//
C_SELECTBOX::C_SELECTBOX(void)
{
	m_Index = 0;
	m_Flag = false;
	m_Type = false;
}

C_SELECTBOX::C_SELECTBOX(S_RegSec regSec)
{
	m_Index = 0;
	m_Flag = false;
	m_Type = false;
	m_sRegSec = regSec;
}

C_SELECTBOX::~C_SELECTBOX()
{
}

bool C_SELECTBOX::Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	m_WndRect = rect;
	//m_WndRect.h = m_Direction_High;//固定高度为16（SYM16X16符号高度）
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;
	//m_Index = 0;

	//字符串长度对齐
	size_t Length = 0;
	strings SelectText;
	SelectText.split(pszSelectText, ';');
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		if (Length < SelectText[i].size())
			Length = SelectText[i].size();
	}
	m_SelectText.clear(); // 防止selectBox中的内容被重复增加
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		string OneText(SelectText[i]);
		if (OneText.size() < Length)
			OneText.append(Length-OneText.size(), ' ');
		m_SelectText.push_back(OneText);
	}
	if (m_SelectText.size() < 2)//最少需要两项供选择
	{
		if (!m_Type)
		{
			m_WndText = m_SelectText[0];
		}
		m_Value = atof(m_WndText.c_str()); // string转换为double类型
		m_Type = true;
		//m_WndText = convertToString(m_Value); // double转化为string类型
	}
  
	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	if (m_Flag)
	{
		SendWndMsg(GM_PAINT, 0x00, 0x00);
	}
	else
	{
		SendWndMsg(GM_CREATE, 0x00, 0x00);
	}
	return true;
}

/** @brief 设置缺省项
 * @param[in] idx 索引值 [0 - (项总数-1)]
 * @return true 设置成功; false 设置失败
 */
bool C_SELECTBOX::SetDefaultItem(S_DWORD idx)
{
	if (!m_Type)
	{
		if (idx < m_SelectText.size())
		{
			m_Index = (S_WORD)idx;
			m_WndText = m_SelectText[m_Index];
			return true;
		}
	}
	else
	{
		m_WndText = convertToString(m_Value);
		return true;
	}
	return false;
}

int C_SELECTBOX::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:
		if (!m_Type)
		{
			char data = 0;
			C_LcdDataIface::ReadData(&m_sRegSec,&data);
#ifndef WIN32
			if (data > 10)
			{
				data = 0;
			}
#else
			if (data > 10 || data < 0)
			{
				data = 0;
			}
#endif
			m_Index = data;
			//loget << "index = " << data << endl;
			m_WndText = m_SelectText[m_Index];
		}
		else
		{	
			// 读取数据 这里读一般为读多字节的内容。
			if (HEXCODE == m_sRegSec.m_type) // 十六进制的读取 要将char转化为str 中间先转为int 否则出错
			{
				char data;
				C_LcdDataIface::ReadData(&m_sRegSec,&data);
				int intData = data;
				if ((intData < 0) || (intData > 100))
				{
					intData = 0;
				}
				string str;
				stringstream stream;
				stream << intData;
				stream >> str;
				m_WndText = str; // 将读到的数据存入到m_WndText中
				m_Value = convertFromString(m_WndText);
			}
			if (BCDCODE == m_sRegSec.m_type)
			{
				char temp[20];
				char *pdata = temp; // 接收读到的数据
				C_LcdDataIface::ReadData(&m_sRegSec,pdata);
				//loget << "tempvalue************** = " << *pdata <<  endl;
				m_WndText = pdata; // 将读到的数据存入到m_WndText中
				m_Value = convertFromString(m_WndText); // string转换为double类型
			}
			
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		//S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		//pdc->DrawRect(WndRect);
		//S_RECT SelectBoxRect = {m_WndRect.x, m_WndRect.y,m_WndRect.w, m_WndRect.h};
		string temp = "                  ";
		pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
		pdc->DrawString(m_WndRect.x, m_WndRect.y, m_WndText.c_str());
		//DrawFocusDirection(pdc);
		delete pdc;
		}
		break;
	case GM_CMD:
		if (wParam ==0x00) SetDefaultItem(lParam);
		break;
	case GM_KEYUP:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		if (wParam == GVK_UP)
		{
			
		}
		else if (wParam == GVK_DN)
		{
			
		}
#ifndef WIN32
		else if (wParam == KEY_SET_LEFT)
#else
		else if (wParam == GVK_LEFT)
#endif
		{
			if (m_Type)
			{
				if (HEXCODE == m_sRegSec.m_type)
				{
					if (m_Value > m_sRegSec.m_minValue)
					{
						m_Value = m_Value - m_sRegSec.m_step * lParam;
						if (m_Value < m_sRegSec.m_minValue)
						{
							m_Value = m_sRegSec.m_minValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						int intData = (int)m_Value;
						string str;
						stringstream stream;
						stream << intData;
						stream >> str;
						m_WndText = str; // 将读到的数据存入到m_WndText中
						size_t newSize = m_WndText.size();
						if (oldSize != newSize)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域*/
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if(m_Value <= m_sRegSec.m_minValue)
					{
						m_Value = m_sRegSec.m_minValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
				}
				if (BCDCODE == m_sRegSec.m_type)
				{
					if (m_Value > m_sRegSec.m_minValue)
					{
						// 处理频率步进
						if (21772.6 == m_sRegSec.m_maxValue)
						{
							//m_sRegSec.m_step = log(m_Value / 19.7) * 100;
							for(int i = 0; i < (int)lParam;i++)
							{
								m_Value = m_Value / STEPFRE;
							}
						}
						else
						{
							m_Value = m_Value - m_sRegSec.m_step * lParam;
						}
						
						if (m_Value < m_sRegSec.m_minValue)
						{
							m_Value = m_sRegSec.m_minValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						m_WndText = convertToString(m_Value);
						size_t newSize = m_WndText.size();
						if (oldSize != newSize && 21772.6 != m_sRegSec.m_maxValue)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
						}
						
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
						m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
					}
					if(m_Value <= m_sRegSec.m_minValue)
					{
						m_Value = m_sRegSec.m_minValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
					}
				}
			}
			else
			{
				if (m_Index > 0)
				{
					if (m_Index < lParam)
					{
						m_Index = 0;
					}
					else
					{
						m_Index -= 1 * lParam;
					}

					C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
					pdc->DrawString(m_WndRect.x, m_WndRect.y, m_SelectText[m_Index].c_str());
					//DrawFocusDirection(pdc);
					// 写入status.dat文件中
					char data = (char)m_Index;
					C_LcdDataIface::WriteData(&m_sRegSec,&data);
					m_WndText = m_SelectText[m_Index];
					m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
				}
			}
			m_Flag = true;
		}
#ifndef WIN32
		else if(wParam == KEY_SET_RIGHT)
#else
		else if(wParam == GVK_RIGHT)
#endif
		{
			
			if (m_Type)
			{
				if (HEXCODE == m_sRegSec.m_type)
				{
					if (m_Value < m_sRegSec.m_maxValue)
					{
						m_Value = m_Value + m_sRegSec.m_step * lParam;
						if (m_Value > m_sRegSec.m_maxValue)
						{
							m_Value = m_sRegSec.m_maxValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						int intData = (int)m_Value;
						string str;
						stringstream stream;
						stream << intData;
						stream >> str;
						m_WndText = str; // 将读到的数据存入到m_WndText中
						size_t newSize = m_WndText.size();
						if (oldSize != newSize)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域*/
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if(m_Value >= m_sRegSec.m_maxValue)
					{
						m_Value = m_sRegSec.m_maxValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
				}
				if (BCDCODE == m_sRegSec.m_type)
				{
					if(m_Value < m_sRegSec.m_maxValue)
					{
						// 处理频率步进
						if (21772.6 == m_sRegSec.m_maxValue)
						{
							for(int i = 0;i < (int)lParam;i++)
							{
								m_Value = m_Value * STEPFRE;
							}
						}
						else
						{
							m_Value = m_Value + m_sRegSec.m_step * lParam;
						}
						
						if (m_Value > m_sRegSec.m_maxValue)
						{
							m_Value = m_sRegSec.m_maxValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						m_WndText = convertToString(m_Value);
						size_t newSize = m_WndText.size();
						if (oldSize != newSize && 21772.6 != m_sRegSec.m_maxValue)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);

						m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
					}
					if(m_Value >= m_sRegSec.m_maxValue)
					{
						m_Value = m_sRegSec.m_maxValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
					}
				}
				
			}
			else
			{
				if (m_Index < m_SelectText.size() - 1)
				{
					m_Index += 1 * lParam;
					if (m_Index > m_SelectText.size() - 1)
					{
						m_Index = m_SelectText.size() - 1;
					}
					C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
					pdc->DrawString(m_WndRect.x, m_WndRect.y, m_SelectText[m_Index].c_str());
					// 写入status.dat文件中
					char data = (char)m_Index;
					C_LcdDataIface::WriteData(&m_sRegSec,&data);
					//DrawFocusDirection(pdc);
					m_WndText = m_SelectText[m_Index];
					m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
				}
			}
			m_Flag = true;
		}
/*
#ifndef WIN32
		else if(wParam == KEY_SET)
#else
		else if(wParam == GVK_OK)
#endif
		{
			if (m_Type)
			{
				// 将修改的数据写入到flash中
				m_Flag = true;
				if (BCDCODE == m_sRegSec.m_type)
				{
					char*data=(char*)m_WndText.data();
					C_LcdDataIface::WriteData(&m_sRegSec,data);
					SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
				}
				if (HEXCODE == m_sRegSec.m_type) // 十六进制的写
				{
					char data;
					int intData;
					m_Value = convertFromString(m_WndText);
					intData = (int)m_Value;
					data = intData;
					C_LcdDataIface::WriteData(&m_sRegSec,&data);
					SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
				}
			}
			else
			{
				// 将修改的数据写入flash中
				char data;
				SetDefaultItem(m_Index);
				m_Flag = true;
				SendWndMsg(GM_CMD,m_Index,GM_CMD_CTRL_UPDATE);
				data = (char)m_Index;
				C_LcdDataIface::WriteData(&m_sRegSec,&data);
				SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
			}
		}

#ifndef WIN32
		else if(wParam == KEY_ESC)
#else
		else if(wParam == GVK_ESC)
#endif
		{
			//SetDefaultItem(m_Index);
			SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
		}*/
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		delete pdc;
		}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_SELECTBOX::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

int C_SELECTBOX::DrawFocusDirection(C_SGUIDC *pdc)
{
	S_WORD x = 0;
	static const char * const FocusDirection[] = {"P","Q","R","S"};
	
	if (m_WndRect.w > m_Direction_Width)
		x = m_WndRect.w - m_Direction_Width;

	pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM16x16));
	if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
	{
		if (m_Index == 0)
			pdc->DrawString(x, 0, FocusDirection[2]);
		else if(m_Index >= m_SelectText.size() - 1)
			pdc->DrawString(x, 0, FocusDirection[1]);
		else
			pdc->DrawString(x, 0, FocusDirection[0]);
	}
	else
	{
		pdc->DrawString(x, 0, FocusDirection[3]);
	}
	return 0;
}

string C_SELECTBOX::convertToString(double d) 
{  
	ostringstream os;  
	os << d;
	return os.str();   
}  

double C_SELECTBOX::convertFromString(string str) 
{  
	istringstream iss(str);  
	double x;  
	iss >> x;
	return x;  
}  

//
//菜单选择框
//响应GM_KEYUP（GVK_UP,GVK_DN,GVK_OK）消息
//选择文本用;间隔
//
C_MENUSELECTBOX::C_MENUSELECTBOX(void)
{
	m_Index = 0;
	m_Flag = false;
	m_Type = false;
}

C_MENUSELECTBOX::C_MENUSELECTBOX(S_RegSec regSec)
{
	m_Index = 0;
	m_Flag = false;
	m_Type = false;
	m_sRegSec = regSec;
}

C_MENUSELECTBOX::~C_MENUSELECTBOX()
{
}

bool C_MENUSELECTBOX::Create(const S_CHAR *pszSelectText, S_DWORD dwStyle, const S_RECT& rect, C_SGUIWND* pParentWnd, S_DWORD nID)
{
	m_WndRect = rect;
	//m_WndRect.h = m_Direction_High;//固定高度为16（SYM16X16符号高度）
	m_pParent = pParentWnd;
	m_flag = dwStyle | WNDF_VISIBLE;
	m_ID = nID;
	//m_Index = 0;

	//字符串长度对齐
	size_t Length = 0;
	strings SelectText;
	SelectText.split(pszSelectText, ';');
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		if (Length < SelectText[i].size())
			Length = SelectText[i].size();
	}
	m_SelectText.clear(); // 防止selectBox中的内容被重复增加
	for (S_WORD i = 0; i < SelectText.size(); i++)
	{
		string OneText(SelectText[i]);
		if (OneText.size() < Length)
			OneText.append(Length-OneText.size(), ' ');
		m_SelectText.push_back(OneText);
	}
	if (m_SelectText.size() < 2)//最少需要两项供选择
	{
		if (!m_Type)
		{
			m_WndText = m_SelectText[0];
		}
		m_Value = atof(m_WndText.c_str()); // string转换为double类型
		m_Type = true;
		//m_WndText = convertToString(m_Value); // double转化为string类型
	}

	if (AddCtrl2ParentWnd(pParentWnd) == false)
		return false;

	if (m_Flag)
	{
		SendWndMsg(GM_PAINT, 0x00, 0x00);
	}
	else
	{
		SendWndMsg(GM_CREATE, 0x00, 0x00);
	}
	return true;
}
/** @brief 设置缺省项
 * @param[in] idx 索引值 [0 - (项总数-1)]
 * @return true 设置成功; false 设置失败
 */
bool C_MENUSELECTBOX::SetDefaultItem(S_DWORD idx)
{
	if (!m_Type)
	{
		if (idx < m_SelectText.size())
		{
			m_Index = (S_WORD)idx;
			m_WndText = m_SelectText[m_Index];
			return true;
		}
	}
	else
	{
		m_WndText = convertToString(m_Value);
		return true;
	}
	return false;
}

int C_MENUSELECTBOX::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	int ok = 0;
	switch (msg)
	{
	case GM_CREATE:
		if (!m_Type)
		{
			char data = 0;
			if (m_sRegSec.m_regid == 0xffff)
			{
				S_STATUS MyStatus;
				C_ParamMode::LoadTermMenuConfig(&MyStatus);

				if (m_sRegSec.m_regSubId == 0xfffc)
				{
					data = MyStatus.m_MenuConfig.m_CurParamMode;
				}
				else if (m_sRegSec.m_regSubId == 0xfffd)
				{
					data = MyStatus.m_MenuConfig.m_MusicInput;
				}
			}
			if (m_sRegSec.m_regid == 0x1201)
			{
				C_LcdDataIface::ReadData(&m_sRegSec,&data);
			}
			if (data > 10)
			{
				data = 0;
			}
			m_Index = data;
			//loget << "index = " << data << endl;
			m_WndText = m_SelectText[m_Index];
		}
		else
		{	
			// 读取数据 这里读一般为读多字节的内容。
			if (HEXCODE == m_sRegSec.m_type) // 十六进制的读取 要将char转化为str 中间先转为int 否则出错
			{
				char data;
				C_LcdDataIface::ReadData(&m_sRegSec,&data);
				int intData = data;
				if ((intData < 0) || (intData > 100))
				{
					intData = 0;
				}
				string str;
				stringstream stream;
				stream << intData;
				stream >> str;
				m_WndText = str; // 将读到的数据存入到m_WndText中
				m_Value = convertFromString(m_WndText);
			}
			if (BCDCODE == m_sRegSec.m_type)
			{
				char temp[20];
				char *pdata = temp; // 接收读到的数据
				C_LcdDataIface::ReadData(&m_sRegSec,pdata);
				//loget << "tempvalue************** = " << *pdata <<  endl;
				m_WndText = pdata; // 将读到的数据存入到m_WndText中
				m_Value = convertFromString(m_WndText); // string转换为double类型
			}
			
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		//S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		//pdc->DrawRect(WndRect);
		//S_RECT SelectBoxRect = {m_WndRect.x, m_WndRect.y,m_WndRect.w, m_WndRect.h};
		string temp = "                  ";
		pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
		pdc->DrawString(m_WndRect.x, m_WndRect.y, m_WndText.c_str());
		//DrawFocusDirection(pdc);
		delete pdc;
		}
		break;
	case GM_CMD:
		if (wParam ==0x00) SetDefaultItem(lParam);
		break;
	case GM_KEYUP:{
		C_SGUIDC *pdc = new C_SGUIDC(this);
		if (wParam == GVK_UP)
		{
			
		}
		else if (wParam == GVK_DN)
		{
			
		}
		else if (wParam == KEY_SET_LEFT)
		{
			if (m_Type)
			{
				if (HEXCODE == m_sRegSec.m_type)
				{
					if (m_Value > m_sRegSec.m_minValue)
					{
						m_Value = m_Value - m_sRegSec.m_step * lParam;
						if (m_Value < m_sRegSec.m_minValue)
						{
							m_Value = m_sRegSec.m_minValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						int intData = (int)m_Value;
						string str;
						stringstream stream;
						stream << intData;
						stream >> str;
						m_WndText = str; // 将读到的数据存入到m_WndText中
						size_t newSize = m_WndText.size();
						if (oldSize != newSize)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域*/
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if(m_Value <= m_sRegSec.m_minValue)
					{
						m_Value = m_sRegSec.m_minValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
				}
				if (BCDCODE == m_sRegSec.m_type)
				{
					if (m_Value > m_sRegSec.m_minValue)
					{
						// 处理频率步进
						if (21772.6 == m_sRegSec.m_maxValue)
						{
							//m_sRegSec.m_step = log(m_Value / 19.7) * 100;
							for(int i = 0; i < (int)lParam;i++)
							{
								m_Value = m_Value / STEPFRE;
							}
						}
						else
						{
							m_Value = m_Value - m_sRegSec.m_step * lParam;
						}

						if (m_Value < m_sRegSec.m_minValue)
						{
							m_Value = m_sRegSec.m_minValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						m_WndText = convertToString(m_Value);
						size_t newSize = m_WndText.size();
						if (oldSize != newSize && 21772.6 != m_sRegSec.m_maxValue)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
						}

						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
						m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
					}
					if(m_Value <= m_sRegSec.m_minValue)
					{
						m_Value = m_sRegSec.m_minValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
					}
				}
			}
			else
			{
				if (m_Index > 0)
				{
					if (m_Index < lParam)
					{
						m_Index = 0;
					}
					else
					{
						m_Index -= 1 * lParam;
					}

					char data = (char)m_Index;

					if (m_sRegSec.m_regid != 0xffff)
					{
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if (m_sRegSec.m_regid == 0xffff)
					{
						S_STATUS MyStatus;
						C_ParamMode::LoadTermMenuConfig(&MyStatus);
						if (m_sRegSec.m_regSubId == 0xffff)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
						else if (m_sRegSec.m_regSubId == 0xfffe)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/status.dat /mnt/dyjc/set/M%d/",data + 1);
							system(cpstr);
						}
						else if (m_sRegSec.m_regSubId == 0xfffd)
						{
							MyStatus.m_MenuConfig.m_MusicInput = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
						}
						else if (m_sRegSec.m_regSubId == 0xfffc)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
						else if (m_sRegSec.m_regSubId == 0xfffb)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
					}

					pdc->DrawString(m_WndRect.x, m_WndRect.y, m_SelectText[m_Index].c_str());
					//DrawFocusDirection(pdc);
					// 写入status.dat文件中
					//char data = (char)m_Index;
					//C_LcdDataIface::WriteData(&m_sRegSec,&data);
					m_WndText = m_SelectText[m_Index];
					m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
				}
			}
			m_Flag = true;
		}
		else if(wParam == KEY_SET_RIGHT)
		{
			if (m_Type)
			{
				if (HEXCODE == m_sRegSec.m_type)
				{
					if (m_Value < m_sRegSec.m_maxValue)
					{
						m_Value = m_Value + m_sRegSec.m_step * lParam;
						if (m_Value > m_sRegSec.m_maxValue)
						{
							m_Value = m_sRegSec.m_maxValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						int intData = (int)m_Value;
						string str;
						stringstream stream;
						stream << intData;
						stream >> str;
						m_WndText = str; // 将读到的数据存入到m_WndText中
						size_t newSize = m_WndText.size();
						if (oldSize != newSize)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域*/
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if(m_Value >= m_sRegSec.m_maxValue)
					{
						m_Value = m_sRegSec.m_maxValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件中
						char data;
						int intTempData = (int)m_Value;
						data = intTempData;
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
				}
				if (BCDCODE == m_sRegSec.m_type)
				{
					if(m_Value < m_sRegSec.m_maxValue)
					{
						// 处理频率步进
						if (21772.6 == m_sRegSec.m_maxValue)
						{
							for(int i = 0;i < (int)lParam;i++)
							{
								m_Value = m_Value * STEPFRE;
							}
						}
						else
						{
							m_Value = m_Value + m_sRegSec.m_step * lParam;
						}

						if (m_Value > m_sRegSec.m_maxValue)
						{
							m_Value = m_sRegSec.m_maxValue;
						}
						size_t oldSize = m_WndText.size();
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						m_WndText = convertToString(m_Value);
						size_t newSize = m_WndText.size();
						if (oldSize != newSize && 21772.6 != m_sRegSec.m_maxValue)
						{
							string temp = "                  ";
							pdc->DrawString(m_WndRect.x, m_WndRect.y, temp.c_str());// 为了刷新之前的区域
						}
						pdc->DrawString(m_WndRect.x, m_WndRect.y,m_WndText.c_str());
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);

						m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
					}
					if(m_Value >= m_sRegSec.m_maxValue)
					{
						m_Value = m_sRegSec.m_maxValue;
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						// 写入status.dat文件
						char*data=(char*)m_WndText.data();
						C_LcdDataIface::WriteData(&m_sRegSec,data);
					}
				}
			}
			else
			{
				if (m_Index < m_SelectText.size() - 1)
				{
					m_Index += 1 * lParam;
					if (m_Index > m_SelectText.size() - 1)
					{
						m_Index = m_SelectText.size() - 1;
					}
					char data = (char)m_Index;

					if (m_sRegSec.m_regid != 0xffff)
					{
						C_LcdDataIface::WriteData2Dsp(&m_sRegSec);
						C_LcdDataIface::WriteData(&m_sRegSec,&data);
					}
					if (m_sRegSec.m_regid == 0xffff)
					{
						S_STATUS MyStatus;
						C_ParamMode::LoadTermMenuConfig(&MyStatus);
						if (m_sRegSec.m_regSubId == 0xffff)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
						else if (m_sRegSec.m_regSubId == 0xfffe)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/status.dat /mnt/dyjc/set/M%d/",data + 1);
							system(cpstr);
						}
						else if (m_sRegSec.m_regSubId == 0xfffd)
						{
							MyStatus.m_MenuConfig.m_MusicInput = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
						}
						else if (m_sRegSec.m_regSubId == 0xfffc)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
						else if (m_sRegSec.m_regSubId == 0xfffb)
						{
							MyStatus.m_MenuConfig.m_CurParamMode = data;
							C_ParamMode::SaveTermMenuConfig(&MyStatus);
							char cpstr[100];
							sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
							system("rm /mnt/dyjc/set/status.dat");
							system(cpstr);
							STATUS.Prepare();
							STATUS.LoadStatus();
						}
					}
					
					pdc->DrawString(m_WndRect.x, m_WndRect.y, m_SelectText[m_Index].c_str());
					// 写入status.dat文件中
					//char data = (char)m_Index;
					//C_LcdDataIface::WriteData(&m_sRegSec,&data);
					//DrawFocusDirection(pdc);
					m_WndText = m_SelectText[m_Index];
					m_pParent->SendWndMsg(GM_CMD, (S_WORD)m_ID, GM_CMD_CTRL_UPDATE);
				}
			}
			m_Flag = true;
		}
		/*else if(wParam == KEY_SET)
		{
			if (m_Type)
			{
				// 将修改的数据写入到flash中
				m_Flag = true;
				if (BCDCODE == m_sRegSec.m_type)
				{
					char*data=(char*)m_WndText.data();
					C_LcdDataIface::WriteData(&m_sRegSec,data);
					SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
				}
				if (HEXCODE == m_sRegSec.m_type) // 十六进制的写
				{
					char data;
					int intData;
					m_Value = convertFromString(m_WndText);
					intData = (int)m_Value;
					data = intData;
					C_LcdDataIface::WriteData(&m_sRegSec,&data);
					SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
				}
			}
			else
			{
				// 将修改的数据写入flash中
				char data;
				SetDefaultItem(m_Index);
				m_Flag = true;
				SendWndMsg(GM_CMD,m_Index,GM_CMD_CTRL_UPDATE);
				data = (char)m_Index;
				
				if (m_sRegSec.m_regid == 0x1201)
				{
					C_LcdDataIface::WriteData(&m_sRegSec,&data);
				}
				if (m_sRegSec.m_regid == 0xffff)
				{
					S_STATUS MyStatus;
					C_ParamMode::LoadTermMenuConfig(&MyStatus);
					if (m_sRegSec.m_regSubId == 0xffff)
					{
						MyStatus.m_MenuConfig.m_CurParamMode = data;
						C_ParamMode::SaveTermMenuConfig(&MyStatus);
						char cpstr[100];
						sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
						system("rm /mnt/dyjc/set/status.dat");
						system(cpstr);
						STATUS.Prepare();
						STATUS.LoadStatus();
					}
					else if (m_sRegSec.m_regSubId == 0xfffe)
					{
						MyStatus.m_MenuConfig.m_CurParamMode = data;
						C_ParamMode::SaveTermMenuConfig(&MyStatus);
						char cpstr[100];
						sprintf(cpstr,"cp -f /mnt/dyjc/set/status.dat /mnt/dyjc/set/M%d/",data + 1);
						system(cpstr);
					}
					else if (m_sRegSec.m_regSubId == 0xfffd)
					{
						MyStatus.m_MenuConfig.m_MusicInput = data;
						C_ParamMode::SaveTermMenuConfig(&MyStatus);
					}
					else if (m_sRegSec.m_regSubId == 0xfffc)
					{
						MyStatus.m_MenuConfig.m_CurParamMode = data;
						C_ParamMode::SaveTermMenuConfig(&MyStatus);
						char cpstr[100];
						sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
						system("rm /mnt/dyjc/set/status.dat");
						system(cpstr);
						STATUS.Prepare();
						STATUS.LoadStatus();
					}
					else if (m_sRegSec.m_regSubId == 0xfffb)
					{
						MyStatus.m_MenuConfig.m_CurParamMode = data;
						C_ParamMode::SaveTermMenuConfig(&MyStatus);
						char cpstr[100];
						sprintf(cpstr,"cp -f /mnt/dyjc/set/M%d/status.dat /mnt/dyjc/set/",data + 1);
						system("rm /mnt/dyjc/set/status.dat");
						system(cpstr);
						STATUS.Prepare();
						STATUS.LoadStatus();
					}
				}
				SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
			}
		}
		else if(wParam == KEY_ESC)
		{
			SetDefaultItem(m_Index);
			SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
		}*/
		else
		{
			ok = m_pParent->SendWndMsg(GM_KEYUP, wParam, lParam);
		}
		delete pdc;
		}
		break;
	default:
		return C_GUICTRL::WndProcess(msg, wParam, lParam);
	}
	return ok;
}

int C_MENUSELECTBOX::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_GUICTRL::DefWndProcess(msg, wParam, lParam);
}

int C_MENUSELECTBOX::DrawFocusDirection(C_SGUIDC *pdc)
{
	S_WORD x = 0;
	static const char * const FocusDirection[] = {"P","Q","R","S"};
	
	if (m_WndRect.w > m_Direction_Width)
		x = m_WndRect.w - m_Direction_Width;

	pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM16x16));
	if ((GetWndLong()&WNDF_FOCUS) == WNDF_FOCUS)
	{
		if (m_Index == 0)
			pdc->DrawString(x, 0, FocusDirection[2]);
		else if(m_Index >= m_SelectText.size() - 1)
			pdc->DrawString(x, 0, FocusDirection[1]);
		else
			pdc->DrawString(x, 0, FocusDirection[0]);
	}
	else
	{
		pdc->DrawString(x, 0, FocusDirection[3]);
	}
	return 0;
}

string C_MENUSELECTBOX::convertToString(double d) 
{  
	ostringstream os;  
	os << d;
	return os.str();   
}  

double C_MENUSELECTBOX::convertFromString(string str) 
{  
	istringstream iss(str);  
	double x;  
	iss >> x;
	return x;  
} 


