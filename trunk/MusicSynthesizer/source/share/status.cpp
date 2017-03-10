/** @file
 *  @brief �û���ϵͳ״̬������
 *  @author Steven Wan
 *  @date 2015/01/06
 *  @version 0.1
 *  @see mprocess/sharemem.h mrpocess/sysipc.h
 *  ע����Ҫ�̳�C_StatusBase�࣬ע�����еĳ��󷽷������ء�
 */
#include "status.h"
#include "ini_sys.h"
#include "ini_serv.h"
#include "timeop.h"
#include "regitem.h"
#include "parammode.h"

#include"Arithmetic.h"

C_STATUS &C_STATUS::Instance(void)
{
	static C_STATUS s_MyStatus;
	return s_MyStatus;
}

/** @brief ���ó�Ա��Ϣ��
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص��������������ڴ洴���ɹ�ʱ���ñ�����
 */
bool C_STATUS::SetInformation(void)
{
	if (C_StatusBase::SetInformation() == false)
		return false;
	m_pMyStatus = (S_STATUS *)m_pStatus;
	return true;
}

/** @brief ��ʼ��ϵͳ״̬
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::InitStatus(void)
{
	if (C_StatusBase::InitStatus() != true)
		return false;


	m_pMyStatus->m_SysConfig.Init();
//	m_pMyStatus->m_SysRun.Init();
	m_pMyStatus->m_IpCofig.Init();
	m_pMyStatus->m_UpChannels.Init(); ///< ����ͨ����Ϣ
	m_pMyStatus->m_DnChannels.Init(); ///< ����ͨ����Ϣ

	for (int i = 0; i < HTTX_PROCESS_MAX; i++)
	m_pMyStatus->m_HttxInfo[i].Init();
	m_pMyStatus->m_PppInfo.Init();
	m_pMyStatus->m_LcdInfo.Init();
	m_pMyStatus->m_MenuConfig.Init(); ///< �˵�ҳ������Ϣ��ʼ��

	/* ���ݱ�ʾ����*/
	//m_pMyStatus->m_RegItems.Init();


	return true;
}

/** @brief �������õ�ϵͳ״̬
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::LoadStatus(void)
{
	if (C_StatusBase::LoadStatus() != true)
		return false;
	C_IniSys::LoadRunInformation(m_pMyStatus);
	C_IniSys::LoadPortMap(m_pMyStatus);
	C_IniSys::LoadTermIPConfig(m_pMyStatus);
	C_IniServer::LoadUpChannl(m_pMyStatus);
	C_IniServer::LoadDnChannl(m_pMyStatus);
	C_IniServer::LoadHttxInfo(m_pMyStatus);
	C_IniServer::LoadPppInfo(m_pMyStatus);
	C_IniServer::LoadLcdInfo(m_pMyStatus);
	C_ParamMode::LoadTermMenuConfig(m_pMyStatus); // ����ģʽ������Ϣ
	C_RegitemCfg::LoadRegItems();


	return true;
}

/** @brief ��������
 *  @return true �ɹ���false ʧ��
 *  @remarks ��������ʱ���Ȼص�������
 */
bool C_STATUS::ParamParse(void)
{
	if (C_StatusBase::ParamParse() != true)
		return false;


	//���帺��Ӳ��״̬������
	return true;
}

S_CHANNEL *C_STATUS::GetHttxChannel(S_HttxSN HttxSN, S_ChannelSN ChannelSN)
{
	if (HttxSN > HTTX_PROCESS_MAX || ChannelSN > HTTX_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_HttxInfo[HttxSN].m_ChannlName[ChannelSN]);
}

S_CHANNEL *C_STATUS::GetPppChannel(int ChannelSN)
{
	if (ChannelSN < 0 || ChannelSN > PPP_CHANNL_MAX)
		return NULL;
	return m_pMyStatus->m_UpChannels.FindChannel(m_pMyStatus->m_PppInfo.m_ChannlName[ChannelSN]);
}




