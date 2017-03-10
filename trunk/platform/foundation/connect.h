/** @file
 *  @brief ���ӳ����ඨ��
 *  @author Steven Wan
 *  @date 2015-01/28
 *  @version 0.1
 *  @note
 *  �ص㴦����������Ӻ�����һ���Ƕ�д���ӣ�һ������������μ����桰���Ӻ���ģ�塱��
 *  ���ù��ӵ�Ŀ����Ҫ����Ӧ�ò���
 */
#ifndef __FND_CONNECT_H__
#define __FND_CONNECT_H__

#include "basedef.h"
#include "channel.h"

#define NET_ETH0_DEV_NAME		"eth0"
#define NET_PPP0_DEV_NAME		"ppp0"

/** @brief ��д���Ӻ������Ͷ��� */
typedef void (*PFN_RWHOOK)(S_CHANNEL *pChannel, const string &strData);
/** @brief ��������������Ͷ��� */
typedef void (*PFN_ERROUT)(bool fWarnLevel, const string &strErrorInfo);

/** @class
 *  @brief ���ӳ��ඨ��
 *  @remarks ��ע�⹳�Ӻ�����ʵ��
 */
class C_CONNECT
{
public:
	C_CONNECT(const S_CHANNEL &channel);
	virtual ~C_CONNECT();

	/** @brief ����
	 *  @return 0 �ɹ�
	 *  @return -1 ʧ��
	 *  @return >0 �����´ο�ʼ���ӵ�����
	 */
	int Connect(void);
	/** @brief �Ͽ�����
	 *  @return true �ɹ�
	 *  @return false ʧ��
	 */
	S_BOOL Disconnect(void);
	/** @brief ����״̬
	 *  @return true ����������
	 *  @return false δ����
	 */
	S_BOOL IsConnected(void);
	/** @brief ��������
	 *  @param[in] strData ���ݻ���
	 *  @return -1 ���ͳ���
	 *  @return >=0 ���͵��ֽ���
	 */
	int SendData(const string &strData);
	/** @brief ��������
	 *  @param[out] strData �����������
	 *  @return -1 ���ճ���
	 *  @return >=0 ���յ��ֽ���
	 */
	int RecvData(string &strData);
	/** @brief ���ö�ȡ���Ӻ��� */
	void SetReadHook(PFN_RWHOOK pfnReadHook){m_pfnReadHook = pfnReadHook;}
	/** @brief ����д�빳�Ӻ��� */
	void SetWritHook(PFN_RWHOOK pfnWritHook){m_pfnWritHook = pfnWritHook;}
	/** @brief ���ô���������Ӻ��� */
	void SetErrorHook(PFN_ERROUT pfnErrorOut){m_pfnErrorOut = pfnErrorOut;}

	/** @brief ������������
	 *  @param[in] Channel ͨ����Ϣ
	 *  @return true �ɹ���false ʧ��
	 *  @remarks ����ʧ��ʱ�п��ܻᵼ�������ж�
	 */
	bool ChangeProperty(const S_CHANNEL &Channel);

	/** @brief ����ͨ����Ϣ
	 *  @param[in] Channel ͨ����Ϣ
	 *  @return true �ɹ���false ʧ��
	 */
	bool SetChannelInfo(const S_CHANNEL &Channel);
	/** @brief ȡͨ����Ϣ
	 *  @param[in] fVirtual true ����ͨ��;false ����ͨ��
	 *  @return ͨ����Ϣ
	 */
	virtual S_CHANNEL GetChannelInfo(bool fVirtual){return m_Channel;}

	/** @brief ������뻺�� */
	virtual S_BOOL ClearInBuffer(void){return true;}
	/** @brief ���������� */
	virtual S_BOOL ClearOutBuffer(void){return true;}

	/** @ȡ�������Թ�����
	 *  @param[in] pPropertyName ��������Ŀǰ֧��ppp0, eth0
	 *  @param[out] pBuffer �������
	 *  @param[in] size ��������С
	 *  @return -1 ����;0�ɹ�;>0 ��Ҫ�Ļ����С
	 *  @remarks ppp0��eth0�Ļ�����S_CHANNEL
	 */
	virtual int GetProperty(const char *pPropertyName, void *pBuffer, S_WORD size){return -1;}

protected:
	/** @brief �������� */
	virtual S_BOOL OnConnect(void) = 0;
	/** @brief �Ͽ����� */
	virtual S_BOOL OnDisconnect(void) = 0;
	/** @brief ���ӳɹ��ж� */
	virtual S_BOOL ConnectSuccessful(void) = 0;
	/** @brief �Ķ����� */
	virtual int ReadData(S_BYTE *pData, int size) = 0;
	/** @brief д������ */
	virtual int WriteData(const S_BYTE *pData, int size) = 0;
	/** @brief д�����ӳ� */
	virtual int WriteDataDelay(int SendByte){return 0;}

	/** @brief �������
	 *  @param[in] fWarnLevel [true|false]�Ƿ񰴸澯�������
	 *  @param[in] strErrorInfo ���������Ϣ
	 *  @return >=0 ������ֽ���
	 */
	int ErrorOut(bool fWarnLevel, const string &strErrorInfo);

protected:
	S_TIME m_timeLastConnect; ///< ���һ������ʱ��
	S_CHANNEL m_Channel; ///< ����ͨ��
	PFN_RWHOOK m_pfnReadHook; ///< ���Ӷ��Ĺ��Ӻ���
	PFN_RWHOOK m_pfnWritHook; ///< ����д�Ĺ��Ӻ���
	PFN_ERROUT m_pfnErrorOut; ///< �����������
};


/** ���Ӻ���ģ��
void PFN_ReadHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" send:"<<HEX<<strData<<endl;
}
void PFN_WritHOOK(S_CHANNEL *pChannel, const string &strData)
{
	if (!strData.empty())
		logvt<<pChannel->m_Name<<" recv:"<<HEX<<strData<<endl;
}
void PFN_ErrorOut(bool fWarnLevel, const string &strErrorInfo)
{
	if (!strErrorInfo.empty())
	{
		if (fWarnLevel == true) logwt<<strErrorInof<<endl;
		else logit<<strErrorInof<<endl;
	}
}
*/

#endif //__FND_CONNECT_H__


