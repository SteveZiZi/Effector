/** @file
 *  @brief 本地操作菜单窗口定义
 *  @author Steven Wan
 *  @date 2012/01/09
 *  @version 0.1
 */
#include "menuwnd.h"
#include "sguiapp.h"
#include "sguictrl.h"
#include "sguidlog.h"
#include "timeop.h"
#include "status.h"
#include "paramwnd.h" //temp yzq add
#include "powerwnd.h"
#include "managwnd.h"
#include "mainwnd.h"
#include "cenwnd.h"
#include "subwnd.h"
#include "surwnd.h"
#include "musicwnd.h"
#include "micwnd.h"
#include "effectwnd.h"

#define MENUCOUNTNUM	11
#define STRUCTCOUNT		4

struct S_UIMENU
{
	const S_CHAR *m_pMenu[MENUCOUNTNUM];
	C_MENUSELECTBOX *m_pSelectBox[MENUCOUNTNUM];
	S_CHAR *m_pValue[MENUCOUNTNUM];
	S_RegSec m_sRegSec[STRUCTCOUNT];
	S_UIMENU *m_pSubMenu[MENUCOUNTNUM];
	S_UIMENU *m_pPMenu;
	S_BYTE m_MenuID;
	S_LONG	m_MenuCount;
};

#define MID_COUNT	20	/**	每级菜单最大ID数*/
#define MID_MKID(lv,off)	(lv*MID_COUNT + off)	/**	根据菜单级数lv和菜单偏移获取菜单ID lv从0开始 off从0开始*/
#define MID_GTLV(id)	((int)(id/MID_COUNT))		/**	根据菜单ID获取菜单的级别*/
#define MID_GTOF(id)	((int)(id%MID_COUNT))		/**	根据菜单ID获取菜单的在子菜单中的偏移*/

//extern S_UIMENU MainMenu;
//S_UIMENU GathQuery =
//{
//	{"1-1 实抄正向有功总电量", "", "", "", "", "", "", ""},
//	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//	&MainMenu, 1
//};
//S_UIMENU ParamQuery =
//{
//	{"2-1 通讯通道设置", "2-2 测量点参数设置", "2-3 终端时间设置", "2-4 界面密码设置", "2-5 终端编号设置", "", "", ""},
//	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//	&MainMenu, 2
//};
//S_UIMENU TermManage =
//{
//	{"3-1 终端版本", "3-2 重启终端", "3-3 告警查询", "3-4 液晶调节", "3-5 信号强度和电池电量", "3-6 无线实时状态", "", ""},
//	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
//	&MainMenu, 3
//};
//S_UIMENU MainMenu =
//{
//	{"1、抄表数据显示", "2、参数设置与查看", "3、终端管理与维护", "", "", "", "", ""},
//	{&GathQuery,&ParamQuery,&TermManage,NULL,NULL,NULL,NULL,NULL},
//	NULL, 0
//};


