/** @file
 *  @brief LCD数据操作接口
 *  @author MSZ
 *  @date 2015/06/03
 *  @version 0.1
 */
#ifndef __LCD_DATAIFACE_H__
#define __LCD_DATAIFACE_H__
#include "basedef.h"
//#include "sgui/sguictrl.h"

enum E_CodeType
{
	BCDCODE,
	HEXCODE,
	STRINGCODE,
	SELECTBOXTYPE
};

struct S_RegSec
{
	S_DWORD m_regid;
	S_DWORD m_regSubId;
	S_BYTE m_pos;
	S_BYTE m_len;
	S_BYTE m_type;
	S_BYTE m_firstByte;
	double m_step;
	double m_minValue;
	double m_maxValue;
	S_BYTE m_intLen;
	S_BYTE m_floatLen;
	S_BYTE m_totalSize; // 所在数据标识的总长度
};

class C_LcdDataIface
{
public:
/*
	brief: 从Flash中读取对应RegId的相应的数据 
	param [in] S_RegSec *regSec:要读取的RegId及其位置
	param [out]S_CHAR *pDataBuf:读出的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/
	static bool ReadData(S_RegSec *regSec,char *pDataBuf);

/*
	brief: 将RegId的相应的数据写人Flash中
	param [in] S_RegSec *regSec:要写入的RegId及其位置
	param [in]S_CHAR *pDataBuf:要写入的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/
	static bool WriteData(S_RegSec *regSec,S_CHAR *pDataBuf);
/*
	brief: 从Flash中读取对应Music音量的相应的数据 
	param [out]S_CHAR *pDataBuf:读出的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/

	static bool ReadMusicData(char *pDataBuf);
/*
	brief: 从Flash中读取对应Mic音量的相应的数据 
	param [out]S_CHAR *pDataBuf:读出的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool ReadMicData(char *pDataBuf);
/*
	brief: 从Flash中读取对应Effect音量的相应的数据 
	param [out]S_CHAR *pDataBuf:读出的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool ReadEffectData(char *pDataBuf);

/*
	brief: 将Music音量的相应的数据写人Flash中
	param [in]S_CHAR *pDataBuf:要写入的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteMusicData(char *pDataBuf);
/*
	brief: 将Mic音量的相应的数据写人Flash中
	param [in]S_CHAR *pDataBuf:要写入的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteMicData(char *pDataBuf);
/*
	brief: 将Effect音量的相应的数据写人Flash中
	param [in]S_CHAR *pDataBuf:要写入的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteEffectData(char *pDataBuf);
	static bool WriteData2Dsp(S_RegSec *regSec);

};

#endif//__LCD_DATAIFACE_H__



