/**
 *  @file
 *  @brief �����û����Ͷ���
 *  @author Steven Wan
 *  @date 2014-07-28
 *  @version v1.0
 *  @note
 *  ����ҵ������������Ͷ���
 */
#ifndef __BASEB_BASETYPE_H__
#define __BASEB_BASETYPE_H__
#include "basedef.h"
#include "bitsetex.h"
#include "timeop.h"
#include "channel.h"

/** @brief ��̨ͨѶ������� */
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
/** @brief ��̨ͨѶͨ����� */
enum S_ChannelSN
{
	HTTX_CHANNEL_1 = 0,
	HTTX_CHANNEL_2,
	HTTX_CHANNEL_3
};

/** @brief ���ݴ洢����¼�� */
struct S_StoreMax
{
	S_WORD m_DayData; //����������¼��
	S_WORD m_MonData; //����������¼��
	S_WORD m_VipData; //�ص㻧��������¼��
	S_WORD m_CbrData; //��������������¼��
};

//
//DA����
//
class C_DA
{
public:
	C_DA(INT16U DA):m_DA1((INT8U*)&DA, 1),m_DA2(DA>>8){}
	~C_DA(){}
	
	/** @brief MPͳ��
	 *  @return 0 �ն˲�����
	 *  @return 1-8 ��ƵȲ�����
	 *  @return 0xFF ���в�����
	 */
	INT8U GetMpCount(void);
	void ResetMP(int pos){m_DA1.ClearBit(pos);}
	/** @brief MP���� */
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
//DT ����
//
class C_DT
{
public:
	C_DT(S_WORD DT):m_DT1((S_BYTE*)&DT, 1),m_DT2(DT>>8){}
	~C_DT(){}
	
	/** @brief ���ܵ�ͳ��
	 *  @return 0 �޹��ܵ�
	 *  @return 1-8 ���ܵ���λ
	 */
	S_BYTE GetFnCount(void){return m_DT1.CountBit();}
	void ResetFN(int pos){m_DT1.ClearBit(pos);}
	/** @brief ���ܵ���� pos[0-7]*/
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
	/** @brief ��PNת����DA
	 * @param[in] PN �������
	 * @return ��ЧDA
	 */
	static S_WORD PN2DA(S_WORD PN);
	/** @brief ��DAת����PN
	 * @param[in] DA ��Ϣ��
	 * @return ��ЧPN
	 */
	static S_WORD DA2PN(S_WORD DA);
};

class C_DTFN
{
public:
	/** @brief ��FNת����DT
	 * @param[in] FN ��Ϣ���ܵ�
	 * @return ��ЧDT
	 */
	static S_WORD FN2DT(S_WORD FN);
	/** @brief ��DTת����FN
	 * @param[in] DT ��Ϣ��
	 * @return ��ЧFN
	 */
	static S_WORD DT2FN(S_WORD DT);
};

/** @brief ����������Ϣ */
struct S_FlowInfo
{
	//#����ʹ�ܿ���
	bool m_FlowCtrlEnable;
	//#��½����
	S_WORD m_LogonInterS;
	S_WORD m_LogonRetryTimes;
	S_WORD m_LogonFailedTimes;
	//#Socket����
	S_WORD m_SocketConnectInterS;
	S_WORD m_SocketConnectRetryTimes;
	S_WORD m_SocketConnectFailedTimes;
	//#���ſ���
	S_WORD m_DialInterS;
	S_WORD m_DialRetryTimes;
	S_WORD m_DialFrequencyM;
	S_WORD m_DialFailedTimes;
	//#��վͨѶ��
	BitSetEx<32> m_MSConnectDate;
	BitSetEx<24> m_MSConnectTime;
	//#������������
	S_DWORD m_DayFlowMax; ///< #0��ʾ��������������
	S_DWORD m_MonFlowMax; ///< #0��ʾ��������������

