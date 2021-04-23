#include "stdafx.h"
#include "TcpServer.h"
#include <algorithm>

CTcpServer::CTcpServer():m_thread(1)
{
	WORD dwVersion = MAKEWORD(2, 2);
	WSAData wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	m_soServer = INVALID_SOCKET;

	m_receiveCB = NULL;
	m_receiveCBParam = NULL;

	m_statusCB = NULL;
	m_statusCBParam = NULL;

	m_hDllIcmp = NULL;
	m_hIcmp = NULL;
	Icmp_CreateFile = NULL;
	Icmp_CloseHandle = NULL;
	Icmp_SendEcho = NULL;

	m_thread.SetThreadExectCallBack(ThreadExectCallBackFunc, this);
	m_thread.UpdateInterval(1);


}


CTcpServer::~CTcpServer()
{
	StopServer();
	WSACleanup();
}

bool CTcpServer::StartServer( UINT nPort)
{
	if (m_soServer == INVALID_SOCKET)
	{
	
		m_soServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	if (m_soServer == INVALID_SOCKET)
	{
		return false;
	}
	
	m_thread.StartRun();

	sockaddr_in addrServer;
	memset(&addrServer, 0, sizeof(sockaddr_in));
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(nPort);
	addrServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	int iResult;

	bool bReuseAddr = true;
	iResult = setsockopt(m_soServer, SOL_SOCKET, SO_REUSEADDR, (char *)&bReuseAddr, sizeof(bReuseAddr));
	if (SOCKET_ERROR == iResult) {
		printf("Failed to set resueaddr socket!\r\n");
		//return false;
	}

	//设置非阻塞方式连接
	unsigned long cmd = 1;
	iResult = ioctlsocket(m_soServer, FIONBIO, &cmd);

	iResult = bind(m_soServer, (sockaddr *)&addrServer, sizeof(addrServer));
	if (SOCKET_ERROR == iResult) {
		printf("Failed to bind address!\r\n");
		return false;
	}

	if (0 != listen(m_soServer, 5)) {
		printf("Failed to listen client!\r\n");
		return false;
	}

	printf("Start server...\r\n");


	return true;
}

void CTcpServer::DisConnectAll()
{
	for (int i = 0; i < m_clientSocketList.size(); i++) {
		SOCKET &tmpS = m_clientSocketList[i];
		if (tmpS != INVALID_SOCKET)
		{
			closesocket(tmpS);
			tmpS = INVALID_SOCKET;
			if (m_statusCB)
			{
				m_statusCB("DisConnectAll", CTcpServer::SocketStatus_Disconnect, m_statusCBParam);
			}
		}
	}

	m_clientSocketList.clear();
}

int CTcpServer::StopServer()
{
	m_thread.StopRun();
	DisConnectAll();
	
	return 0;
}


void CTcpServer::SetReceiveMsgCallBack(fReceiveMsgWithSocketCallBack cb, void *lparam)
{
	m_receiveCB = cb;
	m_receiveCBParam = lparam;
}

void CTcpServer::SetSocketStatusCallBack(fSocketStatusCallBack cb, void *lparam)
{
	m_statusCB = cb;
	m_statusCBParam = lparam;
}


int CTcpServer::PingDevice(const char *szIP)
{
	if (Icmp_SendEcho == NULL || m_hIcmp == NULL)
	{
		if (!LoadIcmpDll())
		{
			return -99; //无法加载dll
		}
	}
	

	IP_OPTION_INFORMATION_ICMP ipInfo;
	PICMP_ECHO_REPLY_ICMP pIcmpEcho;
	int iSize = sizeof(ICMP_ECHO_REPLY_ICMP) + 8;
	char szData[sizeof(ICMP_ECHO_REPLY_ICMP) + 8];
	BOOL bRetsult = FALSE;

	pIcmpEcho = (PICMP_ECHO_REPLY_ICMP)szData;
	ipInfo.Ttl = 255;
	ipInfo.Tos = 0;
	ipInfo.Flags = 0;
	ipInfo.OptionsSize = 0;
	ipInfo.OptionsData = NULL;
	pIcmpEcho->Status = 0;
	Icmp_SendEcho(m_hIcmp, inet_addr(szIP), NULL, 0, &ipInfo, pIcmpEcho, iSize, 1200);
	bRetsult = (pIcmpEcho->Status == 0);
	
	printf("Ping: %s status: %u.\n", szIP, pIcmpEcho->Status);

	return bRetsult ? 0 : -1;
}

int CTcpServer::Send(SOCKET clientSc, const char *szData, int nLen)
{
	int nSend = send(clientSc, szData, nLen, 0);
	if (nSend != nLen)
	{
		printf("");
	}
	return nLen;
}

void CTcpServer::ThreadExectCallBackFunc(UINT threadID, void *lparam)
{
	CTcpServer *m = (CTcpServer*)lparam;
	m->HandleThread();
}

void CTcpServer::CloseSocket(SOCKET s)
{
	for (vector<SOCKET>::iterator iter = m_clientSocketList.begin();iter != m_clientSocketList.end(); )
	{
		if (*iter == s)
		{
			if (m_statusCB)
			{
				sockaddr_in name;
				int namelen = sizeof(sockaddr_in);
				getpeername(s, (sockaddr *)&name, &namelen);
				m_statusCB(inet_ntoa(name.sin_addr), SocketStatus_Disconnect, m_statusCBParam);
			}

			if (*iter != INVALID_SOCKET)
			{
				closesocket(s);
				*iter = INVALID_SOCKET;
			}
			iter = m_clientSocketList.erase(iter);
		}
		else
			iter++;
	}

}

void CTcpServer::HandleThread()
{
	if (m_soServer == INVALID_SOCKET)
	{
		return;
	}
	fd_set fdRead, fdExcept;
	// initialize
	FD_ZERO(&fdRead);
	FD_ZERO(&fdExcept);

	FD_SET(m_soServer, &fdRead);


	sockaddr_in addrAccept;
	int iAcceptLen = sizeof(addrAccept);

	timeval tv;
	int nTimeOut = 10;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;

	for (int  i = 0; i < m_clientSocketList.size(); i++)
	{
		FD_SET(m_clientSocketList[i], &fdRead);
		FD_SET(m_clientSocketList[i], &fdExcept);
		
	}

	int iResult = select(m_soServer + 1, &fdRead,NULL,&fdExcept, &tv);
	if (iResult == SOCKET_ERROR) {
		//printf("SOCKET_ERROR happened...\n");
		int n = WSAGetLastError();
		return;
	}
	else if (iResult == 0) {
		//printf("Time limit expired\n");
		return;
	}

	if (FD_ISSET(m_soServer, &fdRead)) {

		FD_CLR(m_soServer,&fdRead);

		if (m_clientSocketList.size() >= FD_SETSIZE)
		{
			//大于系统限制的连接数
			return;
		}

		 SOCKET soClient = accept(m_soServer, (sockaddr*)&addrAccept, &iAcceptLen);
		if (soClient < 0) {
			return;
		}
		else {
// 			printf("\n[%s:%d] has connected to server!\r\n", inet_ntoa(addrAccept.sin_addr),
// 				ntohs(addrAccept.sin_port));
			if (m_statusCB)
			{
				m_statusCB(inet_ntoa(addrAccept.sin_addr), SocketStatus_Connect, m_statusCBParam);
			}

			m_clientSocketList.push_back(soClient);
		}

		return;
	}


	// check read
	for (int i = 0; i < (int)fdRead.fd_count; i++) {

		if (fdRead.fd_array[i] == m_soServer) {
			continue;
		}
		if (FD_ISSET(fdRead.fd_array[i], &fdRead)) {

			

			sockaddr_in name;
			int namelen = sizeof(sockaddr_in);
			getpeername(fdRead.fd_array[i], (sockaddr *)&name, &namelen);

			char buf[409600] = { 0 };
			int len = 409599;
			int ret = recv(fdRead.fd_array[i], buf, len, 0);
			
			if (ret <= 0 ) {
			
				CloseSocket(fdRead.fd_array[i]);
				
			}
			else {
				if (m_receiveCB)
				{
					buf[ret] = '\0';
					m_receiveCB(fdRead.fd_array[i],inet_ntoa(name.sin_addr), buf, ret, m_receiveCBParam);
				}
				//printf("\nRecv from [%s:%d] : %s\n", inet_ntoa(name.sin_addr), ntohs(name.sin_port), buf);
			}
		
			FD_CLR(fdRead.fd_array[i], &fdRead);
		
		}
	
		//check fdExcept
		if (FD_ISSET(fdExcept.fd_array[i], &fdExcept))
		{
			int lastErr;
			int errlen = sizeof(lastErr);
			getsockopt(fdExcept.fd_array[i], SOL_SOCKET, SO_ERROR, (char*)&lastErr, &errlen);
			
			CloseSocket(fdExcept.fd_array[i]);
			FD_CLR(fdExcept.fd_array[i], &fdExcept);
		}
	}


}

bool CTcpServer::LoadIcmpDll(void)
{
	m_hDllIcmp = LoadLibrary("Icmp.dll");
	if (m_hDllIcmp == NULL)
		return false;
	Icmp_CreateFile = (lpICMP_CreateFile)GetProcAddress(m_hDllIcmp, "IcmpCreateFile");
	if (Icmp_CreateFile == NULL)
		return false;
	Icmp_CloseHandle = (lpICMP_CloseHandle)GetProcAddress(m_hDllIcmp, "IcmpCloseHandle");
	if (Icmp_CloseHandle == NULL)
		return false;
	Icmp_SendEcho = (lpICMP_SendEcho)GetProcAddress(m_hDllIcmp, "IcmpSendEcho");
	if (Icmp_SendEcho == NULL)
		return false;
	m_hIcmp = Icmp_CreateFile();
	return true;
}


CTcpClient::CTcpClient():m_receiveThread(0)
{
	m_isConnect = false;
	WORD dwVersion = MAKEWORD(2, 2);
	WSAData wsaData;
	WSAStartup(WINSOCK_VERSION, &wsaData);

	m_receiveCB = NULL;
	m_receiveCBParam = NULL;

	m_statusCB = NULL;
	m_statusCBParam = NULL;


	m_soClient = INVALID_SOCKET;
	
	m_receiveThread.UpdateInterval(1);
	m_receiveThread.SetThreadExectCallBack(ReiveThreadExectCallBackFunc,this);

}

CTcpClient::~CTcpClient()
{
	
	Disconnect();
	WSACleanup();

}

bool CTcpClient::ConnectServer(CString strServerIp, int nPort)
{
	if (m_isConnect)
	{
		return true;
	}

	if (m_soClient == INVALID_SOCKET)
	{
		m_soClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}

	if (m_soClient == INVALID_SOCKET)
	{
		
		return false;
	}

	sockaddr_in addrServer;
	addrServer.sin_addr.S_un.S_addr = inet_addr(strServerIp);
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(nPort);
	int iResult;

	//设置非阻塞方式连接
	unsigned long ul = 1;
	iResult = ioctlsocket(m_soClient, FIONBIO, (unsigned long*)&ul);

	iResult = connect(m_soClient, (sockaddr *)&addrServer, sizeof(sockaddr_in));

	//设置超时
	timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	fd_set fdConnect;
	FD_ZERO(&fdConnect);
	FD_SET(m_soClient, &fdConnect);
	bool isConnect = false;
	iResult = select(0, &fdConnect, &fdConnect, NULL, &tv);
	if (iResult > 0)
	{
		m_isConnect = true;
		isConnect = true;
		if (m_statusCB)
		{
			m_statusCB("", CTcpServer::SocketStatus_Connect, m_statusCBParam);
		}

	}

	//设置为阻塞模式
	ul = 0;
	iResult = ioctlsocket(m_soClient, FIONBIO, (unsigned long*)&ul);

	m_receiveThread.StartRun();
	
	return isConnect;
}

void CTcpClient::Disconnect()
{

	if (m_soClient != INVALID_SOCKET && m_isConnect)
	{
		m_isConnect = false;
		m_receiveThread.StopRun();
		closesocket(m_soClient);
		m_soClient = INVALID_SOCKET;
		if (m_statusCB)
		{
			m_statusCB("Disconnect", CTcpServer::SocketStatus_Disconnect, m_statusCBParam);
		}
	}
	
	
}

int CTcpClient::Send(const char *szData, int nLen)
{
	int nSend = send(m_soClient, szData, nLen, 0);
	if (nSend != nLen)
	{
		printf("");
	}
	return nLen;
}

void CTcpClient::SetReceiveMsgCallBack(CTcpServer::fReceiveMsgWithSocketCallBack cb, void *lparam)
{
	m_receiveCB = cb;
	m_receiveCBParam = lparam;
}

void CTcpClient::SetSocketStatusCallBack(CTcpServer::fSocketStatusCallBack cb, void *lparam)
{
	m_statusCB = cb;
	m_statusCBParam = lparam;
}

void CTcpClient::ReiveThreadExectCallBackFunc(UINT threadID, void *lparam)
{
	CTcpClient *c = (CTcpClient*)lparam;
	c->HandleSocketReceive();

}

void CTcpClient::HandleSocketReceive()
{
	if (m_soClient == INVALID_SOCKET)
	{
		return;
	}
	fd_set fdRead,fdWrite,fdExcept;
	FD_ZERO(&fdRead);
	FD_ZERO(&fdWrite);
	FD_ZERO(&fdExcept);
	FD_SET(m_soClient, &fdRead);
	FD_SET(m_soClient, &fdWrite);
	FD_SET(m_soClient, &fdExcept);
	timeval tv;
	int nTimeOut = 10;
	tv.tv_sec = 0;
	tv.tv_usec = nTimeOut;
	int iResult = select(0, &fdRead, &fdWrite, &fdExcept, &tv);
	if (iResult == SOCKET_ERROR) {
		return;
	}
	else if (iResult == 0) {
		printf("Time limit expired\n");
	}
	else {
		// check read
		if (FD_ISSET(m_soClient, &fdRead)) {
			sockaddr_in name;
			int namelen = sizeof(sockaddr_in);
			getpeername(m_soClient, (sockaddr *)&name, &namelen);
			

			char buf[409600] = { 0 };
			int len = 409600;
			int ret = recv(m_soClient, buf, len, 0);
		
			if (ret <=0) {
				
				Disconnect();

			}
			else {
					
				if (m_receiveCB)
				{
					buf[ret] = '\0';
					m_receiveCB(m_soClient,inet_ntoa(name.sin_addr), buf, ret,m_receiveCBParam);
				}

			}
		
			FD_CLR(m_soClient, &fdRead);
		}
	
		//check Send
		if (FD_ISSET(m_soClient,&fdExcept))
		{
			
			int lastErr;
			int errlen = sizeof(lastErr);
			getsockopt(m_soClient, SOL_SOCKET, SO_ERROR, (char*)&lastErr, &errlen);
			Disconnect();
			FD_CLR(m_soClient, &fdExcept);
		}

	}
}
