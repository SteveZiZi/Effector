/** @file
 *  @brief ���������
 *  @author Steven Wan
 *  @date 2015-01/30
 *  @version 0.1
 *  @see appcmd.h
 *  @note
 *  1����������̳��Զ���̿��
 *  2�����ļ�����������ɫ���������
 */
#include "nwappcmd.h"
#include "cfg_proc.h"
#include "htrace.h"
#include "basebver.h"
//#include "nwbver.h"
#include "nwdaemon.h"
#include "httx.h"
#include "dbcj.h"
#include "ppp.h"
#include "lcd.h"
#include "jlcy.h"
#include "fhkz.h"
#include "dspdevice.h"

C_NwAppCmd::C_NwAppCmd(int argc, char *argv[]):C_AppCmd(argc, argv)
{
}

C_NwAppCmd::~C_NwAppCmd()
{
}

C_SERVER *C_NwAppCmd::GetServer(const char *pServerName)
{
	static C_SERVER *pDaemonServ = NULL;
	static C_SERVER *phttx1Serv = NULL;
	static C_SERVER *phttx2Serv = NULL;
	static C_SERVER *phttx3Serv = NULL;
	static C_SERVER *pdbcjServ  = NULL;
	static C_SERVER *pPppServ = NULL;
	static C_SERVER *pLcdServ = NULL;
	static C_SERVER *pJlcyServ = NULL;
	static C_SERVER *pFhkzServ = NULL;

	if (strcasecmp(MP_PROCESS_HTTX1, pServerName) == 0)
	{
		if (phttx1Serv == NULL)
			phttx1Serv = new C_Httx1Server;
		return phttx1Serv;
	}
	if (strcasecmp(MP_PROCESS_HTTX2, pServerName) == 0)
	{
		if (phttx2Serv == NULL)
			phttx2Serv = new C_Httx2Server;
		return phttx2Serv;
	}
	if (strcasecmp(MP_PROCESS_HTTX3, pServerName) == 0)
	{
		if (phttx3Serv == NULL)
			phttx3Serv = new C_Httx3Server;
		return phttx3Serv;
	}
	if (strcasecmp(MP_PROCESS_DBCJ, pServerName) == 0)
	{
		if (pdbcjServ == NULL)
			pdbcjServ = new C_DbcjServer;
		return pdbcjServ;
	}
	if (strcasecmp(MP_PROCESS_PPP, pServerName) == 0)
	{
		if (pPppServ == NULL)
			pPppServ = new C_PppServer;
		return pPppServ;
	}
	if (strcasecmp(MP_PROCESS_LCD, pServerName) == 0)
	{
		if (pLcdServ == NULL)
			pLcdServ = new C_LcdServer;
		return pLcdServ;
	}
	if (strcasecmp(MP_PROCESS_JLCY, pServerName) == 0)
	{
		if (pJlcyServ == NULL)
			pJlcyServ = new C_JlcyServer;
		return pJlcyServ;
	}
	if (strcasecmp(MP_PROCESS_FHKZ, pServerName) == 0)
	{
		if (pFhkzServ == NULL)
			pFhkzServ = new C_FhkzServer;
		return pFhkzServ;
	}
	if (strcasecmp(MP_PROCESS_DAEMON, pServerName) == 0)
	{
		if (pDaemonServ == NULL)
			pDaemonServ = new C_NwDaemonServer;
		return pDaemonServ;
	}
	return C_AppCmd::GetServer(pServerName);
}

int C_NwAppCmd::ListVersion(void)
{
	C_AppCmd::ListVersion();
	logn<<"����ҵ���汾:"<<BASEB_GetVersion()<<endl;
	logn<<"����ҵ��ⷢ������:"<<BASEB_GetReleaseDate()<<endl;
//	logn<<"����ҵ���汾:"<<NWB_GetVersion()<<endl;
//	logn<<"����ҵ��ⷢ������:"<<NWB_GetReleaseDate()<<endl;
	logn<<"�����ļ����汾:"<<"unknow"<<endl;
	logn<<"�����ļ�����������:"<<"unknow"<<endl;
	logn<<"������汾:"<<MP_GetSoftwareVersion()<<endl;
	logn<<"�����򷢲�����:"<<MP_GetSoftwareReleaseDate()<<endl;
	logn<<"Ӳ���汾:"<<MP_GetHardwareVersion()<<endl;
	logn<<"Ӳ����������:"<<MP_GetHardwareReleaseDate()<<endl;

	return 0;
}


