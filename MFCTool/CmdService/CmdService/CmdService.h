#pragma once
#pragma pack(1)

enum NET_ES_TYPE
{
	NET_ES_CONTENT = 0,					//采用紧急内容，通过紧急模块实现，szContent维护紧急内容
	NET_ES_LAYOUT,						//采用紧急版式，szContent维护版式文件名
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
	MT_POWER_ON = 0,					//设备开机命令	网管软件提供该功能
	MT_POWER_OFF,						//设备关机命令	网管软件提供该功能
	MT_RESET_DEVICE,					//重新启动命令	网管软件提供该功能
	MT_SCREEN_ON,						//设备显示屏开机命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SCREEN_OFF,						//设备显示屏关机命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_PUBLISH_INFO,					//发布紧急信息命令，即实施紧急状态	紧急管理软件提供该功能
	MT_STOP_PUBLISH,					//取消紧急信息命令，即结束紧急状态	紧急管理软件提供该功能
	MT_START_RUNNING,					//开始运行命令	网管软件提供该功能
	MT_STOP_RUNNING,					//停止运行命令	网管软件提供该功能
	MT_TAKE,							//TAKE命令	网管软件提供该功能
	MT_SOUND_ON,						//开启声音命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SOUND_OFF,						//关闭声音命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SET_VOLUME,						//设置音量（0～255）	网管软件提供该功能
	MT_LIVE_ON,							//开始直播	网管软件提供该功能
	MT_LIVE_OFF,						//停止直播	网管软件提供该功能
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
	CMD_RESULT_WAIT_EXEC = -100,	//等待处理
	CMD_RESULT_LOCAL_ERROR = -2,		//本地错误，例如参数非法
	CMD_RESULT_CONN_ERROR,				//网络故障

	CMD_RESULT_ERROR = 0,				//执行失败，对于状态检测，故障原因由szError描述
	CMD_RESULT_OK,						//执行成功

	//错误命令序号
	CMD_RESULT_NO_CMD,					//没有指定的命令
	CMD_RESULT_NO_LOGON,				//没有登录，不使用

	//登录错误序号
	CMD_RESULT_LOGON_FAIL,				//错误的用户名或密码，不使用
	CMD_RESULT_NO_RIGHT,				//没有权限，不使用

	//紧急状态或者播表、版式控制错误序号
	CMD_RESULT_LOW_RIGHT,				//权限低，不能进行紧急状态的控制，或者是当前处于紧急状态，实施其他控制但权限较低
	CMD_RESULT_LAYOUT_NOT_EXIST,		//版式不存在
	CMD_RESULT_LIST_NOT_EXIST,			//播表不存在，不使用
	CMD_RESULT_NOT_EMERGENT,			//取消设置失败，因为没有处于紧急状态

	//TAKE命令错误序号
	CMD_RESULT_NO_PST,					//当前没有PST节目，不能执行TAKE命令

	//开始运行和停止运行错误序号
	CMD_RESULT_NO_LIST,					//当前没有播表节目，不能执行命令
	CMD_RESULT_NO_LAYOUT,				//当前时间段没有版式，不能执行命令
	CMD_ERSULT_INVALID_INDEX,			//SKIP命令的版式组或者版式节目序号非法，不使用

	//没有板卡
	CMD_RESULT_NO_BOARD,				//不使用

	//直播命令的错误序号
	CMD_RESULT_NO_LIVE_MODULE			//没有直播模块

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
	DATE	dtTaskTime;				//任务时间，网管发布操作任务时，可填写本机时间，但CmdService接收到后会更新为自身本机时间，
									//设备来查询操作任务时直接以CmdService本机时间进行有效判断，从而避免时间不一致带来的判断错误
	int 	iValidTime;				//有效时长，单位秒
	__int16 iTimeLength;
	char    szEditorID[17];
	BYTE	buRightLevel;

	//设备列表
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

}DEVICE_INFO, *LPDEVICE_INFO;				//车站信息结构

#define FLAG_SIZE				4
#define CMD_FLAG				"Cmd"

typedef struct
{
	char szHead[FLAG_SIZE];			//头部标识，固定为"Cmd"
	int  iDataSize;					//本结构以及后续所有数据的字节大小
	BYTE buType;					//内容类型，由CMD_TYPE描述
	BYTE buAck;						//1表示命令的答复，0表示原始命令
	BYTE buCheckInterval;			//命令服务器设定的监控查询间隔，单位秒
	int  iNum;						//后续TASK_LIST或者DEVICE_STATUS结构的数量	
	union
	{
		char szTaskID[33];			//针对CMD_QUERY_TASK_RESULT查询命令有效
		char szDeviceID[9];			//针对CMD_QUERY_TASK查询命令有效
	};

}	CMD_HEAD, *PCMD_HEAD;