	//����ʧ�ܴ�������
	S_WORD m_HeartBeatFailedTimes;

public:	
	void Init(void);
	void ClearFailedInfo(void);
};

/** @brief ������Ϣ */
#define PPP_INFO_STRING_LEN_MAX	36
struct S_DialInfo
{
	S_WORD m_CheckInterM;//����������ӣ�
	S_WORD m_CheckFailTimes;//���ʧ�ܷ�ֵ�������������ֵ�μ��ʧ�ܣ�������
	char m_Apn[PPP_INFO_STRING_LEN_MAX];//APN
	char m_TeleNo[PPP_INFO_STRING_LEN_MAX];//�绰���룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char m_User[PPP_INFO_STRING_LEN_MAX];//�����û���ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
	char m_Pwd[PPP_INFO_STRING_LEN_MAX];//�������룬ΪCONS_DEFAULT_KEYʱ��ʾ����Ĭ��ֵ
};

/** @brief PPP����ͳ�� */
struct S_PppPackage
{
	S_TIME m_timeInit;
	INT64U m_LastRecvPackages;//���հ���Ŀ
	INT64U m_LastSendPackages;//���Ͱ���Ŀ
	INT64U m_TotalRecvPackages;//�ܵĽ��հ���Ŀ
	INT64U m_TotalSendPackages;//�ܵķ��Ͱ���Ŀ
};

/** @brief ��ƹ�Լ���ƶ��弯 */
//{{{
#define MRULE_NAME_GB645_1997 "GB645-1997"
#define MRULE_NAME_GB645_2007 "GB645-2007"
#define MRULE_NAME_JLCY	"ACGather"

//}}}

/** @brief ���ݱ��Ľṹ */
struct S_DATAGRAM
{
	string m_strProtocol; ///< Э������
	string m_strUserData; ///< Э�鱨��
};

/** @brief �ն˵�ַ�ṹ */
#define TERM_ADDR_LEN_MAX		6
struct S_TERMINAL
{
	string m_strProtocol; ///< Э������
	string m_strAddress; ///< �ն˵�ַ
};
struct S_TERMINALS:public vector<S_TERMINAL>
{
};
bool operator<(const S_TERMINAL &left, const S_TERMINAL &right);

//
//��ƼĴ�������
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
/** @brief ��ƼĴ������� */
struct S_REGISTER
{
	S_DWORD m_ID;///< �Ĵ���ID
	S_BYTE m_Type; ///< ֵ���� MR_TYPE_
	S_BYTE m_Len; ///< ֵ���ȣ��ֽ������䳤��0��ʾ
	S_BYTE m_Format; ///< �������,BCD��Ϊ����С��λ,��0x62
	S_BYTE m_Reserve; ///< ����

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
//��ƻ�����Ϣ
//
struct S_MeterBase
{
	string m_strMeterAddr;
	S_COMSET m_ComSet; 
	int m_MeterRuleID;
};

//
//������λͼ�ṹ
//
#define BASEB_MP_COUNT_MAX	1201
struct S_MPBS:public BitSetEx<BASEB_MP_COUNT_MAX> //MP����λ����
{
};
struct S_MBS:public BitSetEx<BASEB_MP_COUNT_MAX> //Meter����λ����
{
};
struct S_MTBS
{
	S_MBS m_AllMBS;///< ��Ч�ĵ��,���е��
	S_MBS m_485MBS;///< ֱ����,�ն�ͨ��485��
	S_MBS m_ZBMBS;///< ��ֱ����,�ն�ͨ���ز������߳�
//	S_MBS m_Vip485MBS;///< �ص㻧ֱ����
	S_MBS m_VipZBMBS;///< ���ص㻧ֱ����
};

//
//��վ������ӳ��ͨ�üĴ���
//
#define MSDI_COMM_REG_MAX	32
struct S_DIBS:public BitSetEx<MSDI_COMM_REG_MAX>
{
};

//
//�ɼ�������Ϣ
//
#define TASKINFO_NAME_MAX	32
//ϵͳ��������������
enum
{
	SYS_TASK_IDLE,
	SYS_TASK_ROUTE,
	SYS_TASK_REAL_TIME,
	SYS_TASK_BROADCAST,

