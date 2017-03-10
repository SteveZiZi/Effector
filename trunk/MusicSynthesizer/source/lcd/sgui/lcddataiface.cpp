#include "lcddataiface.h"
#include "status.h"
#include "Arithmetic.h"
#include "formatconv.h"
#include "dspdevice.h"
/*
	brief: 从Flash中读取对应RegId的相应的数据 
	param [in] S_RegSec *regSec:要读取的RegId及其位置
	param [out]S_CHAR *pDataBuf:读出的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/
bool C_LcdDataIface::ReadData(S_RegSec *regSec,char *pDataBuf)
{
	if ( NULL == regSec)
	{
		return false;
	}
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemBySubRegid((S_WORD)regSec->m_regSubId);
	if (NULL == pRegItem)
	{
		return false;
	}

	switch(regSec->m_type)
	{
	case BCDCODE:
		{
			C_BCD bcdToStr((S_BYTE*)(&pRegItem->m_data[regSec->m_pos - 1]),regSec->m_intLen,regSec->m_floatLen);
			
			unsigned long Integer;
			unsigned short Float;
			bcdToStr.GetBcdData(Integer,Float);

			string str;
			str = string(bcdToStr);
			str += '\0';
			//pDataBuf = (char *)str.data();  // 栈中内容消失 所以传不出去  可以使用new来做
			str.copy(pDataBuf,str.size() + 1,0);
			break;
		}
	case HEXCODE:
		/*{
			memcpy(pDataBuf,&pRegItem->m_data[regSec->m_pos - 1],regSec->m_len);
			char * temp = pDataBuf;
			temp++;
			*temp = '\0';
			break;
		}*/
		*pDataBuf = pRegItem->m_data[regSec->m_pos - 1];
		break;
	case STRINGCODE:
		break;
	case SELECTBOXTYPE:
		*pDataBuf = pRegItem->m_data[regSec->m_pos - 1];
		break;
	default:
		break;
	}
	return true;
}

/*
	brief: 将RegId的相应的数据写人Flash中
	param [in] S_RegSec *regSec:要写入的RegId及其位置
	param [in]S_CHAR *pDataBuf:要写入的数据内容
	return bool: successful:true
				 error:false
	author:MSZ
	date: 2015-06-05
*/

bool C_LcdDataIface::WriteData(S_RegSec *regSec,S_CHAR *pDataBuf)
{
	S_CHAR *pBuffer;

	if ( NULL == regSec)
	{
		return false;
	}
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemBySubRegid((S_WORD)regSec->m_regSubId);
	if (NULL == pRegItem)
	{
		return false;
	}

	pBuffer = pDataBuf;
	if (0xff != regSec->m_firstByte)
	{
		memcpy(&pRegItem->m_data[0],&regSec->m_firstByte,1);
	}
	
	switch(regSec->m_type)
	{
	case BCDCODE:
		{
			string str = pDataBuf;
			// 将string转化为BCD数据
			C_DataFormatConv::ConvFloatStringToBcdData(str,(S_BYTE *)(&pRegItem->m_data[regSec->m_pos - 1]),regSec->m_len,regSec->m_intLen,regSec->m_floatLen);
			break;
		}
	case HEXCODE:
		{
			int dataTemp = 0;
			dataTemp = *pDataBuf;
			pRegItem->m_data[regSec->m_pos - 1] = dataTemp;
			//memcpy(&pRegItem->m_data[regSec->m_pos - 1],pDataBuf,regSec->m_len);
			break;
		}
	case STRINGCODE:
		break;
	case SELECTBOXTYPE:
		{
			
			memcpy(&pRegItem->m_data[regSec->m_pos - 1],pDataBuf,regSec->m_len);
			break;
		}
	default:
		break;
	}

	/*C_DspDevice dsp;
	if (!dsp.Write((S_WORD)regSec->m_regid,pRegItem->m_data,regSec->m_totalSize))
	{
		loget<<"发送数据到DSP失败\n"<<endl;
		return false;
	}*/

	return true;
}


