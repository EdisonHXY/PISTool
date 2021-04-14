#include "stdafx.h"
#include "MFCTools.h"

#define WM_NC_OC (WM_USER+1101)

//声明static 变量
long CMFCTools::m_lOldProc = -1;
NOTIFYICONDATA CMFCTools::m_NotifyIcon;
long CMFCTools::m_lOldProc_Close = -1;
CString CMFCTools::m_messageStr="";
UINT CMFCTools::m_timerID = 1058;
CWnd *CMFCTools::m_applyControl = NULL;
vector<CMFCTools::OC_INI_INFO> CMFCTools::m_iniListInfo;

#ifdef _WIN64 
#define MYGWL_WNDPROC GWLP_WNDPROC
#else 
#define MYGWL_WNDPROC GWL_WNDPROC
#endif

CMFCTools::CMFCTools()
{
	m_statusBar = NULL;
	m_listBox = 0;
}


CMFCTools * CMFCTools::GetInstance()
{
	static CMFCTools tools;
	return &tools;
}

CMFCTools::~CMFCTools()
{
	StopListenControl();
}

#pragma comment(lib, "Version.lib ")
CString CMFCTools::GetVersionInfo(CString szExeFile)
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



void CMFCTools::SetLogListBox(CListTextBox *listBox)
{
	m_listBox = listBox;
}

void CMFCTools::AddToMsgToListBox(CString strLog, int nLevel /*= 0*/)
{

	if (!m_listBox)
	{
		return;
	}
	
	CTime tmCur = CTime::GetCurrentTime();
	CString strLogMsg;
	strLogMsg.Format("%s:%s",  tmCur.Format("%H:%M:%S"),strLog);
	
	COLORREF itemColor = RGB(10, 200, 10);
	if (nLevel <=0)
	{
		itemColor = RGB(10, 200, 10);
	}else if (nLevel ==1)
	{
		itemColor = RGB(200, 200, 10);
	}
	else {
		itemColor = RGB(200, 10, 10);
	}


	m_listBox->SetCenter(false);

	m_listBox->SetRedraw(FALSE);
	if (m_listBox->GetCount() > 300)
		m_listBox->DeleteString(0);

	m_listBox->AddString(strLogMsg, itemColor);
	m_listBox->SetRedraw(TRUE);

	m_listBox->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void CMFCTools::OpenPath(const string &strPath,bool bFile)
{
	ShellExecute(NULL, bFile ? "open": "explore", strPath.data(), NULL, NULL, SW_SHOWNORMAL);
}

void CMFCTools::PrintFile(const string &strPath)
{
	ShellExecute(NULL,  "print", strPath.data(), NULL, NULL, SW_SHOWNORMAL);
}

LRESULT CALLBACK CMFCTools::NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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

		break;
	}

	return CallWindowProc((WNDPROC)CMFCTools::m_lOldProc, hwnd, message, wParam, lParam);
}

BOOL CMFCTools::IsMutexCreated(CString strMutex)
{
	DWORD dwReturn;

	//首先判断是否已经运行，只允许一个实例存在
	HANDLE hMutex = CreateMutex(NULL, true, strMutex);
	dwReturn = GetLastError();
	if (!hMutex || dwReturn == ERROR_ALREADY_EXISTS)
	{
		//AfxMessageBox("不能同时运行InfoTV监视程序!", MB_OK | MB_ICONINFORMATION);
		return true;
	}
	return false;
}

bool CMFCTools::SetTitleWithVersion(CWnd* hd)
{
	if (hd == NULL)
	{
		hd = AfxGetMainWnd();
	}

	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL, szFilePath, MAX_PATH);
	CString versionStr = GetVersionInfo(szFilePath);
	CString titleStr;
	if (hd)
	{
		hd->GetWindowText(titleStr);
		hd->SetWindowText(titleStr + versionStr);
	}
	else
	{
		return false;
	}
	return true;
}

