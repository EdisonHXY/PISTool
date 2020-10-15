/***********************************************************************************
*	��Ȩ����(c)2014 �ڻ����������
*	�ļ�����:	CtrlLib.h
*	�ļ�˵��:	�豸���ƿ������ͷ�ļ�
*	��ǰ�汾:	1.0.20140327
*	����:		��Ρ��
*	��������:	2014-03-27
*************************************************************************************/

#ifndef	_CTRLLIB_H
#define	_CTRLLIB_H

#include <afxtempl.h>


#define MAX_PDP_NUM			32

enum CMD_RESULT
{	
	CMD_RESULT_LOCAL_ERROR = -10,		//���ش�����������Ƿ�
	CMD_RESULT_CONN_ERROR,				//�������
	CMD_RESULT_SERVER_ERROR,			//�������δ���������߷�������Ѿ�����

	CMD_RESULT_ERROR = 0,				//ִ��ʧ�ܣ�����״̬��⣬����ԭ����szError����
	CMD_RESULT_OK,						//ִ�гɹ�

	//�����������
	CMD_RESULT_NO_CMD,					//û��ָ��������
	CMD_RESULT_NO_LOGON,				//û�е�¼����ʹ��

	//��¼�������
	CMD_RESULT_LOGON_FAIL,				//������û��������룬��ʹ��
	CMD_RESULT_NO_RIGHT,				//û��Ȩ�ޣ���ʹ��

	//����״̬���߲�����ʽ���ƴ������
	CMD_RESULT_LOW_RIGHT,				//Ȩ�޵ͣ����ܽ��н���״̬�Ŀ��ƣ������ǵ�ǰ���ڽ���״̬��ʵʩ�������Ƶ�Ȩ�޽ϵ�
	CMD_RESULT_LAYOUT_NOT_EXIST,		//��ʽ������
	CMD_RESULT_LIST_NOT_EXIST,			//�������ڣ���ʹ��
	CMD_RESULT_NOT_EMERGENT,			//ȡ������ʧ�ܣ���Ϊû�д��ڽ���״̬

	//TAKE����������
	CMD_RESULT_NO_PST,					//��ǰû��PST��Ŀ������ִ��TAKE����

	//��ʼ���к�ֹͣ���д������
	CMD_RESULT_NO_LIST,					//��ǰû�в����Ŀ������ִ������
	CMD_RESULT_NO_LAYOUT,				//��ǰʱ���û�а�ʽ������ִ������
	CMD_ERSULT_INVALID_INDEX,			//SKIP����İ�ʽ����߰�ʽ��Ŀ��ŷǷ�����ʹ��

	//û�а忨
	CMD_RESULT_NO_BOARD,				//��ʹ��

	//ֱ������Ĵ������
	CMD_RESULT_NO_LIVE_MODULE			//û��ֱ��ģ��
};

enum NET_ES_TYPE
{
	NET_ET_CONTENT = 0,					//���ý������ݣ�ͨ������ģ��ʵ�֣�szContentά����������
	NET_ET_LAYOUT,						//���ý�����ʽ��szContentά����ʽ�ļ���
	MAX_NET_ES_TYPE
};

enum DLL_LOAD_STATUS
{
	DL_STATUS_OK = 0,
	DL_STATUS_SET_NONE,
	DL_STATUS_LOAD_FAIL,
	DL_STATUS_INIT_FAIL,
	MAX_DL_STATUS
};

enum SCREEN_STATUS
{
	SCREEN_STATUS_OFF = 0,		//�ر�״̬
	SCREEN_STATUS_ON,			//����״̬
	SCREEN_STATUS_ERROR,		//����״̬
	SCREEN_STATUS_UNKNOWN,		//δ֪״̬
	MAX_SCREEN_STATUS
};

