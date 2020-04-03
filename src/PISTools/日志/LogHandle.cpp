#include "stdafx.h"
#include "LogHandle.h"


CLogHandle::CLogHandle()
{
}


CLogHandle::~CLogHandle()
{
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

}


CLogHandle_Group::CLogHandle_Group()
{
}

CLogHandle_Group::~CLogHandle_Group()
{
}

void CLogHandle_Group::DeleteAll()
{
	for (unsigned int i = 0 ; i < m_list.size();++i)
	{
		CLogHandle *ld = m_list[i];
		if (ld)
		{
			delete ld;
			ld = NULL;
		}
	}
	m_list.clear();
}
