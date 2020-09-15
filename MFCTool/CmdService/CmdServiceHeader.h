#pragma once

#define FLAG_SIZE				4
#define CMD_FLAG				"Cmd"
#define MAX_PDP_NUM			32 

#pragma pack(1)

enum CMD_TYPE
{
	CMD_PUBLISH_TASK = 0,			//���ܷ�����������
	CMD_QUERY_TASK_RESULT,			//���ܲ�ѯ��������ִ�н��
	CMD_QUERY_STATUS,				//���ܲ�ѯ�豸״̬
	CMD_QUERY_SINGLE_STATUS,		//���ܲ�ѯ�����豸״̬

	CMD_QUERY_TASK,					//�豸��ѯ�Լ��Ĳ�������
	CMD_FEEDBACK_TASK_RESULT,		//�豸�������������ִ�н��
	CMD_PUBLISH_STATUS,				//�豸�����Լ���״̬

	//����4��ָ��ֻ��Է���PIS�ӿڳ�����Ч
	CMD_SET_CONFIG,					//ϵͳ����������÷������ã�Ԥ��
	CMD_QUERY_SETCONFIG_RESULT,		//ϵͳ���������ѯ��������ִ�н����Ԥ��
	CMD_GET_CONFIG,					//ϵͳ���������ȡ�����豸��Ϣ��Ԥ��
	CMD_QUERY_GETCONFIG_RESULT,		//ϵͳ���������ѯ��ȡ�����豸ִ�н����Ԥ��

	CMD_CANCEL_TASK = 50,			//ȡ����������

	CMD_TEST = 80,					//���Է����Ƿ�������ָ��
};

enum DLL_LOAD_STATUS
{
	DL_STATUS_OK = 0,			//��Ļ����DLL��������
	DL_STATUS_SET_NONE,			//δ������Ļ����DLL
	DL_STATUS_LOAD_FAIL,		//��Ļ����DLL����ʧ��
	DL_STATUS_INIT_FAIL,		//��Ļ����DLL��ʼ��ʧ��
	MAX_DL_STATUS
};

enum SCREEN_STATUS
{
	SCREEN_STATUS_OFF = 0,				//�ر�״̬
	SCREEN_STATUS_ON,					//����״̬
	SCREEN_STATUS_ERROR,				//����״̬
	SCREEN_STATUS_UNKNOWN,				//δ֪״̬
	MAX_SCREEN_STATUS
};

enum MACRO_TYPE_INDEX
{
	MT_POWER_ON = 0,	//�豸��������	��������ṩ�ù���
	MT_POWER_OFF,		//�豸�ػ�����	��������ṩ�ù���
	MT_RESET_DEVICE,	//������������	��������ṩ�ù���
	MT_SCREEN_ON,	//�豸��ʾ����������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SCREEN_OFF,	//�豸��ʾ���ػ�����	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_PUBLISH_INFO,	//����������Ϣ�����ʵʩ����״̬	������������ṩ�ù���
	MT_STOP_PUBLISH,	//ȡ��������Ϣ�������������״̬	������������ṩ�ù���
	MT_START_RUNNING,	//��ʼ��������	��������ṩ�ù���
	MT_STOP_RUNNING,	//ֹͣ��������	��������ṩ�ù���
	MT_TAKE,			//TAKE����	��������ṩ�ù���
	MT_SOUND_ON,		//������������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SOUND_OFF,		//�ر���������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SET_VOLUME,		//����������0��255��	��������ṩ�ù���
	MT_LIVE_ON,			//��ʼֱ��	��������ṩ�ù���
	MT_LIVE_OFF,		//ֱֹͣ��	��������ṩ�ù���
	MAX_MACRO_TYPE
};

enum NET_ES_TYPE
{
	NET_ES_CONTENT = 0,		//���ý������ݣ�ͨ������ģ��ʵ�֣�szContentά����������
	NET_ES_LAYOUT,			//���ý�����ʽ��szContentά����ʽ�ļ���
	MAX_NET_ES_TYPE
};

enum NET_ES_SOUND_TYPE
{
	NET_ES_SOUND_NONE = 0,//������
	NET_ES_SOUND_OFF,//�ر�����
	NET_ES_SOUND_SET_VOLUME,//��������
	MAX_NET_ES_SOUND_TYPE
};

enum CMD_RESULT
{
	CMD_RESULT_WAIT_EXEC = -100,	//�ȴ�����

	CMD_RESULT_DB_ERROR = -90,		//���ݿ���ʴ���

	CMD_RESULT_LOCAL_ERROR = -10,	//���ش�����������Ƿ�
	CMD_RESULT_CONN_ERROR,			//�������
	CMD_RESULT_SERVER_ERROR,		//�������δ���������߷�������Ѿ�����
	CMD_RESULT_POWER_OFF,			//�豸�ѹػ�
	CMD_RESULT_RESET,				//�豸������
	CMD_RESULT_PLAY_DOWN,			//������ֹͣ

	CMD_RESULT_ERROR = 0,				//ִ��ʧ�ܣ�����״̬��⣬����ԭ����szError����
	CMD_RESULT_OK,						//ִ�гɹ�

