/**
 *  @file
 *  @brief ���ͨѶ
 *  @author Steven Wan
 *  @date 2015-03-21
 *  @version v1.0
 *  @note
 *  ��ɱ�Ʋɼ�����
 *  ��ɱ�ƣ��ֳ����ߣ������ϱ�����
 */
#ifndef __BASEB_METER_COMMUNICATION_H__
#define __BASEB_METER_COMMUNICATION_H__
#include "basedef.h"
#include "mrule.h"
#include "fieldbus.h"

/** @class
 *  @brief ���ͨѶ
 */
class C_MeterCom
{
public:
	C_MeterCom(void);
	~C_MeterCom();

public:
	/** @fn
	 *  @brief ��ʼ���������õı�ƹ�Լ
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	int InitMeterRules(void);

	/** @brief �������
	 *  @param[in] MeterRuleID ��ƹ�ԼID
	 *  @return true �ɹ���false ʧ��
	 *  @note ����Read�������������쳣ʱ��Ҫ�˲���
	 */
	bool ClearBuffer(int MeterRuleID);
	/** @brief ���������
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �������
	 *  @return <0:����>=0:�ɹ�
	 */
	int Read(const S_MeterBase &Meter, const S_REGISTER &CommReg, string &strData);
	/** @brief д��Ʋ���
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] strParam �������
	 *  @param[out] strData �������
	 *  @return <0:����>=0:�ɹ�
	 */
	int Write(const S_MeterBase &Meter, const S_REGISTER &CommReg, const string &strParam, string &strData);
	/** @brief �ϱ��������
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strMeterAddress ��Ƶ�ַ���[6BYTES]
	 *  @param[out] strData �������
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
	 */
	int Report(const S_REGISTER &CommReg, string &strMeterAddress, string &strData, S_TIME timeKeeping);
	/** @brief �㲥���ʱ��
	 *  @param[in] Meter �����Ϣ
	 *  @param[in] timeExpected ����ʱ��
	 *  @return <0:����>=0:�ɹ�
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


