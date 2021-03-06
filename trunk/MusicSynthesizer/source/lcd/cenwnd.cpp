/** @file
 *  @brief 本地操作中置菜单窗口定义
 *  @author MSZ
 *  @date 2015/05/30
 *  @version 0.1
 */
#include "sguiapp.h"
#include "sguictrl.h"
#include "sguidlog.h"
#include "timeop.h"
#include "status.h"
#include "managwnd.h"
#include "cenwnd.h"
#include "mainwnd.h"
#include "menuwnd.h"
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
	C_SELECTBOX *m_pSelectBox[MENUCOUNTNUM];
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


S_UIMENU CenMenuNigh =
{
	{"Mute :","Polarity :","Volume:","Delay:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"No;Yes","No;Yes","0;","0;","","","",""},
	{{0x0601,0x5151,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,4},{0x0601,0x5151,3,1,SELECTBOXTYPE,0x05,0,0,0,0,0,4},{0x0601,0x5151,4,1,HEXCODE,0x05,1,0,100,0,0,4},{0x1003,0xB151,3,3,BCDCODE,0x05,1,0,80,2,3,3}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,9
};

S_UIMENU CenMenuEight =
{
	{"Limit Level:","Attack Time:","Release Time:","Release Rate:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"-40;","0.5;","10;","1.2;","","","",""},
	{{0x0301,0x2161,2,2,BCDCODE,0x05,0.1,-40,20,3,1,10},{0x0301,0x2161,6,2,BCDCODE,0x05,0.5,0.5,50,2,1,10},{0x0301,0x2161,8,2,BCDCODE,0x05,10,10,1000,4,0,10},{0x0301,0x2161,4,2,BCDCODE,0x05,1,1.2,20,2,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuNigh,8
};

S_UIMENU CenMenuSeven =
{
	{"No.5 Type:","No.5 Freq:","No.5 Q:","No.5 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0105,0x0561,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,10},{0x0105,0x0561,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,10},{0x0105,0x0561,8,2,BCDCODE,0x05,1,0.4,15,3,1,10},{0x0105,0x0561,6,2,BCDCODE,0x05,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuEight,7
};

S_UIMENU CenMenuSix =
{
	{"No.4 Type:","No.4 Freq:","No.4 Q:","No.4 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0104,0x0461,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,10},{0x0104,0x0461,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,10},{0x0104,0x0461,8,2,BCDCODE,0x05,1,0.4,15,3,1,10},{0x0104,0x0461,6,2,BCDCODE,0x05,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuSeven,6
};

S_UIMENU CenMenuFive =
{
	{"No.3 Type:","No.3 Freq:","No.3 Q:","No.3 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0103,0x0361,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,10},{0x0103,0x0361,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,10},{0x0103,0x0361,8,2,BCDCODE,0x05,1,0.4,15,3,1,10},{0x0103,0x0361,6,2,BCDCODE,0x05,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuSix,5
};

S_UIMENU CenMenuFour =
{
	{"No.2 Type:","No.2 Freq:","No.2 Q:","No.2 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0102,0x0261,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,10},{0x0102,0x0261,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,10},{0x0102,0x0261,8,2,BCDCODE,0x05,1,0.4,15,3,1,10},{0x0102,0x0261,6,2,BCDCODE,0x05,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuFive,4
};

S_UIMENU CenMenuThree =
{
	{"No.1 Type:","No.1 Freq:","No.1 Q:","No.1 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0101,0x0161,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,10},{0x0101,0x0161,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,10},{0x0101,0x0161,8,2,BCDCODE,0x05,1,0.4,15,3,1,10},{0x0101,0x0161,6,2,BCDCODE,0x05,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuFour,3
};

S_UIMENU CenMenuTwo =
{
	{"HPF:","HPF Type:","LPF:","LPF Type:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"19.7;","Buttworther24db;Linkw-riley24db;Bessel24db","19.7;","Buttworther24db;Linkw-riley24db;Bessel24db","","","",""},
	{{0x0201,0x1171,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,5},{0x0201,0x1171,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,5},{0x0202,0x1271,3,3,BCDCODE,0x05,1,19.7,21772.6,5,1,5},{0x0202,0x1271,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,5}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuThree,2
};

S_UIMENU CenMenuOne =
{
	{"Music Level:","Mic Level:","Effect level:","EQ Pass:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","0;","No;Yes","","","",""},
	{{0x0505,0x0505,3,1,HEXCODE,0xff,1,0,100,0,0,8},{0x0503,0x0503,3,1,HEXCODE,0xff,1,0,100,0,0,8},{0x0504,0x0504,3,1,HEXCODE,0xff,1,0,100,0,0,8},{0x1101,0xA161,2,1,SELECTBOXTYPE,0x05,0,0,0,0,0,4}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&CenMenuTwo,1
};

struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	int m_Select;
	int m_TotalM;
};
int C_CENWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = (S_MENUWDATA *)GetWndData(sizeof(S_MENUWDATA));
	const S_RECT Rect[4] = {{95, 1, 85, 16},{95, 17, 85, 16},{95, 34, 85, 16},{95, 51, 85, 16}};
	//const S_RECT RectRect[4] = {{95, 0, 60, 16},{95, 16, 60, 16},{95, 32, 60, 16},{95, 48, 60, 16}};
	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &CenMenuOne;
		pData->m_Select = 0;
		pData->m_TotalM = 0;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuFour;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuFive;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuSix;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuSeven;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuEight;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuNigh;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &CenMenuOne;
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
		//pdc->DrawHLine(0, 122, 160);
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
			RETURNWND();
			pData->m_Select = 0;
		}
		else if (wParam == KEY_CEN)
		{
			pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_KILLFOCUS,0x00,0x00);
			pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
			if (pData->m_pCurMenu == NULL)
			{
				pData->m_pCurMenu = &CenMenuOne;
			}
			InvalidateRect(NULL);
		}
		else if (wParam == KEY_SET)
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
		else if (wParam == KEY_MAIN)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MAINWND,0);
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
		else if (wParam == KEY_MENU)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MENUWND,0);
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


