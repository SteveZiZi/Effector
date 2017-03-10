#ifndef __PARAM_MODE_H__
#define __PARAM_MODE_H__
#include "status.h"


class C_ParamMode
{
public:
	static int LoadTermMenuConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_MENU);
	static int SaveTermMenuConfig(S_STATUS *pStatus, const char *fname = CFG_FILE_INI_MENU);
};


#endif


