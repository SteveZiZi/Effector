/** @file
 *  @brief 本地操作父窗口定义
 *  @author Steven Wan
 *  @date 2012/01/09
 *  @version 0.1
 *  用户窗口必须继承C_UIMWND
 */
#include "uiwnd.h"
#include "timeop.h"
#include "context.h"
#include "status.h"
#include "modem.h"
#include "sguictrl.h"

//sym16x16 符号
const char *s_pProgramKeyState = " p";
const char *s_pUsbDiskPlugin = " [";
const char *s_pGprsSingle = "#$%&`";
const char *s_pGprsSingle_G = "()*+,";
const char *s_pGprsSingle_C = "-./01";
const char *s_pGprsSingle_L = "23456";
const char *s_pCommStatus_T = "?@ABCDEFG";
const char *s_pCommStatus = "HI";
const char *s_pLittleBattery = "JKLMNO";
const char *s_pBitBattery = "7>8>9>:;:<:=";
//sym8x16 符号
const char *s_pLeftPaneDigit = "&'()*+,-./";
const char *s_pRightPaneDigit = "0123456789";
const char *s_pLeftBlackPaneDigit = ":;<=>?@ABC";
const char *s_pRightBlackPaneDigit = "DEFGHIJKLM";


int C_UIMWND::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	//int sn;
	//char AsciiBuffer[32];
	//char AsciiEvent[12];
	//char AsciiMP[12];
	switch (msg)
	{
	case GM_SYSPAINT:{
#if 0
		S_DATE date;
		C_SGUIDC dc(NULL);
		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		sn = GetSignal(), AsciiBuffer[0] = *(s_pGprsSingle + sn);
		sn = GetCommunicationStatus(), AsciiBuffer[1] = *(s_pCommStatus_T + sn);
		sn = GetEventIndicator();
		if (sn < 0) AsciiBuffer[2] = '!';
		else AsciiBuffer[2] = ' ';
		if (sn > 0) AsciiEvent[0] = *(s_pLeftBlackPaneDigit+sn/10%10), AsciiEvent[1] = *(s_pRightBlackPaneDigit+sn%10);
		else AsciiEvent[0] = ' ', AsciiEvent[1] = ' ';
		AsciiEvent[2] = '\0';

		sn = GetBatteryLevel(), AsciiBuffer[3] = *(s_pLittleBattery + sn);
		AsciiBuffer[4] = '\0';

		sn = GetDisplayedMP();
		if (sn > 0) AsciiMP[0] = *(s_pLeftPaneDigit+sn/10%10), AsciiMP[1] = *(s_pRightPaneDigit + sn%10);
		else AsciiMP[0] = ' ', AsciiMP[1] = ' ';
		AsciiMP[2] = '\0';

		bool en = dc.FlushScreenEn(false);
		dc.SelectObject(dc.GetStockGuiObj(SGUI_OBJ_SYM16x16));
		dc.DrawString(2, 1, AsciiBuffer);
		dc.SelectObject(dc.GetStockGuiObj(SGUI_OBJ_SYM08x16));
		if (AsciiEvent[0] != ' ')
            dc.DrawString(2+16*2, 1, AsciiEvent);
		dc.DrawString(2+16*5, 1, AsciiMP);
		sprintf(AsciiBuffer, "%02d:%02d", date.m_Hour, date.m_Min);
		dc.SelectObject(dc.GetStockGuiObj(SGUI_OBJ_F16x16));
		dc.DrawString(116, 1, AsciiBuffer);
		dc.DrawHLine(0, 18, 160);
		dc.FlushScreenEn(en);
		dc.FlushScreen();
#endif
		}
		break;
	default:
		return C_SGUIWND::DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

/** @brief 无线信号获取
 *  @return 0-4 
 */
int C_UIMWND::GetSignal(void)
{
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	if (pPppStatus->m_Signal == 0)
		return 0;
    S_BYTE level = pPppStatus->m_Signal / 8 + 1;
	if (level > 4) level = 4;
	return level;
}

/** @brief 通讯状态获取
 *  @return 0-2 GprsStatus 0未登陆1空闲2通讯
 *  @return 3-5 CdmaStatus 3未登陆4空闲5通讯
 *  @return 6-7 LanStatus 6未登陆7空闲8通讯
 */
int C_UIMWND::GetCommunicationStatus(void)
{
	int status;
	S_PppStatus *pPppStatus = CONTEXT.GetPppStatus();
	S_ConnectStatus *pCS = CONTEXT.GetConnectStatus();
	if (pCS->m_HttxNeedPPP[MP_HTTX_1] == false)
		status = 6;
	else if (pPppStatus->m_ModemType < MODEM_TYPE_GPRS_MAX)
		status = 0;
	else if (pPppStatus->m_ModemType < MODEM_TYPE_CDMA_MAX)
		status = 3;
	else
		status = 0;//default GPRS
	if (pCS->m_Httx1Logon == true)
	{
		status += 1;
		if (C_TIME::TimeNow() - pCS->m_CommunicationTime <= 30)
			status += 1;
	}

	return status;
}

/** @brief 事件状态获取
 *  @return -1 显示感冒号
 *  @return 0 无显示
 *  @return >0 事件编码
 */
int C_UIMWND::GetEventIndicator(void)
{
/*	S_AlarmStatus *pAS = CONTEXT.GetAlarmStatus();
	if (pAS->m_EventNewCnt > 0 || pAS->m_ImportantNewCnt > 0 || pAS->m_NormalNewCnt > 0)
		return -1;*/
	return 0;
}

/** @brief 电池级别获取
 *  @return 0-5
 */
int C_UIMWND::GetBatteryLevel(void)
{
/*	static S_BYTE LastLevel = 5;
	S_PowerStatus *pPowerStatus = CONTEXT.GetPowerStatus();
	if (pPowerStatus->m_fCharge == true && LastLevel < 5)//冲电中，动画显示
		return LastLevel++;

	S_BYTE level = 0;
	if (pPowerStatus->m_BatteryCap > 10)
	{//取实际电量级别
		level = pPowerStatus->m_BatteryCap / 20 + 1;
		if (level > 5) level = 5;
	}

	if (pPowerStatus->m_fCharge == true)
	{//冲电处理
		if (level > 0) level -= 1;//减一格，以便动画显示
		LastLevel = level;
	}

	return level;*/
	return 0;
}

/** @brief 取当前显示信息的测量点号
 *  @return 0 无
 *  @return >0 正常测量点
 */
int C_UIMWND::GetDisplayedMP(void)
{
	return 0;
}

/** @brief delete 控件
 *  @param[in] idMin 最小控件ID号
 *  @param[in] idMax 最大控件ID号
 *  @return 0
 *  @note 控件必需是用new创建的
 */
int C_UIMWND::DeleteWndCtrl(int idMin, int idMax)
{
	for (; idMin <= idMax; idMin++)
	{
		delete GetWndCtrl(idMin);
	}
	return 0;
}

/** @brief 密码验证框
 */
 #define PWDDLAG_CTRLID_NUMEDIT		1
 #define PWDDLAG_CTRLID_BUTTON_OK	2
 #define PWDDLAG_CTRLID_BUTTON_CAN	3
 #define PWDDLAG_DELAY_MAXMS		(30*GM_TIME_INTERVAL_MS)

C_PWDDLOG::C_PWDDLOG(int pwd)
{
	m_pwd = pwd;
	m_DelayMS = 0;
}

C_PWDDLOG::~C_PWDDLOG()
{
}

int C_PWDDLOG::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		m_WndRect.x = 20;
		m_WndRect.w = 120;
		m_WndRect.y = 50, m_WndRect.h = 80;
		CreateCtrl();
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		S_RECT WndRect = {0, 0, m_WndRect.w, m_WndRect.h};
		pdc->DrawRect(WndRect);
		pdc->DrawHLine(1, 16, m_WndRect.w-2);
		pdc->DrawString(2, 2, "请输入密码");
		EndPaint(pdc);
		}	
		break;
	case GM_KEYUP:
		m_DelayMS = 0; ///退出延时重新计数
		if (wParam == GVK_OK || wParam == GVK_DN)
			SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_OK));
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_NUMEDIT));
		else if (wParam == GVK_LEFT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == PWDDLAG_CTRLID_BUTTON_CAN)
				SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_OK));
		}
		else if (wParam == GVK_RIGHT)
		{
			C_GUICTRL *pCtrl = GetFocusCtrl();
			if (pCtrl->GetCtrlID() == PWDDLAG_CTRLID_BUTTON_OK)
				SetFocusCtrl(GetWndCtrl(PWDDLAG_CTRLID_BUTTON_CAN));
		}
		break;
	case GM_CMD:
		m_DelayMS = 0; ///退出延时重新计数
		if (wParam == PWDDLAG_CTRLID_BUTTON_OK && lParam == GM_CMD_BTN_PUSHED)//OK
		{
			C_GUICTRL *pNum = GetWndCtrl(PWDDLAG_CTRLID_NUMEDIT);
			string strNum = pNum->GetWndText();
			int CurPwd = atoi(strNum.c_str());
			if (CurPwd == m_pwd)
			{
				EndDlg();
				return MB_RTN_YESOK;
			}
			C_SGUIDC dc(this);
			dc.DrawString(16, 44, "密码验证失败!!!");
		}
		else if (wParam == PWDDLAG_CTRLID_BUTTON_CAN && lParam == GM_CMD_BTN_PUSHED)//Cancel
		{
			EndDlg();
			return MB_RTN_CANCEL;
		}
		break;
	case GM_TIME:
		m_DelayMS += GM_TIME_INTERVAL_MS;
		if (m_DelayMS > PWDDLAG_DELAY_MAXMS)
		{
			EndDlg();
			return MB_RTN_NULL;
		}
		break;
	case GM_DESTROY:
		DestroyCtrl();
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_PWDDLOG::DefWndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	return C_SGUIDLG::DefWndProcess(msg, wParam, lParam);
}

