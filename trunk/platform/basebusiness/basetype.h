/**
 *  @file
 *  @brief 基本用户类型定义
 *  @author Steven Wan
 *  @date 2014-07-28
 *  @version v1.0
 *  @note
 *  基础业务基本数据类型定义
 */
#ifndef __BASEB_BASETYPE_H__
#define __BASEB_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"
#include "timeop.h"
#include "channel.h"

/** @brief 后台通讯进程序号 */
enum S_HttxSN
{
	MP_HTTX_1 = 0,
	MP_HTTX_2,
	MP_HTTX_3,
	MP_HTTX_4,
	MP_HTTX_5,
	MP_HTTX_6,
	MP_HTTX_7,
	MP_HTTX_8,
	MP_HTTX_9
};
/** @brief 后台通讯通道序号 */
enum S_ChannelSN
{
	HTTX_CHANNEL_1 = 0,
	HTTX_CHANNEL_2,
	HTTX_CHANNEL_3
};

/** @brief 数据存储最大记录数 */
struct S_StoreMax
{
	S_WORD m_DayData; //日数据最大记录数
	S_WORD m_MonData; //月数据最大记录数
	S_WORD m_VipData; //重点户数据最大记录数
	S_WORD m_CbrData; //抄表日数据最大记录数
};

//
//DA解析
//
class C_DA
{
public:
	C_DA(INT16U DA):m_DA1((INT8U*)&DA, 1),m_DA2(DA>>8){}
	~C_DA(){}
	
	/** @brief MP统计
	 *  @return 0 终端测量点
	 *  @return 1-8 表计等测量点
	 *  @return 0xFF 所有测量点
	 */
	INT8U GetMpCount(void);
	void ResetMP(int pos){m_DA1.ClearBit(pos);}
	/** @brief MP计算 */
	bool TestMP(int pos);
	INT16U GetMP(int pos);

	INT8U GetDA1(void){INT8U *p = m_DA1.GetData(); return *p;}
	INT8U GetDA2(void){return m_DA2;}
	INT16U GetDA(void){INT8U *p = m_DA1.GetData(); return (m_DA2<<8)|*p;}

private:
	C_BITSET<1> m_DA1;
	INT8U m_DA2;
};

//
//DT 解析
//
class C_DT
{
public:
	C_DT(S_WORD DT):m_DT1((S_BYTE*)&DT, 1),m_DT2(DT>>8){}
	~C_DT(){}
	
	/** @brief 功能点统计
	 *  @return 0 无功能点
	 *  @return 1-8 功能点置位
	 */
	S_BYTE GetFnCount(void){return m_DT1.CountBit();}
	void ResetFN(int pos){m_DT1.ClearBit(pos);}
	/** @brief 功能点计算 pos[0-7]*/
	bool TestFN(int pos){return m_DT1.TestBit(pos);}
	S_WORD GetFN(int pos){return m_DT2*8 + (pos+1);}

	S_BYTE GetDT1(void){INT8U *p = m_DT1.GetData(); return *p;}
	S_BYTE GetDT2(void){return m_DT2;}
	S_WORD GetDT(void){INT8U *p = m_DT1.GetData(); return (m_DT2<<8)|*p;}

private:
	C_BITSET<1> m_DT1;
	S_BYTE m_DT2;
};

class C_DAPN
{
public:
	/** @brief 将PN转换成DA
	 * @param[in] PN 测量点号
	 * @return 有效DA
	 */
	static S_WORD PN2DA(S_WORD PN);
	/** @brief 将DA转换成PN
	 * @param[in] DA 信息点
	 * @return 有效PN
	 */
	static S_WORD DA2PN(S_WORD DA);
};

class C_DTFN
{
public:
	/** @brief 将FN转换成DT
	 * @param[in] FN 信息功能点
	 * @return 有效DT
	 */
	static S_WORD FN2DT(S_WORD FN);
	/** @brief 将DT转换成FN
	 * @param[in] DT 信息类
	 * @return 有效FN
	 */
	static S_WORD DT2FN(S_WORD DT);
};

/** @brief 流量控制信息 */
struct S_FlowInfo
{
	//#流控使能控制
	bool m_FlowCtrlEnable;
	//#登陆控制
	S_WORD m_LogonInterS;
	S_WORD m_LogonRetryTimes;
	S_WORD m_LogonFailedTimes;
	//#Socket控制
	S_WORD m_SocketConnectInterS;
	S_WORD m_SocketConnectRetryTimes;
	S_WORD m_SocketConnectFailedTimes;
	//#拨号控制
	S_WORD m_DialInterS;
	S_WORD m_DialRetryTimes;
	S_WORD m_DialFrequencyM;
	S_WORD m_DialFailedTimes;
	//#主站通讯日
	BitSetEx<32> m_MSConnectDate;
	BitSetEx<24> m_MSConnectTime;
	//#日月流量控制
	S_DWORD m_DayFlowMax; ///< #0表示不进行流量控制
	S_DWORD m_MonFlowMax; ///< #0表示不进行流量控制

