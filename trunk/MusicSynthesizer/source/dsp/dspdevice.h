#ifndef _DSP_DEVICE_H_
#define _DSP_DEVICE_H_

#include "connect.h"
#include "sysbase.h"
#include "status.h"
#include "connfact.h"

#define HEAD_GUIDE_FLAG_FIRST		0xEB
#define HEAD_GUIDE_FLAG_SECOND	0x90
#define END_FLAG	0x16
#define FRAME_LEN_MIN	7

class C_DspCmdChannel
{
public:
	static void Init(void);
	static int SendData(const string& iStrData);
	static int RecvData(string& iStrData);
private:
	static S_CHANNEL *m_pChannel;
	static C_CONNECT *m_pConnect; 
};

typedef bool (*FP_ServerHb)(void);
class C_DspDevice
{
public:
	C_DspDevice()
	{
		C_DspCmdChannel::Init();
	}
	~C_DspDevice(){}
	
	bool StartUp(void);
	bool Write(const string& iStrData,string& oStrData);
	bool Write(S_WORD di,S_CHAR *pData,S_WORD len);
	void SetHeartBeatHandler(FP_ServerHb fp_ServerHb){ Func_HeartBeat = fp_ServerHb;}
private:
	bool FramePrase(const string& iStrData);
	S_WORD MakeFrame(S_CHAR ctlcode,S_WORD di,S_CHAR *pData,S_WORD len,string& oFrame);
	void InitialParam(void);
private:
	FP_ServerHb Func_HeartBeat;
	
};

#endif

