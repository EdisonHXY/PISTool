#pragma once


//打开控制台
DllExport bool OC_OpenConsole(CString titleStr);

//关闭控制台
DllExport bool OC_CloseConsole();

//输出文字
DllExport void OC_WriteConsole(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

//设置控制台前缀 时间格式
DllExport void OC_SetConsoleOutPattern(CString timeFormatStr);

//设置不同类型下的颜色 FOREGROUND_GREEN
DllExport void OC_SetConsoleColor(WRITETPYE wp, WORD colorValue);
