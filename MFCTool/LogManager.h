#pragma once
//===============================================================
//Remarks:
//          管理日志
//Date:  2021/4/14
//Author:HXY10
//===============================================================

#include "log/LogHandle.h"
#include "MfcUI/MFCTools.h"
#include "MfcUI/ThreadCheck.h"
class CLogManager
{
public:
	static CLogManager *GetInstance();
	~CLogManager();

	enum LOGTYPE
	{
		LOGTYPE_INFO,
		LOGTYPE_WARNING,
		LOGTYPE_ERROR
	};
	
	void RecordLog(const string &strLog, LOGTYPE lType = LOGTYPE_INFO, bool bWritFile = true, bool bWritConsole = true);


private:
	CLogManager();
	CLogHandle m_logFile;
	CMyLock m_lock;

};

