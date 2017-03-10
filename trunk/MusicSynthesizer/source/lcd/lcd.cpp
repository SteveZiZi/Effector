/** @file
 *  @brief 本地操作与显示功能定义
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 */
#include "lcd.h"
#include "sysipc.h"
#include "htrace.h"
#include "sguiapp.h"
#include "sguimsg.h"
#include "sguidc.h"
#include "cfg_usrp.h"
#include "timeop.h"
#include "menuwnd.h"
#include "status.h"
#include "context.h"
#include "sysctrl.h"
#include "cenwnd.h"
#include "subwnd.h"
#include "surwnd.h"
#include "musicwnd.h"
#include "micwnd.h"
#include "effectwnd.h"
#include "mainwnd.h"
#include "lcddataiface.h"
#include "parammode.h"


C_UIMAPP  theUIMApp;

#define SYS_FLUSH_TIMEOUT	4
#define BOTTOM_INFO_FLUSH_INTERVAL_S	2
#define DESKTOP_TURNVIEW_TIMEOUT_S		90

C_UIMAPP::C_UIMAPP(void)
{
	m_CloseLight = 60;
	m_BackDesktop = 40;
	m_SysFlush   = SYS_FLUSH_TIMEOUT;
}

C_UIMAPP::~C_UIMAPP()
{
}

int C_UIMAPP::TranslateMsg(S_GUIMSG &msg)
{
	S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
	switch (msg.Msg)
	{
	case GM_KEYDN:
	case GM_KEYUP:
		m_CloseLight = pLcdInfo->m_LCDLightTimeoutS;
		m_BackDesktop = pLcdInfo->m_BackDesktopTimeoutS;
		loget<<"m_CloseLight="<<m_CloseLight<<",打开背光灯"<<endl;
		OpenBacklight();
		break;
	case GM_TIME:
		if (m_CloseLight > 0)
		{
			m_CloseLight--;
		}
		else if (m_CloseLight == 0)
		{
			CloseBacklight();
			m_CloseLight = pLcdInfo->m_LCDLightTimeoutS;
		}
		if (m_BackDesktop > 0)
		{
			m_BackDesktop--;
		}
		else if (m_BackDesktop == 0)
		{
			if (EmptyWndStack() == 0)
				msg.pWnd = NULL, msg.Msg = GM_UNKNOW;
			m_BackDesktop = pLcdInfo->m_BackDesktopTimeoutS;
		}
		m_SysFlush--;
		if (m_SysFlush <= 0)
		{
			S_GUIMSG aMsg;
			aMsg.Msg = GM_SYSPAINT;
			m_MsgQ.push(aMsg);
			m_SysFlush = SYS_FLUSH_TIMEOUT;
			// 心跳 
			C_SERVER *pServer = C_SERVER::GetCurrentServer();
			pServer->HeartBeat();
		}
		break;
	default:
		break;
	}
	return C_SGUIAPP::TranslateMsg(msg);
}

/*static char *WeekName[] =
{
	"星期日","星期一","星期二","星期三","星期四","星期五","星期六"
};*/

S_CHAR *MainDisplay[] = 
{
	"Music:","Mic:","Effect:","FB.EX:","Input:","Param:"
};

typedef struct _MAIN_DISPLAY_VALUE  
{
	S_CHAR MusicValue;
	S_CHAR MicValue;
	S_CHAR EffectValue;
	S_CHAR FBEXValue;
	S_CHAR InputModel;
	S_CHAR ParamValue; 

	void Init()
	{
		MusicValue = 45;
		MicValue = 25;
		EffectValue = 20;
		FBEXValue = 1;
		InputModel = 1;
		ParamValue = 1;
	};
}MAINDISPLAYVALUE,*PMAINDISPLAYVALUE;


