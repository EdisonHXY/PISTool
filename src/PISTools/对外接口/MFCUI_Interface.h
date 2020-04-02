#pragma once

/************************************************************************/
/*                ���湦��                                              */
/************************************************************************/

//��ȡ�汾�� 
DllExport int OC_GetVersionInfo(CString szExeFile, CString &szVersion); 

//��ȡ��������·�� �� ������
DllExport bool OC_GetExeDirAndFileName(CString &dir, CString &fileName);

//��ȡ�����ļ� 
DllExport int OC_ReadCfgFromIni(vector<OC_INI_INFO> &listInfo, CString filePath);

//���������ļ�
DllExport int OC_SaveCfgToIni(vector<OC_INI_INFO> &listInfo, CString filePath);

//����״̬��
DllExport int OC_AddStatueBar(CWnd* pParentWnd, int nParts = 0, int* pWidths = NULL);

//ɾ��״̬��
DllExport int OC_DeleteStatueBar();

//״̬����Ϣ ��Ҫ�����״̬��
DllExport int OC_SetStatueBarText(CString title, int nParts = 0);

//��С����������
DllExport int OC_TrayToBottom(HWND hwn, HICON icon, CString tipMsg);

//ȡ������
DllExport int OC_CancelTrayToBottom(HWND hwn);


//�رյ�����ʾ
DllExport int OC_CloseTip(HWND hwn, CString tipStr);

//ȡ���رյ�����ʾ
DllExport void OC_CancelCloseTip(HWND hwn);

//��ȡ��ǰӦ�õİ汾��
DllExport int OC_GetCurrentVersionInfo(CString &szVersion);

//��ȡ��ǰĿ¼�����ļ� 
DllExport int OC_ReadCurrentCfgFromIni(vector<OC_INI_INFO> &listInfo, CString fileName);

//���浱ǰĿ¼�����ļ�
DllExport int OC_SaveCurrentCfgToIni(vector<OC_INI_INFO> &listInfo, CString fileName);

