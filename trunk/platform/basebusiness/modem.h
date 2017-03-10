/**
 *  @file
 *  @brief MODEM������
 *  @author Steven Wan
 *  @date 2015-02-06
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MODEM_H__
#define __BASEB_MODEM_H__
#include "basedef.h"
#include "connect.h"

enum MODEM_TYPE
{
	MODEM_TYPE_COMMON,
	MODEM_TYPE_GPRS_COMMON,

	MODEM_TYPE_GPRS_MAX,

	MODEM_TYPE_CDMA_COMMON,

	MODEM_TYPE_CDMA_MAX,

	MODEM_TYPE_MAX
};


class C_MODEM
{
public:
	C_MODEM(MODEM_TYPE type, const char* name);
	virtual ~C_MODEM(){}

	MODEM_TYPE GetType() const{return m_Type;}
	const char* GetName() const{return m_Name.c_str();}
	void SetModemConnection(C_CONNECT *pModemConnect){m_pModemConnect = pModemConnect;}
	C_CONNECT *GetModemConnection(){return m_pModemConnect;}

	virtual int PowerOn(int PowerOnDelayS);
	virtual int PowerOff(int PowerOffDelayS);
	//���CD״̬������-1��ʾδ֪������0��ʾ��CD������1��ʾ��CD
	virtual int CheckCD();
	/** @brief AT COMMAND */
	virtual int GetIprCommand(string& strCmd, int baud);//��������Modem������
	//��ʼ�����������Ҫ�ֲ����У�step��ʾ���裬��0��ʼ������0��ʾû����һ���裬����1��ʾ������һ���裬����-1��ʾ��֧��
	virtual int GetInitCommand(string& strCmd,int step);
	virtual int GetTestCommand(string& strCmd);
	virtual int GetSignalCommand(string& strCmd);
	virtual int GetAutoConnectCommand(string& strCmd);
	virtual int GetFactoryCommand(string& strCmd);
	virtual int GetModelCommand(string& strCmd);
	virtual int GetSimCimiCommand(string& strCmd);

	virtual int ParseSignal(const string& strAnswer);
	virtual bool HasRing(const string& strAnswer);
	virtual int ParseConnectBaud(const string& strAnswer);

public:
	virtual int TestModem(int times = 3);
	virtual int InitModem(const char* initCmd = NULL);
	virtual int GetSignal(void);
	virtual int SyncModemBaud(int baud = 0);
	virtual int GetModemInfo(string &strModemFact, string &strModemModel);
	virtual int GetSimInfo(string &strSimInfo);

protected:
	int ModemStdComm(const string& strCmd, string& strAnswer);
	int ModemSendRecv(const string& strCmd, string& strAnswer);
	int ExtractModemInfo(const string &strAnswer, string &strOut);

protected:
	MODEM_TYPE m_Type;
	string m_Name;
	C_CONNECT *m_pModemConnect;
};

class C_GprsModem:public C_MODEM
{
public:
	C_GprsModem(MODEM_TYPE type, const char* name):C_MODEM(type,name){}

	//��ʼ�����������Ҫ�ֲ����У�step��ʾ���裬��0��ʼ������0��ʾû����һ���裬����1��ʾ������һ���裬����-1��ʾ��֧��
	virtual int GetInitCommand(string& strCmd,int step);
};


class C_ModemFactory
{
public:
	static C_MODEM* CreateModem(INT8U modemType);
	static void DestroyModem(C_MODEM* pModem);
};

#endif//__BASEB_MODEM_H__

