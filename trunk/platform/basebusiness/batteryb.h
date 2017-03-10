/**
 *  @file
 *  @brief ������������
 *  @author Steven Wan
 *  @date 2014-12-26
 *  @version v1.0
 */
#ifndef __BASEB_BATTERY_BASE_H__
#define __BASEB_BATTERY_BASE_H__
#include "basedef.h"
#include "timeop.h"

/** @class
 *  @brief ��س�����
 *  @remarks ����BatteryCharge()֮ǰ�ȵ���InitDevice()
 */
class C_BatteryBase
{
public:
	enum{BATTERY_CHARGE_LIMIT=90, BATTERY_CHARGE_FULL=100};

public:
	C_BatteryBase(void);
	virtual ~C_BatteryBase();

	/** @brief ����豸��ʼ�� */
	int InitDevice(void);
	/** @brief ������ */
	int BatteryCharge(void);

protected:
	/** @brief �����ж�
	 *  @return true ϵͳ���磻false ϵͳδ����
	 */
	virtual bool GetPowerOff(void);
	/** @brief ˢ�³��״̬
	 *  @param[in] fCharge true ���ڳ��;false δ���
	 */
	virtual int SetBatteryChargeStatus(bool fCharge);
	/** @brief ˢ�µ�ǰ��ص���
	 *  @param[in] Capability ��ǰ��ص���
	 */
	virtual int SetBatteryCapability(int Capability);
	/** @brief ȡ�����С������
	 *  @return �����С������
	 */
	virtual int GetBatteryChargeLimit(void);
	/** @brief ȡ���������
	 *  @return ���������
	 */
	virtual int GetBatteryChargeFull(void);
	/** @brief ȡ��س��ʱ��
	 *  @return ��س��ʱ��
	 *  @remarks ��λ����
	 */
	virtual int GetBatteryChargeHours(void);

	int _BatteryCharge(void);
	int BatteryChargeProcess(int CurBatteryLevel);
	int BatteryNormalProcess(int CurBatteryLevel);

	int GetBatteryCap(void);
	int EnableBatteryCharge(void);
	int DisableBatteryCharge(void);

protected:
	bool m_BatteryChargeState;///< ���״̬:true ���;false����
	S_TIME m_Charge100Time;///< ��س䵽100��ʱ�䣬�Ӵ�ʱ���ٳ�AfterCap100ChargeHoursʱ�䣬����
	int m_CurBatteryCap;///< ��ǰ��ص���

	static const int AfterCap100ChargeHours = 6*60*60;///< �䵽100%���ٳ�೤ʱ��
};

#endif//__BASEB_BATTERY_BASE_H__

