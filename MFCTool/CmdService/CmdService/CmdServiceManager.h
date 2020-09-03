#pragma once
#include "CmdService.h"
#include "..\Socket\SyncSocket.h"
#include "DeviceInfo.h"
#include <string>
#include <vector>
using namespace std;

class CCmdServiceManager
{
public:
	~CCmdServiceManager();
	static CCmdServiceManager *GetInstance();


	typedef void(*fLogCallBack)(string strLog, int nLevel, void *lParam);

	//���÷������ĵ�ַ
	void SetCmdServiceAddress(const char *ip,UINT port);

	//��ѯ�����豸״̬
	void QueryAllDeviceStatus(vector<DEVICE_STATUS> *listDevice);
	
	//��ѯ�����豸״̬
	bool QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue);

	//���ͽ�����Ϣ����Ӫ��Ϣ ����ȫ����
	BOOL SendEmMsg(const vector<string> &arryID,const  string &content, bool isFullScreen, __int16 timeLength,bool isReplac = true,bool isCloseAudio = false);
	//����������Ϣ
	BOOL EndEmMsg(const vector<string> &arryID, bool isFullScreen);
	
	//�����豸״̬ ���ػ� ��Ļ�����ر� ����
	BOOL SendDeviceStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd);

	//�������� ���� �ر� ��������
	BOOL SendDeviceVolumeStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd,int leftVolue = 0,int rightVolue = 0);

	//������־�ص�
	void SetLogCallBack(fLogCallBack cb, void *lparam);
	

private:
	CCmdServiceManager();
	fLogCallBack m_logCB;
	void *m_logCBParam;
	char m_ipAddress[256];
	UINT m_port;

private:
	void RecordLogToFile(const char *,bool bIsError = false);

	string MD5Encode32(const char* szIn, int iSize);
	string GetUniqueID(void);
	
};

