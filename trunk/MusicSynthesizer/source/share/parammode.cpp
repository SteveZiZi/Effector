#include "parammode.h"
#include "sysbase.h"


/** @brief �˵��������ݿ�*/
cfg_opt_t OPT_MENUINFO_INFO[] =
{
	//������ ,����Ĭ��ֵ  ,��־	configuration file is case insensitive
	CFG_INT("CurParamMode", 0, CFGF_NOCASE),
	CFG_INT("MusicInput", 0, CFGF_NOCASE),
	CFG_END()
};


/** @fn C_IniSys::LoadTermMenuConfig
 *  @brief ����ģʽ��Ϣ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] fname:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_ParamMode::LoadTermMenuConfig(S_STATUS *pStatus, const char *fname)
{
	cfg_t* cfg;

	cfg = cfg_init(OPT_MENUINFO_INFO, CFGF_NONE);
	if (C_IniBase::ParseSetFile(cfg, fname) != 0)
	{
		cfg_free(cfg);
		return -1;
	}
	struct S_MenuConfig &MenuInfo = pStatus->m_MenuConfig;

	MenuInfo.m_CurParamMode = (INT8U)cfg_getint(cfg,"CurParamMode");
	MenuInfo.m_MusicInput = (INT8U)cfg_getint(cfg,"MusicInput");
	cfg_free(cfg);
	return 0;
}

/** @fn C_IniSys::SaveTermMenuConfig
 *  @brief ����ģʽ��Ϣ����
 *  @param[out] pStatus:���ýṹ��
 *  @param[in] cfgFile:�ļ���
 *  @return 0:successful, -1:failed
 */
int C_ParamMode::SaveTermMenuConfig(S_STATUS *pStatus, const char *fname)
{
	char buffer[64];
	string strFileInfo;

	struct S_MenuConfig &MenuInfo = pStatus->m_MenuConfig;
	
	sprintf(buffer, "%d", MenuInfo.m_CurParamMode);
	strFileInfo.append("CurParamMode = ").append(buffer);
	sprintf(buffer, "%d", MenuInfo.m_MusicInput);
	strFileInfo.append("\r\nMusicInput = ").append(buffer);
	strFileInfo.append("\r\n\r\n");

	return C_IniBase::SaveSetFile(strFileInfo, fname);
}
