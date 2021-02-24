#pragma once
#include "��־/LogHandle.h"
#include "MfcUI/MFCTools.h"


typedef struct __RUNPARAM
{

}RUNPARAM;

class CCenterManager
{
public:
	CCenterManager();
	~CCenterManager();

	//��ʼ��UI
	void InitMFCUI(HICON icon,bool bStartMin = true);

	//��ʼ
	bool Start(RUNPARAM &pParam);
	
	//ֹͣ
	bool Stop();

protected:
	 CLogHandle m_log;
	
protected:
	void RecoderLog(CString strLog, int nLevel = 0);


};

