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

	m_hPipe = CreateFile(			//�ܵ�����һ��������ļ�
		preStr.data(),	//�������ļ���
		GENERIC_READ | GENERIC_WRITE,	//�ļ�ģʽ
		0,								//�Ƿ���
		NULL,							//ָ��һ��SECURITY_ATTRIBUTES�ṹ��ָ��
		OPEN_EXISTING,					//��������
		FILE_ATTRIBUTE_NORMAL,			//�ļ�����(����,ֻ��)NORMALΪĬ������
		NULL);							//ģ�崴���ļ��ľ��

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
	bool bRet = WriteFile(m_hPipe, strMsg.data(), strMsg.length(), &wlen, 0);	//��ͻ��˷�������
	return bRet ? 0 : -2;
}


bool CPipeClient::OpenExe(CString exePath)
{
	m_openHandle = NULL;
	//��������
	STARTUPINFO startupInfo = { sizeof(startupInfo) };					//������Ϣ	
	startupInfo.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;	//ʹ��wSHOWWIndows��Ա
	startupInfo.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pinfo;
	if (!CreateProcess(exePath.GetBuffer(), NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &startupInfo, &pinfo))
	{
		//�������̴���
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
		if (ReadFile(ser->m_hPipe, buf, 4095, &rlen, NULL) == FALSE) //���ܿͻ��˷��͹���������
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