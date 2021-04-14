
#include "StdAfx.h"
#include "LogToFile.h"
#include <io.h>

CLogToFile_OC::CLogToFile_OC(void)
{
	m_dwFilePos = 0;
	m_dwCurPos = 0;
	strcpy_s(m_szLogFile, "Log");
	m_dwLogFileSize = 12582912;

	m_iDayFileCount = 6;
	m_iLogDayCount = 20;
	m_hFile = NULL;
	m_bOnlyFile = FALSE;
	memset(m_logSaveFlag, 0, 15);
	m_logSaveFlag[LOG_TYPE_INFO] = 1;
	m_logSaveFlag[LOG_TYPE_IMPORTANT] = 1;
	m_logSaveFlag[LOG_TYPE_WARNING] = 1;
	m_logSaveFlag[LOG_TYPE_ERROR] = 1;

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, '\\'))[0] = 0; 


	_tcscpy_s(m_szDirectory, szFilePath);
	_tcscat_s(m_szDirectory, "\\");
	//m_szDirectory = szFilePath;
//	m_strDirectory += "\\Log\\";
}

CLogToFile_OC::~CLogToFile_OC(void)
{
	if(m_hFile != NULL)
		CloseHandle(m_hFile);
}

BOOL CLogToFile_OC::SetSaveFlag(LOGTYPE logType, BYTE byFlag)
{
	if(logType < LOG_TYPE_INFO || logType > LOG_TYPE_NETERROR)
		return FALSE;
	
	m_logSaveFlag[logType] = byFlag;

	return TRUE;
}

//==========================================================================================
//	功能:		初始化日志文件句柄，为写入打开文件
//	参数:		无
//	返回:		无
//===========================================================================================
void CLogToFile_OC::InitLogFile(void)
{
	if(m_bOnlyFile)			//单一日志文件
	{
		if(m_hFile == NULL)
		{
			char szFileName[MAX_PATH];
			_stprintf_s(szFileName, "%s%s.log", m_szDirectory, m_szLogFile);
			m_hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if(m_dwCurPos <= 0)
			{
				m_dwFilePos = 0;
				SetFilePointer(m_hFile, 0, 0, FILE_END);
			}
			else
			{
				SetFilePointer(m_hFile, m_dwFilePos, 0, FILE_CURRENT);
			}		
		}

		if(INVALID_HANDLE_VALUE == m_hFile)
			return;

		DWORD dwFileSize = GetFileSize(m_hFile, NULL);

		if(dwFileSize > m_dwLogFileSize)
		{
			DWORD dwNowPos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT); 
			if(dwNowPos >= m_dwCurPos)
			{
				m_dwFilePos = 0;
				m_dwCurPos = dwNowPos;
				SetFilePointer(m_hFile, 0, 0, FILE_BEGIN);
				dwNowPos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT); 
				
			}	
		}
		else
			m_dwCurPos = 0;
	}
	else
	{
		CString strFileName = GetLogFileName();
		if(m_strCurFile.Compare(strFileName) != 0)
		{
			if(m_hFile != NULL)
				CloseHandle(m_hFile);
			m_strCurFile = strFileName;
			strFileName.Insert(0, m_szDirectory);
			m_hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
								NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			SetFilePointer(m_hFile, 0, 0, FILE_END);
		}
		else if(m_hFile == NULL)
		{
			strFileName.Insert(0, m_szDirectory);
			m_hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
				NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			SetFilePointer(m_hFile, 0, 0, FILE_END);
		}

		if(INVALID_HANDLE_VALUE == m_hFile)
			return;

		ClearLogFile();
	}
}

//==========================================================================================
//	功能:		获取文件名，每次写入时调用。用于每天记录多个日志文件
//	参数:		无
//	返回:		无
//===========================================================================================
CString CLogToFile_OC::GetLogFileName(void)
{
	CTime time = CTime::GetCurrentTime();
	CString strFile;

	if (time.GetHour() == 0 || m_iDayFileCount == 0)
	{
		strFile.Format("%s-%d%s", time.Format("%Y.%m.%d"), 0, ".log");

	}
	else
	{
		strFile.Format("%s-%d%s",time.Format("%Y.%m.%d"), time.GetHour() / (24 / max(m_iDayFileCount, 1)), ".log");

	}
	return strFile;
}

//==========================================================================================
//	功能:		初始化日志文件目录
//	参数:		szLogDirectory: 文件路径
//	返回:		无
//===========================================================================================
void CLogToFile_OC::InitLogDirectory(CHAR *szLogDirectory)
{
	if(szLogDirectory != NULL)
		_tcscpy_s(m_szDirectory, szLogDirectory);

	if(_taccess(szLogDirectory, 0) != 0)
	{
		::CreateDirectory(szLogDirectory, NULL);
	}
}

