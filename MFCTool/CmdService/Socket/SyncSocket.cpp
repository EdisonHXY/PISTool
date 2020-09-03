/*******************************************************************************
*	��Ȩ����(c)	2009
*	�ļ�����:	SyncSocket.cpp
*	�ļ�˵��:	Socket ͨ������ʵ���ļ�
*	��ǰ�汾:	1.0.20150812
*	����:		��Ρ��
*	��������:	2009-12-16
*******************************************************************************/
#include "StdAfx.h"
#include "SyncSocket.h"

#pragma comment(lib, "wsock32.lib")
//////////////////////////////////////////////////////////////////////////////
CSyncSocket::CSyncSocket(void)
{
	WSADATA wsaData;
	int iResult = WSAStartup(/*MAKEWORD(1, 1)*/0x0202, &wsaData);		//��ʼ��
	if(iResult != 0)
	{
		AfxMessageBox("Socket����ʧ��");
		return;
	}
	m_socketFile = NULL;
	m_hEnentWnd = NULL;
	m_nSockMsg = 0;
	m_hSocket = INVALID_SOCKET;
	m_iTimeOutConnect = 5000;			//���ӳ�ʱ
	m_iTimeOutSend = 0;
	m_iTimeOutReceive = 0;

	funAccept = NULL;
	funReceive = NULL;

	m_bAcceptThread = FALSE;
	m_bReceiveThread = FALSE;
}

CSyncSocket::~CSyncSocket(void)
{
	// ����Accept�߳� ---------------------------------
	if(m_bAcceptThread)
	{
		m_bAcceptThread = FALSE;
		if(::WaitForSingleObject(m_hAcceptThread, 3000) != WAIT_OBJECT_0)
			::TerminateThread(m_hAcceptThread, 0);
 	
		CloseHandle(m_hAcceptThread);
	}
	
	// ���ٽ����߳� ---------------------------------
	if(m_bReceiveThread)
	{
		m_bReceiveThread = FALSE;
		if(::WaitForSingleObject(m_hReceiveThread, 3000) != WAIT_OBJECT_0)
			::TerminateThread(m_hReceiveThread, 0);
		CloseHandle(m_hReceiveThread);
	}

	if(m_hSocket != INVALID_SOCKET)
		Close();

//	WSACleanup();
}

void CSyncSocket::SetNoBlock(void)
{
	if(m_hSocket != INVALID_SOCKET)
	{
		ULONG ulNoBlock = 1;
		ioctlsocket(m_hSocket, FIONBIO, &ulNoBlock);
	}
}

//==================================================================================
//		����:	��ʼ��������socket
//		����:	nSocketPort: �˿�; iSocketType: Sokcet����; lpszSocketAddress: ����IP
//		����:	�ɹ�: TRUE; ʧ��: FALSE
//==================================================================================
BOOL CSyncSocket::Create(UINT nSocketPort, int iSocketType, const char*  lpszSocketAddress)
{
	ASSERT(m_hSocket == INVALID_SOCKET);

//	WSADATA wsaData;
//	int iResult = WSAStartup(/*MAKEWORD(1, 1)*/0x0202, &wsaData);		//��ʼ��
//	if(iResult != 0)
//	{
//		return FALSE;
//	}

	m_hSocket = socket(AF_INET, iSocketType, 0);		//socket����
	if(m_hSocket != INVALID_SOCKET)
	{
//		ULONG ulNoBlock = 1;
//		ioctlsocket(m_hSocket, FIONBIO, &ulNoBlock);

		if(nSocketPort == 0)
			return TRUE;

		if(Bind(nSocketPort, lpszSocketAddress))		//����˿ںϷ�����а�
			return TRUE;
		
		Close();
	}

	
	
	return FALSE;
}

