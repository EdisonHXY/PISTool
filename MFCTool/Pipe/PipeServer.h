#pragma once

/************************************************************************/
/* 进程间通信                                                           */
/************************************************************************/

#include <stdio.h>
#include <windows.h>
#include <string>
using namespace  std;
class CPipeServer
{
public:
	CPipeServer();
	~CPipeServer();

	typedef void(*PipReveiceCallBack)(char *szBuffer, int nLen,int nErrCode,void *lparma);
	int StartPipeServer(string strPipeName,PipReveiceCallBack cb,void *lparam);
	void StopPipe();
	int SendMessageToClient(string strMsg);
private:
	PipReveiceCallBack m_receiveCB;
	void *m_receiveCBParam;
	HANDLE m_hPipe;
	HANDLE m_hEvent;
	HANDLE m_hThread;
	bool m_bStopCheck;
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
};

