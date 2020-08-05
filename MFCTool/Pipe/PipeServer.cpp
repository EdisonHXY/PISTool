#include "stdafx.h"
#include "PipeServer.h"


CPipeServer::CPipeServer()
{
	m_hPipe = INVALID_HANDLE_VALUE;
	m_hEvent = NULL;
	m_hThread = NULL;
	m_bStopCheck = true;
	m_receiveCB = NULL;
	m_receiveCBParam = NULL;
}


CPipeServer::~CPipeServer()
{
}

int CPipeServer::StartPipeServer(string strPipeName, PipReveiceCallBack cb, void *lparam)
{
	m_receiveCB = cb;
	m_receiveCBParam = lparam;

	if (m_hPipe != INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	string preStr = "\\\\.\\Pipe\\";
	preStr += strPipeName;
	m_hPipe = CreateNamedPipe(
		preStr.data(),						//管道名
		PIPE_ACCESS_DUPLEX,									//管道类型 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,	//管道参数
		PIPE_UNLIMITED_INSTANCES,							//管道能创建的最大实例数量
		0,													//输出缓冲区长度 0表示默认
		0,													//输入缓冲区长度 0表示默认
		NMPWAIT_WAIT_FOREVER,								//超时时间
		NULL);

	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		printf("Create Pipe Error(%d)\n", GetLastError());
		return -1;
	}
	m_bStopCheck = false;
	m_hEvent = CreateEvent(false, false, NULL, NULL);
	m_hThread =  CreateThread(NULL, 0, ThreadProc, this, 0, NULL);

	return 0;
}

void CPipeServer::StopPipe()
{
	m_bStopCheck = true;
	SetEvent(m_hEvent);

	WaitForSingleObject(m_hEvent, 5000);
	m_hThread = NULL;

	if (m_hPipe !=INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
	}
}

int CPipeServer::SendMessageToClient(string strMsg)
{
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	DWORD wlen = 0;
	bool bRet = WriteFile(m_hPipe, strMsg.data(), strMsg.length(), &wlen, 0);	//向客户端发送内容
	return bRet ? 0 : -2;
}

DWORD WINAPI CPipeServer::ThreadProc(LPVOID lpParameter)
{
	CPipeServer *ser = (CPipeServer*)lpParameter;
	if (ser->m_hPipe == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	if (ConnectNamedPipe(ser->m_hPipe, NULL) == NULL)	//阻塞等待客户端连接。
	{
		printf("Connection failed!\n");
		return -2;
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
			ser->m_receiveCB(buf, rlen,0, ser->m_receiveCBParam);
		}

		WaitForSingleObject(ser->m_hEvent, 500);
		
	}
	SetEvent(ser->m_hEvent);
	return 0;
}
