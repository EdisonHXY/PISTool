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
	//�汾��
	CWnd* hd  = AfxGetMainWnd();
	CMFCTools::GetInstance()->SetTitleWithVersion(hd);
	
	CString strDir, strFileName;
	CMFCTools::GetInstance()->GetExeDirAndFileName(strDir, strFileName);
	
	//�˳���ʾ
	CString strTip;
	strTip.Format("�Ƿ��˳�%s?", strFileName);
	CMFCTools::GetInstance()->CloseTip(hd->m_hWnd, strTip);

	//��С��
	CMFCTools::GetInstance()->TrayToBottom(hd->m_hWnd, icon, strFileName, bStartMin);


}

bool CCenterManager::Start(const CenterParam &cenParam)
{
	m_params = cenParam;

	Stop();
	
	//�������
	CMFCTools::GetInstance()->SaveIniSetting();


	bool bRet = true;
	
	return bRet;
}

bool CCenterManager::Stop()
{

	return true;
}
