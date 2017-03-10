/**
 *  @file
 *  @brief ��ƹ�Լ������
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
	MRULE_PARSE_ERR_NO = 0, //�ɹ�
	MRULE_PARSE_ERR_ADDR = -1, //��ַ��
	MRULE_PARSE_ERR_DI = -2, //�������
	MRULE_PARSE_ERR_CTRL = -3, //�������
	MRULE_PARSE_ERR_REG = -4, //�Ĵ�����
	
	MRULE_PARSE_ERR_OTHER = -9 //������
};

/** @brief ��ƼĴ�����ͨ�üĴ���ӳ�� */
struct S_RegisterMap
{
	S_DWORD m_CommID;///< ͨ�üĴ���ID
	S_DWORD m_LocalID; ///< ��ƼĴ���ID

public:
	S_RegisterMap(void){memset(this, 0x00, sizeof(*this));}
};
struct S_RegisterMaps:public vector<S_RegisterMap>
{
};
bool operator<(const S_RegisterMap &left, const S_RegisterMap &right);

/** @brief ��ƹ�Լ����ӿڶ��� */
class C_MRULE
{
public:
	C_MRULE(void);
	virtual ~C_MRULE();

public:
	S_COMSET GetComSet(void);
	/** @brief ��ȡ��ƹ�Լ���� */
	virtual char *GetName(void);
	/** @brief �����Լ����*/
	virtual bool ClearBuffer(void);

	/** @brief ��ȡ����֡
	 *  @param[in] strBuffer �����
	 *  @param[out] strCmd ֡�������
	 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	 */
	virtual int ExtractCmd(const string &strBuffer, string &strCmd);

	/** @brief �������֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] DataGram ֡�������
	 *  @return <0: ������޴�Ҫ��֡; =0: ������֡��ֱ�ӷ�������; >0: ��֡�ɹ�
	 */
	virtual int MakeReadCmd(const string &strAddress, const S_REGISTER &CommReg, S_DATAGRAM &DataGram);
	/** @brief �����������֡
	 *  @param[in] strCmd ��������֡,��Ϊ�����������
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];=0:�ɹ�
	 */
	virtual int ParseReadCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief ��д����֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] strData �������뻺��
	 *  @param[out] DataGram ֡�������
	 *  @return <0: ������޴�Ҫ��֡; =0: ������֡��ֱ�ӷ�������; >0: ��֡�ɹ�
	 */
	virtual int MakeWriteCmd(const string &strAddress, const S_REGISTER &CommReg, const string &strData, S_DATAGRAM &DataGram);
	/** @brief ����д�����֡
	 *  @param[in] strCmd ��������֡
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[out] strData �����������
	 *  @return <0:����[�μ�MRULE_PARSE_ERR_];=0:�ɹ�
	 */
	virtual int ParseWriteCmd(const string &strCmd, const S_REGISTER &CommReg, string &strData);

	/** @brief ��㲥Уʱ֡
	 *  @param[in] strAddress ��Ƶ�ַ
	 *  @param[in] CommReg ͨ�üĴ���
	 *  @param[in] timeExpected ����ʱ��
	 *  @param[out] DataGram ֡�������
	 *  @return <0: ������޴�Ҫ��֡; =0: ������֡��ֱ�ӷ�������; >0: ��֡�ɹ�
	 */
	virtual int MakeBroadcastCmd(const string &strAddress, const S_REGISTER &CommReg, S_TIME timeExpected, S_DATAGRAM &DataGram);

protected:
	S_COMSET m_ComSet;
};

#endif //__BASEB_MRULE_H__

