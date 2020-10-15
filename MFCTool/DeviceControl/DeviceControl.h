#pragma once
#include "CtrlLib.h"
#include "CPing.h"
#include <string>
#include <vector>
using namespace std;
class CDeviceControl
{
public:
	static void SetDllName(CString dllName);
	static CDeviceControl *GetInstance();
	~CDeviceControl();

	typedef void(*fLogCallBack)(const char * str,void *lparam,bool isError);

	//������־�ص�
	void SetLogCallBack(fLogCallBack cb, void *lparam);

	//�����豸�Ķ���
	void SetDeviceMap(vector< pair<string , string > > &listMap);
	//������Ϣ
	int SendMsg(vector<string > listDevice,const  char *pszText, bool isFullScreen = false);
	int EndMsg(vector<string > listDevice);
	//��ѯ�����豸״̬
	int QuerySingleStatus(const char* szDeviceID, CHECK_STATUS_PARAM &deStatue, PLAY_CONTENT &PlayContent, NET_ES_PARAM &esParam);

	BOOL GetCmdResult(const string &cmdID, vector<TASK_LIST> &resultList, int receiveCount = 10000);

	int ExcetCmd(vector<string > listDevice, CTRL_CMD cmdID,int param =0);

public:
	int SendMsg(const char *pIpStr, char *pszText, bool isFullScreen = false);
	int EndMsg(const char *pIpStr);
	int ExcetCmd(const char *pIpStr, CTRL_CMD cmdID, int param = 0);

	int GetDeviceStatus(const char *ipStr, CHECK_STATUS_PARAM &param);
	int GetDevicePlayContent(const char *ipStr, PLAY_CONTENT &param);
	int GetDeviceEmsContent(const char *ipStr, NET_ES_PARAM &param);
	void RecoreLog(const char *str, bool isError = false);
	string GetIpStrFromList(string deviceStr);

private:
	static CString m_dllName;
	CDeviceControl(const char * libPath);
	string m_dllPath;
	CCtrlLib m_cclib;
	CPing m_ping;
	vector< pair<string , string > > m_mapList;

	vector<TASK_LIST> m_resultTask; //��¼ִ�н��
	fLogCallBack m_logCB;
	void *m_logCBParam;
};

