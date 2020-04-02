#pragma once
#include <vector>
using namespace std;

#define WM_NC_OC (WM_USER+1101)

class CUIHandler
{
public:
	CUIHandler();
	~CUIHandler();
	
	//获取程序所在路径 和 程序名
	bool GetExeDirAndFileName(CString &dir,CString &fileName);

	//获取版本号
	 CString GetVersionInfo(CString szExeFile);
	
	 //读取配置文件
	 void ReadIniSettting(vector<OC_INI_INFO> &listInfo, CString filePath);

	//保存配置文件
	 void SaveIniSetting(vector<OC_INI_INFO> &listInfo, CString filePath);

	//增加状态栏
	void AddStatueBar(CWnd* pParentWnd,int nParts = 0,  int* pWidths = NULL);
	
	//删除状态栏
	void DeleteStatueBar();

	//状态栏信息
	void SetStatueBarText(CString title,int nParts = 0);

	//最小化到托盘
	void TrayToBottom(HWND hwn, HICON icon, CString tipMsg,UINT nIdMenuRsource = 0);

	//取消最小化托盘
	void CancelTrayToBottom(HWND hwn);

	//关闭弹出提示
	int CloseTip(HWND hwn, CString tipStr);
	
	//取消关闭弹出提示
	void CancelCloseTip(HWND hwn);

private:
	CStatusBarCtrl *m_statusBar;

	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); 
	static LRESULT CALLBACK CloseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

