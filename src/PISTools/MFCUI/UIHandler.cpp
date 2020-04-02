#include "stdafx.h"
#include "UIHandler.h"



CUIHandler::CUIHandler()
{
	m_statusBar = NULL;
}



CUIHandler::~CUIHandler()
{
}


static NOTIFYICONDATA g_NotifyIcon;
static long g_lOldProc = -1;
static long g_lOldProc_Close = -1;
static CString g_messageStr;
#pragma comment(lib, "Version.lib ")
CString CUIHandler::GetVersionInfo(CString szExeFile)
{
	DWORD dwHandle, dwSize = 0;
	PBYTE pInfoData;
	PTCH pData;
	UINT uDataSize;
	CString strVersion = _T("");
	dwSize = GetFileVersionInfoSize(szExeFile, &dwHandle);
	if (dwSize)
	{
		pInfoData = new BYTE[dwSize];
		if (pInfoData)
		{
			if (GetFileVersionInfo(szExeFile, 0, dwSize, pInfoData))
			{	//copyright info
				VerQueryValue(pInfoData, _T("\\StringFileInfo\\080404b0\\LegalCopyright"), (void **)&pData, &uDataSize);
				//m_strCopyRight = pData;
				//company
				VerQueryValue(pInfoData, _T("\\StringFileInfo\\080404b0\\CompanyName"), (void **)&pData, &uDataSize);
				//m_strCompany = pData;
				//Product name
				VerQueryValue(pInfoData, _T("\\StringFileInfo\\080404b0\\ProductName"), (void **)&pData, &uDataSize);
				//m_strVersion = pData;
				//version info
				VerQueryValue(pInfoData, _T("\\StringFileInfo\\080404b0\\FileVersion"), (void **)&pData, &uDataSize);
				strVersion = strVersion + _T(" ") + pData;
				strVersion.Replace(_T(", "), _T("."));
			}
			delete pInfoData;
		}
	}

	return strVersion;
}

bool CUIHandler::GetExeDirAndFileName(CString &dir, CString &fileName)
{
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);

	CString tmpStr = _tcsrchr(szFilePath, '\\');
	(_tcsrchr(szFilePath, '\\'))[0] = 0;
	dir = szFilePath;
	int iIndex = tmpStr.ReverseFind('.');
	fileName = tmpStr.Mid(1, iIndex - 1);

	return true;
}

void CUIHandler::SaveIniSetting(vector<OC_INI_INFO> &listInfo, CString filePath)
{
	for (unsigned int i = 0; i < listInfo.size(); ++i)
	{
		OC_INI_INFO info = listInfo[i];
		::WritePrivateProfileString(info.headerName, info.keyName, info.valueName, filePath);
	}
}

void CUIHandler::AddStatueBar(CWnd* pParentWnd,int nParts, int* pWidths)
{
	if (m_statusBar || pParentWnd == NULL)
	{
		return;
	}

	m_statusBar = new CStatusBarCtrl;

	CRect rcClient;
	pParentWnd->GetClientRect(rcClient);
	m_statusBar->Create(WS_CHILD | WS_VISIBLE , CRect(0, rcClient.bottom - 20, rcClient.right, rcClient.bottom), pParentWnd, 0);

	if (pWidths)
	{
		m_statusBar->SetParts(nParts, pWidths);
	}
	

}

void CUIHandler::DeleteStatueBar()
{
	if (m_statusBar)
	{
		m_statusBar->DestroyWindow();
		delete m_statusBar;
		m_statusBar = NULL;
	}
}

void CUIHandler::SetStatueBarText(CString title, int nParts)
{
	if (m_statusBar)
	{
		m_statusBar->SetText(title, nParts, 0);
	}
}


