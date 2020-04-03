#include "stdafx.h"
#include "../����ӿ�/Log_Interface.h"

#include "LogHandle.h"
static CLogHandle_Group g_logGroup;
 
static int JudgeIndex(int nIndex)
{
	
	return nIndex >= g_logGroup.m_list.size() ? OC_ERROR_CODE_PARAM : OC_ERROR_CODE_NONE;
	
}
int OC_InitLog(int nIndex)
{
	if (nIndex >= g_logGroup.m_list.size())
	{
		CLogHandle * ld = new CLogHandle;
		if (ld == NULL)
		{
			return OC_ERROR_CODE_FAIL;
		}

		g_logGroup.m_list.push_back(ld);

	}

	return OC_ERROR_CODE_NONE;
}

//��ʼ��
int OC_SetLogType(int nIndex, bool isSingleFile)
{
	int nRet = JudgeIndex(nIndex);
	
	if (nRet == OC_ERROR_CODE_NONE)
	{
		g_logGroup.m_list[nIndex]->InitLogType(isSingleFile);
	}
	return nRet;

}

//���ñ����λ�ú�����
int OC_SetSavePath(int nIndex, CString logDir, CString logName)
{
	int nRet = JudgeIndex(nIndex);

	if (nRet == OC_ERROR_CODE_NONE)
	{
		g_logGroup.m_list[nIndex]->SetSavePath(logDir, logName);
	}
	return nRet;
}

//�����ļ���С���ڵ�һ�ļ�
int OC_SetFileSizeForSingle(int nIndex, int iSizeKB)
{
	int nRet = JudgeIndex(nIndex);

	if (nRet == OC_ERROR_CODE_NONE)
	{
		g_logGroup.m_list[nIndex]->SetFileSizeForSingle(iSizeKB);
	}
	return nRet;
}

//�����ļ����� һ�����
int OC_SetLogFileCountForMulit(int nIndex, int iDayFileCount, int iLogDayCount)
{
	int nRet = JudgeIndex(nIndex);

	if (nRet == OC_ERROR_CODE_NONE)
	{
		g_logGroup.m_list[nIndex]->SetLogFileCountForMulit(iDayFileCount, iLogDayCount);
	}
	return nRet;
}

//�������
int OC_WritLog(int nIndex, const char *szMsg, WRITETPYE wp )
{
	int nRet = JudgeIndex(nIndex);

	if (nRet == OC_ERROR_CODE_NONE)
	{
		g_logGroup.m_list[nIndex]->WritLog(szMsg, wp);
	}
	return nRet;
}

 void OC_DeletAllLogs()
{
	 g_logGroup.DeleteAll();
	 
}

