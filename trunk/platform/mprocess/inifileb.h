/** @file
 *  @brief �����ļ������ඨ��
 *  @author Steven Wan
 *  @date 2015/03/15
 *  @version 0.1
 *  1���ṩ�û������ļ�������ӿ�
 *  @see foundation/confuse
 */
#ifndef __MP_INI_FILEB_H__
#define __MP_INI_FILEB_H__
#include "confuse.h"
#include "basedef.h"

/** @brief �������Ӧ�� */
struct S_CFGITEM
{
	int m_val;
	const char *m_pkey;
};

/** @class
 *  @brief ��ʼ���ļ�����������
 *  @see foundation/confuse/confuse.h
 *  @remarks INI�ļ��������ô�����н���
 */
class C_IniBase
{
public:
	/** @fn C_IniBase::ParseSetFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 *  @note ����˳��:�����û��ļ�����ȱʡ�ļ�
	 */
	static int ParseSetFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseSystemFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseSystemFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::ParseRegisterFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] fname:�ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int ParseRegisterFile(cfg_t *cfg, const char *fname);
	/** @fn C_IniBase::SaveSetFile
	 *  @brief ���������ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSetFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveSystemFile
	 *  @brief ����ϵͳ�ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveSystemFile(const string &strBuffer, const char *fname);
	/** @fn C_IniBase::SaveRegisterFile
	 *  @brief ����ϵͳ�ļ�
	 *  @param[in] strBuffer ������Ϣ��
	 *  @param[in] fname �ļ���
	 *  @return 0:successful, -1:failed
	 */
	static int SaveRegisterFile(const string &strBuffer, const char *fname);

	/** @fn C_IniBase::MakeValue
	 *  @brief ȡ������
	 *  @param[in] pkey ָ������
	 *  @param[in] pkeys ������������ָ��
	 *  @param[in] size �����С
	 *  @return >=0 ��Ч���� -1 ��Ч����
	 */
	static int MakeValue(const char *pkey, const char **pkeys, int size);
	/** @fn C_IniBase::MakeKey
	 *  @brief ȡָ������
	 *  @param[in] idx ������
	 *  @param[in] pkeys ������������ָ��
	 *  @param[in] size �����С
	 *  @return NULL ��Чָ�룻!NULL ������ָ��
	 */
	static const char *MakeKey(int idx, const char **pkeys, int size);
	/** @fn C_IniBase::GetBoolKey
	 *  @brief ȡbool����true/false
	 *  @param[in] fbool true/false
	 *  @return ָ��"true|false"��ָ��
	 */
	static const char *GetBoolKey(bool fbool);

	/** @fn C_IniBase::MakeKey
	 *  @brief ȡָ������
	 *  @param[in] val ָ��ֵ
	 *  @param[in] pCfgItem ������������ָ��
	 *  @param[in] size �����С
	 *  @return "unknow"��Key����//NULL ��Чָ�룻!NULL ������ָ��
	 */
	static const char *MakeKey(const S_CFGITEM *pCfgItem, S_WORD size, int val);
	/** @fn C_IniBase::MakeValue
	 *  @brief ȡ������
	 *  @param[in] pkey ָ������
	 *  @param[in] pCfgItem ������������ָ��
	 *  @param[in] size �����С
	 *  @return >=0 ��Чֵ -1 ��Чֵ
	 */
	static int MakeValue(const S_CFGITEM *pCfgItem, S_WORD size, const char *pkey);

private:
	/** @fn C_IniBase::ParseCfgFile
	 *  @brief �����ļ�����
	 *  @param[out] cfg:���ýṹ��
	 *  @param[in] cfgFile:�ļ���(��·��)
	 *  @return 0:successful, -1:failed
	 */
	static int ParseCfgFile(cfg_t* cfg, const char *cfgFile);
	/** @fn C_IniBase::SaveCfgFile
	 *  @brief ���������ļ�
	 *  @param[in] strBuffer:������Ϣ��
	 *  @param[in] cfgFile:�ļ���(��·��)
	 *  @return 0:successful, -1:failed
	 */
	static int SaveCfgFile(const string &strBuffer, const char *cfgFile);
};


#endif //__MP_INI_FILEB_H__
