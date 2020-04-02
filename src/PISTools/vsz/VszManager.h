#pragma once
#include "VSZip.h"
class CVszManager
{
public:
	CVszManager();
	~CVszManager();

	//��ѹ
	int DecompressFile(CString filePath, CString &decompressDir);

	//��ȡ��ѹ�ļ����е���Ƶ
	BOOL GetVideoFromePlayList(CString strDir, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg);

private:
	BOOL DeleteTree(CString strPath);
	BOOL IsExistFile(LPCTSTR szFile);
	BOOL GetLayoutVideoList(CString strDir, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg);
	BOOL GetModuleVideoList(LPCTSTR szFile, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg);
	CString GetFileName(CString strFullPath);
	CString GetFilePath(CString strFullPath, BOOL bIncludeSlash = true);
	CString GetFileExt(CString strFullPath);
	int FindArrayItem(CStringArray & strArray, CString strItem, BOOL bCaseSensitive, int iDefault);
};

