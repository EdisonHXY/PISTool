#include "StdAfx.h"
#include "SQLiteDBManage.h"

#pragma   comment(lib, "sqlite3.lib")

CSQLiteDBManage::CSQLiteDBManage(void)
{
	m_pSQLiteConn = NULL;
}

CSQLiteDBManage::~CSQLiteDBManage(void)
{
	ClearFieldValue();
	CloseDB();
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
	m_verFieldValue[szColName].push_back(szValue);

}

void CSQLiteDBManage::ClearFieldValue(void)
{
	m_mutexProcess.lock();
	m_verFieldValue.clear();
	m_mutexProcess.unLock();
}



BOOL CSQLiteDBManage::InitDBAdo(const string &strSourceFile)
{
	if(strSourceFile.size() == 0)
		return FALSE;

	string strCvr = ASCII2UTF_8(strSourceFile.data());

	if(strCvr.size() == 0)
	{
		return FALSE;
	}

	int iRet = sqlite3_open(strCvr.data(), &m_pSQLiteConn);
	if(iRet != SQLITE_OK) 
	{
		m_pSQLiteConn = NULL;
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


BOOL CSQLiteDBManage::QueryDBASCI(const CHAR *lpszSQL)
{
	ClearFieldValue();

	string strSQL = lpszSQL;
	strSQL = ASCII2UTF_8(strSQL);

	return Execute(strSQL.data());


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

	m_mutexProcess.lock();
	int iRet = sqlite3_exec(m_pSQLiteConn, szBuffer, _sql_callback, this, &pszErrMsg); 
	m_mutexProcess.unLock();

    if(iRet != SQLITE_OK) 
    { 
        fprintf(stderr, "SQL error: %s\n", pszErrMsg); 
        sqlite3_free(pszErrMsg); 
		//glLogSave.WriteLog(LOG_TYPE_ERROR, "CSQLiteDBManage::Execute2 - %s - %s", szBuffer, pszErrMsg);
		CLogManager::GetInstance()->RecordLog(pszErrMsg,CLogManager::LOGTYPE_ERROR);
		return FALSE;
    } 


	return TRUE;	
}

void CSQLiteDBManage::GetFieldValueMap(DBRECORDMAP &dbMap, UINT &nRecordCount)
{
	m_mutexProcess.lock();
	nRecordCount = 0;
	dbMap = m_verFieldValue;
	if (m_verFieldValue.size() > 0 )
	{
		nRecordCount = m_verFieldValue.begin()->second.size();
	}
	m_mutexProcess.unLock();
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


string CSQLiteDBManage::WideByte2Acsi(const wstring& wstrcode)
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

wstring CSQLiteDBManage::Acsi2WideByte(const string& strascii)
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

CString CSQLiteDBManage::GetStringFieldValue(int iPos,const string& strField, DBRECORDMAP &recordMap)
{
	if (iPos <  recordMap[strField].size())
	{
		return recordMap[strField][iPos].data();
	}

	return "";
}

BOOL CSQLiteDBManage::GetStringFieldValue(int iPos, const string& strField, char *szBuf, DBRECORDMAP &recordMap)
{
	if (szBuf == 0)
	{
		return FALSE;
	}

	if (iPos < recordMap[strField].size())
	{
		strcpy(szBuf,recordMap[strField][iPos].data());
		return TRUE;
	}

	return false;
}

int CSQLiteDBManage::GetIntFieldValue(int iPos, const string& strField, DBRECORDMAP &recordMap)
{
	if (iPos < recordMap[strField].size())
	{
		return atoi(recordMap[strField][iPos].data());
	}

	return 0;
}

string CSQLiteDBManage::UTF_82ASCII(const string &strUtf8Code)
{
	string strRet("");  
  
    //先把 utf8 转为 unicode   
    wstring wstr = Utf82Unicode(strUtf8Code);  
  
    //最后把 unicode 转为 ascii  
    strRet = WideByte2Acsi(wstr);  
  
    return strRet;  
}


string CSQLiteDBManage::ASCII2UTF_8(const string &strAsciiCode)
{
	string strRet("");  
  
    //先把 ascii 转为 unicode   
    wstring wstr = Acsi2WideByte(strAsciiCode);  
  
    //最后把 unicode 转为 utf8  
    strRet = Unicode2Utf8(wstr);  
  
    return strRet;  	
}
