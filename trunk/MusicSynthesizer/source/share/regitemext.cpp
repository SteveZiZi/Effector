#include "sysbase.h"
#include "regitemext.h"
#include "timeop.h"
#include "status.h"
#include "fileop.h"
#include <fstream>

bool C_RegitemExt_0b01::SaveData(const S_CHAR *pBuf, S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemByRegid(0x0b01);
	
	if (len != pRegItem->GetLength())
	{
		return false;
	}
	char bcdStr7B[7] = {0};
	memcpy(bcdStr7B,pBuf,len);
	C_TIME::SetSysHWClock_Big(bcdStr7B);
	return true;
}

S_WORD C_RegitemExt_0b01::ReadData(S_CHAR *pBuf)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	pRegItem = pRegItemsArea->GetDataItemByRegid(0x0b01);
	
	char bcdStr7B[7] = {0};
	C_TIME::CurTime2BCDStr_Big(bcdStr7B);
	memcpy(pBuf,bcdStr7B,7);
	return pRegItem->GetLength();
}

S_WORD C_RegitemExt_FFFF::ReadData(string &oStrData)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;
	S_WORD pos=0;
	char dataBuf[4096];

	for (int i=0; i < pRegItemsArea->GetItemNum(); i++)
	{
		pRegItem  = pRegItemsArea->GetDataItemByIndex(i);
		if (pRegItem->m_bDataBlock == true)
		{
			continue;
		}

		if (pRegItem->IsValidData() == true)
		{
			memcpy(dataBuf+pos,(char*)(&pRegItem->m_regid),2);
			pos += 2;
			memcpy(dataBuf+pos,(char*)&pRegItem->m_Len,2);
			pos += 2;
			memcpy(dataBuf+pos,pRegItem->m_data,pRegItem->m_Len);
			pos += pRegItem->m_Len;
		}
/*		else
		{
			memcpy(dataBuf+pos,(char*)(&pRegItem->m_regid),2);
			pos += 2;
			memcpy(dataBuf+pos,invaliddata,2);
			pos += 2;
		}*/
	}
	oStrData.clear();
	oStrData.assign(dataBuf,pos);
	return pos;
}

// ����Ϊ����Ҫ���з�������ݽ��з����
// sunId subId1 subId2�ֱ�Ϊ��ӵ�ԭ����ʽΪ��0111+��0101��һ�ֽ�bit0��*3+��0101��һ�ֽ�bitbit1~3����ͬ
bool C_RegitemExt_0101::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0102::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0211 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0103::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0311 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0104::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0411 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0105::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0511 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0106::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0611 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0107::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0711 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0108::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0811 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0109::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0911 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010A::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0A11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010B::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0B11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010C::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0C11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010D::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0D11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010E::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0E11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_010F::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x0F11 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0201::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x1111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0202::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x1211 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0203::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x1311 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0301::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x2111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0401::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x3111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_0601::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0x5111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_1101::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0xA111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}

