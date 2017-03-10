/**
 *  @file
 *  @brief 现场总线命令过滤器
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note 现场总线接收数据后调用
 */
#ifndef __BASEB_CMD_FILTER_H__
#define __BASEB_CMD_FILTER_H__
#include "basedef.h"

class C_CmdFilter
{
public:
	/** @brief 命令过滤 */
	C_CmdFilter(const string &strFilterName);
	~C_CmdFilter();

public:
	/** @brief 命令过滤
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	 */
	int operator()(const string &strBuffer, string &strCmd)const;

private:
	string m_strFilterName; //命令过滤器名称
};

#endif //__BASEB_CMD_FILTER_H__

