#include "protocol.h"
#include <algorithm>
#include <numeric>


#define FRAME_CTL_POS	2
#define FRAME_LEN_POS 3
#define FRAME_DI_POS 5
#define FRAME_DATA_POS		7




/*
brief: 从接受缓冲区提取完整帧命令
param [in]string& strRecvMsBuffer:串口接收缓冲区数据
param [out]string& strCmd:从缓冲区解析出的一完整帧
return S_WORD:-1:帧命令提取失败，0:帧命令提取成功
author:Steven Wan
date: 2015-04-23
*/ 
S_SHORT C_Protocol::ExtractCmd(string& strRecvMsBuffer, string& strCmd)
{
	string oStrRecvMsBuffer;
	if (ExtractFrame(strRecvMsBuffer,oStrRecvMsBuffer,strCmd) == false)
	{
		strRecvMsBuffer.clear();
		return -1;
	}
	strRecvMsBuffer = oStrRecvMsBuffer;
	return 0;
}

/*
brief: 规约解析帧
param [in]const string& m_iStrRecvMsBuffer:串口缓冲区数据
param [in]string& m_oStrRecvMsBuffer:已解析提取完一帧后的串口缓冲区数据
param [out]string& oStrData:从缓冲区解析出的一完整帧
return bool:false:帧提取失败，true:帧提取成功
author:Steven Wan
date: 2015-04-23
*/ 
bool C_Protocol::ExtractFrame(const string& iStrRecvMsBuffer,string& oStrRecvMsBuffer,string& strCmd)
{
	if (iStrRecvMsBuffer.empty())
	{
		return false;
	}
	string::const_iterator iter = find(iStrRecvMsBuffer.begin(), iStrRecvMsBuffer.end(), (char)HEAD_GUIDE_FLAG_FIRST);
	
	if (distance(iter, iStrRecvMsBuffer.end()) < FRAME_LEN_MIN)
	{
		return false;
	}

	S_BYTE Start1 = (S_BYTE)*iter; 
	S_BYTE Start2 = (S_BYTE)*(iter+1);

	if (Start1 != HEAD_GUIDE_FLAG_FIRST || Start2 != HEAD_GUIDE_FLAG_SECOND)
	{
		loget<<"前导符出错"<<endl;
		return false;
	}

	//S_WORD len = (S_WORD)(*(iter + 3) |(*(iter + 4)<<8));
	S_WORD len = (S_WORD)((S_BYTE)(*(iter + 3)) + (S_BYTE)(*(iter + 4))*256);
	string::const_iterator end_iter = iter + 6 + len; 
	if (end_iter > iStrRecvMsBuffer.end())
	{
		loget<<len<<endl;
		loget<<(unsigned int)((S_BYTE)(*(iter+3))|(S_BYTE)(*(iter + 4)<<8))<<endl;
		loget<<"输入缓冲区数据不足"<<endl;
		return false;
	}

	if ((S_BYTE)(*(iter + 9)) == (S_BYTE)0x28)
	{
		loget<<"倒数第二帧"<<endl;
		;
	}
	
	if ( (S_BYTE)(*(end_iter)) != (S_BYTE)END_FLAG)
	{
		loget<<"无帧结束符"<<endl;
		oStrRecvMsBuffer.clear();//可能帧格式错误没有结束符，所以要丢掉这帧
		return false;
	}

	char checkSum = accumulate(iter, end_iter-1,0);

	if (checkSum!=(*(end_iter-1)))
	{
		loget<<"校验错误"<<endl;
		return false;
	}
	
	strCmd.assign(iter,end_iter+1);
	
	oStrRecvMsBuffer.assign(end_iter+1,iStrRecvMsBuffer.end());

	return true;

}

