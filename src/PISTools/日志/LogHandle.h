#pragma once
#include "LogToFile.h"
class CLogHandle
{
public:
	CLogHandle();
	~CLogHandle();

	//初始化
	void InitLogType(bool isSingleFile);

	//设置保存的位置和名字
	void SetSavePath(CString logDir, CString logName);

	//设置文件大小对于单一文件
	void SetFileSizeForSingle(int iSizeKB);
	
	//设置文件个数 一天多少
	void SetLogFileCountForMulit(int iDayFileCount, int iLogDayCount);
	
	//输出文字
	void WritLog(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

private:
	CLogToFile_OC m_log;

};

class CLogHandle_Group
{
public:
	CLogHandle_Group();
	~CLogHandle_Group();

public:
	vector<CLogHandle *> m_list;
	void DeleteAll();
};



