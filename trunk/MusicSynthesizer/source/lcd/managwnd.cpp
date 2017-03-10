/** @file
 *  @brief 终端管理与维护窗口定义
 *  @author Steven Wan yezhengqiu
 *  @date 2012/02/21
 *  @version 0.1
 */
#include "sguiapp.h"
#include "managwnd.h"
#include "version.h"
#include "syscfg.h"
#include "sysctrl.h"
#include "context.h"
#include "status.h"
#include "modem.h"
#include "sguictrl.h"

const char *g_TermName[] ={"厂站终端","专变终端","公变终端","集中器终端"};

//液晶调节	可调节终端液晶显示对比度、背光亮度等
int C_ScreenParamViewWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	switch (msg)
	{
	case GM_CREATE:{
		C_BUTTON *pbtnSet = new C_BUTTON;
		S_RECT rect = {60, LINEXY(5), 32, 16};
		pbtnSet->Create("设置", 0x00, rect, this, 1);
		SetFocusCtrl(pbtnSet);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		S_LcdInfo *pInfo = STATUS.GetLcdInfo();
		sprintf(buffer, "液晶显示对比度:  %d", pInfo->m_LCDGreyScale);
		pdc->DrawString(STRINGX, LINEXY(0), buffer);
		pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
			RETURNWND();
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_BTN_PUSHED)
		{
			if (C_PWD::Certification() == true)
				GOTOWND(C_ScreenParamSetWnd, 0x00);
		}
		break;
	case GM_DESTROY:
		DeleteWndCtrl(1, 1);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}


//液晶调节	可调节终端液晶显示对比度、背光亮度等
const char *ScreenParamSetInfo[] = {"                ", "显示参数设置成功"};
int C_ScreenParamSetWnd::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	char buffer[64];
	S_MenuCtrl *pData = static_cast<S_MenuCtrl *>(GetWndData(sizeof(S_MenuCtrl)));
	switch (msg)
	{
	case GM_CREATE:{
		pData->m_InfoIndex = 0;
		S_LcdInfo *pInfo = STATUS.GetLcdInfo();
		S_RECT NumRect = {96, 1*20+6, 18, 16};
		C_NUMEDIT *pContrastEdit = new C_NUMEDIT;
		sprintf(buffer, "%02d", pInfo->m_LCDGreyScale);
		pContrastEdit->Create(buffer, NUMEDIT_STYLE_DEC, NumRect, this, 1);
		C_BUTTON *pbtnSet = new C_BUTTON;
		S_RECT SetRect = {60, 90, 32, 16};
		pbtnSet->Create("确定", 0x00, SetRect, this, 2);
		SetFocusCtrl(pContrastEdit);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F16x16));
		pdc->DrawString(STRINGX, 0*20+8, "液晶调节");
		pdc->DrawString(STRINGX, 1*20+8, "显示对比度:");
		pdc->DrawHLine(0, 122, 160);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		pdc->DrawString(20, 126, ScreenParamSetInfo[pData->m_InfoIndex]);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_CANCEL)
		{
			S_LcdInfo *pInfo = STATUS.GetLcdInfo();
			SGUI_contrast(pInfo->m_LCDGreyScale);
			RETURNWND();
		}
		else if (wParam == GVK_UP)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_OK)
			SetFocusCtrl(GetWndCtrl(2));
		break;
	case GM_CMD:
		if (wParam == 1 && lParam == GM_CMD_CTRL_UPDATE)
		{
			string strContrast = GetWndCtrl(1)->GetWndText();
			int contrast = atoi(strContrast.c_str());
			SGUI_contrast(contrast);
		}
		else if (wParam == 2 && lParam == GM_CMD_BTN_PUSHED)
		{
			string strContrast = GetWndCtrl(1)->GetWndText();
			int contrast = atoi(strContrast.c_str());
			sprintf(buffer, " 确定对比度改为:%d", contrast);
			if (SGui_MsgBox("提示", buffer, MB_BTN_OKCANCEL) == MB_RTN_YESOK)
			{
				S_LcdInfo *pInfo = STATUS.GetLcdInfo();
				pInfo->m_LCDGreyScale = contrast;
//				STATUS.SaveLcdInfo();
				pData->m_InfoIndex = 1;
			}
			else
			{
				pData->m_InfoIndex = 0;
				SetFocusCtrl(GetWndCtrl(1));
			}
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		DeleteWndCtrl(1, 2);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;	
}


