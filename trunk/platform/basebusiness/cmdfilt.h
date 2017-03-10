/**
 *  @file
 *  @brief �ֳ��������������
 *  @author Steven Wan
 *  @date 2014-05-10
 *  @version v1.0
 *  @note �ֳ����߽������ݺ����
 */
#ifndef __BASEB_CMD_FILTER_H__
#define __BASEB_CMD_FILTER_H__
#include "basedef.h"

class C_CmdFilter
{
public:
	/** @brief ������� */
	C_CmdFilter(const string &strFilterName);
	~C_CmdFilter();

public:
	/** @brief �������
	 *  @param[in] strBuffer �����
	 *  @param[out] strCmd ֡�������
	 *  @return <0: ����[����ֵ�ǳ�����ֽ���]; =0: ��֡; >0: �ɹ�
	 */
	int operator()(const string &strBuffer, string &strCmd)const;

private:
	string m_strFilterName; //�������������
};

#endif //__BASEB_CMD_FILTER_H__

