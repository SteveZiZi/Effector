/** @file
 *  @brief �����㷨����
 *  @author Steven Wan
 *  @date 2015/02/14
 *  @version 0.1
 */
#ifndef __FND_BASEALGO_H__
#define __FND_BASEALGO_H__
#include "basedef.h"

/** @brief BCD���㷨
 *  @note
 *  1�����֧��8λ������4λС��
 *  2������BCD�����У�BCD�밴С�˴��
 */
class C_BCD
{
public:
	C_BCD(void);
	C_BCD(S_DWORD Integer, S_WORD Float = 0);
	C_BCD(const string &strBcdAscii);
	C_BCD(const S_BYTE *pbcdInteger4B, const S_BYTE *pbcdFloat2B);
	C_BCD(const S_BYTE *pbcdBuffer, S_BYTE BitSizeInteger, S_BYTE BitSizeFloat);
	C_BCD(const C_BCD &r);
	~C_BCD();

	/** @brief ȡBCD����
	 *  @return ���ظ�ʽΪǰ4λС����8λ������С�˷�ʽ
	 */
	const S_BYTE *GetBcdData(void) const;
	/** @brief ȡBCD����
	 *  @param[out] Integer 16��������
	 *  @param[out] Float 16����С��
	 *  @return void
	 */
	void GetBcdData(S_DWORD &Integer, S_WORD &Float) const;

	C_BCD &operator=(const C_BCD &r);
	C_BCD &operator+=(const C_BCD &r);
	C_BCD &operator+=(S_DWORD val);
	C_BCD &operator-=(const C_BCD &r);
	C_BCD &operator-=(S_DWORD val);
	C_BCD &operator*=(const C_BCD &r);
	C_BCD &operator*=(S_DWORD val);

	friend bool operator>(const C_BCD &left, const C_BCD &right);
	friend bool operator>=(const C_BCD &left, const C_BCD &right);
	friend bool operator<(const C_BCD &left, const C_BCD &right);
	friend bool operator<=(const C_BCD &left, const C_BCD &right);
	friend bool operator==(const C_BCD &left, const C_BCD &right);
	friend bool operator!=(const C_BCD &left, const C_BCD &right);

	operator double()const;
	operator string()const;

private:
	bool ValidBcd(const S_BYTE *pbcdBuffer, S_WORD ByteSize);
	bool ValidBcd(const string &strBcdAscii);
	S_DWORD Convert(S_DWORD dwBCD);
	S_DWORD Convert(const string &strBcdAscii);
	void Correct(S_DWORD Integer, S_DWORD Float);

private:
	static const S_DWORD POWER[8];// = {1,10,100,1000,10000,100000,1000000,10000000};
	S_DWORD m_Integer;
	S_WORD m_Float;
	mutable S_BYTE m_bcdBuffer[6];
};

//nonmember
C_BCD operator+(const C_BCD &left, const C_BCD &right);
C_BCD operator+(const C_BCD &left, S_DWORD val);
C_BCD operator-(const C_BCD &left, const C_BCD &right);
C_BCD operator-(const C_BCD &left, S_DWORD val);
C_BCD operator*(const C_BCD &left, const C_BCD &right);
C_BCD operator*(const C_BCD &left, S_DWORD val);

bool operator>(const C_BCD &left, const C_BCD &right);
bool operator>=(const C_BCD &left, const C_BCD &right);
bool operator<(const C_BCD &left, const C_BCD &right);
bool operator<=(const C_BCD &left, const C_BCD &right);
bool operator==(const C_BCD &left, const C_BCD &right);
bool operator!=(const C_BCD &left, const C_BCD &right);


//
//
//
class C_BcdString:public string
{
public:
	C_BcdString(const string &strBcd);

	C_BcdString &operator<<(S_WORD BcdBit);
	C_BcdString &operator>>(S_WORD BcdBit);
};

#endif //__FND_BASEALGO_H__


