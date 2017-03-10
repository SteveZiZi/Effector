/************************************************************************
 * Copyright (c) XT Co., Ltd. 
 * All rights reserved. 
 *  
 * File Name：protocol.h
 * Function：通讯协议层数据处理模块
 
 *
 * Author：Steven Wan
 * Date：2015-04-21
 * Version：1.0         
 *
 * Reversion: 
 * Author: 
 * Date: 
**************************************************************************/
#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

/****************************************************************************/
/*                           Include file                                   								*/
/****************************************************************************/
#include "sysbase.h"
#include "dspdevice.h"

/****************************************************************************/
/*                               Define                                     								*/
/****************************************************************************/

/****************************************************************************/
/*                            Enum  Define                                 								*/
/****************************************************************************/ 
typedef enum
{
	ERR_READ_INVALID=1,
	ERR_WRITE__INVALID=2,	
	ERR_DI_NOT_EXIST=3,	
	ERR_CHECKSUM=4,
	ERR_FRAME_FORMAT=5,
	ERR_TYPE_MAX
}T_emErrCode;

/****************************************************************************/
/*                            Struct Define                                 								*/
/****************************************************************************/
/*基本帧结构*/
struct S_FrameStu
{
	S_CHAR m_ctlcode;//控制码
	S_WORD m_di;//数据标识
	string m_strdata;//数据内容
	S_WORD m_diLen;//数据长度
};


/*通讯协议类*/
class C_Protocol
{
public:
	C_Protocol(){}
	S_SHORT Handle(C_DspDevice *pDspDevice,string& iStrData, string& oStrData);
	
private:
	S_SHORT ExtractCmd(string& strRecvMsBuffer, string& strCmd);
	S_WORD ReadCmd(S_WORD di, string& ostrCmd);
	S_WORD BatchReadCmd(S_WORD di, string& ostrCmd);
	bool ExtractFrame(const string& iStrRecvMsBuffer,string& oStrRecvMsBuffer,string& strCmd);
	S_WORD MakeFrame(S_CHAR ctlcode,S_WORD di,S_CHAR *pData,S_WORD len,string& oFrame);
	S_WORD MakeFrame(S_CHAR ctlcode,S_CHAR cErrCode,string& oFrame);
	S_WORD MakeFrame(S_CHAR ctlcode,string& iStrData,string& oFrame);	
	S_WORD MakeFrame(S_CHAR ctlcode,string& oFrame);
	void ParseFrame(const string& strCmd,S_FrameStu&  frameStu);
	bool SaveFrame(const S_FrameStu&  frameStu);

};


#endif

