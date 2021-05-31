#include "StdAfx.h"
#include "SQLiteDBManage.h"

#pragma   comment(lib, "sqlite3.lib")

CSQLiteDBManage::CSQLiteDBManage(void)
{
	m_pSQLiteConn = NULL;
	m_iRecordPos = 0;


}

CSQLiteDBManage::~CSQLiteDBManage(void)
{
	ClearFieldValue();
	CloseDB();



}



void CSQLiteDBManage::Lock_Start(void)
{
	m_mutexProcess.lock();
}

void CSQLiteDBManage::Lock_End(void)
{
	m_mutexProcess.unLock();
}


int CSQLiteDBManage::_sql_callback(void *param, int argc, char **argv, char **szColName)
{
	CSQLiteDBManage *pSqlite = (CSQLiteDBManage *)param;
	if(pSqlite != NULL)
		pSqlite->RecvExecuteRet(argc, argv, szColName);

	return 0;
}

void CSQLiteDBManage::RecvExecuteRet(int argc, char **argv, char **szColName)
{
	string strValue, strColName;
    for(int i=0; i<argc; i++) 
    { 
		if(argv[i] == 0)
			strValue = "NULL";
		else
			strValue = argv[i];
		
		strColName = szColName[i];

		AddRecordValue(UTF_82ASCII(strColName).data(), UTF_82ASCII(strValue).data());
    } 
}

void CSQLiteDBManage::AddRecordValue(const char *szColName, const char *szValue)
{
	LPRECORD lpRecord = NULL;
	for(int i = 0; i < (int)m_verFieldValue.size(); i++)
	{
		lpRecord = m_verFieldValue.at(i);
		if(strcmp(lpRecord->szField, szColName) == 0)
		{
			lpRecord->verFieldValue.push_back(szValue);
			return;
		}
	}

	lpRecord = new RECORD;
	strcpy(lpRecord->szField, szColName);
	lpRecord->verFieldValue.push_back(szValue);
	m_verFieldValue.push_back(lpRecord);
	
}

void CSQLiteDBManage::ClearFieldValue(void)
{
	m_iRecordPos = 0;
	for(int i=0; i<(int)m_verFieldValue.size(); i++)
		delete m_verFieldValue.at(i);
	
	m_verFieldValue.clear();
}

void CSQLiteDBManage::InitDBParam(CString strSourceFile, CString strPass)
{
	m_strDBFileName = (char *)(LPCSTR)strSourceFile;
}
	
void CSQLiteDBManage::SetPass(char *szPass)
{
	if(m_pSQLiteConn == NULL)
		return;

	//sqlite3_rekey(m_pSQLiteConn, szPass, strlen(szPass));
}

BOOL CSQLiteDBManage::InitDBAdo(void)
{
	if(m_strDBFileName.size() == 0)
		return FALSE;

	string strCvr = ASCII2UTF_8(m_strDBFileName);

	if(strCvr.size() == 0)
	{
		CString strLog;
		strLog.Format("数据库路径转换失败 %s - %s", m_strDBFileName.data(), strCvr.data());
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "数据库路径转换失败 %s - %s", m_strDBFileName.data(), strCvr.data());
		return FALSE;
	}

	int iRet = sqlite3_open(strCvr.data(), &m_pSQLiteConn);
	if(iRet != SQLITE_OK) 
	{
		m_pSQLiteConn = NULL;
		CString strLog;
		strLog.Format("数据库无法打开 %s - Cur:%s -  %d", m_strDBFileName.data(), strCvr.data(), iRet);
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "数据库无法打开, 软件退出  %s - Cur:%s -  %d", m_strDBFileName.data(), strCvr.data(), iRet);
		
	//	exit(1);

		return FALSE;
	}

	return TRUE;
}


void CSQLiteDBManage::CloseDB(void)
{
	if(m_pSQLiteConn == NULL)
		return;

	if(sqlite3_close(m_pSQLiteConn) != SQLITE_OK)  
    {  
		CString strMsg;
		strMsg.Format("数据库无法关闭，错误代码：%s\n", sqlite3_errmsg(m_pSQLiteConn));
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::CloseDB - %s", strMsg); 
    }  

	m_pSQLiteConn = NULL;
}

