/** @file
 *  @brief �û���ϵͳ״̬������
 *  @author Steven Wan
 *  @date 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_StatusBase�࣬ע�����еĳ��󷽷������ء�
 */
#ifndef __NW_STATUS_H__
#define __NW_STATUS_H__
#include "cfg_stat.h"
#include "sysipc.h"
#include "basetype.h"
#include "ini_serv.h"
#include "ini_sys.h"

class C_STATUS:public C_StatusBase
{
public:
	virtual ~C_STATUS(){}
	static C_STATUS &Instance(void);

	/** @brief ���ó�Ա��Ϣ��
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
	 */
	virtual bool SetInformation(void);
	/** @brief ��ʼ��ϵͳ״̬
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool InitStatus(void);
	/** @brief �������õ�ϵͳ״̬
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool LoadStatus(void);
	/** @brief ��������
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ��������ʱ���Ȼص�������
	 */
	virtual bool ParamParse(void);

public:
	/** @brief Get������*/
	S_SYSCFG *GetSystemConfig(void){return &m_pMyStatus->m_SysConfig;}
	S_SYSRUN *GetSysRunInfo(void){return &m_pMyStatus->m_SysRun;}
	S_CHANNEL *GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN);
	S_CHANNEL *GetPppChannel(int ChannelSN);
	S_HttxInfo *GetHttxInfo(S_HttxSN httxSN){return &m_pMyStatus->m_HttxInfo[httxSN];}
	S_PppInfo *GetPppInfo(void){return &m_pMyStatus->m_PppInfo;}
	S_IpConfig *GetIpConfig(void){return &m_pMyStatus->m_IpCofig;}
	S_LcdInfo *GetLcdInfo(void){return &m_pMyStatus->m_LcdInfo;}
	S_DnChannel *GetDnChannelInfo(void){return &m_pMyStatus->m_DnChannels;}
	
public:
	S_RegItems *GetRegitemsArea(void){return &m_pMyStatus->m_RegItems;}

public:
	/** @brief ���������� */
	int SaveUpChannel(void){return C_IniServer::SaveUpChannl(m_pMyStatus);}
	int SaveHttxInfo(void){return C_IniServer::SaveHttxInfo(m_pMyStatus);}
	int SaveTermIPConfig(void){return C_IniSys::SaveTermIPConfig(m_pMyStatus);}
	int SavePppInfo(void){return C_IniServer::SavePppInfo(m_pMyStatus);}
	int SaveSysRunInfo(void){return C_IniSys::SaveRunInformation(m_pMyStatus);}
	

protected:
	C_STATUS(void){m_pMyStatus = NULL;}


protected:
	S_STATUS *m_pMyStatus;
};

#define STATUS C_STATUS::Instance()

#endif//__NW_STATUS_H__

