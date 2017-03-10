/** @file
 *  @brief 系统服务加载和保存
 *  @author Steven Wan
 *  @date date 2015/01/07
 *  @version 0.1
 *  定义系统服务加载和保存函数
 */
#include "ini_serv.h"
#include "inifileb.h"
#include "osrelate.h"
#include "sysctrl.h"
#include "cfg_proc.h"
#include "htrace.h"
#include "strings.h"
#include "basealgo.h"
#include "Arithmetic.h"

/** @brief 通道配置结构体 */
cfg_opt_t OPT_CHANNL_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_false,CFGF_NOCASE),
	CFG_STR("Name", "Serial", CFGF_NONE),
	CFG_STR("Type", "Com", CFGF_NONE),
	CFG_STR("Setting", "7,1200.8.1.2,Serial,NoCD", CFGF_NONE),
	CFG_INT("ReadTimeoutMS", 3000, CFGF_NONE),
	CFG_INT("WriteTimeoutMS", 3000, CFGF_NONE),
	CFG_INT("SubReadTimeoutMS", 50, CFGF_NONE),
	CFG_INT("CharReadTimeoutMS", 50, CFGF_NONE),
	CFG_INT("IdleTimeoutS", 0, CFGF_NONE),
	CFG_INT("ConnectTimeoutS", 30, CFGF_NONE),
	CFG_INT("ConnectInterS", 5, CFGF_NONE),
	CFG_INT("PackageLenMax", 500, CFGF_NONE),

	CFG_END()
};
cfg_opt_t OPT_CHANNLS_SETTINGS[] =
{
	CFG_SEC("Channel", OPT_CHANNL_SETTINGS, CFGF_MULTI | CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadChannel
 *  @brief 加载指定通道
 *  @param[out] pChannl:通道指针
 *  @param[in] cfgChannel:通道字串配置信息
 */
void C_IniServer::LoadChannel(cfg_t* cfgChannel, struct S_CHANNEL *pChannl)
{
	pChannl->m_Enable = true;
	memset(pChannl->m_Name, 0x00, sizeof(pChannl->m_Name));
	strncpy((char *)pChannl->m_Name, cfg_getstr(cfgChannel,"Name"),sizeof(pChannl->m_Name)-1);
	pChannl->m_Type=(S_BYTE)MakeChannelTypeVal(cfg_getstr(cfgChannel, "Type"));
	PortSettingParse(pChannl, cfg_getstr(cfgChannel,"Setting"));

	pChannl->m_ReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "ReadTimeoutMS");
	pChannl->m_WriteTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "WriteTimeoutMS");
	pChannl->m_SubReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "SubReadTimeoutMS");
	pChannl->m_CharReadTimeoutMS=(S_WORD)cfg_getint(cfgChannel, "CharReadTimeoutMS");
	pChannl->m_ConnectTimeoutS=(S_BYTE)cfg_getint(cfgChannel, "ConnectTimeoutS");
	pChannl->m_ConnectInterS=(S_WORD)cfg_getint(cfgChannel, "ConnectInterS");
	pChannl->m_IdleTimeoutS=(S_WORD)cfg_getint(cfgChannel, "IdleTimeoutS");
	pChannl->m_PackageLenMax=(S_WORD)cfg_getint(cfgChannel, "PackageLenMax");
}

char *C_IniServer::MakeChannelTypeKey(int val)
{
	if (val == CHANNEL_TYPE_NET) return "Net";
	if (val == CHANNEL_TYPE_COM) return "Com";
	return "Unknow";
}

int C_IniServer::MakeChannelTypeVal(char *pKey)
{
	if (strcasecmp(pKey, "Net") == 0) return CHANNEL_TYPE_NET;
	if (strcasecmp(pKey, "Com") == 0) return CHANNEL_TYPE_COM;
	return CHANNEL_TYPE_UNKNOW;
}