#define MAX_PDP_NUM			32 
typedef struct
{
	char szStatus[924];					//设定的显示信息的内容或者返回的状态字符串
										//对于CHECK_STATUS命令，返回内容见下面的描述
	char szError[194];					//返回的错误描述
										//对于CHECK_STATUS命令，当硬盘空间告警时，应给出返回的告警信息
	_int8 iStatus;						//设备状态
	_int8 iAlarmLevel;					//故障等级
	_int8 bEmergent;					//是否处于紧急状态
										//硬盘状态
	_int8 bHDProperty;					//是否具备硬盘空间属性
	_int8 iHDStatus;					//硬盘检测结果：CMD_RESULT_OK或者CMD_RESULT_ERROR
	char szHDStatus[256];				//硬盘逻辑驱动器的空间描述
	ULONGLONG uHDSumSpace;				//放置素材的硬盘总空间，单位为字节，若为0，表示设备不支持硬盘空间属性
	ULONGLONG uHDFreeSpace;				//放置素材的硬盘剩余空间，单位为字节
										//对应的受控屏幕状态
	_int8 bScreenProperty;				//是否具备受控屏幕属性
	char szPDPName[32];					//受控屏幕名称，例如电源控制器，PDP屏
	_int8 iScreenDllOk;					//屏幕控制DLL的加载状态，由DLL_LOAD_STATUS描述
	_int8 iPDPNum;						//受控屏幕数量
	_int8 iPDPStatus[MAX_PDP_NUM];		//由SCREEN_STATUS枚举结构描述

}CHECK_STATUS_PARAM, *PCHECK_STATUS_PARAM;


typedef struct
{
	int  iRightLevel;					//权限级别码，数字越大级别越高

	int  iEsType;						//紧急状态类型，由enum NET_ES_TYPE结构描述
	char szContent[1022];				//表示需要设置的紧急状态内容或者紧急版式名称，以NULL结束。
	char iSoundType;					//发布紧急信息时声音处理类型，0表示不处理，1表示关闭声音，2表示设置音量，由枚举结构NET_ES_SOUND_TYPE描述
	BYTE buLeftVolume;					//发布紧急信息时调整的左声道音量
	BYTE buRightVolume;					//发布紧急信息时调整的右声道音量
	BOOL bFullScreen;					//如果是设置紧急内容，表示是否使用全屏显示。
										//若为true，则使用全屏实现紧急内容显示，否则只使用视频区域
	WORD wTimeLength;					//紧急状态持续时间，若为0，表示一直处于紧急状态，直到手动停止
	WORD wReplaceInfo;					//紧急信息的播出模式，1表示替换之前的，0表示追加到最后
}	NET_ES_PARAM, *PNET_ES_PARAM;

typedef struct
{
	int  iModuleNum;					//模块个数
	char szPlayContent[1280];			//播出内容描述字符串

}	PLAY_CONTENT, *PPLAY_CONTENT;

enum CMD_TYPE
{
	CMD_PUBLISH_TASK = 0,			//网管发布操作任务
	CMD_QUERY_TASK_RESULT,			//网管查询操作任务执行结果
	CMD_QUERY_STATUS,				//网管查询设备状态
	CMD_QUERY_SINGLE_STATUS,		//网管查询单个设备状态

	CMD_QUERY_TASK,					//设备查询自己的操作任务
	CMD_FEEDBACK_TASK_RESULT,		//设备反馈操作任务的执行结果
	CMD_PUBLISH_STATUS,				//设备发布自己的状态
};


typedef struct
{
	char szDeviceCode[9];
	__int8	iCheckResult;			//设备状态值
	__int8	iAlarmLevel;			//报警等级，0到4，0表示无故障
	__int16 iReserve;				//预留
	union
	{
		DATE dtUpdateTime;			//状态数据的更新时间，设备上报状态给CmdService时可填写本机时间，但CmdService接收到后会更新为自身本机时间，
		int  iTimeOffset;			//网管来查询设备状态时，CmdService将查询时间减去更新时间，获得时间差iTimeOffset，代表状态数据的有效程度，网管应只判断iTimeOffset
	};
	CHECK_STATUS_PARAM csParam;		//状态检测参数结构
	NET_ES_PARAM emParam;			//紧急状态控制参数结构
	PLAY_CONTENT PlayContent;		//播出内容结构

}	DEVICE_STATUS, *PDEVICE_STATUS;

#pragma pack()
