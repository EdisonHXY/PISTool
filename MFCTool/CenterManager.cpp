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

bool CCenterManager::Start(RUNPARAM &pParam)
{
	bool bRet = true;

	return bRet;
}

bool CCenterManager::Stop()
{
	return true;
}

void CCenterManager::RecoderLog(CString strLog, int nLevel)
{
	m_log.WritLog(strLog, (CLogHandle::WRITETPYE)nLevel);
}