void CLogToFile_OC::InitLogDirectory(LPCTSTR pszLogDirectory)
{
	if(pszLogDirectory != NULL)
		_tcscpy_s(m_szDirectory, pszLogDirectory);

	if(_taccess(pszLogDirectory, 0) != 0)
	{
		::CreateDirectory(pszLogDirectory, NULL);
	}
}

//==========================================================================================
//	功能:		初始化日志文件名
//	参数:		szFile: 记录单一文件日志时用于日志名
//	返回:		无
//===========================================================================================
void CLogToFile_OC::InitLogFile(CHAR *szFile, BOOL bDelFlag)
{
	if(szFile != NULL)
		_tcscpy_s(m_szLogFile, szFile);

	m_bDelFlag = bDelFlag;
}

void CLogToFile_OC::InitLogFile(LPCTSTR pszName, BOOL bDelFlag)
{
	if(pszName != NULL)
		_tcscpy_s(m_szLogFile, pszName);

	m_bDelFlag = bDelFlag;
}

//==========================================================================================
//	功能:		初始化日志文件大小
//	参数:		szFile: 记录单一文件日志时限制文件大小
//	返回:		无
//===========================================================================================
void CLogToFile_OC::InitFileSize(int iSizeKB)
{
	if(iSizeKB > 1000 && iSizeKB < 30720)
		m_dwLogFileSize = iSizeKB;
}

//==========================================================================================
//	功能:		初始化每天日志数量及保存天数
//	参数:		iDayFileCount: 对于每天保存多个文件时标志日志数量， iLogDayCount: 日志保存天数
//	返回:		无
//===========================================================================================
void CLogToFile_OC::InitLogFileCount(int iDayFileCount, int iLogDayCount)
{
	if(iDayFileCount > 0 && iDayFileCount < 25)
		m_iDayFileCount = iDayFileCount;
	if(iLogDayCount > 0 && iLogDayCount < 366)
		m_iLogDayCount = iLogDayCount;
}


