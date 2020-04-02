#pragma once
#include <vector>
using namespace std;

#define WM_NC_OC (WM_USER+1101)

class CUIHandler
{
public:
	CUIHandler();
	~CUIHandler();
	
	//��ȡ��������·�� �� ������
	bool GetExeDirAndFileName(CString &dir,CString &fileName);

	//��ȡ�汾��
	 CString GetVersionInfo(CString szExeFile);
	
	 //��ȡ�����ļ�
	 void ReadIniSettting(vector<OC_INI_INFO> &listInfo, CString filePath);

	//���������ļ�
	 void SaveIniSetting(vector<OC_INI_INFO> &listInfo, CString filePath);

	//����״̬��
	void AddStatueBar(CWnd* pParentWnd,int nParts = 0,  int* pWidths = NULL);
	
	//ɾ��״̬��
	void DeleteStatueBar();

	//״̬����Ϣ
	void SetStatueBarText(CString title,int nParts = 0);

	//��С��������
	void TrayToBottom(HWND hwn, HICON icon, CString tipMsg,UINT nIdMenuRsource = 0);

	//ȡ����С������
	void CancelTrayToBottom(HWND hwn);

	//�رյ�����ʾ
	int CloseTip(HWND hwn, CString tipStr);
	
	//ȡ���رյ�����ʾ
	void CancelCloseTip(HWND hwn);

private:
	CStatusBarCtrl *m_statusBar;

	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam); 
	static LRESULT CALLBACK CloseWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

