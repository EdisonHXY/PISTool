#include "stdafx.h"
#include "LogHandle.h"


CLogHandle::CLogHandle()
{
	InitializeCriticalSection(&m_criSection);  //使用前必须初始化
}


CLogHandle::~CLogHandle()
{
	DeleteCriticalSection(&m_criSection);
}

void CLogHandle::InitLogType(bool isSingleFile)
{
	return m_log.SetLogOnly(isSingleFile);
}

void CLogHandle::SetSavePath(CString logDir, CString logName)
{
	m_log.InitLogDirectory(logDir);
	m_log.InitLogFile(logName);
}

void CLogHandle::SetFileSizeForSingle(int iSizeKB)
{
	m_log.InitFileSize(iSizeKB);
}

void CLogHandle::SetLogFileCountForMulit(int iDayFileCount, int iLogDayCount)
{
	m_log.InitLogFileCount(iDayFileCount, iLogDayCount);
}

void CLogHandle::WritLog(const char *szMsg, WRITETPYE wp /*= WRITETPYE_INFO*/)
{
	EnterCriticalSection(&m_criSection); //表示进入临界区
	CLogToFile_OC::LOGTYPE lp = CLogToFile_OC::LOG_TYPE_INFO;
	switch (wp)
	{
	case WRITETPYE_INFO:
		lp = CLogToFile_OC::LOG_TYPE_INFO;
		break;
	case WRITETPYE_WARN:
		lp = CLogToFile_OC::LOG_TYPE_WARNING;
		break;
	case WRITETPYE_ERROR:
		lp = CLogToFile_OC::LOG_TYPE_ERROR;
		break;
	default:
		break;
	}


	m_log.WriteLog(szMsg, lp);
	LeaveCriticalSection(&m_criSection);
}

void CLogHandle::WriteHexLog(const char *szPreMsg, const BYTE * szMsg, UINT nlen, WRITETPYE wp /*= WRITETPYE_INFO*/)
{
	CString str, tmpStr;
	str = szPreMsg;

	for (int i = 0;i<nlen;++i)
	{
		tmpStr.Format("%x ", szMsg[i]);
		str += tmpStr;
	}

	WritLog(str, wp);
}

