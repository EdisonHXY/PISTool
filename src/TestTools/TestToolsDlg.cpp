
// TestToolsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestTools.h"
#include "TestToolsDlg.h"
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


// CTestToolsDlg �Ի���


#include "../PISTools/����ӿ�/All_Interface.h"

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
END_MESSAGE_MAP()


// CTestToolsDlg ��Ϣ�������

BOOL CTestToolsDlg::OnInitDialog()
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
	
	
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestToolsDlg::OnPaint()
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
HCURSOR CTestToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestToolsDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString dirStr,fielNameStr;
	OC_GetExeDirAndFileName(dirStr,fielNameStr);
	CString tipMsg;
	tipMsg.Format(_T("���ƣ�%s,·����%s"), fielNameStr, dirStr);
	AfxMessageBox(tipMsg);
}


void CTestToolsDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString versionStr;
	OC_GetCurrentVersionInfo(versionStr);
	AfxMessageBox(versionStr);
}


void CTestToolsDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int partS[2] = { 400,600 };

	OC_AddStatueBar(this,2,partS);

	OC_SetStatueBarText(_T("׼������"), 0);
	OC_SetStatueBarText(_T("�ڶ�����"), 1);

}


void CTestToolsDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnBnClickedButton3();//��Ҫ��״̬����ʾ����

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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OC_TrayToBottom(m_hWnd, m_hIcon, _T("���̳���"));
}


void CTestToolsDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OC_CancelTrayToBottom(m_hWnd);
}


void CTestToolsDlg::OnBnClickedButton8()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OC_DeleteStatueBar();
}


void CTestToolsDlg::OnBnClickedButton9()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OC_CloseTip(m_hWnd, _T("�Ƿ�ر�?"));
}


void CTestToolsDlg::OnBnClickedButton10()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OC_CancelCloseTip(m_hWnd);
}
