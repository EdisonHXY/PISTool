#pragma once
#include <stdio.h>
#include <windows.h>
#include <string>
using namespace  std;
class CPipeClient
{
public:
	CPipeClient();
	~CPipeClient();
	typedef void(*PipReveiceCallBack)(char *szBuffer, int nLen, int nErrCode, void *lparma);
	int ConnectPipeServer(string strName);
	void StarReceive(PipReveiceCallBack cb, void *lparam);
	void StopPipe();
	int SendMessageToClient(string strMsg);

	//打开进程
	bool OpenExe(CString exePath);
	//关闭进程
	void CloseExe();

private:
	PipReveiceCallBack m_receiveCB;
	void *m_receiveCBParam;
	HANDLE m_hPipe;
	HANDLE m_hEvent;
	HANDLE m_hThread;
	bool m_bStopCheck;
	HANDLE m_openHandle;
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
};

