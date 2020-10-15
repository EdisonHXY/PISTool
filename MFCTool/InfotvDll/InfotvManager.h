#pragma once
/*!
 * \class ����infotvģ����
 *
 * \brief 
 *
 * \author HXY10
 * \date ʮ�� 2020
 */
#include "InfotvHandle_Parent.h"
#include "ConfigDlg_Parent.h"

class CInfotvManager
{
public:
	CInfotvManager();
	~CInfotvManager();

	//���ü̳�Infotv_Parent����
	static void SetInstance(CInfotvHandle_Parent *infotv);
	
	//���ü̳�SYSTEM_PARAM_Parent����
	static void SetParamInstance(SYSTEM_PARAM_Parent *param);

	//���ü̳�ConfigDlg_Parent����
	static void SetDlgInstance(CConfigDlg_Parent *dlg);

	//������󴴽�ʵ����
	static void SetCreatMaxNum(UINT maxNum);

	//���� dll ������·���� dll����
	static void SetCurDirAndModueName(CString dir,CString name);

	//���� ģ������� ��  ����ͼ��
	static void SetDescription(HBITMAP * pHBmp, char * szBuf);

	static bool IsExistFile(LPCTSTR szFile);

	/******************
		
			����Ĳ����ͷ����ڲ�ʹ�ã��������ú͵���

	*******************************************/
public:
	//��ȡʵ�� �Լ�ʹ��
	static CInfotvHandle_Parent *GetInstance();
	static CConfigDlg_Parent *GetDlgInstance();
	static SYSTEM_PARAM_Parent *GetParamInstance();

public:
	static 	HBITMAP __hBmp[3];   //ģ���ͼ��
	static char __szTempName[50]; //ģ����
	static CInfotvHandle_Parent *__infotv;
	static CConfigDlg_Parent * __cfgDlg;
	static SYSTEM_PARAM_Parent *__sysParam;
	static char __szInfotvVersion[50]; //��¼infotv�İ汾��
	static UINT __maxInstanceNum;//��¼��󴴽�������
	static CUIntArray		__guInstance; //��¼���д�����ʵ��
	static UINT			__guMaxNum;      //��¼ʵ��������

	static UINT				__giHeight;	// ��Ļ��
	static UINT				__giWidth;	// ��Ļ��
	static BOOL			__gbPlayMode;	// �ж��������Ƿ���֧�ֲ��ŵĳ���

	static CString __strCurDir; //���е�Ŀ¼
	static CString __strModuleName; //ģ����

};

