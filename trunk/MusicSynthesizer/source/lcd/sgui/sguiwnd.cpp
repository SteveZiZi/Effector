//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguiwnd.cpp
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#include "sguiwnd.h"
#include "sguiapp.h"
#include "sguibase.h"
#include "sguicart.h"
#include "sguictrl.h"

/** GUI base window define
  */
C_WNDBASE::C_WNDBASE(void):C_SGUIOBJ(C_SGUIOBJ::OBJ_T_WNDB)
{
	m_WndRect.x = 0;
	m_WndRect.y = 0;
	m_WndRect.w = SGUI_LcdWidth();
	m_WndRect.h = SGUI_LcdHeight();
	m_InvalidRect.x = 0;
	m_InvalidRect.y = 0;
	m_InvalidRect.w = 0;
	m_InvalidRect.h = 0;

	m_flag = WNDF_VISIBLE;

	m_pParent = NULL;
}

C_WNDBASE::~C_WNDBASE()
{
}

int C_WNDBASE::SendWndMsg(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return WndProcess(msg, wParam, lParam);
}

int C_WNDBASE::PostWndMsg(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_GUIMSG aMsg;

	aMsg.pWnd = this;
	aMsg.Msg = msg;
	aMsg.wParam = wParam;
	aMsg.lParam = lParam;
	SGuiApp->PostGuiMsg(aMsg);

	return 0;
}

int C_WNDBASE::ClientToScreen(S_RECT &rect)
{
	rect.x += m_WndRect.x, rect.y += m_WndRect.y;
	if (rect.x+rect.w > m_WndRect.x+m_WndRect.w)
		rect.w = m_WndRect.x+m_WndRect.w - rect.x;
	if (rect.y+rect.h > m_WndRect.y+m_WndRect.h)
		rect.h = m_WndRect.y+m_WndRect.h - rect.y;

	C_WNDBASE *pWnd = m_pParent;
	while (pWnd)
	{
		rect.x += pWnd->m_WndRect.x, rect.y += pWnd->m_WndRect.y;
		if (rect.x+rect.w > pWnd->m_WndRect.x+pWnd->m_WndRect.w)
			rect.w = pWnd->m_WndRect.x+pWnd->m_WndRect.w - rect.x;
		if (rect.y+rect.h > pWnd->m_WndRect.y+pWnd->m_WndRect.h)
			rect.h = pWnd->m_WndRect.y+pWnd->m_WndRect.h - rect.y;

		pWnd = pWnd->m_pParent;
	}

	return 0;
}

void C_WNDBASE::UpdateWnd(void)
{
	if (m_InvalidRect.w != 0 && m_InvalidRect.h != 0)
	{
		S_RECT rect = m_InvalidRect;
		//ClientToScreen(rect);
		SGUI_flushScreen(rect.x, rect.y, rect.w, rect.h);
	}
	else
	{
		SGUI_flushScreen(m_WndRect.x, m_WndRect.y, m_WndRect.w, m_WndRect.h);
	}

	m_InvalidRect.x = 0;
	m_InvalidRect.y = 0;
	m_InvalidRect.w = 0;
	m_InvalidRect.h = 0;
}

void C_WNDBASE::ShowWnd(bool bShow)
{
	if (bShow == true)
		m_flag |= WNDF_VISIBLE;
	else
		m_flag &= ~WNDF_VISIBLE;
}

void C_WNDBASE::EnableWnd(bool bEnable)
{
	if (bEnable == true)
		m_flag |= WNDF_DISABLED;
	else
		m_flag &= ~WNDF_DISABLED;
}

int C_WNDBASE::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(msg, wParam, lParam);
}

int C_WNDBASE::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return 0;
}

/** GUI window define
  */
C_GUIWNDB::C_GUIWNDB(void)
{
	m_pCurCaret = NULL;
}

C_GUIWNDB::~C_GUIWNDB(void)
{
	delete m_pCurCaret;
}

int C_GUIWNDB::CreateCaret(S_WORD w, S_WORD h)
{
	m_pCurCaret = new C_SGUICARET(this, w, h);
	SGuiApp->SetAppCaret(m_pCurCaret);
	return 0;
}

