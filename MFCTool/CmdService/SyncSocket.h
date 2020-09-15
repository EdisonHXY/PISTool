/*******************************************************************************
*	��Ȩ����(c)	2009
*	�ļ�����:	SyncSocket.h
*	�ļ�˵��:	Socket ͨ������ͷ�ļ�
*	��ǰ�汾:	1.0.20150812
*	����:		��Ρ��
*	��������:	2009-12-16
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
	
	BOOL Create(UINT nSocketPort = 0, int iSocketType = SOCK_STREAM,	//����socket����ʼ��
		const char*  lpszSocketAddress = NULL);
	BOOL Bind(UINT nSocketPort, const char*  lpszSocketAddress);				//��δ�����׽��ָ�һ��������
	BOOL Listen(UINT nBackLog);											//��ָ���׽����ϼ�����������
	BOOL Connect(const char*  lpszHostAddress, UINT nHostPort);				//��ָ���׽����ϳ�ʼ������
	BOOL Accept(CSyncSocket &sockAccept, SOCKADDR *lpSockAddr = NULL,	//ȷ���������ӣ���������һ�����������������׽�����ϵ������ԭʼ�׽��ַ��ص�����״̬
		int *lpSockAddrLen = NULL, SOCKET hSocket = NULL);
	void Close(void);													//�ر�					
	int Send(const void *lpBuf, int iBufLen, int iFlags = 0);			//��һ�������׽��ַ�������
	int SendTo(const void *lpBuf, int iBufLen, UINT nHostPort,			//��һ�����ӻ�δ�����׽��ַ�������
		const char*  lpszHostAddress = NULL, int iFlags = 0);
	
	int Receive(void *lpBuf, int iBufLen, int iFlags = 0);				//��һ�����ӵ��׽����Ͻ�������
	int ReceiveFrom(void *lpBuf, int iBufLen, CString &strSocketAddress,//��һ�����ӻ�δ���ӵ��׽����Ͻ�������
		UINT &nSocketPort, int iFlags = 0);

	int GetPeerName(CString &strPeerAddress, UINT &nPeerPort);			//��ȡ��ָ���׽������ӵĶԵȷ�������
	BOOL GetSockName(CString &strSocketAddress, UINT &nSocketPort);		//��ȡָ���׽��ֵĵ�ǰ����

	void SetTimeOut(TimeOutType timeoutType, int iTimeOut);				//���ó�ʱ, iTimeOutΪ ms

	BOOL RegisterEvent(HWND hWnd, UINT nMsg = WM_SYNCSOCKET_NOTIFY,		//ע�ᴰ����Ϣ
		long lEvent = FD_READ|FD_WRITE|FD_OOB|FD_ACCEPT|FD_CONNECT|FD_CLOSE);

	void RegisterFun(lp_Accept callbackAccept, LPARAM lParam);			//ע��accept�ص�����
	void RegisterFun(lp_Receive callbackReceive, LPARAM lParam);		//ע��Receive�ص�����

	BOOL SetSockOpt(int iOptionName, const void *lpOptionValue,			//������ָ���׽�����ص�ѡ��
		int iOptionLen, int iLevel = SOL_SOCKET);
	BOOL IoCtlSocket(long lCommand, DWORD *lpArgument)					//Ϊ�׽����ṩ����
		{ return (SOCKET_ERROR != ioctlsocket(m_hSocket, lCommand, lpArgument)); }

	int GetLastError(void) { return WSAGetLastError(); }
	
	void SetSocket(SOCKET hSocket);
	SOCKET GetSocket(void);
	void SetNoBlock(void);

	static UINT ThreadAcceptFunc(LPVOID lParam);		//accept�̺߳���
	static UINT ThreadReceiveFunc(LPVOID lParam);		//Receive�̺߳���


private:
	BOOL Connect(SOCKADDR_IN &sockAddr, int iSockAddrLen);

public:
	SOCKET m_hSocket;		//�׽��־��
	FILE * m_socketFile;
private:
	
	HWND m_hEnentWnd;		//�����¼�������
	UINT m_nSockMsg;		//������Ϣ
	long m_lSockEvent;
	
	int m_iTimeOutConnect;	//���ӳ�ʱ
	int m_iTimeOutSend;		
	int m_iTimeOutReceive;

	BOOL m_bAcceptThread;
	BOOL m_bReceiveThread;
	HANDLE m_hAcceptThread;
	HANDLE m_hReceiveThread;

	lp_Accept funAccept;		//accept�ص��������
	lp_Receive funReceive;		//Receive�ص��������

	LPARAM m_lRecvParam, m_lAcpParam;
};

#endif