bool C_LcdDataIface::ReadMusicData(char *pDataBuf)
{
	S_RegSec regSecMusic;
	regSecMusic.m_regid = 0x0601;
	regSecMusic.m_regSubId = 0x5131;
	regSecMusic.m_pos = 4;
	regSecMusic.m_len = 1;
	regSecMusic.m_type = HEXCODE;
	regSecMusic.m_firstByte = 0xff;
	regSecMusic.m_step = 1;
	regSecMusic.m_minValue = 0;
	regSecMusic.m_maxValue = 100;
	regSecMusic.m_intLen = 0;
	regSecMusic.m_floatLen = 0;
	regSecMusic.m_totalSize = 4;

	if (ReadData(&regSecMusic,pDataBuf))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool C_LcdDataIface::WriteMusicData(char *pDataBuf)
{
	S_RegSec regSecMusic;
	regSecMusic.m_regid = 0x0601;
	regSecMusic.m_regSubId = 0x5131;
	regSecMusic.m_pos = 4;
	regSecMusic.m_len = 1;
	regSecMusic.m_type = HEXCODE;
	regSecMusic.m_firstByte = 0xff;
	regSecMusic.m_step = 1;
	regSecMusic.m_minValue = 0;
	regSecMusic.m_maxValue = 100;
	regSecMusic.m_intLen = 0;
	regSecMusic.m_floatLen = 0;
	regSecMusic.m_totalSize = 4;

	if (WriteData(&regSecMusic,pDataBuf))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool C_LcdDataIface::ReadMicData(char *pDataBuf)
{
	S_RegSec regSecMic;
	regSecMic.m_regid = 0x0601;
	regSecMic.m_regSubId = 0x5111;
	regSecMic.m_pos = 4;
	regSecMic.m_len = 1;
	regSecMic.m_type = HEXCODE;
	regSecMic.m_firstByte = 0xff;
	regSecMic.m_step = 1;
	regSecMic.m_minValue = 0;
	regSecMic.m_maxValue = 100;
	regSecMic.m_intLen = 0;
	regSecMic.m_floatLen = 0;
	regSecMic.m_totalSize = 4;

	if (ReadData(&regSecMic,pDataBuf))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool C_LcdDataIface::WriteMicData(char *pDataBuf)
{
	S_RegSec regSecMic;
	regSecMic.m_regid = 0x0601;
	regSecMic.m_regSubId = 0x5111;
	regSecMic.m_pos = 4;
	regSecMic.m_len = 1;
	regSecMic.m_type = HEXCODE;
	regSecMic.m_firstByte = 0xff;
	regSecMic.m_step = 1;
	regSecMic.m_minValue = 0;
	regSecMic.m_maxValue = 100;
	regSecMic.m_intLen = 0;
	regSecMic.m_floatLen = 0;
	regSecMic.m_totalSize = 4;

	if (WriteData(&regSecMic,pDataBuf))
	{
		return true;
	} 
	else
	{
		return false;
	}
}


bool C_LcdDataIface::ReadEffectData(char *pDataBuf)
{
	S_RegSec regSecEffect;
	regSecEffect.m_regid = 0x0601;
	regSecEffect.m_regSubId = 0x5121;
	regSecEffect.m_pos = 4;
	regSecEffect.m_len = 1;
	regSecEffect.m_type = HEXCODE;
	regSecEffect.m_firstByte = 0xff;
	regSecEffect.m_step = 1;
	regSecEffect.m_minValue = 0;
	regSecEffect.m_maxValue = 100;
	regSecEffect.m_intLen = 0;
	regSecEffect.m_floatLen = 0;
	regSecEffect.m_totalSize = 4;

	if (ReadData(&regSecEffect,pDataBuf))
	{
		return true;
	} 
	else
	{
		return false;
	}
}


bool C_LcdDataIface::WriteEffectData(char *pDataBuf)
{
	S_RegSec regSecEffect;
	regSecEffect.m_regid = 0x0601;
	regSecEffect.m_regSubId = 0x5121;
	regSecEffect.m_pos = 4;
	regSecEffect.m_len = 1;
	regSecEffect.m_type = HEXCODE;
	regSecEffect.m_firstByte = 0xff;
	regSecEffect.m_step = 1;
	regSecEffect.m_minValue = 0;
	regSecEffect.m_maxValue = 100;
	regSecEffect.m_intLen = 0;
	regSecEffect.m_floatLen = 0;
	regSecEffect.m_totalSize = 4;

	if (WriteData(&regSecEffect,pDataBuf))
	{
		return true;
	} 
	else
	{
		return false;
	}
}


bool C_LcdDataIface::WriteData2Dsp(S_RegSec *regSec)
{
	if ( NULL == regSec)
	{
		return false;
	}
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemBySubRegid((S_WORD)regSec->m_regSubId);
	if (NULL == pRegItem)
	{
		return false;
	}

	C_DspDevice dsp;
	if (!dsp.Write((S_WORD)regSec->m_regid,pRegItem->m_data,regSec->m_totalSize))
	{
		loget<<"发送数据到DSP失败\n"<<endl;
		return false;
	}
	return true;
}



