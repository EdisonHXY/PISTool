#include "stdafx.h"
#include "InfotvInterface.h"

#include "GhgCopier.h"



static UINT FindNextIndex()
{
	int i, iSize = CInfotvManager::__guInstance.GetSize();
	UINT uIndex = 0;
	GHG_PARAM * pParam;

	if (iSize == 0)
	{
		uIndex = 0;
	}
	else if (iSize == ((int)CInfotvManager::__guMaxNum + 1))
	{
		uIndex = CInfotvManager::__guMaxNum + 1;
	}
	else
	{
		for (i = 0; i < iSize; i++)
		{
			pParam = (GHG_PARAM*)CInfotvManager::__guInstance[i];
			if (pParam->uIndex > (UINT)i)
			{
				uIndex = i;
				break;
			}
		}
		if (i == iSize) uIndex = iSize;
	}

	return uIndex;
}

static BOOL StartDraw(HGHG hGhg)
{
	int iLeft, iTop, iWidth, iHeight;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	iLeft = pParam->sysParam->iPosX;
	iTop = pParam->sysParam->iPosY;
	iWidth = pParam->sysParam->iWidth;
	iHeight = pParam->sysParam->iHeight;
	GDK_SetBound(pParam->context, iLeft, iTop, iWidth, iHeight);
	SYSTEMTIME curTime;

	GetLocalTime(&curTime);
	CInfotvManager::GetInstance()->DrawContent(pParam, curTime,0);
	return TRUE;
}

static BOOL EndDraw(HCONTEXT context)
{
	GDK_ClearScreen(context);
	GDK_Clear(context);
	return TRUE;
}



void ghg_UninitDll()
{

}

UINT ghg_MainVersion(LPCTSTR szMainVersion)
{
	sprintf(CInfotvManager::__szInfotvVersion, szMainVersion);
	return 0;
}

//初始化程序
UINT ghg_IsValidDll(void * pBoardParam, HWND hMain, UINT uMsg, BOOL bPlayMode, int iWidth, int iHeight)
{
	CInfotvManager::__gbPlayMode = bPlayMode;
	CInfotvManager::__giWidth = iWidth;
	CInfotvManager::__giHeight = iHeight;
	return 0x10000134;
}

BOOL ghg_IsMoreModule()
{
	if (CInfotvManager::__guInstance.GetSize() < CInfotvManager::__maxInstanceNum)
		return TRUE;
	else
		return FALSE;
}
BOOL ghg_IsModeSupported(int iBoardMode)
{
	return TRUE;
}