bool CMFCTools::GetExeDirAndFileName(CString &dir, CString &fileName)
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

int CMFCTools::SetStatueBarText(CString title, int nParts /*= 0*/)
{
	if (m_statusBar)
	{
		m_statusBar->SetText(title, nParts, 0);
		return 0;
	}
	else
	{
		return -1;
	}
}

int CMFCTools::AddStatueBar(CWnd* pParentWnd, int nParts /*= 0*/, int* pWidths /*= NULL*/)
{
	if (m_statusBar)
	{
		return 0;
	}

	if (pParentWnd == NULL)
	{
		pParentWnd = AfxGetMainWnd();
		
	}
	if (pParentWnd == NULL)
	{
		return -1;
	}
	
	m_statusBar = new CStatusBarCtrl;

	CRect rcClient;
	pParentWnd->GetClientRect(rcClient);
	m_statusBar->Create(WS_CHILD | WS_VISIBLE, CRect(0, rcClient.bottom - 20, rcClient.right, rcClient.bottom), pParentWnd, 0);

	if (pWidths)
	{
		m_statusBar->SetParts(nParts, pWidths);
	}

}

int CMFCTools::DeleteStatueBar()
{
	if (m_statusBar)
	{
		m_statusBar->DestroyWindow();
		delete m_statusBar;
		m_statusBar = NULL;
	}
	return 0;
}

void CMFCTools::TrayToBottom(HWND hwn, HICON icon, CString tipMsg, bool isStartMin, UINT nIdMenuRsource /*= 0*/)
{
	
	if (m_lOldProc != -1)
	{
		return;
	}

	m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hIcon = icon;
	m_NotifyIcon.hWnd = hwn;
	lstrcpy(m_NotifyIcon.szTip, tipMsg);
	m_NotifyIcon.uCallbackMessage = WM_NC_OC;
	m_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);

	//g_hMenu = CreatePopupMenu();//生成托盘菜单	
	//AppendMenu(g_hMenu, MF_STRING, ID_EXIT, TEXT("退出"));

	
	m_lOldProc = SetWindowLong(m_NotifyIcon.hWnd, MYGWL_WNDPROC, long(NewWndProc));

	if (isStartMin)
	{
		PostMessage(hwn,WM_SYSCOMMAND, SC_MINIMIZE, 0);
	}

}

void CMFCTools::CancelTrayToBottom(HWND hwn)
{
	if (m_lOldProc == -1)
	{
		return;
	}
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon); //在托盘区删除图标
	SetWindowLong(hwn, MYGWL_WNDPROC, m_lOldProc);
	m_lOldProc = -1;
}

int CMFCTools::CloseTip(HWND hwn, CString tipStr)
{
	if (m_lOldProc_Close != -1)
	{
		return 0;
	}
	m_messageStr = tipStr;
	m_lOldProc_Close = SetWindowLong(hwn, MYGWL_WNDPROC, long(CloseWndProc));
	return 0;
}

void CMFCTools::CancelCloseTip(HWND hwn)
{
	if (m_lOldProc_Close == -1)
	{
		return;
	}

	SetWindowLong(hwn, MYGWL_WNDPROC, m_lOldProc_Close);
	m_lOldProc_Close = -1;
}

VOID CALLBACK CMFCTools::TimerFunc(HWND wnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	if (nIDEvent == m_timerID)
	{
		bool bEnable = false;
		for (unsigned int i = 0; i < m_iniListInfo.size();++i)
		{
			OC_INI_INFO &info = m_iniListInfo[i];
			if (info.wndControl)
			{
				if (info.nType == InfoType_Text)
				{
					CString str;
					info.wndControl->GetWindowText(str);
					if (str != info.valueName)
					{
						bEnable = true;
						break;
					}
				}
				else if (info.nType == InfoType_CheckBtn)
				{
					CButton *btn = (CButton*)info.wndControl;
					int nCheck = btn->GetCheck();
					int vNum = _ttoi(info.valueName);
					vNum = vNum == 0 ? 0 : 1;
					if (nCheck != vNum)
					{

						bEnable = true;
						break;
					}
				}
				


			}


		}

		if (m_applyControl)
		{
			m_applyControl->EnableWindow(bEnable);
		}
	}

}