int C_GUIWNDB::SetCaretPos(S_WORD x, S_WORD y)
{
	if (m_pCurCaret == NULL)
		return -1;

	if (m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndMsg(GM_PAINT, 0, 0);

	S_RECT rect = m_pCurCaret->GetWndRect();
	rect.x = x, rect.y = y;
	m_pCurCaret->SetWndRect(rect);

	m_pCurCaret->SendWndMsg(GM_PAINT, 0, 0);

	return 0;
}

int C_GUIWNDB::GetCaretPos(S_WORD &x, S_WORD &y)
{
	if (m_pCurCaret == NULL)
		return -1;
	S_RECT rect = m_pCurCaret->GetWndRect();
	x = rect.x,  y = rect.y;

	return 0;
}

int C_GUIWNDB::ShowCaret(void)
{
	if (m_pCurCaret == NULL)
		return -1;
	m_pCurCaret->ShowWnd(true);
	return 0;
}

int C_GUIWNDB::HideCaret(void)
{
	if (m_pCurCaret == NULL)
		return -1;
	if (m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndMsg(GM_PAINT, 0, 0);
	m_pCurCaret->ShowWnd(false);
	return 0;
}

int C_GUIWNDB::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(msg, wParam, lParam);
}

int C_GUIWNDB::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_SETFOCUS:
		m_flag |= WNDF_FOCUS;
		SGuiApp->SetAppCaret(m_pCurCaret);
		break;
	case GM_KILLFOCUS:{
		C_SGUICARET *pCaret = SGuiApp->GetAppCaret();
		if (pCaret != NULL)
		{
			if (pCaret->GetCaretStatus())
				pCaret->SendWndMsg(GM_PAINT, 0x00, 0x00);
		}
		m_flag &= ~WNDF_FOCUS;
		SGuiApp->SetAppCaret(NULL);}
		break;
	default:
		return C_WNDBASE::DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}


/** User wnd define
 */
C_SGUIWND::C_SGUIWND(void)
{
	m_ObjT = OBJ_T_WND;

	m_WndRect.x = 0;
	m_WndRect.y = 0;
	m_WndRect.w = SGUI_LcdWidth();
	m_WndRect.h = SGUI_LcdHeight();

	m_pWndData  = NULL;
	m_pFatherWndData = NULL;
	m_ScrnFlushEn = false;
}

C_SGUIWND::~C_SGUIWND()
{
}

C_SGUIDC *C_SGUIWND::BeginPaint(void)
{
	C_SGUIDC *pdc = new C_SGUIDC(this);
	S_RECT rect = m_WndRect;

	C_SGUICARET *pAppCaret = SGuiApp->GetAppCaret();
	if (pAppCaret && pAppCaret->GetCaretStatus())
		pAppCaret->SendWndMsg(GM_PAINT, 0, 0);
	if (m_pCurCaret && m_pCurCaret->GetCaretStatus())
		m_pCurCaret->SendWndMsg(GM_PAINT, 0, 0);

	m_ScrnFlushEn = SGUI_FlushScreenEn(false);
	pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_WPEN));
	rect.x = 0, rect.y = 0;
	pdc->FillRect(rect);
	pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_BPEN));

	return pdc;
}

void C_SGUIWND::EndPaint(C_SGUIDC *pDC)
{
	if (!m_CtrlQ.empty())
	{
		for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
		{
			C_GUICTRL *pWnd = m_CtrlQ[i];
			if (pWnd->GetWndLong() & WNDF_VISIBLE)
				pWnd->SendWndMsg(GM_PAINT, 0x00, 0x00);
		}
	}
	
	SGUI_FlushScreenEn(m_ScrnFlushEn);
	UpdateWnd();
	delete pDC;
}

void *C_SGUIWND::GetWndData(int size)
{
	if (m_pWndData == NULL)
		m_pWndData = new char[size];

	return m_pWndData;
}

void C_SGUIWND::ReleaseWndData(void)
{
	if (m_pWndData != NULL)
		delete[] m_pWndData;
	m_pWndData = NULL;
}

void *C_SGUIWND::GetFatherWndData(void)
{
	return m_pFatherWndData;
}

void C_SGUIWND::SetFatherWndData(void *pFatherWndData)
{
	m_pFatherWndData = pFatherWndData;
}

int C_SGUIWND::InvalidateRect(const S_RECT *pRect)
{
	S_GUIMSG aMsg;

	/** @brief 防止消息队列中有多个GM_PAINT消息,
	 * 但此种机制还不完善,暂时屏蔽
	 */
//	if (m_InvalidRect.w!=0x00 && m_InvalidRect.h!=0x00)
//		return -1;

	if (pRect == NULL)
		m_InvalidRect = m_WndRect;
	else
		m_InvalidRect = *pRect;

	/** push GM_PAINT msg
	 */
	aMsg.pWnd = this;
	aMsg.Msg = GM_PAINT;
	aMsg.wParam = 0;
	aMsg.lParam = 0;
	SGuiApp->PostGuiMsg(aMsg);

	return 0;
}

