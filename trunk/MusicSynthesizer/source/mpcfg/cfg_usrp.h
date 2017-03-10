/** @file
 *  @brief ϵͳ����Ŀ¼����Ҫ�ļ�����
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 *  1�����ļ��������ļ�����Ҫ�û�����
 *  2��ʵ���ն�ϵͳ����ͱ�����Ŀ¼����һ��
 */
#ifndef __MP_CFG_USRPATH_H__
#define __MP_CFG_USRPATH_H__

#include "confuse.h"
/** base path */
#ifdef WIN32
#define CFG_PATH_BASE	"../../mnt/"
#else
#define CFG_PATH_BASE	"/mnt/"
#endif

/** dir define */
#define CFG_PATH_TMP		"/tmp/"
#define CFG_PATH_SYSTEM		CFG_PATH_BASE"system/"
#define CFG_PATH_RESOURCE	CFG_PATH_BASE"resource/"

#define CFG_PATH_DATA		CFG_PATH_BASE"data/"
#define CFG_PATH_LOG		CFG_PATH_DATA"log/"
#define CFG_PATH_ALARM		CFG_PATH_DATA"alarm/"
#define CFG_PATH_ENERGY		CFG_PATH_DATA"energy/"

#define CFG_PATH_DYJC		CFG_PATH_BASE"dyjc/"
#define CFG_PATH_PROGRAM	CFG_PATH_DYJC"program/"
#define CFG_PATH_SCRIPT		CFG_PATH_DYJC"script/"
#define CFG_PATH_REGISTER		CFG_PATH_DYJC"register/"
#define CFG_PATH_SET		CFG_PATH_DYJC"set/"
#define CFG_PATH_SET_DEFAULT	CFG_PATH_SET"default/"
#define CFG_PATH_SET_USER		CFG_PATH_SET"user/"

#define CFG_PATH_RAMDISK		CFG_PATH_BASE"ramdisk/"
#define CFG_PATH_UPDATE		CFG_PATH_BASE"update/"


/** system file define */
#define CFG_FILE_PROGRAM_NAME	"dyjc"

#define CFG_FILE_CONTEXT	"context.dat"
#define CFG_FILE_STATUS		"status.dat"
#define CFG_FILE_AUTO_UPDATE_KEY	"auto_update.key"

/** script file define */
#define CFG_FILE_SCRIPT_ON_START	"on_start.srt"
#define CFG_FILE_SCRIPT_ON_STOP		"on_stop.srt"
#define CFG_FILE_SCRIPT_DATA_RESET	"on_data_reset.srt"
#define CFG_FILE_SCRIPT_FACT_RESET	"on_factory_reset.srt"
#define CFG_FILE_SCRIPT_PARAM_RESET	"on_param_reset.srt"
#define CFG_FILE_SCRIPT_STAT_RESET	"on_status_reset.srt"
#define CFG_FILE_SCRIPT_PPP_GPRS_ON			"ppp_gprs_on.srt"			//PPP�������߽ű�(GPRS)
#define CFG_FILE_SCRIPT_PPP_GPRS_OFF		"ppp_gprs_off.srt"			//PPP�������߽ű�(GPRS)
#define CFG_FILE_SCRIPT_PPP_CDMA_ON			"ppp_cdma_on.srt"			//PPP�������߽ű�(CDMA)
#define CFG_FILE_SCRIPT_PPP_CDMA_OFF		"ppp_cdma_off.srt"			//PPP�������߽ű�(CDMA)
#define CFG_FILE_SCRIPT_PPP_MUXD			"ppp_muxd.srt"		//ppp���ڸ���
#define CFG_FILE_SCRIPT_PPP_CLEAR			"ppp_clear.srt"		//ppp��Ϣ���


/** user ini file define */
//system dir
#define CFG_FILE_INI_SYSCFG		"syscfg.ini"
#define CFG_FILE_INI_PORTMAP	"portmap.ini"
#define CFG_FILE_INI_SYSRUN		"sysrun.inf"
//set dir
#define CFG_FILE_INI_TRACE			"trace.ini"
#define CFG_FILE_INI_UPCHANNL		"upchannl.ini"					//����ͨ�������ļ�
#define CFG_FILE_INI_DNCHANNL		"dnchannl.ini"					//����ͨ�������ļ�
#define CFG_FILE_INI_HTTX			"httx.ini"
#define CFG_FILE_INI_METER			"meter.ini"
#define CFG_FILE_INI_IPCONFIG		"ipconfig.ini"
#define CFG_FILE_INI_FLOWCTRL		"flowctrl.ini"
#define CFG_FILE_INI_TASK		 	"gathert.ini"
#define CFG_FILE_INI_PPP			"ppp.ini"
#define CFG_FILE_INI_LCD			"lcd.ini"
#define CFG_FILE_INI_ALARM			"alarm.ini"
#define CFG_FILE_INI_DBCJ			"dbcj.ini"
#define CFG_FILE_INI_FORWARD		"forward.ini"
#define CFG_FILE_INI_NORMAL_TASK	"normalt.ini"
#define CFG_FILE_INI_FORWARD_TASK		"forwardt.ini"
#define CFG_FILE_INI_GATHERFNRGE	"gather_fnreg.ini"
#define CFG_FILE_INI_POWER_LIMIT	"powlimit.ini"
#define CFG_FILE_INI_PWR_CTRL		"pwrctrl.ini"
#define CFG_FILE_INI_DL_CTRL		"dlctrl.ini"
#define CFG_FILE_INI_ANALOG		"analog.ini"
#define CFG_FILE_INI_PULSE			"pulse.ini"
#define CFG_FILE_INI_SUM			"sum.ini"
#define CFG_FILE_INI_DIFF_RULE		"diffrule.ini"
#define CFG_FILE_INI_JLCY			"jlcy.ini"
#define CFG_FILE_INI_FHKZ			"fhkz.ini"
#define CFG_FILE_INI_MENU			"menuconfig.ini"

#define CFG_FILE_INIT_GB645REG		"reg_gb645.ini"
#define CFG_FILE_INIT_GB2007REG		"reg_gb2007.ini"

#endif//__MP_CFG_USRPATH_H__

