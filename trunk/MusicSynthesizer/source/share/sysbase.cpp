/** @file
 *  @brief 用户层系统基础类定义
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


/******************* 数据标示*********************/
/*
brief: 保存数据项数据
param [in]char *pBuf:带保存 数据缓冲区指针
param [in]int len:待保存数据区长度
return bool:true:成功false:失败
author:Steven Wan
date: 2015-04-23
*/  
bool S_RegItem::SaveData(const S_CHAR *pBuf, S_WORD len)
{
	bool ret = false;
	if ((len != m_Len)&&(0x0c01 != m_regid))
	{
		loget<<"保存数据长度为"<<len<<",不符合数据项长度要求"<<endl;
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
brief: 获取数据项数据
param [in]char *pBuf: 数据缓冲区指针
return int读取的数据项长度
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
brief: 批量获取全部数据项数据
param [out]string &oStrData:批量读取数据项数据串
return int读取的数据长度
author:Steven Wan
date: 2015-06-23
*/  
S_WORD S_RegItem::BatchReadData(string &oStrData)
{
	return C_RegitemExt_FFFF::ReadData(oStrData);
}

/*
brief: 判断数据项数据是否有效
param [in]void
return bool:true,有效；false:无效
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
		loget<<"超出数据项索引范围"<<endl;
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
		loget<<"已经超过系统定义的最大数据项数量\n"<<endl;
		return false;
	}
	m_Items[m_count++] = *pItem;
	return true;
}

