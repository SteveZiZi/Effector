#include "protocol.h"
#include <algorithm>
#include <numeric>


#define FRAME_CTL_POS	2
#define FRAME_LEN_POS 3
#define FRAME_DI_POS 5
#define FRAME_DATA_POS		7




/*
brief: �ӽ��ܻ�������ȡ����֡����
param [in]string& strRecvMsBuffer:���ڽ��ջ���������
param [out]string& strCmd:�ӻ�������������һ����֡
return S_WORD:-1:֡������ȡʧ�ܣ�0:֡������ȡ�ɹ�
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
brief: ��Լ����֡
param [in]const string& m_iStrRecvMsBuffer:���ڻ���������
param [in]string& m_oStrRecvMsBuffer:�ѽ�����ȡ��һ֡��Ĵ��ڻ���������
param [out]string& oStrData:�ӻ�������������һ����֡
return bool:false:֡��ȡʧ�ܣ�true:֡��ȡ�ɹ�
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
		loget<<"ǰ��������"<<endl;
		return false;
	}

	//S_WORD len = (S_WORD)(*(iter + 3) |(*(iter + 4)<<8));
	S_WORD len = (S_WORD)((S_BYTE)(*(iter + 3)) + (S_BYTE)(*(iter + 4))*256);
	string::const_iterator end_iter = iter + 6 + len; 
	if (end_iter > iStrRecvMsBuffer.end())
	{
		loget<<len<<endl;
		loget<<(unsigned int)((S_BYTE)(*(iter+3))|(S_BYTE)(*(iter + 4)<<8))<<endl;
		loget<<"���뻺�������ݲ���"<<endl;
		return false;
	}

	if ((S_BYTE)(*(iter + 9)) == (S_BYTE)0x28)
	{
		loget<<"�����ڶ�֡"<<endl;
		;
	}
	
	if ( (S_BYTE)(*(end_iter)) != (S_BYTE)END_FLAG)
	{
		loget<<"��֡������"<<endl;
		oStrRecvMsBuffer.clear();//����֡��ʽ����û�н�����������Ҫ������֡
		return false;
	}

	char checkSum = accumulate(iter, end_iter-1,0);

	if (checkSum!=(*(end_iter-1)))
	{
		loget<<"У�����"<<endl;
		return false;
	}
	
	strCmd.assign(iter,end_iter+1);
	
	oStrRecvMsBuffer.assign(end_iter+1,iStrRecvMsBuffer.end());

	return true;

}

/*
brief: ����DI���ݱ�ʾ��ȡ������֡
param [in]S_WORD di:���ݱ�ʶ
param [out]string& oStrData:���ض�ȡ֡
return 0:��ȡ�ɹ���-1:��ȡʧ��
author:Steven Wan
date: 2015-04-24
*/  
S_WORD C_Protocol::ReadCmd(S_WORD di, string& ostrCmd)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	S_CHAR buf[CONS_REGITEM_DATABUF_MAXLEN]={0};
	
	ostrCmd.clear();

	pRegItem = pRegItemsArea->GetDataItemByRegid(di);
    
	if (pRegItem != NULL)
	{//��������ڵĴ���
		int len = pRegItem->ReadData(buf);
		MakeFrame((S_CHAR)CTL_READ_ACK, di,buf, len, ostrCmd);
	}
	else
	{//��������ڵĴ���
		loget<<"���������"<<endl;
		MakeFrame((S_CHAR)CTL_READ_ERR_ACK,(S_CHAR)ERR_READ_INVALID, ostrCmd);
	}
	return 0;		
	
}
S_WORD C_Protocol::BatchReadCmd(S_WORD di, string& ostrCmd)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	string batchRdStrData;

	ostrCmd.clear();
	pRegItem = pRegItemsArea->GetDataItemByRegid(di);
	if (pRegItem != NULL)
	{//��������ڵĴ���
		pRegItem->BatchReadData(batchRdStrData);
		MakeFrame((S_CHAR)CTL_BATCH_READ_ACK, batchRdStrData, ostrCmd);
	}
	return 0;
}
/*
brief: ���ݴ���ӿڣ�������ͨ�����յ����ݽ��д���
param [in]C_DspDevice *pDspDevice:Dspģ��������
param [in]string& iStrData:��������
param [out]string& oStrData:����������
return 0:�Ѵ���-1:δ����
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
	{//��ȡ����һ֡����
		ParseFrame(strCmd, frameStu);
		if (frameStu.m_ctlcode == (S_CHAR)CTL_WRITE)
		{
			if (0x0c01 == frameStu.m_di)
			{
				//����֡����
				if (SaveFrame(frameStu) == true)
				{
					MakeFrame((S_CHAR)CTL_WRITE_ACK,oStrData);
				}
				else
				{
					MakeFrame((S_CHAR)CTL_WRITE_ERR_ACK,(S_CHAR)ERR_WRITE__INVALID,oStrData);
				}
				
			}
			else if (pDspDevice->Write(strCmd,oStrData) == true)//��DSP�豸д����
			{
				logvt<<"Dsp����д�ɹ�"<<endl;
				S_FrameStu tmpFrameStu;
				ParseFrame(oStrData, tmpFrameStu);	

				if (tmpFrameStu.m_ctlcode == (S_CHAR)CTL_WRITE_ACK)//����дDSP�ɹ������µ��ļ�ϵͳ
				{
					//����֡����
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
			string tempStc; // ��ֿ�������֡
			string tempReplyBegin; // �ظ�֡��ͷ
			string tempReplyData; // �ظ�֡����
			tempReplyData.clear();
			string tempReplyEnd; // �ظ�֡��β

			// ������֡��֣�Ȼ��ÿ����������д��dsp������ɹ���������ļ�ϵͳ
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
					//����֡����
					//SaveFrame(frameStu);
					S_WORD regid;
					regid = (S_WORD)((frameStu.m_strdata[i]) | (frameStu.m_strdata[i + 1]*256));
					S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
					S_RegItem* pRegItem;
					pRegItem = pRegItemsArea->GetDataItemByRegid(regid);

					

					if (pRegItem != NULL)
					{
						pRegItem->SaveData(tempStc.data(), frameStu.m_strdata[i + 2] + frameStu.m_strdata[i + 3]*256);
					}
					else
					{
						loget<<"SaveFrame:������"<<HEX<<regid<<"������"<<endl;
						return -1;
					}
					tempReplyData.push_back(0x00);
				}
				else
				{
					tempReplyData.push_back(0x05);
				}
			}
			
			// ��֯�ظ�֡
			S_BYTE length = 0;
			tempReplyBegin.clear();
			tempReplyBegin.push_back((char)0xEB);
			tempReplyBegin.push_back((char)0x90);
			tempReplyBegin.push_back((char)0x85);
			// ����ظ�֡�еĳ���ֵ
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
		else if (frameStu.m_ctlcode == (S_CHAR)CTL_READ)//������
		{
			ReadCmd(frameStu.m_di,oStrData);	
		}
		else if (frameStu.m_ctlcode == (S_CHAR)CTL_BATCH_READ)//������
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
brief: ��Լ��֡
param [in]S_CHAR ctlcode:������
param [in]S_WORD di:���ݱ�ʶ
param [in]S_CHAR *pData:������ָ��
param [out]S_WORD len:���������ݳ���
param [out]string& oFrame:��֡����
return 0:��֡�ɹ���-1:��֡ʧ��
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
brief: �쳣��֡
param [in]S_CHAR ctlcode:������
param [in]S_CHAR cErrCode:������
param [out]string& oFrame:��֡����
return 0:��֡�ɹ���-1:��֡ʧ��
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
brief: ����д�ظ���֡
param [in]S_CHAR ctlcode:������
param [out]string& oFrame:��֡����
return 0:��֡�ɹ���-1:��֡ʧ��
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
brief: ��������֡
param [in]S_CHAR ctlcode:������
param [in]string& :������ȡ������������
param [out]string& oFrame:��֡����
return 0:��֡�ɹ���-1:��֡ʧ��
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
brief: ��֡���ݴ��������л���֡�ṹ
param [in]const string& strCmd:����֡���ݴ�
param [out]S_FrameStu&  frameStu:֡�ṹ
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
			frameStu.m_diLen =  ((S_WORD)strCmd[FRAME_LEN_POS])&0X00FF + ((((S_WORD)strCmd[FRAME_LEN_POS+1])&0x00ff)*256);//�˴��޸�
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
brief: ��֡������������ݱ������ļ�ϵͳ
param [in]const S_FrameStu&  frameStu:�������֡�ṹ
param [out]bool:true:����ɹ�������ʧ��
author:Steven Wan
date: 2015-04-23
*/ 

bool C_Protocol::SaveFrame(const S_FrameStu&  frameStu)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemByRegid(frameStu.m_di);

	if (pRegItem != NULL)
	{
		logvt<<"������"<<HEX<<frameStu.m_di<<"��������:"<<HEX<<frameStu.m_strdata<<endl;
		return pRegItem->SaveData(frameStu.m_strdata.data(), frameStu.m_diLen);
	}
	else
	{
		loget<<"SaveFrame:���������"<<endl;
		return false;
	}
}