/** @fn ServerCfg::LoadUpChannl
 *  @brief 加载上行通道参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadUpChannl(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg = cfg_init(OPT_CHANNLS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	int ChannlIdx = 0;
	cfg_t* cfgChannel;
	pStatus->m_UpChannels.Init();
	for(S_DWORD i = 0; i < cfg_size(cfg, "Channel"); i++)
	{
		cfgChannel = cfg_getnsec(cfg, "Channel", i);
		if (cfg_getbool(cfgChannel, "Enable") == cfg_false)
			continue;

		struct S_CHANNEL *pChannl = pStatus->m_UpChannels.FindChannel(ChannlIdx++);
		if (pChannl == NULL)
			break;

		LoadChannel(cfgChannel, pChannl);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveUpChannl
 *  @brief 保存上行通道参数配置
 *  @param[in] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveUpChannl(S_STATUS *pStatus, const char *fname)
{
	char Buffer[64];
	string strFileInfo;

	strFileInfo.append("#note:主站,PPP,级联通道\r\n"
		"#ChannelType:Net,Com\r\n"
		"#NetType: TcpClient,TcpServer,UdpClient,UdpServer,Tcp,Udp\r\n"
		"#PPP: NoPPP,NeedPPP\r\n"
		"#DataBit:5,6,7,8\r\n"
		"#StopBit:1->1位,2->2位,3->1.5\r\n"
		"#Parity:0->no parity;1->odd parity;2->even parity\r\n"
		"#CD:NoCD,NeedCD\r\n\r\n");

	int ChannlIdx = 0;
	struct S_CHANNEL *pChannel = NULL;
	while ((pChannel = pStatus->m_UpChannels.FindChannel(ChannlIdx++))!= NULL)
	{
		if (pChannel->m_Enable == false)
			continue;

		strFileInfo.append("Channel\r\n{");
		strFileInfo.append("\r\n\tEnable=true");
		strFileInfo.append("\r\n\tName=").append((char *)pChannel->m_Name);
		strFileInfo.append("\r\n\tType=").append(MakeChannelTypeKey(pChannel->m_Type));

		PortSettingPrint(pChannel, Buffer);
		strFileInfo.append("\r\n\tSetting=\"").append(Buffer).append("\"");

		sprintf(Buffer,"%d",pChannel->m_ReadTimeoutMS);
		strFileInfo.append("\r\n\tReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_WriteTimeoutMS);
		strFileInfo.append("\r\n\tWriteTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_SubReadTimeoutMS);
		strFileInfo.append("\r\n\tSubReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_CharReadTimeoutMS);
		strFileInfo.append("\r\n\tCharReadTimeoutMS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_ConnectTimeoutS);
		strFileInfo.append("\r\n\tConnectTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_ConnectInterS);
		strFileInfo.append("\r\n\tConnectInterS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_IdleTimeoutS);
		strFileInfo.append("\r\n\tIdleTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_PackageLenMax);
		strFileInfo.append("\r\n\tPackageLenMax=").append(Buffer);
		strFileInfo.append("\r\n}\r\n\r\n");	
	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @fn C_IniServer::LoadDnChannl
 *  @brief 加载采表通道参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadDnChannl(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg;
	cfg = cfg_init(OPT_CHANNLS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}

	int ChannlIdx = 0;
	cfg_t* cfgChannel;
	pStatus->m_DnChannels.Init();
	for(unsigned int i = 0; i<cfg_size(cfg, "Channel"); i++)
	{
		cfgChannel = cfg_getnsec(cfg, "Channel", i);
		if (cfg_getbool(cfgChannel, "Enable") == cfg_false)
			continue;

		struct S_CHANNEL *pChannl = pStatus->m_DnChannels.FindChannel(ChannlIdx++);
		if (pChannl == NULL)
			break;

		LoadChannel(cfgChannel,pChannl);
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveDnChannl
 * @brief 保存采表通道参数配置
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveDnChannl(S_STATUS *pStatus, const char *fname)
{
	string strFileInfo;
	char Buffer[64];

	strFileInfo.append("#note:下行采表通道\r\n"
		"#ChannelType:Net,Com\r\n"
		"#NetType: TcpClient,TcpServer,UdpClient,UdpServer,Tcp,Udp\r\n"
		"#PPP: NoPPP,NeedPPP\r\n"
		"#DataBit:5,6,7,8\r\n"
		"#StopBit:1->1位,2->2位,3->1.5\r\n"
		"#Parity:0->no parity;1->odd parity;2->even parity\r\n"
		"#CD:NoCD,NeedCD\r\n\r\n");

	int ChannlIdx = 0;
	struct S_CHANNEL *pChannel = NULL;
	while ((pChannel = pStatus->m_DnChannels.FindChannel(ChannlIdx++))!= NULL)
	{
		if (pChannel->m_Enable == false)
			continue;

		strFileInfo.append("Channel\r\n{");
		strFileInfo.append("\r\n\tEnable=true");
		strFileInfo.append("\r\n\tName=").append((char *)pChannel->m_Name);
		strFileInfo.append("\r\n\tType=").append(MakeChannelTypeKey(pChannel->m_Type));

		PortSettingPrint(pChannel, Buffer);
		strFileInfo.append("\r\n\tSetting=\"").append(Buffer).append("\"");

		sprintf(Buffer,"%d",pChannel->m_ReadTimeoutMS);
		strFileInfo.append("\r\n\tReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_WriteTimeoutMS);
		strFileInfo.append("\r\n\tWriteTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_SubReadTimeoutMS);
		strFileInfo.append("\r\n\tSubReadTimeoutMS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_CharReadTimeoutMS);
		strFileInfo.append("\r\n\tCharReadTimeoutMS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_ConnectTimeoutS);
		strFileInfo.append("\r\n\tConnectTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_ConnectInterS);
		strFileInfo.append("\r\n\tConnectInterS=").append(Buffer);

		sprintf(Buffer,"%d",pChannel->m_IdleTimeoutS);
		strFileInfo.append("\r\n\tIdleTimeoutS=").append(Buffer);
		sprintf(Buffer,"%d",pChannel->m_PackageLenMax);
		strFileInfo.append("\r\n\tPackageLenMax=").append(Buffer);
		strFileInfo.append("\r\n}\r\n\r\n");	
	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief 主站通讯参数配置结构体 */
