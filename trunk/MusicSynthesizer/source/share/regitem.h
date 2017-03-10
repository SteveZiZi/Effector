#ifndef _REGITEM_H
#define  _REGITEM_H

#include "sysbase.h"

#define CFG_FILE_REGITEM "cfg_regitem.ini"
#define REGITEM_INVALID_DATA_FLAG		0xff



class C_RegitemCfg
{
public:
	 C_RegitemCfg()
	{
	}
	 ~C_RegitemCfg()
	{
	}
	static bool LoadRegItems(const char* ipFile=CFG_FILE_REGITEM);
};

#endif

