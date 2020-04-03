
#ifndef	__LOGTOFILE_H1__
#define	__LOGTOFILE_H1__

/*
CString strLogDirectory = m_strCurDir + _T("Log\\");
m_logSave.InitLogDirectory(strLogDirectory);
m_logSave.InitLogFile(AfxGetApp()->m_pszExeName);
//m_logSave.InitLogFileCount(5, 7);
m_logSave.SetLogOnly(TRUE);*/



///////////////////////////////////////////////////////////////////////////////////////
class CLogToFile_OC
{
public:
	enum LOGTYPE
	{
		LOG_TYPE_INFO = 0,		// 一般信息
		LOG_TYPE_IMPORTANT,		// 重要信息
		LOG_TYPE_WARNING,		// 警告信息
		LOG_TYPE_ERROR,			// 错误信息
		LOG_TYPE_NETSEND,		// 网络发送信息
		LOG_TYPE_NETRECV,		// 网络接收信息
		LOG_TYPE_NETERROR		// 网络数据错误
	};

public:
	CLogToFile_OC(void);
	~CLogToFile_OC(void);
	void InitLogDirectory(CHAR *szLogDirectory);
	void InitLogDirectory(LPCTSTR pszLogDirectory);
	void InitLogFile(LPCTSTR pszName, BOOL bDelFlag = TRUE);
	void InitLogFile(CHAR *szFile, BOOL bDelFlag = TRUE);
	void SetLogOnly(BOOL bOnlyFile){m_bOnlyFile = bOnlyFile;};
	void InitFileSize(int iSizeKB);
	void InitLogFileCount(int iDayFileCount, int iLogDayCount);
	BOOL SetSaveFlag(LOGTYPE logType, BYTE byFlag);

	void WriteLog(LOGTYPE logType, CHAR *szFunction, CHAR *szMessage);
	void WriteLog(LOGTYPE logType, const CHAR *lpszFormat, ...);
	void WriteLog(LOGTYPE logType, PCHAR szLog);
	void WriteLog(LPCTSTR lpszLog, LOGTYPE logType = LOG_TYPE_INFO);

private:
	void InitLogFile(void);
	CString GetLogFileName(void);
	void ClearLogFile(void);

private:
	DWORD m_dwLogFileSize;
	CHAR m_szLogFile[MAX_PATH];
	BOOL m_bOnlyFile;
	int m_iDayFileCount;
	int m_iLogDayCount;
	HANDLE m_hFile;
	CString m_strCurFile;
	CHAR m_szDirectory[MAX_PATH];
	CTime m_tmClear;
	BOOL m_bDelFlag;
	DWORD m_dwCurPos;
	DWORD m_dwFilePos;

	BYTE m_logSaveFlag[15];
};

#endif


