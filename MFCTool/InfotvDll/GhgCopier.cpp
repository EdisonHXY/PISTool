// GhgCopier.cpp: implementation of the CGhgCopier class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GhgCopier.h"
#include "InfotvManager.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGhgCopier::CGhgCopier()
{
	m_pParam = NULL;
	szError = NULL;

	m_iStep = 0;
	arySrc.SetSize(0);
	aryDes.SetSize(0);
	aryCopy.SetSize(0);
}

CGhgCopier::~CGhgCopier()
{	
	arySrc.SetSize(0);
	aryDes.SetSize(0);
	aryCopy.SetSize(0);
}

BOOL CGhgCopier::Setup(GHG_PARAM *pParam)
{
	ASSERT(pParam);

	m_pParam = pParam;
	szError = pParam->szErrorString ;

	// 确定路径
	m_strOldPath = pParam->szOldPath;
	m_strNewPath = pParam->szCurPath;
	m_strOldPath.MakeLower();
	m_strNewPath.MakeLower();
	if (m_strNewPath.IsEmpty()) 
	{	sprintf(szError, "没有指定当前路径");
		return FALSE;
	}
	if (m_strOldPath.IsEmpty())
	{	m_strOldPath = m_strNewPath;
		m_bSame = TRUE;
	}
	else
	{	m_bSame = (m_strOldPath == m_strNewPath);
	}
	ASSERT(m_strNewPath.Right(1) == "\\");
	ASSERT(m_strOldPath.Right(1) == "\\");
	m_strSaveFile.Format("%s%s.Flt", m_strNewPath,CInfotvManager::__strModuleName);

	m_iStep = 1;
	arySrc.SetSize(0);
	aryDes.SetSize(0);
	aryCopy.SetSize(0);

	return TRUE;
}

VOID CGhgCopier::AddFile(LPCTSTR strFile, BOOL bCopy)
{ 
	ASSERT(m_iStep >= 1);
	
	arySrc.Add(strFile); 
	aryCopy.Add(bCopy); 
	m_iStep = 2;
}

CString CGhgCopier::MakeNewName(LPCTSTR strFile)
{
	int iIndex;
	CString strName = strFile, strExt , strIndex , strRet;

	int iIdx = strName.ReverseFind('.');
	if (iIdx != -1)
	{	strExt = strName.Mid(iIdx);
		strName = strName.Left(iIdx);
	}
	else
	{	strExt = "";
	}

	int iLen = strName.GetLength();
	int i = 0;
	for ( i = iLen - 1 ; i <= 0 ; i--)
	{	
		if (strName[i] > '9' && strName[i] < '0') break;
	}

	if (i != iLen - 1)
	{	strIndex = strName.Mid(i + 1);
		iIndex = atoi(strIndex) + 1;
		strName = strName.Left(i + 1);
	}
	else
	{	iIndex = 0;
	}

	int iSize = arySrc.GetSize();
	for (i = 0 ; i < iSize ; i++ , iIndex ++)
	{	
		strIndex.Format("%d", iIndex);
		strRet = strName + strIndex + strExt;
		if (strRet == aryDes[i]) continue;
		break;
	}

	return strRet;
}

VOID CGhgCopier::PrepareCopy()
{
	ASSERT(m_iStep >= 2);
	int i, j , iIndex;
	CString strFile, strNewFile, strPath;

	int iSize = arySrc.GetSize();
	ASSERT(iSize == aryCopy.GetSize());
	aryDes.SetSize(iSize);

	// Src数据全部变小写，Des数据变为文件名
	for (i = 0 ; i < iSize ; i++)
	{	iIndex = arySrc[i].ReverseFind('\\');
		if (iIndex > 0)
		{	strFile = arySrc[i].Mid(iIndex + 1);
			arySrc[i].MakeLower(); 
		}
		else
		{	strFile = arySrc[i];
			strPath = m_strOldPath + strFile;
			strPath.MakeLower() ;
			arySrc.SetAt(i, strPath);
		}
		strFile.MakeLower();
		aryDes.SetAt(i, strFile);
	}

	// 检验是否存在与模块系统文件冲突的文件
	for (i = 0 ; i < iSize ; i++)
	{	
		if (aryCopy[i]) continue;
		iIndex = 0;
		strFile = aryDes[i];
		for (j = 0 ; j < iSize ; j++)
		{	
			if (j == i) continue;
			if (aryDes[j] == strFile)
			{
				ASSERT(aryCopy[j]);
				if (iIndex == 0) 
				{	strNewFile = MakeNewName(strFile);
					iIndex ++;
				}
				aryDes[j] = strNewFile;
			}
		}
	}

	for (i = 0 ; i < iSize ; i++)
	{
		if (!aryCopy[i]) continue;

		// 寻找是否有同名文件
		strFile = aryDes[i];
		for (j = i + 1 ; j < iSize ; j++)
		{
			if ((aryDes[i] == aryDes[j]) && (arySrc[i] != arySrc[j]))
			{
				strFile = MakeNewName(strFile);
				break;
			}
		}

		if (m_strNewPath + strFile == arySrc[i]) aryCopy[i] = false;

		// 寻找同一文件
		for (j = i + 1 ; j < iSize ; j++)
		{
			if (arySrc[i] == arySrc[j])
			{	aryCopy[j] = false;
				aryDes[j] = strFile;
			}
		}

		aryDes[i] = strFile;
	}

	m_iStep = 3;
}

