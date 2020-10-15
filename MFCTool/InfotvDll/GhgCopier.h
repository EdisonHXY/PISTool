// GhgCopier.h: interface for the CGhgCopier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GHGCOPIER_H__B2D89335_00DC_4645_BD08_DA9D63ED4EAC__INCLUDED_)
#define AFX_GHGCOPIER_H__B2D89335_00DC_4645_BD08_DA9D63ED4EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGhgCopier  
{
public:
	CGhgCopier();
	virtual ~CGhgCopier();

// Attributes
public:
private:
	BOOL m_bSame;
	CString m_strSaveFile, m_strOldPath, m_strNewPath ; 
	GHG_PARAM  * m_pParam ;
	char * szError;

	CStringArray arySrc, aryDes;
	CUIntArray aryCopy;

	int m_iStep;

// Operations
public:
	BOOL Setup(GHG_PARAM *pParam);
	VOID AddFile(LPCTSTR strFile, BOOL bCopy = TRUE);
	VOID PrepareCopy();
	BOOL Verify();
	BOOL Copy();
	BOOL WriteFile();

	int GetArrayIndex() { ASSERT( m_iStep >= 1); return arySrc.GetSize(); }
	CString GetNewFile(int iIndex)	{ASSERT(m_iStep == 5); return aryDes[iIndex]; }

	CStringArray& GetDstArr() { return aryDes; }

private:
	BOOL WriteFileList(const int iType, const char * szFileName, const int iIndex, const char * szFile);
	BOOL WriteListNum(const int iType, const int iCount, const char * szFile);
	CString MakeNewName(LPCTSTR strFile);
};

#endif // !defined(AFX_GHGCOPIER_H__B2D89335_00DC_4645_BD08_DA9D63ED4EAC__INCLUDED_)
