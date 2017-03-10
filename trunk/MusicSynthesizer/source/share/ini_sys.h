/** @file
 *  @brief 系统配置加载和保存
 *  @author Steven Wan
 *  @date  2015/01/07
 *  @version 0.1
 *  定义系统配置加载和保存函数
 */
#ifndef __NW_INI_SYS_H__
#define __NW_INI_SYS_H__
#include "cfg_stat.h"
#include "cfg_cont.h"
#include "cfg_usrp.h"

class C_IniSys
{
public:
	static int LoadRunInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSRUN);
	static int SaveRunInformation(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_SYSRUN);
	static int LoadPortMap(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_PORTMAP);

	static int LoadTermIPConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_IPCONFIG);
	static int SaveTermIPConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_IPCONFIG);


};

#endif//__NW_INI_SYS_H__