int C_PWDDLOG::CreateCtrl(void)
{
	S_RECT NumRect = {35, 22, 50, 16};
	C_NUMEDIT *pNumEdit = new C_NUMEDIT;
	pNumEdit->Create("999999", NUMEDIT_STYLE_DEC, NumRect, this, PWDDLAG_CTRLID_NUMEDIT);
	
	C_BUTTON *pbtnOK = new C_BUTTON;
	C_BUTTON *pbtnCancel = new C_BUTTON;
	S_RECT rect;
	rect.w = 32; rect.h = 16;
	rect.x = (m_WndRect.w - (rect.w + rect.w + 10)) / 2;
	rect.y = 60;
	pbtnOK->Create("确定", 0x00, rect, this, PWDDLAG_CTRLID_BUTTON_OK);
	rect.x += rect.w + 10;
	pbtnCancel->Create("取消", 0x00, rect, this, PWDDLAG_CTRLID_BUTTON_CAN);

	SetFocusCtrl(pNumEdit);
	return 0;
}

int C_PWDDLOG::DestroyCtrl(void)
{
	delete (GetWndCtrl(PWDDLAG_CTRLID_NUMEDIT));
	delete (GetWndCtrl(PWDDLAG_CTRLID_BUTTON_OK));
	delete (GetWndCtrl(PWDDLAG_CTRLID_BUTTON_CAN));
	return 0;
}


/** @brief 密码验证
 */
bool C_PWD::m_fCertification = false;

/** @brief 密码验证
 *  @return true 验证通过;false 未通过
 *  @remarks 消息框和对话框不能调用本函数
 */
bool C_PWD::Certification(void)
{
	if (m_fCertification == true)
		return true;
	S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
	C_PWDDLOG PwdDlog(pLcdInfo->m_LCDPassword);
	if (PwdDlog.DoModel() == MB_RTN_YESOK)
	{
		m_fCertification = true;
		return true;
	}
	return false;
}

/** @brief 设置密码验证标志
 */
void C_PWD::SetCertification(bool fOk)
{
	m_fCertification = fOk;
}

