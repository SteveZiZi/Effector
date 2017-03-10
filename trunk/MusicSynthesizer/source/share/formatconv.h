/** @file
 *  @brief 数据转换类
 *  @author Steven Wan
 *  @date 2015-06/13
 *  @version 0.1
 */
#ifndef _FORMATCONV_H_
#define _FORMATCONV_H_
#include "sysbase.h"


class C_DataFormatConv
{
public:
	static bool ConvFloatStringToBcdData(string floatString,S_BYTE *pBcdBuf, S_BYTE bcdBufLen,S_BYTE &intLen,S_BYTE &floatLen);
	static bool ConvIntToBcd(long long val, S_BYTE *pBcdBuf, int bcdLen);
};




#endif

