/******************************************************************************************
*	版权所有(c) 2013 北京冠华天视工程部
*	文件名称:	DeviceInfo.h
*	文件说明:	设备信息头文件
*	当前版本:	1.0.20130618
*	作者:		贾巍云
*	创建日期:	2013-06-18
******************************************************************************************/
#ifndef	__DEVICEINFO_H__
#define	__DEVICEINFO_H__

#include "..\Socket\SyncSocket.h"
#include <afxtempl.h>

class CDeviceInfo
{
public:
	CDeviceInfo(void);
	virtual ~CDeviceInfo(void);

	int SendCmd(char *szCmd, int iSize);
	BOOL Connect(void);

	
public:
	CHAR m_szDeviceCode[16];
	CHAR m_szDeviceAddr[24];
	BOOL m_bConnected;

	int m_iDevicePort;
	CSyncSocket m_sockCmd;

};

typedef CArray<CDeviceInfo*, CDeviceInfo*> ArryDevice;

#endif