BOOL CSQLiteDBManage::QueryDB(CString strSql)
{
	ClearFieldValue();
	return Execute(strSql);
}

BOOL CSQLiteDBManage::QueryDBASCI(const CHAR *lpszSQL)
{
	ClearFieldValue();

	string strSQL = lpszSQL;
	strSQL = ASCII2UTF_8(strSQL);

	return Execute(strSQL.data());


}

BOOL CSQLiteDBManage::QueryDB(const CHAR *lpszFormat, ...)
{
	ClearFieldValue();

	int iPos = 0;
	string strValue;
	int iValue;
	char szValue[512];
	CHAR szBuffer[512];
	memset(szValue, '\0', 512);
	memset(szBuffer, '\0', 512);

	va_list vaList;
	va_start(vaList, lpszFormat);
	
	for(int i=0; i<(int)strlen(lpszFormat); i++)
	{
		szBuffer[iPos ++] = lpszFormat[i];
		strcpy(szValue, szBuffer);
		
		if(i > 0 && lpszFormat[i - 1] == '%')
		{
			switch(lpszFormat[i])
			{
			case 'd':
				iValue = va_arg(vaList, int);
				sprintf(szValue, szBuffer, iValue);
				break;
			case 's':
				strValue = va_arg(vaList, char *);
				//sprintf(szValue, szBuffer, strValue.data());
				sprintf(szValue, szBuffer, ASCII2UTF_8(strValue).data());
				break;
			case 'u':
				iValue = va_arg(vaList, int);
				sprintf(szValue, szBuffer, iValue);
				break;
			default:
				break;
			}

			strcpy(szBuffer, szValue);
			iPos = strlen(szBuffer);
		}
	}

	//_vstprintf(szBuffer, lpszFormat, vaList);
	va_end(vaList);
	return Execute(szBuffer);
}

BOOL CSQLiteDBManage::Execute(const CString &strSql)
{
	if(m_pSQLiteConn == NULL)
		return FALSE;

	char *pszErrMsg = NULL; 

	Lock_Start();
	int iRet = sqlite3_exec(m_pSQLiteConn, (LPCSTR)strSql, _sql_callback, this, &pszErrMsg); 
	Lock_End();

    if(iRet != SQLITE_OK) 
    { 
        fprintf(stderr, "SQL error: %s\n", pszErrMsg); 
        sqlite3_free(pszErrMsg); 
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Execute1 - %s - %s", strSql, pszErrMsg);
		return FALSE;
    } 


	return TRUE;
}

BOOL CSQLiteDBManage::Execute(const CHAR *lpszFormat, ...)
{
	if(m_pSQLiteConn == NULL)
		return FALSE;

	int iPos = 0;
	string strValue;
	int iValue;
	char szValue[3872];
	CHAR szBuffer[3872];
	memset(szValue, '\0', 3872);
	memset(szBuffer, '\0', 3872);

	va_list vaList;
	va_start(vaList, lpszFormat);
	
	for(int i=0; i<(int)strlen(lpszFormat); i++)
	{
		szBuffer[iPos ++] = lpszFormat[i];
		strcpy(szValue, szBuffer);
		
		if(i > 0 && lpszFormat[i - 1] == '%')
		{
			switch(lpszFormat[i])
			{
			case 'd':
				iValue = va_arg(vaList, int);
				sprintf(szValue, szBuffer, iValue);
				break;
			case 's':
				strValue = va_arg(vaList, char *);
				//sprintf(szValue, szBuffer, strValue.data());
				sprintf(szValue, szBuffer, ASCII2UTF_8(strValue).data());
				break;
			case 'u':
				iValue = va_arg(vaList, int);
				sprintf(szValue, szBuffer, iValue);
				break;
			default:
				break;
			}

			strcpy(szBuffer, szValue);
			iPos = strlen(szBuffer);
		}
	}

	va_end(vaList);
	
	char *pszErrMsg = NULL; 

	Lock_Start();
	int iRet = sqlite3_exec(m_pSQLiteConn, szBuffer, _sql_callback, this, &pszErrMsg); 
	Lock_End();

    if(iRet != SQLITE_OK) 
    { 
        fprintf(stderr, "SQL error: %s\n", pszErrMsg); 
        sqlite3_free(pszErrMsg); 
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Execute2 - %s - %s", szBuffer, pszErrMsg);

		return FALSE;
    } 


	return TRUE;	
}

