
// MFCToolDlg.h : ͷ�ļ�
//

#pragma once
#include "MfcUI/MFCTools.h"
#include "����̨/ConsoleHandle.h"
#include "afxwin.h"
#include "��־/LogHandle.h"
#include "Com/ComHandle.h"
#include "Tcp/TcpServer.h"
// CMFCToolDlg �Ի���
class CMFCToolDlg : public CDialogEx
{
// ����
public:
	CMFCToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	void testMfcUI();


// ʵ��
protected:
	HICON m_hIcon;
	CConsoleHandle m_consoleHandle;
	CLogHandle m_log;
	CComHandle m_com;
	CTcpServer m_tcpServier;
	static VOID CALLBACK ReceiveDataFromComFunc(char* buf, UINT nLeng, void *lParam);
	static void ReceiveMsgWithSocketCallBackFunc(CString ipStr, char *szBuffer, UINT nLength, void *lparam);
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CComboBox m_combox_consoleType;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonOpencom();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnDestroy();
};
