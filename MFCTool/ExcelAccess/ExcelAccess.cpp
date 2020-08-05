
#include "StdAfx.h"
#include "ExcelAccess.h"

CExcelAccess::CExcelAccess(void)
{
	
}

CExcelAccess::~CExcelAccess(void)
{
	if(m_dbExcel.IsOpen())
		m_dbExcel.Close();
}

void CExcelAccess::CloseExcel(void)
{
	if(m_dbExcel.IsOpen())
		m_dbExcel.Close();
}

BOOL CExcelAccess::CreateExcel(CString strExcelFile)
{
	CString strDriver = GetExcelDriver(), strSQL;
	strSQL.Format(_T("DRIVER={%s};DSN='''';FIRSTROWHASNAMES=1;READONLY=FALSE;CREATE_DB=\"%s\";DBQ=%s"),
		strDriver, strExcelFile, strExcelFile);
	
	TRY
	{
		return m_dbExcel.OpenEx(strSQL, CDatabase::noOdbcDialog);
	}
	CATCH_ALL(e)
	{
		AfxMessageBox("û���ҵ���ӦExcel����");
		TRACE1("Excel����û�а�װ: %s", strDriver);
	}
	END_CATCH_ALL;

	return FALSE;
}

BOOL CExcelAccess::OpenExcel(CString strExcelFile)
{
	CString strDriver = GetExcelDriver();
	if(strDriver.IsEmpty())
	{
		AfxMessageBox("û���ҵ���ӦExcel����");
		m_strFile = _T("");
		return FALSE;
	}
	
	CString strDSN;
	strDSN.Format(_T("ODBC;DRIVER={%s};DSN='';DBQ=%s"), strDriver, strExcelFile);
	
	TRY
	{
		// �����ݿ�(��Excel�ļ�)
		m_dbExcel.Open(NULL, FALSE, FALSE, strDSN);
	}
	CATCH(CDBException, e)
	{
		AfxMessageBox("��Excel����: " + e->m_strError);
	}
	END_CATCH;
	
	int iPos = strExcelFile.ReverseFind('\\') + 1;
	int iEnd = strExcelFile.ReverseFind('.');
	m_strFile = strExcelFile.Mid(iPos, iEnd - iPos);
	return TRUE;
}

int CExcelAccess::GetFieldValue(CStringArray &arryValue, CString strTable, int iFieldIndex)
{
	if(!m_dbExcel.IsOpen())
		return 0;
	
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM [%s]"), strTable + _T("$"));
	CRecordset recset(&m_dbExcel);
	
	if(!recset.Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly))
		return 0;
	
	CString strValue;
	int iTemp = recset.GetRecordCount();
	while(!recset.IsEOF())
	{
		recset.GetFieldValue(iFieldIndex, strValue);
		arryValue.Add(strValue);
		recset.MoveNext();
	}	
	
	return arryValue.GetSize();
}

CString CExcelAccess::GetExcelDriver(void)
{
	TCHAR szBuf[2001];
	WORD wBufMax = 2000;
	WORD wBufOut;
	TCHAR *pszBuf = szBuf;
	CString strDriver;
	
	// ��ȡ�Ѱ�װ����������(������odbcinst.h��)
	if(!SQLGetInstalledDrivers(szBuf, wBufMax, &wBufOut))
		return _T("");
	
	// �����Ѱ�װ�������Ƿ���Excel...
	do
	{
		if(_tcsstr(pszBuf, "Microsoft Excel Driver")/*"Microsoft Excel Driver (*.xls, *.xlsx")*/ != 0)
		{
			strDriver = CString(pszBuf);
			break;
		}
	
		pszBuf = _tcschr(pszBuf, '\0') + 1;
		
	}
	while(pszBuf[1] != '\0');
	
	return strDriver;
}