enum CTRL_CMD
{
	CTRL_CMD_SWITCH_DEVICE = 0,			//�豸���ػ��������iParamΪ0��ʾ�ػ���Ϊ1��ʾ������
	CTRL_CMD_SWITCH_SCREEN,				//�豸��Ļ���ػ��������iParamΪ0��ʾ�رգ�Ϊ1��ʾ������
	CTRL_CMD_END_CONTENT,				//��������״̬���iParam��Ч
	CTRL_CMD_SWITCH_RUNNING,			//��ʽ���м�ֹͣ�������iParamΪ0��ʾֹͣ���ţ�Ϊ1��ʾ��ʼ����
	CTRL_CMD_TAKE,						//TAKE���iParam��Ч
	CTRL_CMD_RESET_DEVICE,				//��λ���iParam��Ч
	CTRL_CMD_SWITCH_SOUND,				//���������ر����iParamΪ0��ʾ�رգ�Ϊ1��ʾ����
	CTRL_CMD_SET_VOLUME,				//����������С���iParamΪ������С��0��255
	CTRL_CMD_SWITCH_LIVE,				//ֱ���������iParamΪ0��ʾ����ֱ����Ϊ1��ʾ��ʼֱ��
	CTRL_CMD_WAKEUP_DEVICE = 100,		//���������豸��Ҫ���ѵ��豸IP��MACͨ��ctrl_ExecuteCmd������szError���ݽ�������NULL�ָ���
	CTRL_CMD_HD_SPACE = 200,			//�Ƿ�֧��Ӳ�̿ռ�����
};


typedef struct
{
	char	szDeviceCode[9];
	__int16 iExecResult;
	DATE    dtExecTime;

}	TASK_LIST, *PTASK_LIST;

#pragma pack(1)

//const	int		CTRLTYPE_NONE		= 0;	//δ֪
//const	int		CTRLTYPE_OK			= 1;	//֧��
//const	int		CTRLTYPE_NO			= 2;	//��֧��


//typedef struct
//{	
//	int  iRightLevel;					//Ȩ�޼����룬����Խ�󼶱�Խ��
//
//	int  iEsType;						//����״̬���ͣ���enum NET_ES_TYPE�ṹ����
//	char szContent[1025];				//��ʾ��Ҫ���õĽ���״̬���ݻ��߽�����ʽ���ƣ���NULL������
//	BOOL bFullScreen;					//��������ý������ݣ���ʾ�Ƿ�ʹ��ȫ����ʾ��
//										//��Ϊtrue����ʹ��ȫ��ʵ�ֽ���������ʾ������ֻʹ����Ƶ����
//	UINT uTimeLength;					//����״̬����ʱ�䣬��Ϊ0����ʾһֱ���ڽ���״̬��ֱ���ֶ�ֹͣ
//
//}	NET_ES_PARAM, * PNET_ES_PARAM;



typedef struct
{	
	int  iRightLevel;					//Ȩ�޼����룬����Խ�󼶱�Խ��

	int  iEsType;						//����״̬���ͣ���enum NET_ES_TYPE�ṹ����
	char szContent[1022];				//��ʾ��Ҫ���õĽ���״̬���ݻ��߽�����ʽ���ƣ���NULL������
	char iSoundType;					//����������Ϣʱ�����������ͣ�0��ʾ������1��ʾ�ر�������2��ʾ��������
	BYTE buLeftVolume;					//����������Ϣʱ����������������
	BYTE buRightVolume;					//����������Ϣʱ����������������
	BOOL bFullScreen;					//��������ý������ݣ���ʾ�Ƿ�ʹ��ȫ����ʾ��
										//��Ϊtrue����ʹ��ȫ��ʵ�ֽ���������ʾ������ֻʹ����Ƶ����
	WORD wTimeLength;					//����״̬����ʱ�䣬��Ϊ0����ʾһֱ���ڽ���״̬��ֱ���ֶ�ֹͣ
	WORD wReplaceInfo;					//������Ϣ�Ĳ���ģʽ��1��ʾ�滻֮ǰ�ģ�0��ʾ׷�ӵ����
//	UINT uTimeLength;					//����״̬����ʱ�䣬��Ϊ0����ʾһֱ���ڽ���״̬��ֱ���ֶ�ֹͣ

}	NET_ES_PARAM, * PNET_ES_PARAM;


