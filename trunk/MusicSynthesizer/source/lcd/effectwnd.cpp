/** @file
 *  @brief 本地操作效果菜单窗口定义
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
#include "effectwnd.h"
#include "mainwnd.h"
#include "menuwnd.h"
#include "cenwnd.h"
#include "subwnd.h"
#include "surwnd.h"
#include "musicwnd.h"
#include "micwnd.h"

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


S_UIMENU EffectMenuTourteen =
{
	{"No.5 Type:","No.5 Freq:","No.5 Q:","No.5 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0105,0x0521,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0105,0x0521,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0105,0x0521,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0105,0x0521,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	NULL,14
};

S_UIMENU EffectMenuThirteen =
{
	{"No.4 Type:","No.4 Freq:","No.4 Q:","No.4 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0104,0x0421,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0104,0x0421,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0104,0x0421,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0104,0x0421,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuTourteen,13
};

S_UIMENU EffectMenuTwelve =
{
	{"No.3 Type:","No.3 Freq:","No.3 Q:","No.3 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0103,0x0321,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0103,0x0321,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0103,0x0321,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0103,0x0321,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuThirteen,12
};

S_UIMENU EffectMenuEleven =
{
	{"No.2 Type:","No.2 Freq:","No.2 Q:","No.2 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0102,0x0221,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0102,0x0221,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0102,0x0221,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0102,0x0221,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuTwelve,11
};

S_UIMENU EffectMenuTen =
{
	{"No.1 Type:","No.1 Freq:","No.1 Q:","No.1 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0101,0x0121,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0101,0x0121,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0101,0x0121,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0101,0x0121,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuEleven,10
};

S_UIMENU EffectMenuNigh =
{
	{"Reverb Level:","Direct Level:","Reverb PDelay:","Reverb Time:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","0;","0;","","","",""},
	{{0x1002,0x1002,1,1,HEXCODE,0xff,1,0,100,0,0,5},{0x1002,0x1002,2,1,HEXCODE,0xff,1,0,100,0,0,5},{0x1002,0x1002,3,1,HEXCODE,0xff,2,0,200,0,0,5},{0x1002,0x1002,4,3,BCDCODE,0xff,62.5,0,500,4,1,5}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuTen,9
};

S_UIMENU EffectMenuEight =
{
	{"Echo Hpf:","Echo Lpf:","Reverb Hpf:","Reverb Lpf:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"19.7;","19.7;","19.7;","19.7;","","","",""},
	{{0x0201,0x1121,3,3,BCDCODE,0x02,1,19.7,21772.6,5,1,5},{0x0202,0x1221,3,3,BCDCODE,0x02,1,19.7,21772.6,5,1,5},{0x0201,0x1131,3,3,BCDCODE,0x04,1,19.7,21772.6,5,1,5},{0x0202,0x1231,3,3,BCDCODE,0x04,1,19.7,21772.6,5,1,5}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuNigh,8
};

S_UIMENU EffectMenuSeven =
{
	{"No.5 Type:","No.5 Freq:","No.5 Q:","No.5 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0105,0x0521,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0105,0x0521,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0105,0x0521,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0105,0x0521,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuEight,7
};

S_UIMENU EffectMenuSix =
{
	{"No.4 Type:","No.4 Freq:","No.4 Q:","No.4 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0104,0x0421,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0104,0x0421,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0104,0x0421,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0104,0x0421,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuSeven,6
};

S_UIMENU EffectMenuFive =
{
	{"No.3 Type:","No.3 Freq:","No.3 Q:","No.3 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0103,0x0321,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0103,0x0321,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0103,0x0321,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0103,0x0321,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuSix,5
};

S_UIMENU EffectMenuFour =
{
	{"No.2 Type:","No.2 Freq:","No.2 Q:","No.2 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0102,0x0221,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0102,0x0221,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0102,0x0221,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0102,0x0221,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuFive,4
};

S_UIMENU EffectMenuThree =
{
	{"No.1 Type:","No.1 Freq:","No.1 Q:","No.1 Gain:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"Parametric;LowShelf 12db;HighShelf 12 db","19.7;","0.4;","-15.0;","","","",""},
	{{0x0101,0x0121,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,10},{0x0101,0x0121,3,3,BCDCODE,0x02,50,19.7,21772.6,5,1,10},{0x0101,0x0121,8,2,BCDCODE,0x02,1,0.4,15,3,1,10},{0x0101,0x0121,6,2,BCDCODE,0x02,0.1,-15,15,3,1,10}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuFour,3
};

S_UIMENU EffectMenuTwo =
{
	{"Echo Repeat:","Right Delay:","Per Delay R:","EQ Pass:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","0;","No;Yes","","","",""},
	{{0x1001,0x1001,8,1,HEXCODE,0xff,1,0,90,0,0,8},{0x1001,0x1001,7,1,HEXCODE,0xff,1,-50,50,0,0,8},{0x1001,0x1001,6,1,HEXCODE,0xff,1,0,50,0,0,8},{0x1101,0xA111,2,1,SELECTBOXTYPE,0x02,0,0,0,0,0,4}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuThree,2
};

S_UIMENU EffectMenuOne =
{
	{"Echo Level:","Direct Level:","Per Delay:","Echo Delay:","","","",""},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	{"0;","0;","0;","0;","","","",""},
	{{0x1001,0x1001,1,1,HEXCODE,0xff,1,0,100,0,0,8},{0x1001,0x1001,2,1,HEXCODE,0xff,1,0,100,0,0,8},{0x1001,0x1001,3,1,HEXCODE,0xff,1,0,250,0,0,8},{0x1001,0x1001,4,2,HEXCODE,0xff,2,0,500,0,0,8}},
	{NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL},
	&EffectMenuTwo,1
};

struct S_MENUWDATA
{
	S_UIMENU *m_pCurMenu;
	int m_Select;
	int m_TotalM;
};
int C_EFFECTWND::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	S_MENUWDATA *pData = (S_MENUWDATA *)GetWndData(sizeof(S_MENUWDATA));
	const S_RECT Rect[4] = {{95, 1, 85, 16},{95, 17, 85, 16},{95, 34, 85, 16},{95, 51, 85, 16}};
	//const S_RECT RectRect[4] = {{90, 0, 60, 16},{90, 16, 60, 16},{90, 32, 60, 16},{90, 48, 60, 16}};
	switch (msg)
	{
	case GM_CREATE:
		pData->m_pCurMenu = &EffectMenuOne;
		pData->m_Select = 0;
		pData->m_TotalM = 0;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuTwo;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuThree;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuFour;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuFive;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuSix;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuSeven;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuEight;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuNigh;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuTen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuEleven;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuTwelve;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuThirteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuTourteen;
		for (int i = 0; pData->m_pCurMenu->m_pMenu[i][0]; i++, pData->m_TotalM=i)
		{
			pData->m_pCurMenu->m_pSelectBox[i] = new C_SELECTBOX(pData->m_pCurMenu->m_sRegSec[i]);
		}
		pData->m_pCurMenu = &EffectMenuOne;
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
		else if (wParam == KEY_EFFECT)
		{
			pData->m_pCurMenu->m_pSelectBox[pData->m_Select]->SendWndMsg(GM_KILLFOCUS,0x00,0x00);
			pData->m_pCurMenu = pData->m_pCurMenu->m_pPMenu;
			if (pData->m_pCurMenu == NULL)
			{
				pData->m_pCurMenu = &EffectMenuOne;
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
		else if (wParam == KEY_MAIN)
		{
			RETURNWND();
			ReleaseWndData();
			C_PWD::SetCertification(false);
			GOTOWND(C_MAINWND,0);
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

