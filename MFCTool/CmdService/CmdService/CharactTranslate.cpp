/******************************************************************************************
*	版权所有(c) 2018 北京冠华天视软件部
*	文件名称:	CharactTranslate.cpp
*	文件说明:	字符编码换换声明文件
*	当前版本:	1.0.20180714
*	作者:		贾巍云
*	创建日期:	2018-07-14
******************************************************************************************/
#include "stdafx.h"
#include "CharactTranslate.h"

#include <atlbase.h>
#include <atlconv.h>

////////////////////////////////////////////////////////////////////////////////////////////
CCharTrans::CCharTrans(void)
{  
  

}  
  
CCharTrans::~CCharTrans(void)  
{  
  
}


CString CCharTrans::GBKToUtf8(const CString &strGBK)
{
	CString strUtf8;
// 	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
//     unsigned short * wszUtf8 = new unsigned short[len + 1];
//     memset(wszUtf8, 0, len * 2 + 2);
//     MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, wszUtf8, len);
// 	
//     len = WideCharToMultiByte(CP_UTF8, 0, wszUtf8, -1, NULL, 0, NULL, NULL); 
//     char *szUtf8 = new char[len + 1];
//     memset(szUtf8, 0, len + 1);
//     WideCharToMultiByte (CP_UTF8, 0, wszUtf8, -1, szUtf8, len, NULL,NULL);
// 	
//     strUtf8 = szUtf8;
//     delete[] szUtf8;
//     delete[] wszUtf8;	
// 
// 	szUtf8 = NULL;
// 	wszUtf8 = NULL;

	return strUtf8;
}


CString CCharTrans::Utf8ToGBK(const CString &strUtf8)
{
	CString strGBK;
// 	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, NULL, 0);
// 	wchar_t * szUnicode = new wchar_t[len];
// 	memset(szUnicode, 0, len);
// 	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUtf8, -1, szUnicode, len);
// 	
// 	len = WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, NULL, 0, NULL, NULL);
// 	char *szGbk = new char[len];
// 	memset(szGbk, 0, len);
// 	WideCharToMultiByte(CP_ACP,0, szUnicode, -1, szGbk, len, NULL, NULL);
// 	
// 	strGBK = szGbk;
// 
// 	delete[]szUnicode;
// 	delete[]szGbk;
// 	szUnicode = NULL;
// 	szGbk = NULL;

	return strGBK;
}


int CCharTrans::GBKToUnicode(const CString &strGBK, wchar_t *pUnicodeBuf)
{
// 	CString strUnicode;
// 	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, NULL, 0);
// 	wchar_t *szUnicode = new wchar_t[len];
// 	memset(szUnicode, 0, len * 2);
// 	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strGBK, -1, szUnicode, len);
// 	
// 	strUnicode = szUnicode;
// 
// 	delete[]szUnicode;
// 	szUnicode = NULL;
// 
// 	return strUnicode;

// 	int len = strlen((LPCSTR)strGBK);
// 	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, strGBK,  - 1, NULL, 0);
// 	memset(pUnicodeBuf, 0, (unicodeLen + 1) *sizeof(wchar_t));
// 	::MultiByteToWideChar(CP_ACP, 0, strGBK,  - 1, (LPWSTR)pUnicodeBuf, unicodeLen);

	return 0;
}


CString CCharTrans::UnicodeToGBK(const WCHAR *szUnicode)
{
	CString strGBK;
	if(szUnicode == NULL)
		return strGBK;

	int len = WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, NULL, 0, NULL, NULL);
	char *szGbk = new char[len + 1];
	memset(szGbk, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, szUnicode, -1, szGbk, len, NULL, NULL);
	
	strGBK = szGbk;
	delete[]szGbk;
	szGbk = NULL;
	return strGBK;


// 	USES_CONVERSION;
//     strGBK = W2A((LPCWSTR)strUnicode);
// 	return strGBK;
	
}
