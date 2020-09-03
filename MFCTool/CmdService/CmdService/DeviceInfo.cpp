/******************************************************************************************
*	版权所有(c) 2013 北京冠华天视工程部
*	文件名称:	DeviceInfo.cpp
*	文件说明:	设备信息源文件
*	当前版本:	1.0.20130618
*	作者:		贾巍云
*	创建日期:	2013-06-18
******************************************************************************************/
#include "stdafx.h"
#include "DeviceInfo.h"


/////////////////////////////////////////////////////////////////////////////
CDeviceInfo::CDeviceInfo(void)
{
	m_bConnected = FALSE;

	memset(m_szDeviceCode, 0, 16);
	strcpy_s(m_szDeviceAddr, "127.0.0.1");
	
	m_iDevicePort = 9999;
}

CDeviceInfo::~CDeviceInfo(void)
{
	
}



int CDeviceInfo::SendCmd(char *szCmd, int iSize)
{
	if(!m_bConnected)
		return 0;

	int iSend = m_sockCmd.Send(szCmd, iSize);
	if(iSend != iSize)
		m_bConnected = FALSE;

	/*
	CString strLog, strValue;
	for(int i=0; i<iSize; i++)
	{
		strValue.Format(L"%02X ", (BYTE)szCmd[i]);
		strLog += strValue;
	}

	e_g_LogSave.WriteLog(LOG_TYPE_INFO, "指令发送: %s", strLog);*/

	return iSend;

}

BOOL CDeviceInfo::Connect(void)
{
	m_sockCmd.Close();

	if(m_sockCmd.Create(0))
	{
		m_sockCmd.SetTimeOut(TIMEOUT_SEND, 1000);
		m_sockCmd.SetTimeOut(TIMEOUT_CONNECT, 1000);

		if(m_sockCmd.Connect(m_szDeviceAddr, m_iDevicePort))
		{
			
			m_bConnected = TRUE;
		}
		else
		{
			m_bConnected = FALSE;
		}

	}

	return m_bConnected;
}
