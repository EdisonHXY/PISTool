#pragma once

/************************************************************************/
/*                界面功能                                              */
/************************************************************************/

//获取版本号 
DllExport int OC_GetVersionInfo(CString szExeFile, CString &szVersion); 

//获取程序所在路径 和 程序名
DllExport bool OC_GetExeDirAndFileName(CString &dir, CString &fileName);

//读取配置文件 
DllExport int OC_ReadCfgFromIni(vector<OC_INI_INFO> &listInfo, CString filePath);

//保存配置文件
DllExport int OC_SaveCfgToIni(vector<OC_INI_INFO> &listInfo, CString filePath);

//增加状态栏
DllExport int OC_AddStatueBar(CWnd* pParentWnd, int nParts = 0, int* pWidths = NULL);

//删除状态栏
DllExport int OC_DeleteStatueBar();

//状态栏信息 需要先添加状态栏
DllExport int OC_SetStatueBarText(CString title, int nParts = 0);

//最小化程序到托盘
DllExport int OC_TrayToBottom(HWND hwn, HICON icon, CString tipMsg);

//取消托盘
DllExport int OC_CancelTrayToBottom(HWND hwn);


//关闭弹出提示
DllExport int OC_CloseTip(HWND hwn, CString tipStr);

//取消关闭弹出提示
DllExport void OC_CancelCloseTip(HWND hwn);

//获取当前应用的版本号
DllExport int OC_GetCurrentVersionInfo(CString &szVersion);

//读取当前目录配置文件 
DllExport int OC_ReadCurrentCfgFromIni(vector<OC_INI_INFO> &listInfo, CString fileName);

//保存当前目录配置文件
DllExport int OC_SaveCurrentCfgToIni(vector<OC_INI_INFO> &listInfo, CString fileName);

