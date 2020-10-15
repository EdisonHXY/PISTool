#include "stdafx.h"
#include "DeviceControl.h"

static CDeviceControl *g_cmd = NULL;
CString CDeviceControl::m_dllName = "PlayCtrl.dll";
CDeviceControl::CDeviceControl(const char * libPath):m_dllPath(libPath)
{
	m_resultTask.clear();
	m_mapList.clear();
	m_logCB = NULL;
	m_logCBParam = NULL;

}

void CDeviceControl::SetDllName(CString dllName)
{
	m_dllName = dllName;

}

CDeviceControl * CDeviceControl::GetInstance()
{
	if (g_cmd)
	{
		return g_cmd;
	}

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	CString tmpStr = _tcsrchr(szFilePath, '\\');
	(_tcsrchr(szFilePath, '\\'))[0] = 0;
	CString dir = szFilePath;
	g_cmd = new CDeviceControl(dir + "\\" + m_dllName);
	//g_cmd = new CDeviceControl(dir + "\\PDPCtrl.dll");
	
	return g_cmd;
}

CDeviceControl::~CDeviceControl()
{
}

void CDeviceControl::SetLogCallBack(fLogCallBack cb, void *lparam)
{
	m_logCB = cb;
	m_logCBParam = lparam;
}

void CDeviceControl::SetDeviceMap(vector<pair<string , string >> &listMap)
{
	m_mapList.clear();
	for (int i =0 ;i<listMap.size();++i)
	{

		pair<string, string > p = listMap[i];
		m_mapList.push_back(p);

	}

}

int CDeviceControl::SendMsg(const char *pIpStr, char *pszText, bool isFullScreen)
{
	
	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			CString strlog;
			strlog.Format("Load dll Fail,%s", m_dllName);
			RecoreLog(strlog, true);
			return -1;
		}
		
	}
	NET_ES_PARAM esParam;
	memset(&esParam, 0, sizeof(esParam));
	esParam.bFullScreen = isFullScreen;
	esParam.iEsType = 0;
	esParam.iRightLevel = 255;
	sprintf(esParam.szContent, pszText);

	esParam.wTimeLength = 0;
	esParam.wReplaceInfo = 1;

	int iRet = CMD_RESULT_LOCAL_ERROR;
	PVOID pVoid = NULL;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_SetContent != NULL)
	{
		pVoid = m_cclib.ctrl_InitDll(pIpStr, "", 255);
		iRet = m_cclib.ctrl_SetContent(pVoid, &esParam);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);

	}
	CString strlog;
	strlog.Format("发送紧急消息 返回值 = %d！ip=%s", iRet, pIpStr);
	RecoreLog(strlog, iRet != CMD_RESULT_OK);

	return iRet;

}

TASK_LIST GetDefaultTask(const char *szCode)
{
	TASK_LIST tl;
	tl.iExecResult = -1;
	COleDateTime dt;
	tl.dtExecTime = dt.GetCurrentTime();
	strcpy(tl.szDeviceCode, szCode);
	return tl;
}

int CDeviceControl::SendMsg(vector<string > listDevice,const char *pszText, bool isFullScreen /*= false*/)
{
	m_resultTask.clear();

	int nRet = -1;
	for (int i = 0 ; i < listDevice.size();++i)
	{
		CString strlog;
		string ipStr = GetIpStrFromList(listDevice[i]);
		TASK_LIST tl = GetDefaultTask(listDevice[i].c_str());
		if (ipStr.length() < 5)
		{
			strlog.Format("发送紧急消息失败！ 设备编号=%s,ip=%s", listDevice[i].c_str(), ipStr.c_str());
			RecoreLog(strlog,true);
			m_resultTask.push_back(tl);
			continue;
		}

		nRet = SendMsg(ipStr.c_str(), (char*)pszText, isFullScreen);
		if (nRet == CMD_RESULT_OK)
		{
			tl.iExecResult = 1;
		}
		m_resultTask.push_back(tl);

		strlog.Format("发送紧急消息%s！ 设备编号=%s,ip=%s", nRet == CMD_RESULT_OK ? "成功":"失败", listDevice[i].c_str(), ipStr.c_str());
		RecoreLog(strlog, nRet != CMD_RESULT_OK);

	}

	return 0;

}

