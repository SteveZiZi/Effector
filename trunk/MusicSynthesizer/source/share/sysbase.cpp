/** @file
 *  @brief �û���ϵͳ�����ඨ��
 *  @author Steven Wan
 *  date 2015/01/06
 *  @version 0.1
 */
#include "sysbase.h"
#include "status.h"
#include "timeop.h"
#include "regitemext.h"

void S_SYSCFG::Init(void)
{
	memset(this, 0x00, sizeof(*this));
	m_InitIRDA = true;
	m_HeartBeatRetryTimes = 3;
	m_ReportAgainIntervalM = 120;
}


/******************* ���ݱ�ʾ*********************/
/*
brief: ��������������
param [in]char *pBuf:������ ���ݻ�����ָ��
param [in]int len:����������������
return bool:true:�ɹ�false:ʧ��
author:Steven Wan
date: 2015-04-23
*/  
bool S_RegItem::SaveData(const S_CHAR *pBuf, S_WORD len)
{
	bool ret = false;
	if ((len != m_Len)&&(0x0c01 != m_regid))
	{
		loget<<"�������ݳ���Ϊ"<<len<<",�������������Ҫ��"<<endl;
		return false;
	}
	
	switch(m_regid)
	{
		case 0x0b01:
			ret = C_RegitemExt_0b01::SaveData(pBuf,len);
			break;
		case 0x0101:
			ret = C_RegitemExt_0101::SaveData(pBuf,len);
			break;
		case 0x0102:
			ret = C_RegitemExt_0102::SaveData(pBuf,len);
			break;
		case 0x0103:
			ret = C_RegitemExt_0103::SaveData(pBuf,len);
			break;
		case 0x0104:
			ret = C_RegitemExt_0104::SaveData(pBuf,len);
			break;
		case 0x0105:
			ret = C_RegitemExt_0105::SaveData(pBuf,len);
			break;
		case 0x0106:
			ret = C_RegitemExt_0106::SaveData(pBuf,len);
			break;
		case 0x0107:
			ret = C_RegitemExt_0107::SaveData(pBuf,len);
			break;
		case 0x0108:
			ret = C_RegitemExt_0108::SaveData(pBuf,len);
			break;
		case 0x0109:
			ret = C_RegitemExt_0109::SaveData(pBuf,len);
			break;
		case 0x010A:
			ret = C_RegitemExt_010A::SaveData(pBuf,len);
			break;
		case 0x010B:
			ret = C_RegitemExt_010B::SaveData(pBuf,len);
			break;
		case 0x010C:
			ret = C_RegitemExt_010C::SaveData(pBuf,len);
			break;
		case 0x010D:
			ret = C_RegitemExt_010D::SaveData(pBuf,len);
			break;
		case 0x010E:
			ret = C_RegitemExt_010E::SaveData(pBuf,len);
			break;
		case 0x010F:
			ret = C_RegitemExt_010F::SaveData(pBuf,len);
			break;
		case 0x0201:
			ret = C_RegitemExt_0201::SaveData(pBuf,len);
			break;
		case 0x0202:
			ret = C_RegitemExt_0202::SaveData(pBuf,len);
			break;
		case 0x0203:
			ret = C_RegitemExt_0203::SaveData(pBuf,len);
			break;
		case 0x0301:
			ret = C_RegitemExt_0301::SaveData(pBuf,len);
			break;
		case 0x0401:
			ret = C_RegitemExt_0401::SaveData(pBuf,len);
			break;
		case 0x0601:
			ret = C_RegitemExt_0601::SaveData(pBuf,len);
			break;
		case 0x1101:
			ret = C_RegitemExt_1101::SaveData(pBuf,len);
			break;
		case 0x1003:
			ret = C_RegitemExt_1003::SaveData(pBuf,len);
			break;
		case 0x0c01:
			ret = C_RegitemExt_0C01::SaveData(pBuf,len);
			break;
		default:
			memcpy(m_data,pBuf,len);
			ret = true;
			break;
	}
	SystemCmd(CONS_SYNC_COMMAND"&");
	return ret;		
}

/*
brief: ��ȡ����������
param [in]char *pBuf: ���ݻ�����ָ��
return int��ȡ���������
author:Steven Wan
date: 2015-04-23
*/  
S_WORD S_RegItem::ReadData(S_CHAR *pBuf)
{
	switch(m_regid)
	{
		case 0x0b01:
			C_RegitemExt_0b01::ReadData(pBuf);
			break;

		default:
			memcpy(pBuf,m_data,m_Len);
			break;
			
	}
	return m_Len;
}
/*
brief: ������ȡȫ������������
param [out]string &oStrData:������ȡ���������ݴ�
return int��ȡ�����ݳ���
author:Steven Wan
date: 2015-06-23
*/  
S_WORD S_RegItem::BatchReadData(string &oStrData)
{
	return C_RegitemExt_FFFF::ReadData(oStrData);
}

/*
brief: �ж������������Ƿ���Ч
param [in]void
return bool:true,��Ч��false:��Ч
author:Steven Wan
date: 2015-06-23
*/  
bool S_RegItem::IsValidData(void)
{
	S_CHAR invaliddata[CONS_REGITEM_DATABUF_MAXLEN];
	memset(invaliddata,0xff,CONS_REGITEM_DATABUF_MAXLEN);
	if (memcmp(m_data,invaliddata,m_Len) == 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
/*
brief: get RegItem type according to RegItem name
param [in]INT16U regid: regid of RegItem 
return S_RegItem*:a pointer point to address of regitem which have found by regid  
author:Steven Wan
date: 2012-04-09
*/  
S_RegItem* S_RegItems::GetDataItemByRegid(S_WORD regid)
{
	for (S_WORD i=0; i<m_count; i++)
	{
		if (m_Items[i].m_regid == regid)
		{
			return &m_Items[i];
		}
	}
	return NULL;
}

/*
brief: get RegItem type according to subregid of RegItem 
param [in]INT16U subregid: subregid of RegItem 
return S_RegItem*:a pointer point to address of regitem which have found by regid  
author:Steven Wan
date: 2015-06-24
*/  
S_RegItem* S_RegItems::GetDataItemBySubRegid(S_WORD subregid)
{
	for (S_WORD i=0; i<m_count; i++)
	{
		if (m_Items[i].m_subregid == subregid)
		{
			return &m_Items[i];
		}
	}
	return NULL;
}
/*
brief: get RegItem type according to RegItem name
param [in]S_WORD Index:index of regitems arry 
return S_RegItem*:a pointer point to address of regitem which have found by regid  
author:Steven Wan
date: 2012-04-09
*/  
S_RegItem* S_RegItems::GetDataItemByIndex(S_WORD Index)
{
	if (Index >= m_count)
	{
		loget<<"����������������Χ"<<endl;
		return NULL;
	}
	return &m_Items[Index];
}
/*	brief: add a RegItem type to RegItem types muster
	param [in] S_RegItem member pointer
	return bool 
	author:Steven Wan
	date: 2012-04-13
*/  
bool S_RegItems::AddItem(S_RegItem *pItem)
{
	if (m_count > CONS_REGITEMS_MAX)
	{
		loget<<"�Ѿ�����ϵͳ������������������\n"<<endl;
		return false;
	}
	m_Items[m_count++] = *pItem;
	return true;
}

