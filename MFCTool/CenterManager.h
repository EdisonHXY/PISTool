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

	//初始化UI
	void InitMFCUI(HICON icon, bool bStartMin = true);

	//开始 
	bool Start(const CenterParam &cenParam);
	
	//停止
	bool Stop();

private:
	CenterParam m_params;

};

