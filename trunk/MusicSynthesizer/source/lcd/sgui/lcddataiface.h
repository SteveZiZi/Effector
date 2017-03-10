/** @file
 *  @brief LCD���ݲ����ӿ�
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
	S_BYTE m_totalSize; // �������ݱ�ʶ���ܳ���
};

class C_LcdDataIface
{
public:
/*
	brief: ��Flash�ж�ȡ��ӦRegId����Ӧ������ 
	param [in] S_RegSec *regSec:Ҫ��ȡ��RegId����λ��
	param [out]S_CHAR *pDataBuf:��������������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/
	static bool ReadData(S_RegSec *regSec,char *pDataBuf);

/*
	brief: ��RegId����Ӧ������д��Flash��
	param [in] S_RegSec *regSec:Ҫд���RegId����λ��
	param [in]S_CHAR *pDataBuf:Ҫд�����������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/
	static bool WriteData(S_RegSec *regSec,S_CHAR *pDataBuf);
/*
	brief: ��Flash�ж�ȡ��ӦMusic��������Ӧ������ 
	param [out]S_CHAR *pDataBuf:��������������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/

	static bool ReadMusicData(char *pDataBuf);
/*
	brief: ��Flash�ж�ȡ��ӦMic��������Ӧ������ 
	param [out]S_CHAR *pDataBuf:��������������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool ReadMicData(char *pDataBuf);
/*
	brief: ��Flash�ж�ȡ��ӦEffect��������Ӧ������ 
	param [out]S_CHAR *pDataBuf:��������������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool ReadEffectData(char *pDataBuf);

/*
	brief: ��Music��������Ӧ������д��Flash��
	param [in]S_CHAR *pDataBuf:Ҫд�����������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteMusicData(char *pDataBuf);
/*
	brief: ��Mic��������Ӧ������д��Flash��
	param [in]S_CHAR *pDataBuf:Ҫд�����������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteMicData(char *pDataBuf);
/*
	brief: ��Effect��������Ӧ������д��Flash��
	param [in]S_CHAR *pDataBuf:Ҫд�����������
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-11-17
*/
	static bool WriteEffectData(char *pDataBuf);
	static bool WriteData2Dsp(S_RegSec *regSec);

};

#endif//__LCD_DATAIFACE_H__