	//�����������
	CMD_RESULT_NO_CMD,					//û��ָ��������
	CMD_RESULT_NO_LOGON,				//û�е�¼����ʹ��

	//��¼�������
	CMD_RESULT_LOGON_FAIL,				//������û��������룬��ʹ��
	CMD_RESULT_NO_RIGHT,				//û��Ȩ�ޣ���ʹ��

	//����״̬���߲�����ʽ���ƴ������
	CMD_RESULT_LOW_RIGHT,	//Ȩ�޵ͣ����ܽ��н���״̬�Ŀ��ƣ������ǵ�ǰ���ڽ���״̬��ʵʩ�������Ƶ�Ȩ�޽ϵ�
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
	CMD_RESULT_NO_LIVE_MODULE,			//û��ֱ��ģ��

	//��������������
	CMD_RESULT_BOARD_ERROR = 50,		//�忨����
	CMD_RESULT_CL_ERROR,				//�ӿڼ�������
	CMD_RESULT_WDT_ERROR,				//wdt����
	CMD_RESULT_SCREEN_ERROR,			//��Դ����������
	CMD_RESULT_UPS_ERROR,				//UPSģ�����
	CMD_RESULT_HD_ERROR,				//Ӳ�̹���
	CMD_RESULT_LOW_SPACE,				//Ӳ�̿ռ䲻��
	CMD_RESULT_CPU_ERROR,				//CPU�¶ȹ���
	CMD_RESULT_CPU_OVERLOAD,			//CPUռ���ʹ���
};


typedef struct __NET_ES_PARAM
{
	int  iRightLevel;					//Ȩ�޼����룬����Խ�󼶱�Խ��

	int  iEsType;						//����״̬���ͣ���enum NET_ES_TYPE�ṹ����
	char szContent[1022];				//��ʾ��Ҫ���õĽ���״̬���ݻ��߽�����ʽ���ƣ���NULL������
	char iSoundType;					//����������Ϣʱ�����������ͣ�0��ʾ������1��ʾ�ر�������2��ʾ������������ö�ٽṹNET_ES_SOUND_TYPE����
	BYTE buLeftVolume;					//����������Ϣʱ����������������
	BYTE buRightVolume;					//����������Ϣʱ����������������
	BOOL bFullScreen;					//��������ý������ݣ���ʾ�Ƿ�ʹ��ȫ����ʾ��
										//��Ϊtrue����ʹ��ȫ��ʵ�ֽ���������ʾ������ֻʹ����Ƶ����
	WORD wTimeLength;					//����״̬����ʱ�䣬��Ϊ0����ʾһֱ���ڽ���״̬��ֱ���ֶ�ֹͣ
	WORD wReplaceInfo;					//������Ϣ�Ĳ���ģʽ��1��ʾ�滻֮ǰ�ģ�0��ʾ׷�ӵ����

	__NET_ES_PARAM()
	{
		strcpy(szContent, "");
		iSoundType = 0;
	}

}	NET_ES_PARAM, *PNET_ES_PARAM;

typedef struct __PLAY_CONTENT
{
	int  iModuleNum;					//ģ�����
	char szPlayContent[1280];			//�������������ַ���

	__PLAY_CONTENT()
	{
		iModuleNum = 0;
		strcpy(szPlayContent, "");
	}

}	PLAY_CONTENT, *PPLAY_CONTENT;

typedef struct
{
	char	szDeviceCode[9];//�������񷢲��Ķ�����豸���
	__int16 iExecResult;//�ͻ��������������ǰ�����뽫�ó�Ա�趨ΪCMD_RESULT_WAIT_EXEC����ʾ�ȴ������ͻ��������ȡ���������ִ�н��ʱ���ó�Ա��ʾ��Ӧ�豸��ִ�н������ö�ٽṹCMD_RESULT����
	DATE    dtExecTime;//�ͻ��������ȡ���������ִ�н��ʱ���ó�Ա��ʾ��Ӧ�豸��ִ��ʱ�䡣

}	TASK_LIST, *PTASK_LIST;


typedef struct
{
	TASK_LIST TaskList;
	char	szTaskID[33];

}	TASK_RESULT, *PTASK_RESULT;

typedef struct 
{
	char szHead[FLAG_SIZE];			//ͷ����ʶ���̶�Ϊ"Cmd"
	int  iDataSize;					//���ṹ�Լ������������ݵ��ֽڴ�С
	BYTE buType;					//�������ͣ���CMD_TYPE����
	BYTE buAck;						//1��ʾ����Ĵ𸴣�0��ʾԭʼ����
	BYTE buCheckInterval;			//����������趨�ļ�ز�ѯ�������λ��
	int  iNum;						//����TASK_LIST����DEVICE_STATUS�ṹ������	
	union
	{
		char szTaskID[33];			//���CMD_QUERY_TASK_RESULT��ѯ������Ч
		char szDeviceID[9];			//���CMD_QUERY_TASK��ѯ������Ч
	};

}	CMD_HEAD, *PCMD_HEAD;