S_UIMENU MenuThree =
{
	{"Effect Start:","Effect Limit:","Mode:","Reset Param:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","Auto;Manual","M1;M2;M3;M4;M5;M6;M7","","","",""},
	{{0x0501,0x0501,3,1,HEXCODE,0x01,1,0,100,0,0,4},{0x0502,0x0502,3,1,HEXCODE,0x01,1,0,100,0,0,4},{0x1201,0x1201,1,1,SELECTBOXTYPE,0xff,0,0,0,0,0,0},{0xffff,0xfffb,0,0,SELECTBOXTYPE,0xff,0,0,0,0,0,0}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,3
};

S_UIMENU MenuTwo =
{
	{"Music Start:","Music Limit:","Mic Start:","Mic Limit:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","0;","0;","","","",""},
	{{0x0501,0x0501,4,1,HEXCODE,0x01,1,0,100,0,0,4},{0x0502,0x0502,4,1,HEXCODE,0x01,1,0,100,0,0,4},{0x0501,0x0501,2,1,HEXCODE,0x01,1,0,100,0,0,4},{0x0502,0x0502,2,1,HEXCODE,0x01,1,0,100,0,0,4}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MenuThree,2
};

S_UIMENU MenuOne =
{
	{"Recall Param:","Save Param:","Music Input:","Param Start:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"M1;M2;M3;M4;M5;M6;M7","M1;M2;M3;M4;M5;M6;M7","#1;#2","M1;M2;M3;M4;M5;M6;M7","","","",""},
	{{0xffff,0xffff,0,0,SELECTBOXTYPE,0xff,0,0,0,0,0,0},{0xffff,0xfffe,0,0,SELECTBOXTYPE,0xff,0,0,0,0,0,0},{0xffff,0xfffd,0,0,SELECTBOXTYPE,0xff,0,0,0,0,0,0},{0xffff,0xfffc,0,0,SELECTBOXTYPE,0xff,0,0,0,0,0,0}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MenuTwo,1
};



struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	int m_Select;
	int m_TotalM;
};
int C_MENUWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = (S_MENUWDATA *)GetWndData(sizeof(S_MENUWDATA));
	const S_RECT Rect[4] = {{90, 1, 90, 16},{90, 17, 90, 16},{90, 34, 90, 16},{90, 51, 90, 16}};
	//const S_RECT RectRect[4] = {{90, 0, 60, 16},{90, 16, 60, 16},{90, 32, 60, 16},{90, 48, 60, 16}};
	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &MenuOne;
		pData->m_Select = 0;
		pData->m_TotalM = 0;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_MENUSELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MenuTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_MENUSELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MenuThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_MENUSELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MenuOne;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pdc->DrawString(14, i*17+0, pData->m_pCurMenu->m_pMenu[i]);
			pData->m_pCurMenu->m_pSelectBox[i]->Create(pData->m_pCurMenu->m_pValue[i],0x00,Rect[i],this,i);
		}
		//S_RECT rect = {0, pData->m_Select*16+0, 192, 16};
		//pdc->DrawRect(rect);
		
		pdc->DrawString(2,pData->m_Select*16+1,">");
		/*pdc->ReverseRect(rect);
		pdc->ReverseRect(RectRect[pData->m_Select]);
		pdc->DrawHLine(0, 122, 160);*/
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == KEY_DOWN)
		{
			pData->m_Select++;
			if (pData->m_Select >= pData->m_TotalM)
				pData->m_Select = 0;
			//pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_PAINT,0,0);
			InvalidateRect(NULL);
		}
		else if (wParam == KEY_UP)
		{
			pData->m_Select--;
			if (pData->m_Select <= -1)
				pData->m_Select = pData->m_TotalM-1;
			InvalidateRect(NULL);
		}
		else if (wParam == KEY_ESC)
		{
			/*if (pData->m_pCurMenu->m_pPMenu)
			{
				pData->m_Select = pData->m_pCurMenu->m_MenuID - 1; 
				pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
				InvalidateRect(NULL);
			}
			else
			{*/
				RETURNWND();
				pData->m_Select = 0;
			/*}*/
		}
		else if (wParam == KEY_MENU)
		{
			pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_KILLFOCUS,0x00,0x00);
			if (pData->m_pCurMenu->m_pSubMenu[pData->m_Select] != NULL)
			{
				pData->m_pCurMenu = pData->m_pCurMenu->m_pSubMenu[pData->m_Select];
				pData->m_Select = 0;
				InvalidateRect(NULL);
			}
			else
			{
				pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
				if (pData->m_pCurMenu == NULL)
				{
					pData->m_pCurMenu = &MenuOne;
				}
				InvalidateRect(NULL);
#if 0			
				//抄表数据显示
				if (pData->m_pCurMenu->m_MenuID == 1 && pData->m_Select == 0)
					GOTOWND(C_PowerZXYGWnd, 0x00);//可实时抄读各测量点正向有功总电量
				//参数设置与查看
				else if (pData->m_pCurMenu->m_MenuID == 2 && pData->m_Select == 0)
					GOTOWND(C_CommunMenuWnd, 0x00);//通讯通道设置
				else if (pData->m_pCurMenu->m_MenuID == 2 && pData->m_Select == 1)
					GOTOWND(C_MeterParamViewWnd, 0x00);//测量点参数设置
				else if (pData->m_pCurMenu->m_MenuID == 2 && pData->m_Select == 2)
					GOTOWND(C_TerminalTimeViewWnd, 0x00);//终端时间设置
				else if (pData->m_pCurMenu->m_MenuID == 2 && pData->m_Select == 3)
					GOTOWND(C_PasswordWnd, 0x00);//界面密码设置
				else if (pData->m_pCurMenu->m_MenuID == 2 && pData->m_Select == 4)
					GOTOWND(C_RtuaViewWnd, 0x00);//终端编号设置
				//终端管理与维护
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 0)
					GOTOWND(C_ShowVersionWnd, 0x00);//终端版本	可显示终端软、硬件版本等
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 1)
					GOTOWND(C_RebootWnd, 0x00);//重启终端	重新启动终端
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 2)
					GOTOWND(C_QueryAlarmWnd, 0x00);//告警查询 包括告警代码、中文显示事件内容、发生时间等）
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 3)
					GOTOWND(C_ScreenParamViewWnd, 0x00);//液晶调节	可调节终端液晶显示对比度、背光亮度等
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 4)
					GOTOWND(C_SignalAndBatteryWnd, 0x00);//信号强度和电池电量
				else if (pData->m_pCurMenu->m_MenuID == 3 && pData->m_Select == 5)
					GOTOWND(C_WirelessWnd, 0x00);//无线实时状态	可提供终端连接无线网络的实时状态信息
