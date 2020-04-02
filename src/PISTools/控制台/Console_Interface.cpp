#include "stdafx.h"
#include "..\对外接口\Console_Interface.h"
#include "ConsoleHandle.h"

static CConsoleHandle g_consolHandle;
//打开控制台
bool OC_OpenConsole(CString titleStr)
{
	return g_consolHandle.OpenConsole(titleStr);
 }

//关闭控制台
bool OC_CloseConsole()
{
	return g_consolHandle.CloseConsole();
 }

//输出文字
void OC_WriteConsole(const char *szMsg, WRITETPYE wp )
{
	return g_consolHandle.WriteConsole(szMsg,wp);
}

 void OC_SetConsoleOutPattern(CString timeFormatStr)
{
	 return g_consolHandle.SetOutPattern(timeFormatStr);
}

  void OC_SetConsoleColor(WRITETPYE wp, WORD colorValue)
 {
	  return g_consolHandle.SetColor(wp, colorValue);
 }