cfg_opt_t OPT_HTTX_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_BOOL("ReportEnable", cfg_false,CFGF_NOCASE),
	CFG_INT("ResendTimes", 3, CFGF_NONE),
	CFG_INT("WaitResponseTimeoutS", 20, CFGF_NONE),
	CFG_INT("LoginPeriodS", 0, CFGF_NONE),
	CFG_INT("HeartBeatPeriodM", 0, CFGF_NONE),
	CFG_INT("MonTrafficLimit", 0, CFGF_NONE),
	CFG_INT("MsgConfirmCode", 0, CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),

	CFG_END()
};
cfg_opt_t OPT_HTTXS_SETTINGS[] =
{
	CFG_SEC("Httx", OPT_HTTX_SETTINGS, CFGF_MULTI | CFGF_TITLE |  CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadHttxInfo
 *  @brief 加载主站通讯参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadHttxInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_HTTXS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	cfg_t* cfgHttx;
	for(unsigned int i = 0; i < cfg_size(cfg, "Httx"); i++)
	{
		int httxID = 0;
		cfgHttx = cfg_getnsec(cfg, "Httx", i);
		sscanf(cfg_title(cfgHttx), "httx%d", &httxID);
		if (httxID < 1 || httxID > HTTX_PROCESS_MAX)
			continue;

		C_SYSCTRL::EnableProcess(GetHttxProcessName(httxID), cfg_getbool(cfgHttx, "Enable")? true:false);
		S_HttxInfo& info = pStatus->m_HttxInfo[httxID-1];

		info.Init();
		info.m_ReportEnable = cfg_getbool(cfgHttx,"ReportEnable")? true:false;
		info.m_MsgConfirmCode = (S_BYTE)cfg_getint(cfgHttx,"MsgConfirmCode");
		info.m_HeartBeatPeriodM = (S_BYTE)cfg_getint(cfgHttx,"HeartBeatPeriodM");
		info.m_ResendTimes = (S_BYTE)cfg_getint(cfgHttx,"ResendTimes"); ///< =3 #重发次数
		info.m_WaitResponseTimeoutS = (S_WORD)cfg_getint(cfgHttx,"WaitResponseTimeoutS"); ///< =6 #终端等待从动站响应的超时时间
		info.m_LoginPeriodS = (S_WORD)cfg_getint(cfgHttx,"LoginPeriodS");
		info.m_MonTrafficLimit = (S_DWORD)cfg_getint(cfgHttx,"MonTrafficLimit");

		for (unsigned int i = 0; i < cfg_size(cfgHttx, "Channel") && i < HTTX_CHANNL_MAX; i++)
		{
			strncpy(info.m_ChannlName[i], cfg_getnstr(cfgHttx, "Channel", i), sizeof(info.m_ChannlName[i])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveHttxInfo
 *  @brief 保存主站通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveHttxInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note 后台通讯配置文件\r\n" "#see upchannl.ini\r\n\r\n");

	for(int i = 0; i < HTTX_PROCESS_MAX; i++)
	{
		S_HttxInfo *pInfo = &pStatus->m_HttxInfo[i];

		sprintf(Buffer, "%d", i+1);
		strFileInfo.append("Httx httx").append(Buffer).append("\r\n{");
		strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(GetHttxProcessName(i+1))?"true":"false");
		strFileInfo.append("\r\n\tReportEnable=").append(pInfo->m_ReportEnable==true?"true":"false");

		sprintf(Buffer,"%d", pInfo->m_ResendTimes);
		strFileInfo.append("\r\n\tResendTimes=").append(Buffer).append(" #重发次数");
		sprintf(Buffer,"%d", pInfo->m_WaitResponseTimeoutS);
		strFileInfo.append("\r\n\tWaitResponseTimeoutS=").append(Buffer).append(" #终端等待从动站响应的超时时间");
		sprintf(Buffer,"%d", pInfo->m_LoginPeriodS);
		strFileInfo.append("\r\n\tLoginPeriodS=").append(Buffer).append(" #0表示不要登陆");
		sprintf(Buffer,"%d", pInfo->m_HeartBeatPeriodM);
		strFileInfo.append("\r\n\tHeartBeatPeriodM=").append(Buffer).append(" #0表示不要心跳");
		sprintf(Buffer,"%d", pInfo->m_MonTrafficLimit);
		strFileInfo.append("\r\n\tMonTrafficLimit=").append(Buffer).append(" #0表示不须要流量控制");
		sprintf(Buffer,"%d", pInfo->m_MsgConfirmCode);
		strFileInfo.append("\r\n\tMsgConfirmCode=").append(Buffer).append(" #0表示不进行消息认证 1-255:消息认证方案号");

		strFileInfo.append("\r\n\tChannel={");
		for (int i = 0; i < HTTX_CHANNL_MAX; i++)
		{
			if (pInfo->m_ChannlName[i][0])
				strFileInfo.append(pInfo->m_ChannlName[i]).append(",");
		}
		if (*(strFileInfo.end()-1) == ',')
			strFileInfo.erase(strFileInfo.end()-1);
		strFileInfo.append("} #主通道名(必需), 备用通道名1(可选), 备用通道名2(可选)");
		strFileInfo.append("\r\n}\r\n\r\n");

	}
	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

char *C_IniServer::GetHttxProcessName(int SN)
{
	switch (SN)
	{
	case 1: return MP_PROCESS_HTTX1;
	case 2: return MP_PROCESS_HTTX2;
	case 3: return MP_PROCESS_HTTX3;
	}
	return MP_PROCESS_HTTX1;
}

int C_IniServer::PortSettingParse(struct S_CHANNEL *pChannl, const char *pSetting)
{
	if (pChannl->m_Type == CHANNEL_TYPE_NET)
	{
		int ok = PortSettingParse_Net(pChannl,  pSetting);
		if (ok == -1)
			loget<<"Error netsetting("<<pSetting<<") format,please use format[IP:PortNo,Mode,Property]"<<endl;
		return ok;
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_COM)
	{
		int ok = PortSettingParse_Com(pChannl,  pSetting);
		if (ok == -1)
			loget<<"Error comsetting("<<pSetting<<") format,please use format[PortNo,Baud.DataBit.StopBit.CheckForm,PortType,CD[NoCD]]"<<endl;
		return ok;
	}
	return -1;
}

S_CFGITEM NetConnectType[] =
{
	{NET_CONNECT_TCP, "tcp"},
	{NET_CONNECT_UDP, "udp"},
	{NET_CONNECT_TCP_SERVER, "TcpServer"},
	{NET_CONNECT_TCP_CLIENT, "TcpClient"},
	{NET_CONNECT_UDP_SERVER, "UdpServer"},
	{NET_CONNECT_UDP_CLIENT, "UdpClient"},
};
S_CFGITEM NetNeedPpp[] =
{
	{NET_CONNECT_PPP_NO, "NoPPP"},
	{NET_CONNECT_PPP_YES, "NeedPPP"},
};
int C_IniServer::PortSettingParse_Net(struct S_CHANNEL *pChannl, const char *pSetting)
{
	strings strSetting;
	int size = strSetting.split(pSetting, ',');
	if (size <= 0)
		return -1;

	int ip[4];
	int portNo;
	if (sscanf(strSetting[0].c_str(),"%d.%d.%d.%d:%d",ip,ip+1,ip+2,ip+3,&portNo) != 5)
		return -1;
	pChannl->m_PortSet.m_NetSet.m_IP[0] = (S_BYTE)ip[0];
	pChannl->m_PortSet.m_NetSet.m_IP[1] = (S_BYTE)ip[1];
	pChannl->m_PortSet.m_NetSet.m_IP[2] = (S_BYTE)ip[2];
	pChannl->m_PortSet.m_NetSet.m_IP[3] = (S_BYTE)ip[3];
	pChannl->m_PortSet.m_NetSet.m_PortNO = (S_WORD)portNo;
	
	if (size > 1)
		pChannl->m_PortSet.m_NetSet.m_Type = C_IniBase::MakeValue(NetConnectType, sizeof(NetConnectType)/sizeof(NetConnectType[0]), strSetting[1].c_str());
	if (size > 2)
		pChannl->m_PortSet.m_NetSet.m_NeedPPP = C_IniBase::MakeValue(NetNeedPpp, sizeof(NetNeedPpp)/sizeof(NetNeedPpp[0]), strSetting[2].c_str());

	return 0;
}

int C_IniServer::PortSettingParse_Com(struct S_CHANNEL *pChannl, const char *pSetting)
{
	int portNo=1, baud=0, databit=-1, stopbit=-1, checkform=-1;
	strings strSetting;
	int size = strSetting.split(pSetting, ',');
	if (size < 2)
		return -1;

	if (sscanf(strSetting[0].c_str(),"VCom%d",&portNo) != 1)
		return -1;
	pChannl->m_PortSet.m_ComSet.m_PortNO = (S_DWORD)portNo;

	if (sscanf(strSetting[1].c_str(),"%d.%d.%d.%d",&baud,&databit,&stopbit,&checkform) != 4)
		return -1;
	pChannl->m_PortSet.m_ComSet.m_Baud = (S_DWORD)baud;
	pChannl->m_PortSet.m_ComSet.m_DataBit = (S_DWORD)databit;
	pChannl->m_PortSet.m_ComSet.m_StopBit = (S_DWORD) stopbit;
	pChannl->m_PortSet.m_ComSet.m_Parity = (S_DWORD)checkform;

	if (size > 2)
	{
		if (strcasecmp(strSetting[2].c_str(),"Serial") == 0)
		{
			;
		}
	}

	if (size > 3)
	{
		if (strcasecmp(strSetting[3].c_str(), "CD") == 0 || strcasecmp(strSetting[3].c_str(),"CDCheck") == 0)
			pChannl->m_PortSet.m_ComSet.m_CDCheck = COMM_CDCHECK_YES;
		else if (strcasecmp(strSetting[3].c_str(),"NoCD") == 0 || strcasecmp(strSetting[3].c_str(),"NoCDCheck") == 0)
			pChannl->m_PortSet.m_ComSet.m_CDCheck = COMM_CDCHECK_NO;
	}
	return 0;
}

int C_IniServer::PortSettingPrint(struct S_CHANNEL *pChannl, char *pSetting)
{
	if (pChannl->m_Type == CHANNEL_TYPE_NET)
	{
		return PortSettingPrint_Net(pChannl,  pSetting);
	}
	else if (pChannl->m_Type == CHANNEL_TYPE_COM)
	{
		return PortSettingPrint_Com(pChannl,  pSetting);
	}
	return -1;
}

int C_IniServer::PortSettingPrint_Net(struct S_CHANNEL *pChannl, char *pSetting)
{
	S_NETSET &NetSet = pChannl->m_PortSet.m_NetSet;
	const char *pNetType = C_IniBase::MakeKey(NetConnectType, sizeof(NetConnectType)/sizeof(NetConnectType[0]), NetSet.m_Type);
	const char *pNeedPpp = C_IniBase::MakeKey(NetNeedPpp, sizeof(NetNeedPpp)/sizeof(NetNeedPpp[0]), NetSet.m_NeedPPP);
	sprintf(pSetting, "%d.%d.%d.%d:%d,%s,%s", NetSet.m_IP[0], NetSet.m_IP[1], NetSet.m_IP[2], NetSet.m_IP[3], NetSet.m_PortNO, pNetType, pNeedPpp);
	return 0;
}

int C_IniServer::PortSettingPrint_Com(struct S_CHANNEL *pChannl, char *pSetting)
{
	S_COMSET &ComSet = pChannl->m_PortSet.m_ComSet;
	char *pPortType = "Serial";
	char cdCheck[8];
	if (pChannl->m_PortSet.m_ComSet.m_CDCheck == COMM_CDCHECK_YES)
		sprintf(cdCheck, "%s", "CD");
	else
		sprintf(cdCheck, "%s", "NoCD");
	sprintf(pSetting, "VCom%d,%d.%d.%d.%d,%s,%s", ComSet.m_PortNO, ComSet.m_Baud, ComSet.m_DataBit, ComSet.m_StopBit, ComSet.m_Parity, pPortType, cdCheck);
	return 0;
}


/** @brief PPP通讯参数配置结构体 */
cfg_opt_t OPT_PPP_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_INT("ModemPowerOffDelayS", 2, CFGF_NONE),
	CFG_INT("ModemPowerOnDelayS", 4, CFGF_NONE),
	CFG_INT("RefreshSignalInterS", 300, CFGF_NONE),
	CFG_INT("RedialInterM", 30, CFGF_NONE),
	CFG_INT("RedialTimesPerDay", 3, CFGF_NONE),
	CFG_INT("CheckInterM", 0, CFGF_NONE),
	CFG_INT("CheckFailTimes", 0, CFGF_NONE),
	CFG_STR("MasterPhone", "10086", CFGF_NONE),
	CFG_STR("SmsCenterNo", "13988998899", CFGF_NONE),
	CFG_STR("Apn", "CMNET", CFGF_NONE),
	CFG_STR("TeleNo", "*99***1#", CFGF_NONE),
	CFG_STR("UserName", "", CFGF_NONE),
	CFG_STR("Password", "", CFGF_NONE),
	CFG_STR("MasterPhone", "10086", CFGF_NONE),
	CFG_STR_LIST("Channel", "{}", CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_PPPS_SETTINGS[] =
{
	CFG_SEC("Ppp", OPT_PPP_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadPppInfo
 *  @brief 加载主站通讯参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadPppInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_PPPS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	cfg_t* cfgPpp;
	for(unsigned int i = 0; i < cfg_size(cfg, "Ppp"); i++)
	{
		cfgPpp = cfg_getnsec(cfg, "Ppp", i);
		C_SYSCTRL::EnableProcess(MP_PROCESS_PPP, cfg_getbool(cfgPpp, "Enable")? true:false);
		S_PppInfo& info = pStatus->m_PppInfo;

		info.Init();
		info.m_ModemPowerOffDelayS = cfg_getint(cfgPpp,"ModemPowerOffDelayS");
		info.m_ModemPowerOnDelayS = cfg_getint(cfgPpp,"ModemPowerOnDelayS");
		info.m_RefreshSignalInterS = (S_WORD)cfg_getint(cfgPpp,"RefreshSignalInterS"); ///=300 #刷新无线信号间隔，缺省300秒
		info.m_RedialInterM = (S_WORD)cfg_getint(cfgPpp,"RedialInterM");
		info.m_RedialTimesPerDay = (S_WORD)cfg_getint(cfgPpp,"RedialTimesPerDay");
		info.m_DialInfo.m_CheckInterM = (S_WORD)cfg_getint(cfgPpp,"CheckInterM");
		info.m_DialInfo.m_CheckFailTimes = (S_WORD)cfg_getint(cfgPpp,"CheckFailTimes");
		strncpy(info.m_DialInfo.m_Apn, cfg_getstr(cfgPpp, "Apn"), sizeof(info.m_DialInfo.m_Apn)-1);
		strncpy(info.m_DialInfo.m_TeleNo, cfg_getstr(cfgPpp, "TeleNo"), sizeof(info.m_DialInfo.m_TeleNo)-1);
		strncpy(info.m_DialInfo.m_User, cfg_getstr(cfgPpp, "UserName"), sizeof(info.m_DialInfo.m_User)-1);
		strncpy(info.m_DialInfo.m_Pwd, cfg_getstr(cfgPpp, "Password"), sizeof(info.m_DialInfo.m_Pwd)-1);
		strncpy(info.m_MasterPhone, cfg_getstr(cfgPpp, "MasterPhone"), sizeof(info.m_MasterPhone)-1);
		strncpy(info.m_SmsCenterNo, cfg_getstr(cfgPpp, "SmsCenterNo"), sizeof(info.m_SmsCenterNo)-1);

		for (unsigned int i = 0; i < cfg_size(cfgPpp, "Channel") && i < PPP_CHANNL_MAX; i++)
		{
			strncpy(info.m_ChannlName[i], cfg_getnstr(cfgPpp, "Channel", i), sizeof(info.m_ChannlName[i])-1);
		}
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SavePppInfo
 *  @brief 保存主站通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SavePppInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note PPP拨号通讯配置文件\r\n" "#see upchannl.ini\r\n\r\n");

	S_PppInfo *pInfo = &pStatus->m_PppInfo;
	strFileInfo.append("Ppp\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_PPP)?"true":"false");

	sprintf(Buffer,"%d", pInfo->m_ModemPowerOffDelayS);
	strFileInfo.append("\r\n\tModemPowerOffDelayS=").append(Buffer);
	sprintf(Buffer,"%d", pInfo->m_ModemPowerOnDelayS);
	strFileInfo.append("\r\n\tModemPowerOnDelayS=").append(Buffer);

	sprintf(Buffer,"%d", pInfo->m_RefreshSignalInterS);
	strFileInfo.append("\r\n\tRefreshSignalInterS=").append(Buffer).append(" #刷新无线信号间隔，缺省300秒");
	sprintf(Buffer,"%d", pInfo->m_RedialInterM);
	strFileInfo.append("\r\n\tRedialInterM=").append(Buffer).append(" #重拨间隔，缺省30分钟");
	sprintf(Buffer,"%d", pInfo->m_RedialTimesPerDay);
	strFileInfo.append("\r\n\tRedialTimesPerDay=").append(Buffer).append(" #日重拨次数,缺省3次,为0则不限");
	sprintf(Buffer,"%d", pInfo->m_DialInfo.m_CheckInterM);
	strFileInfo.append("\r\n\tCheckInterM=").append(Buffer).append(" #pppd检查间隔，缺省为0");
	sprintf(Buffer,"%d", pInfo->m_DialInfo.m_CheckFailTimes);
	strFileInfo.append("\r\n\tCheckFailTimes=").append(Buffer).append(" #pppd失败次数，缺省为0");

	strFileInfo.append("\r\n\tMasterPhone=\"").append(pInfo->m_MasterPhone).append("\"");
	strFileInfo.append("\r\n\tSmsCenterNo=\"").append(pInfo->m_SmsCenterNo).append("\"");
	strFileInfo.append("\r\n\tApn=\"").append(pInfo->m_DialInfo.m_Apn).append("\"");
	strFileInfo.append("\r\n\tTeleNo=\"").append(pInfo->m_DialInfo.m_TeleNo).append("\"");
	strFileInfo.append("\r\n\tUserName=\"").append(pInfo->m_DialInfo.m_User).append("\"");
	strFileInfo.append("\r\n\tPassword=\"").append(pInfo->m_DialInfo.m_Pwd).append("\"");

	strFileInfo.append("\r\n\tChannel={");
	for (int i = 0; i < PPP_CHANNL_MAX; i++)
	{
		if (pInfo->m_ChannlName[i][0])
			strFileInfo.append(pInfo->m_ChannlName[i]).append(",");
	}
	if (*(strFileInfo.end()-1) == ',')
		strFileInfo.erase(strFileInfo.end()-1);
	strFileInfo.append("} #MODEM通道,GPRS通道,AT命令通道,SMS通道");
	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}

/** @brief LCD参数配置结构体 */
cfg_opt_t OPT_LCD_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("TermName", "***台区集中器", CFGF_NONE),
	CFG_INT("LCDPassword", 0, CFGF_NONE),
	CFG_INT("LCDGreyScale", 195, CFGF_NONE),
	CFG_INT("LCDBrightness", 100, CFGF_NONE),
	CFG_INT("LCDLightTimeoutS", 60, CFGF_NONE),
	CFG_INT("BackDesktopTimeoutS", 40, CFGF_NONE),
	CFG_END()
};
cfg_opt_t OPT_LCDS_SETTINGS[] =
{
	CFG_SEC("Lcd", OPT_LCD_SETTINGS, CFGF_MULTI |CFGF_NOCASE),
	CFG_END()
};

/** @fn C_IniServer::LoadLcdInfo
 *  @brief 加载主站通讯参数
 *  @param[out] pStatus:配置结构体
 *  @param[in] fname:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::LoadLcdInfo(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg  = cfg_init(OPT_LCDS_SETTINGS, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname)!=0)
	{
		cfg_free(cfg);
		return -1;
	}

	cfg_t* cfgLcd;
	for(unsigned int i = 0; i < cfg_size(cfg, "Lcd"); i++)
	{
		cfgLcd = cfg_getnsec(cfg, "Lcd", i);
		C_SYSCTRL::EnableProcess(MP_PROCESS_LCD, cfg_getbool(cfgLcd, "Enable")? true:false);
		S_LcdInfo& info = pStatus->m_LcdInfo;

		info.Init();
		info.m_LCDPassword = cfg_getint(cfgLcd, "LCDPassword"); ///< =0 #液晶调试设置权限密码
		if (info.m_LCDPassword < 0 || info.m_LCDPassword > 999999)
			info.m_LCDPassword = 0;
		info.m_LCDGreyScale = cfg_getint(cfgLcd, "LCDGreyScale"); ///< =195 #液晶灰度值
		if (info.m_LCDGreyScale < 0 || info.m_LCDGreyScale > 99)
			info.m_LCDGreyScale = 99;
		info.m_LCDBrightness = cfg_getint(cfgLcd, "LCDBrightness"); ///=100 #液晶背光亮度
		if (info.m_LCDBrightness < 0 || info.m_LCDBrightness > 99)
			info.m_LCDBrightness = 99;
		info.m_LCDLightTimeoutS = cfg_getint(cfgLcd, "LCDLightTimeoutS"); ///< =60 #液晶背光灯点亮时间
		if (info.m_LCDLightTimeoutS < 0 || info.m_LCDLightTimeoutS > 5*60)
			info.m_LCDLightTimeoutS = 60;
		info.m_BackDesktopTimeoutS = cfg_getint(cfgLcd, "BackDesktopTimeoutS"); ///< =40 #返回桌面的时间
		if (info.m_BackDesktopTimeoutS < 0 || info.m_BackDesktopTimeoutS > 5*60)
			info.m_BackDesktopTimeoutS = 40;
		strncpy(info.m_TermName, cfg_getstr(cfgLcd, "TermName"), sizeof(info.m_TermName)-1);///< =***台区集中器
	}

	cfg_free(cfg);
	return 0;
}

/** @fn C_IniServer::SaveLcdInfo
 *  @brief 保存主站通讯参数
 *  @param[in] pStatus:配置结构体
 *  @param[in] name:文件名
 *  @return 0:successful, -1:failed
 */
int C_IniServer::SaveLcdInfo(S_STATUS *pStatus, const char *fname)
{
	char Buffer[32];
	string strFileInfo;

	strFileInfo.append("#note LCD配置文件\r\n\r\n");

	S_LcdInfo *pInfo = &pStatus->m_LcdInfo;
	strFileInfo.append("Lcd\r\n{");
	strFileInfo.append("\r\n\tEnable=").append(C_SYSCTRL::GetProcessEnable(MP_PROCESS_LCD)?"true":"false");

	strFileInfo.append("\r\n\tTermName=").append(pInfo->m_TermName);
	sprintf(Buffer,"%d", pInfo->m_LCDPassword);
	strFileInfo.append("\r\n\tLCDPassword=").append(Buffer).append(" #液晶调试设置权限密码");
	sprintf(Buffer,"%d", pInfo->m_LCDGreyScale);
	strFileInfo.append("\r\n\tLCDGreyScale=").append(Buffer).append(" #液晶灰度值 0-99");
	sprintf(Buffer,"%d", pInfo->m_LCDBrightness);
	strFileInfo.append("\r\n\tLCDBrightness=").append(Buffer).append(" #液晶背光亮度 0-99");
	sprintf(Buffer,"%d", pInfo->m_LCDLightTimeoutS);
	strFileInfo.append("\r\n\tLCDLightTimeoutS=").append(Buffer).append(" #液晶背光灯点亮时间");
	sprintf(Buffer,"%d", pInfo->m_BackDesktopTimeoutS);
	strFileInfo.append("\r\n\tBackDesktopTimeoutS=").append(Buffer).append(" #返回桌面的时间");

	strFileInfo.append("\r\n}\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}



