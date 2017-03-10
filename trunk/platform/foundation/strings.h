/** @file
 *  @brief ������strings����
 *  @author Steven Wan
 *  @date 2015-01/07
 *  @version 0.1
 */
#ifndef __FND_STRINGS_H__
#define __FND_STRINGS_H__
#include "basedef.h"

/** @class
 *  @brief string������
 */
class strings:public vector<string>
{
public:
	/** @brief ��ָ������������ַ���
	 *  @param[in] strSrc Դ�ַ���
	 *  @param[in] delimit �����
	 *  @return �ֿ����ִ�����
	 */
	int split(const string &strSrc, char delimit);

	/** @brief ׷��һ���ִ�����
	 *  @param[in] strsAppend:��׷�ӵ��ִ�����
	 *  @return strings&
	 */
	strings & append(const strings& strsAppend)
	{		
		insert(end(), strsAppend.begin(), strsAppend.end());
		return *this;
	}
	/** @brief ���Դ�
	 *  @return true����һ�����ǿ�;false���д�Ϊ��
	 */
	bool Any(void)const;
	/** @brief ���Դ�
	 *  @return true����һ������;false���д�Ϊ�ǿ�
	 */
	bool AnyEmpty(void)const;
	/** @brief ͳ�ƴ��������ж��ٸ��ǿմ�
	 *  @return �ǿմ�ͳ��ֵ
	 */
	S_WORD Count(void)const;
	/** @brief ���ݴ��л�
	 *  @return ���л��󳤶�
	 */
	S_WORD Serialize(string &strData)const;
};


#endif//__FND_FNDVERSION_H__

