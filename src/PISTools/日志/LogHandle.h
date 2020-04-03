#pragma once
#include "LogToFile.h"
class CLogHandle
{
public:
	CLogHandle();
	~CLogHandle();

	//��ʼ��
	void InitLogType(bool isSingleFile);

	//���ñ����λ�ú�����
	void SetSavePath(CString logDir, CString logName);

	//�����ļ���С���ڵ�һ�ļ�
	void SetFileSizeForSingle(int iSizeKB);
	
	//�����ļ����� һ�����
	void SetLogFileCountForMulit(int iDayFileCount, int iLogDayCount);
	
	//�������
	void WritLog(const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

private:
	CLogToFile_OC m_log;

};

class CLogHandle_Group
{
public:
	CLogHandle_Group();
	~CLogHandle_Group();

public:
	vector<CLogHandle *> m_list;
	void DeleteAll();
};



