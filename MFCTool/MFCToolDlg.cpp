
// MFCToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MFCToolDlg.h"
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


// CMFCToolDlg 对话框



CMFCToolDlg::CMFCToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_combox_consoleType);
}

void CMFCToolDlg::testMfcUI()
{
	//添加版本号
	CMFCTools::GetInstance()->SetTitleWithVersion();

	//添加状态栏
	CMFCTools::GetInstance()->AddStatueBar();
	CMFCTools::GetInstance()->SetStatueBarText("准备就绪");

	//最小化到托盘
	CMFCTools::GetInstance()->TrayToBottom(m_hWnd, m_hIcon, "测试Tools", false);

	//关闭时的提示
	CMFCTools::GetInstance()->CloseTip(m_hWnd, "是否关闭？");

	CMFCTools::GetInstance()->StartListenControlFor(GetDlgItem(IDC_BUTTON_SAVE));

	//读取配置文件
	CMFCTools::OC_INI_INFO info;
	vector<CMFCTools::OC_INI_INFO> list;
	info.headerName = "setting";
	info.keyName = "ip";
	info.defaultName = "10.0.0.1";
	info.wndControl = GetDlgItem(IDC_EDIT2);
	list.push_back(info);

	info.keyName = "port";
	info.defaultName = "123";
	info.wndControl = GetDlgItem(IDC_EDIT3);
	list.push_back(info);

	info.keyName = "tt";
	info.defaultName = "23";
	info.wndControl = GetDlgItem(IDC_EDIT4);
	list.push_back(info);

	info.keyName = "serverPort";
	info.defaultName = "1000";
	info.wndControl = GetDlgItem(IDC_EDIT_ServerPort);
	list.push_back(info);
	info.keyName = "ClientPort";
	info.defaultName = "1000";
	info.wndControl = GetDlgItem(IDC_EDIT_ClientPort);
	list.push_back(info);

	info.keyName = "ServerIP";
	info.defaultName = "0.0.0.0";
	info.wndControl = GetDlgItem(IDC_IPADDRESS1);
	list.push_back(info);
	CMFCTools::GetInstance()->ReadIniSettting(list);
}

BEGIN_MESSAGE_MAP(CMFCToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMFCToolDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCToolDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCToolDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_OpenCom, &CMFCToolDlg::OnBnClickedButtonOpencom)
	ON_BN_CLICKED(IDC_BUTTON_Start, &CMFCToolDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCToolDlg::OnBnClickedButton4)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMFCToolDlg 消息处理程序

VOID CALLBACK CMFCToolDlg::ReceiveDataFromComFunc(char* buf,UINT nLeng, void *lParam)
{
	CMFCToolDlg *dlg = (CMFCToolDlg*)lParam;
	if (nLeng <= 0)
	{
		return;
	}
	CString str,strTmp;
	for (int i = 0 ; i < nLeng;++i)
	{
		strTmp.Format("%02X ", (BYTE)buf[i]);
		str += strTmp;
	}
	
	dlg->GetDlgItem(IDC_Edit_receive)->SetWindowText(str);
}

void CMFCToolDlg::ReceiveMsgWithSocketCallBackFunc(CString ipStr,char *szBuffer, UINT nLength, void *lparam)
{
	CMFCToolDlg *dlg = (CMFCToolDlg*)lparam;

	CString str;
	dlg->GetDlgItem(IDC_EDIT_showServerMsg)->GetWindowText(str);
	str += "\r\n";
	str += ipStr;
	dlg->GetDlgItem(IDC_EDIT_showServerMsg)->SetWindowText(str + szBuffer );

}

BOOL CMFCToolDlg::OnInitDialog()
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

	testMfcUI();

	m_combox_consoleType.AddString("Info");
	m_combox_consoleType.AddString("Warning");
	m_combox_consoleType.AddString("Error");
	m_combox_consoleType.SetCurSel(0);

	//打开日志
	m_log.InitLogType(true);
	//m_log.SetLogFileCountForMulit(12, 2);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCToolDlg::OnPaint()
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
HCURSOR CMFCToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCToolDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CMFCTools::GetInstance()->SaveIniSetting();
}


void CMFCToolDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_consoleHandle.OpenConsole("测试控制台");
}


void CMFCToolDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_consoleHandle.CloseConsole();
}


void CMFCToolDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT nIndex = m_combox_consoleType.GetCurSel();
	
	CConsoleHandle::WRITETPYE tp;
	if (nIndex == 0)
	{
		tp = CConsoleHandle::WRITETPYE_INFO;
	}else if (nIndex == 1)
	{
		tp = CConsoleHandle::WRITETPYE_WARN;
	}
	else
	{
		tp = CConsoleHandle::WRITETPYE_ERROR;
	}

	CString str;
	GetDlgItem(IDC_EDIT1)->GetWindowText(str);

	m_consoleHandle.WriteConsole(str, tp);
	m_log.WritLog(str, (CLogHandle::WRITETPYE)tp);
}


void CMFCToolDlg::OnBnClickedButtonOpencom()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bRet = m_com.OpenComPort(1);
	m_com.SetReceiveDataCallBack(ReceiveDataFromComFunc, this);
	if (!bRet)
	{
		AfxMessageBox("打开端口失败");
		return;
	}
	BYTE msg[24] = { 0 };
	msg[0] = 0x10;
	msg[1] = 0x11;
	msg[2] = 0x12;
	m_com.WriteDataToCom((char*)msg, 3);

}


void CMFCToolDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tcpServier.StopServer();
	CString str;
	GetDlgItem(IDC_EDIT_ServerPort)->GetWindowText(str);

	m_tcpServier.SetReceiveMsgCallBack(ReceiveMsgWithSocketCallBackFunc, this);
	int nRet = m_tcpServier.StartServer(_ttoi(str));
	if (nRet !=0)
	{
		AfxMessageBox("失败");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_Start)->EnableWindow(false);
	}
}


void CMFCToolDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	GetDlgItem(IDC_EDIT_serverMsg)->GetWindowText(str);

	m_tcpServier.SendMsg(str, str.GetLength());

}


void CMFCToolDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}