/*
brief: 根据DI数据标示读取其数据帧
param [in]S_WORD di:数据标识
param [out]string& oStrData:返回读取帧
return 0:读取成功，-1:读取失败
author:Steven Wan
date: 2015-04-24
*/  
S_WORD C_Protocol::ReadCmd(S_WORD di, string& ostrCmd)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	S_CHAR buf[CONS_REGITEM_DATABUF_MAXLEN]={0};
	
	ostrCmd.clear();

	pRegItem = pRegItemsArea->GetDataItemByRegid(di);
    
	if (pRegItem != NULL)
	{//数据项存在的处理
		int len = pRegItem->ReadData(buf);
		MakeFrame((S_CHAR)CTL_READ_ACK, di,buf, len, ostrCmd);
	}
	else
	{//数据项不存在的处理
		loget<<"数据项不存在"<<endl;
		MakeFrame((S_CHAR)CTL_READ_ERR_ACK,(S_CHAR)ERR_READ_INVALID, ostrCmd);
	}
	return 0;		
	
}
S_WORD C_Protocol::BatchReadCmd(S_WORD di, string& ostrCmd)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	string batchRdStrData;

	ostrCmd.clear();
	pRegItem = pRegItemsArea->GetDataItemByRegid(di);
	if (pRegItem != NULL)
	{//数据项存在的处理
		pRegItem->BatchReadData(batchRdStrData);
		MakeFrame((S_CHAR)CTL_BATCH_READ_ACK, batchRdStrData, ostrCmd);
	}
	return 0;
}
/*
brief: 数据处理接口，对上行通道接收的数据进行处理
param [in]C_DspDevice *pDspDevice:Dsp模块操作句柄
param [in]string& iStrData:接收数据
param [out]string& oStrData:处理返回数据
return 0:已处理，-1:未处理
author:Steven Wan
date: 2015-04-23
*/  
S_SHORT C_Protocol::Handle(C_DspDevice *pDspDevice,string& iStrData, string& oStrData)
{
	string strCmd;
	S_FrameStu frameStu;
	S_WORD ret;

	ret = ExtractCmd(iStrData,strCmd);
	if (ret == 0)
	{//提取完整一帧命令
		ParseFrame(strCmd, frameStu);
		if (frameStu.m_ctlcode == (S_CHAR)CTL_WRITE)
		{
			if (0x0c01 == frameStu.m_di)
			{
				//保存帧数据
				if (SaveFrame(frameStu) == true)
				{
					MakeFrame((S_CHAR)CTL_WRITE_ACK,oStrData);
				}
				else
				{
					MakeFrame((S_CHAR)CTL_WRITE_ERR_ACK,(S_CHAR)ERR_WRITE__INVALID,oStrData);
				}
				
			}
			else if (pDspDevice->Write(strCmd,oStrData) == true)//对DSP设备写命令
			{
				logvt<<"Dsp命令写成功"<<endl;
				S_FrameStu tmpFrameStu;
				ParseFrame(oStrData, tmpFrameStu);	

				if (tmpFrameStu.m_ctlcode == (S_CHAR)CTL_WRITE_ACK)//参数写DSP成功，更新到文件系统
				{
					//保存帧数据
					SaveFrame(frameStu);
				}
			}
			else
			{
				MakeFrame((S_CHAR)CTL_WRITE_ERR_ACK,(S_CHAR)ERR_WRITE__INVALID,oStrData);
			}
		}
		else if (frameStu.m_ctlcode == (S_CHAR)CTL_MULT_WRITE)
		{
			string tempStc; // 拆分开的数据帧
			string tempReplyBegin; // 回复帧开头
			string tempReplyData; // 回复帧数据
			tempReplyData.clear();
			string tempReplyEnd; // 回复帧结尾

			// 将批量帧拆分，然后将每个完整数据写入dsp，如果成功，则存入文件系统
			loget<<frameStu.m_diLen<<endl;//////////////////////
			for (int i = 0; i < frameStu.m_diLen;i = i + 4 + frameStu.m_strdata[i + 2] + frameStu.m_strdata[i + 3]*256)
			{
				tempReplyData.push_back(frameStu.m_strdata[i]);
				tempReplyData.push_back(frameStu.m_strdata[i + 1]);

				tempStc.clear();
				for (int j = 0;j < frameStu.m_strdata[i + 2] + frameStu.m_strdata[i + 3]*256;j++)
				{
					tempStc[j] = frameStu.m_strdata[i + 4 + j];
				}
				if (pDspDevice->Write(tempStc,oStrData) == true)
				{
					//保存帧数据
					//SaveFrame(frameStu);
					S_WORD regid;
					regid = (S_WORD)((frameStu.m_strdata[i]) | (frameStu.m_strdata[i + 1]*256));
					S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
					S_RegItem* pRegItem;
					pRegItem = pRegItemsArea->GetDataItemByRegid(regid);

					

					if (pRegItem != NULL)
					{
						pRegItem->SaveData(tempStc.data(), frameStu.m_strdata[i + 2] + frameStu.m_strdata[i + 3]*256);
					}
					else
					{
						loget<<"SaveFrame:数据项"<<HEX<<regid<<"不存在"<<endl;
						return -1;
					}
					tempReplyData.push_back(0x00);
				}
				else
				{
					tempReplyData.push_back(0x05);
				}
			}
			
			// 组织回复帧
			S_BYTE length = 0;
			tempReplyBegin.clear();
			tempReplyBegin.push_back((char)0xEB);
			tempReplyBegin.push_back((char)0x90);
			tempReplyBegin.push_back((char)0x85);
			// 计算回复帧中的长度值
			for (int i = 0; i < frameStu.m_diLen;i = i + 4 + frameStu.m_strdata[i + 2] + frameStu.m_strdata[i + 3]*256)
			{
				length += 1;
			}
			length *= 3;
			tempReplyBegin.push_back(length);

			S_BYTE cs = 0;
			for (int i = 0;i < length;i++)
			{
				cs += tempReplyData[i];
			}
			cs %= 256;
			tempReplyEnd.clear();
			tempReplyEnd.push_back(cs); //cs
			tempReplyEnd.push_back((char)0x16);
			
			oStrData = tempReplyBegin + tempReplyData + tempReplyEnd;
		}
		else if (frameStu.m_ctlcode == (S_CHAR)CTL_READ)//读参数
		{
			ReadCmd(frameStu.m_di,oStrData);	
		}
		else if (frameStu.m_ctlcode == (S_CHAR)CTL_BATCH_READ)//读参数
		{
			BatchReadCmd(frameStu.m_di,oStrData);	
		}
		else
		{
			return -1;
		}
		return 0;
	}
	else
	{
		return -1;
	}
	
}

