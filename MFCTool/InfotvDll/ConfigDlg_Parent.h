#pragma once
#include "afxdialogex.h"

// CConfigDlg_Parent �Ի���

class CConfigDlg_Parent : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg_Parent)

public:
	CConfigDlg_Parent(UINT nIDD, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigDlg_Parent();


public:
	//�������Ĳ��� �����infotv���õķֱ��ʴ�С
	virtual void SetParam(SYSTEM_PARAM_Parent *sysParam,UINT nWidth,UINT nHeight) = 0; 
	//��ȡ�ڲ��Ĳ���
	virtual void GetParam(SYSTEM_PARAM_Parent *sysParam) = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
