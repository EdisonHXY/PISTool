#include "stdafx.h"
#include "CenterManager.h"


CCenterManager::CCenterManager()
{
	
}


CCenterManager::~CCenterManager()
{
}

void CCenterManager::InitMFCUI(HICON icon, bool bStartMin)
{
	//版本号
	CWnd* hd  = AfxGetMainWnd();
	CMFCTools::GetInstance()->SetTitleWithVersion(hd);
	
	CString strDir, strFileName;
	CMFCTools::GetInstance()->GetExeDirAndFileName(strDir, strFileName);
	
	//退出提示
	CString strTip;
	strTip.Format("是否退出%s?", strFileName);
	CMFCTools::GetInstance()->CloseTip(hd->m_hWnd, strTip);

	//最小化
	CMFCTools::GetInstance()->TrayToBottom(hd->m_hWnd, icon, strFileName, bStartMin);

	//日志
	m_log.SetSavePath(strDir + "//log//", "");

	RecoderLog("初始化UI");

}

bool CCenterManager::Start()
{
	Stop();
	
	//保存参数
	CMFCTools::GetInstance()->SaveIniSetting();

	//记录参数
	int nCount = CMFCTools::GetInstance()->m_iniListInfo.size();
	CString strLog;
	CString strTmp;
	for (int i = 0 ; i < nCount;++i)
	{
		CMFCTools::OC_INI_INFO info = CMFCTools::GetInstance()->m_iniListInfo[i];
		strTmp.Format("运行参数：(%s)=(%s) ", info.keyName, info.valueName);
		strLog += strTmp;
	}

	RecoderLog(strLog);

	bool bRet = true;
	
	RecoderLog("开始运行");
	return bRet;
}

bool CCenterManager::Stop()
{
	RecoderLog("停止运行");
	return true;
}

void CCenterManager::RecoderLog(CString strLog, int nLevel)
{
	m_log.WritLog(strLog, (CLogHandle::WRITETPYE)nLevel);
}
