#include "Arithmetic.h"

/*
	brief: convert char to bcd 
	param [in] char cHexData:char type value
	param [out]void
	return char: bcd value
	author:Steven Wan
	date: 2015-02-09
*/
char ConvertCharToBcd(char cHexData)
{
   char i;
   i=((cHexData%100)/10)*16+(cHexData%10);
   return i;
}

/*
	brief: convert bcd to char 
	param [in] char cHexData:char type value
	param [out]void
	return char: bcd value
	author:Steven Wan
	date: 2015-02-09
*/
char ConvertBcdToChar(char cBCDData)
{
   char i;
   i=((cBCDData>>4)&0x0f)*10+(cBCDData&0x0f);
   return i;
}
/*
	brief: get current system time by YY/MM/DD/MIN/SS(201202090916),total 7 bytes,format BCD code 
	param [in] char* o_cpBcdTime:the data buffer that store current time.
	param [out]char* o_cpBcdTime:the data buffer that store current time. 
	return void: NONE
	author:Steven Wan
	date: 2015-02-09
*/
void GetCurBCDTimeByYMDHMS(char* p7BcdTime)
{
	time_t tt;
	struct tm *curtime;

	tt = (time_t)(C_TIME::TimeNow());
	curtime=localtime(&tt);
	p7BcdTime[0]=ConvertCharToBcd((curtime->tm_year+1900)/100);
	p7BcdTime[1]=ConvertCharToBcd((curtime->tm_year+1900)%100);
	p7BcdTime[2]=ConvertCharToBcd(curtime->tm_mon+1);
	p7BcdTime[3]=ConvertCharToBcd(curtime->tm_mday);
	p7BcdTime[4]=ConvertCharToBcd(curtime->tm_hour);
	p7BcdTime[5]=ConvertCharToBcd(curtime->tm_min);
	p7BcdTime[6]=ConvertCharToBcd(curtime->tm_sec);
}

/*
	brief: Convert 7bytes bcd time to string display,ex "2015-02-12 11:54:32" 
	param [in] char* p7BcdTime:the bcd time buffer that will be convert to string.
	param [out] none
	return void: char *:string pointer
	author:Steven Wan
	date: 2015-02-13
*/
char* ConvertBCDTimeToStr(char* p7BcdTime)
{
	unsigned char* pBcdTime=(unsigned char*)p7BcdTime;
	static char sTimeStr[19];
	sprintf(sTimeStr,"%02X%02X-%02X-%02X %02X:%02X:%02X",pBcdTime[0],pBcdTime[1],pBcdTime[2],pBcdTime[3],pBcdTime[4],pBcdTime[5],pBcdTime[6]);
	return sTimeStr;
}

/*
	brief: Convert double type data to BCD data
	param [in] double i_dValue
			   unsigned char i_cZhenShuBit:	integral digis
			   unsigned char i_cXiaoShuBit: decimal digis
			   unsigned char* o_cDstBCD:bcd data
	param [out] none
	return void: char *:string pointer
	author:Steven Wan
	date: 2015-02-13
*/
bool ConvertDoubleToBCD(double i_dValue,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit,unsigned char* o_cDstBCD)
{
	char tmp[300];
	//char tmp1[300];
	double Value;
	size_t total;
	unsigned char ByteSize;
	unsigned char CharIndex;
	unsigned char ByteIndex;
	char xx;

	//保证字节完整性
	if ( (i_cZhenShuBit+i_cXiaoShuBit)%2!= 0) 
	{
		return false;
	}
	
	//把目的地址清空为0
	ByteSize=(i_cZhenShuBit+i_cXiaoShuBit)/2;
    memset(o_cDstBCD,0,ByteSize);


	//得到目的字符串如:300.12500(3.5格式)
	Value=fabs(i_dValue);

	//修改格式 sprint("%*.*f",val)指定输出的格式长度
	sprintf(tmp,"%*.*f",i_cZhenShuBit,i_cXiaoShuBit,Value);
	//sprintf(tmp,tmp1,Value);
	
	//以小数位->整数位方向,填充目的串
	total=strlen(tmp);
	CharIndex=0;
    ByteIndex=0;
	for(CharIndex=0;(CharIndex<i_cZhenShuBit+i_cXiaoShuBit)&&(total>0);CharIndex++)
	{
        xx=tmp[total-1];
		if (xx=='.') 
		{
			total--;
			xx=tmp[total-1];
		}
		if (CharIndex%2==0)
		{
			o_cDstBCD[ByteIndex]&=0xf0;
			o_cDstBCD[ByteIndex]|=(ConvertAsciiToHex(xx)&0x0f);
		}
		else
		{
			o_cDstBCD[ByteIndex]&=0x0f;
            o_cDstBCD[ByteIndex]|=((ConvertAsciiToHex(xx)<<4)&0xf0);
			ByteIndex++;
		}
		total--;
	}

	return true;
}
double mypow(int x1)
{
	
	if (x1>=0) return pow(10.0,x1);
	if (x1<0) 
	{
		return ( 1/pow(10.0,abs(x1)) );
	}
	return 0.0;
}
/*
	brief: Convert BCD type data to double data
	param [in] unsigned char* o_cDstBCD:bcd data
			   unsigned char i_cZhenShuBit:	integral digis
			   unsigned char i_cXiaoShuBit: decimal digis
	param [out] none
	return void: double
	author:Steven Wan
	date: 2015-02-13
*/
double ConvertBCDToDouble(const unsigned char* i_cBCD,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit)
{
	double Value=0;
	int  ZhiShu;
	unsigned char CharIndex;
	unsigned char ByteIndex;
	unsigned char ByteSize;
	unsigned char tmp;
	unsigned char flag=0;
	unsigned char CharNum;
	

	if ( (i_cZhenShuBit+i_cXiaoShuBit)%2== 0)
	{
        ByteSize=(i_cZhenShuBit+i_cXiaoShuBit)/2;
		flag=0;
	}
	else
	{
		ByteSize=(i_cZhenShuBit+i_cXiaoShuBit)/2+1;
		flag=1;
	}
	Value=0.0;
	ByteIndex=0;
	CharIndex=0;
	ZhiShu=-1*i_cXiaoShuBit;
	
	if (flag==0) CharNum=ByteSize*2;
	else CharNum=ByteSize*2-1;
	
	for (CharIndex=0;CharIndex<CharNum;CharIndex++)
	{
		if(CharIndex%2==0)
		{
			tmp=i_cBCD[ByteIndex]&0x0f;
			if (tmp>9) return -1;
            Value+=ConvertBcdToChar(tmp)*mypow(ZhiShu);
		}
		else
		{
			tmp=i_cBCD[ByteIndex]>>4&0x0f;
			if (tmp>9) return -1;
			Value+=ConvertBcdToChar(tmp)*mypow(ZhiShu);
			ByteIndex++;
		}
		ZhiShu++;
	}
	return Value;
}
/*
	brief: Convert ASCII char to hex char
	param [in] char i_cAscChar
	return unsigned char: hex char 
	author:Steven Wan
	date: 2015-02-13
*/
unsigned char ConvertAsciiToHex(char i_cAscChar)
{
	unsigned char c;

	if (i_cAscChar>='A') 
	{
		 c=i_cAscChar-'A'+10;
	}
	else 
	{
		c=i_cAscChar-48;
	}
	c%=16;
	return c;
}

