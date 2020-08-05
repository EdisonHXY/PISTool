#include "stdafx.h"
#include "UDPClient.h"

CUDPClient::CUDPClient()
{
	bool bret = AfxSocketInit();

}


CUDPClient::~CUDPClient()
{
	m_clientSockt.Close();
}

int CUDPClient::SendToMSg(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress /*= NULL*/)
{
	
	if (m_clientSockt.m_hSocket == INVALID_SOCKET)
	{
		AfxSocketInit();
		if (!m_clientSockt.Create(0, SOCK_DGRAM))
		{
		
			AfxMessageBox("UDP Socket Create fail");
		}
	}

	return m_clientSockt.SendTo(lpBuf, nBufLen, nHostPort, lpszHostAddress);
}



