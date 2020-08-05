#include "stdafx.h"
#include "ComHandle.h"

static CComHandle * g_this = NULL;
CComHandle::CComHandle()
{
	m_nComPort = 0;
	m_cbParam = NULL;
	m_receiveCB = NULL;
	g_this = this;
}


CComHandle::~CComHandle()
{
	CloseComPort();
}

bool CComHandle::OpenComPort(int iComPort, ComParamTag *param /*= NULL*/)
{
	CloseComPort();
	
	if (sio_open(iComPort) != SIO_OK)
	{
		return false;
	}
	m_nComPort = iComPort;
	if (!SetComPortParam(param))
	{
		CloseComPort();
		return false;
	}

	return true;
}

bool CComHandle::CloseComPort()
{
	if (m_nComPort > 0 )
	{
		sio_close(m_nComPort);
	}
	return true;
}

int CComHandle::WriteDataToCom(char *byCmd, UINT nWriteCount)
{
	if (m_nComPort <= 0 )
	{
		return -1;
	}

	return sio_write(m_nComPort, byCmd, nWriteCount);

}

void CComHandle::SetReceiveDataCallBack(fReceiveDataFromComCallBack cb, void *lparam)
{
	m_cbParam = lparam;
	m_receiveCB = cb;
}

int CComHandle::ReadBuf(char* buf, UINT nLeng)
{
	int iRecvLen = sio_read(m_nComPort, buf, nLeng);

	return iRecvLen;
}

bool CComHandle::SetComPortParam(ComParam *param)
{

	int baud = 0;
	int mode = 0;
	if (param == NULL)
	{
		baud = B9600;
		mode = P_NONE | BIT_8 | STOP_1;
	}
	else
	{
		GetRealParam(param,baud, mode);
	}
	//参数设置
	if (sio_ioctl(m_nComPort, baud, mode) != SIO_OK)
		return false;

	//超时时间
	if (sio_SetReadTimeouts(m_nComPort, 50, 10) != SIO_OK)
		return false;

	if (sio_SetWriteTimeouts(m_nComPort, 0) != SIO_OK)
		return false;

	sio_cnt_irq(m_nComPort, ReceiveDataInfo, 1);   //中断函数

	return true;
}

void CComHandle::GetRealParam(ComParam *param,int &baud, int &mode)
{
	//波特率
	switch (param->nBtSpeed)
	{
	case 50:
		baud = B50;
		break;
	case 75:
		baud = B75;
		break;
	case 110:
		baud = B110;
		break;
	case 134:
		baud = B134;
		break;
	case 150:
		baud = B150;
		break;
	case 300:
		baud = B300;
		break;
	case 600:
		baud = B600;
		break;
	case 1200:
		baud = B1200;
		break;

	case 1800:
		baud = B1800;
		break;
	case 2400:
		baud = B2400;
		break;
	case 4800:
		baud = B4800;
		break;
	case 7200:
		baud = B7200;
		break;
	case 9600:
		baud = B9600;
		break;
	case 19200:
		baud = B19200;
		break;
	case 38400:
		baud = B38400;
		break;
	case 57600:
		baud = B57600;
		break;
	case 115200:
		baud = B115200;
		break;
	case 230400:
		baud = B230400;
		break;
	case 460800:
		baud = B460800;
		break;
	case 921600:
		baud = B921600;
		break;
	default:
		baud = B9600;
		break;
	}
	int dataNum = BIT_8;
	switch (param->nData)
	{
	case 8:
		dataNum = BIT_8;
		break;
	case 7:
		dataNum = BIT_7;
		break;
	case 6:
		dataNum = BIT_6;
		break;
	case 5:
		dataNum = BIT_5;
		break;
	default:
		dataNum = BIT_8;
		break;
	}

	int stopNum = STOP_1;
	switch (param->nStop)
	{
	case 1:
		stopNum = STOP_1;
		break;
	case 2:
		stopNum = STOP_2;
		break;
	default:
		stopNum = STOP_1;
		break;
	}

	mode = P_NONE | dataNum | stopNum;
}

void CALLBACK CComHandle::ReceiveDataInfo(int iPort)
{
	char szReceBuf[80];
	memset(szReceBuf, 0, 80);
	int iRecvLen = sio_read(iPort, szReceBuf, 80);

	if (g_this->m_receiveCB)
	{
		g_this->m_receiveCB(szReceBuf, iRecvLen,g_this->m_cbParam);
	}
}
