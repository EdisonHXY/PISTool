#pragma once
#include <afxsock.h>
#include "Icmp.h"
#include <vector>

#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")
#include "../MfcUI/ThreadCheck.h"
#include <map>

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
	//返回值是 剩余未处理的数据长度
	typedef int(*fReceiveMsgWithSocketCallBack)(SOCKET clientSc, char *szInputBuffer, UINT nInputLength, void *lparam);
	
	//client
	typedef int(*fReceiveClientMsgWithSocketCallBack)(SOCKET clientSc, CString ipStr,char *szInputBuffer, UINT nInputLength, void *lparam);


	typedef void(*fCloseSocketCallBack)(CString ipStr, void *lparam);
	typedef void(*fSocketStatusCallBack)(CString clientIpAddress, SocketStatus nStatus, void *lparam);
	
	//开始
	bool StartServer( UINT nPort);

	//停止
	int StopServer();

	//断开所有连接
	void DisConnectAll();

	//设置接收消息的回调
	void SetReceiveMsgCallBack(fReceiveMsgWithSocketCallBack cb, void *lparam);

	//设置连接的建立和 断开的回调
	void SetSocketStatusCallBack(fSocketStatusCallBack cb, void *lparam);

	//ping 设备 
	int PingDevice(const char *szIP);

	//发送
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

private:
	//黏包时的数据
	map<SOCKET, pair<char*, int> > m_preMsgMap;

	//保存数据
	void savePreMsg(SOCKET s,char *szMsg, int nLen);

	//清空数据
	void deletePreMsg(SOCKET s);
	void getNewDataMsg(SOCKET s,const char* szMgs, int nLen, char **szOutMsg, int &nOutLen);
};


class CTcpClient
{
public:
	CTcpClient();
	~CTcpClient();

	bool ConnectServer(CString strServerIp,int nPort);
	void Disconnect();
	int Send(const char *szData, int nLen);
	//设置接收消息的回调
	void SetReceiveMsgCallBack(CTcpServer::fReceiveClientMsgWithSocketCallBack cb, void *lparam);
	//设置连接的建立和 断开的回调
	void SetSocketStatusCallBack(CTcpServer::fSocketStatusCallBack cb, void *lparam);

private:
	SOCKET m_soClient;
	CThreadCheck m_receiveThread;
	bool m_isConnect;
private:
	CTcpServer::fReceiveClientMsgWithSocketCallBack m_receiveCB;
	void* m_receiveCBParam;

	CTcpServer::fSocketStatusCallBack m_statusCB;
	void *m_statusCBParam;

	static void ReiveThreadExectCallBackFunc(UINT threadID, void *lparam);
	void HandleSocketReceive();
};
