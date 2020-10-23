#include "stdafx.h"
#include "UDPRecive.h"


CUDPRecive::CUDPRecive()
{
	m_rcCB = NULL;
	m_rcParam = NULL;
	AfxSocketInit();
}


CUDPRecive::~CUDPRecive()
{
}

BOOL CUDPRecive::JoinGroup(CString addressStr, UINT port)
{
	//port = 10000;
	if (!CreateReceivingSocket(addressStr, port))		/* Create Socket for receiving and join the host group */
		return FALSE;

	return TRUE;
}


BOOL CUDPRecive::CreateReceivingSocket(LPCTSTR szIP, UINT uPort)
{
	/* Create socket for receiving packets from multicast group */
	if (!Create(uPort, SOCK_DGRAM, FD_READ))
		return FALSE;

	BOOL bMultipleApps = TRUE;		/* allow reuse of local port if needed */
	SetSockOpt(SO_REUSEADDR, (void*)&bMultipleApps, sizeof(BOOL), SOL_SOCKET);

	/* Fill m_saHostGroup_in for sending datagrams */
	memset(&m_saHostGroup, 0, sizeof(m_saHostGroup));
	m_saHostGroup.sin_family = AF_INET;
	m_saHostGroup.sin_addr.s_addr = inet_addr(szIP);
	m_saHostGroup.sin_port = htons((USHORT)uPort);

	/* Join the multicast group */
	m_mrMReq.imr_multiaddr.s_addr = inet_addr(szIP);		/* group addr */
	m_mrMReq.imr_interface.s_addr = htons(INADDR_ANY);		/* use default */
	if (setsockopt(m_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq)) < 0)
		return FALSE;

	return TRUE;
}

BOOL CUDPRecive::LeaveGroup()
{

	setsockopt(m_hSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char FAR *)&m_mrMReq, sizeof(m_mrMReq));
	Close();

					// Close receving socket
	return TRUE;
}


void CUDPRecive::SetReceiveCallBack(fReceiveCallBack cb, void*lparam)
{
	m_rcCB = cb;
	m_rcParam = lparam;
}

void CUDPRecive::OnReceive(int nErrorCode)
{
	char szBuf[65535] = { 0 };
	int iRec = Receive(szBuf, 65535);

	if (m_rcCB && iRec > 0)
	{
		szBuf[iRec] = 0;
		m_rcCB(szBuf,iRec, m_rcParam);

	}

	CAsyncSocket::OnReceive(nErrorCode);
}
