#include "stdafx.h"
#include "ConsoleHandle.h"
#include <iostream>

CConsoleHandle::CConsoleHandle()
{
	m_enableWriteConsole = false;
	InitializeCriticalSection(&m_lock);
	m_infoColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	m_warnColor = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
	m_errorColor = FOREGROUND_RED | FOREGROUND_INTENSITY;

	m_formatStr = "[%Y-%m-%d %H:%M:%S]";
}


CConsoleHandle::~CConsoleHandle()
{
	DeleteCriticalSection(&m_lock);
}
bool CConsoleHandle::OpenConsole(CString titleStr)
{
	EnterCriticalSection(&m_lock);
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
		hwnd = ::FindWindow(NULL, titleStr);//���ҿ���̨ 
	}

	HMENU   hmenu = ::GetSystemMenu(hwnd, FALSE);//��ȡ�˵�
	::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);//�Ƴ��ر� ��ֹ�رտ���̨ʱ Ҳ��������ر���

	LeaveCriticalSection(&m_lock);
	return bRet;
}

bool CConsoleHandle::CloseConsole()
{
	EnterCriticalSection(&m_lock);
	fclose(stdout);
	m_enableWriteConsole = false;
	bool bRet = FreeConsole();
	LeaveCriticalSection(&m_lock);
	return  bRet;
}

void CConsoleHandle::WriteConsole(const char *szMsg, WRITETPYE wp /*= WRITETPYE_INFO*/)
{
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!hOut || !m_enableWriteConsole)
	{
		return;
	}
	EnterCriticalSection(&m_lock);
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
	LeaveCriticalSection(&m_lock);
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