typedef struct
{
	char szStatus[924];					//�趨����ʾ��Ϣ�����ݻ��߷��ص�״̬�ַ���
										//����CHECK_STATUS����������ݼ����������
	char szError[196];					//���صĴ�������
										//����CHECK_STATUS�����Ӳ�̿ռ�澯ʱ��Ӧ�������صĸ澯��Ϣ
	_int8 bEmergent;					//�Ƿ��ڽ���״̬
	//Ӳ��״̬
	_int8 bHDProperty;					//�Ƿ�߱�Ӳ�̿ռ�����
	_int8 iHDStatus;					//Ӳ�̼������CMD_RESULT_OK����CMD_RESULT_ERROR
	char szHDStatus[256];				//Ӳ���߼��������Ŀռ�����
	ULONGLONG uHDSumSpace;				//�����زĵ�Ӳ���ܿռ䣬��λΪ�ֽڣ���Ϊ0����ʾ�豸��֧��Ӳ�̿ռ�����
	ULONGLONG uHDFreeSpace;				//�����زĵ�Ӳ��ʣ��ռ䣬��λΪ�ֽ�
	//��Ӧ���ܿ���Ļ״̬
	_int8 bScreenProperty;				//�Ƿ�߱��ܿ���Ļ����
	char szPDPName[32];					//�ܿ���Ļ���ƣ������Դ��������PDP��
	_int8 iScreenDllOk;					//��Ļ����DLL�ļ���״̬����DLL_LOAD_STATUS����
	_int8 iPDPNum;						//�ܿ���Ļ����
	_int8 iPDPStatus[MAX_PDP_NUM];		//��SCREEN_STATUSö�ٽṹ����

}	CHECK_STATUS_PARAM, * PCHECK_STATUS_PARAM;

typedef struct
{	
	int  iModuleNum;					//ģ�����
	char szPlayContent[1280];			//�������������ַ���

}	PLAY_CONTENT, * PPLAY_CONTENT;


//���ֶ������Բ���������2�ֽ�
typedef struct tagSTRPARAM
{
	WORD chCmd;			//��������
	WORD uCmdParam;	    //�������

}STRPARAM, *PSTRPARAM;

//������ʾ�����������ݰ�ͷ
typedef struct tagLEDFONTPARAM
{
	  DWORD command;	//���ݰ������ʶ�����½�Ŀ����(=0x55AA5501)��
	  DWORD dst_count;  //Ŀ���豸����
	  //Ŀ���豸��ַ�����ڱ�ʶ��ʾ����ȫ��������ʾ��Ҫ����ͳһ���
	  WORD  dst_addr[256];
	  DWORD param_count;  //��������
	  STRPARAM param[16];  //���ֶ�������
} LEDFONTPARAM, *PLEDFONTPARAM;


#pragma pack()

typedef void *	HWINDIB;		//����windows��DIB����


//ģ��Ϸ��Ժ���
//������ͨ�����øú����������ж�һ��DLL�Ƿ��ǺϷ���ģ�顣����û����޹ص�DLL�ŵ�ģ���Ŀ¼�£�ͨ���ú����Ϳ��Ա���Ѹ�DLL����Ϊϵͳ��ģ�顣������ֻ��Ҫ�ڽ���������ģ��ʱ����һ�θú������ɡ�����ԭ�����£�
//����ģ��İ汾�ţ�������ڵ���������İ汾�ţ����ʾDLL�ǺϷ���ģ�飬���򲻺Ϸ�����Ӧ���ٽ��������ĺ������á�
typedef UINT (* lpctrl_GetDllVersion)();									//get dll version

//ģ���ʼ������
//������ͨ�����øú�����ʹ�ø���ģ����Գ�ʼ�����ԵĲ�����������������ȵȡ�������ֻ��Ҫ�ڽ���������ģ��ʱ����һ�θú������ɡ�����ԭ�����£�
//��������һ��������ָ�룬��Ҫ���������ά�����������NULL����ʾ��ʼ�����ɹ���
//szComOrIP��ʾҪ���Ƶ��豸��IP��ַ��szMacΪ�������ַ��iRightLevelΪ�����ߺ��û���Ȩ��
typedef void * (* lpctrl_InitDll)(LPCTSTR szComOrIP, LPCTSTR szMac, int iRightLevel);		//initial dll, such as creating network conection, open com.

