#pragma once
#include "VSZip.h"
class CVszManager
{
public:
	CVszManager();
	~CVszManager();

	//解压
	int DecompressFile(CString filePath, CString &decompressDir);

	//获取解压文件夹中的视频
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