BOOL CSQLiteDBManage::IsEOF(void)
{
	if(m_verFieldValue.size() == 0)
		return TRUE;

	if(m_iRecordPos == m_verFieldValue[0]->verFieldValue.size())
		return TRUE;
	
	return FALSE;
}

void CSQLiteDBManage::ResMoveNext(void)
{
	if(m_verFieldValue.size() != 0)
	{
		if(m_iRecordPos < (int)m_verFieldValue[0]->verFieldValue.size())
			m_iRecordPos ++;
	}
}

void CSQLiteDBManage::ResMoveFirst(void)
{
	m_iRecordPos = 0;
}

void CSQLiteDBManage::ResMoveLast(void)
{
	if(m_verFieldValue.size() != 0)
		m_iRecordPos = m_verFieldValue[0]->verFieldValue.size() - 1;

}

void CSQLiteDBManage::ResMovePrevious(void)
{
	if(m_iRecordPos > 0)
		m_iRecordPos --;
}


CString CSQLiteDBManage::GetStringFieldValue(CString strField)
{
	for(int i=0; i<(int)m_verFieldValue.size(); i++)
	{
		if(strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			return m_verFieldValue[i]->verFieldValue[m_iRecordPos].data();
		}
	}

	return _T("");
}


int CSQLiteDBManage::GetIntFieldValue(CString strField)
{
	for(int i=0; i<(int)m_verFieldValue.size(); i++)
	{
		if(strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			return atoi(m_verFieldValue[i]->verFieldValue[m_iRecordPos].data());
		}
	}

	return 0;
}

UINT CSQLiteDBManage::GetUIntFieldValue(CString strField)
{
	for(int i=0; i<(int)m_verFieldValue.size(); i++)
	{
		if(strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			return String2UINT(m_verFieldValue[i]->verFieldValue[m_iRecordPos]);
		}
	}

	return 0;
}


BOOL CSQLiteDBManage::GetStringFieldValue(CString strField, char *szBuf)
{
	if(szBuf == NULL)
		return FALSE;
	
	for(int i=0; i<(int)m_verFieldValue.size(); i++)
	{
		if(strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			strcpy(szBuf, m_verFieldValue[i]->verFieldValue[m_iRecordPos].data());
			return TRUE;
		}
	}

	return FALSE;
}


CString CSQLiteDBManage::GetStringFieldValue(int iPos, CString strField)
{
	if (iPos < 0 || m_verFieldValue.size() == 0)
		return _T("");

	for (int i = 0; i<(int)m_verFieldValue.size(); i++)
	{
		if (strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			if (iPos < 0 || m_verFieldValue[i]->verFieldValue.size() == 0 || iPos >= (int)m_verFieldValue[i]->verFieldValue.size())
				return _T("");

			return m_verFieldValue[i]->verFieldValue[iPos].data();
		}
	}

	return _T("");
}


int CSQLiteDBManage::GetIntFieldValue(int iPos, CString strField)
{
	for (int i = 0; i<(int)m_verFieldValue.size(); i++)
	{
		if (strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			if (iPos < 0 || m_verFieldValue[i]->verFieldValue.size() == 0 || iPos >= (int)m_verFieldValue[i]->verFieldValue.size())
				return 0;

			return atoi(m_verFieldValue[i]->verFieldValue[iPos].data());
		}
	}

	return 0;
}

UINT CSQLiteDBManage::GetUIntFieldValue(int iPos, CString strField)
{
	for (int i = 0; i<(int)m_verFieldValue.size(); i++)
	{
		if (strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			if (iPos < 0 || m_verFieldValue[i]->verFieldValue.size() == 0 || iPos >= (int)m_verFieldValue[i]->verFieldValue.size())
				return 0;

			return String2UINT(m_verFieldValue[i]->verFieldValue[iPos]);
		}
	}

	return 0;
}