//ģ��ǳ�ʼ������
//������ͨ�����øú�����ʹ�ø���ģ������ͷŸ��Ե��ڴ�ȵȡ�������ֻ��Ҫ���˳������ͷ�ģ��ʱ����һ�θú������ɡ�����ԭ�����£�
//�������true����ʾ�ǳ�ʼ���ɹ���
typedef BOOL (* lpctrl_UninitDll)(void * lpVoid);							//Uninitial dll, such as stop running

//��ʾ�������ú���
//������ͨ�����øú�������������ģ������öԻ��򣬵������ֲ��������û���Ҫ����һ��ģ��Ĳ���ʱ����������Ҫ���ø�ģ��Ĳ������ú���������ԭ�����£�
//�������IDOK����ʾģ��Ĳ��������˸Ķ���
typedef int  (* lpctrl_ShowConfigDlg)();									//show config dialog

//��ȡģ�����ƺ���
//������ͨ�����øú�������ȡģ������ƣ�ʹ���û��ܹ�ʶ��ģ�顣����ԭ�����£�
//�ú�������ģ�����Ƶ�ʵ�ʳ��ȣ���λ�ֽڡ�
typedef int  (* lpctrl_GetName)(char * szName, int iSize);					//get dll module name


//��ȡ��ǰ����״̬����
//pES����ά�����صĽ���״̬���ݡ����ؽ����ö������CMD_RESULT����
typedef int  (* lpctrl_GetContent)(void* lpVoid, PNET_ES_PARAM pES);

//���ý���״̬
//pES��ʾҪ���õĽ���״̬���ݡ����ؽ����ö������CMD_RESULT����
typedef int  (* lpctrl_SetContent)(void* lpVoid, PNET_ES_PARAM pES);		//set content

//״̬��⺯��
//������ͨ�����øú���������ܼ���豸��״̬������ԭ�����£�
//����ֵΪint���ͣ�����ö������������pParamά�����ص�״̬����
//����szStatus����"����1\tֵ\n����2\tֵ\n"�ı��뷽ʽ��ʹ����������Բ���ListCtrl�ؼ����б�ķ�ʽ����״̬��ʾ��
typedef int  (* lpctrl_CheckStatus)(void * lpVoid, PCHECK_STATUS_PARAM pParam);		//The param is the value returned by InitDll function

//���Ⱥ��������ؽ����ö������CMD_RESULT������iCmd��ʾִ�е��������ͣ�iParamΪ���������
//szErrorά�����صĴ���������iLen��ʾ�������ĳ��ȣ�����������ֻ���CTRL_CMD_SWITCH_SCREEN������Ч
typedef int  (* lpctrl_ExecuteCmd)(void * lpVoid, int iCmd, int iParam, PCHAR szError, int iLen);
//�ж��Ƿ����ָ������
typedef BOOL (* lpctrl_SupportCmd)(int iCmd);
//��ȡ��ǰ������������
//pParam����ά�����ص��������ݡ����ؽ����ö������CMD_RESULT����
typedef int(*lpctrl_GetPlayContent)(void* lpVoid, PPLAY_CONTENT pParam);	// ��ò���ģ�������

//////////////////////////////////////////////////////////////////////////////////////
class CCtrlLib
{	
public:
	CCtrlLib(void);
	~CCtrlLib(void);

	BOOL LoadFunction(const char *szPath);
	bool isLoadSuccess();


public:
	HINSTANCE m_hctrlDll;
	lpctrl_InitDll			ctrl_InitDll;
	lpctrl_UninitDll		ctrl_UninitDll;
	lpctrl_GetContent		ctrl_GetContent;
	lpctrl_SetContent		ctrl_SetContent;
	lpctrl_CheckStatus		ctrl_CheckStatus;
	lpctrl_ExecuteCmd		ctrl_ExecuteCmd;
	lpctrl_SupportCmd		ctrl_SupportCmd;
	lpctrl_GetPlayContent	ctrl_GetPlayContent ;
	
};

#endif