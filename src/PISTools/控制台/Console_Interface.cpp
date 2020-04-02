#include "stdafx.h"
#include "..\����ӿ�\Console_Interface.h"
#include "ConsoleHandle.h"

static CConsoleHandle g_consolHandle;
//�򿪿���̨
bool OC_OpenConsole(CString titleStr)
{
	return g_consolHandle.OpenConsole(titleStr);
 }

//�رտ���̨
bool OC_CloseConsole()
{
	return g_consolHandle.CloseConsole();
 }

//�������
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