/*
brief: 规约组帧
param [in]S_CHAR ctlcode:控制码
param [in]S_WORD di:数据标识
param [in]S_CHAR *pData:数据区指针
param [out]S_WORD len:数据区内容长度
param [out]string& oFrame:组帧命令
return 0:组帧成功，-1:组帧失败
author:Steven Wan
date: 2015-04-23
*/ 
S_WORD C_Protocol::MakeFrame(S_CHAR ctlcode,S_WORD di,S_CHAR *pData,S_WORD len,string& oFrame)
{
	string oPackage;
	
	oPackage.clear();
	oPackage.push_back((char)0xEB);
	oPackage.push_back((char)0x90);

	oPackage.push_back(ctlcode);

	oPackage.push_back((char)(len&0xff));
	oPackage.push_back((char)((len>>8)&0xff));
	
	oPackage.push_back((char)(di&0xff));
	oPackage.push_back((char)((di>>8)&0xff));

	for (S_WORD i=0; i<len; i++)
	{
		oPackage.push_back(pData[i]);
	}

	INT8U checkSum=0;
	for (S_WORD i=0;i<oPackage.size();i++)
	{
		checkSum+=oPackage[i];
	}
	
	oPackage.push_back(checkSum);
	oPackage.push_back((char)0x16);
	oFrame = oPackage;
	return 0;
}

/*
brief: 异常组帧
param [in]S_CHAR ctlcode:控制码
param [in]S_CHAR cErrCode:错误码
param [out]string& oFrame:组帧命令
return 0:组帧成功，-1:组帧失败
author:Steven Wan
date: 2015-04-23
*/ 
S_WORD C_Protocol::MakeFrame(S_CHAR ctlcode,S_CHAR cErrCode,string& oFrame)
{
	string oPackage;
	
	oPackage.clear();
	oPackage.push_back((S_CHAR)0xEB);
	oPackage.push_back((S_CHAR)0x90);

	oPackage.push_back(ctlcode);

	oPackage.push_back((S_CHAR)0x01);
	oPackage.push_back((S_CHAR)0x00);
	oPackage.push_back(cErrCode);
	
	INT8U checkSum=0;
	for (S_WORD i=0;i<oPackage.size();i++)
	{
		checkSum+=oPackage[i];
	}
	
	oPackage.push_back(checkSum);
	oPackage.push_back((S_CHAR)0x16);
	oFrame = oPackage;
	return 0;
}

/*
brief: 正常写回复组帧
param [in]S_CHAR ctlcode:控制码
param [out]string& oFrame:组帧命令
return 0:组帧成功，-1:组帧失败
author:Steven Wan
date: 2015-10-21
*/ 
S_WORD C_Protocol::MakeFrame(S_CHAR ctlcode,string& oFrame)
{
	string oPackage;
	
	oPackage.clear();
	oPackage.push_back((S_CHAR)0xEB);
	oPackage.push_back((S_CHAR)0x90);

	oPackage.push_back(ctlcode);

	oPackage.push_back((S_CHAR)0x00);
	oPackage.push_back((S_CHAR)0x00);
	
	INT8U checkSum=0;
	for (S_WORD i=0;i<oPackage.size();i++)
	{
		checkSum+=oPackage[i];
	}
	
	oPackage.push_back(checkSum);
	oPackage.push_back((S_CHAR)0x16);
	oFrame = oPackage;
	return 0;
}