	SYS_TASK_MAX
};
//�û�����
#define USER_TASK_MAX		20
#define GATHER_TASK_MAX	(USER_TASK_MAX+SYS_TASK_MAX)
//ϵͳ����ID��ʼ
#define SYS_TASK_ID_BEGIN	0xFFFFFF00
//���ȼ�����
#define TASK_PRIORITY_IDLE	1
#define TASK_PRIORITY_REALTIME	201
#define TASK_PRIORITY_USER_BEGIN	10
#define TASK_PRIORITY_USER_END		200
//������
#define TASK_NAME_IDLE	"IdleTask"
#define TASK_NAME_ROUTE	"RouteTask"
#define TASK_NAME_REALTIME	"RealtimeTask"
#define TASK_NAME_BROADCAST	"BroadcastTask"
#define TASK_NAME_VIP	"VipTask"
#define TASK_NAME_DAY	"DayTask"
#define TASK_NAME_MON	"MonTask"
#define TASK_NAME_CHECK	"CheckTask"
#define TASK_NAME_TERM	"TermTask"
//ϵͳ�����ʶ
#define MSDI_TASK_IDLE			(SYS_TASK_ID_BEGIN + SYS_TASK_IDLE)
#define MSDI_TASK_ROUTE		(SYS_TASK_ID_BEGIN + SYS_TASK_ROUTE)
#define MSDI_TASK_REALTIME		(SYS_TASK_ID_BEGIN + SYS_TASK_REAL_TIME)
#define MSDI_TASK_BROADCAST	(SYS_TASK_ID_BEGIN + SYS_TASK_BROADCAST)

struct S_TaskInfo
{
	char m_TypeName[TASKINFO_NAME_MAX];///< ����������
	S_DWORD m_MSDI;///< ��վ����ID������֮����ҪΨһ
	S_BYTE m_Priority;///< ���ȼ���Խ���ʾ���ȼ�Խ��
	S_BYTE m_RetryTimes;///< һ�����������Դ���:0��ʾ���޴���
	S_BYTE m_MaxStartTimes;///< �����������:0��ʾ���޴���
	S_BYTE m_PeriodVal;///< ����ֵ:0��ʾ��������
	S_TIME_UNIT m_TimeUnit;///< ���ڵ�λ
	S_TIME m_StandardTime;///< ��׼��ʼʱ��
	S_DWORD m_ExecTimeS;///< ���������ִ��ʱ�䣬��λ��,0��ʾ����ʱ��
	S_DWORD m_ReportPeriodS;///< ���������ϱ����ڣ�����ϱ�����

	S_MBS m_MBS; ///< �������λ
	S_REGISTERS m_CommRegs;///< ͨ�����ݼĴ���
	S_DIBS m_DisableRegs; ///< �Ĵ�������λ

	string m_strPrivateData;///< ˽������

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
	S_TIME m_timeStart;///< ����������ʼ��ʵ��ʱ��
	S_TIME m_timeStdStart; ///< ����������ʼ�ı�׼ʱ��

	S_BYTE m_CurrTimes;///< ��ǰִ�д���,������ִ�����޸�
	S_BYTE m_TaskStatus; ///< ����״̬
	S_WORD m_idxMeter; ///< ��ǰ���ڲɵĲ�����
	S_BYTE m_idxReg; ///< �Ĵ�������
	strings m_strDatas;

public:
	S_TaskCtrlStatus(void);
};

struct S_TaskFreezeStatus
{
	bool m_Enable;
	S_DWORD m_MSDI; ///< ��վ���ݱ�ʶ��Ψһ��
	S_TIME m_timeLastRaise; ///< ���һ������ʱ��
	S_TIME m_timeStdStart; ///< m_SuccessfulMBS��Чʱ��
	S_MBS m_SuccessfulMBS; ///< ��¼����ɹ��ı��
	S_MTBS *m_pMTBS; ///< �������ָ��

public:
	S_TaskFreezeStatus(void);
};


#endif//__BASEB_BASETYPE_H__



