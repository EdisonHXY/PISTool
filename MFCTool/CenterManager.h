#pragma once
#include "日志/LogHandle.h"
#include "MfcUI/MFCTools.h"


typedef struct __RUNPARAM
{

}RUNPARAM;

class CCenterManager
{
public:
	CCenterManager();
	~CCenterManager();

	//初始化UI
	void InitMFCUI(HICON icon,bool bStartMin = true);

	//开始
	bool Start(RUNPARAM &pParam);
	
	//停止
	bool Stop();

protected:
	 CLogHandle m_log;
	
protected:
	void RecoderLog(CString strLog, int nLevel = 0);


};

