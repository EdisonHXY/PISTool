
// MFCToolDlg.h : 头文件
//

#pragma once
#include "MfcUI/MFCTools.h"
#include "控制台/ConsoleHandle.h"
#include "afxwin.h"
#include "日志/LogHandle.h"
#include "Com/ComHandle.h"
#include "Tcp/TcpServer.h"
// CMFCToolDlg 对话框
class CMFCToolDlg : public CDialogEx
{
// 构造
public:
	CMFCToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void testMfcUI();


// 实现
protected:
	HICON m_hIcon;
	CConsoleHandle m_consoleHandle;
	CLogHandle m_log;
	CComHandle m_com;
	CTcpServer m_tcpServier;
	static VOID CALLBACK ReceiveDataFromComFunc(char* buf, UINT nLeng, void *lParam);
	static void ReceiveMsgWithSocketCallBackFunc(CString ipStr, char *szBuffer, UINT nLength, void *lparam);
	// 生成的消息映射函数
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