//==================================================================================
//		����:	�˿ڰ�
//		����:	nSocketPort: �˿�; lpszSocketAddress: ����IP
//		����:	�ɹ�: TRUE; ʧ��: FALSE
//==================================================================================
BOOL CSyncSocket::Bind(UINT nSocketPort, const char* lpszSocketAddress)
{
	if(nSocketPort == 0)
		return TRUE;
	
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	if(lpszSocketAddress == NULL)	
		sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{	

		DWORD lResult = inet_addr(lpszSocketAddress);
		if(lResult == INADDR_NONE)
		{	
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}

		sockAddr.sin_addr.s_addr = lResult;
	}
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(nSocketPort);

	if(bind(m_hSocket, (struct sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)	//��
	{
		return FALSE;
	}

	return TRUE;
}

//==================================================================================
//		����:	�˿ڼ���
//		����:	nBackLog: ͬʱ��������
//		����:	�ɹ�: TRUE; ʧ��: FALSE
//==================================================================================
BOOL CSyncSocket::Listen(UINT nBackLog)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	if(listen(m_hSocket, nBackLog) == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}

//==================================================================================
//		����:	����
//		����:	lpszHostAddress: ����Զ��������ַ; nHostPort: Զ�������˿�
//		����:	�ɹ�: TRUE; ʧ��: FALSE
//==================================================================================
BOOL CSyncSocket::Connect(const char*  lpszHostAddress, UINT nHostPort)
{
	ASSERT(lpszHostAddress != NULL);

	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);

	if(sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(lpszHostAddress);
		if(lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError(WSAEINVAL);
			return FALSE;
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);

	return Connect(sockAddr, sizeof(sockAddr));
}

BOOL CSyncSocket::Connect(SOCKADDR_IN &sockAddr, int iSockAddrLen)
{
	FD_SET writefds;
	TIMEVAL tmVal;
	ULONG uNoBlock = 1;		//�����������ģʽ�����
	int iResult, iError;
	BOOL bResult = FALSE;

	if(m_iTimeOutConnect == 0)
	{	
		iResult = connect(m_hSocket, (SOCKADDR*)&sockAddr, iSockAddrLen);
		if(iResult != SOCKET_ERROR || GetLastError() == WSAEISCONN)
			return TRUE;

		return FALSE;
	}

	//���������ӳ�ʱʱ�� -------------------------------------
	ioctlsocket(m_hSocket, FIONBIO, &uNoBlock);
	iResult = connect(m_hSocket, (SOCKADDR*)&sockAddr, iSockAddrLen);
	if(iResult != SOCKET_ERROR)
		bResult = TRUE;
	else
	{	
		iError = GetLastError();
		if(iError == WSAEWOULDBLOCK)
		{	
			FD_ZERO(&writefds);					// clear set
			FD_SET(m_hSocket, &writefds);		// add sock to set  
			tmVal.tv_sec = m_iTimeOutConnect / 1000;
			tmVal.tv_usec = m_iTimeOutConnect % 1000 * 1000;
			iResult = select(0, NULL, &writefds, NULL, &tmVal);
			if(iResult > 0)
			{	
				bResult = TRUE;
			}
			else if(iResult == 0)		//��ʱ
			{
				bResult = FALSE;
			}
		}
		else if(iError == WSAEISCONN)
		{	
			bResult = TRUE;
		}
	}

	uNoBlock = 0;
	ioctlsocket(m_hSocket, FIONBIO, &uNoBlock);

	return bResult;
}

int CSyncSocket::Send(const void *lpBuf, int iBufLen, int iFlags)
{

	int nRet = send(m_hSocket, (LPSTR)lpBuf, iBufLen, iFlags);

	return nRet;

//  	if (m_socketFile)
//  	{
// 		fflush(m_socketFile);
//  		//fclose(f);
//  	}

	return nRet;
}

int CSyncSocket::SendTo(const void *lpBuf, int iBufLen, UINT nHostPort, 
	const char*  lpszHostAddress, int iFlags)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr,0,sizeof(sockAddr));

	sockAddr.sin_family = AF_INET;
	if(lpszHostAddress == NULL)
		sockAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	else
	{
		sockAddr.sin_addr.s_addr = inet_addr(lpszHostAddress);
		if(sockAddr.sin_addr.s_addr == INADDR_NONE)
		{
			LPHOSTENT lphost;
			lphost = gethostbyname(lpszHostAddress);
			if(lphost != NULL)
				sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
			else
			{
				WSASetLastError(WSAEINVAL);
				return SOCKET_ERROR;
			}
		}
	}

	sockAddr.sin_port = htons((u_short)nHostPort);
	
	return sendto(m_hSocket, (LPSTR)lpBuf, iBufLen, iFlags, (SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

int CSyncSocket::Receive(void *lpBuf, int iBufLen, int iFlags)
{
	return recv(m_hSocket, (LPSTR)lpBuf, iBufLen, iFlags);
}

int CSyncSocket::ReceiveFrom(void *lpBuf, int iBufLen, CString &strSocketAddress,
		UINT &nSocketPort, int iFlags)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int iSockAddrLen = sizeof(sockAddr);
	int iResult = recvfrom(m_hSocket, (LPSTR)lpBuf, iBufLen, iFlags,
		(SOCKADDR*)&sockAddr, &iSockAddrLen);

	if(iResult != SOCKET_ERROR)
	{
		nSocketPort = ntohs(sockAddr.sin_port);
		strSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return iResult;
}

//==================================================================================
//		����:	ע�ᴰ��֪ͨ��Ϣ
//		����:	hWnd: ������Ϣ�Ĵ�����; nMsg: ��Ϣ���; lEvent: ע�����Ȥ����Ϣ
//		����:	�ɹ�: TRUE; ʧ��: FALSE
//==================================================================================
BOOL CSyncSocket::RegisterEvent(HWND hWnd, UINT nMsg, long lEvent)
{
	ASSERT(m_hSocket != INVALID_SOCKET);
	ASSERT(hWnd != NULL);

	m_nSockMsg = nMsg;
	m_lSockEvent = lEvent;
	m_hEnentWnd = hWnd;
	return WSAAsyncSelect(m_hSocket, hWnd, nMsg, lEvent) != SOCKET_ERROR;

	/**************ʹ��*******************
	switch(nMsg)
	{
	case WM_SYNCSOCKET_NOTIFY:
		WSAGETSELECTEVENT(lParam): 
		FD_CLOSE
		FD_READ...
		{
			if(WSAGETSELECTERROR(lParam))
				{...������}
			else
				len = recv(wParam, lpBuffer, Length, 0);
		}

	}
	**********************************/
}

//==================================================================================
//		����:	���ø��ʱ
//		����:	timeoutType: ��ʱʱ��; iTimeOut: �����ó�ʱ����
//		����:	��
//==================================================================================
void CSyncSocket::SetTimeOut(TimeOutType timeoutType, int iTimeOut)
{
	switch(timeoutType)
	{
	case TIMEOUT_SEND:
		m_iTimeOutSend = iTimeOut;
		setsockopt(m_hSocket, SOL_SOCKET, SO_SNDTIMEO, 
			(char *)&m_iTimeOutSend, sizeof(m_iTimeOutSend));
		break;
	case TIMEOUT_RECEIVE:
		m_iTimeOutReceive = iTimeOut;
		setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO, 
			(char *)&m_iTimeOutReceive, sizeof(m_iTimeOutReceive));
		break;
	case TIMEOUT_CONNECT:
		m_iTimeOutConnect = iTimeOut;
		break;
	default:
		break;
	}
}

BOOL CSyncSocket::Accept(CSyncSocket &sockAccept, SOCKADDR *lpSockAddr, 
						 int *lpSockAddrLen,  SOCKET hSocket)
{
	ASSERT(sockAccept.m_hSocket == INVALID_SOCKET);
	if(hSocket == NULL)
		sockAccept.m_hSocket = accept(m_hSocket, lpSockAddr, lpSockAddrLen);
	else
		sockAccept.m_hSocket = hSocket;
	
	if(m_hEnentWnd != NULL)
		sockAccept.RegisterEvent(m_hEnentWnd, m_nSockMsg, m_lSockEvent);

	if(funAccept != NULL)
		sockAccept.RegisterFun(funAccept, m_lAcpParam);
	
	if(funReceive != NULL)
		sockAccept.RegisterFun(funReceive, m_lRecvParam);
		
	
	return TRUE;
}

void CSyncSocket::Close(void)
{
	if(m_hSocket != INVALID_SOCKET)
	{
		//0 �C ��������� 
		//1 �C ��������
		//2 �C �������ͺͽ���(�� close() һ��)
		//shutdown(m_hSocket, 2);

		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}	
}

int CSyncSocket::GetPeerName(CString &strPeerAddress, UINT &nPeerPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int iSockAddrLen = sizeof(sockAddr);
	int iResult = getpeername(m_hSocket, (SOCKADDR*)&sockAddr, &iSockAddrLen);
	if(SOCKET_ERROR != iResult)
	{
		nPeerPort = ntohs(sockAddr.sin_port);
		strPeerAddress = inet_ntoa(sockAddr.sin_addr);
	}

	return iResult;
}

BOOL CSyncSocket::GetSockName(CString &strSocketAddress, UINT &nSocketPort)
{
	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));

	int iSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getsockname(m_hSocket, (SOCKADDR*)&sockAddr, &iSockAddrLen);
	if(bResult)
	{
		nSocketPort = ntohs(sockAddr.sin_port);
		strSocketAddress = inet_ntoa(sockAddr.sin_addr);
	}
	return bResult;
}