int CDeviceControl::EndMsg(const char *pIpStr)
{

	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			return -1;
		}
	}
	PVOID pVoid = NULL;
	int iRet = CMD_RESULT_LOCAL_ERROR;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_ExecuteCmd != NULL)
	{
		//glLogSave.WriteLog(LOG_TYPE_INFO, "信息发送 %s - %s", pDeviceInfo->m_strDeviceIP, pszText);
		pVoid = m_cclib.ctrl_InitDll(pIpStr, "", 255);
		iRet = m_cclib.ctrl_ExecuteCmd(pVoid, CTRL_CMD_END_CONTENT,0,NULL,0);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);

	}
	CString strlog;
	strlog.Format("取消紧急消息 返回值 = %d！ip=%s", iRet, pIpStr);
	RecoreLog(strlog, iRet != CMD_RESULT_OK);

	return iRet;
}

int CDeviceControl::EndMsg(vector<string > listDevice)
{
	m_resultTask.clear();
	int nRet = 0;
	for (int i = 0; i < listDevice.size();++i)
	{
		CString strlog;
		string ipStr = GetIpStrFromList(listDevice[i]);
		TASK_LIST tl = GetDefaultTask(listDevice[i].c_str());
		if (ipStr.length() < 5)
		{
			m_resultTask.push_back(tl);
			strlog.Format("取消紧急消息失败！ 设备编号=%s,ip=%s", listDevice[i].c_str(), ipStr.c_str());
			RecoreLog(strlog,true);
			continue;
		}

		nRet = EndMsg(ipStr.c_str());
		 if (nRet == CMD_RESULT_OK)
		 {
			 tl.iExecResult = 1;
		 }
		 m_resultTask.push_back(tl);

		 strlog.Format("取消紧急消息%s！ 设备编号=%s,ip=%s", nRet == CMD_RESULT_OK ? "成功" : "失败", listDevice[i].c_str(), ipStr.c_str());
		 RecoreLog(strlog, nRet != CMD_RESULT_OK);
	}
	return 0;
}

int CDeviceControl::ExcetCmd(const char *pIpStr, CTRL_CMD cmdID,int param)
{
	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			return -1;
		}
	}
	PVOID pVoid = NULL;
	int iRet = CMD_RESULT_LOCAL_ERROR;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_ExecuteCmd != NULL)
	{
		pVoid = m_cclib.ctrl_InitDll(pIpStr, "", 255);
		iRet = m_cclib.ctrl_ExecuteCmd(pVoid, cmdID, param, NULL, 0);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);

	}


	return iRet;
}

int CDeviceControl::ExcetCmd(vector<string > listDevice, CTRL_CMD cmdID, int param)
{
	m_resultTask.clear();
	int nRet = 0;

	CString strType;

	switch (cmdID)
	{
	case CTRL_CMD_SWITCH_DEVICE:
		strType = param == 1 ? "开机" : "关机";
		break;
	case CTRL_CMD_SWITCH_SCREEN:
		strType = param == 1 ? "开屏" : "关屏";
		break;
	case CTRL_CMD_END_CONTENT:
		strType = "取消紧急消息";
		break;
	case CTRL_CMD_SWITCH_RUNNING:
		strType = param == 1 ? "开始播放" : "停止播放";
		break;
	case CTRL_CMD_TAKE:
		strType = "Take 命令";
		break;
	case CTRL_CMD_RESET_DEVICE:
		strType = "复位";
		break;
	case CTRL_CMD_SWITCH_SOUND:
		strType == param == 1 ? "打开声音" : "关闭声音";
		break;
	case CTRL_CMD_SET_VOLUME:
		strType.Format("设置音量%d", param);
		break;
	case CTRL_CMD_SWITCH_LIVE:
		strType == param == 1 ? "开始直播" : "结束直播";
		break;
	case CTRL_CMD_WAKEUP_DEVICE:
		strType.Format("唤醒其他设备%d", param);
		break;
	case CTRL_CMD_HD_SPACE:
		strType = "是否支持硬盘属性";
		break;
	default:
		strType = "不支持的类型";
		break;
	}

	for (int i = 0; i < listDevice.size();++i)
	{
		CString strlog;
		string ipStr = GetIpStrFromList(listDevice[i]);
		TASK_LIST tl = GetDefaultTask(listDevice[i].c_str());
		if (ipStr.length() < 5)
		{
			m_resultTask.push_back(tl);
			strlog.Format("%s失败！ 设备编号=%s,ip=%s", strType,listDevice[i].c_str(), ipStr.c_str());
			RecoreLog(strlog, true);
			continue;
		}

		nRet = ExcetCmd(ipStr.c_str(),cmdID, param);
		if (nRet == CMD_RESULT_OK)
		{
			tl.iExecResult = 1;
		}
		m_resultTask.push_back(tl);

		strlog.Format("%s%s！ 设备编号=%s,ip=%s", strType,nRet == CMD_RESULT_OK ? "成功" : "失败", listDevice[i].c_str(), ipStr.c_str());
		RecoreLog(strlog, nRet != CMD_RESULT_OK);
	}
	return 0;
}