bool CMFCTools::StartListenControlFor(CWnd *wnd)
{
	if (m_applyControl)
	{
		return true;
	}

	if (wnd == NULL)
	{
		return false;
	}
	m_applyControl = wnd;
	m_timerID = SetTimer(NULL, m_timerID, 500, TimerFunc);
}

void CMFCTools::StopListenControl()
{
	KillTimer(NULL,m_timerID);
	m_applyControl = NULL;
}

void CMFCTools::ReadIniSettting(vector<OC_INI_INFO> &listInfo, CString filePath)
{
	m_iniListInfo.clear();
	if (filePath.IsEmpty())
	{
		CString dir, name;
		GetExeDirAndFileName(dir, name);
		filePath = dir+"\\" + name + ".ini";
	}

	m_iniFilePath = filePath;

	CFileFind finder;   //查找是否存在ini文件，若不存在，则生成一个新的默认设置的ini文件
	BOOL ifFind = finder.FindFile(filePath);

	if (!ifFind)
	{
		for (unsigned int i = 0; i < listInfo.size();++i)
		{
			OC_INI_INFO info = listInfo[i];

			::WritePrivateProfileString(info.headerName, info.keyName, info.defaultName, filePath);
		}
	}

	for (unsigned int i = 0; i < listInfo.size(); ++i)
	{
		OC_INI_INFO &info = listInfo[i];
		info.valueName = "";
		::GetPrivateProfileString(info.headerName, info.keyName, info.defaultName, info.valueName.GetBuffer(MAX_PATH), MAX_PATH, filePath);

		if (info.wndControl)
		{
			if (info.nType == InfoType_Text)
			{
				info.wndControl->SetWindowText(info.valueName);
			}
			else if (info.nType == InfoType_CheckBtn)
			{
				int nNum = _ttoi(info.valueName);
				CButton *checBtn = (CButton*)info.wndControl;
				checBtn->SetCheck(nNum == 0 ? 0 : 1);
			}
			
		}

		m_iniListInfo.push_back(info);
	}

}

void CMFCTools::UpdataIniSetting(CString headerName, CString keyName, CString valueStr)
{
	for (unsigned int i = 0; i < m_iniListInfo.size(); ++i)
	{
		OC_INI_INFO &info = m_iniListInfo[i];
		if (info.headerName == headerName && info.keyName == keyName)
		{
			info.valueName = valueStr;
		}
	}
}

void CMFCTools::SaveIniSetting()
{
	for (unsigned int i = 0; i < m_iniListInfo.size(); ++i)
	{
		OC_INI_INFO &info = m_iniListInfo[i];

		if (info.wndControl)
		{
			if (info.nType == InfoType_Text)
			{
				info.wndControl->GetWindowText(info.valueName);
			}
			else if (info.nType == InfoType_CheckBtn)
			{
				CButton *btn = (CButton*)info.wndControl;
				int nCheck =  btn->GetCheck();
				info.valueName = nCheck == 0 ? "0" : "1";
			}
			
		}

		::WritePrivateProfileString(info.headerName, info.keyName, info.valueName, m_iniFilePath);
	}
}

LRESULT CALLBACK CMFCTools::CloseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_CLOSE)
	{
		int nRet = AfxMessageBox(m_messageStr, 1);
		if (nRet != 1)
		{
			return 0;
		}
		Shell_NotifyIcon(NIM_DELETE, &m_NotifyIcon); //在托盘区删除图标
	}

	return CallWindowProc((WNDPROC)m_lOldProc_Close, hwnd, message, wParam, lParam);
}