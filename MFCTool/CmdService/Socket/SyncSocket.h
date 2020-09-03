/*******************************************************************************
*	版权所有(c)	2009
*	文件名称:	SyncSocket.h
*	文件说明:	Socket 通信信类头文件
*	当前版本:	1.0.20150812
*	作者:		贾巍云
*	创建日期:	2009-12-16
*******************************************************************************/
#ifndef	__SYNCSOCKET_H__
#define	__SYNCSOCKET_H__

#include <winsock2.h>
#include <mswsock.h>  

#pragma   comment(lib,   "ws2_32.lib")   

typedef void(*fReceiveMsgCallBack)(char *recInfo, unsigned int length, int nErrorCode, void *lparam);
typedef void(*fResultStatueCallBack)(int nErrorCode);
enum SocketEventType{ EVENT_SEND, EVENT_RECEIVE, EVENT_CONNECT, EVENT_ACCEPT };
enum TimeOutType{ TIMEOUT_SEND, TIMEOUT_RECEIVE, TIMEOUT_CONNECT };

#define		WM_SYNCSOCKET_NOTIFY		WM_USER + 0x0100

#define		READBUFSIZE			307200
#define		WRITEBUFSIZE		307200

typedef void (CALLBACK *lp_Accept)(SOCKET hSocket, LPARAM lParam);//_cdecl
typedef void (CALLBACK *lp_Receive)(void *lpBuf, int iLen, LPARAM lParam);

/////////////////////////////////////////////////////////////
class CSyncSocket
{
public:
	CSyncSocket(void);
	~CSyncSocket(void);
	
	BOOL Create(UINT nSocketPort = 0, int iSocketType = SOCK_STREAM,	//创建socket并初始化
		const char*  lpszSocketAddress = NULL);
	BOOL Bind(UINT nSocketPort, const char*  lpszSocketAddress);				//给未命名套接字赋一个本地名
	BOOL Listen(UINT nBackLog);											//在指定套接字上监听外来连接
	BOOL Connect(const char*  lpszHostAddress, UINT nHostPort);				//在指定套接字上初始化连接
	BOOL Accept(CSyncSocket &sockAccept, SOCKADDR *lpSockAddr = NULL,	//确认外来连接，并将它与一个立即建立的数据套接字联系起来。原始套接字返回到监听状态
		int *lpSockAddrLen = NULL, SOCKET hSocket = NULL);
	void Close(void);													//关闭					
	int Send(const void *lpBuf, int iBufLen, int iFlags = 0);			//给一个连接套接字发送数据
	int SendTo(const void *lpBuf, int iBufLen, UINT nHostPort,			//给一个连接或未连接套接字发送数据
		const char*  lpszHostAddress = NULL, int iFlags = 0);
	
	int Receive(void *lpBuf, int iBufLen, int iFlags = 0);				//从一个连接的套接字上接收数据
	int ReceiveFrom(void *lpBuf, int iBufLen, CString &strSocketAddress,//从一个连接或未连接的套接字上接收数据
		UINT &nSocketPort, int iFlags = 0);

	int GetPeerName(CString &strPeerAddress, UINT &nPeerPort);			//获取与指定套接字连接的对等方的名字
	BOOL GetSockName(CString &strSocketAddress, UINT &nSocketPort);		//获取指定套接字的当前名字

	void SetTimeOut(TimeOutType timeoutType, int iTimeOut);				//设置超时, iTimeOut为 ms

	BOOL RegisterEvent(HWND hWnd, UINT nMsg = WM_SYNCSOCKET_NOTIFY,		//注册窗体消息
		long lEvent = FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE);

	void RegisterFun(lp_Accept callbackAccept, LPARAM lParam);			//注册accept回调函数
	void RegisterFun(lp_Receive callbackReceive, LPARAM lParam);		//注册Receive回调函数

	BOOL SetSockOpt(int iOptionName, const void *lpOptionValue,			//设置与指定套接字相关的选项
		int iOptionLen, int iLevel = SOL_SOCKET);
	BOOL IoCtlSocket(long lCommand, DWORD *lpArgument)					//为套接字提供控制
		{ return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); }

	int GetLastError(void) { return WSAGetLastError(); }
	
	void SetSocket(SOCKET hSocket);
	SOCKET GetSocket(void);
	void SetNoBlock(void);

	static UINT ThreadAcceptFunc(LPVOID lParam);		//accept线程函数
	static UINT ThreadReceiveFunc(LPVOID lParam);		//Receive线程函数


private:
	BOOL Connect(SOCKADDR_IN &sockAddr, int iSockAddrLen);

public:
	SOCKET m_hSocket;		//套接字句柄
	FILE * m_socketFile;
private:
	
	HWND m_hEnentWnd;		//接收事件窗体句柄
	UINT m_nSockMsg;		//接收消息
	long m_lSockEvent;
	
	int m_iTimeOutConnect;	//连接超时
	int m_iTimeOutSend;		
	int m_iTimeOutReceive;

	BOOL m_bAcceptThread;
	BOOL m_bReceiveThread;
	HANDLE m_hAcceptThread;
	HANDLE m_hReceiveThread;

	lp_Accept funAccept;		//accept回调函数句柄
	lp_Receive funReceive;		//Receive回调函数句柄

	LPARAM m_lRecvParam, m_lAcpParam;
};

#endif
