#include "stdafx.h"
#include "ConsoleHandle.h"
#include <iostream>

CConsoleHandle::CConsoleHandle()
{
	m_enableWriteConsole = false;

	m_infoColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	m_warnColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	m_errorColor = FOREGROUND_RED | FOREGROUND_INTENSITY;

	m_formatStr = "[%Y-%m-%d %H:%M:%S]";
}


CConsoleHandle::~CConsoleHandle()
{
}
bool CConsoleHandle::OpenConsole(CString titleStr)
{
	bool bRet = AllocConsole();
	if (bRet)
	{
		SetConsoleTitle(titleStr);
	}
	FILE *strem = NULL,*streamold = NULL;
	freopen_s(&strem,"CONOUT$", "w+t", stdout);
	m_enableWriteConsole = true;

	HWND hwnd = NULL;

	while (NULL == hwnd)
	{
		hwnd = ::FindWindow(NULL, titleStr);//查找控制台 
	}

	HMENU   hmenu = ::GetSystemMenu(hwnd, FALSE);//获取菜单
	::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);//移除关闭 防止关闭控制台时 也把主程序关闭了


	return bRet;
}

bool CConsoleHandle::CloseConsole()
{

	fclose(stdout);
	m_enableWriteConsole = false;
	return  FreeConsole();
}

void CConsoleHandle::WriteConsole(const char *szMsg, WRITETPYE wp /*= WRITETPYE_INFO*/)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hOut)
	{
		return;
	}

	switch (wp)
	{
	case WRITETPYE_INFO:
		SetConsoleTextAttribute(hOut, m_infoColor);
		break;
	case WRITETPYE_WARN:
		SetConsoleTextAttribute(hOut, m_warnColor);
		break;
	case WRITETPYE_ERROR:
		SetConsoleTextAttribute(hOut, m_errorColor);
		break;
	default:
		break;
	}
	CTime t = CTime::GetCurrentTime();
	CString preStr = t.Format(m_formatStr);
	CString outStr;
	outStr.Format(_T("%s : %s \n"), preStr, szMsg);
	fwrite(outStr, outStr.GetLength(), 1, stdout);
	fflush(stdout);
}

void CConsoleHandle::SetOutPattern(CString formatStr)
{
	m_formatStr = formatStr;
}

void CConsoleHandle::SetColor(WRITETPYE wp, WORD colorValue)
{
	switch (wp)
	{
	case WRITETPYE_INFO:
		m_infoColor = colorValue;
		break;
	case WRITETPYE_WARN:
		m_warnColor = colorValue;
		break;
	case WRITETPYE_ERROR:
		m_errorColor = m_warnColor;
		break;
	default:
		break;
	}
}


