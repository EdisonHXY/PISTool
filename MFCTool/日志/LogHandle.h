#pragma once
#include "LogToFile.h"
#include <vector>
using namespace std;

class CLogHandle
{
public:
	CLogHandle();
	~CLogHandle();

	enum WRITETPYE
	{
		WRITETPYE_INFO, //正常信息
		WRITETPYE_WARN, //警告信息
		WRITETPYE_ERROR //错误信息
	};
	//初始化
	void InitLogType(bool isSingleFile);

	//设置保存的位置和名字
	void SetSavePath(CString logDir, CString logName);

	//设置文件大小对于单一文件
	void SetFileSizeForSingle(int iSizeKB);
	
	//设置文件个数                一天保存多少个日志   日志保存天数
	void SetLogFileCountForMulit(int iDayFileCount, int iLogDayCount);
	
	//输出文字
	void WritLog(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

	//输出16进制
	void WriteHexLog(const char *szPreMsg,const BYTE * szMsg, UINT nlen, WRITETPYE wp = WRITETPYE_INFO);

private:
	CLogToFile_OC m_log;
	CRITICAL_SECTION m_criSection;
};




