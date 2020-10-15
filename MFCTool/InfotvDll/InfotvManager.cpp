#include "stdafx.h"
#include "InfotvManager.h"
#include "io.h"
UINT CInfotvManager::__maxInstanceNum = 0;
BOOL CInfotvManager::__gbPlayMode = false;
CInfotvHandle_Parent * CInfotvManager::__infotv = NULL;
CConfigDlg_Parent * CInfotvManager::__cfgDlg = NULL;
SYSTEM_PARAM_Parent *CInfotvManager::__sysParam = NULL;

UINT CInfotvManager::__guMaxNum = 0;
UINT CInfotvManager::__giWidth = 0;
UINT CInfotvManager::__giHeight = 0;

CUIntArray CInfotvManager::__guInstance;
HBITMAP CInfotvManager::__hBmp[3] = { 0 };

char CInfotvManager::__szTempName[50] = "";
char CInfotvManager::__szInfotvVersion[50] = "";

CString  CInfotvManager::__strModuleName = "";
CString CInfotvManager::__strCurDir = "";

CInfotvManager::CInfotvManager()
{
}


CInfotvManager::~CInfotvManager()
{
}

bool CInfotvManager::IsExistFile(LPCTSTR szFile)
{
	return (szFile[0] != 0 && _access(szFile, 0) != -1);
}

void CInfotvManager::SetInstance(CInfotvHandle_Parent *infotv)
{
	__infotv = infotv;
}


void CInfotvManager::SetParamInstance(SYSTEM_PARAM_Parent *param)
{
	__sysParam = param;
}

void CInfotvManager::SetDlgInstance(CConfigDlg_Parent *dlg)
{
	__cfgDlg = dlg;
}

void CInfotvManager::SetCreatMaxNum(UINT maxNum)
{
	__maxInstanceNum = maxNum;
}

void CInfotvManager::SetCurDirAndModueName(CString dir, CString name)
{
	__strCurDir = dir;
	__strModuleName = name;
}

void CInfotvManager::SetDescription(HBITMAP * pHBmp, char * szBuf)
{
	// »ñµÃÍ¼Æ¬¾ä±ú
	for (int i = 0; i < 3; i++)
	{
		__hBmp[i] = pHBmp[i];
	}

	sprintf(__szTempName, szBuf);

}

CInfotvHandle_Parent * CInfotvManager::GetInstance()
{
	return __infotv;
}

CConfigDlg_Parent * CInfotvManager::GetDlgInstance()
{
	return __cfgDlg;
}

SYSTEM_PARAM_Parent * CInfotvManager::GetParamInstance()
{
	return __sysParam;
}
