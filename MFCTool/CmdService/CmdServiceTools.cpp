#include "stdafx.h"
#include "CmdServiceTools.h"
#include "md5.h"
#include "SyncSocket.h"

CCmdServiceTools::CCmdServiceTools()
{
	m_logCB = NULL;
	m_logCBParam = NULL;
}

CCmdServiceTools::~CCmdServiceTools()
{
}

void CCmdServiceTools::SetCmdServiceAddress(const char *ip, UINT port, const char *editId)
{
	strcpy_s(m_ipAddress, ip);
	strcpy_s(m_editId, editId);
	m_port = port;
}

void CCmdServiceTools::SetLogCallBack(fLogCallBack cb, void *lparam)
{
	m_logCB = cb;
	m_logCBParam = lparam;
}

bool CCmdServiceTools::QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue)
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
		RecordLogToFile("QuerySingleStatus: create socket error", true);
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

bool CCmdServiceTools::PublishEmgency(const vector<string> &arryDeviceID, const EmgencyInfo &info, string &taskID)
{
	RecordLogToFile("开始发送紧急信息...");

	char szBuf[128 * 1024];
	memset(szBuf, 0, 128 * 1024);

	COleDateTime tm;
	int i, iDataSize, iSend;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_SET pTaskSet;
	PTASK_LIST pTaskList;

	tm = COleDateTime::GetCurrentTime();
	//prepare data
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryDeviceID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryDeviceID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	string strMD5 = GetUniqueID();
	strcpy(pTaskSet->szTaskID, strMD5.c_str());
	pTaskSet->iTaskType = (__int8)MT_PUBLISH_INFO;
	pTaskSet->iEmType = (__int8)info.nType;
	pTaskSet->iFullScreen = info.bFullScreen ? 1 : 0;
	pTaskSet->iReplaceInfo = info.bIsReplay ? 1 : 0;
	pTaskSet->iSoundType = info.nAudio;
	pTaskSet->buLeftVolume = info.nLeftValue;
	pTaskSet->buRightVolume = info.nRightValue;
	strcpy(pTaskSet->szEmergencyInfo, info.szEmergencyInfo);
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = info.iTimeLength;
	if (pTaskSet->iTimeLength < 0)
		pTaskSet->iTimeLength = 0;

	strcpy(pTaskSet->szEditorID, m_editId);
	pTaskSet->buRightLevel = info.exectLevel;
	taskID = pTaskSet->szTaskID;
	char logStr[256] = { 0 };

	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryDeviceID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "发送信息 %s", arryDeviceID[i].c_str());
		RecordLogToFile(logStr);
	}
	CSyncSocket sockCmd;
	if (!sockCmd.Create())
	{
		RecordLogToFile("Socket creat fail",true);
		return false;
	}
	
	if (!sockCmd.Connect(m_ipAddress, m_port))
	{
		sockCmd.Close();
		CString strLog;
		strLog.Format("Socket Connect (%s:%d) fail", m_ipAddress, m_port);
		RecordLogToFile(strLog, true);
		return false;
	}
		

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		RecordLogToFile("cmdService 信息发送失败", true);
		sockCmd.Close();
		return false;
	}
	else
	{
		RecordLogToFile("cmdService 信息发送成功");
	}
	sockCmd.Close();
	return true;
}

bool CCmdServiceTools::StopEmgency(const vector<string> &arryDeviceID, string &taskID, unsigned int exectLevel)
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
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryDeviceID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryDeviceID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	strcpy(pTaskSet->szTaskID, GetUniqueID().c_str());
	pTaskSet->iTaskType = (__int8)MT_STOP_PUBLISH;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = 0;
	pTaskSet->iReplaceInfo = (__int8)0;
	pTaskSet->iSoundType = (__int8)NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = 0;
	pTaskSet->buRightVolume = 0;
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = 0;
	strcpy(pTaskSet->szEditorID, m_editId);
	pTaskSet->buRightLevel = exectLevel;

	taskID = pTaskSet->szTaskID;

	char logStr[256] = { 0 };
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryDeviceID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "停止信息 %s", arryDeviceID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
	{
		RecordLogToFile("Socket creat fail", true);
		return false;
	}

	if (!sockCmd.Connect(m_ipAddress, m_port))
	{
		sockCmd.Close();
		CString strLog;
		strLog.Format("Socket Connect (%s:%d) fail", m_ipAddress, m_port);
		RecordLogToFile(strLog, true);
		return false;
	}

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		sockCmd.Close();
		return false;
	}

	sockCmd.Close();

	return true;
}

