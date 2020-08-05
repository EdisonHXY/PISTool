#pragma once
#include <vector>
using namespace std;

typedef void(*fUiControlChangedCallBack)(bool isChanged); //�Ƿ��пؼ������˱仯

class CMFCTools
{
public:
	static CMFCTools *GetInstance();
	~CMFCTools();
public:
	//�����ļ�INI
	typedef struct OC_INI_INFO_TAG
	{
		CString headerName;
		CString keyName;
		CString valueName;
		CString defaultName;
		CWnd* wndControl;//ui�ؼ� �ǿ�ʱ �������Ӧ�Ķ�ȡ�ʹ洢
		
		OC_INI_INFO_TAG()
		{
			wndControl = NULL;
		}

	}OC_INI_INFO;

	
	//����Ψһ�� ��֧�ֶ࿪ InitInstance()��ʹ��
	static BOOL IsMutexCreated(CString strMutex);

	//������Ӱ汾��
	bool SetTitleWithVersion(CWnd* hd = NULL);

	//��ȡ��������·�� �� ������
	bool GetExeDirAndFileName(CString &dir, CString &fileName);

	//״̬����Ϣ ��Ҫ�����״̬��
	int SetStatueBarText(CString title, int nParts = 0);

	//����״̬��
	int AddStatueBar(CWnd* pParentWnd = NULL, int nParts = 0, int* pWidths = NULL);

	//ɾ��״̬��
	int DeleteStatueBar();

	//��С��������
	void TrayToBottom(HWND hwn, HICON icon, CString tipMsg, bool isStartMin = false,UINT nIdMenuRsource = 0);

	//ȡ����С������
	void CancelTrayToBottom(HWND hwn);

	//�رյ�����ʾ
	int CloseTip(HWND hwn, CString tipStr);

	//ȡ���رյ�����ʾ
	void CancelCloseTip(HWND hwn);

	
	//��ini�ļ��ж�Ӧ�Ŀؼ������仯ʱ����wnd�ؼ���Enable �� disable
	bool StartListenControlFor(CWnd *wnd);
	void StopListenControl();

	//��ȡ�����ļ� 
	void ReadIniSettting(vector<OC_INI_INFO> &listInfo,CString filePath = NULL);

	//���������ļ�����
	void UpdataIniSetting(CString headerName, CString keyName, CString valueStr);

	//���������ļ�
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

