#include "formatconv.h"

/*
brief: ������ת��ΪBCD���ʽ�������뻺����
param [in]long long val:������ֵ
param [in]int bcdLen:���ݻ���������
param [out]S_BYTE *pBcdBuf:BCD���ʽ���ݴ洢��ָ��
return true:ת���ɹ���false:ת��ʧ��
author:Steven Wan
date: 2015-06-13
*/ 
bool C_DataFormatConv::ConvIntToBcd(long long val, S_BYTE *pBcdBuf, int bcdLen)
{
	char buf[20]={0};
	int size=0;

	if (pBcdBuf == NULL || bcdLen < 1)
	{
		return false;
	}
	
	memset(pBcdBuf,0x00,bcdLen);	
	
	for(int i =0 ;i<bcdLen*2;i++) 
	{ 
		buf[i] = (char)(val%10); 
		val/=10; 
	}
	
	size = bcdLen*2;
	for(int i=0,j=0; i< size;) 
	{ 
		pBcdBuf[j] = 0x00;
		pBcdBuf[j] = (unsigned char)((buf[i++])&0x0f);
		pBcdBuf[j] |= (unsigned char)(((buf[i++])&0x0f)<<4);
		j++;
	}
	return true;
}

/*
brief: ��������ʽ�ִ�ת��ΪBCD���ʽ�������뻺����
param [in]string floatString:��������ʽ�ִ�
param [in]int bcdLen:���ݻ���������
param [out]S_BYTE *pBcdBuf:BCD���ʽ���ݴ洢��ָ��
return true:ת���ɹ���false:ת��ʧ��
author:Steven Wan
date: 2015-06-13
*/ 
bool C_DataFormatConv::ConvFloatStringToBcdData(string floatString,S_BYTE *pBcdBuf, S_BYTE bcdBufLen,S_BYTE &intLen,S_BYTE &floatLen)
{
	strings floatStrs;
	floatStrs.split(floatString.c_str(),'.');
	intLen = floatStrs[0].size();
	floatLen = floatStrs[1].size();
	string::iterator iterDot = find(floatString.begin(),floatString.end(),'.');
	floatString.erase(iterDot);	
	long long value = atoi(floatString.c_str());
	return ConvIntToBcd(value, pBcdBuf,  bcdBufLen);
}