#endif			
			}
		}
		else if (wParam == KEY_SET)
		{
			//SetFocusCtrl(pData->m_pCurMenu->m_pSelectBox[pData->m_Select]);
			pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_SETFOCUS, 0x00, 0x00);
		}
		else if(wParam == KEY_MUSIC_LEFT)
		{
			char data = 0;
			C_LcdDataIface::ReadMusicData(&data);
			int intData = data;
			intData -= 1 * (int)lParam;
			if (intData < 0)
			{
				intData = 0;
			}
			data = intData;
			C_LcdDataIface::WriteMusicData(&data);
		}
		else if (wParam == KEY_MUSIC_RIGHT)
		{
			char data = 0;
			C_LcdDataIface::ReadMusicData(&data);
			int intData = data;
			intData += 1 * (int)lParam;
			if (intData > 100)
			{
				intData = 100;
			}
			data = intData;
			C_LcdDataIface::WriteMusicData(&data);
		}
		else if (wParam == KEY_MIC_LEFT)
		{
			char data = 0;
			C_LcdDataIface::ReadMicData(&data);
			int intData = data;
			intData -= 1 * (int)lParam;
			if (intData < 0)
			{
				intData = 0;
			}
			data = intData;
			C_LcdDataIface::WriteMicData(&data);
		}
		else if (wParam == KEY_MIC_RIGHT)
		{
			char data = 0;
			C_LcdDataIface::ReadMicData(&data);
			int intData = data;
			intData += 1 * (int)lParam;
			if (intData > 100)
			{
				intData = 100;
			}
			data = intData;
			C_LcdDataIface::WriteMicData(&data);
		}
		else if (wParam == KEY_EFFECT_LEFT)
		{
			char data = 0;
			C_LcdDataIface::ReadEffectData(&data);
			int intData = data;
			intData -= 1 * (int)lParam;
			if (intData < 0)
			{
				intData = 0;
			}
			data = intData;
			C_LcdDataIface::WriteEffectData(&data);
		}
		else if (wParam == KEY_EFFECT_RIGHT)
		{

			char data = 0;
			C_LcdDataIface::ReadEffectData(&data);
			int intData = data;
			intData += 1 * (int)lParam;
			if (intData > 100)
			{
				intData = 100;
			}
			data = intData;
			C_LcdDataIface::WriteEffectData(&data);
		}
		else if (wParam == KEY_SUB)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_SUBWND,0);
		}
		else if (wParam == KEY_CEN)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_CENWND,0);
		}
		else if (wParam == KEY_SUR)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_SURWND,0);
		}
		else if (wParam == KEY_MIC)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MICWND,0);
		}
		else if (wParam == KEY_MUSIC)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MUSICWND,0);
		}
		else if (wParam == KEY_EFFECT)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_EFFECTWND,0);
		}
		else if (wParam == KEY_MAIN)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MAINWND,0);
		}
		else if (wParam == KEY_ESC)
		{
			
		}
		break;
	case GM_DESTROY:
		ReleaseWndData();
		C_PWD::SetCertification(false);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

//
//以下为demo，不须要请注释
//
int C_CTRLWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {10, 20, 34, 16};
//		S_RECT rect2 = {50, 20, 34, 16};
		S_RECT rect3 = {90, 20, 54, 18};
		C_NUMEDIT *pNumEdit1 = new C_NUMEDIT;
//		C_NUMEDIT *pNumEdit2 = new C_NUMEDIT;
		C_SELECTBOX *pSelectBox = new C_SELECTBOX;
		pNumEdit1->Create("89ABC", NUMEDIT_STYLE_HEX, rect1, this, 1);
