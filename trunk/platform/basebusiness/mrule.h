/**
 *  @file
 *  @brief 表计规约基础类
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note
 */
#ifndef __BASEB_MRULE_H__
#define __BASEB_MRULE_H__
#include "basedef.h"
#include "basetype.h"

enum
{
	MRULE_PARSE_ERR_NO = 0, //成功
	MRULE_PARSE_ERR_ADDR = -1, //地址错
	MRULE_PARSE_ERR_DI = -2, //数据项错
	MRULE_PARSE_ERR_CTRL = -3, //控制码错
	MRULE_PARSE_ERR_REG = -4, //寄存器错
	
	MRULE_PARSE_ERR_OTHER = -9 //其它错
};

/** @brief 表计寄存器与通用寄存器映射 */
struct S_RegisterMap
{
	S_DWORD m_CommID;///< 通用寄存器ID
	S_DWORD m_LocalID; ///< 表计寄存器ID

public:
	S_RegisterMap(void){memset(this, 0x00, sizeof(*this));}
};
struct S_RegisterMaps:public vector<S_RegisterMap>
{
};
bool operator<(const S_RegisterMap &left, const S_RegisterMap &right);

/** @brief 表计规约抽象接口定义 */
class C_MRULE
{
public:
	C_MRULE(void);
	virtual ~C_MRULE();

public:
	S_COMSET GetComSet(void);
	/** @brief 获取表计规约名称 */
	virtual char *GetName(void);
	/** @brief 清除规约缓冲*/
	virtual bool ClearBuffer(void);

	/** @brief 提取命令帧
	 *  @param[in] strBuffer 命令缓冲
	 *  @param[out] strCmd 帧输出缓冲
	 *  @return <0: 出错[绝对值是出错的字节数]; =0: 半帧; >0: 成功
	 */
	virtual int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief 组读命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <0: 出错或无此要求帧; =0: 无需组帧，直接分析缓冲; >0: 组帧成功
	 */
	virtual int MakeReadCmd(const string &strAddress, const S_REGISTER &CommReg, S_DATAGRAM &DataGram);
	/** @brief 分析读命令返回帧
	 *  @param[in] strCmd 返回命令帧,如为空则分析缓冲
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];=0:成功
	 */
	virtual int ParseReadCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief 组写命令帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] strData 数据输入缓冲
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <0: 出错或无此要求帧; =0: 无需组帧，直接分析缓冲; >0: 组帧成功
	 */
	virtual int MakeWriteCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strData, S_DATAGRAM &DataGram);
	/** @brief 分析写命令返回帧
	 *  @param[in] strCmd 返回命令帧
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出缓冲
	 *  @return <0:出错[参见MRULE_PARSE_ERR_];=0:成功
	 */
	virtual int ParseWriteCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief 组广播校时帧
	 *  @param[in] strAddress 表计地址
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] timeExpected 期望时间
	 *  @param[out] DataGram 帧输出缓冲
	 *  @return <0: 出错或无此要求帧; =0: 无需组帧，直接分析缓冲; >0: 组帧成功
	 */
	virtual int MakeBroadcastCmd(const string &strAddress, const S_REGISTER &CommReg, S_TIME timeExpected, S_DATAGRAM &DataGram);

protected:
	S_COMSET m_ComSet;
};

#endif //__BASEB_MRULE_H__