bool CCmdServiceTools::ExectCommand(const vector<string> &arryDeviceID, MACRO_TYPE_INDEX cmdID, unsigned int leftVolue /*= 0*/, unsigned int rightVolue /*= 0*/, string &taskID, unsigned int exectLevel /*= 50*/)
{
	
	string cmdDes = GetCommandDes(cmdID);
	CString strLog;
	strLog.Format("开始执行%s", cmdDes.data());
	RecordLogToFile(strLog);

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
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_SET) + arryDeviceID.size() * sizeof(TASK_LIST);

	pCmd = (PCMD_HEAD)szBuf;
	pData = (PBYTE)szBuf;
	memset(pCmd, 0, iDataSize);
	pCmd->buType = CMD_PUBLISH_TASK;
	pCmd->iDataSize = iDataSize;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = arryDeviceID.size();
	//init task set
	pTaskSet = (PTASK_SET)(pData + sizeof(CMD_HEAD));
	strcpy(pTaskSet->szTaskID, GetUniqueID().c_str());
	pTaskSet->iTaskType = (__int8)cmdID;
	pTaskSet->iEmType = (__int8)NET_ES_CONTENT;
	pTaskSet->iFullScreen = 0;
	pTaskSet->iReplaceInfo = (__int8)0;
	pTaskSet->iSoundType = cmdID ==  MT_SET_VOLUME ?  NET_ES_SOUND_SET_VOLUME : NET_ES_SOUND_NONE;
	pTaskSet->buLeftVolume = leftVolue;
	pTaskSet->buRightVolume = rightVolue;
	pTaskSet->dtTaskTime = tm.operator DATE();
	pTaskSet->iValidTime = 120;
	pTaskSet->iTimeLength = 0;
	strcpy(pTaskSet->szEditorID, m_editId);
	pTaskSet->buRightLevel = exectLevel;
	taskID = pTaskSet->szTaskID;
	char logStr[256] = { 0 };
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_SET) + sizeof(TASK_LIST) * i);
		strcpy(pTaskList->szDeviceCode, arryDeviceID[i].c_str());
		pTaskList->iExecResult = CMD_RESULT_WAIT_EXEC;

		sprintf_s(logStr, "执行%s 信息 %s",cmdDes.data(), arryDeviceID[i].c_str());
		RecordLogToFile(logStr);
	}

	if (!sockCmd.Create())
	{
		RecordLogToFile("Socket creat fail", true);
		return false;
	}

	if (!sockCmd.Connect(m_ipAddress, m_port))
	{
		sockCmd.Close();
		CString strLog;
		strLog.Format("Socket Connect (%s:%d) fail", m_ipAddress, m_port);
		RecordLogToFile(strLog, true);
		return false;
	}

	iSend = sockCmd.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		sockCmd.Close();
		return false;
	}

	sockCmd.Close();

	return true;
}

bool CCmdServiceTools::UpLoadDeviceStatus(const char *szDeviceId, const DEVICE_STATUS &deStatue)
{
	int i, iSize, iDataSize, iSend, iReceive;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	//PDEVICE_STATUS pDS;
	//
	CSyncSocket sock;
	bool bRet = false;
	//allocate memory
	iDataSize = sizeof(CMD_HEAD) + sizeof(DEVICE_STATUS);
	pData = new BYTE[iDataSize];
	if (!pData)
	{
		RecordLogToFile("UpdateStatus: allocate memory error");
		goto ErrorExit;
	}
	memset(pData, 0, iDataSize);

	//init socket
	if (!sock.Create())
	{
		RecordLogToFile("UpdateStatus: create socket error");
		goto ErrorExit;
	}
	if (!sock.Connect(m_ipAddress, m_port))
	{
		RecordLogToFile("UpdateStatus: Connect server error");
		goto ErrorExit;
	}


	//init cmd
	pCmd = (PCMD_HEAD)pData;
	pCmd->buType = CMD_PUBLISH_STATUS;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = 1;
	pCmd->iDataSize = iDataSize;

	//pDS = (PDEVICE_STATUS)(pData + sizeof(CMD_HEAD) + sizeof(DEVICE_STATUS));
	//pDS->dtUpdateTime = tm.operator DATE();
	//设置pDS成员
	memcpy(pData + sizeof(CMD_HEAD), &deStatue, sizeof(DEVICE_STATUS));
	
	//send
	iSend = sock.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		goto ErrorExit;
	}
	Sleep(500);
	iReceive = sock.Receive(pData, iDataSize);
	bRet = true;