int CDeviceControl::QuerySingleStatus(const char* szDeviceID, CHECK_STATUS_PARAM &deStatue, PLAY_CONTENT &PlayContent, NET_ES_PARAM &esParam)
{
	string ipStr = GetIpStrFromList(szDeviceID);
	if (ipStr.length() < 5)
	{
		return -1;
	}
	int nRet = GetDeviceStatus(ipStr.c_str(), deStatue);
	int iRet = GetDevicePlayContent(ipStr.c_str(), PlayContent);
	iRet = GetDeviceEmsContent(ipStr.c_str(), esParam);
	return  nRet;
}

BOOL CDeviceControl::GetCmdResult(const string &cmdID, vector<TASK_LIST> &resultList, int receiveCount /*= 10000*/)
{
	resultList.clear();
	for (int i =0;i<m_resultTask.size();++i)
	{
		resultList.push_back(m_resultTask[i]);
	}
	return true;
}

int CDeviceControl::GetDeviceStatus(const char *ipStr, CHECK_STATUS_PARAM &param)
{
	memset(&param, 0, sizeof(CHECK_STATUS_PARAM));
	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			return -1;
		}
	}
	int iRet = CMD_RESULT_LOCAL_ERROR;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_CheckStatus != NULL)
	{
		PVOID pVoid = NULL;
		pVoid = m_cclib.ctrl_InitDll(ipStr, "", 255);
		iRet = m_cclib.ctrl_CheckStatus(pVoid, &param);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);
	}

	return iRet;
}

int CDeviceControl::GetDevicePlayContent(const char *ipStr, PLAY_CONTENT &param)
{
	memset(&param, 0, sizeof(PLAY_CONTENT));
	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			return -1;
		}
	}
	int iRet = CMD_RESULT_LOCAL_ERROR;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_GetPlayContent != NULL)
	{
		PVOID pVoid = NULL;
		pVoid = m_cclib.ctrl_InitDll(ipStr, "", 255);
		iRet = m_cclib.ctrl_GetPlayContent(pVoid, &param);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);
	}

	return iRet;
}

int CDeviceControl::GetDeviceEmsContent(const char *ipStr, NET_ES_PARAM &param)
{
	memset(&param, 0, sizeof(NET_ES_PARAM));
	if (!m_cclib.isLoadSuccess())
	{
		//加载失败
		bool bRet = m_cclib.LoadFunction(m_dllPath.c_str());
		if (!bRet)
		{
			return -1;
		}
	}
	int iRet = CMD_RESULT_LOCAL_ERROR;
	if (m_cclib.ctrl_InitDll != NULL && m_cclib.ctrl_GetContent != NULL)
	{
		PVOID pVoid = NULL;
		pVoid = m_cclib.ctrl_InitDll(ipStr, "", 255);
		iRet = m_cclib.ctrl_GetContent(pVoid, &param);
		if (pVoid != NULL)
			m_cclib.ctrl_UninitDll(pVoid);
	}

	return iRet;
}

void CDeviceControl::RecoreLog(const char *str, bool isError)
{
	if (m_logCB)
	{
		m_logCB(str, m_logCBParam,isError);
	}
}

string CDeviceControl::GetIpStrFromList(string deviceStr)
{
	
	for (int i = 0 ;i < m_mapList.size();++i)
	{
		if (m_mapList[i].first == deviceStr )
		{
			return m_mapList[i].second;
		}
	}
	string str;
	return str;
}

