
// MFCToolDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTool.h"
#include "MFCToolDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMFCToolDlg �Ի���



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
	//��Ӱ汾��
	CMFCTools::GetInstance()->SetTitleWithVersion();

	//���״̬��
	CMFCTools::GetInstance()->AddStatueBar();
	CMFCTools::GetInstance()->SetStatueBarText("׼������");

	//��С��������
	CMFCTools::GetInstance()->TrayToBottom(m_hWnd, m_hIcon, "����Tools", false);

	//�ر�ʱ����ʾ
	CMFCTools::GetInstance()->CloseTip(m_hWnd, "�Ƿ�رգ�");

	CMFCTools::GetInstance()->StartListenControlFor(GetDlgItem(IDC_BUTTON_SAVE));

	//��ȡ�����ļ�
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


// CMFCToolDlg ��Ϣ�������

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

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	testMfcUI();

	m_combox_consoleType.AddString("Info");
	m_combox_consoleType.AddString("Warning");
	m_combox_consoleType.AddString("Error");
	m_combox_consoleType.SetCurSel(0);

	//����־
	m_log.InitLogType(true);
	//m_log.SetLogFileCountForMulit(12, 2);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCToolDlg::OnBnClickedButtonSave()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CMFCTools::GetInstance()->SaveIniSetting();
}


void CMFCToolDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_consoleHandle.OpenConsole("���Կ���̨");
}


void CMFCToolDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_consoleHandle.CloseConsole();
}


void CMFCToolDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	bool bRet = m_com.OpenComPort(1);
	m_com.SetReceiveDataCallBack(ReceiveDataFromComFunc, this);
	if (!bRet)
	{
		AfxMessageBox("�򿪶˿�ʧ��");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_tcpServier.StopServer();
	CString str;
	GetDlgItem(IDC_EDIT_ServerPort)->GetWindowText(str);

	m_tcpServier.SetReceiveMsgCallBack(ReceiveMsgWithSocketCallBackFunc, this);
	int nRet = m_tcpServier.StartServer(_ttoi(str));
	if (nRet !=0)
	{
		AfxMessageBox("ʧ��");
	}
	else
	{
		GetDlgItem(IDC_BUTTON_Start)->EnableWindow(false);
	}
}


void CMFCToolDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	GetDlgItem(IDC_EDIT_serverMsg)->GetWindowText(str);

	m_tcpServier.SendMsg(str, str.GetLength());

}


void CMFCToolDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}
