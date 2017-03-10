/** @file
 *  @brief 基础类strings定义
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 */
#ifndef __FND_STRINGS_H__
#define __FND_STRINGS_H__
#include "basedef.h"

/** @class
 *  @brief string串数组
 */
class strings:public vector<string>
{
public:
	/** @brief 按指定间隔符分离字符串
	 *  @param[in] strSrc 源字符串
	 *  @param[in] delimit 间隔符
	 *  @return 分开的字串个数
	 */
	int split(const string &strSrc, char delimit);

	/** @brief 追加一个字串数组
	 *  @param[in] strsAppend:待追加的字串数组
	 *  @return strings&
	 */
	strings & append(const strings& strsAppend)
	{		
		insert(end(), strsAppend.begin(), strsAppend.end());
		return *this;
	}
	/** @brief 测试串
	 *  @return true任意一个串非空;false所有串为空
	 */
	bool Any(void)const;
	/** @brief 测试串
	 *  @return true任意一个串空;false所有串为非空
	 */
	bool AnyEmpty(void)const;
	/** @brief 统计串数组中有多少个非空串
	 *  @return 非空串统计值
	 */
	S_WORD Count(void)const;
	/** @brief 数据串行化
	 *  @return 串行化后长度
	 */
	S_WORD Serialize(string &strData)const;
};


#endif//__FND_FNDVERSION_H__

