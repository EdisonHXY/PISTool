
// TestToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestTools.h"
#include "TestToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestToolsDlg 对话框


#include "../PISTools/对外接口/All_Interface.h"

CTestToolsDlg::CTestToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTTOOLS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestToolsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTestToolsDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTestToolsDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CTestToolsDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CTestToolsDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CTestToolsDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CTestToolsDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CTestToolsDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CTestToolsDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CTestToolsDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CTestToolsDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_CHECK_Console, &CTestToolsDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON13, &CTestToolsDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CTestToolsDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON11, &CTestToolsDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CTestToolsDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON15, &CTestToolsDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &CTestToolsDlg::OnBnClickedButton16)
END_MESSAGE_MAP()


// CTestToolsDlg 消息处理程序

BOOL CTestToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	CEdit *ed = (CEdit*)GetDlgItem(IDC_EDIT_SQL);
	if (!ed)
	{
		return 0;
	}
	COleDateTime tm = COleDateTime::GetCurrentTime();

	CString timStr = tm.Format("%Y-%m-%d %H:%M:%S");
	CString strCmd;
	strCmd.Format("Insert Into %s "
		"(List_ID, List_Title, List_Status, Update_Time, "
		"Delete_Date,Editor_ID, Edit_Time, "
		"Scope_Edit_Time, \"Audit\", List_MD5, Import_Line) "
		"values ('%s', '%s', %d, '%s', '%s', '%s', '%s', "
		" '%s', %d, '%s', '%s')", "Default_List",
		"100", "测试100", 1, timStr,
		timStr,"ocean", timStr,
		timStr,0, "szzz", "L999");
	ed->SetWindowText(strCmd);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestToolsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString dirStr,fielNameStr;
	OC_GetExeDirAndFileName(dirStr,fielNameStr);
	CString tipMsg;
	tipMsg.Format(_T("名称：%s,路径：%s"), fielNameStr, dirStr);
	AfxMessageBox(tipMsg);
}


void CTestToolsDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString versionStr;
	OC_GetCurrentVersionInfo(versionStr);
	AfxMessageBox(versionStr);
}


void CTestToolsDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	int partS[2] = { 400,600 };

	OC_AddStatueBar(this,2,partS);

	OC_SetStatueBarText(_T("准备就绪"), 0);
	OC_SetStatueBarText(_T("第二部分"), 1);

}


void CTestToolsDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	OnBnClickedButton3();//需要在状态栏显示数据

	vector<OC_INI_INFO> list;
	OC_INI_INFO info;
	
	info.headerName = "set";

	for (int i = 0 ; i <3;++i)
	{
		CString keyStr;
		keyStr.Format(_T("name%d"),i);
		info.keyName = keyStr;
		CString defaultStr;
		defaultStr.Format(_T("ocean%d"), i);
		info.defaultName = defaultStr;
		list.push_back(info);
	}

	OC_ReadCurrentCfgFromIni(list, "setting.ini");

	CString tipStr;
	for (int i = 0; i <3; ++i)
	{
		OC_INI_INFO info1 = list[i];
		CString name;
		name.Format(_T("%s = %s "), info1.keyName,info1.valueName);
		tipStr += name;
		if (i != 3)
		{
			tipStr += " ";
		}
	}

	OC_SetStatueBarText(tipStr);


}


void CTestToolsDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<OC_INI_INFO> list;
	OC_INI_INFO info;

	info.headerName = "set";

	for (int i = 0; i < 3; ++i)
	{
		CString keyStr;
		keyStr.Format(_T("name%d"), i);
		info.keyName = keyStr;
		CString valueSt;
		valueSt.Format(_T("value%d"), i);
		info.valueName = valueSt;
		list.push_back(info);
	}

	OC_SaveCurrentCfgToIni(list, "setting.ini");

}


void CTestToolsDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_TrayToBottom(m_hWnd, m_hIcon, _T("托盘程序"));
}


void CTestToolsDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_CancelTrayToBottom(m_hWnd);
}


void CTestToolsDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_DeleteStatueBar();
}


void CTestToolsDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_CloseTip(m_hWnd, _T("是否关闭?"));
}


void CTestToolsDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_CancelCloseTip(m_hWnd);
}


void CTestToolsDlg::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	CButton *btn = (CButton *)GetDlgItem(IDC_CHECK_Console);
	if (btn)
	{
		 btn->GetCheck() == 1 ? OC_OpenConsole(_T("测试控制台")) : OC_CloseConsole();
	}

	OC_WriteConsole("测试info");
	OC_WriteConsole("测试warnning",WRITETPYE_WARN);
	OC_WriteConsole("测试error",WRITETPYE_ERROR);


}


void CTestToolsDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码

	OC_SetConsoleColor(WRITETPYE_ERROR, FOREGROUND_GREEN | BACKGROUND_RED);
	OC_SetConsoleColor(WRITETPYE_INFO, FOREGROUND_GREEN | BACKGROUND_BLUE);
	OC_SetConsoleColor(WRITETPYE_WARN, FOREGROUND_RED | BACKGROUND_BLUE);

}


void CTestToolsDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_SetConsoleOutPattern(_T("只显示时分秒[%H:%M:%S]"));
}


void CTestToolsDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码

	CEdit *ed = (CEdit*)GetDlgItem(IDC_EDIT_SQL);
	if (!ed)
	{
		return;
	}
	OC_DB_PARAMS p;
	p.addressStr = "192.168.1.109";
	p.passwordStr = "ghyt2001";
	p.usrName = "sa";
	p.name = "pis";
	//p.timeOut = 5;
	OC_SetDataBaseParams(0, p);

	CString sqlStr;
	ed->GetWindowText(sqlStr);
	CString errStr;
	bool bRet = OC_ExectSql(0, sqlStr, errStr);

	
	OC_OpenConsole(_T("测试控制台"));
	

	CString tipMsg;
	tipMsg.Format(_T("执行结果：%s,%s"),bRet ? "Success" : "Fail",bRet ? "":errStr);
	OC_WriteConsole(tipMsg, bRet ? WRITETPYE_INFO: WRITETPYE_ERROR);
}


void CTestToolsDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_OpenConsole(_T("测试控制台"));

	CStringArray keyArr;

	keyArr.Add("List_ID");
	keyArr.Add("List_Title");
	keyArr.Add("Editor_ID");

	map<CString, CStringArray *> valueList;
	


	CString errStr="";
	CString sqlStr = "Select * from Default_List";
	
	CEdit *ed = (CEdit*)GetDlgItem(IDC_EDIT_SQL);
	if (ed)
	{
		ed->SetWindowText(sqlStr);
	}



	bool bRet = OC_ExectSelectSql(0, sqlStr,keyArr,valueList, errStr);

	


	CString tipMsg;
	tipMsg.Format(_T("执行结果：%s,%s"), bRet ? "Success" : "Fail", bRet ? "" : errStr);
	OC_WriteConsole(tipMsg, bRet ? WRITETPYE_INFO : WRITETPYE_ERROR);

	if (bRet)
	{
		tipMsg = _T("key:");
		std::map<CString, CStringArray* >::iterator it ;
		for (unsigned int i = 0;i < keyArr.GetSize();++i)
		{
			CString key = keyArr[i];
			
			it = valueList.find(key);

			if (it != valueList.end())
			{
				
				tipMsg = "key:" + key;
				tipMsg += " value:";
				
				 CStringArray *strList = it->second;

				for (unsigned int j = 0 ; j < strList->GetSize();++j)
				{
					tipMsg += (*strList)[j] + " ";

				}

			}
			else {
				tipMsg.Format(_T("key=%s,value=无"), key);
			}
			
			OC_WriteConsole(tipMsg);
		}

	

	}

	OC_ReleaseBuffer(0, valueList);
}


void CTestToolsDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_OpenConsole(_T("测试控制台"));

	CStringArray keyArr;

	keyArr.Add("List_ID");
	keyArr.Add("Device_Pos");
	keyArr.Add("Station_Code");
	keyArr.Add("Line_Code");

	map<CString, CStringArray *> valueList;
	CString errStr = "";
	CString sqlStr ;
	sqlStr = "Select * from Default_List_Scope where List_ID = 00001";
	CEdit *ed = (CEdit*)GetDlgItem(IDC_EDIT_SQL);
	if (ed)
	{
		ed->SetWindowText(sqlStr);
	}

	bool bRet = OC_ExectSelectSql(0, sqlStr, keyArr, valueList, errStr);


	CString tipMsg;
	tipMsg.Format(_T("执行结果：%s,%s"), bRet ? "Success" : "Fail", bRet ? "" : errStr);
	OC_WriteConsole(tipMsg, bRet ? WRITETPYE_INFO : WRITETPYE_ERROR);

	if (bRet)
	{
		tipMsg = _T("key:");
		std::map<CString, CStringArray*>::iterator it;
		for (unsigned int i = 0; i < keyArr.GetSize(); ++i)
		{
			CString key = keyArr[i];

			it = valueList.find(key);

			if (it != valueList.end())
			{

				tipMsg = "key:" + key;
				tipMsg += " value:";

				CStringArray *strList = it->second;

				for (unsigned int j = 0; j < strList->GetSize(); ++j)
				{
					tipMsg += (*strList)[j] + " ";

				}

			}
			else {
				tipMsg.Format(_T("key=%s,value=无"), key);
			}

			OC_WriteConsole(tipMsg);
		}



	}


	OC_ReleaseBuffer(0, valueList);

}


void CTestToolsDlg::OnBnClickedButton16()
{
	// TODO: 在此添加控件通知处理程序代码
	OC_InitLog(0);
	for (unsigned int i = 0 ; i < 100;++i)
	{
		CString tmpStr;
		tmpStr.Format(_T("测试日志=%d"), i);

		OC_WritLog(0, tmpStr);
	}


}
