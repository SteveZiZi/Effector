/** @file
 *  @brief 系统配置加载和保存
 *  @author Steven Wan
 *  @date date 2015/01/06
 *  @version 0.1
 *  定义系统配置加载和保存函数
 */
#include "ini_sys.h"
#include "inifileb.h"
#include "timeop.h"
#include "version.h"
#include "flowctrl.h"
#include "status.h"

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_SYSCFG_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("RTUA", "50010001", CFGF_NOCASE),
	CFG_STR("SysUser", "root", CFGF_NOCASE),
	CFG_STR("SysPassword", "123456", CFGF_NOCASE),
	CFG_BOOL("InitIRDA", cfg_false, CFGF_NOCASE),
	CFG_INT("HeartBeatRetryTimes", 3, CFGF_NOCASE),
	CFG_INT("ReportAgainIntervalM", 120, CFGF_NOCASE),
	CFG_END()
};




/** @brief 系统运行信息参数结构体 */
cfg_opt_t OPT_RUNNING_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("ReleaseDate", "2009-09-09", CFGF_NOCASE),
	CFG_STR("HWVersion", "GZNF-CQ01-DJGZ33-6100", CFGF_NOCASE),
	CFG_STR("SWVersion", "GZNF-CQ01-DJGZ33-6100", CFGF_NOCASE),
	CFG_STR("MPVersion", "0.00", CFGF_NOCASE),
	CFG_STR("PowerCutDate", "2010-6-6", CFGF_NOCASE),
	CFG_STR("PowerCutTime", "10:30:30", CFGF_NOCASE),
	CFG_STR("RouterType", "XC", CFGF_NOCASE),
	CFG_STR("ChipVersion", "V1",CFGF_NOCASE),
	CFG_STR("ModemFactory", "unknow",CFGF_NOCASE),
	CFG_STR("ModemModel", "unknow",CFGF_NOCASE),
	CFG_STR("SimIP", "0.0.0.0", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadRunInformation
 *  @brief 加载系统版本备份
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadRunInformation(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_RUNNING_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	S_SYSRUN &SysRun = pStatus->m_SysRun;

	strncpy(SysRun.m_ReleaseDate, cfg_getstr(cfg, "ReleaseDate"), sizeof(SysRun.m_ReleaseDate)-1);
	strncpy(SysRun.m_HWVersion, cfg_getstr(cfg, "HWVersion"), sizeof(SysRun.m_HWVersion)-1);
	strncpy(SysRun.m_SWVersion, cfg_getstr(cfg, "SWVersion"), sizeof(SysRun.m_SWVersion)-1);
	strncpy(SysRun.m_MPVersion, cfg_getstr(cfg, "MPVersion"), sizeof(SysRun.m_MPVersion)-1);

	int year=1900,month=1,day=1,hour=0,minute=0,second=0;
	sscanf(cfg_getstr(cfg,"PowerCutDate"), "%d-%d-%d", &year, &month, &day);
	sscanf(cfg_getstr(cfg,"PowerCutTime"), "%d:%d:%d", &hour, &minute, &second);
	S_DATE date;
	date.m_Year = year;
	date.m_Mon  = month;
	date.m_Day = day;
	date.m_Hour = hour;
	date.m_Min  = minute;
	date.m_Sec  = second;
	SysRun.m_timePowerCut = C_TIME::MakeTime(date);

	strncpy(SysRun.m_RouterType, cfg_getstr(cfg, "RouterType"), sizeof(SysRun.m_RouterType)-1);
	strncpy(SysRun.m_ChipVersion, cfg_getstr(cfg, "ChipVersion"), sizeof(SysRun.m_ChipVersion)-1);
	strncpy(SysRun.m_ModemFactory, cfg_getstr(cfg, "ModemFactory"), sizeof(SysRun.m_ModemFactory)-1);
	strncpy(SysRun.m_ModemModel, cfg_getstr(cfg, "ModemModel"), sizeof(SysRun.m_ModemModel)-1);
	
	int ip0 = 0, ip1 = 0, ip2 = 0, ip3 = 0;
	sscanf(cfg_getstr(cfg,"SimIP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	INT8U *IP = (INT8U*)&SysRun.m_SimIP;
	IP[0] = (INT8U)ip0, IP[1] = (INT8U)ip1, IP[2] = (INT8U)ip2, IP[3] = (INT8U)ip3;

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveRunInformation
 *  @brief 保存系统版本备份
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveRunInformation(S_STATUS *pStatus, const char *fname)
{
	string strInfo;
	char Buffer[32];

	S_SYSRUN &SysRun = pStatus->m_SysRun;

	strInfo.append("#注意：本文件由系统自动维护，不得人为更改！！！\r\n\r\n");
	strInfo.append("#版本信息\r\n");
	strInfo.append("ReleaseDate=").append(CONS_SW_REAL_DATE).append(" #版本发布日期\r\n");
	strInfo.append("HWVersion=").append(CONS_HARD_VERSION).append(" #硬件系统版本号\r\n");
	memset(Buffer, 0x00, sizeof(Buffer));
	strInfo.append("SWVersion=").append(CONS_SOFT_VERSION).append(" #软件系统版本号\r\n");
	strInfo.append("MPVersion=").append(CONS_MAIN_VERSION).append(" #主程序版本号\r\n\r\n");

	strInfo.append("#系统前一次掉电时间\r\n");
	S_DATE date;
	C_TIME::LocalTime(SysRun.m_timePowerCut, date);
	sprintf(Buffer, "%d-%d-%d", date.m_Year, date.m_Mon, date.m_Day);
	strInfo.append("PowerCutDate=").append(Buffer).append(" #年-月-日\r\n");
	sprintf(Buffer, "%d:%d:%d", date.m_Hour, date.m_Min, date.m_Sec);
	strInfo.append("PowerCutTime=").append(Buffer).append(" #时:分:秒\r\n\r\n");

	strInfo.append("#下行模块信息\r\n");
	strInfo.append("RouterType=\"").append(SysRun.m_RouterType).append("\" #XC,DR,YXD,RB,SR,LME,DX\r\n");
	strInfo.append("ChipVersion=\"").append(SysRun.m_ChipVersion).append("\" #模块版本号,00为无效\r\n\r\n");

	strInfo.append("#MODEM信息\r\n");
	strInfo.append("ModemFactory=\"").append(SysRun.m_ModemFactory).append("\" #MODEM生产厂家\r\n");
	strInfo.append("ModemModel=\"").append(SysRun.m_ModemModel).append("\" #MODEM型号\r\n");
	INT8U *IP = (INT8U*)&SysRun.m_SimIP;
	sprintf(Buffer, "%d.%d.%d.%d", IP[0], IP[1], IP[2], IP[3]);
	strInfo.append("SimIP=\"").append(Buffer).append("\" #Sim卡IP\r\n\r\n");
	
	return C_IniBase::SaveSystemFile(strInfo, fname);
}

/** @brief 系统配置参数结构体 */
cfg_opt_t OPT_PORTMAP_SETTINGS[] =
{	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_INT("VCom0",  0, CFGF_NOCASE),
	CFG_INT("VCom1",  0, CFGF_NOCASE),
	CFG_INT("VCom2",  0, CFGF_NOCASE),
	CFG_INT("VCom3",  0, CFGF_NOCASE),
	CFG_INT("VCom4",  0, CFGF_NOCASE),
	CFG_INT("VCom5",  0, CFGF_NOCASE),
	CFG_INT("VCom6",  0, CFGF_NOCASE),
	CFG_INT("VCom7",  0, CFGF_NOCASE),
	CFG_INT("VCom8",  0, CFGF_NOCASE),
	CFG_INT("VCom9",  0, CFGF_NOCASE),
	CFG_INT("VCom10", 0, CFGF_NOCASE),
	CFG_INT("VCom11", 0, CFGF_NOCASE),
	CFG_INT("VCom12", 0, CFGF_NOCASE),
	CFG_INT("VCom13", 0, CFGF_NOCASE),
	CFG_INT("VCom14", 0, CFGF_NOCASE),
	CFG_INT("VCom15", 0, CFGF_NOCASE),
	CFG_INT("VCom16", 0, CFGF_NOCASE),
	CFG_INT("VCom17", 0, CFGF_NOCASE),
	CFG_INT("VCom18", 0, CFGF_NOCASE),
	CFG_INT("VCom19", 0, CFGF_NOCASE),
	CFG_INT("VCom20", 0, CFGF_NOCASE),
	CFG_INT("VCom21", 0, CFGF_NOCASE),
	CFG_INT("VCom22", 0, CFGF_NOCASE),
	CFG_INT("VCom23", 0, CFGF_NOCASE),
	CFG_INT("VCom24", 0, CFGF_NOCASE),
	CFG_INT("VCom25", 0, CFGF_NOCASE),
	CFG_INT("VCom26", 0, CFGF_NOCASE),
	CFG_INT("VCom27", 0, CFGF_NOCASE),
	CFG_INT("VCom28", 0, CFGF_NOCASE),
	CFG_INT("VCom29", 0, CFGF_NOCASE),
	CFG_INT("VCom30", 0, CFGF_NOCASE),
	CFG_INT("VCom31", 0, CFGF_NOCASE),
	CFG_INT("VCom32",  0, CFGF_NOCASE),
	CFG_INT("VCom33",  0, CFGF_NOCASE),
	CFG_INT("VCom34",  0, CFGF_NOCASE),
	CFG_INT("VCom35",  0, CFGF_NOCASE),
	CFG_INT("VCom36",  0, CFGF_NOCASE),
	CFG_INT("VCom37",  0, CFGF_NOCASE),
	CFG_INT("VCom38",  0, CFGF_NOCASE),
	CFG_INT("VCom39",  0, CFGF_NOCASE),
	CFG_INT("VCom40", 0, CFGF_NOCASE),
	CFG_INT("VCom41", 0, CFGF_NOCASE),
	CFG_INT("VCom42", 0, CFGF_NOCASE),
	CFG_INT("VCom43", 0, CFGF_NOCASE),
	CFG_INT("VCom44", 0, CFGF_NOCASE),
	CFG_INT("VCom45", 0, CFGF_NOCASE),
	CFG_INT("VCom46", 0, CFGF_NOCASE),
	CFG_INT("VCom47", 0, CFGF_NOCASE),
	CFG_INT("VCom48", 0, CFGF_NOCASE),
	CFG_INT("VCom49", 0, CFGF_NOCASE),
	CFG_INT("VCom50", 0, CFGF_NOCASE),
	CFG_INT("VCom51", 0, CFGF_NOCASE),
	CFG_INT("VCom52", 0, CFGF_NOCASE),
	CFG_INT("VCom53", 0, CFGF_NOCASE),
	CFG_INT("VCom54", 0, CFGF_NOCASE),
	CFG_INT("VCom55", 0, CFGF_NOCASE),
	CFG_INT("VCom56", 0, CFGF_NOCASE),
	CFG_INT("VCom57", 0, CFGF_NOCASE),
	CFG_INT("VCom58", 0, CFGF_NOCASE),
	CFG_INT("VCom59", 0, CFGF_NOCASE),
	CFG_INT("VCom60", 0, CFGF_NOCASE),
	CFG_INT("VCom61", 0, CFGF_NOCASE),
	CFG_INT("VCom62", 0, CFGF_NOCASE),
	CFG_INT("VCom63", 0, CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadPortMap
 *  @brief 加载系统配置参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadPortMap(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_PORTMAP_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSystemFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	S_SYSCFG &SysConfig = pStatus->m_SysConfig;

	for (size_t i = 0; i < SYSTEM_SERIAL_PORT_MAX; i++)
	{
		char CfgItem[12];
		sprintf(CfgItem, "VCom%d", i);
		SysConfig.m_PortMap[i] = (S_BYTE)cfg_getint(cfg, CfgItem);
	}

	cfg_free(cfg);
	return 0;
}

/** @brief IP控制数据块*/
cfg_opt_t OPT_IPINFO_INFO[] =
{
	//参数名 ,参数默认值  ,标志	configuration file is case insensitive
	CFG_STR("IP", "192.168.0.100", CFGF_NOCASE),
	CFG_STR("NETMASK", "255.255.255.0", CFGF_NOCASE),
	CFG_STR("GW", "192.168.0.1", CFGF_NOCASE),
	CFG_INT("ProxyType", 0, CFGF_NOCASE),
	CFG_STR("ProxyIP", "192.168.9.1", CFGF_NOCASE),//徐冠+将GPRS的设置集中写
	CFG_INT("ProxyPort", 10000, CFGF_NOCASE),
	CFG_INT("ProxyLink", 0, CFGF_NOCASE),
	CFG_STR("UserName", "", CFGF_NOCASE),
	CFG_STR("PassWord", "", CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniSys::LoadTermIPConfig
 *  @brief 加载IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::LoadTermIPConfig(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg;
	int ip0,ip1,ip2,ip3;

	cfg = cfg_init(OPT_IPINFO_INFO, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	struct S_IpConfig &IpInfo = pStatus->m_IpCofig;

	sscanf(cfg_getstr(cfg,"IP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_TermIP[0] = ip0; IpInfo.m_TermIP[1] = ip1; IpInfo.m_TermIP[2] = ip2; IpInfo.m_TermIP[3] = ip3; 
	sscanf(cfg_getstr(cfg,"NETMASK"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_NetMask[0] = ip0; IpInfo.m_NetMask[1] = ip1; IpInfo.m_NetMask[2] = ip2; IpInfo.m_NetMask[3] = ip3; 
	sscanf(cfg_getstr(cfg,"GW"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_GateWay[0] = ip0; IpInfo.m_GateWay[1] = ip1; IpInfo.m_GateWay[2] = ip2; IpInfo.m_GateWay[3] = ip3; 

	IpInfo.m_ProxyType = (INT8U)cfg_getint(cfg, "ProxyType");
	sscanf(cfg_getstr(cfg,"ProxyIP"), "%d.%d.%d.%d", &ip0, &ip1, &ip2, &ip3);
	IpInfo.m_ProxyIP[0] = ip0; IpInfo.m_ProxyIP[1] = ip1; IpInfo.m_ProxyIP[2] = ip2; IpInfo.m_ProxyIP[3] = ip3; 
	IpInfo.m_ProxyPort = (INT16U)cfg_getint(cfg, "ProxyPort");
	IpInfo.m_ProxyLink = (INT8U)cfg_getint(cfg, "ProxyLink");
	memset(IpInfo.m_UserName, 0x00, sizeof(IpInfo.m_UserName));
	strncpy((char *)IpInfo.m_UserName, cfg_getstr(cfg, "UserName"), 20);
	memset(IpInfo.m_Password, 0x00, sizeof(IpInfo.m_Password));
	strncpy((char *)IpInfo.m_Password, cfg_getstr(cfg, "PassWord"), 20);

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveTermIPConfig
 *  @brief 保存IP信息配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] cfgFile:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniSys::SaveTermIPConfig(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	struct S_IpConfig &IpInfo = pStatus->m_IpCofig;

	strFileInfo.append("#ipconfig.ini\r\n");
	strFileInfo.append("#function: ip config\r\n");

	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_TermIP[0], IpInfo.m_TermIP[1], IpInfo.m_TermIP[2], IpInfo.m_TermIP[3]);
	strFileInfo.append("\r\nIP = ").append(buffer).append(" #集中器本地网口IP");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_NetMask[0], IpInfo.m_NetMask[1], IpInfo.m_NetMask[2], IpInfo.m_NetMask[3]);
	strFileInfo.append("\r\nNETMASK = ").append(buffer).append(" #子网掩码");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_GateWay[0], IpInfo.m_GateWay[1], IpInfo.m_GateWay[2], IpInfo.m_GateWay[3]);
	strFileInfo.append("\r\nGW = ").append(buffer).append(" #网关");

	sprintf(buffer, "%d", IpInfo.m_ProxyType);
	strFileInfo.append("\r\nProxyType = ").append(buffer).append(" #0～3依次表示:不使用代理,http connect代理,socks4代理,socks5代理");
	sprintf(buffer, "%d.%d.%d.%d", IpInfo.m_ProxyIP[0], IpInfo.m_ProxyIP[1], IpInfo.m_ProxyIP[2], IpInfo.m_ProxyIP[3]);
	strFileInfo.append("\r\nProxyIP = ").append(buffer).append(" #代理服务器IP");
	sprintf(buffer, "%d", IpInfo.m_ProxyPort);
	strFileInfo.append("\r\nProxyPort = ").append(buffer);
	sprintf(buffer, "%d", IpInfo.m_ProxyLink);
	strFileInfo.append("\r\nProxyLink = ").append(buffer).append(" #0～1依次表示：无需验证、需要用户名/密码");
	strFileInfo.append("\r\nUserName = \"").append((char *)IpInfo.m_UserName).append("\"");
	strFileInfo.append("\r\nPassWord = \"").append((char *)IpInfo.m_Password).append("\"");

	strFileInfo.append("\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}