typedef struct
{
	char	szTaskID[33];//���������ID��ţ��ͻ��������Ҫ�Զ����ɣ���ȷ��ȫ��Ψһ��
	__int8	iTaskType;//������������ͣ���ö�ٽṹMACRO_TYPE_INDEX����������֧�����е�MT_POWER_ON����
	__int8	iEmType;//��������Ϊ����������Ϣʱ�ķ�ʽ�������ı���������ʽ������ö�ٽṹNET_ES_TYPE����
	__int8	iFullScreen;//��������Ϊ����������Ϣʱ����ʾ��ʽ��1��ʾȫ����ʾ��0��ʾ������ʾ
	__int8	iReplaceInfo;//��������Ϊ����������Ϣʱ�Ĳ�����ʽ��1��ʾ���ǣ�0��ʾ���棻
	__int8	iSoundType;//��������Ϊ����������Ϣʱ����������ʽ���������ر���������������������ö�ٽṹNET_ES_SOUND_TYPE����
	BYTE	buLeftVolume;//��������Ϊ�����������߷���������Ϣ��ͬʱ��������ʽΪ����������ʱ����������������Χ0��255
	BYTE	buRightVolume;//��������Ϊ�����������߷���������Ϣ��ͬʱ��������ʽΪ����������ʱ����������������Χ0��255
	char	szEmergencyInfo[1022];//��������Ϊ����������Ϣʱ�Ľ����ı����߽�����ʽ���ƣ�
	DATE	dtTaskTime;//��������ķ���ʱ�䣬CmdService���պ����������ʱ�������趨�ó�Ա��
	int 	iValidTime;				//��Чʱ������λ��
	__int16 iTimeLength;//��������Ϊ����������Ϣʱ������״̬�ĳ���ʱ�䣬��λ�룬���Ϊ0����ʾ����״̬������ֹͣ����
	char    szEditorID[17];//�������񷢲��ߵ�ID
	BYTE	buRightLevel;//�������񷢲��ߵ�Ȩ�޼���0��255������Խ�󣬼���Խ��

	//�豸�б�
	CMapStringToPtr * pMapDevice;

}	TASK_SET, *PTASK_SET;

typedef struct __CHECK_STATUS_PARAM
{
	char szStatus[924];					//�趨����ʾ��Ϣ�����ݻ��߷��ص�״̬�ַ���
	char szError[194];					//���صĴ�������
	_int8 iStatus;						//�豸״̬
	_int8 iAlarmLevel;					//���ϵȼ�
	_int8 bEmergent;					//�Ƿ��ڽ���״̬
										//Ӳ��״̬
	_int8 bHDProperty;				//�Ƿ�߱�Ӳ�̿ռ�����
	_int8 iHDStatus;				//Ӳ�̼������CMD_RESULT_OK����CMD_RESULT_ERROR
	char szHDStatus[256];				//Ӳ���߼��������Ŀռ�����
	ULONGLONG uHDSumSpace;				//�����زĵ�Ӳ���ܿռ䣬��λΪ��
	ULONGLONG uHDFreeSpace;				//�����زĵ�Ӳ��ʣ��ռ䣬��λΪ�ֽ�
										//��Ӧ���ܿ���Ļ״̬
	_int8 bScreenProperty;				//�Ƿ�߱��ܿ���Ļ����
	char szPDPName[32];					//�ܿ���Ļ���ƣ������Դ��������PDP��
	_int8 iScreenDllOk;					//��Ļ����DLL�ļ���״̬����DLL_LOAD_STATUS����
	_int8 iPDPNum;						//�ܿ���Ļ����
	_int8 iPDPStatus[MAX_PDP_NUM];		//��SCREEN_STATUSö�ٽṹ����

	__CHECK_STATUS_PARAM()
	{
		strcpy(szStatus, "");
		strcpy(szError, "");
		strcpy(szPDPName, "");
		strcpy(szHDStatus, "");
		memset(iPDPStatus, 0, MAX_PDP_NUM);
	}

}	CHECK_STATUS_PARAM, *PCHECK_STATUS_PARAM;

typedef struct __DEVICE_STATUS
{
	char szDeviceCode[9];
	__int8	iReserve1;				//Ԥ��
	__int8	iReserve2;				//Ԥ��
	__int16 iReserve;				//Ԥ��
	union
	{
		DATE dtUpdateTime;
		int  iTimeOffset;
	};
	CHECK_STATUS_PARAM csParam;		//״̬�������ṹ
	NET_ES_PARAM emParam;			//����״̬���Ʋ����ṹ
	PLAY_CONTENT PlayContent;		//�������ݽṹ

	__DEVICE_STATUS()
	{
		iTimeOffset = 0;
		memset(szDeviceCode, 0, 9);
		iReserve = iReserve1 = iReserve2 = 0;
		memset(&csParam, 0, sizeof(CHECK_STATUS_PARAM));
		memset(&emParam, 0, sizeof(NET_ES_PARAM));
		memset(&PlayContent, 0, sizeof(PLAY_CONTENT));
	}

}	DEVICE_STATUS, *PDEVICE_STATUS;



#pragma pack()

