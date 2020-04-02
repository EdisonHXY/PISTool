#include "stdafx.h"
#include "VszManager.h"


CVszManager::CVszManager()
{
}


CVszManager::~CVszManager()
{
}

int CVszManager::DecompressFile(CString filePath, CString &decompressDir)
{
	//删除存在的目录
	//int iIndex = filePath.ReverseFind('.');
	//decompressDir = filePath.Left(iIndex);
	//DeleteTree(decompressDir);

	//decompress
	if (VS_Decompress(filePath, decompressDir, NULL, NULL) != VSZ_OK)
	{
		return -1;
	}

	return 0;
}

BOOL CVszManager::DeleteTree(CString strPath)
{
	HANDLE hFind;
	BOOL bFound;
	WIN32_FIND_DATA wfData;

	bFound = true;
	hFind = FindFirstFile(strPath + "\\*.*", &wfData);
	while (hFind != INVALID_HANDLE_VALUE && bFound)
	{
		if (wfData.cFileName[0] != '.')
		{
			SetFileAttributes(strPath + "\\" + wfData.cFileName, FILE_ATTRIBUTE_NORMAL);
			if (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if (!DeleteTree(strPath + "\\" + wfData.cFileName))
				{
					return false;
				}
			}
			else
			{
				if (!DeleteFile(strPath + "\\" + wfData.cFileName))
				{
					return false;
				}
			}
		}
		bFound = FindNextFile(hFind, &wfData);
	}
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	SetFileAttributes(strPath, FILE_ATTRIBUTE_NORMAL);
	return RemoveDirectory(strPath);
}

BOOL CVszManager::IsExistFile(LPCTSTR szFile)
{
	if (!szFile || szFile[0] == 0)
	{
		return false;
	}
	return PathFileExists(szFile);
}

BOOL CVszManager::GetVideoFromePlayList(CString strDir, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg)
{
	HANDLE hFind;
	BOOL bFound;
	WIN32_FIND_DATA wfData;

	bFound = true;
	hFind = FindFirstFile(strDir + "*.*", &wfData);
	while (hFind != INVALID_HANDLE_VALUE && bFound)
	{
		if ((wfData.cFileName[0] != '.') && (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (!GetLayoutVideoList(strDir + wfData.cFileName + "\\", strArray, strArrayMD5, strMsg))
			{
				return false;
			}
		}
		bFound = FindNextFile(hFind, &wfData);
	}
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	return true;
}

BOOL CVszManager::GetLayoutVideoList(CString strDir, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg)
{
	HANDLE hFind;
	BOOL bFound;
	CString strModule, strFlt;
	WIN32_FIND_DATA wfData;

	//枚举子目录
	bFound = true;
	hFind = FindFirstFile(strDir + "*.*", &wfData);
	while (hFind != INVALID_HANDLE_VALUE && bFound)
	{
		strModule = wfData.cFileName;
		strModule.MakeUpper();
		if ((wfData.cFileName[0] != '.') && (wfData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			strFlt.Format("%s%s\\%s.flt", strDir, wfData.cFileName, wfData.cFileName);
			if (!GetModuleVideoList(strFlt, strArray, strArrayMD5, strMsg))
			{
				return false;
			}
		}
		bFound = FindNextFile(hFind, &wfData);
	}
	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	return true;
}

BOOL CVszManager::GetModuleVideoList(LPCTSTR szFile, CStringArray & strArray, CStringArray & strArrayMD5, CString & strMsg)
{
	int i, iSize;
	char szBuf[MAX_PATH];
	CString strField, strName, strPath;

	iSize = 0;
	if (!IsExistFile(szFile))
	{
		return true;
	}
	if (!GetPrivateProfileStruct("MEDIALIST", "FileNum", &iSize, sizeof(iSize), szFile))
	{	//无法读取文件数量，认为正常
		return true;
	}
	strPath = GetFilePath(szFile);
	for (i = 0; i < iSize; i++)
	{
		strField.Format("File%d", i);
		GetPrivateProfileString("MEDIALIST", strField, "", szBuf, sizeof(szBuf), szFile);
		if (szBuf[0] != 0)
		{
			strName = GetFileName(szBuf);
			if (FindArrayItem(strArray, strName, false, -1) == -1)
			{
				strArray.Add(strName);
				strArrayMD5.Add(strPath + strName + ".md5");
			}
		}
	}
	return true;
}

CString CVszManager::GetFileName(CString strFullPath)
{
	int iIndex, iLength;

	iIndex = strFullPath.ReverseFind('\\');
	iLength = strFullPath.GetLength();
	if (iIndex != -1 && iIndex != iLength - 1)
	{
		return strFullPath.Mid(iIndex + 1);
	}
	return strFullPath;
}

CString CVszManager::GetFilePath(CString strFullPath, BOOL bIncludeSlash)
{
	int iIndex = strFullPath.ReverseFind('\\');
	if (iIndex != -1)
	{
		return strFullPath.Left(iIndex + ((bIncludeSlash) ? 1 : 0));
	}
	return strFullPath;
}

CString CVszManager::GetFileExt(CString strFullPath)
{
	int iIndex = strFullPath.ReverseFind('.');
	if (iIndex != -1)
	{
		return strFullPath.Mid(iIndex + 1);
	}
	return "";
}

int CVszManager::FindArrayItem(CStringArray & strArray, CString strItem, BOOL bCaseSensitive, int iDefault)
{
	int i, iSize;

	iSize = strArray.GetSize();
	for (i = 0; i < iSize; i++)
	{
		if (bCaseSensitive)
		{
			if (strItem.Compare(strArray[i]) == 0)
			{
				return i;
			}
		}
		else
		{
			if (strItem.CompareNoCase(strArray[i]) == 0)
			{
				return i;
			}
		}
	}
	return iDefault;
}
