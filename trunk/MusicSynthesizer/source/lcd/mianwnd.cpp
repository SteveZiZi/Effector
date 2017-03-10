/** @file
 *  @brief 本地操作主菜单窗口定义
 *  @author MSZ
 *  @date 2015/06/01
 *  @version 0.1
 */
#include "sguiapp.h"
#include "sguictrl.h"
#include "sguidlog.h"
#include "timeop.h"
#include "status.h"
#include "managwnd.h"
#include "mianwnd.h"

#define MENUCOUNTNUM	11
struct S_UIMENU
{
	const S_CHAR *m_pMenu[MENUCOUNTNUM];
	C_SELECTBOX *m_pSelectBox[MENUCOUNTNUM];
	S_CHAR *m_pValue[MENUCOUNTNUM];
	S_UIMENU *m_pSubMenu[MENUCOUNTNUM];
	S_UIMENU *m_pPMenu;
	S_BYTE m_MenuID;
	S_LONG	m_MenuCount;
};

#define MID_COUNT	20	/**	每级菜单最大ID数*/
#define MID_MKID(lv,off)	(lv*MID_COUNT + off)	/**	根据菜单级数lv和菜单偏移获取菜单ID lv从0开始 off从0开始*/
#define MID_GTLV(id)	((int)(id/MID_COUNT))		/**	根据菜单ID获取菜单的级别*/
#define MID_GTOF(id)	((int)(id%MID_COUNT))		/**	根据菜单ID获取菜单的在子菜单中的偏移*/


S_UIMENU MainMenuSix =
{
	{"Mute L:","Mute R:","L Delay:","R Delay:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"No;Yes","No;Yes","00ms;10ms;20ms;30ms;40ms;50ms","00ms;10ms;20ms;30ms;40ms;50ms","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,6
};

S_UIMENU MainMenuFive =
{
	{"Limit Level:","Attack Time:","Release Time:","Release Ratio:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0db;1db;2db;3db;4db;5db","0.5ms;1ms;2ms;5ms;10ms","10ms;20ms;50ms;100ms","1.2;1.5;2;3;4;6;10","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuSix,5
};

S_UIMENU MainMenuSubThree =
{
	{"HS:","Frequency:","Q:","Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"1;2;3;4;5","20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","0.4;0.5;1.0;1.5;2.0;10;15;50;128","-15db;-12db;-9db;-6db;-3db;0db;3db;6db;9db;12db;15db","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,9
};

S_UIMENU MainMenuSubTwo =
{
	{"LS:","Frequency:","Q:","Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"1;2;3;4;5","20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","0.4;0.5;1.0;1.5;2.0;10;15;50;128","-15db;-12db;-9db;-6db;-3db;0db;3db;6db;9db;12db;15db","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuSubThree,8
};

S_UIMENU MainMenuSubOne =
{
	{"PEQ:","Frequency:","Q:","Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"1;2;3;4;5","20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","0.4;0.5;1.0;1.5;2.0;10;15;50;128","-15db;-12db;-9db;-6db;-3db;0db;3db;6db;9db;12db;15db","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuSubTwo,7
};

S_UIMENU MainMenuFour =
{
	{"PEQ/LS/HS","Frequency:","Q:","Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{";;","20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","0.4;0.5;1.0;1.5;2.0;10;15;50;128","-15db;-12db;-9db;-6db;-3db;0db;3db;6db;9db;12db;15db","","","",""},
	{&MainMenuSubOne,&MainMenuSubTwo,&MainMenuSubThree,NULL,NULL,NULL,NULL,NULL},
	&MainMenuFive,4
};

S_UIMENU MainMenuThree =
{
	{"HPF:","HPF Type:","LPF:","LPF Type:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","Buttworther24db;Linkw-riley24db;Bessel24db","20Hz;50Hz;200Hz;500Hz;1kHz;2kHz;5kHz;10kHz;20kHz","Buttworther24db;Linkw-riley24db;Bessel24db","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuFour,3
};

S_UIMENU MainMenuTwo =
{
	{"Mode:","Style:","EQ Pass:","Linkage:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Audio;Manual","Sing;Disco","Yes;No","Yes;No","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuThree,2
};

S_UIMENU MainMenuOne =
{
	{"Music Level:","Mic Level:","Echo level:","Reverb Level:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"00;10;20;30;40;50;60;70;80;90;100","00;10;20;30;40;50;60;70;80;90;100","00;10;20;30;40;50;60;70;80;90;100","0;1;2;3;4;5;6;7;8;9;10","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MainMenuTwo,1
};

struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	int m_Select;
	int m_TotalM;
};
int C_MAINWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = (S_MENUWDATA *)GetWndData(sizeof(S_MENUWDATA));
	const S_RECT Rect[4] = {{90, 1, 90, 16},{90, 17, 90, 16},{90, 34, 90, 16},{90, 51, 90, 16}};
	//const S_RECT RectRect[4] = {{90, 0, 60, 16},{90, 16, 60, 16},{90, 32, 60, 16},{90, 48, 60, 16}};
	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &MainMenuOne;
		pData->m_Select = 0;
		pData->m_TotalM = 0;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuFour;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuFive;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuSix;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		// 创建子菜单
		pData->m_pCurMenu = &MainMenuSubOne;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuSubTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuSubThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX;
		}
		pData->m_pCurMenu = &MainMenuOne;
		break;
	case GM_PAINT:{
		C_SGUIDC *pdc = BeginPaint();
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pdc->DrawString(14, i*17+0, pData->m_pCurMenu->m_pMenu[i]);
			pData->m_pCurMenu->m_pSelectBox[i]->Create(pData->m_pCurMenu->m_pValue[i],0x00,Rect[i],this,i);
		}
		S_RECT rect = {0, pData->m_Select*16+0, 192, 16};
		pdc->DrawRect(rect);
		//pdc->DrawHLine(0, 122, 160);
		EndPaint(pdc);
		}
		break;
	case GM_KEYUP:
		if (wParam == GVK_DN)
		{
			pData->m_Select++;
			if (pData->m_Select >= pData->m_TotalM)
				pData->m_Select = 0;
			//pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_PAINT,0,0);
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_UP)
		{
			pData->m_Select--;
			if (pData->m_Select <= -1)
				pData->m_Select = pData->m_TotalM-1;
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_CANCEL)
		{
			RETURNWND();
			pData->m_Select = 0;
		}
		else if (wParam == GVK_RIGHT)
		{
			pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
			if (pData->m_pCurMenu == NULL)
			{
				pData->m_pCurMenu = &MainMenuOne;
			}
			InvalidateRect(NULL);
		}
		else if (wParam == GVK_OK)
		{
			if (pData->m_pCurMenu->m_pSubMenu[pData->m_Select] != NULL)
			{
				pData->m_pCurMenu = pData->m_pCurMenu->m_pSubMenu[pData->m_Select];
				pData->m_Select = 0;
				InvalidateRect(NULL);
			}
			else
			{
				//SetFocusCtrl(pData->m_pCurMenu->m_pSelectBox[pData->m_Select]);
				pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_SETFOCUS, 0x00, 0x00);
			}
		}
		else if (wParam == GVK_ESC)
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
