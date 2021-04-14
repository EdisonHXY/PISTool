#include "stdafx.h"
#include "LogManager.h"


CLogManager::CLogManager()
{
	CString dir, name;
	CMFCTools::GetInstance()->GetExeDirAndFileName(dir, name);

	m_logFile.SetSavePath(dir + "//log//",name);

	m_logFile.SetLogFileCountForMulit(6, 30);

}


CLogManager * CLogManager::GetInstance()
{
	static CLogManager lm;
	return &lm;
}

CLogManager::~CLogManager()
{
}

void CLogManager::RecordLog(const string &strLog, LOGTYPE lType /*= LOGTYPE_INFO*/, bool bWritFile /*= true*/, bool bWritConsole /*= true*/)
{
	
	if (bWritFile)
	{
		m_logFile.WritLog(strLog.data(), (CLogHandle::WRITETPYE)lType);
	}

	if (bWritConsole)
	{
		m_lock.lock();
		CMFCTools::GetInstance()->AddToMsgToListBox(strLog.data(), (int)lType);
		m_lock.unLock();
	}
}
