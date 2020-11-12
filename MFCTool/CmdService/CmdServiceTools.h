#pragma once
/*!
 * \class CCmdServiceTools
 *
 * \brief ��CmdService����
 *
 * \author HXY10
 * \date ���� 2020
 */
#include "CmdServiceHeader.h"
#include <string>
#include <vector>
using namespace std;
class CCmdServiceTools
{
public:
	CCmdServiceTools();
	~CCmdServiceTools();

	typedef struct __EmgencyInfo
	{
		char	szEmergencyInfo[1022];//��Ϣ����
		bool bFullScreen; //�Ƿ���ȫ��
	
		bool bIsReplay;	//���滹���滻
		NET_ES_TYPE nType; //��ʽ��������

		NET_ES_SOUND_TYPE nAudio; //���������Ĵ���
		//����������ֵ 0-255
		unsigned int nLeftValue; 
		unsigned int nRightValue;

		unsigned int iTimeLength; //����ʱ�� 0��ʶһֱִ��
		unsigned int exectLevel; //ִ��Ȩ�� 0 -255 

		__EmgencyInfo()
		{
			bFullScreen = false;
			bIsReplay = true;
			nType = NET_ES_CONTENT;
			nAudio = NET_ES_SOUND_NONE;
			memset(szEmergencyInfo, 0, 1022);
			iTimeLength = 0;
			exectLevel = 100;
		}
	}EmgencyInfo;

	typedef void(*fLogCallBack)(string strLog, int nLevel, void *lParam);

	//���÷������ĵ�ַ
	void SetCmdServiceAddress(const char *ip, UINT port,const char *editId="110");

	//������־�ص�
	void SetLogCallBack(fLogCallBack cb, void *lparam);

	//��ѯ�����豸״̬
	bool QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue);

	//����������Ϣ
	bool PublishEmgency(const vector<string> &arryDeviceID,const EmgencyInfo &info, string &taskID = string());

	//ֹͣ������Ϣ
	bool StopEmgency(const vector<string> &arryDeviceID, string &taskID = string(), unsigned int exectLevel = 128);

	//ִ����������
	bool ExectCommand(const vector<string> &arryDeviceID, MACRO_TYPE_INDEX cmdID, unsigned int leftVolue = 0, unsigned int rightVolue = 0, string &taskID = string(), unsigned int exectLevel = 50);

	//�ϱ��豸��״̬
	bool UpLoadDeviceStatus(const char *szDeviceId, const DEVICE_STATUS &deStatue);

	//��ѯ��������ִ�н�� nCount Ϊ��ѯ���豸���� vector ������ǽ��
	bool QueryExecuteResult(const string &taskID, vector<TASK_LIST> &resultList,unsigned int nCount);

	//��ѯ�Լ�����������
	bool QueryTask(const char* szDeviceID, vector<TASK_SET> &resultList);

	//�ϴ�ִ�н��
	bool UpLoadTASK_RESULT(const char *szDeviceId, const TASK_RESULT &taskRsult);

private:
	fLogCallBack m_logCB;
	void *m_logCBParam;
	char m_ipAddress[256];
	UINT m_port;
	char m_editId[50];
private:
	void RecordLogToFile(const char *, bool bIsError = false);

	string MD5Encode32(const char* szIn, int iSize);
	string GetUniqueID(void);

	string GetCommandDes(MACRO_TYPE_INDEX cmdID);


};

