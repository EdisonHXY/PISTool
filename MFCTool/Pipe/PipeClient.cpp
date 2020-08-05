#include "stdafx.h"
#include "PipeClient.h"


CPipeClient::CPipeClient()
{
	m_hPipe = INVALID_HANDLE_VALUE;
	m_hEvent = NULL;
	m_hThread = NULL;
	m_bStopCheck = true;
	m_receiveCB = NULL;
	m_receiveCBParam = NULL;
}


CPipeClient::~CPipeClient()
{
}

int CPipeClient::ConnectPipeServer(string strName)
{
	
	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	string preStr = "\\\\.\\Pipe\\";
	preStr += strName;
	BOOL bRet = WaitNamedPipe(preStr.data(), NMPWAIT_WAIT_FOREVER);

	if (!bRet)
	{
		printf("connect the namedPipe failed!\n");
		return -1;
	}

	m_hPipe = CreateFile(			//管道属于一种特殊的文件
		preStr.data(),	//创建的文件名
		GENERIC_READ | GENERIC_WRITE,	//文件模式
		0,								//是否共享
		NULL,							//指向一个SECURITY_ATTRIBUTES结构的指针
		OPEN_EXISTING,					//创建参数
		FILE_ATTRIBUTE_NORMAL,			//文件属性(隐藏,只读)NORMAL为默认属性
		NULL);							//模板创建文件的句柄

	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		printf("open the exit pipe failed!\n");
		return -2;
	}


	return 0;
}

void CPipeClient::StarReceive(PipReveiceCallBack cb, void *lparam)
{
	m_receiveCB = cb;
	m_receiveCBParam = lparam;
	m_bStopCheck = false;
	m_hEvent = CreateEvent(false, false, NULL, NULL);
	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);

}

void CPipeClient::StopPipe()
{
	m_bStopCheck = true;
	SetEvent(m_hEvent);

	WaitForSingleObject(m_hEvent, 5000);
	m_hThread = NULL;

	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
}

int CPipeClient::SendMessageToClient(string strMsg)
{
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	DWORD wlen = 0;
	bool bRet = WriteFile(m_hPipe, strMsg.data(), strMsg.length(), &wlen, 0);	//向客户端发送内容
	return bRet ? 0 : -2;
}


bool CPipeClient::OpenExe(CString exePath)
{
	m_openHandle = NULL;
	//创建进程
	STARTUPINFO startupInfo = { sizeof(startupInfo) };					//进程信息	
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;	//使用wSHOWWIndows成员
	startupInfo.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pinfo;
	if (!CreateProcess(exePath.GetBuffer(), NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &startupInfo, &pinfo))
	{
		//创建进程错误
		return false;
	}
	m_openHandle = pinfo.hProcess;

	return true;
}

void CPipeClient::CloseExe()
{
	if (m_openHandle)
	{
		TerminateProcess(m_openHandle, -1);
	}
}

DWORD WINAPI CPipeClient::ThreadProc(LPVOID lpParameter)
{
	CPipeClient *ser = (CPipeClient*)lpParameter;
	if (ser->m_hPipe == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	printf("Connection Success!\n");
	char buf[4096] = "";
	DWORD rlen = 0;
	while (!ser->m_bStopCheck)
	{
		if (ReadFile(ser->m_hPipe, buf, 4095, &rlen, NULL) == FALSE) //接受客户端发送过来的内容
		{
			CloseHandle(ser->m_hPipe);
			ser->m_hPipe = INVALID_HANDLE_VALUE;
			printf("Read Data From Pipe Failed!\n");

			if (ser->m_receiveCB)
			{
				ser->m_receiveCB("", 0, -1, ser->m_receiveCBParam);
			}


			break;
		}

		if (ser->m_receiveCB)
		{
			ser->m_receiveCB(buf, rlen, 0, ser->m_receiveCBParam);
		}

		WaitForSingleObject(ser->m_hEvent, 500);

	}
	SetEvent(ser->m_hEvent);
	return 0;
}