C_GUICTRL *C_SGUIWND::GetFocusCtrl(void)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
	{
		C_GUICTRL *pWnd = m_CtrlQ[i];
		S_DWORD flag = pWnd->GetWndLong();
		if (flag & WNDF_FOCUS)
			return pWnd;
	}
	return NULL;
}

C_GUICTRL *C_SGUIWND::SetFocusCtrl(C_GUICTRL *pFocusCtrl)
{
	C_GUICTRL *pCurFocus = GetFocusCtrl();
	if (pCurFocus == pFocusCtrl)
		return pFocusCtrl;
	if (pCurFocus != NULL)
		pCurFocus->SendWndMsg(GM_KILLFOCUS, 0x00, 0x00);
	if (pFocusCtrl != NULL)
		pFocusCtrl->SendWndMsg(GM_SETFOCUS, 0x00, 0x00);
	else
		SendWndMsg(GM_SETFOCUS, 0x00, 0x00);

	InvalidateRect(NULL);
	return pCurFocus;
}

C_GUICTRL *C_SGUIWND::GetWndCtrl(int nID)
{
	if (m_CtrlQ.empty())
		return NULL;
	for (S_DWORD i = 0; i < m_CtrlQ.size(); i++)
	{
		C_GUICTRL *pWnd = m_CtrlQ[i];
		if (pWnd->GetCtrlID() == (S_DWORD)nID)
			return pWnd;
	}
	return NULL;
}

int C_SGUIWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return DefWndProcess(msg, wParam, lParam);
}

int C_SGUIWND::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_QUIT:
		SGuiApp->PopWnd();
		break;
	default:
		return C_GUIWNDB::DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}


//
//
//
/** @brief 单屏及多屏菜单控制结构 */
bool S_MenuScrollCtrl::ScrollCtrl(S_WORD key)
{
	bool fCtrl = false;
	if (key == GVK_RIGHT)
	{
		if (m_StartM + m_LineOfScreen < m_MenuCnt)
		{
			m_StartM += m_LineOfScreen;
			if (m_StartM + m_LineOfScreen > m_MenuCnt)
				m_StartM = m_MenuCnt - m_LineOfScreen;
			fCtrl = true;
		}
	}
	else if (key == GVK_LEFT)
	{
		if (m_StartM > 0)
		{
			m_StartM -= m_LineOfScreen;
			if (m_StartM < 0) m_StartM = 0;
			fCtrl = true;
		}
	}
	else if (key == GVK_UP)
	{
		if (m_SelectM > 0)
			m_SelectM -= 1;
		else if (m_StartM > 0)
			m_StartM -= 1;
		else
		{
			if (m_MenuCnt > m_LineOfScreen) // 多屏处理
			{
				m_StartM = m_MenuCnt - m_LineOfScreen;
				m_SelectM = m_LineOfScreen - 1;
			}
			else // 单屏处理
			{
				m_SelectM = m_MenuCnt - 1;
			}
		}
		fCtrl = true;
	}
	else if (key == GVK_DN)
	{
		if (m_SelectM+1 < m_LineOfScreen && m_SelectM+1 < m_MenuCnt)
			m_SelectM += 1;
		else if (m_StartM + m_LineOfScreen < m_MenuCnt)
			m_StartM += 1;
		else
			m_StartM = 0, m_SelectM = 0;
		fCtrl = true;
	}
	return fCtrl;
}

bool S_MenuScrollCtrl::SetMenuHighlight(int MenuCnt, int MenuID)
{
	m_MenuCnt = MenuCnt;
	if (MenuID >= MenuCnt) // 错误
		m_StartM = 0, m_SelectM = 0;
	else if (MenuID < m_LineOfScreen)
		m_StartM = 0, m_SelectM = MenuID;
	else
		m_StartM = MenuID - m_LineOfScreen, m_SelectM = m_LineOfScreen - 1;
	return true;
}

int S_MenuScrollCtrl::GetSelectMenuID(void)
{
	return m_SelectM + m_StartM;
}

int S_MenuScrollCtrl::GetHighlight(void)
{
	return m_SelectM;
}




