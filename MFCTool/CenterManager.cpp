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

	//��־
	m_log.SetSavePath(strDir + "//log//", "");

	RecoderLog("��ʼ��UI");

}

bool CCenterManager::Start()
{
	Stop();
	
	//�������
	CMFCTools::GetInstance()->SaveIniSetting();

	//��¼����
	int nCount = CMFCTools::GetInstance()->m_iniListInfo.size();
	CString strLog;
	CString strTmp;
	for (int i = 0 ; i < nCount;++i)
	{
		CMFCTools::OC_INI_INFO info = CMFCTools::GetInstance()->m_iniListInfo[i];
		strTmp.Format("���в�����(%s)=(%s) ", info.keyName, info.valueName);
		strLog += strTmp;
	}

	RecoderLog(strLog);

	bool bRet = true;
	
	RecoderLog("��ʼ����");
	return bRet;
}

bool CCenterManager::Stop()
{
	RecoderLog("ֹͣ����");
	return true;
}

void CCenterManager::RecoderLog(CString strLog, int nLevel)
{
	m_log.WritLog(strLog, (CLogHandle::WRITETPYE)nLevel);
}
