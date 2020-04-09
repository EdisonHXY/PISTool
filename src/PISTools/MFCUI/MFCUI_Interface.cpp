
#include "stdafx.h"
#include "..\对外接口\MFCUI_Interface.h"
#include "UIHandler.h"
CUIHandler m_uiHandler;


int OC_GetVersionInfo(CString szExeFile, CString &szVersion)
{

	szVersion = m_uiHandler.GetVersionInfo(szExeFile);
	return OC_ERROR_CODE_NONE;
}

bool OC_GetExeDirAndFileName(CString &dir, CString &fileName)
{
	return m_uiHandler.GetExeDirAndFileName(dir, fileName);
}

int OC_GetCurrentVersionInfo(CString &szVersion)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	szVersion = m_uiHandler.GetVersionInfo(szFilePath);
	return OC_ERROR_CODE_NONE;
}

int OC_ReadCurrentCfgFromIni(vector<OC_INI_INFO> &listInfo, CString fileName)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, '\\'))[0] = 0;
	CString filePath = szFilePath;
	filePath += "\\" + fileName;
	m_uiHandler.ReadIniSettting(listInfo, filePath);
	return OC_ERROR_CODE_NONE;
}

int OC_SaveCurrentCfgToIni(vector<OC_INI_INFO> &listInfo, CString fileName)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	(_tcsrchr(szFilePath, '\\'))[0] = 0;
	CString filePath = szFilePath;
	filePath += "\\" + fileName;
	m_uiHandler.SaveIniSetting(listInfo, filePath);
	return OC_ERROR_CODE_NONE;
}

 BOOL OC_IsMutexCreated(CString strMutex)
{
	 return CUIHandler::IsMutexCreated(strMutex);
}

int OC_ReadCfgFromIni(vector<OC_INI_INFO>  &listInfo, CString filePath)
{

	m_uiHandler.ReadIniSettting(listInfo, filePath);
	return OC_ERROR_CODE_NONE;
}

int OC_SaveCfgToIni(vector<OC_INI_INFO>  &listInfo, CString filePath)
{
	m_uiHandler.SaveIniSetting(listInfo, filePath);
	return OC_ERROR_CODE_NONE;
}

int OC_AddStatueBar(CWnd* pParentWnd, int nParts /*= 1*/, int* pWidths /*= NULL*/)
{
	if (pParentWnd == NULL)
	{
		pParentWnd = AfxGetMainWnd();
		// return OC_ERROR_CODE_PARAM;
	}
	m_uiHandler.AddStatueBar(pParentWnd, nParts, pWidths);
	return OC_ERROR_CODE_NONE;
}

DllExport int OC_DeleteStatueBar()
{
	m_uiHandler.DeleteStatueBar();

	return OC_ERROR_CODE_NONE;
}

int OC_SetStatueBarText(CString title, int nParts /*= 0*/)
{
	m_uiHandler.SetStatueBarText(title, nParts);
	return OC_ERROR_CODE_NONE;
}

int OC_TrayToBottom(HWND hwn, HICON icon, CString tipMsg)
{
	if (hwn == NULL)
	{
		return OC_ERROR_CODE_PARAM;
	}
	m_uiHandler.TrayToBottom(hwn, icon, tipMsg, 0);
	return OC_ERROR_CODE_NONE;
}

int OC_CancelTrayToBottom(HWND hwn)
{
	if (hwn == NULL)
	{
		return OC_ERROR_CODE_PARAM;
	}
	m_uiHandler.CancelTrayToBottom(hwn);
	return OC_ERROR_CODE_NONE;
}

DllExport int OC_CloseTip(HWND hwn, CString tipStr)
{

	m_uiHandler.CloseTip(hwn, tipStr);
	return OC_ERROR_CODE_NONE;
}

DllExport void OC_CancelCloseTip(HWND hwn)
{
	m_uiHandler.CancelCloseTip(hwn);
	
}