//		pNumEdit2->Create("89ABC", 0x00, rect2, this, 2);
		pSelectBox->Create("89;ABC;我是谁", 0x00, rect3, this, 3);
		SetFocusCtrl(pNumEdit1);
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		delete (GetWndCtrl(1));
//		delete (GetWndCtrl(2));
		delete (GetWndCtrl(3));
		break;
	case GM_KEYUP:
		if (wParam == GVK_LEFT)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_RIGHT)
			SetFocusCtrl(GetWndCtrl(3));
		else if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK)
			SGui_MsgBox("提示", "  消息框弹出了！", 0x00);
		break;
	case GM_CMD:
		if (wParam == 3 && lParam == GM_CMD_CTRL_UPDATE)
		{
			char buffer[32];
			sprintf(buffer, "选择了%d", lParam);
			SGui_MsgBox("提示", buffer, 0x00);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MSGBOXWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		break;
	case GM_KEYUP:
		if (wParam == GVK_UP)
			SGui_MsgBox("提示", "  消息框弹出了！", MB_BTN_OK);
		else if (wParam == GVK_DN)
			SGui_MsgBox("提示", "  消息框弹出了！", MB_BTN_OKCANCEL);
		else if (wParam == GVK_CANCEL)
			RETURNWND();
		else if (wParam == GVK_OK)
			SGui_MsgBox("提示", "  消息框弹出了！", 0x00);
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_MSGSENDWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {60, 18, 70, 16};
		S_RECT rect2 = {10, 70, 34, 16};
		S_RECT rect3 = {10, 50, 140, 16};
		C_NUMEDIT *pNumEdit1 = new C_NUMEDIT;
		C_VKBOARD *pVk = new C_VKBOARD;
		C_TEXTEDIT *pEdit = new C_TEXTEDIT;
		pNumEdit1->Create("13560303396", NUMEDIT_STYLE_DEC, rect1, this, 1);
		pVk->Create(0x00, rect2, this, 2);
		pEdit->Create("                     ", 0x00, rect3, this, 3);
		SetFocusCtrl(pNumEdit1);
		}break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(10, 2, "虚拟键盘的\"Ent\"发送消息");
		pdc->DrawString(10, 20, "手机号:");
		pdc->DrawString(10, 36, "发送内容:");
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		delete (GetWndCtrl(1));
		delete (GetWndCtrl(2));
		delete (GetWndCtrl(3));
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
			SetFocusCtrl(GetWndCtrl(2));
		break;
	case GM_CHAR:
		if (wParam == GVK_ENTER)
		{
			string msg;
			C_GUICTRL *pCtrl = GetWndCtrl(1);
			msg.append(pCtrl->GetWndText());
			pCtrl = GetWndCtrl(3);
			msg.append(pCtrl->GetWndText());
//			C_SYSIPC::Send2Server(HDJS_SERID_PPP, C_SYSIPC::SYSIPC_TYPE_MSG_SEND, msg);
		}
		else if (wParam == GVK_ESC)
			SetFocusCtrl(GetWndCtrl(1));
		else if (wParam == GVK_F1)
			RETURNWND();
		else
		{
			C_GUICTRL *pCtrl = GetWndCtrl(3);
			pCtrl->SendWndMsg(GM_CHAR, wParam, lParam);
		}
		break;
	case GM_TIME:{
//		int SerID;S_BYTE MsgT;
		string msg;
//		if ((C_SYSIPC::Query4Server(SerID, MsgT, msg) >= 0) && MsgT == C_SYSIPC::SYSIPC_TYPE_MSG_SEND)
		{
			string strDA, strUD;
//			strDA.append(msg.begin(), msg.begin()+14);
//			strUD.assign(msg.begin()+14, msg.end());
//			SGui_MsgBox(strDA.c_str(), strUD.c_str(), MB_BTN_OK);
		}
		}break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_VKWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	switch (msg)
	{
	case GM_CREATE:{
		S_RECT rect1 = {10, 70, 34, 16};
		S_RECT rect2 = {12, 10, 134, 16};
		C_VKBOARD *pVk = new C_VKBOARD;
		C_TEXTEDIT *pEdit = new C_TEXTEDIT;
		pVk->Create(0x00, rect1, this, 1);
		pEdit->Create("          ", 0x00, rect2, this, 2);
		SetFocusCtrl(pVk);
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		delete (GetWndCtrl(1));
		delete (GetWndCtrl(2));
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			SetFocusCtrl(GetWndCtrl(1));
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
	case GM_CHAR:
		if (wParam == GVK_ENTER)
		{
		}
		else if (wParam == GVK_ESC)
		{
			SetFocusCtrl(GetWndCtrl(2));
		}
		else if (wParam == GVK_F1)
		{
		}
		else
		{
			C_GUICTRL *pCtrl = GetWndCtrl(2);
			pCtrl->SendWndMsg(GM_CHAR, wParam, lParam);
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

int C_FONTWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	static char FontShow[2] = " ";
	switch (msg)
	{
	case GM_CREATE:{
		}
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		pdc->DrawString(10, 10, FontShow);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM16x16));
		pdc->DrawString(30, 10, FontShow);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_SYM08x16));
		pdc->DrawString(50, 10, FontShow);
		EndPaint(pdc);
		}
		break;
	case GM_DESTROY:
		break;
	case GM_KEYUP:
		if (wParam == GVK_OK)
		{
			FontShow[0] += 1;
			if (FontShow[0] > '~')
				FontShow[0] = ' ';
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
		}
		break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}