void CLogToFile_OC::WriteLog(LOGTYPE logType, PCHAR szLog)
{
	if(logType < LOG_TYPE_INFO || logType > LOG_TYPE_NETERROR || m_logSaveFlag[logType] == 0)
		return;

	InitLogFile();
	if(m_hFile == NULL)
		return;
	
	CString strLog;
	CTime timeCur = CTime::GetCurrentTime();

	switch(logType)
	{
	case LOG_TYPE_ERROR:
		strLog.Format("Error > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_INFO:
		strLog.Format("Info > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_IMPORTANT:
		strLog.Format("Important > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_WARNING:
		strLog.Format("Warning > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_NETSEND:
		strLog.Format("Send	> [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_NETRECV:
		strLog.Format("Recv > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	case LOG_TYPE_NETERROR:
		strLog.Format("NetErr > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	default:
		strLog.Format("Msg > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szLog);
		break;
	}

	DWORD dwWritten;
	WriteFile(m_hFile, strLog, strLog.GetLength(), &dwWritten, NULL);

	m_dwFilePos += strLog.GetLength();		

	if(m_bDelFlag)
	{
		if(m_hFile != NULL)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

void CLogToFile_OC::WriteLog(LPCTSTR lpszLog, LOGTYPE logType)
{
	if(logType < LOG_TYPE_INFO || logType > LOG_TYPE_NETERROR || m_logSaveFlag[logType] == 0)
		return;

	InitLogFile();
	if(m_hFile == NULL)
		return;
	
	CString strLog;
	CTime timeCur = CTime::GetCurrentTime();

	switch(logType)
	{
	case LOG_TYPE_ERROR:
		strLog.Format("Error > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_INFO:
		strLog.Format("Info > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_IMPORTANT:
		strLog.Format("Important > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_WARNING:
		strLog.Format("Warning > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_NETSEND:
		strLog.Format("Send > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_NETRECV:
		strLog.Format("Recv > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	case LOG_TYPE_NETERROR:
		strLog.Format("NetErr > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	default:
		strLog.Format(" Msg > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), lpszLog);
		break;
	}

	DWORD dwWritten;
	WriteFile(m_hFile, strLog, strLog.GetLength(), &dwWritten, NULL);

	m_dwFilePos += strLog.GetLength();
	if(m_bDelFlag)
	{
		if(m_hFile != NULL)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

void CLogToFile_OC::WriteLog(LOGTYPE logType, CHAR *szFunction, CHAR *szMessage)
{
	if(logType < LOG_TYPE_INFO || logType > LOG_TYPE_NETERROR || m_logSaveFlag[logType] == 0)
		return;

	InitLogFile();
	if(m_hFile == NULL)
		return;
	
	CString strLog;
	CTime timeCur = CTime::GetCurrentTime();

	switch(logType)
	{
	case LOG_TYPE_ERROR:
		strLog.Format("Error > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"),  szFunction, szMessage);
		break;
	case LOG_TYPE_INFO:
		strLog.Format("Info > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	case LOG_TYPE_IMPORTANT:
		strLog.Format("Important > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	case LOG_TYPE_WARNING:
		strLog.Format("Warning > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	case LOG_TYPE_NETSEND:
		strLog.Format("Send > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	case LOG_TYPE_NETRECV:
		strLog.Format("Recv > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	case LOG_TYPE_NETERROR:
		strLog.Format("NetErr > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szFunction, szMessage);
		break;
	default:
		strLog.Format(" Msg > [%s] <%s-%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"),  szFunction, szMessage);
		break;
	}

	DWORD dwWritten;
	WriteFile(m_hFile, strLog, strLog.GetLength(), &dwWritten, NULL);

	m_dwFilePos += strLog.GetLength(); 

	if(m_bDelFlag)
	{
		if(m_hFile != NULL)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

void CLogToFile_OC::WriteLog(LOGTYPE logType, const CHAR *lpszFormat, ...)
{
	if(logType < LOG_TYPE_INFO || logType > LOG_TYPE_NETERROR || m_logSaveFlag[logType] == 0)
		return;

	InitLogFile();
	if(m_hFile == NULL)
		return;
	
	CHAR szBuffer[512];
	CString strLog;
	CTime timeCur = CTime::GetCurrentTime();

	va_list vaList;
	va_start(vaList, lpszFormat);
	_vstprintf_s(szBuffer, lpszFormat, vaList);
	va_end(vaList);

	switch(logType)
	{
	case LOG_TYPE_ERROR:
		strLog.Format("Error > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_INFO:
		strLog.Format("Info > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_IMPORTANT:
		strLog.Format("Important > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_WARNING:
		strLog.Format("Warning > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_NETSEND:
		strLog.Format("Send > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_NETRECV:
		strLog.Format("Recv > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	case LOG_TYPE_NETERROR:
		strLog.Format("NetErr > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	default:
		strLog.Format(" Msg > [%s] <%s> \r\n", timeCur.Format("%Y-%m-%d %H:%M:%S"), szBuffer);
		break;
	}

	DWORD dwWritten;
	WriteFile(m_hFile, strLog, strLog.GetLength(), &dwWritten, NULL);

	m_dwFilePos += strLog.GetLength();

	if(m_bDelFlag)
	{
		if(m_hFile != NULL)
			CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

//==========================================================================================
//	功能:		清除超出保存天数的日志
//	参数:		无
//	返回:		无
//===========================================================================================
void CLogToFile_OC::ClearLogFile(void)
{
	CTime timeCur = CTime::GetCurrentTime();
	CTimeSpan tmSpan = timeCur - m_tmClear;
	if(tmSpan.GetDays() == 0)
		return;

	m_tmClear = timeCur;
	CFileFind fileFind;
	CString strFindFile;
	strFindFile.Format("%s*.log", m_szDirectory);

	if(fileFind.FindFile(strFindFile)) 
	{
		while(fileFind.FindNextFile())
		{
			CString strFile = fileFind.GetFileName();
			strFile.Remove('.');
			strFile = strFile.Left(8);
			CString strYear = strFile.Left(4), strMonth = strFile.Mid(4, 2), strDay = strFile.Right(2);
			int iYear = _ttoi(strYear.GetBuffer(strYear.GetLength()));
			int iMonth = _ttoi(strMonth.GetBuffer(strMonth.GetLength()));
			int iDay = _ttoi(strDay.GetBuffer(strDay.GetLength()));
			strYear.ReleaseBuffer(strYear.GetLength());
			strMonth.ReleaseBuffer(strMonth.GetLength());
			strDay.ReleaseBuffer(strDay.GetLength());
			
			if(iYear < 1979 || iYear > 2050 || iMonth <= 0 || iMonth > 12 || iDay <= 0 || iDay > 31)
				continue;

			CTime timeFile(iYear, iMonth, iDay, timeCur.GetHour(), timeCur.GetMinute(), timeCur.GetSecond());
			tmSpan = timeCur - timeFile;
			if(tmSpan.GetDays() > m_iLogDayCount)
				DeleteFile(fileFind.GetFilePath());
		}
	}

	fileFind.Close();
}