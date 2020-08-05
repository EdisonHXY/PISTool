#pragma once
#include "afxsock.h"
class CUDPRecive :
	public CAsyncSocket
{
public:
	CUDPRecive();
	~CUDPRecive();
	typedef void(*fReceiveCallBack)(void * pAtsInfo, int iSize, void*lparam);

	BOOL JoinGroup(CString addressStr, UINT port);
	BOOL LeaveGroup();

	void SetReceiveCallBack(fReceiveCallBack cb, void*lparam);

	virtual void OnReceive(int nErrorCode);
private:
	//multicast if
	SOCKADDR_IN	m_saHostGroup;		// SOCKADDR structure to hold IP/Port of the Host group to send data to it
	ip_mreq		m_mrMReq;			// Contains IP and interface of the host group
	BOOL CreateReceivingSocket(LPCTSTR szIP, UINT uPort);

	fReceiveCallBack m_rcCB;
	void*m_rcParam;
};