/*
brief: 批量读组帧
param [in]S_CHAR ctlcode:控制码
param [in]string& :批量读取的数据项数据
param [out]string& oFrame:组帧命令
return 0:组帧成功，-1:组帧失败
author:Steven Wan
date: 2015-04-23
*/ 
S_WORD C_Protocol::MakeFrame(S_CHAR ctlcode,string& iStrData,string& oFrame)
{
	string oPackage;
	S_WORD datalen = (S_WORD)(iStrData.size());
	
	oPackage.clear();
	oPackage.push_back((S_CHAR)0xEB);
	oPackage.push_back((S_CHAR)0x90);

	oPackage.push_back(ctlcode);

	oPackage.push_back((S_CHAR)(datalen&0xff));
	oPackage.push_back((S_CHAR)((datalen>>8)&0xff));
	oPackage.append(iStrData);

	INT8U checkSum=0;
	for (S_WORD i=0;i<oPackage.size();i++)
	{
		checkSum+=oPackage[i];
	}
	
	oPackage.push_back(checkSum);
	oPackage.push_back((S_CHAR)0x16);
	oFrame = oPackage;
	return 0;
}

/*
brief: 把帧数据串，反串行化成帧结构
param [in]const string& strCmd:完整帧数据串
param [out]S_FrameStu&  frameStu:帧结构
author:Steven Wan
date: 2015-04-23
*/ 

void C_Protocol::ParseFrame(const string& strCmd,S_FrameStu&  frameStu)

{
	frameStu.m_strdata.clear();
	frameStu.m_ctlcode = strCmd[FRAME_CTL_POS];

	switch (frameStu.m_ctlcode)
	{
		case (S_CHAR)CTL_WRITE_ACK:
			frameStu.m_diLen =  strCmd[FRAME_LEN_POS] + strCmd[FRAME_LEN_POS+1]*256;
			break;
		case (S_CHAR)CTL_MULT_WRITE:
			frameStu.m_diLen =  ((S_WORD)strCmd[FRAME_LEN_POS])&0X00FF + ((((S_WORD)strCmd[FRAME_LEN_POS+1])&0x00ff)*256);//此处修改
			//frameStu.m_diLen =  strCmd[FRAME_LEN_POS] + strCmd[FRAME_LEN_POS+1]*256 - 2;
			if (frameStu.m_diLen > 0)
			{
				frameStu.m_strdata.assign(strCmd.begin()+FRAME_DI_POS,strCmd.begin()+ FRAME_DI_POS + frameStu.m_diLen);
					loget<<HEX<<frameStu.m_diLen<<endl;
			}
			break;

		default:
			frameStu.m_di = (S_WORD)((strCmd[FRAME_DI_POS])|(strCmd[FRAME_DI_POS+1]<<8));
			frameStu.m_diLen =  (S_WORD)((S_BYTE)strCmd[FRAME_LEN_POS] |((S_BYTE)strCmd[FRAME_LEN_POS+1]<<8));
			frameStu.m_diLen -= 2;
			if (frameStu.m_diLen > 0)
			{
				frameStu.m_strdata.assign(strCmd.begin()+FRAME_DATA_POS,strCmd.begin()+FRAME_DATA_POS+frameStu.m_diLen);
			}
			break;
	}
}

/*
brief: 把帧里数据项的内容保存至文件系统
param [in]const S_FrameStu&  frameStu:待保存的帧结构
param [out]bool:true:保存成功，保存失败
author:Steven Wan
date: 2015-04-23
*/ 

bool C_Protocol::SaveFrame(const S_FrameStu&  frameStu)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//获取数据项寄存器组指针
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemByRegid(frameStu.m_di);

	if (pRegItem != NULL)
	{
		logvt<<"数据项"<<HEX<<frameStu.m_di<<"数据内容:"<<HEX<<frameStu.m_strdata<<endl;
		return pRegItem->SaveData(frameStu.m_strdata.data(), frameStu.m_diLen);
	}
	else
	{
		loget<<"SaveFrame:数据项不存在"<<endl;
		return false;
	}
}

