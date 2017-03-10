#include "status.h"
#include "regitem.h"

cfg_opt_t OPT_REG_SETTINGS[] =
{
	CFG_BOOL("Enable", cfg_true,CFGF_NOCASE),
	CFG_STR("RegID","FFFF",CFGF_NOCASE),
	CFG_STR("SubRegID","FFFF",CFGF_NOCASE),
	CFG_INT("Len", 0, CFGF_NOCASE),
	CFG_BOOL("DataBlock",cfg_false, CFGF_NOCASE),
	CFG_END()
};
cfg_opt_t OPT_REGS_SETTINGS[] =
{
	CFG_SEC("Register", OPT_REG_SETTINGS, CFGF_MULTI |  CFGF_NOCASE),
	CFG_END()
};


bool C_RegitemCfg::LoadRegItems(const char* ipFile)
{
	cfg_t* cfgDI;
	cfg_t* cfg;
	
	S_RegItems * pRegItemsArea = STATUS.GetRegitemsArea();

	if (pRegItemsArea->GetItemNum() > 0 )
	{
		return true;
	}
	
	cfg = cfg_init(OPT_REGS_SETTINGS, CFGF_NONE);

	if (C_IniBase::ParseSetFile(cfg,ipFile)!=0)
	{
		cfg_free(cfg);
		return false;
	}
	else
	{
		S_WORD validItemNum = 0;
		for(unsigned int i = 0; i < cfg_size(cfg, "Register"); i++)
		{
			cfgDI = cfg_getnsec(cfg, "Register", i);
			if (cfg_getbool(cfgDI, "Enable")==cfg_true)
			{
				validItemNum++;
			}
		}

		if (validItemNum == pRegItemsArea->GetItemNum() && validItemNum > 0)
		{
			return true;
		}

		int value;
		for(unsigned int i = 0; i < cfg_size(cfg, "Register"); i++)
		{
			cfgDI = cfg_getnsec(cfg, "Register", i);
			if (cfg_getbool(cfgDI, "Enable")==cfg_true)
			{
				S_RegItem item;
				sscanf(cfg_getstr(cfgDI, "RegID"),"%4x",&value);
				item.m_regid = value;
				sscanf(cfg_getstr(cfgDI, "SubRegID"),"%4x",&value);
				item.m_subregid = value;
				item.m_Len=(unsigned char)cfg_getint(cfgDI,"Len");
				item.m_bDataBlock = cfg_getbool(cfgDI, "DataBlock")==cfg_true?true:false;
				memset(item.m_data,REGITEM_INVALID_DATA_FLAG,CONS_REGITEM_DATABUF_MAXLEN);	
				pRegItemsArea->AddItem(&item);
			}
		}
	}
	cfg_free(cfg);
	return true;
}




