/******************************************************************************************
*	��Ȩ����(c) 2013 �����ڻ����ӹ��̲�
*	�ļ�����:	DeviceInfo.h
*	�ļ�˵��:	�豸��Ϣͷ�ļ�
*	��ǰ�汾:	1.0.20130618
*	����:		��Ρ��
*	��������:	2013-06-18
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