//==================================================================================
//		����:	ע��accpet�ص�����
//		����:	callbackAccept: ����ָ��
//		����:	��
//==================================================================================
void CSyncSocket::RegisterFun(lp_Accept callbackAccept, LPARAM lParam)
{
	funAccept = callbackAccept;
	
	m_lAcpParam = lParam;

	m_bAcceptThread = TRUE;
	DWORD dwThreadId;
	m_hAcceptThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&(ThreadAcceptFunc),
				this, NULL,	&dwThreadId);
	
	SetThreadPriority(m_hAcceptThread, THREAD_PRIORITY_NORMAL/*THREAD_PRIORITY_ABOVE_NORMAL*/);
}

//==================================================================================
//		����:	ע��Receive�ص�����
//		����:	callbackReceive: ����ָ��
//		����:	��
//==================================================================================
void CSyncSocket::RegisterFun(lp_Receive callbackReceive, LPARAM lParam)
{
	m_lRecvParam = lParam;
	funReceive = callbackReceive;

	m_bReceiveThread = TRUE;
	DWORD dwThreadId;
	m_hReceiveThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&(ThreadReceiveFunc),
				this, NULL,	&dwThreadId);
	
	SetThreadPriority(m_hAcceptThread, THREAD_PRIORITY_HIGHEST);
}

