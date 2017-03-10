/** @file
 *  @brief 本地操作麦克风菜单窗口定义
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

S_UIMENU MicMenuSeventeen =
{
	{"Limit Level:","Attack Time:","Release Time:","Release Rate:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"-40;","0.5;","10;","1.2;","","","",""},
	{{0x0301,0x2111,2,2,BCDCODE,0x00,0.1,-40,20,3,1,10},{0x0301,0x2111,6,2,BCDCODE,0x00,0.5,0.5,50,2,1,10},{0x0301,0x2111,8,2,BCDCODE,0x00,10,10,1000,4,0,10},{0x0301,0x2111,4,2,BCDCODE,0x00,1,1.2,20.1,2,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,17
};

S_UIMENU MicMenuSixteen =
{
	{"No.15 Type:","No.15 Freq:","No.15 Q:","No.15 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010F,0x0F11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010F,0x0F11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010F,0x0F11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010F,0x0F11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuSeventeen,16
};

S_UIMENU MicMenuFifteen =
{
	{"No.14 Type:","No.14 Freq:","No.14 Q:","No.14 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010E,0x0E11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010E,0x0E11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010E,0x0E11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010E,0x0E11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuSixteen,15
};

S_UIMENU MicMenuFourteen =
{
	{"No.13 Type:","No.13 Freq:","No.13 Q:","No.13 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010D,0x0D11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010D,0x0D11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010D,0x0D11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010D,0x0D11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuFifteen,14
};

S_UIMENU MicMenuThirteen =
{
	{"No.12 Type:","No.12 Freq:","No.12 Q:","No.12 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010C,0x0C11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010C,0x0C11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010C,0x0C11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010C,0x0C11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuFourteen,13
};

S_UIMENU MicMenuTwelve =
{
	{"No.11 Type:","No.11 Freq:","No.11 Q:","No.11 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010B,0x0B11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010B,0x0B11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010B,0x0B11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010B,0x0B11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuThirteen,12
};

S_UIMENU MicMenuEleven =
{
	{"No.10 Type:","No.10 Freq:","No.10 Q:","No.10 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x010A,0x0A11,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x010A,0x0A11,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x010A,0x0A11,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x010A,0x0A11,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuTwelve,11
};

S_UIMENU MicMenuTen =
{
	{"No.9 Type:","No.9 Freq:","No.9 Q:","No.9 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0109,0x0911,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0109,0x0911,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0109,0x0911,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0109,0x0911,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuEleven,10
};

S_UIMENU MicMenuNigh =
{
	{"No.8 Type:","No.8 Freq:","No.8 Q:","No.8 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0108,0x0811,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0108,0x0811,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0108,0x0811,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0108,0x0811,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuTen,9
};

S_UIMENU MicMenuEight =
{
	{"No.7 Type:","No.7 Freq:","No.7 Q:","No.7 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0107,0x0711,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0107,0x0711,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0107,0x0711,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0107,0x0711,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuNigh,8
};

S_UIMENU MicMenuSeven =
{
	{"No.6 Type:","No.6 Freq:","No.6 Q:","No.6 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0106,0x0611,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0106,0x0611,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0106,0x0611,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0106,0x0611,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuEight,7
};

S_UIMENU MicMenuSix =
{
	{"No.5 Type:","No.5 Freq:","No.5 Q:","No.5 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0105,0x0511,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0105,0x0511,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0105,0x0511,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0105,0x0511,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuSeven,6
};

S_UIMENU MicMenuFive =
{
	{"No.4 Type:","No.4 Freq:","No.4 Q:","No.4 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0104,0x0411,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0104,0x0411,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0104,0x0411,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0104,0x0411,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuSix,5
};

S_UIMENU MicMenuFour =
{
	{"No.3 Type:","No.3 Freq:","No.3 Q:","No.3 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0103,0x0311,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0103,0x0311,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0103,0x0311,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0103,0x0311,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuFive,4
};

S_UIMENU MicMenuThree =
{
	{"No.2 Type:","No.2 Freq:","No.2 Q:","No.2 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0102,0x0211,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0102,0x0211,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,10},{0x0102,0x0211,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0102,0x0211,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuFour,3
};

S_UIMENU MicMenuTwo =
{
	{"No.1 Type:","No.1 Freq:","No.1 Q:","No.1 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0101,0x0111,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,10},{0x0101,0x0111,3,3,BCDCODE,0x00,50,19.7,21772.6,5,1,10},{0x0101,0x0111,8,2,BCDCODE,0x00,1,0.4,15,3,1,10},{0x0101,0x0111,6,2,BCDCODE,0x00,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuThree,2
};

S_UIMENU MicMenuOne =
{
	{"Mic HPF:","Mic LPF:","Mic FB.EX:","EQ Pass:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"19.7;","19.7;","OFF;#1;#2;#3;","No;Yes","","","",""},
	{{0x0201,0x1111,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,5},{0x0202,0x1211,3,3,BCDCODE,0x00,1,19.7,21772.6,5,1,5},{0x0401,0x3111,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,7},{0x1101,0xA111,2,1,SELECTBOXTYPE,0x00,0,0,0,0,0,4}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&MicMenuTwo,1
};

struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	int m_Select;
	int m_TotalM;
};
int C_MICWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = (S_MENUWDATA *)GetWndData(sizeof(S_MENUWDATA));
	const S_RECT Rect[4] = {{95, 1, 85, 16},{95, 17, 85, 16},{95, 34, 85, 16},{95, 51, 85, 16}};
	//const S_RECT RectRect[4] = {{90, 0, 60, 16},{90, 16, 60, 16},{90, 32, 60, 16},{90, 48, 60, 16}};
	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &MicMenuOne;
		pData->m_Select = 0;
		pData->m_TotalM = 0;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuFour;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuFive;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuSix;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuSeven;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuEight;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuNigh;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuTen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuEleven;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuTwelve;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuThirteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuFourteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuFifteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuSixteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuSeventeen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &MicMenuOne;
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
		else if (wParam == KEY_MIC)
		{
			pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_KILLFOCUS,0x00,0x00);
			pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
			if (pData->m_pCurMenu == NULL)
			{
				pData->m_pCurMenu = &MicMenuTwo;
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
		else if (wParam == KEY_MAIN)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MAINWND,0);
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
