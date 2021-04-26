#include "stdafx.h"
#include "CenterManager.h"


CCenterManager::CCenterManager()
{
	
}


CCenterManager::~CCenterManager()
{
	Stop();
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


}

bool CCenterManager::Start(const CenterParam &cenParam)
{
	m_params = cenParam;

	Stop();
	
	//保存参数
	CMFCTools::GetInstance()->SaveIniSetting();


	bool bRet = true;
	
	return bRet;
}

bool CCenterManager::Stop()
{

	return true;
}
