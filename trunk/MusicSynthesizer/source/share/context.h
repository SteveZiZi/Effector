/** @file
 *  @brief �û���ϵͳ������������
 *  @author Steven Wan
 *  @date 2015/01/07
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_ContextBase�࣬ע�����еĳ��󷽷������ء�
 */
#ifndef __NW_CONTEXT_H__
#define __NW_CONTEXT_H__
#include "cfg_cont.h"
#include "sysipc.h"

enum
{
	LCD_INDICATOR_PPP_MODEM_RESET, //	"MODEM�����ϵ�...",
	LCD_INDICATOR_PPP_MODEM_CHECK, //	"���MODEM...    ",
	LCD_INDICATOR_PPP_MODEM_SIGNAL, //	"��������ź�... "
	LCD_INDICATOR_PPP_DIALING, //	"PPP���ڲ���...  ",
	LCD_INDICATOR_PPP_CONNECTED, //	"PPP���ӳɹ�...  ",
	LCD_INDICATOR_PPP_FAILED, //	"PPP����ʧ��     "

	LCD_INDICATOR_PPP_MAX
};
enum
{
	LCD_INDICATOR_DBCJ_GATHER_FINISH, //	"��������        ",
	LCD_INDICATOR_DBCJ_GATHER, //	"���ڳ�����...   ",
	LCD_INDICATOR_DBCJ_ROUTER, //	"����·����...   ",
	LCD_INDICATOR_DBCJ_REPORT, //	"���ڱ����ϱ���..",
	LCD_INDICATOR_DBCJ_REALCMD, //	"��Ӧʵʱ����... ",
	LCD_INDICATOR_DBCJ_RECOGNISE_FB, //	"����ʶ������ģ��",
	LCD_INDICATOR_DBCJ_FB_UNKNOW, //	"δ֪������ģ��  ",
	LCD_INDICATOR_DBCJ_FB_OK, //	"����ģ��ʶ��ɹ�"

	LCD_INDICATOR_DBCJ_MAX
};

class C_CONTEXT:public C_ContextBase
{
public:
	virtual ~C_CONTEXT(){}
	static C_CONTEXT &Instance(void);

	/** @brief ���ó�Ա��Ϣ��
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
	 */
	virtual bool SetInformation(void);
	/** @brief ��ʼ��ϵͳ������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool InitContext(void);
	/** @brief �������õ�ϵͳ������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool LoadContext(void);
	/** @brief ��������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ParamParse(void);

public:
	void SetReconnectFlag(bool fConnect){m_pMyContext->m_ConnectStatus.m_Reconnect = fConnect;}
	bool GetReconnectFlag(void){return m_pMyContext->m_ConnectStatus.m_Reconnect;}

	S_ConnectStatus *GetConnectStatus(void){return &m_pMyContext->m_ConnectStatus;}
	S_HttxStatus *GetHttxStatus(S_HttxSN httxSN){return &m_pMyContext->m_HttxStatus[httxSN];}
	S_PppStatus *GetPppStatus(void){return &m_pMyContext->m_PppStatus;}


public:
	/** @brief ����LCDָʾ��
	 *  @param[in] pProcessName ������
	 *  @param[in] Indicator �μ�LCD_INDICATOR_
	 */
	bool SetLcdBottomInfoIndicator(const char *pProcessName, S_BYTE Indicator);
	const char *GetLcdBottomInfo(const char *pProcessName);
public:/*���вɼ�������ݽṹ*/

	
protected:
	C_CONTEXT(void){m_pMyContext = NULL;}

protected:
	S_CONTEXT *m_pMyContext;
};

#define CONTEXT C_CONTEXT::Instance()

#endif//__NW_CONTEXT_H__