SOCKET CSyncSocket::GetSocket(void)
{
	return m_hSocket;
}

void CSyncSocket::SetSocket(SOCKET hSocket)
{
	m_hSocket = hSocket;
}

UINT CSyncSocket::ThreadAcceptFunc(LPVOID lParam)
{
	CSyncSocket *pThis = (CSyncSocket*)lParam;
	sockaddr_in addrClient;
	int addrClientlen = sizeof(addrClient);
	SOCKET hSockClient = NULL;
	FD_SET readfds;
	struct timeval timeout;
             
	timeout.tv_sec = 15;               
	timeout.tv_usec=0;
	
	while(pThis->m_bAcceptThread)
	{
		ULONG uNoBlock = 1;
		ioctlsocket(pThis->m_hSocket, FIONBIO, &uNoBlock); 
		
		FD_ZERO(&readfds);
		FD_SET(pThis->m_hSocket, &readfds);

		int iResult = select(pThis->m_hSocket + 1,&readfds, 0, 0, &timeout);
		if(iResult == 0 || iResult == SOCKET_ERROR) //��ʱ
			continue;

		if(!FD_ISSET(pThis->m_hSocket, &readfds))	//��һ��������׼������
			continue;

		hSockClient = accept(pThis->m_hSocket,
			(sockaddr FAR*)&addrClient, &addrClientlen);

		if(hSockClient != SOCKET_ERROR)
		{
			if(pThis->funAccept != NULL)
				pThis->funAccept(hSockClient, pThis->m_lAcpParam);
		}

		uNoBlock = 0;
		ioctlsocket(pThis->m_hSocket, FIONBIO, &uNoBlock);
		//FD_CLR(pThis->m_hSocket, &readfds);

		Sleep(2);
	}

	return 1;
}

UINT CSyncSocket::ThreadReceiveFunc(LPVOID lParam)
{
	CSyncSocket *pThis = (CSyncSocket*)lParam;
	while(pThis->m_bReceiveThread)
	{
		char szBuf[READBUFSIZE];

		int iLen = pThis->Receive(szBuf, READBUFSIZE);
		//e_g_LogSave.WriteLog(ConvertWA::ConvertWA_A2W(szBuf).c_str());
		if(iLen != SOCKET_ERROR)
		{
			szBuf[iLen] = '\0';
			pThis->funReceive(szBuf, iLen, pThis->m_lRecvParam);
		}
		else
			Sleep(1);
	}

	return 1;
}

BOOL CSyncSocket::SetSockOpt(int iOptionName, const void *lpOptionValue,
		int iOptionLen, int iLevel)
{
	return (setsockopt(m_hSocket, iLevel, iOptionName, 
		(char *)lpOptionValue, iOptionLen) == SOCKET_ERROR);
}