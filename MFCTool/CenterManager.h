#pragma once
#include "MfcUI/ThreadCheck.h"
#include "MfcUI/MFCTools.h"
#include "LogManager.h"

class CCenterManager
{
public:
	CCenterManager();
	~CCenterManager();

	typedef struct __CenterParam
	{



	}CenterParam;

	//��ʼ��UI
	void InitMFCUI(HICON icon, bool bStartMin = true);

	//��ʼ 
	bool Start(const CenterParam &cenParam);
	
	//ֹͣ
	bool Stop();

private:
	CenterParam m_params;

};