BOOL CGhgCopier::Verify()
{
	ASSERT( m_iStep >= 3);

	int iSize = arySrc.GetSize();

	for (int i = 0 ; i < iSize ;i ++)
	{
		if (aryCopy[i] && !CInfotvManager::IsExistFile(arySrc[i]))
		{	sprintf(szError, "文件'%s'不存在", arySrc[i]);
			return FALSE;
		}
	}

	m_iStep = 4;
	return TRUE;
}

BOOL CGhgCopier::Copy()
{
	ASSERT(m_iStep >= 4);
	int iSize = arySrc.GetSize();
	ASSERT(aryDes.GetSize() == iSize && aryCopy.GetSize() == iSize);
	DWORD dw;
	CString strOldFile, strNewFile;

	for (int i = 0 ; i < iSize ; i++)
	{	
		if (aryCopy[i])
		{	strOldFile = arySrc[i];
			strNewFile = m_strNewPath + aryDes[i];
			dw = ::GetFileAttributes(strOldFile);
			if ((dw && FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
			{	dw &= ~FILE_ATTRIBUTE_READONLY;
				::SetFileAttributes(strOldFile, dw);
			}
			if (!CopyFile(strOldFile, strNewFile, false))
			{	sprintf(szError, "拷贝‘%s’文件出错!", strOldFile);
				return FALSE;
			}
		}
	}

	m_iStep = 5;

	return TRUE;
}

BOOL CGhgCopier::WriteFileList(const int iType, const char * szFileName, const int iIndex, const char * szFile)
{
	CString strSection, strKey;

	switch(iType)
	{
	case 0:
		strSection = "FILELIST";
		break;
	case 1:
		strSection = "MEDIALIST";
		break;
	default:
		ASSERT(FALSE);
	}

	strKey.Format("File%d", iIndex);

	return WritePrivateProfileString(strSection, strKey, szFileName, szFile);
}

BOOL CGhgCopier::WriteListNum(const int iType, const int iCount, const char * szFile)
{
	int iNum = iCount;
	CString strSection, strKey;

	switch(iType)
	{
	case 0:
		strSection = "FILELIST";
		break;
	case 1:
		strSection = "MEDIALIST";
		break;
	default:
		ASSERT(FALSE);
	}

	strKey = "FileNum";

	return WritePrivateProfileStruct(strSection, strKey, &iNum, sizeof(int), szFile);
}

BOOL CGhgCopier::WriteFile()
{
	ASSERT(m_iStep >= 5);

	BOOL bRet = TRUE;
	int iSize = arySrc.GetSize();
	ASSERT(aryDes.GetSize() == iSize && aryCopy.GetSize() == iSize);

	// 去掉相同的数据
	for (int i = 0 ; i < aryDes.GetSize() ; i++)
	{
		for (int j = aryDes.GetSize() - 1; j > i ; j --)
		{
			if (aryDes[j] == aryDes[i]) aryDes.RemoveAt(j);
		}
	}

	// 去掉文件只读属性
	DWORD dw = ::GetFileAttributes(m_strSaveFile);
	if ((dw && FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
	{	dw &= ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(m_strSaveFile, dw);
	}

	// 写入数据到文件
	iSize = aryDes.GetSize();
	WriteListNum(0, iSize , m_strSaveFile);
	int i = 0;
	for (i = 0 ; i < iSize ; i++)
	{	
		if (!WriteFileList(0, aryDes[i], i , m_strSaveFile)) 
			bRet = FALSE;
	}

	aryDes.SetSize(0);
	arySrc.SetSize(0);
	aryCopy.SetSize(0);
	m_iStep = 0;

	return bRet;
}