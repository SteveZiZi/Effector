/** @file
 *  @brief �û���ϵͳ�����ඨ��
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

/* ���������Ͷ���*/
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
 *  @brief ϵͳ���ýṹ��
 */
struct S_SYSCFG
{
	/** �ն˵�ַ
	 */
	S_DWORD m_RTUA; ///< �ն˵�ַ=45210008 BCD+HEX,�ֽڴ洢��ʽ:21 45 08 00,��WORD���ն˵�ַ��WORD�������룬

	/** ��½��Ϣ
	 */
	char m_SysUser[20];
	char m_SysPassword[20];

	/** ������Ϣ
	 */
	 bool m_InitIRDA;
	INT16U m_HeartBeatRetryTimes;//��������  ��
	INT16U m_ReportAgainIntervalM;//��ǰ���񲹱�ʱ����

	/** �˿�ӳ���
	 */
	S_BYTE m_PortMap[SYSTEM_SERIAL_PORT_MAX];

public:
	void Init(void);
	/** @brief �����ն˵�ַ */
	void SetRTUA(const char *pRTUA);
	/** @brief ��ȡ�ն˵�ַ */
	char *GetRTUA(char *pRTUA);
};

/** @class S_SYSRUN
 *  @brief ϵͳ������Ϣ�ṹ��
 */
struct S_SYSRUN
{
	/** �汾��Ϣ
	 */
	char m_ReleaseDate[32]; ///< ��������
	char m_HWVersion[32]; ///< Ӳ���汾��
	char m_SWVersion[32]; ///< ����汾��
	char m_MPVersion[12]; ///< ������汾��

	/** �ն˵�����Ϣ
	 */
	S_TIME m_timePowerCut;

	/** ����ģ����Ϣ */
	char m_RouterType[8]; ///< XC #XC,DR,YXD,RB,SR,LME,DX
	char m_ChipVersion[8]; ///< V1 #ģ��汾��,00Ϊ��Ч

	/** MODEM��Ϣ */
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
 *  @brief ͨ�����ģ�� 
 *  @note Ŀǰ��������ͨ��(12)������ͨ��(3)
 */
template <int ChannlCnt>
struct S_CHANNELS
{
	struct S_CHANNEL m_Item[ChannlCnt];

	void Init(void){memset(this, 0x00, sizeof(*this));}
	/** @brief ��ͨ��������ͨ�� */
	struct S_CHANNEL *FindChannel(const char *ChannlName)
	{
		for (int i = 0; i < ChannlCnt; ++i)
		{
			if (m_Item[i].m_Enable == true && strcasecmp((char *)m_Item[i].m_Name, ChannlName) == 0)
				return &m_Item[i];
		}
		return NULL;
	}

		/** @brief �����⴮�ںŲ���ͨ�� */
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


	/** @brief ��ͨ����������ͨ�� */
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
	S_BYTE m_MsgConfirmCode; ///<  = 0 #0��ʾ��������Ϣ��֤,1-255:��Ϣ��֤������
	S_BYTE m_HeartBeatPeriodM; ///< =30 #0��ʾ��Ҫ����
	S_BYTE m_ResendTimes; ///< =3 #�ط�����
	S_WORD m_WaitResponseTimeoutS; ///< =6 #�ն˵ȴ��Ӷ�վ��Ӧ�ĳ�ʱʱ��
	S_WORD m_LoginPeriodS; ///< =60 #0��ʾ��Ҫ��½
	S_DWORD m_MonTrafficLimit; ///< =0 #0��ʾ����Ҫ��������
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
	S_WORD m_RefreshSignalInterS;///< ˢ�������źż����ȱʡ300��
	S_WORD m_RedialInterM; ///< �����ز������ȱʡ30����
	S_WORD m_RedialTimesPerDay; ///< #���ز�����
	S_DialInfo m_DialInfo; ///< ������Ϣ
	char m_MasterPhone[16]; ///< ��վ�绰����
	char m_SmsCenterNo[16]; ///< ��Ϣ���ĺ���
	char m_ChannlName[PPP_CHANNL_MAX][CONS_CHANNEL_NAME_MAX_LEN]; ///< #MODEMͨ��,GPRSͨ��,AT����ͨ��,SMSͨ��

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
	S_BYTE m_LcdBottomInfoIndicator; ///< Һ����ʾPPP״ָ̬ʾ��
	S_BYTE m_Signal; ///< �ź�ǿ��(0~31)
	S_TIME m_timeSignal; ///< ȡ�ź�ʱ��
	S_DWORD m_SimIP;
	S_TIME m_timeLastOnline;
	S_PppPackage m_PppPackage;

	void Init(void){memset(this, 0x00, sizeof(*this));}
};


/** @brief ��վͨѶ״̬
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
	int m_LCDPassword; ///< =0 #Һ����������Ȩ������
	int m_LCDGreyScale; ///< =195 #Һ���Ҷ�ֵ
	int m_LCDBrightness; ///< Һ����������
	int m_LCDLightTimeoutS; ///< =60 #Һ������Ƶ���ʱ��
	int m_BackDesktopTimeoutS; ///< =40 #���������ʱ��
	char m_TermName[LCD_TERM_NAME_LEN_MAX]; ///< =***"�����Զ�������ն�"

	void Init(void)
	{
		memset(this, 0x00, sizeof(*this));
		m_LCDPassword = 0;
		m_LCDGreyScale = 195;
		m_LCDBrightness = 100;
		m_LCDLightTimeoutS = 60;
		m_BackDesktopTimeoutS = 40;
		strcpy(m_TermName, "�����Զ�������ն�");
	}

};

#define DBCJ_CHANNL_485I	0
#define DBCJ_CHANNL_485II	1
#define DBCJ_CHANNL_ZBWX	2
#define DBCJ_CHANNL_MAX	3

/*************** ���ݱ�ʾ*****************/
#define CONS_REGITEM_DATABUF_MAXLEN		16

struct S_RegItem
{
	S_WORD m_regid;
	S_WORD m_subregid;	
	S_WORD m_Len; //����
	S_BYTE m_Type;//�ο�RegData��Flag˵��,Ϊ15ʱ��ʾ�Զ�������
	bool m_bDataBlock;	//���ݿ��ʶ
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