bool C_RegitemExt_1003::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_RegItems* pRegItemsArea = STATUS.GetRegitemsArea();//��ȡ������Ĵ�����ָ��
	S_RegItem* pRegItem;

	S_WORD subId,subId1,subId2;
	subId1 = ((pBuf[0] & 0x01) * 3) <<4;
	subId2 = (pBuf[0] & 0x0E) << 3;
	subId = 0xB111 + subId1 + subId2;

	pRegItem = pRegItemsArea->GetDataItemBySubRegid(subId);

	if (len != pRegItem->GetLength())
	{
		return false;
	}
	memcpy(&pRegItem->m_data[0],pBuf,len);
	return true;
}
S_WORD C_RegitemExt_0C01::frameNowNo = 0;
bool C_RegitemExt_0C01::SaveData(const S_CHAR *pBuf,S_WORD len)
{
	S_WORD frameTotal;
	S_WORD frameNum;
	string tempContext;

	//frameTotal = (((S_WORD)pBuf[1]) << 8) | (S_WORD)pBuf[0];
	//frameNum = (((S_WORD)pBuf[3]) << 8) | (S_WORD)pBuf[2];
	frameTotal = (S_WORD)((S_WORD)(pBuf[1] << 8) | (S_BYTE)(pBuf[0]));
	frameNum = (S_WORD)((S_WORD)(pBuf[3] << 8) | (S_BYTE)(pBuf[2]));
	/*loget << "��֡��" << HEX << pBuf[1] << pBuf[0] << endl;
	loget << "֡���" << HEX << pBuf[3] << pBuf[2] << endl;*/
	loget << "֡���" << frameNum <<endl;
	loget << "��֡��" << frameTotal << endl;
	if (1 == frameNum)
	{
		frameNowNo = 0;
		tempContext.clear();
#ifdef WIN32
		system("del /mnt/update/Dspupdate.dat");
#else
		system("rm /mnt/update/Dspupdate.dat");
#endif
	}
	frameNowNo = frameNowNo + 1;
	if (frameNum < frameTotal)
	{
		// �ж��Ƿ���֡��ʧ
		if (frameNowNo - frameNum > 1)
		{
			loget << "����֡��ʧ" << endl;
			/*loget << "��ǰ֡No" << frameNowNo << endl;
			loget << "֡���" << frameNum << endl;*/
			return false;
		}
		// �洢���ݵ���ʱ�ļ���
		tempContext.clear();
		for (int i = 0;i < len - 4;i++)
		{
			//loget << HEX << pBuf[i + 2] << endl;
			tempContext.push_back(pBuf[i + 4]);
		}
		fstream writeFile;
		string tempFileName = "/mnt/update/tempfile";
		writeFile.open(tempFileName.c_str(),ios::out | ios::app| ios::binary);
		if (!writeFile)
		{
			loget << "д����ʱ�ļ�ʧ��" << endl;
			return false;
		}
		else
		{
			loget << "д����ʱ�ļ��ɹ�" << endl;
			/*loget << "֡���" << HEX << frameNum <<endl;
			loget << "��֡��" << HEX << frameTotal << endl;
			loget << "֡���" << frameNum << endl;
			loget << "��֡��" << frameTotal << endl;*/
			writeFile.write(tempContext.data(),(std::streamsize)tempContext.size());//�컪�޸ģ�ԭ���ǣ�//writeFile.write(tempContext.data(),(S_WORD)tempContext.size());
		}
		writeFile.close();
		//loget << tempContext << endl;
	}
	else if (frameNum == frameTotal)
	{
		// ����ʱ�ļ��е����ݶ�ȡ���� �ŵ�tempContext��
		fstream readFile;
		string tempFileName = "/mnt/update/tempfile";
		readFile.open(tempFileName.c_str(),ios::in | std::ios::binary);
		/*loget << "frameNum = " << frameNum << endl;
		loget << "frameTotal = " << frameTotal << endl;*/
		if (!readFile)
		{
			loget << "����ʱ�ļ�ʧ��" << endl;
			return false;
		}
		else
		{
			tempContext.clear();
			istreambuf_iterator<char> beg(readFile), end;
			string temp(beg, end);

			/*//�컪���
			loget << HEX << beg << endl;
			loget << HEX << end << endl;
			loget << HEX << temp.size() << endl;
			*/
			tempContext = temp;
			
		}
		readFile.close();
		// �����ʱ�ļ�
#ifdef WIN32
		system("del /mnt/update/tempfile");
#else
		system("rm /mnt/update/tempfile");
#endif
		//loget << tempContext << endl;
		// ��֡�л�ȡCRC
		unsigned short CRC;
		S_BYTE crc[2];
		crc[0] = (S_BYTE)pBuf[4];
		crc[1] = (S_BYTE)pBuf[5];
		CRC = crc[0] | ((unsigned short)crc[1] << 8);
		/*loget << HEX << CRC << endl;
		loget << HEX << tempContext.size() << endl;
		loget << HEX << CRC16RTU((unsigned char *)tempContext.data(),(tempContext.size()))<< endl;*/
		// CRCУ
		if (CRC == CRC16RTU((const unsigned char *)tempContext.data(),(unsigned int)(tempContext.size())))
		{
			loget << "У��ɹ�" << endl ;
			// ����ʱ�ļ�����ΪDspupdate.dat
			ofstream writeFile;
			string fileName = "/mnt/update/Dspupdate.dat";
			writeFile.open(fileName.c_str(),ios::out | std::ios::binary);
			if (!writeFile)
			{
				loget << "����Dspupdate.datʧ��" << endl;
				return false;
			}
			else
			{
				writeFile.write(tempContext.data(),(std::streamsize)tempContext.size());
			}
			//loget << tempContext << endl;
			writeFile.close();
#ifdef WIN32

#else
			system("/mnt/update/update.sh");
			logvt<<"����/mnt/update/update.sh������½ű�"<<endl;
#endif
		}
		else
		{
			// �ظ�У��ʧ��
			loget << "�ظ�У��ʧ��" << endl;
			return false;
		}
	}
	else if(frameNum > frameTotal)
	{
		loget << "�����ļ�����" << endl;
		/*loget << "֡���" << frameNum << endl;
		loget << "��֡��" << frameTotal << endl;*/
		return false;
	}
	return true;
}

/******************************************************
*��������:CRC16RTU
*��   ��:pszBuf  ҪУ�������
        unLength У�����ݵĳ�
*��   ��:У��ֵ
*��   ��:ѭ������У��-16
         ��RTU��׼-0xA001��
*******************************************************/
unsigned short C_RegitemExt_0C01::CRC16RTU(const unsigned char * pszBuf, unsigned int unLength)
{
	unsigned short  CRC=0XFFFF;
	unsigned int CRC_count;

	for(CRC_count=0;CRC_count<unLength;CRC_count++)
	{
		int i;

		CRC=CRC^*(pszBuf+CRC_count);

		for(i=0;i<8;i++)
		{
			if(CRC&1)
			{
				CRC>>=1;
				CRC^=0xA001;
			}
			else
			{ 
                CRC>>=1;
			}
				
		}
	}

	return CRC;
}

