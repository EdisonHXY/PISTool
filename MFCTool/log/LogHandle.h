#pragma once
#include "LogToFile.h"
#include <vector>
using namespace std;

class CLogHandle
{
public:
	CLogHandle();
	~CLogHandle();

	enum WRITETPYE
	{
		WRITETPYE_INFO, //������Ϣ
		WRITETPYE_WARN, //������Ϣ
		WRITETPYE_ERROR //������Ϣ
	};
	//��ʼ��
	void InitLogType(bool isSingleFile);

	//���ñ����λ�ú�����
	void SetSavePath(CString logDir, CString logName);

	//�����ļ���С���ڵ�һ�ļ�
	void SetFileSizeForSingle(int iSizeKB);
	
	//�����ļ�����                һ�챣����ٸ���־   ��־��������
	void SetLogFileCountForMulit(int iDayFileCount, int iLogDayCount);
	
	//�������
	void WritLog(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

	//���16����
	void WriteHexLog(const char *szPreMsg,const BYTE * szMsg, UINT nlen, WRITETPYE wp = WRITETPYE_INFO);

private:
	CLogToFile_OC m_log;
	CRITICAL_SECTION m_criSection;
};




