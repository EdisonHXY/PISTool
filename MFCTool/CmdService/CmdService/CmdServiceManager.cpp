#include "stdafx.h"
#include "CmdServiceManager.h"
//#include "CharactTranslate.h"
#include "../MD5_file/md5.h"

CCmdServiceManager::CCmdServiceManager()
{
	m_logCB = NULL;
	m_logCBParam = NULL;
}


CCmdServiceManager::~CCmdServiceManager()
{
}

CCmdServiceManager * CCmdServiceManager::GetInstance()
{
	static CCmdServiceManager cmd;
	return &cmd;
}

void CCmdServiceManager::SetCmdServiceAddress(const char *ip, UINT port)
{
	strcpy_s(m_ipAddress, ip);
	m_port = port;
}

void CCmdServiceManager::QueryAllDeviceStatus(vector<DEVICE_STATUS> *listDevice)
{
	if (listDevice == NULL)
	{
		return;
	}
	listDevice->clear();

	int i, iDataSize, iSend, iReceive, iRet;
	PBYTE pData = NULL;
	char szBuf[4096];
	
	PCMD_HEAD pCmd, pTemp = (PCMD_HEAD)szBuf;
	CSyncSocket sock;

	//prepare buffer
	iDataSize = sizeof(CMD_HEAD) + sizeof(DEVICE_STATUS) * 200;
	pData = new BYTE[iDataSize];
	if (!pData)
	{
		RecordLogToFile("QueryStatus: allocate memory error",true);
		goto ErrorExit;
	}
	//init cmd head
	pCmd = (PCMD_HEAD)pData;
	memset(pCmd, 0, sizeof(CMD_HEAD));
	pCmd->buType = CMD_QUERY_STATUS;
	pCmd->iDataSize = sizeof(CMD_HEAD);
	strcpy(pCmd->szHead, CMD_FLAG);

	//init socket
	if (!sock.Create())
	{
		RecordLogToFile("QueryStatus: create socket error",true);
		goto ErrorExit;
	}
	if (!sock.Connect(m_ipAddress, m_port))
	{
		RecordLogToFile("QueryStatus: Connect server error", true);
		goto ErrorExit;
	}
	sock.SetTimeOut(TIMEOUT_SEND, 3000);

	//send and receive
	iSend = sock.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		RecordLogToFile("QueryStatus: Send query status error", true);
		goto ErrorExit;
	}
	Sleep(50);
	iReceive = 0;
	iRet = sock.Receive(pTemp, sizeof(CMD_HEAD));
	if (iRet == sizeof(CMD_HEAD) && pTemp->buAck)
	{
		memcpy(pData, pTemp, iRet);
		iReceive += iRet;
		while (iReceive < pCmd->iDataSize)
		{
			iRet = sock.Receive(szBuf, sizeof(szBuf));
			if (iRet > 0)
			{
				if (iDataSize > iReceive)
				{
					memcpy(pData + iReceive, szBuf, iRet);
				}
				else
				{
					RecordLogToFile("QueryStatus: Receive query status ack error 接收size 过小", true);
					goto ErrorExit;
				}
			
				iReceive += iRet;
			}
			else
			{
				break;
			}
		}
		//check
		if (iReceive == pCmd->iDataSize)
		{
			for (i = 0; i < pCmd->iNum; i++)
			{
				PDEVICE_STATUS pDS;
				pDS = (PDEVICE_STATUS)(pData + sizeof(CMD_HEAD) + sizeof(DEVICE_STATUS) * i);
				//pDS即设备状态结构指针
				
				if (pDS->iCheckResult == CMD_RESULT_OK)
				{
					DEVICE_STATUS s;
					memcpy(&s, pDS, sizeof(DEVICE_STATUS));
					listDevice->push_back(s);
				}
				

			}
		}
		else
		{
			RecordLogToFile("QueryStatus: Receive query status ack error", true);
			goto ErrorExit;
		}
	}
	else
	{
		RecordLogToFile("QueryStatus: Receive query status ack error", true);
		goto ErrorExit;
	}

	//close and free data
ErrorExit:
	if (pData)
	{
		delete pData;
	}
	sock.Close();
}