// 创建和释放模块实例
int  ghg_CreateModule(HGHG &hGhg, int iModuleNum, int iBoardMode)
{
	if (CInfotvManager::GetInstance() == NULL)
	{
		return GHG_RESULT_ERROR;
	}


	int i, iIndex;
	char szBuf[MAX_PATH];
	BOOL bFind = false;
	UINT uIndex = 0;

	memset(szBuf, 0, MAX_PATH);
	if (!ghg_IsMoreModule()) return GHG_RESULT_MORE_MODULE;

	// 获得最新编号
	int iSize = CInfotvManager::__guInstance.GetSize();
	ASSERT(iSize <= int(CInfotvManager::__guMaxNum + 1));
	if (iModuleNum == -1)
	{
		uIndex = FindNextIndex();
	}
	else
	{
		uIndex = iModuleNum;
		for (i = 0; i < iSize; i++)
		{
			if (((GHG_PARAM*)CInfotvManager::__guInstance[i])->uIndex == uIndex)
			{
				hGhg = (HGHG)CInfotvManager::__guInstance[i];
				if (((GHG_PARAM*)hGhg)->context) return GHG_RESULT_OK;
				bFind = true;
				iIndex = i;
				break;
			}
		}
	}

	// 创建模块
	if (!bFind)
	{
		GHG_PARAM *param = new GHG_PARAM;
		param->sysParam = CInfotvManager::GetParamInstance();
		param->bPlaying = false;
		param->bPause = false;
		
		hGhg = param;
		if (hGhg == NULL) return GHG_RESULT_ERROR;
		
		CInfotvManager::GetInstance()->SetDefaultParam((GHG_PARAM*)hGhg);
		((GHG_PARAM*)hGhg)->uIndex = uIndex;

		for (i = iSize - 1; i >= 0; i--)
		{
			if (((GHG_PARAM*)CInfotvManager::__guInstance[i])->uIndex < uIndex) break;
		}
		iIndex = i + 1;
	}

	// 创建Context
	sprintf(szBuf, "%s%d", CInfotvManager::__strModuleName, uIndex + 1);
	((GHG_PARAM*)hGhg)->context = GDK_CreateArea(szBuf);  //根据该串创建显示区域和初始化板卡事件

	if (((GHG_PARAM*)hGhg)->context == NULL)
	{
		delete ((GHG_PARAM*)hGhg);
		hGhg = NULL;
		return GHG_RESULT_ERROR;
	}

	if (!bFind)
	{
		CInfotvManager::__guInstance.InsertAt(iIndex, (UINT)hGhg);
	}

	if (uIndex > CInfotvManager::__guMaxNum) CInfotvManager::__guMaxNum = uIndex;

	return GHG_RESULT_OK;
}
BOOL ghg_FreeModule(HGHG hGhg)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	// 停止程序，删除Context
	ghg_EndPlay(hGhg);
	::Sleep(100);
	GDK_DestroyArea(pParam->context);

	// 去掉引用
	int iSize = CInfotvManager::__guInstance.GetSize();
	int i = 0;
	for (i = 0; i < iSize; i++)
	{
		if (hGhg == (HGHG)CInfotvManager::__guInstance[i]) break;
	}
	ASSERT(i < iSize);
	CInfotvManager::__guInstance.RemoveAt(i);

	// 释放内存
	delete (GHG_PARAM *)hGhg;

	return TRUE;
}

// 配置设置
BOOL ghg_GetModuleFlag(HGHG hGhg, char * szModule, int & iModuleNum)
{
	ASSERT(hGhg != NULL);
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	// 返回模块名称
	int iSize = _snprintf(szModule, 31, CInfotvManager::__strModuleName);
	szModule[iSize] = 0;

	iModuleNum = pParam->uIndex;

	return TRUE;
}
int  ghg_GetName(HGHG hGhg, char * szName, int iSize)
{
	ASSERT(hGhg != NULL);
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	CString strName;

	// 获得模块名称
	if (strlen(pParam->sysParam->szModule) != 0)
	{
		strName = pParam->sysParam->szModule;
	}
	else
	{
		strName.Format("%s%d", CInfotvManager::__strModuleName, pParam->uIndex);
	}

	int iLen = _snprintf(szName, iSize - 1, strName);
	szName[iLen] = 0;

	return iLen;
}
int  ghg_GetDescription(HBITMAP * pHBmp, char * szBuf, int iSize)
{

	// 获得图片句柄
	for (int i = 0; i < 3; i++)
	{
		pHBmp[i] = CInfotvManager::__hBmp[i];
	}

	//模块描述
	int iLen = _snprintf(szBuf, iSize - 1, CInfotvManager::__szTempName);
	szBuf[iLen] = 0;

	return iLen;
}
int  ghg_ShowConfigDlg(HGHG hGhg)
{

	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//显示参数设置对话框
	int iRet = 0;
	if (CInfotvManager::GetDlgInstance() == NULL)
	{
		//没有设置配置界面
		return 0;
	}

	CInfotvManager::GetDlgInstance()->SetParam(pParam->sysParam,CInfotvManager::__giWidth,CInfotvManager::__giHeight);    //根据全局参数变量初始化
	iRet = CInfotvManager::GetDlgInstance()->DoModal();
	if (iRet == IDOK)
	{
		CInfotvManager::GetDlgInstance()->GetParam(pParam->sysParam);  //从对话框得到配置参数
	}
	return iRet;

}