ErrorExit:
	if (pData)
	{
		delete pData;
	}
	sock.Close();


	return bRet;
}

bool CCmdServiceTools::QueryExecuteResult(const string &taskID, vector<TASK_LIST> &resultList, unsigned int nCount)
{
	resultList.clear();
	int i, iDataSize, iSend, iReceive;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	PTASK_LIST pTaskList;

	CSyncSocket sock;


	//prepare data
	iDataSize = sizeof(CMD_HEAD) + nCount * sizeof(TASK_LIST);
	pData = new BYTE[iDataSize];
	if (!pData)
	{
		RecordLogToFile("allocate memory error",true);
		goto ErrorExit;
	}
	//init cmd head
	pCmd = (PCMD_HEAD)pData;
	memset(pCmd, 0, sizeof(CMD_HEAD));
	pCmd->buType = CMD_QUERY_TASK_RESULT;
	pCmd->iDataSize = sizeof(CMD_HEAD);
	strcpy(pCmd->szHead, CMD_FLAG);
	strcpy(pCmd->szTaskID, taskID.data());

	//init socket
	if (!sock.Create())
	{
		RecordLogToFile("create socket error",true);
		goto ErrorExit;
	}
	if (!sock.Connect(m_ipAddress, m_port))
	{
		RecordLogToFile("Connect server error",true);
		goto ErrorExit;
	}
	
	//send
	iSend = sock.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		RecordLogToFile("Send data error",true);
		goto ErrorExit;
	}
	Sleep(50);
	iReceive = sock.Receive(pCmd, iDataSize);
	if (!(iReceive == iDataSize && pCmd->iDataSize == iReceive && pCmd->buAck))
	{
		RecordLogToFile("Receive data error",true);
		goto ErrorExit;
	}

	//parse result
	for (i = 0; i < pCmd->iNum; i++)
	{
		pTaskList = (PTASK_LIST)(pData + sizeof(CMD_HEAD) + sizeof(TASK_LIST) * i);
		//pTaskList为执行结果
		resultList.push_back(*pTaskList);
	}

	//close and free data
	sock.Close();
	delete pData;

	return true;

ErrorExit:
	if (pData)
	{
		delete pData;
	}
	sock.Close();
	return false;
}

bool CCmdServiceTools::QueryTask(const char* szDeviceID, vector<TASK_SET> &resultList)
{
	int iSend, iReceive;
	char szBuf[4096];
	bool bRet = false;
	PCMD_HEAD pCmd = (PCMD_HEAD)szBuf;
	CSyncSocket sock;
	PDEVICE_STATUS pDS;
	//init cmd head
	memset(pCmd, 0, sizeof(CMD_HEAD));
	pCmd->buType = CMD_QUERY_TASK;
	pCmd->iDataSize = sizeof(CMD_HEAD);
	strcpy(pCmd->szHead, CMD_FLAG);
	strcpy(pCmd->szDeviceID, szDeviceID);
	resultList.clear();
	do 
	{
	
		//init socket
		if (!sock.Create())
		{
			RecordLogToFile("CMD_QUERY_TASK: create socket error", true);
			break;
		}
		if (!sock.Connect(m_ipAddress, m_port))
		{
			RecordLogToFile("CMD_QUERY_TASK: Connect server error", true);
			break;
		}
		sock.SetTimeOut(TIMEOUT_SEND, 3000);

		//send and receive
		iSend = sock.Send(pCmd, pCmd->iDataSize);
		if (iSend != pCmd->iDataSize)
		{
			RecordLogToFile("CMD_QUERY_TASK: Send query status error", true);
			break;
		}
		Sleep(50);
		iReceive = sock.Receive(szBuf, sizeof(szBuf));
		if (!( pCmd->iDataSize == iReceive && pCmd->buAck))
		{
			RecordLogToFile("Receive data error", true);
			break;
		}
		//parse result
		PTASK_SET pTaskSet;
		for (int i = 0; i < pCmd->iNum; i++)
		{
			pTaskSet = (PTASK_SET)(szBuf + sizeof(CMD_HEAD) + sizeof(TASK_SET) * i);
			//pTaskList为执行结果
			resultList.push_back(*pTaskSet);
		}
		bRet = true;

	} while (0);

	sock.Close();
	return bRet;
}