	//心跳失败次数计数
	S_WORD m_HeartBeatFailedTimes;

public:	
	void Init(void);
	void ClearFailedInfo(void);
};

/** @brief 拨号信息 */
#define PPP_INFO_STRING_LEN_MAX	36
struct S_DialInfo
{
	S_WORD m_CheckInterM;//检测间隔（分钟）
	S_WORD m_CheckFailTimes;//检测失败阀值，如果连续此数值次检测失败，将断线
	char m_Apn[PPP_INFO_STRING_LEN_MAX];//APN
	char m_TeleNo[PPP_INFO_STRING_LEN_MAX];//电话号码，为CONS_DEFAULT_KEY时表示采用默认值
	char m_User[PPP_INFO_STRING_LEN_MAX];//拨号用户，为CONS_DEFAULT_KEY时表示采用默认值
	char m_Pwd[PPP_INFO_STRING_LEN_MAX];//拨号密码，为CONS_DEFAULT_KEY时表示采用默认值
};

/** @brief PPP报文统计 */
struct S_PppPackage
{
	S_TIME m_timeInit;
	INT64U m_LastRecvPackages;//接收包数目
	INT64U m_LastSendPackages;//发送包数目
	INT64U m_TotalRecvPackages;//总的接收包数目
	INT64U m_TotalSendPackages;//总的发送包数目
};

/** @brief 表计规约名称定义集 */
//{{{
#define MRULE_NAME_GB645_1997 "GB645-1997"
#define MRULE_NAME_GB645_2007 "GB645-2007"
#define MRULE_NAME_JLCY	"ACGather"

//}}}

/** @brief 数据报文结构 */
struct S_DATAGRAM
{
	string m_strProtocol; ///< 协议名称
	string m_strUserData; ///< 协议报文
};

/** @brief 终端地址结构 */
#define TERM_ADDR_LEN_MAX		6
struct S_TERMINAL
{
	string m_strProtocol; ///< 协议名称
	string m_strAddress; ///< 终端地址
};
struct S_TERMINALS:public vector<S_TERMINAL>
{
};
bool operator<(const S_TERMINAL &left, const S_TERMINAL &right);

//
//表计寄存器定义
//
enum
{
	MR_TYPE_UNKNOW,
	MR_TYPE_BCD,
	MR_TYPE_BIN,
	MR_TYPE_BCD_BLOCK,
	MR_TYPE_BCD_TARIFF,
	MR_TYPE_BCD_TIME,
	MR_TYPE_BCD_TIME_BLOCK,
	MR_TYPE_ASCII,
	MR_TYPE_STATUS,
	MR_TYPE_TIME,

	MR_TYPE_OTHER = 0x7F
};
/** @brief 表计寄存器定义 */
struct S_REGISTER
{
	S_DWORD m_ID;///< 寄存器ID
	S_BYTE m_Type; ///< 值类型 MR_TYPE_
	S_BYTE m_Len; ///< 值长度，字节数，变长用0表示
	S_BYTE m_Format; ///< 类型相关,BCD类为整数小数位,如0x62
	S_BYTE m_Reserve; ///< 保留

public:
	S_REGISTER(void){memset(this, 0x00, sizeof(*this));}
	const char *TypeName(void)const;
	int FillData(string &strData)const;
};
struct S_REGISTERS:public vector<S_REGISTER>
{
};
bool operator<(const S_REGISTER &left, const S_REGISTER &right);

//
//表计基础信息
//
struct S_MeterBase
{
	string m_strMeterAddr;
	S_COMSET m_ComSet; 
	int m_MeterRuleID;
};

//
//测量点位图结构
//
#define BASEB_MP_COUNT_MAX	1201
struct S_MPBS:public BitSetEx<BASEB_MP_COUNT_MAX> //MP数据位定义
{
};
struct S_MBS:public BitSetEx<BASEB_MP_COUNT_MAX> //Meter数据位定义
{
};
struct S_MTBS
{
	S_MBS m_AllMBS;///< 有效的电表,所有电表
	S_MBS m_485MBS;///< 直抄表,终端通过485抄
	S_MBS m_ZBMBS;///< 非直抄表,终端通过载波或无线抄
//	S_MBS m_Vip485MBS;///< 重点户直抄表
	S_MBS m_VipZBMBS;///< 非重点户直抄表
};