//设置模块位置和获取
BOOL ghg_GetPosRect(HGHG hGhg, RECT & rc)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	//坐标系转换
	rc.left = pParam->sysParam->iPosX;
	rc.right = rc.left + pParam->sysParam->iWidth;
	rc.top = CInfotvManager::__giHeight - pParam->sysParam->iPosY - pParam->sysParam->iHeight;
	rc.bottom = rc.top + pParam->sysParam->iHeight;
	return true;
}

BOOL ghg_SetPosRect(HGHG hGhg, RECT rc)
{
	int iPosX, iPosY, iWidth, iHeight;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	//坐标系转换
	iPosX = rc.left;
	iPosY = CInfotvManager::__giHeight - rc.bottom;
	iWidth = rc.right - rc.left;
	iHeight = rc.bottom - rc.top;
	if (iPosX != pParam->sysParam->iPosX || iWidth != pParam->sysParam->iWidth
		|| iPosY != pParam->sysParam->iPosY || iHeight != pParam->sysParam->iHeight)
	{
		pParam->sysParam->iPosX = iPosX;
		pParam->sysParam->iWidth = iWidth;
		pParam->sysParam->iPosY = iPosY;
		pParam->sysParam->iHeight = iHeight;
		return true;
	}
	return false;
}

void ghg_SetStopMode(HGHG hGhg, BOOL bClearScreen)
{
	ASSERT(hGhg != NULL);
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	pParam->bCleanScreenOnStop = bClearScreen;
	return;
}
int  ghg_GetPlayContent(HGHG hGhg, char * szContent, int iBufSize)
{
	int iSize, iIndex;
	CString strRet;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (pParam->bPlaying)
	{
		strRet = CInfotvManager::__szTempName;
	}
	else
	{
		strRet = "未播出";
	}

	iSize = strRet.GetLength();
	if (szContent == NULL) return iSize;

	iIndex = sprintf(szContent, strRet.Left(iBufSize - 1));
	szContent[iIndex] = 0;

	return iIndex;
}


