#pragma once
/*!
 * \class infotvģ��ʵ�ֵĸ���
 *
 * \brief ʹ�õ�ʱ����Ҫ�������� ���һ��� SYSTEM_PARAM_Parent ʵ���Լ��ṹ��
 *
 * \author HXY10
 * \date ʮ�� 2020
 */
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include <vector>
#include <string>
using namespace std;
typedef struct
{
	char szModule[64];				//ģ������
	//λ�ô�С
	int  iPosX, iPosY;
	int  iWidth, iHeight;

	//����
	int  iBkType;
	COLORREF crPure, crBlend1, crBlend2;
	int  iPureAlpha, iBlendAlpha1, iBlendAlpha2;
	int  iBlendColorType;							//����ɫ����
	char szBkFile[MAX_PATH];


	BOOL bStartByOther;				// �Ƿ�������ģ������

}	SYSTEM_PARAM_Parent;

typedef struct __GHG_PARAM
{
	//ϵͳ����
	SYSTEM_PARAM_Parent *sysParam;

	HCONTEXT context;
	int		iOldType;
	BOOL	bPlaying;
	BOOL	bPause;
	BOOL	bCleanScreenOnStop;	// ֹͣʱ�Ƿ���������
	UINT	uIndex;
	char	szErrorString[MAX_PATH];
	char	szCurPath[MAX_PATH];
	char	szOldPath[MAX_PATH];

	__GHG_PARAM()
	{

		context = 0;
		iOldType = 0;
		bPlaying = FALSE;
		bPause = FALSE;
		bCleanScreenOnStop = FALSE;
		uIndex = 0;
		memset(szOldPath, 0, MAX_PATH);
		memset(szCurPath, 0, MAX_PATH);
		memset(szErrorString, 0, MAX_PATH);
	}

}	GHG_PARAM, *PGHG_PARAM;

class CInfotvHandle_Parent
{

public:
	CInfotvHandle_Parent();
	~CInfotvHandle_Parent();

	//����Ĭ�ϵĲ���
	virtual void SetDefaultParam(GHG_PARAM *) = 0;

	//����������ļ�
	virtual bool SaveParamToFile(const SYSTEM_PARAM_Parent *param,CString szIniFile, CString szXmlFile) = 0;

	//�������ļ��ж�ȡ����
	virtual void ReadParamFromFile(SYSTEM_PARAM_Parent *param, CString szIniFile) = 0;

	//����ǰ����֤����
	virtual bool VerifyPlayData(GHG_PARAM *param) = 0;

	//��ʼ���� ���Ի��Ʊ��� �����鲥��
	virtual void StarPlay(GHG_PARAM * hGhg) = 0;

	//ֹͣ���� �ر��鲥��
	virtual void EndPlay(GHG_PARAM * pParam) = 0;

	//�������� ��ʱ���ᶨʱ����
	virtual void DrawContent(GHG_PARAM * hGhg, SYSTEMTIME sysTime, UINT uInterval) = 0;

	//��ȡ��Ҫ��ӵ��ļ� ������ӵı���ͼƬ�� ������·�����ļ� ������ ģ����
	virtual void GetNeedAddFiles(GHG_PARAM * pParam,vector<string>  &picList) = 0;

	//�����ӵ��ļ� ���� ���������ļ��Ĳ��� ���� ������ ָ���ļ���·��
	virtual void UpdateAddFileParam(GHG_PARAM * pParam,CStringArray &newFilePath) = 0;


public:
	//��ȡģ��Ĵ�С
	void GetModuleSize(UINT &nWidth, UINT &nHeight);

	//����Ĭ������
	void SetDefaultFont(FONT_PARAM &font, char * szFont, int iSize);
	
	//��ȡ��Ҫ���ŵ��ļ�·�� 
	CString GetPicFilePath(GHG_PARAM *pParam, CString strFileName);


	//��������
	void SetFontParam(HCONTEXT hContext, FONT_PARAM font);
	

	void SetColorParam(HCONTEXT context, FONT_PARAM font, FILL_TYPE fillType);
	

	//���������xmlԪ��
	TiXmlElement * CreateFontElement(LPCTSTR szName, FONT_PARAM * pFont);
	//���ñ�������������
	void  SetBackParamAndDraw(GHG_PARAM * pParam);

	//��ȡģ�������Ŀ¼����
	void GetModelDirAndFileName(CString &dir, CString &fileName);
};

