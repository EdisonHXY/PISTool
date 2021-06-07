// ConfigDlg_Parent.cpp : 实现文件
//

#include "stdafx.h"
//#include "..//TosSvg.h"
#include "ConfigDlg_Parent.h"



// CConfigDlg_Parent 对话框

IMPLEMENT_DYNAMIC(CConfigDlg_Parent, CDialogEx)

CConfigDlg_Parent::CConfigDlg_Parent(UINT nIDD, CWnd* pParent /*=NULL*/)
	: CDialogEx(nIDD, pParent)
{

}

CConfigDlg_Parent::~CConfigDlg_Parent()
{
}

void CConfigDlg_Parent::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg_Parent, CDialogEx)
END_MESSAGE_MAP()


// CConfigDlg_Parent 消息处理程序
