#pragma once
//��ʼ����־
DllExport int OC_InitLog(int nIndex);

//������־����
DllExport int OC_SetLogType(int nIndex,bool isSingleFile);

//���ñ����λ�ú�����
DllExport int OC_SetSavePath(int nIndex, CString logDir, CString logName);

//�����ļ���С���ڵ�һ�ļ�
DllExport int OC_SetFileSizeForSingle(int nIndex, int iSizeKB);

//�����ļ����� һ�����ɶ��ٸ��ļ������������
DllExport int OC_SetLogFileCountForMulit(int nIndex, int iDayFileCount, int iLogDayCount);

//�������
DllExport int OC_WritLog(int nIndex, const char *szMsg, WRITETPYE wp = WRITETPYE_INFO);

//����������־����
DllExport void OC_DeletAllLogs();
