#pragma once
/*!
 * \class CCmdServiceTools
 *
 * \brief 与CmdService交互
 *
 * \author HXY10
 * \date 九月 2020
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
		char	szEmergencyInfo[1022];//消息内容
		bool bFullScreen; //是否是全屏
	
		bool bIsReplay;	//跟随还是替换
		NET_ES_TYPE nType; //板式还是文字

		NET_ES_SOUND_TYPE nAudio; //对于声音的处理
		//左右声道的值 0-255
		unsigned int nLeftValue; 
		unsigned int nRightValue;

		unsigned int iTimeLength; //持续时间 0标识一直执行
		unsigned int exectLevel; //执行权限 0 -255 

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

	//设置服务器的地址
	void SetCmdServiceAddress(const char *ip, UINT port,const char *editId="110");

	//设置日志回调
	void SetLogCallBack(fLogCallBack cb, void *lparam);

	//查询单个设备状态
	bool QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue);

	//发布紧急消息
	bool PublishEmgency(const vector<string> &arryDeviceID,const EmgencyInfo &info, string &taskID = string());

	//停止紧急消息
	bool StopEmgency(const vector<string> &arryDeviceID, string &taskID = string(), unsigned int exectLevel = 128);

	//执行其他命令
	bool ExectCommand(const vector<string> &arryDeviceID, MACRO_TYPE_INDEX cmdID, unsigned int leftVolue = 0, unsigned int rightVolue = 0, string &taskID = string(), unsigned int exectLevel = 50);

	//上报设备的状态
	bool UpLoadDeviceStatus(const char *szDeviceId, const DEVICE_STATUS &deStatue);

	//查询操作任务执行结果 nCount 为查询的设备数量 vector 保存的是结果
	bool QueryExecuteResult(const string &taskID, vector<TASK_LIST> &resultList,unsigned int nCount);

	//查询自己发布的任务
	bool QueryTask(const char* szDeviceID, vector<TASK_SET> &resultList);

	//上传执行结果
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

