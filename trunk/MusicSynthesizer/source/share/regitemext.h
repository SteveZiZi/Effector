#ifndef REGITEM_EXT_H_
#define REGITEM_EXT_H_


class C_RegitemExt_0b01
{
public:
	static bool SaveData(const S_CHAR *pBuf, S_WORD len);
	static S_WORD ReadData(S_CHAR *pBuf);
};

class C_RegitemExt_FFFF
{
public:
	static S_WORD ReadData(string &oStrData);
};

class C_RegitemExt_0101
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
	//static S_WORD ReadData(S_CHAR *pBuf);
};

class C_RegitemExt_0102
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0103
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0104
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0105
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0106
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0107
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0108
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0109
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010A
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010B
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010C
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010D
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010E
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_010F
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0201
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0202
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0203
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0301
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0401
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0601
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_1101
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_1003
{
public:
	static bool SaveData(const S_CHAR *pBuf,S_WORD len);
};

class C_RegitemExt_0C01
{
public:
	static bool SaveData(const S_CHAR * pBuf,S_WORD len);
	
private:
	static S_WORD frameNowNo;
	static unsigned short CRC16RTU(const unsigned char * pszBuf, unsigned int unLength);
};
#endif