// 关联数据处理
BOOL ghg_CopyData(HGHG hGhg)
{
	ASSERT(hGhg);

	CGhgCopier copier;
	GHG_PARAM * pParam = (GHG_PARAM*)hGhg;

	if (!copier.Setup((GHG_PARAM*)hGhg)) return FALSE;

	// 添加文件
	vector<string> fileList;
	fileList.clear();
	CInfotvManager::GetInstance()->GetNeedAddFiles(pParam,fileList);

	for (int i = 0 ;i < fileList.size();++i)
	{
		copier.AddFile(fileList[i].data());
	}

// 	if (pParam->sysParam.iBkType == FILL_TYPE_PICTURE)
// 	{
// 		copier.AddFile(pParam->sysParam.szBkFile);
// 	}
// 	else
// 	{
// 		sprintf(pParam->sysParam.szBkFile, "");
// 	}

	if (copier.GetArrayIndex() > 0)
	{
		copier.PrepareCopy();
		if (!copier.Verify()) return FALSE;
		if (!copier.Copy()) return FALSE;

		// 更新数据
// 		if (pParam->sysParam.iBkType == FILL_TYPE_PICTURE)
// 		{
// 			sprintf(pParam->sysParam.szBkFile, copier.GetNewFile(0));
// 		}

		CInfotvManager::GetInstance()->UpdateAddFileParam(pParam,copier.GetDstArr());


		copier.WriteFile();
	}
	// 确定路径
	sprintf(pParam->szOldPath, pParam->szCurPath);

	return TRUE;
}
BOOL ghg_RemoveData(HGHG hGhg)
{
	BOOL bFind = TRUE, bRet = true;
	CString strFile, strPath;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	if (hGhg == NULL)
	{
		sprintf(pParam->szErrorString, "RemoveData函数传递参数无效!");
		return FALSE;
	}

	strPath = pParam->szCurPath;
	if (strPath.Right(1) != "\\") strPath += "\\";
	strFile = strPath + "*.*";

	HANDLE hFind;
	WIN32_FIND_DATA	wfData;

	hFind = FindFirstFile(strFile, &wfData);
	if (hFind == INVALID_HANDLE_VALUE) return TRUE;

	do
	{
		if (bRet && !DeleteFile(strPath + wfData.cFileName))
		{
			bRet = false;
			sprintf(pParam->szErrorString, "删除部分文件失败");
		}
		bFind = FindNextFile(hFind, &wfData);
	} while (bFind);

	FindClose(hFind);
	return TRUE;
}
void ghg_SetCurrentPath(HGHG hGhg, LPCTSTR szPath)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	int iLen = _snprintf(pParam->szCurPath, MAX_PATH - 1, szPath);

	if (pParam->szCurPath[iLen - 1] != '\\')
	{
		if (iLen != MAX_PATH - 1)
		{
			pParam->szCurPath[iLen] = '\\';
			iLen++;
		}
		else
		{
			pParam->szCurPath[iLen - 1] = '\\';
		}
	}
	pParam->szCurPath[iLen] = 0;
	if (pParam->szOldPath[0] == 0) sprintf(pParam->szOldPath, pParam->szCurPath);

}
BOOL ghg_SaveConfig(HGHG hGhg)
{
	ASSERT(hGhg != NULL);
	
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	CString strFile, strTemp;

	// 获得保存文件名称
	strFile = pParam->szCurPath;
	if (strFile.GetLength() == 0)
	{
		sprintf(pParam->szErrorString, "没有指定保存路径!");
		return FALSE;
	}
	if (strFile.Right(1) != "\\") strFile += "\\";
	strTemp.Format("%s%d.ini", CInfotvManager::__strModuleName, pParam->uIndex);
	strFile += strTemp;

	//先清除
	DeleteFile(strFile);
	int iIndex = strFile.ReverseFind('.');
	return	CInfotvManager::GetInstance()->SaveParamToFile((pParam->sysParam), strFile, strFile.Left(iIndex + 1) + "XML");


}
BOOL ghg_ReadConfig(HGHG hGhg, LPCTSTR szPath)
{
	int iLen;
	BOOL bPlaying;
	CString strFile, strPath;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	// 获得保存文件名称
	strPath = szPath;
	if (strPath.GetLength() == 0)
	{
		sprintf(pParam->szErrorString, "没有指定保存路径!");
		return FALSE;
	}
	if (strPath.Right(1) != "\\")
	{
		strPath += "\\";
	}
	strFile.Format("%s%d.ini", CInfotvManager::__strModuleName, pParam->uIndex);
	strFile = strPath + strFile;

	if (!CInfotvManager::IsExistFile(strFile))
	{
		return TRUE;
	}

	bPlaying = pParam->bPlaying;
	if (bPlaying)
	{
		ghg_EndPlay(hGhg);
	}

	iLen = _snprintf(pParam->szCurPath, MAX_PATH - 1, strPath);
	pParam->szCurPath[iLen] = 0;
	//read param
	//

	CInfotvManager::GetInstance()->ReadParamFromFile((pParam->sysParam), strFile);

	//process playing
	if (bPlaying)
	{
		if (!ghg_PrepareData(hGhg, true))
		{
			return FALSE;
		}
		if (!ghg_StartPlay(hGhg, true))
		{
			return FALSE;
		}
	}

	return TRUE;
}
void ghg_SetMediaPath(LPCTSTR szPath)
{
	return;
}

// 内容设置
int  ghg_GetModuleConfig(HGHG hGhg, void * pData)
{
	return 0;
	ASSERT(hGhg != NULL);
// 	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
// 
// 	if (pData != NULL)
// 	{
// 		memcpy(pData, &(pParam->sysParam), sizeof(SYSTEM_PARAM));
// 	}
// 
// 	return sizeof(SYSTEM_PARAM);
}

BOOL ghg_SetModuleConfig(HGHG hGhg, void * pData, int iSize, BOOL bReadFile)
{
	return TRUE;
}