LRESULT  CALLBACK  CUIHandler::NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) //判断消息类型
	{
	case WM_NC_OC:
		//如果是用户定义的消息 
		if (lParam == WM_LBUTTONDBLCLK)

		{
			//鼠标双击时主窗口出现 
			if (CWnd::FromHandle(hwnd)->IsWindowVisible()) //判断窗口当前状态
			{
				CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE); //隐藏窗口
			}
			else
			{
				CWnd::FromHandle(hwnd)->ShowWindow(SW_SHOW); //显示窗口
			}

		}
		else if (lParam == WM_RBUTTONDOWN)
		{ //鼠标右键单击弹出选单 
			/*POINT pt; 
			GetCursorPos(&pt);
			int cmd = TrackPopupMenu(g_hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd,
				NULL);
			if (cmd == ID_EXIT) {

				PostMessage(hwnd, WM_DESTROY, NULL, NULL);

			}*/
		}
		break;
	case WM_SYSCOMMAND:
		//如果是系统消息 
		if (wParam == SC_MINIMIZE)
		{
			//接收到最小化消息时主窗口隐藏 
			CWnd::FromHandle(hwnd)->ShowWindow(SW_HIDE);
			return 0;
		}
		
		break;
	}
	
	return CallWindowProc((WNDPROC)g_lOldProc, hwnd, message, wParam, lParam);
}


LRESULT CALLBACK CUIHandler::CloseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		int nRet = AfxMessageBox(g_messageStr, 1);
		if (nRet != 1)
		{
			return 0;
		}

	}


	return CallWindowProc((WNDPROC)g_lOldProc_Close, hwnd, message, wParam, lParam);
}

void CUIHandler::TrayToBottom(HWND hwn, HICON icon, CString tipMsg, UINT nIdMenuRsource /*= 0*/)
{
	if (g_lOldProc != -1)
	{
		return;
	}
	g_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	g_NotifyIcon.hIcon = icon;
	g_NotifyIcon.hWnd = hwn;
	lstrcpy(g_NotifyIcon.szTip, tipMsg);
	g_NotifyIcon.uCallbackMessage = WM_NC_OC;
	g_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &g_NotifyIcon);

	//g_hMenu = CreatePopupMenu();//生成托盘菜单	
	//AppendMenu(g_hMenu, MF_STRING, ID_EXIT, TEXT("退出"));

	g_lOldProc = SetWindowLong(g_NotifyIcon.hWnd, GWL_WNDPROC, long(NewWndProc));

}

void CUIHandler::CancelTrayToBottom(HWND hwn)
{
	if (g_lOldProc == -1 )
	{
		return;
	}
	Shell_NotifyIcon(NIM_DELETE, &g_NotifyIcon); //在托盘区删除图标
	SetWindowLong(hwn, GWL_WNDPROC, g_lOldProc);
	g_lOldProc = -1;
}

int CUIHandler::CloseTip(HWND hwn, CString tipStr)
{
	if (g_lOldProc_Close != -1)
	{
		return 0;
	}
	g_messageStr = tipStr;
	g_lOldProc_Close = SetWindowLong(hwn, GWL_WNDPROC, long(CloseWndProc));
	return 0;
}

void CUIHandler::CancelCloseTip(HWND hwn)
{
	if (g_lOldProc_Close == -1)
	{
		return;
	}

	SetWindowLong(hwn, GWL_WNDPROC, g_lOldProc_Close);
	g_lOldProc_Close = -1;
}

void CUIHandler::ReadIniSettting(vector<OC_INI_INFO> &listInfo, CString filePath)
{
	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件
	BOOL ifFind = finder.FindFile(filePath);
	
	if (!ifFind)
	{
		for (unsigned int i = 0 ; i < listInfo.size();++i)
		{
			OC_INI_INFO info = listInfo[i];
			::WritePrivateProfileString(info.headerName, info.keyName, info.defaultName,filePath);
		}
	}

	for (unsigned int i = 0; i < listInfo.size(); ++i)
	{
		OC_INI_INFO &info = listInfo[i];
		::GetPrivateProfileString(info.headerName, info.keyName, info.defaultName,info.valueName.GetBuffer(MAX_PATH),MAX_PATH, filePath);
	}
}
