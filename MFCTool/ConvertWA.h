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
	//多字头转宽字头
	static std::wstring ConvertWA_A2W(string strSrc);

	//宽字头转多字头
	static std::string ConvertWA_W2A(std::wstring wstrSrc);

	static std::string UTF8_To_string(const std::string & str);
	static std::string string_To_UTF8(const std::string & str);

	//秒变 时分秒 60 -> 00:01:00
	static std::string SecToHMS(unsigned int sec);
	static unsigned int HMSToSec(const string &hms);

	//完整的日期 之间的差值2019-11-16 00:00:00  2019-11-15 23:59:10 返回秒
	static int DateTimeSpanToSec(CString &startTime,CString &endTime);

	//16进制转字符
	static void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen);
	static void AsciiToHex(unsigned char * pAscii, unsigned char * pHex, int nLen);
	//十六进制数转换成二进制字符串
	static void HexToBinStr(unsigned char* hexStr, unsigned char* binStr, int lenHex);
	static void BinStrToHex(unsigned char* binStr, unsigned char* hexStr, int lenBin);

	//16进制int互转转换  356 ->bytes[0] = 0x64 bytes[1] = 0x01
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

	//BYTE 与 位之间的转换
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


	/* 返回ch字符在sign数组中的序号 */
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
	/* 十六进制数转换为十进制数 */
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

	// 使用字符串分割
	static void Stringsplit(const string& str, const string& splits, vector<string>& res)
	{
		if (str == "")		return;
		//在字符串末尾也加入分隔符，方便截取最后一段
		string strs = str + splits;
		size_t pos = strs.find(splits);
		int step = splits.size();

		// 若找不到内容则字符串搜索函数返回 npos
		while (pos != strs.npos)
		{
			string temp = strs.substr(0, pos);
			res.push_back(temp);
			//去掉已分割的字符串,在剩下的字符串中进行分割
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

	//去除尾部空格
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