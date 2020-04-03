#pragma once
//初始化日志
DllExport int OC_InitLog(int nIndex);

//设置日志类型
DllExport int OC_SetLogType(int nIndex,bool isSingleFile);

//设置保存的位置和名字
DllExport int OC_SetSavePath(int nIndex, CString logDir, CString logName);

//设置文件大小对于单一文件
DllExport int OC_SetFileSizeForSingle(int nIndex, int iSizeKB);

//设置文件个数 一天生成多少个文件，保存多少天
DllExport int OC_SetLogFileCountForMulit(int nIndex, int iDayFileCount, int iLogDayCount);

//输出文字
DllExport int OC_WritLog(int nIndex, const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

//销毁所有日志对象
DllExport void OC_DeletAllLogs();
