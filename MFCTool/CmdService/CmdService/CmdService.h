#pragma once
#pragma pack(1)

enum NET_ES_TYPE
{
	NET_ES_CONTENT = 0,					//���ý������ݣ�ͨ������ģ��ʵ�֣�szContentά����������
	NET_ES_LAYOUT,						//���ý�����ʽ��szContentά����ʽ�ļ���
	MAX_NET_ES_TYPE
};

enum NET_ES_SOUND_TYPE
{
	NET_ES_SOUND_NONE = 0,
	NET_ES_SOUND_OFF,
	NET_ES_SOUND_SET_VOLUME,
	MAX_NET_ES_SOUND_TYPE
};



enum MACRO_TYPE_INDEX
{
	MT_POWER_ON = 0,					//�豸��������	��������ṩ�ù���
	MT_POWER_OFF,						//�豸�ػ�����	��������ṩ�ù���
	MT_RESET_DEVICE,					//������������	��������ṩ�ù���
	MT_SCREEN_ON,						//�豸��ʾ����������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SCREEN_OFF,						//�豸��ʾ���ػ�����	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_PUBLISH_INFO,					//����������Ϣ�����ʵʩ����״̬	������������ṩ�ù���
	MT_STOP_PUBLISH,					//ȡ��������Ϣ�������������״̬	������������ṩ�ù���
	MT_START_RUNNING,					//��ʼ��������	��������ṩ�ù���
	MT_STOP_RUNNING,					//ֹͣ��������	��������ṩ�ù���
	MT_TAKE,							//TAKE����	��������ṩ�ù���
	MT_SOUND_ON,						//������������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SOUND_OFF,						//�ر���������	��������ṩ�ù��ܣ�������������ṩ�ù���
	MT_SET_VOLUME,						//����������0��255��	��������ṩ�ù���
	MT_LIVE_ON,							//��ʼֱ��	��������ṩ�ù���
	MT_LIVE_OFF,						//ֱֹͣ��	��������ṩ�ù���
	MAX_MACRO_TYPE
};




typedef struct tagTRAINEMMSG
{
	BYTE byHead;
	WORD wLen;
	BYTE byFunc;
	BYTE byFlag;
	WORD wID;
	BYTE byType;
	BYTE byClear;
	char szStartTm[19];
	char szEndTm[19];
	char szBufText[4096];

}	TRAINEMMSG, *LPTRAINEMMSG;



enum CMD_RESULT
{
	CMD_RESULT_WAIT_EXEC = -100,	//�ȴ�����
	CMD_RESULT_LOCAL_ERROR = -2,		//���ش�����������Ƿ�
	CMD_RESULT_CONN_ERROR,				//�������

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

typedef struct
{
	char	szDeviceCode[9];
	__int16 iExecResult;
	DATE    dtExecTime;

}	TASK_LIST, *PTASK_LIST;

typedef struct
{
	TASK_LIST TaskList;
	char	szTaskID[33];

}	TASK_RESULT, *PTASK_RESULT;

typedef struct
{
	char	szTaskID[33];
	__int8	iTaskType;
	__int8	iEmType;
	__int8	iFullScreen;
	__int8	iReplaceInfo;
	__int8	iSoundType;
	BYTE	buLeftVolume;
	BYTE	buRightVolume;
	char	szEmergencyInfo[1022];
	DATE	dtTaskTime;				//����ʱ�䣬���ܷ�����������ʱ������д����ʱ�䣬��CmdService���յ�������Ϊ������ʱ�䣬
									//�豸����ѯ��������ʱֱ����CmdService����ʱ�������Ч�жϣ��Ӷ�����ʱ�䲻һ�´������жϴ���
	int 	iValidTime;				//��Чʱ������λ��
	__int16 iTimeLength;
	char    szEditorID[17];
	BYTE	buRightLevel;

	//�豸�б�
	CMapStringToPtr * pMapDevice;

}	TASK_SET, *PTASK_SET;


typedef struct
{
	char szDeviceCode[10];
	char szDeviceName[45];
	char szDeviceIP[20];
	char szStationCode[8];
	char szStationName[45];

	char szError[196];
	char szStatus[924];

	_int8 iStatus;

}DEVICE_INFO, *LPDEVICE_INFO;				//��վ��Ϣ�ṹ

#define FLAG_SIZE				4
#define CMD_FLAG				"Cmd"

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

#define MAX_PDP_NUM			32 
typedef struct
{
	char szStatus[924];					//�趨����ʾ��Ϣ�����ݻ��߷��ص�״̬�ַ���
										//����CHECK_STATUS����������ݼ����������
	char szError[194];					//���صĴ�������
										//����CHECK_STATUS�����Ӳ�̿ռ�澯ʱ��Ӧ�������صĸ澯��Ϣ
	_int8 iStatus;						//�豸״̬
	_int8 iAlarmLevel;					//���ϵȼ�
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

}CHECK_STATUS_PARAM, *PCHECK_STATUS_PARAM;


typedef struct
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
}	NET_ES_PARAM, *PNET_ES_PARAM;

typedef struct
{
	int  iModuleNum;					//ģ�����
	char szPlayContent[1280];			//�������������ַ���

}	PLAY_CONTENT, *PPLAY_CONTENT;

enum CMD_TYPE
{
	CMD_PUBLISH_TASK = 0,			//���ܷ�����������
	CMD_QUERY_TASK_RESULT,			//���ܲ�ѯ��������ִ�н��
	CMD_QUERY_STATUS,				//���ܲ�ѯ�豸״̬
	CMD_QUERY_SINGLE_STATUS,		//���ܲ�ѯ�����豸״̬

	CMD_QUERY_TASK,					//�豸��ѯ�Լ��Ĳ�������
	CMD_FEEDBACK_TASK_RESULT,		//�豸�������������ִ�н��
	CMD_PUBLISH_STATUS,				//�豸�����Լ���״̬
};


typedef struct
{
	char szDeviceCode[9];
	__int8	iCheckResult;			//�豸״ֵ̬
	__int8	iAlarmLevel;			//�����ȼ���0��4��0��ʾ�޹���
	__int16 iReserve;				//Ԥ��
	union
	{
		DATE dtUpdateTime;			//״̬���ݵĸ���ʱ�䣬�豸�ϱ�״̬��CmdServiceʱ����д����ʱ�䣬��CmdService���յ�������Ϊ������ʱ�䣬
		int  iTimeOffset;			//��������ѯ�豸״̬ʱ��CmdService����ѯʱ���ȥ����ʱ�䣬���ʱ���iTimeOffset������״̬���ݵ���Ч�̶ȣ�����Ӧֻ�ж�iTimeOffset
	};
	CHECK_STATUS_PARAM csParam;		//״̬�������ṹ
	NET_ES_PARAM emParam;			//����״̬���Ʋ����ṹ
	PLAY_CONTENT PlayContent;		//�������ݽṹ

}	DEVICE_STATUS, *PDEVICE_STATUS;

#pragma pack()
