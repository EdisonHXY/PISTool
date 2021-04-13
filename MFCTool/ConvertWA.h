// ConvertWA.h: interface for the ConvertWA class.
//
// ConvertWA Debug 1.0
// Copyright (C) 2016-2026 Beijing Mysher Technology CO., LTD All rights reserved
// Use in commercial applications requires written permission
// This software is provided "as is", with no warranty.

#pragma once
#include <string>
#include <sstream>
#include <vector>
using namespace std;
class ConvertWA
{
public:
	//����ͷת����ͷ
	static std::wstring ConvertWA_A2W(string strSrc);

	//����ͷת����ͷ
	static std::string ConvertWA_W2A(std::wstring wstrSrc);

	static std::string UTF8_To_string(const std::string & str);
	static std::string string_To_UTF8(const std::string & str);

	//��� ʱ���� 60 -> 00:01:00
	static std::string SecToHMS(unsigned int sec);
	static unsigned int HMSToSec(const string &hms);

	//���������� ֮��Ĳ�ֵ2019-11-16 00:00:00  2019-11-15 23:59:10 ������
	static int DateTimeSpanToSec(CString &startTime,CString &endTime);

	//16����ת�ַ�
	static void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen);
	static void AsciiToHex(unsigned char * pAscii, unsigned char * pHex, int nLen);
	//ʮ��������ת���ɶ������ַ���
	static void HexToBinStr(unsigned char* hexStr, unsigned char* binStr, int lenHex);
	static void BinStrToHex(unsigned char* binStr, unsigned char* hexStr, int lenBin);

	//16����int��תת��  356 ->bytes[0] = 0x64 bytes[1] = 0x01
	static void intToByte2(int i, BYTE *bytes)
	{
		bytes[0] = (byte)(0xff & i);
		bytes[1] = (byte)((0xff00 & i) >> 8);
	}

	static int Byte2ToInt(BYTE *bytes)
	{

		int num = bytes[0] & 0xFF;
		num |= ((bytes[1] << 8) & 0xFF00);

		return num;

	}

	//BYTE �� λ֮���ת��
	static void GetBitFromByte(const BYTE byData, int *bitList)
	{
		for (int i =0 ;i<8;++i)
		{
			int nTmp = (1 << i);
			bitList[i] = ((byData & nTmp) == nTmp )? 1 : 0;
		}

	}

	static void BitListToByte(BYTE *bData,const int *bitList)
	{
		int n = 0;
		for (int i = 0 ; i < 8;++i)
		{
			int nB = bitList[i] * (1 << i);
			n += nB;
		}
		*bData = n;
	}
	
	// "0x14" -> 0x14
	static void GetHexFromStr(CWnd *wd, BYTE *nByte)
	{
		CString strText;
		wd->GetWindowText(strText);
		int nTmpNum = 0;
		sscanf(strText, "%x", &nTmpNum);
		*nByte = nTmpNum;
	}


	/* ����ch�ַ���sign�����е���� */
	static int getIndexOfSigns(char ch)
	{
		if (ch >= '0' && ch <= '9')
		{
			return ch - '0';
		}
		if (ch >= 'A' && ch <= 'F')
		{
			return ch - 'A' + 10;
		}
		if (ch >= 'a' && ch <= 'f')
		{
			return ch - 'a' + 10;
		}
		return -1;
	}
	/* ʮ��������ת��Ϊʮ������ */
	static long hexToDec(const char *source)
	{
		long sum = 0;
		long t = 1;
		int i, len;

		len = strlen(source);
		for (i = len - 1; i >= 0; i--)
		{
			sum += t * getIndexOfSigns(*(source + i));
			t *= 16;
		}

		return sum;
	}

	// ʹ���ַ����ָ�
	static void Stringsplit(const string& str, const string& splits, vector<string>& res)
	{
		if (str == "")		return;
		//���ַ���ĩβҲ����ָ����������ȡ���һ��
		string strs = str + splits;
		size_t pos = strs.find(splits);
		int step = splits.size();

		// ���Ҳ����������ַ��������������� npos
		while (pos != strs.npos)
		{
			string temp = strs.substr(0, pos);
			res.push_back(temp);
			//ȥ���ѷָ���ַ���,��ʣ�µ��ַ����н��зָ�
			strs = strs.substr(pos + step, strs.size());
			pos = strs.find(splits);
		}
	}

	static void SplitString(const string& s, vector<string>& sv, const char flag /*= ' '*/)
	{
		sv.clear();
		istringstream iss(s);
		string temp;

		while (getline(iss, temp, flag)) {
			sv.push_back(temp);
		}
		return;
	}

	//ȥ��β���ո�
	static std::string& trim(std::string &s)
	{
		if (s.empty())
		{
			return s;
		}

		//s.erase(0, s.find_first_not_of(" "));
		s.erase(s.find_last_not_of(" ") + 1);
		return s;
	}

};