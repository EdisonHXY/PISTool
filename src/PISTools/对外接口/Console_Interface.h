#pragma once


//�򿪿���̨
DllExport bool OC_OpenConsole(CString titleStr);

//�رտ���̨
DllExport bool OC_CloseConsole();

//�������
DllExport void OC_WriteConsole(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

//���ÿ���̨ǰ׺ ʱ���ʽ
DllExport void OC_SetConsoleOutPattern(CString timeFormatStr);

//���ò�ͬ�����µ���ɫ FOREGROUND_GREEN
DllExport void OC_SetConsoleColor(WRITETPYE wp, WORD colorValue);
