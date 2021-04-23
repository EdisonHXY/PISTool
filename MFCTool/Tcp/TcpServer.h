#pragma once
#include <afxsock.h>
#include "Icmp.h"
#include <vector>

#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include "../MfcUI/ThreadCheck.h"

using namespace std;

class CTcpServer
{
public:
	CTcpServer();
	~CTcpServer();

	enum SocketStatus
	{
		SocketStatus_Connect,
		SocketStatus_Disconnect
	};
	typedef void(*fReceiveMsgWithSocketCallBack)(SOCKET clientSc,CString ipStr, char *szBuffer, UINT nLength, void *lparam);
	typedef void(*fCloseSocketCallBack)(CString ipStr, void *lparam);
	typedef void(*fSocketStatusCallBack)(CString clientIpAddress, SocketStatus nStatus, void *lparam);
	
	//��ʼ
	bool StartServer( UINT nPort);

	//ֹͣ
	int StopServer();

	//�Ͽ���������
	void DisConnectAll();

	//���ý�����Ϣ�Ļص�
	void SetReceiveMsgCallBack(fReceiveMsgWithSocketCallBack cb, void *lparam);

	//�������ӵĽ����� �Ͽ��Ļص�
	void SetSocketStatusCallBack(fSocketStatusCallBack cb, void *lparam);

	//ping �豸 
	int PingDevice(const char *szIP);

	//����
	int Send(SOCKET clientSc,const char *szData, int nLen);

private:
	fReceiveMsgWithSocketCallBack m_receiveCB;
	void* m_receiveCBParam;

	fSocketStatusCallBack m_statusCB;
	void *m_statusCBParam;

	SOCKET m_soServer;
	CThreadCheck m_thread;
private:
	static void ThreadExectCallBackFunc(UINT threadID, void *lparam);
	void CloseSocket(SOCKET s);
	void HandleThread();

	vector<SOCKET> m_clientSocketList;
private:
	bool LoadIcmpDll(void);
	HINSTANCE m_hDllIcmp;
	HANDLE m_hIcmp;
	lpICMP_CreateFile Icmp_CreateFile;
	lpICMP_CloseHandle Icmp_CloseHandle;
	lpICMP_SendEcho   Icmp_SendEcho;
};


class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	bool ConnectServer(CString strServerIp,int nPort);
	void Disconnect();
	int Send(const char *szData, int nLen);
	//���ý�����Ϣ�Ļص�
	void SetReceiveMsgCallBack(CTcpServer::fReceiveMsgWithSocketCallBack cb, void *lparam);
	//�������ӵĽ����� �Ͽ��Ļص�
	void SetSocketStatusCallBack(CTcpServer::fSocketStatusCallBack cb, void *lparam);

private:
	SOCKET m_soClient;
	CThreadCheck m_receiveThread;
	bool m_isConnect;
private:
	CTcpServer::fReceiveMsgWithSocketCallBack m_receiveCB;
	void* m_receiveCBParam;

	CTcpServer::fSocketStatusCallBack m_statusCB;
	void *m_statusCBParam;

	static void ReiveThreadExectCallBackFunc(UINT threadID, void *lparam);
	void HandleSocketReceive();
};
