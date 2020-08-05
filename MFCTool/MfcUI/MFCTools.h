#pragma once
#include <vector>
using namespace std;

typedef void(*fUiControlChangedCallBack)(bool isChanged); //是否有控件发生了变化

class CMFCTools
{
public:
	static CMFCTools *GetInstance();
	~CMFCTools();
public:
	//配置文件INI
	typedef struct OC_INI_INFO_TAG
	{
		CString headerName;
		CString keyName;
		CString valueName;
		CString defaultName;
		CWnd* wndControl;//ui控件 非空时 会进行相应的读取和存储
		
		OC_INI_INFO_TAG()
		{
			wndControl = NULL;
		}

	}OC_INI_INFO;

	
	//程序唯一性 不支持多开 InitInstance()中使用
	static BOOL IsMutexCreated(CString strMutex);

	//标题添加版本号
	bool SetTitleWithVersion(CWnd* hd = NULL);

	//获取程序所在路径 和 程序名
	bool GetExeDirAndFileName(CString &dir, CString &fileName);

	//状态栏信息 需要先添加状态栏
	int SetStatueBarText(CString title, int nParts = 0);

	//增加状态栏
	int AddStatueBar(CWnd* pParentWnd = NULL, int nParts = 0, int* pWidths = NULL);

	//删除状态栏
	int DeleteStatueBar();

	//最小化到托盘
	void TrayToBottom(HWND hwn, HICON icon, CString tipMsg, bool isStartMin = false,UINT nIdMenuRsource = 0);

	//取消最小化托盘
	void CancelTrayToBottom(HWND hwn);

	//关闭弹出提示
	int CloseTip(HWND hwn, CString tipStr);

	//取消关闭弹出提示
	void CancelCloseTip(HWND hwn);

	
	//当ini文件中对应的控件发生变化时，将wnd控件置Enable 或 disable
	bool StartListenControlFor(CWnd *wnd);
	void StopListenControl();

	//读取配置文件 
	void ReadIniSettting(vector<OC_INI_INFO> &listInfo,CString filePath = NULL);

	//更新配置文件中项
	void UpdataIniSetting(CString headerName, CString keyName, CString valueStr);

	//保存配置文件
	void SaveIniSetting();

protected:
	CStatusBarCtrl *m_statusBar;
	static long m_lOldProc;
	static NOTIFYICONDATA m_NotifyIcon;
	static long m_lOldProc_Close;
	static CString m_messageStr;
	static vector<OC_INI_INFO> m_iniListInfo;
	CString m_iniFilePath;
	static CWnd *m_applyControl;
	static UINT m_timerID;
private:
	CMFCTools();
	CString GetVersionInfo(CString szExeFile);
	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK CloseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static VOID CALLBACK TimerFunc(HWND, UINT, UINT_PTR, DWORD);
};

