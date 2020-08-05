#pragma once
#include <afxsock.h>

class CUDPClient
{
public:
	CUDPClient();
	~CUDPClient();

	int SendToMSg(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);

	
private:
	CAsyncSocket m_clientSockt;

};

