//---------------------------------------------------------------------------------
// Copyright Steven Wan 2009-2010
// E-mail:   Steven Wan@163.com
// Project:			SGUI(Simple GUI)
// File:			sguimsg.h
// Description:		SGUI for Power user elect energy acquire system;unsupport multi_thread
//-------------------------------------------------------------
// Reversion Histroy:
//-------------------------------------------------------------
// Version		date		operations				by who
// 1.0.0		2010-01-18  Create                  Kingsea
//---------------------------------------------------------------------------------
#ifndef __SGUI_MSG_H__
#define __SGUI_MSG_H__
#include "basedef.h"

enum
{
	GM_UNKNOW,
	GM_KEYDN,
	GM_KEYUP,
	GM_CHAR,
	GM_CREATE,
	GM_RESTORE,
	GM_PAINT,
	GM_SYSPAINT,
	GM_TIME,
	GM_CMD,
	GM_DESTROY,
	GM_QUIT,

	GM_SETFOCUS,
	GM_KILLFOCUS,
	GM_AUTOBACK,//return 0 enable;return !0 disable

	GM_SYSKEY,

	/** follow user msg
	 */
	GM_USER = 100,

	GM_MAX,
	
};

#ifndef WIN32
enum
{
	GVK_KEYBEGIN, // begin
	

	// key
	KEY_UP = 128,
	KEY_DOWN = 64,
	KEY_MENU = 32,
	KEY_ESC = 16,
	KEY_SET = 2048,
	KEY_EFFECT = 1024,
	KEY_MAIN = 8,
	KEY_SUB = 4,
	KEY_CEN = 2,
	KEY_SUR = 1,
	KEY_MIC = 512,
	KEY_MUSIC = 256,
	KEY_SET_LEFT = 28,
	KEY_SET_RIGHT = 12,
	KEY_EFFECT_LEFT = 21,
	KEY_EFFECT_RIGHT = 9,
	KEY_MIC_LEFT = 14,
	KEY_MIC_RIGHT = 6,
	KEY_MUSIC_LEFT = 7,
	KEY_MUSIC_RIGHT = 3,


	GVK_UP = 2049,
	GVK_DN = 2050,
	GVK_LEFT = 2051,
	GVK_RIGHT = 2052,
	GVK_CANCEL = 2053,
	GVK_OK = 2054,
	GVK_PGUP = 2055,
	GVK_PGDN = 2056,

	GVK_KEYEND = 2057, // end

	GVK_SYSKEYBEGIN, // begin
	GVK_PROGRAMKEY,
	GVK_SYSKEYEND, // end

	GVK_KEYMAX
};
#else
enum
{
	GVK_KEYBEGIN, // begin
	GVK_UP,
	GVK_DN,
	GVK_LEFT,
	GVK_RIGHT,
	GVK_CANCEL,
	GVK_OK,
	GVK_PGUP,
	GVK_PGDN,
	GVK_KEYEND, // end

	GVK_SYSKEYBEGIN, // begin
	GVK_PROGRAMKEY,
	GVK_SYSKEYEND, // end

	GVK_KEYMAX,
	KEY_UP,
	KEY_DOWN,
	KEY_MENU,
	KEY_ESC,
	KEY_SET,
	KEY_EFFECT,
	KEY_MAIN,
	KEY_SUB,
	KEY_CEN,
	KEY_SUR,
	KEY_MIC,
	KEY_MUSIC,
	KEY_SET_LEFT,
	KEY_SET_RIGHT,
	KEY_EFFECT_LEFT,
	KEY_EFFECT_RIGHT,
	KEY_MIC_LEFT,
	KEY_MIC_RIGHT,
	KEY_MUSIC_LEFT,
	KEY_MUSIC_RIGHT
};

#endif

#define GVK_ENTER	0x0D
#define GVK_ESC		0x1B
#define GVK_BACKSPACE		0x08
#define GVK_TAB		0x09
#define GVK_F1		0x70

#define GM_TIME_INTERVAL_MS		1000

class C_WNDBASE;
struct S_GUIMSG
{
	C_WNDBASE *pWnd;
	S_WORD  Msg;
	S_WORD  wParam;
	S_DWORD lParam;

	S_GUIMSG(void){pWnd=NULL;Msg=GM_UNKNOW;wParam=0;lParam=0;}
};


#endif //__SGUI_OBJ_H__
