/** @file
 *  @brief 用户层系统基础类定义
 *  @author Steven Wan
 * date 2015/01/07
 *  @version 0.1
 */
#ifndef __NW_SYS_BASE_H__
#define __NW_SYS_BASE_H__
#include "basedef.h"
#include "syscfg.h"
#include "basedef.h"
#include "basetype.h"
#include "channel.h"
#include "bitsetex.h"
#include "htrace.h"

#define CONS_CHANNEL_NAME_MAX_LEN		16

#define CONS_SYNC_COMMAND "sync"

/* 控制码类型定义*/
#define CTL_READ	0x01
#define CTL_READ_ACK	0x81
#define CTL_READ_ERR_ACK	0xC1

#define CTL_BATCH_READ	0x02
#define CTL_BATCH_READ_ACK	0x82

#define CTL_WRITE	0x04
#define CTL_MULT_WRITE	0x05
#define CTL_WRITE_ACK	0x84
#define CTL_WRITE_ERR_ACK	0xC4


/** @class S_SYSCFG
 *  @brief 系统配置结构体
 */
struct S_SYSCFG
{
	/** 终端地址
	 */
	S_DWORD m_RTUA; ///< 终端地址=45210008 BCD+HEX,字节存储格式:21 45 08 00,高WORD是终端地址低WORD是区域码，

	/** 登陆信息
	 */
	char m_SysUser[20];
	char m_SysPassword[20];

	/** 配置信息
	 */
	 bool m_InitIRDA;
	INT16U m_HeartBeatRetryTimes;//心跳次数  次
	INT16U m_ReportAgainIntervalM;//当前任务补报时间间隔

	/** 端口映射表
	 */
	S_BYTE m_PortMap[SYSTEM_SERIAL_PORT_MAX];

public:
	void Init(void);
	/** @brief 设置终端地址 */
	void SetRTUA(const char *pRTUA);
	/** @brief 获取终端地址 */
	char *GetRTUA(char *pRTUA);
};

/** @class S_SYSRUN
 *  @brief 系统运行信息结构体
 */
struct S_SYSRUN
{
	/** 版本信息
	 */
	char m_ReleaseDate[32]; ///< 发布日期
	char m_HWVersion[32]; ///< 硬件版本号
	char m_SWVersion[32]; ///< 软件版本号
	char m_MPVersion[12]; ///< 主程序版本号

	/** 终端掉电信息
	 */
	S_TIME m_timePowerCut;

	/** 下行模块信息 */
	char m_RouterType[8]; ///< XC #XC,DR,YXD,RB,SR,LME,DX
	char m_ChipVersion[8]; ///< V1 #模块版本号,00为无效

	/** MODEM信息 */
	char m_ModemFactory[16]; ///< unknow
	char m_ModemModel[16]; ///< M590
	S_DWORD m_SimIP;

public:
	void Init(void);
};

struct S_IpConfig
{
	INT8U m_TermIP[4];
	INT8U m_NetMask[4];
	INT8U m_GateWay[4];
	INT8U m_ProxyIP[4];
	INT8U m_ProxyType;
	INT8U m_ProxyLink;
	INT16U m_ProxyPort;
	INT8U m_UserName[24];
	INT8U m_Password[24];

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

struct S_MenuConfig
{
	INT8U m_CurParamMode;
	INT8U m_MusicInput;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};

/** @class S_CHANNELS
 *  @brief 通道组合模板 
 *  @note 目前仅有上行通道(12)和下行通道(3)
 */
template <int ChannlCnt>
struct S_CHANNELS
{
	struct S_CHANNEL m_Item[ChannlCnt];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @brief 按通道名查找通道 */
	struct S_CHANNEL *FindChannel(const char *ChannlName)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && strcasecmp((char *)m_Item[i].m_Name, ChannlName) == 0)
				return &m_Item[i];
		}
		return NULL;
	}

		/** @brief 按虚拟串口号查找通道 */
	bool GetChannel(const INT8U vcomNo,S_CHANNEL& channel)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && m_Item[i].m_PortSet.m_ComSet.m_PortNO == vcomNo)
			{
				channel = m_Item[i];
				return true;
			}
		}
		return false;
	}


	/** @brief 按通道索引查找通道 */
	struct S_CHANNEL *FindChannel(int ChannlIdx)
	{
		if (ChannlIdx >= ChannlCnt || ChannlIdx < 0)
			return NULL;
		return &m_Item[ChannlIdx];
	}
};

struct S_UpChannel:public S_CHANNELS<CHANNEL_UP_COUNT>
{
};

struct S_DnChannel:public S_CHANNELS<CHANNEL_DN_COUNT>
{
};



typedef enum _TASK_STATE
{
	TASK_FINISHED=0,
	TASK_BREAK,
	TASK_STANDBY
}T_emTaskState;

typedef enum _TIME_UNIT
{
	UNIT_MINUTE=0,
	UNIT_HOUR,
	UNIT_DAY,
	UNIT_MONTH,
	UNIT_MAX
}T_emTIME_UNIT;

typedef enum
{
	VCOM1_RS4851=1,
	VCOM2_RS4852=2,
	VCOM31_ZB = 31
}T_emVcom_Type;





