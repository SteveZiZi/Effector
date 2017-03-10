/**
 *  @file
 *  @brief �����ֳ����߳���ӿ�
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note 
 */
#ifndef __BASEB_FIELDBUS_H__
#define __BASEB_FIELDBUS_H__
#include "basedef.h"
#include "basetype.h"
#include "timeop.h"
#include "connect.h"
#include "strings.h"
#include "cmdfilt.h"

/**
 *  @class C_FieldBus
 *  @brief �ֳ����߿�����
 *  @author Steven Wan
 *  @note
 *  1���ֳ����߳���ӿڣ�
 *  2���ֳ����߸��࣬���е��ֳ����߶���̳и��࣬��485���ߣ��ز����ߣ�΢�����������ߣ�
 *  3���������ֳ����߱����ṩ�Ĺ��ܡ�
 */
class C_FieldBus
{
public:
	/** @brief �ֳ����ߵ�״̬�� */
	enum{FB_STATUS_NORMAL,FB_STATUS_REPORT_DATA,FB_STATUS_REPORT_METER,FB_STATUS_ROUTER};
public:
	C_FieldBus(void);
	virtual ~C_FieldBus();

public:
	const char *GetName(void);
	C_CONNECT *GetConnection(void);
	C_CONNECT *SetConnection(C_CONNECT *pConnection);
	C_CmdFilter *SetCommandFilter(C_CmdFilter *pCmdFilter);
	
	/** @brief �ֳ�����Ӳ��λ��������λ���ţ� 
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int Reset(void);
	/** @brief ������������ 
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int ChangeConnectionProperty(const S_COMSET &ComSet);
	/** @brief �ֳ�����״̬ͬ�������ϵ�����������Ҫ״̬ͬ�� 
	 *  @param[in] strModuleAddr ģ���ַ����û�У���ͬ����ַ
	 *  @param[out] strFactory ���ģ�鹤������Ͱ汾��
	 *  @return 0:�ɹ�; -1:ʧ��
	 */
	virtual int SyncStatus(const string &strModuleAddr, string &strFactory);
	/** @brief �ֳ�����·����������ѯ���� 
	 *  @param[in] timeKeeping ·��ʱ�䣬0:�˳�·��;0xFFFFFFFF:��ѯ·��״̬;����������·��
	 *  @param[in] Meter485 �ɼ�����485���ַ
	 *  @param[in] MeterZB �ز����ַ
	 *  @param[in] Relay �ɼ�����ַ
	 *  @return <0:ʧ�� 0:�ɹ���·�ɽ��� >0:ʣ��·��ʱ��,��λ��
	 */
	virtual int Router(S_TIME timeKeeping, const S_TERMINALS &Meter485, const S_TERMINALS &MeterZB, const S_TERMINALS &Relay);
	/** @brief �ֳ����ߵ����������ϱ����� 
	 *  @param[out] CtlCmd ��������
	 *  @param[out] DataGram �������
	 *  @param[in] timeKeeping �ϱ�ʱ��;0:�˳��ϱ�;0xFFFFFFFF:��ѯ;����:�����ϱ�
	 *  @return <0:���� 0:�����ϱ� >0:�ϱ�ʣ��ʱ��
	 */
	virtual int ReportData(int &CtlCmd, S_DATAGRAM &DataGram, S_TIME timeKeeping);
	/** @brief �ֳ����ߵ��������ϱ����� 
	 *  @param[out] strMeterAddr ��Ƶ�ַ
	 *  @param[in] timeKeeping �ϱ�����ʱ��
	 *  @return -1:�ϱ����� 0:�ϱ����� >0:ʣ���ϱ�ʱ��
	 */
	virtual int ReportMeter(string &strMeterAddr, S_TIME timeKeeping);
	/** @brief �ֳ����ߵ����ݳ������� 
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @param[out] strData �������
	 *  @return <0:���� >=0:�ɹ�
	 */
	virtual int RequestData(const string &strRelay, const S_DATAGRAM &DataGram, string &strData);
	/** @brief �㲥���� 
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @return <0:���� >=0:�ɹ�
	 */
	virtual int Broadcast(const string &strRelay, const S_DATAGRAM &DataGram);
	/** @brief �ֳ����ߵ������������ 
	 *  @param[in] CmdID ����ID
	 *  @param[in] strRelay �м̵�ַ+Ŀ���ַ��Ŀ���ַ�����ÿ����ַռ6�ֽڣ�
	 *  @param[in] DataGram �������
	 *  @param[out] strData �������
	 *  @return <0:���� >=0:�ɹ�
	 */
	virtual int RequestCmd(int CmdID, const string &strRelay, const S_DATAGRAM &DataGram, string &strData);

protected:
	int ClearConnectionBuffer(void);
	int SendCmd(const string &strCmd);
	int RecvCmd(string &strCmd);

protected:
	const char *m_pFieldBusName;///< �ֳ����߿���������
	C_CONNECT *m_pConnection;///< �ֳ����߿�������������
	C_CmdFilter *m_pCmdFilter;
};

#endif//__BASEB_FIELDBUS_H__