//
//主站数据项映射通用寄存器
//
#define MSDI_COMM_REG_MAX	32
struct S_DIBS:public BitSetEx<MSDI_COMM_REG_MAX>
{
};

//
//采集任务信息
//
#define TASKINFO_NAME_MAX	32
//系统任务索引及总数
enum
{
	SYS_TASK_IDLE,
	SYS_TASK_ROUTE,
	SYS_TASK_REAL_TIME,
	SYS_TASK_BROADCAST,

	SYS_TASK_MAX
};
//用户任务
#define USER_TASK_MAX		20
#define GATHER_TASK_MAX	(USER_TASK_MAX+SYS_TASK_MAX)
//系统任务ID开始
#define SYS_TASK_ID_BEGIN	0xFFFFFF00
//优先级区段
#define TASK_PRIORITY_IDLE	1
#define TASK_PRIORITY_REALTIME	201
#define TASK_PRIORITY_USER_BEGIN	10
#define TASK_PRIORITY_USER_END		200
//任务名
#define TASK_NAME_IDLE	"IdleTask"
#define TASK_NAME_ROUTE	"RouteTask"
#define TASK_NAME_REALTIME	"RealtimeTask"
#define TASK_NAME_BROADCAST	"BroadcastTask"
#define TASK_NAME_VIP	"VipTask"
#define TASK_NAME_DAY	"DayTask"
#define TASK_NAME_MON	"MonTask"
#define TASK_NAME_CHECK	"CheckTask"
#define TASK_NAME_TERM	"TermTask"
//系统任务标识
#define MSDI_TASK_IDLE			(SYS_TASK_ID_BEGIN + SYS_TASK_IDLE)
#define MSDI_TASK_ROUTE		(SYS_TASK_ID_BEGIN + SYS_TASK_ROUTE)
#define MSDI_TASK_REALTIME		(SYS_TASK_ID_BEGIN + SYS_TASK_REAL_TIME)
#define MSDI_TASK_BROADCAST	(SYS_TASK_ID_BEGIN + SYS_TASK_BROADCAST)

struct S_TaskInfo
{
	char m_TypeName[TASKINFO_NAME_MAX];///< 任务类型名
	S_DWORD m_MSDI;///< 主站数据ID，任务之间需要唯一
	S_BYTE m_Priority;///< 优先级，越大表示优先级越高
	S_BYTE m_RetryTimes;///< 一个周期内重试次数:0表示不限次数
	S_BYTE m_MaxStartTimes;///< 最大启动次数:0表示不限次数
	S_BYTE m_PeriodVal;///< 周期值:0表示立即启动
	S_TIME_UNIT m_TimeUnit;///< 周期单位
	S_TIME m_StandardTime;///< 标准开始时间
	S_DWORD m_ExecTimeS;///< 周期内最大执行时间，单位秒,0表示不限时间
	S_DWORD m_ReportPeriodS;///< 数据主动上报周期，表计上报周期

	S_MBS m_MBS; ///< 表计屏蔽位
	S_REGISTERS m_CommRegs;///< 通用数据寄存器
	S_DIBS m_DisableRegs; ///< 寄存器屏蔽位

	string m_strPrivateData;///< 私有数据

public:
	S_TaskInfo(void);
};

enum
{
	TASK_CTRL_STATUS_INIT = 0,
	TASK_CTRL_STATUS_DIRECT,
	TASK_CTRL_STATUS_REPORT,
	TASK_CTRL_STATUS_INDIRECT,

	TASK_CTRL_STATUS_FINISH
};
struct S_TaskCtrlStatus
{
	S_TIME m_timeStart;///< 本次启动开始的实际时间
	S_TIME m_timeStdStart; ///< 本次启动开始的标准时间

	S_BYTE m_CurrTimes;///< 当前执行次数,由任务执行器修改
	S_BYTE m_TaskStatus; ///< 任务状态
	S_WORD m_idxMeter; ///< 当前正在采的测量点
	S_BYTE m_idxReg; ///< 寄存器索引
	strings m_strDatas;

public:
	S_TaskCtrlStatus(void);
};

struct S_TaskFreezeStatus
{
	bool m_Enable;
	S_DWORD m_MSDI; ///< 主站数据标识，唯一性
	S_TIME m_timeLastRaise; ///< 最后一次启动时间
	S_TIME m_timeStdStart; ///< m_SuccessfulMBS有效时间
	S_MBS m_SuccessfulMBS; ///< 记录冻结成功的表计
	S_MTBS *m_pMTBS; ///< 表计类型指针

public:
	S_TaskFreezeStatus(void);
};


#endif//__BASEB_BASETYPE_H__