void CCmdServiceManager::RecordLogToFile(const char *info,bool bIsError)
{
	if (m_logCB)
	{
		m_logCB(info, bIsError ? 1 : 0, m_logCBParam);
	}
}


string CCmdServiceManager::MD5Encode32(const char* szIn, int iSize)
{
	char szOut[36];

	MD5 md5;
	if (iSize == -1)
	{
		iSize = strlen(szIn);
	}
	md5.update((UCHAR *)szIn, iSize);
	md5.finalize();
	md5.hex_digest(szOut, sizeof(szOut));

	return szOut;
}


string CCmdServiceManager::GetUniqueID(void)
{
	SYSTEMTIME sysTime;
	DWORD dwTime;
	static ULONGLONG ulIndex = 0;

	GetLocalTime(&sysTime);
	dwTime = GetTickCount();

	char strID[256] = { 0 };
	sprintf_s(strID, "%04d-%02d-%02d  %2d:%02d:%02d.%d  %d  %d  %I64d  %s",
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, dwTime, GetCurrentProcessId(),
		ulIndex++, "10.3.0.10");


	string strMD5 = MD5Encode32(strID, strlen(strID));
	return strMD5;
}

BOOL CCmdServiceManager::SendEmMsg(const vector<string> &arryID,const string &content,bool isFullScreen, __int16 timeLength, bool isReplac, bool  isCloseAudio)
{
	RecordLogToFile("开始发送紧急信息...");

	char szBuf[128 * 1024];
	memset(szBuf, 0, 128 * 1024);

	CSyncSocket sockCmd;

	COleDateTime tm;
	int i, iDataSize, iSend;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_SET pTaskSet;
	PTASK_LIST pTaskList;

	tm = COleDateTime::GetCurrentTime();
	//prepare data
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	string strMD5 = GetUniqueID();
	strcpy(pTaskSet->szTaskID, strMD5.c_str());
	pTaskSet->iTaskType = (__int8)MT_PUBLISH_INFO;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = (__int8)isFullScreen;
	pTaskSet->iReplaceInfo = isReplac ?  1: 0;
	pTaskSet->iSoundType = isCloseAudio ? (__int8)NET_ES_SOUND_OFF : (__int8)NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = 0;
	pTaskSet->buRightVolume = 0;
	strcpy(pTaskSet->szEmergencyInfo, content.c_str());
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = timeLength;
	if (pTaskSet->iTimeLength < 0)
		pTaskSet->iTimeLength = 0;

	strcpy(pTaskSet->szEditorID, "99");
	pTaskSet->buRightLevel = 50;

	char logStr[256] = { 0 };

	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "发送信息 %s", arryID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
		return FALSE;


	if (!sockCmd.Connect(m_ipAddress, m_port))
		return FALSE;

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		RecordLogToFile("cmdService 信息发送失败", true);
		return FALSE;
	}
	else
	{
		RecordLogToFile("cmdService 信息发送成功");
	}

	return TRUE;

}

BOOL CCmdServiceManager::EndEmMsg(const vector<string> &arryID, bool isFullScreen)
{
	char szBuf[128 * 1024];
	memset(szBuf, 0, 128 * 1024);

	CSyncSocket sockCmd;

	COleDateTime tm;
	int i, iDataSize, iSend;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_SET pTaskSet;
	PTASK_LIST pTaskList;

	tm = COleDateTime::GetCurrentTime();
	//prepare data
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	strcpy(pTaskSet->szTaskID, GetUniqueID().c_str());
	pTaskSet->iTaskType = (__int8)MT_STOP_PUBLISH;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = (__int8)isFullScreen;
	pTaskSet->iReplaceInfo = (__int8)0;
	pTaskSet->iSoundType = (__int8)NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = 0;
	pTaskSet->buRightVolume = 0;
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = 0;
	strcpy(pTaskSet->szEditorID, "99");
	pTaskSet->buRightLevel = 50;
	char logStr[256] = { 0 };
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "停止信息 %s", arryID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
		return FALSE;

	if (!sockCmd.Connect(m_ipAddress, m_port))
		return FALSE;

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		return FALSE;
	}



	return TRUE;
}