int C_DESKTOP::WndProcess(S_WORD msg, S_WORD wParam, S_DWORD lParam)
{
	MAINDISPLAYVALUE mainDisplayValue;
	
	switch (msg)
	{
	case GM_CREATE:
		mainDisplayValue.Init();
		break;
	case GM_PAINT:{
		//int size;
		mainDisplayValue.Init();
		S_DATE date;
		char strBuf[64];
		//S_LcdInfo *pLcdInfo = STATUS.GetLcdInfo();
		C_SGUIDC *pdc = BeginPaint();

		C_TIME::LocalTime(C_TIME::TimeNow(), date);
		pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		//size = pdc->GetStringExtent(pLcdInfo->m_TermName);
		//pdc->DrawString((160-size)/2, 30, pLcdInfo->m_TermName);
		//sprintf(strBuf, "%d-%02d-%02d", date.m_Year, date.m_Mon, date.m_Day);
		//size = pdc->GetStringExtent(strBuf);
		//pdc->DrawString((160-size)/2, 60, strBuf);
		//size = pdc->GetStringExtent(WeekName[date.m_WeekDay]);
		//pdc->DrawString((160-size)/2, 80, WeekName[date.m_WeekDay]);	
		//pdc->DrawHLine(0, 122, 160);
		/*pdc->SelectObject(pdc->GetStockGuiObj(SGUI_OBJ_F12x12));
		if (m_fPppInfo == true) pdc->DrawString(20, 126, GetPppBottomInfo());
		else pdc->DrawString(20, 126, GetDbcjBottomInfo());*/
		
		// 第一行
		pdc->DrawString(11, 0, MainDisplay[0]);

		char data = 0;
		C_LcdDataIface::ReadMusicData(&data);
		int intData = data;
		mainDisplayValue.MusicValue = data;
		sprintf(strBuf,"%d ",mainDisplayValue.MusicValue);
		pdc->DrawString(63, 0, strBuf);

		pdc->DrawString(90, 0, MainDisplay[1]);

		data = 0;
		C_LcdDataIface::ReadMicData(&data);
		intData = data;
		mainDisplayValue.MicValue = data;
		sprintf(strBuf,"%d ",mainDisplayValue.MicValue);
		pdc->DrawString(130, 0, strBuf);
		// 第二行
		pdc->DrawString(11, 16, MainDisplay[2]);

		data = 0;
		C_LcdDataIface::ReadEffectData(&data);
		intData = data;
		mainDisplayValue.EffectValue = data;
		sprintf(strBuf,"%d ",mainDisplayValue.EffectValue);
		pdc->DrawString(63, 16, strBuf);

		pdc->DrawString(90, 16, MainDisplay[3]);
		sprintf(strBuf,"%d ",mainDisplayValue.FBEXValue);
		pdc->DrawString(130, 16, strBuf);
		// 第三行
		S_STATUS MyStatus;
		C_ParamMode::LoadTermMenuConfig(&MyStatus);
		mainDisplayValue.InputModel = MyStatus.m_MenuConfig.m_MusicInput;
		mainDisplayValue.ParamValue = MyStatus.m_MenuConfig.m_CurParamMode;
		pdc->DrawString(11, 32, MainDisplay[4]);
		sprintf(strBuf,"#%d ",mainDisplayValue.InputModel + 1);
		pdc->DrawString(63, 32, strBuf);
		// 第四行
		pdc->DrawString(11, 48, MainDisplay[5]);
		sprintf(strBuf,"M%d ",mainDisplayValue.ParamValue + 1);
		pdc->DrawString(63, 48, strBuf);

		EndPaint(pdc);
		
		}break;
	case GM_TIME:
		m_DesktopTimeoutS += 1;
		if (m_DesktopTimeoutS % BOTTOM_INFO_FLUSH_INTERVAL_S == 0)
		{
			m_fPppInfo = !m_fPppInfo;
			InvalidateRect(NULL);
		}
		if (m_DesktopTimeoutS >= DESKTOP_TURNVIEW_TIMEOUT_S)
		{
			//m_DesktopTimeoutS = 0;
			//GOTOWND(C_TURNVIEW, 0);
		}
		break;
	case GM_KEYUP:
		//m_DesktopTimeoutS = 0;
#ifndef WIN32
		switch (wParam)
		{
		case KEY_MAIN:
			GOTOWND(C_MAINWND,0);
			break;
		case KEY_SUB:
			GOTOWND(C_SUBWND,0);
			break;
		case KEY_CEN:
			GOTOWND(C_CENWND,0);
			break;
		case KEY_SUR:
			GOTOWND(C_SURWND,0);
			break;
		case KEY_MIC:
			GOTOWND(C_MICWND,0);
			break;
		case KEY_MUSIC:
			GOTOWND(C_MUSICWND,0);
			break;
		case KEY_EFFECT:
			GOTOWND(C_EFFECTWND,0);
			break;
		case KEY_MENU:
			GOTOWND(C_MENUWND,0);
			break;
		case KEY_MUSIC_LEFT:
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
				mainDisplayValue.MusicValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_MUSIC_RIGHT:
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
				mainDisplayValue.MusicValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_MIC_LEFT:
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
				mainDisplayValue.MicValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_MIC_RIGHT:
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
				mainDisplayValue.MicValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_EFFECT_LEFT:
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
				mainDisplayValue.EffectValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_EFFECT_RIGHT:
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
				mainDisplayValue.EffectValue = data;
				InvalidateRect(NULL);
				break;
			}
		case KEY_SET_LEFT:
			break;
		case KEY_SET_RIGHT:
			break;
		case KEY_SET:
			break;
		default:
			break;
		}
#else 
		GOTOWND(C_MAINWND, 0);
		break;
#endif
		//GOTOWND(C_MICWND, 0);
		//break;
	default:
		return DefWndProcess(msg, wParam, lParam);
	}
	return 0;
}

const char *C_DESKTOP::GetDbcjBottomInfo(void)
{
	return CONTEXT.GetLcdBottomInfo(MP_PROCESS_DBCJ);
}

const char *C_DESKTOP::GetPppBottomInfo(void)
{
	return CONTEXT.GetLcdBottomInfo(MP_PROCESS_PPP);
}

//
//
//
int C_LcdServer::BeforeService(void)
{
	m_pDesktop = new C_DESKTOP;
	if (theUIMApp.SysInit(CFG_PATH_RESOURCE) != 0)
		return -1;
	m_pLcdInfo = STATUS.GetLcdInfo();

	C_SGUIAPP::SetCurApp(&theUIMApp);
	theUIMApp.SetDesktopWnd(m_pDesktop);
	theUIMApp.CloseBacklight();
	theUIMApp.SetCloseLightDelay(m_pLcdInfo->m_LCDLightTimeoutS);
	theUIMApp.SetBackDesktopDelay(m_pLcdInfo->m_BackDesktopTimeoutS);
	theUIMApp.SetLcdContrast(85);
	theUIMApp.SetLcdBrightness(m_pLcdInfo->m_LCDBrightness);
	theUIMApp.SetLcdGrayness(m_pLcdInfo->m_LCDGreyScale);
	theUIMApp.SetLcdDirection(0);

	return 0;
}

int C_LcdServer::AfterService(void)
{
	delete m_pDesktop;
	return 0;
}

int C_LcdServer::OnTickService(void)
{
	S_GUIMSG aMsg;
	while (1)
	{
		aMsg = theUIMApp.GetGuiMsg();
		theUIMApp.TranslateMsg(aMsg);
		theUIMApp.DispatchGuiMsg(aMsg);
	}
	return 0;
}