//
//process info
//
#define HTTX_CHANNL_MAX		3
struct S_HttxInfo
{
	bool m_ReportEnable;
	S_BYTE m_MsgConfirmCode; ///<  = 0 #0表示不进行消息认证,1-255:消息认证方案号
	S_BYTE m_HeartBeatPeriodM; ///< =30 #0表示不要心跳
	S_BYTE m_ResendTimes; ///< =3 #重发次数
	S_WORD m_WaitResponseTimeoutS; ///< =6 #终端等待从动站响应的超时时间
	S_WORD m_LoginPeriodS; ///< =60 #0表示不要登陆
	S_DWORD m_MonTrafficLimit; ///< =0 #0表示不须要流量控制
	char m_ChannlName[HTTX_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN];

public:
	void Init(void){memset(this, 0x00, sizeof(*this));}
	bool NeedHeartBeat(void){return m_HeartBeatPeriodM>0;}
	bool NeedLogon(void){return m_LoginPeriodS>0;}
};

struct S_HttxStatus
{
//	S_HttxSN m_HttxSN;
	bool m_fMasterTerm;
	bool m_fForwardEnable;
	S_WORD m_ResponseCmdCnt;
	S_WORD m_ReportCmdCnt;

	void Init(void)
	{
//		m_HttxSN = MP_HTTX_1;
		m_fMasterTerm = true;
		m_fForwardEnable = true;
		m_ResponseCmdCnt = 0;
		m_ReportCmdCnt = 0;
	}
};

#define PPP_CHANNL_MODEM	0
#define PPP_CHANNL_GPRS	1
#define PPP_CHANNL_AT		2
#define PPP_CHANNL_SMS		3
#define PPP_CHANNL_MAX	4
struct S_PppInfo
{
	int m_ModemPowerOffDelayS;
	int m_ModemPowerOnDelayS;
	S_WORD m_RefreshSignalInterS;///< 刷新无线信号间隔，缺省300秒
	S_WORD m_RedialInterM; ///< 掉线重拨间隔，缺省30分钟
	S_WORD m_RedialTimesPerDay; ///< #日重拨次数
	S_DialInfo m_DialInfo; ///< 拨号信息
	char m_MasterPhone[16]; ///< 主站电话号码
	char m_SmsCenterNo[16]; ///< 信息中心号码
	char m_ChannlName[PPP_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #MODEM通道,GPRS通道,AT命令通道,SMS通道

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_ModemPowerOffDelayS = 2;
		m_ModemPowerOnDelayS = 4;
		m_RefreshSignalInterS = 300;
		m_RedialInterM = 30;
		m_RedialTimesPerDay = 3;
	}
};

struct S_PppStatus
{
	int m_ModemType;
	char m_ModemFactory[32];
	char m_ModemModel[32];
	S_BYTE m_LcdBottomInfoIndicator; ///< 液晶显示PPP状态指示器
	S_BYTE m_Signal; ///< 信号强度(0~31)
	S_TIME m_timeSignal; ///< 取信号时间
	S_DWORD m_SimIP;
	S_TIME m_timeLastOnline;
	S_PppPackage m_PppPackage;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};


/** @brief 主站通讯状态
 */
struct S_ConnectStatus
{
	volatile bool m_HttxNeedPPP[3];
	volatile bool m_PppOnline;
	volatile bool m_Httx1Logon;
	volatile bool m_Reconnect;
	volatile S_TIME m_Httx1LogonTime;
	volatile S_TIME m_CommunicationTime;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};


#define LCD_TERM_NAME_LEN_MAX	32
struct S_LcdInfo
{
	int m_LCDPassword; ///< =0 #液晶调试设置权限密码
	int m_LCDGreyScale; ///< =195 #液晶灰度值
	int m_LCDBrightness; ///< 液晶背光亮度
	int m_LCDLightTimeoutS; ///< =60 #液晶背光灯点亮时间
	int m_BackDesktopTimeoutS; ///< =40 #返回桌面的时间
	char m_TermName[LCD_TERM_NAME_LEN_MAX]; ///< =***"配网自动化测控终端"

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_LCDPassword = 0;
		m_LCDGreyScale = 195;
		m_LCDBrightness = 100;
		m_LCDLightTimeoutS = 60;
		m_BackDesktopTimeoutS = 40;
		strcpy(m_TermName, "配网自动化测控终端");
	}

};

#define DBCJ_CHANNL_485I	0
#define DBCJ_CHANNL_485II	1
#define DBCJ_CHANNL_ZBWX	2
#define DBCJ_CHANNL_MAX	3

/*************** 数据标示*****************/
#define CONS_REGITEM_DATABUF_MAXLEN		16

struct S_RegItem
{
	S_WORD m_regid;
	S_WORD m_subregid;	
	S_WORD m_Len; //长度
	S_BYTE m_Type;//参考RegData的Flag说明,为15时表示自定义类型
	bool m_bDataBlock;	//数据块标识
	S_CHAR m_data[CONS_REGITEM_DATABUF_MAXLEN];
public:
	inline void init()
	{
		memset(this,0x00,sizeof(this));
	}
	
	bool SaveData(const S_CHAR *pBuf, S_WORD len);
	S_WORD ReadData(S_CHAR *pBuf);
	S_WORD BatchReadData(string &oStrData);	
	S_WORD GetLength(void){return m_Len;}
	bool IsValidData(void);
	S_WORD GetRegid(void){return m_regid;}
	S_WORD GetSubRegid(void){return m_subregid;}
};

#define CONS_REGITEMS_MAX		300
struct S_RegItems
{
	void Init()
	{
		m_count = 0;
	}
	S_RegItem* GetDataItemByRegid(S_WORD regid);
	S_RegItem* S_RegItems::GetDataItemBySubRegid(S_WORD subregid);	
	S_RegItem* GetDataItemByIndex(S_WORD Index);
	bool AddItem(S_RegItem *pItem);
	S_WORD GetItemNum(void){return m_count;}
private:
	S_RegItem m_Items[CONS_REGITEMS_MAX];
	S_WORD m_count;
};



#endif//__NW_SYS_BASE_H__