BOOL CSQLiteDBManage::GetStringFieldValue(int iPos, CString strField, char *szBuf)
{
	if (szBuf == NULL)
		return FALSE;

	for (int i = 0; i<(int)m_verFieldValue.size(); i++)
	{
		if (strField.Compare(m_verFieldValue[i]->szField) == 0)
		{
			if (iPos < 0 || m_verFieldValue[i]->verFieldValue.size() == 0 || iPos >= (int)m_verFieldValue[i]->verFieldValue.size())
				return FALSE;

			strcpy(szBuf, m_verFieldValue[i]->verFieldValue[iPos].data());
			return TRUE;
		}
	}

	return FALSE;
}

	
void CSQLiteDBManage::GetAllSchemaName(CStringArray *strArray)
{

}

long CSQLiteDBManage::GetFieldCount(void)
{
	return m_verFieldValue.size();
}

int CSQLiteDBManage::GetRecordsetCount(void)
{
	if(m_verFieldValue.size() == 0)
		return 0;

	return m_verFieldValue[0]->verFieldValue.size();
}


UINT CSQLiteDBManage::String2UINT(const string &strValue)
{
	UINT data=0;
	_stscanf(strValue.data(), _T("%d"),&data);
	return data;
}


// ====================================================================================================
std::wstring CSQLiteDBManage::Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);  
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)  
    {  
        throw std::exception("Invalid UTF-8 sequence.");  
    }  
    if (widesize == 0)  
    {  
        throw std::exception("Error in conversion.");  
    }  
  
    std::vector<wchar_t> resultstring(widesize);  
  
    int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);  
  
    if (convresult != widesize)  
    {  
        throw std::exception("La falla!");  
    }  
  
    return std::wstring(&resultstring[0]);  
}


string CSQLiteDBManage::WideByte2Acsi(wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);  
    if (asciisize == ERROR_NO_UNICODE_TRANSLATION)  
    {  
        throw std::exception("Invalid UTF-8 sequence.");  
    }  
    if (asciisize == 0)  
    {  
        throw std::exception("Error in conversion.");  
    }  
    std::vector<char> resultstring(asciisize);  
    int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);  
  
    if (convresult != asciisize)  
    {  
        throw std::exception("La falla!");  
    }  
  
    return std::string(&resultstring[0]);  	
}

wstring CSQLiteDBManage::Acsi2WideByte(string& strascii)
{
	int codepage = AreFileApisANSI() ? CP_ACP : CP_OEMCP;

	int widesize = MultiByteToWideChar (codepage, 0, (char*)strascii.c_str(), -1, NULL, 0);  
    if (widesize == ERROR_NO_UNICODE_TRANSLATION)  
    {  
        throw std::exception("Invalid UTF-8 sequence.");  
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Acsi2WideByte - conversion size %s", strascii.data());
    }  
    if (widesize == 0)  
    {  
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Acsi2WideByte - conversion size == 0");
        throw std::exception("Error in conversion.");  
    }  
    std::vector<wchar_t> resultstring(widesize);  
    int convresult = MultiByteToWideChar (codepage, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);  
  
    if (convresult != widesize)  
    {  
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Acsi2WideByte - convresult != widesize %s", strascii.data());
        throw std::exception("La falla!");  
    }  
  
    return std::wstring(&resultstring[0]);  
}


std::string CSQLiteDBManage::Unicode2Utf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);  
    if (utf8size == 0)  
    {  
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Unicode2Utf8 - utf8size == 0");
        throw std::exception("Error in conversion.");  
    }  
  
    std::vector<char> resultstring(utf8size);  
  
    int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);  
  
    if (convresult != utf8size)  
    {  
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Unicode2Utf8 - convresult != utf8size");
        throw std::exception("La falla!");  
    }  
  
    return std::string(&resultstring[0]);  	
}

string CSQLiteDBManage::UTF_82ASCII(string &strUtf8Code)
{
	string strRet("");  
  
    //先把 utf8 转为 unicode   
    wstring wstr = Utf82Unicode(strUtf8Code);  
  
    //最后把 unicode 转为 ascii  
    strRet = WideByte2Acsi(wstr);  
  
    return strRet;  
}


string CSQLiteDBManage::ASCII2UTF_8(string &strAsciiCode)
{
	string strRet("");  
  
    //先把 ascii 转为 unicode   
    wstring wstr = Acsi2WideByte(strAsciiCode);  
  
    //最后把 unicode 转为 utf8  
    strRet = Unicode2Utf8(wstr);  
  
    return strRet;  	
}
