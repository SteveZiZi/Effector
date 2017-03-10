/**
 *  @file
 *  @brief 表计通讯
 *  @author Steven Wan
 *  @date 2015-03-21
 *  @version v1.0
 *  @note
 *  完成表计采集功能
 *  完成表计（现场总线）数据上报功能
 */
#ifndef __BASEB_METER_COMMUNICATION_H__
#define __BASEB_METER_COMMUNICATION_H__
#include "basedef.h"
#include "mrule.h"
#include "fieldbus.h"

/** @class
 *  @brief 表计通讯
 */
class C_MeterCom
{
public:
	C_MeterCom(void);
	~C_MeterCom();

public:
	/** @fn
	 *  @brief 初始化所有配置的表计规约
	 *  @return 0:成功; -1:失败
	 */
	int InitMeterRules(void);

	/** @brief 清除缓冲
	 *  @param[in] MeterRuleID 表计规约ID
	 *  @return true 成功；false 失败
	 *  @note 仅在Read操作返回数据异常时需要此操作
	 */
	bool ClearBuffer(int MeterRuleID);
	/** @brief 读表计数据
	 *  @param[in] Meter 表计信息
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strData 数据输出
	 *  @return <0:出错；>=0:成功
	 */
	int Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
	/** @brief 写表计参数
	 *  @param[in] Meter 表计信息
	 *  @param[in] CommReg 通用寄存器
	 *  @param[in] strParam 输入参数
	 *  @param[out] strData 数据输出
	 *  @return <0:出错；>=0:成功
	 */
	int Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData);
	/** @brief 上报表计数据
	 *  @param[in] CommReg 通用寄存器
	 *  @param[out] strMeterAddress 表计地址输出[6BYTES]
	 *  @param[out] strData 数据输出
	 *  @param[in] timeKeeping 上报时间;0:退出上报;0xFFFFFFFF:查询;其它:进入上报
	 *  @return <0:出错 0:结束上报 >0:上报剩余时间
	 */
	int Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping);
	/** @brief 广播表计时间
	 *  @param[in] Meter 表计信息
	 *  @param[in] timeExpected 期望时间
	 *  @return <0:出错；>=0:成功
	 */
	int Broadcast(const S_MeterBase &Meter, S_TIME timeExpected);

protected:
	int InitMeterCommInfo(const S_MeterBase &Meter);
	int ExtractCmd(string &strBuffer, string &strRecvCmd);

protected:
	C_MRULE *m_pMRule;
	C_FieldBus *m_pFB;
};

#endif //__BASEB_METER_COMMUNICATION_H__


