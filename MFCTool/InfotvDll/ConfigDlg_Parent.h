#pragma once
#include "afxdialogex.h"

// CConfigDlg_Parent 对话框

class CConfigDlg_Parent : public CDialogEx
{
	DECLARE_DYNAMIC(CConfigDlg_Parent)

public:
	CConfigDlg_Parent(UINT nIDD, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CConfigDlg_Parent();


public:
	//传进来的参数 宽高是infotv设置的分辨率大小
	virtual void SetParam(SYSTEM_PARAM_Parent *sysParam,UINT nWidth,UINT nHeight) = 0; 
	//获取内部的参数
	virtual void GetParam(SYSTEM_PARAM_Parent *sysParam) = 0;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