/*
	brief: BCD data fomat check
	param [in] const unsigned char * data: bcd data buffer pointer
			   int dataLen:bcd data length
	param [out] none
	return bool: true:the bcd data is valid 
				 false:the bcd data is invalid 
	author:Steven Wan
	date: 2015-02-13
*/

bool IsVaildBCDData(const unsigned char * data,int dataLen)
{
	for (int i=0;i!=dataLen;i++)
	{
		unsigned char tmp=*(data+i);
		if(((tmp&0x0F)>0x09)||((tmp&0xF0)>0x90))
		{
			return false;
		}
	}
	return true;
}

/*
	brief: 7 bytes BCD time data valid check
	param [in] const unsigned char* bcd7Data: 
	param [out] none
	return bool: true:the bcd data is valid 
				 false:the bcd data is invalid 
	author:Steven Wan
	date: 2015-02-13
*/

bool IsValidBcdTime(const unsigned char* bcd7Data)
{
	if (bcd7Data[0]==0x00 || (bcd7Data[0]&0x0f)>0x09 || (bcd7Data[0]&0xf0) > 0x90)
	{
		return false;
	}
	if	((bcd7Data[1]&0x0f)>0x09 || (bcd7Data[1]&0xf0) > 0x90)
	{
		return false;
	}
	if (bcd7Data[2]==0x00 || ((bcd7Data[2]&0x0f)>2)&&((bcd7Data[2]&0xf0) > 0x10))
	{
		return false;
	}
	if (bcd7Data[3]==0x00 || ((bcd7Data[3]&0x0f)>1)&&((bcd7Data[3]&0xf0) > 0x30))
	{
		return false;
	}
	if (((bcd7Data[4]&0x0f)>0x03)&&((bcd7Data[4]&0xf0) > 0x20))
	{
		return false;
	}
	if (((bcd7Data[5]&0x0f)>0x09)&&((bcd7Data[5]&0xf0) > 0x50))
	{
		return false;
	}
	if (((bcd7Data[6]&0x0f)>0x09)&&((bcd7Data[6]&0xf0) > 0x50))
	{
		return false;
	}
	return true;
}

/*
	brief: format string with ","
	param [in]  
	param [out] 
				  
	author:zhgx
	date: 2015-02-23
*/
bool FormatWithComma(string &strobj)
{
	for(INT16U i=1;i<strobj.size();i+=2)
		strobj.insert((size_t)i,1,',');

	return true;
}
/*
	brief:
	param [in]  
	param [out] 
				  
	author:zhgx
	date: 
*/
int PercentToBCD(char PercentData,C_BCD &bcdData)
{
	if((PercentData + 100) < 0)
		PercentData = 0;
	unsigned long Integer = (100+PercentData)/100;
	unsigned short Float = ((100+PercentData)%100)*100;

	C_BCD bcdTemp(Integer,Float);
	bcdData = bcdTemp;

    return 0;
}