BOOL CCmdServiceManager::SendDeviceStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd)
{
	char szBuf[128 * 1024];
	memset(szBuf, 0, 128 * 1024);

	CSyncSocket sockCmd;

	COleDateTime tm;
	int i, iDataSize, iSend;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_SET pTaskSet;
	PTASK_LIST pTaskList;

	tm = COleDateTime::GetCurrentTime();
	//prepare data
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	strcpy(pTaskSet->szTaskID, GetUniqueID().c_str());
	pTaskSet->iTaskType = (__int8)devCmd;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = (__int8)0;
	pTaskSet->iReplaceInfo = (__int8)0;
	pTaskSet->iSoundType = (__int8)NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = 0;
	pTaskSet->buRightVolume = 0;
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = 0;
	strcpy(pTaskSet->szEditorID, "99");
	pTaskSet->buRightLevel = 50;
	char logStr[256] = { 0 };
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "DeviceStatue发布消息 %s", arryID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
		return FALSE;

	if (!sockCmd.Connect(m_ipAddress, m_port))
		return FALSE;

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		return FALSE;
	}



	return TRUE;
}

BOOL CCmdServiceManager::SendDeviceVolumeStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd, int leftVolue, int rightVolue /*= 0*/)
{
	char szBuf[128 * 1024];
	memset(szBuf, 0, 128 * 1024);

	CSyncSocket sockCmd;

	COleDateTime tm;
	int i, iDataSize, iSend;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_SET pTaskSet;
	PTASK_LIST pTaskList;

	tm = COleDateTime::GetCurrentTime();
	//prepare data
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	strcpy(pTaskSet->szTaskID, GetUniqueID().c_str());
	pTaskSet->iTaskType = (__int8)devCmd;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = (__int8)0;
	pTaskSet->iReplaceInfo = (__int8)0;
	pTaskSet->iSoundType = (__int8)NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = leftVolue;
	pTaskSet->buRightVolume = rightVolue;
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = 0;
	strcpy(pTaskSet->szEditorID, "99");
	pTaskSet->buRightLevel = 50;
	char logStr[256] = { 0 };
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "音量设置 发布消息 %s", arryID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
		return FALSE;

	if (!sockCmd.Connect(m_ipAddress, m_port))
		return FALSE;

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		return FALSE;
	}



	return TRUE;
}

void CCmdServiceManager::SetLogCallBack(fLogCallBack cb, void *lparam)
{
	m_logCB = cb;
	m_logCBParam = lparam;
}

bool CCmdServiceManager::QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue)
{
	int iSend, iReceive;
	char szBuf[4096];
	bool bRet = false;
	PCMD_HEAD pCmd = (PCMD_HEAD)szBuf;
	CSyncSocket sock;
	PDEVICE_STATUS pDS;
	//init cmd head
	memset(pCmd, 0, sizeof(CMD_HEAD));
	pCmd->buType = CMD_QUERY_SINGLE_STATUS;
	pCmd->iDataSize = sizeof(CMD_HEAD);
	strcpy(pCmd->szHead, CMD_FLAG);
	strcpy(pCmd->szDeviceID, szDeviceID);

	//init socket
	if (!sock.Create())
	{
		RecordLogToFile("QuerySingleStatus: create socket error",true);
		goto ErrorExit;
	}
	if (!sock.Connect(m_ipAddress, m_port))
	{
		RecordLogToFile("QuerySingleStatus: Connect server error", true);
		goto ErrorExit;
	}
	sock.SetTimeOut(TIMEOUT_SEND, 3000);

	//send and receive
	iSend = sock.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		RecordLogToFile("QuerySingleStatus: Send query status error", true);
		goto ErrorExit;
	}
	Sleep(50);
	iReceive = sock.Receive(szBuf, sizeof(szBuf));
	if (iReceive >= sizeof(CMD_HEAD) && pCmd->iDataSize == iReceive && pCmd->buAck)
	{
		if (pCmd->iNum)
		{
			pDS = (PDEVICE_STATUS)(szBuf + sizeof(CMD_HEAD));
			//pDS即设备状态结构指针
			deStatue = *pDS;
			bRet = true;
		}
	}
	else
	{
		RecordLogToFile("QuerySingleStatus: Receive query status ack error", true);
		goto ErrorExit;
	}

	//close and free data
ErrorExit:
	sock.Close();
	return bRet;
}
