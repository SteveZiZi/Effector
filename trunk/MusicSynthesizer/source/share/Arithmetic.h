/**
 * @FILENAME: Arithmetic.h
 * @BRIEF define basal Arithmetic function
 * @AUTHOR Steven Wan
 * @DATE 2015-02-09
 * @VERSION 1.0
*/

#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_ 

#include <math.h>
#include <time.h>
#include "basedef.h"
#include "timeop.h"
#include "sysbase.h"
#include "basealgo.h"

/*
	brief: convert char to bcd 
	param [in] char cHexData:char type value
	param [out]void
	return char: bcd value
	author:Steven Wan
	date: 2015-02-09
*/
char ConvertCharToBcd(char cHexData);

/*
	brief: convert bcd to char 
	param [in] char cHexData:char type value
	param [out]void
	return char: bcd value
	author:Steven Wan
	date: 2015-02-09
*/
char ConvertBcdToChar(char cBCDData);

/*
	brief: get current system time by YY/MM/DD/MIN/SS(201202090916),total 7 bytes,format BCD code 
	param [in] char* o_cpBcdTime:the data buffer that store current time.
	param [out]char* o_cpBcdTime:the data buffer that store current time. 
	return void: NONE
	author:Steven Wan
	date: 2015-02-09
*/
void GetCurBCDTimeByYMDHMS(char* p7BcdTime);

/*
	brief: Convert 7bytes bcd time to string display,ex "2015-02-12 11:54:32" 
	param [in] char* p7BcdTime:the bcd time buffer that will be convert to string.
	param [out] none
	return void: char *:string pointer
	author:Steven Wan
	date: 2015-02-13
*/
char* ConvertBCDTimeToStr(char* p7BcdTime);

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

bool ConvertDoubleToBCD(double i_dValue,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit,unsigned char* o_cDstBCD);

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
double ConvertBCDToDouble(const unsigned char* i_cBCD,unsigned char i_cZhenShuBit,unsigned char i_cXiaoShuBit);
/*
	brief: Convert ASCII char to hex char
	param [in] char i_cAscChar
	return unsigned char: hex char 
	author:Steven Wan
	date: 2015-02-13
*/
unsigned char ConvertAsciiToHex(char i_cAscChar);

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

bool IsVaildBCDData(const unsigned char * data,int dataLen);

/*
	brief: 7 bytes BCD time data valid check
	param [in] const unsigned char* bcd7Data: 
	param [out] none
	return bool: true:the bcd data is valid 
				 false:the bcd data is invalid 
	author:Steven Wan
	date: 2015-02-13
*/

bool IsValidBcdTime(const unsigned char* bcd7Data);

/*
	brief: format string with ","
	param [in]  
	param [out] 
				  
	author:zhgx
	date: 2015-02-23
*/
bool FormatWithComma(string &strobj);
/*
	brief: 
	param [in]  
	param [out] 
				  
	author:zhgx
	date:
*/
int PercentToBCD(char PercentData,C_BCD &bcdData);
/*
	brief: 
	param [in]  
	param [out] 
				  
	author:zhgx
	date:
*/

#endif