// 播放相关
int  ghg_PrepareData(HGHG hGhg, BOOL bForce)
{
	BOOL bRet = TRUE;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!CInfotvManager::__gbPlayMode) return PLAY_RESULT_ONLY_EDIT;
	if (pParam->sysParam->bStartByOther && !bForce) return PLAY_RESULT_NEED_FORCE;

	bRet = CInfotvManager::GetInstance()->VerifyPlayData(pParam);

	if (bRet) return PLAY_RESULT_OK;
	else return PLAY_RESULT_ERROR;

}
int  ghg_StartPlay(HGHG hGhg, BOOL bForce)
{
	BOOL bRet = TRUE;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!CInfotvManager::__gbPlayMode) return PLAY_RESULT_ONLY_EDIT;
	if (pParam->sysParam->bStartByOther && !bForce) return PLAY_RESULT_NEED_FORCE;
	if (pParam->bPlaying) return PLAY_RESULT_OK;

	CInfotvManager::GetInstance()->StarPlay(pParam);

	 bRet = StartDraw(hGhg);
	if (bRet)
	{
		pParam->bPlaying = TRUE;
		GDK_PlayNormal(pParam->context, true);
	}

	if (bRet) return PLAY_RESULT_OK;
	else return PLAY_RESULT_ERROR;
}
BOOL ghg_EndPlay(HGHG hGhg)
{

	BOOL bRet = TRUE;
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!pParam->bPlaying) { return TRUE; }

	GDK_PlayNormal(pParam->context, false);

	CInfotvManager::GetInstance()->EndPlay(pParam);

	if (pParam->bCleanScreenOnStop)
	{
		bRet = EndDraw(pParam->context);
	}
	if (bRet)
	{
		pParam->bPause = false;
		pParam->bPlaying = false;
	}
	return bRet;
}
BOOL ghg_ApplyParam(HGHG hGhg)
{
	ASSERT(hGhg != NULL);
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!pParam->bPlaying) return TRUE;

	if (!ghg_EndPlay(hGhg)) return FALSE;
	if (PLAY_RESULT_OK != ghg_PrepareData(hGhg, true)) return FALSE;
	if (PLAY_RESULT_OK != ghg_StartPlay(hGhg, true)) return FALSE;

	return TRUE;
}
BOOL ghg_PausePlay(HGHG hGhg)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!pParam->bPlaying || pParam->bPause)
	{
		return TRUE;
	}

	pParam->bPause = TRUE;
	return TRUE;
}
BOOL ghg_ReStartPlay(HGHG hGhg)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!pParam->bPlaying || !pParam->bPause)
		return TRUE;

	StartDraw(hGhg);
	pParam->bPause = FALSE;

	return TRUE;
}
int  ghg_GetModuleStatus(HGHG hGhg)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;

	if (!pParam->bPlaying)
	{
		return MODULE_STATUS_STOP;
	}
	else
	{
		if (pParam->bPause)
		{
			return MODULE_STATUS_PAUSE;
		}
		else
		{
			return MODULE_STATUS_PLAY;
		}
	}

	return -1;
}
int  ghg_ProcessKeyMsg(HGHG hGhg, UINT uKeyCode, BOOL bCtrlDown, BOOL bShiftDown)
{
	return 0;
}
void ghg_ProcessTimer(HGHG hGhg, SYSTEMTIME sysTime, UINT uInterval)
{
	GHG_PARAM *pParam = (GHG_PARAM *)hGhg;
	if (!pParam->bPlaying)
	{
		return;
	}

	CInfotvManager::GetInstance()->DrawContent((GHG_PARAM *)hGhg,sysTime,uInterval);
}
BOOL ghg_ProcessAudio(HGHG hGhg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return TRUE;
}


// 错误处理	
int  ghg_GetErrorMsg(HGHG hGhg, char * szMsg, int iSize)
{
	GHG_PARAM * pParam = (GHG_PARAM *)hGhg;
	int iLen = strlen(pParam->szErrorString);

	iLen = min(iLen, iSize - 1);
	strncpy(szMsg, pParam->szErrorString, iLen);
	szMsg[iLen] = 0;

	return iLen;
}
