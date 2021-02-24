#pragma once
#include "��־/LogHandle.h"
#include "MfcUI/MFCTools.h"

class CCenterManager
{
public:
	CCenterManager();
	~CCenterManager();


	//��ʼ��UI
	void InitMFCUI(HICON icon, bool bStartMin = true);

	//��ʼ �������ini�л�ȡ
	bool Start();
	
	//ֹͣ
	bool Stop();

protected:
	 CLogHandle m_log;

protected:
	void RecoderLog(CString strLog, int nLevel = 0);

};