bool CCmdServiceTools::UpLoadTASK_RESULT(const char *szDeviceId, const TASK_RESULT &taskRsult)
{
	int i, iSize, iDataSize, iSend, iReceive;
	PBYTE pData = NULL;
	PCMD_HEAD pCmd;
	//PDEVICE_STATUS pDS;
	//
	CSyncSocket sock;
	bool bRet = false;
	//allocate memory
	iDataSize = sizeof(CMD_HEAD) + sizeof(TASK_RESULT);
	pData = new BYTE[iDataSize];
	if (!pData)
	{
		RecordLogToFile("UpLoadTASK_RESULT: allocate memory error");
		goto ErrorExit;
	}
	memset(pData, 0, iDataSize);

	//init socket
	if (!sock.Create())
	{
		RecordLogToFile("UpLoadTASK_RESULT: create socket error");
		goto ErrorExit;
	}
	if (!sock.Connect(m_ipAddress, m_port))
	{
		RecordLogToFile("UpLoadTASK_RESULT: Connect server error");
		goto ErrorExit;
	}


	//init cmd
	pCmd = (PCMD_HEAD)pData;
	pCmd->buType = CMD_FEEDBACK_TASK_RESULT;
	strcpy(pCmd->szHead, CMD_FLAG);
	pCmd->iNum = 1;
	pCmd->iDataSize = iDataSize;

	//pDS = (PDEVICE_STATUS)(pData + sizeof(CMD_HEAD) + sizeof(DEVICE_STATUS));
	//pDS->dtUpdateTime = tm.operator DATE();
	//设置pDS成员
	memcpy(pData + sizeof(CMD_HEAD), &taskRsult, sizeof(TASK_RESULT));

	//send
	iSend = sock.Send(pCmd, pCmd->iDataSize);
	if (iSend != pCmd->iDataSize)
	{
		goto ErrorExit;
	}
	Sleep(500);
	iReceive = sock.Receive(pData, iDataSize);
	bRet = true;
ErrorExit:
	if (pData)
	{
		delete pData;
	}
	sock.Close();


	return bRet;
}

string CCmdServiceTools::MD5Encode32(const char* szIn, int iSize)
{
	char szOut[36] = { 0 };

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

string CCmdServiceTools::GetUniqueID(void)
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

std::string CCmdServiceTools::GetCommandDes(MACRO_TYPE_INDEX cmdID)
{
	string des = "";
	switch (cmdID)
	{
	case MT_POWER_ON:
		des = "开机";
		break;
	case MT_POWER_OFF:
		des = "关机";
		break;
	case MT_RESET_DEVICE:
		des = "重启";
		break;
	case MT_SCREEN_ON:
		des = "开屏";
		break;
	case MT_SCREEN_OFF:
		des = "关屏";
		break;
	case MT_PUBLISH_INFO:
		des = "发布紧急信息";
		break;
	case MT_STOP_PUBLISH:
		des = "停止紧急消息";
		break;
	case MT_START_RUNNING:
		des = "开始运行";
		break;
	case MT_STOP_RUNNING:
		des = "停止运行";
		break;
	case MT_TAKE:
		des = "TAKE";
		break;
	case MT_SOUND_ON:
		des = "开启声音";
		break;
	case MT_SOUND_OFF:
		des = "关闭声音";
		break;
	case MT_SET_VOLUME:
		des = "设置声音";
		break;
	case MT_LIVE_ON:
		des = "开启直播";
		break;
	case MT_LIVE_OFF:
		des = "关闭直播";
		break;
	case MAX_MACRO_TYPE:
		des = "MAX_MACRO_TYPE";
		break;
	default:
		break;
	}

	return des;
}

void CCmdServiceTools::RecordLogToFile(const char * info, bool bIsError /*= false*/)
{
	if (m_logCB)
	{
		m_logCB(info, bIsError ? 1 : 0, m_logCBParam);
	}

}
