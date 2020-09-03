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

	//设置服务器的地址
	void SetCmdServiceAddress(const char *ip,UINT port);

	//查询所有设备状态
	void QueryAllDeviceStatus(vector<DEVICE_STATUS> *listDevice);
	
	//查询单个设备状态
	bool QuerySingleStatus(const char* szDeviceID, DEVICE_STATUS &deStatue);

	//发送紧急消息（运营信息 不是全屏）
	BOOL SendEmMsg(const vector<string> &arryID,const  string &content, bool isFullScreen, __int16 timeLength,bool isReplac = true,bool isCloseAudio = false);
	//撤销紧急消息
	BOOL EndEmMsg(const vector<string> &arryID, bool isFullScreen);
	
	//发布设备状态 开关机 屏幕开启关闭 重启
	BOOL SendDeviceStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd);

	//声音设置 开启 关闭 设置音量
	BOOL SendDeviceVolumeStatue(const vector<string> &arryID, MACRO_TYPE_INDEX devCmd,int leftVolue = 0,int rightVolue = 0);

	//设置日志回调
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

