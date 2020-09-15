#pragma once

#define FLAG_SIZE				4
#define CMD_FLAG				"Cmd"
#define MAX_PDP_NUM			32 

#pragma pack(1)

enum CMD_TYPE
{
	CMD_PUBLISH_TASK = 0,			//网管发布操作任务
	CMD_QUERY_TASK_RESULT,			//网管查询操作任务执行结果
	CMD_QUERY_STATUS,				//网管查询设备状态
	CMD_QUERY_SINGLE_STATUS,		//网管查询单个设备状态

	CMD_QUERY_TASK,					//设备查询自己的操作任务
	CMD_FEEDBACK_TASK_RESULT,		//设备反馈操作任务的执行结果
	CMD_PUBLISH_STATUS,				//设备发布自己的状态

	//下面4个指令只针对分线PIS接口程序有效
	CMD_SET_CONFIG,					//系统管理软件设置分线配置，预留
	CMD_QUERY_SETCONFIG_RESULT,		//系统管理软件查询设置任务执行结果，预留
	CMD_GET_CONFIG,					//系统管理软件获取分线设备信息，预留
	CMD_QUERY_GETCONFIG_RESULT,		//系统管理软件查询获取分线设备执行结果，预留

	CMD_CANCEL_TASK = 50,			//取消操作任务

	CMD_TEST = 80,					//测试服务是否正常的指令
};

enum DLL_LOAD_STATUS
{
	DL_STATUS_OK = 0,			//屏幕控制DLL加载正常
	DL_STATUS_SET_NONE,			//未设置屏幕控制DLL
	DL_STATUS_LOAD_FAIL,		//屏幕控制DLL加载失败
	DL_STATUS_INIT_FAIL,		//屏幕控制DLL初始化失败
	MAX_DL_STATUS
};

enum SCREEN_STATUS
{
	SCREEN_STATUS_OFF = 0,				//关闭状态
	SCREEN_STATUS_ON,					//开启状态
	SCREEN_STATUS_ERROR,				//故障状态
	SCREEN_STATUS_UNKNOWN,				//未知状态
	MAX_SCREEN_STATUS
};

enum MACRO_TYPE_INDEX
{
	MT_POWER_ON = 0,	//设备开机命令	网管软件提供该功能
	MT_POWER_OFF,		//设备关机命令	网管软件提供该功能
	MT_RESET_DEVICE,	//重新启动命令	网管软件提供该功能
	MT_SCREEN_ON,	//设备显示屏开机命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SCREEN_OFF,	//设备显示屏关机命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_PUBLISH_INFO,	//发布紧急信息命令，即实施紧急状态	紧急管理软件提供该功能
	MT_STOP_PUBLISH,	//取消紧急信息命令，即结束紧急状态	紧急管理软件提供该功能
	MT_START_RUNNING,	//开始运行命令	网管软件提供该功能
	MT_STOP_RUNNING,	//停止运行命令	网管软件提供该功能
	MT_TAKE,			//TAKE命令	网管软件提供该功能
	MT_SOUND_ON,		//开启声音命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SOUND_OFF,		//关闭声音命令	网管软件提供该功能，紧急管理软件提供该功能
	MT_SET_VOLUME,		//设置音量（0～255）	网管软件提供该功能
	MT_LIVE_ON,			//开始直播	网管软件提供该功能
	MT_LIVE_OFF,		//停止直播	网管软件提供该功能
	MAX_MACRO_TYPE
};

enum NET_ES_TYPE
{
	NET_ES_CONTENT = 0,		//采用紧急内容，通过紧急模块实现，szContent维护紧急内容
	NET_ES_LAYOUT,			//采用紧急版式，szContent维护版式文件名
	MAX_NET_ES_TYPE
};

enum NET_ES_SOUND_TYPE
{
	NET_ES_SOUND_NONE = 0,//不处理
	NET_ES_SOUND_OFF,//关闭声音
	NET_ES_SOUND_SET_VOLUME,//设置音量
	MAX_NET_ES_SOUND_TYPE
};

enum CMD_RESULT
{
	CMD_RESULT_WAIT_EXEC = -100,	//等待处理

	CMD_RESULT_DB_ERROR = -90,		//数据库访问错误

	CMD_RESULT_LOCAL_ERROR = -10,	//本地错误，例如参数非法
	CMD_RESULT_CONN_ERROR,			//网络故障
	CMD_RESULT_SERVER_ERROR,		//网络服务未启动，或者服务程序已经死机
	CMD_RESULT_POWER_OFF,			//设备已关机
	CMD_RESULT_RESET,				//设备在重启
	CMD_RESULT_PLAY_DOWN,			//播出已停止

	CMD_RESULT_ERROR = 0,				//执行失败，对于状态检测，故障原因由szError描述
	CMD_RESULT_OK,						//执行成功

	//错误命令序号
	CMD_RESULT_NO_CMD,					//没有指定的命令
	CMD_RESULT_NO_LOGON,				//没有登录，不使用

	//登录错误序号
	CMD_RESULT_LOGON_FAIL,				//错误的用户名或密码，不使用
	CMD_RESULT_NO_RIGHT,				//没有权限，不使用

	//紧急状态或者播表、版式控制错误序号
	CMD_RESULT_LOW_RIGHT,	//权限低，不能进行紧急状态的控制，或者是当前处于紧急状态，实施其他控制但权限较低
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
	CMD_RESULT_NO_LIVE_MODULE,			//没有直播模块

	//控制器故障类型
	CMD_RESULT_BOARD_ERROR = 50,		//板卡故障
	CMD_RESULT_CL_ERROR,				//接口监听故障
	CMD_RESULT_WDT_ERROR,				//wdt故障
	CMD_RESULT_SCREEN_ERROR,			//电源控制器故障
	CMD_RESULT_UPS_ERROR,				//UPS模块故障
	CMD_RESULT_HD_ERROR,				//硬盘故障
	CMD_RESULT_LOW_SPACE,				//硬盘空间不足
	CMD_RESULT_CPU_ERROR,				//CPU温度过高
	CMD_RESULT_CPU_OVERLOAD,			//CPU占用率过载
};


typedef struct __NET_ES_PARAM
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

	__NET_ES_PARAM()
	{
		strcpy(szContent, "");
		iSoundType = 0;
	}

}	NET_ES_PARAM, *PNET_ES_PARAM;

typedef struct __PLAY_CONTENT
{
	int  iModuleNum;					//模块个数
	char szPlayContent[1280];			//播出内容描述字符串

	__PLAY_CONTENT()
	{
		iModuleNum = 0;
		strcpy(szPlayContent, "");
	}

}	PLAY_CONTENT, *PPLAY_CONTENT;

typedef struct
{
	char	szDeviceCode[9];//操作任务发布的对象的设备编号
	__int16 iExecResult;//客户端软件发布命令前，必须将该成员设定为CMD_RESULT_WAIT_EXEC，表示等待处理；客户端软件获取操作任务的执行结果时，该成员表示对应设备的执行结果，由枚举结构CMD_RESULT描述
	DATE    dtExecTime;//客户端软件获取操作任务的执行结果时，该成员表示对应设备的执行时间。

}	TASK_LIST, *PTASK_LIST;


typedef struct
{
	TASK_LIST TaskList;
	char	szTaskID[33];

}	TASK_RESULT, *PTASK_RESULT;

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

typedef struct
{
	char	szTaskID[33];//操作任务的ID编号，客户端软件需要自动生成，并确保全局唯一性
	__int8	iTaskType;//操作任务的类型，由枚举结构MACRO_TYPE_INDEX描述，但不支持其中的MT_POWER_ON命令
	__int8	iEmType;//操作任务为发布紧急信息时的方式（紧急文本、紧急版式），由枚举结构NET_ES_TYPE描述
	__int8	iFullScreen;//操作任务为发布紧急信息时的显示方式，1表示全屏显示，0表示区域显示
	__int8	iReplaceInfo;//操作任务为发布紧急信息时的播出方式，1表示覆盖，0表示跟随；
	__int8	iSoundType;//操作任务为发布紧急信息时的声音处理方式（不处理、关闭声音、设置音量），由枚举结构NET_ES_SOUND_TYPE描述
	BYTE	buLeftVolume;//操作任务为设置音量或者发布紧急信息（同时声音处理方式为调整音量）时的左声道音量，范围0～255
	BYTE	buRightVolume;//操作任务为设置音量或者发布紧急信息（同时声音处理方式为调整音量）时的右声道音量，范围0～255
	char	szEmergencyInfo[1022];//操作任务为发布紧急信息时的紧急文本或者紧急版式名称；
	DATE	dtTaskTime;//操作任务的发布时间，CmdService接收后会以自身本机时间重新设定该成员；
	int 	iValidTime;				//有效时长，单位秒
	__int16 iTimeLength;//操作任务为发布紧急信息时，紧急状态的持续时间，单位秒，如果为0，表示紧急状态持续到停止命令
	char    szEditorID[17];//操作任务发布者的ID
	BYTE	buRightLevel;//操作任务发布者的权限级别，0～255，数字越大，级别越高

	//设备列表
	CMapStringToPtr * pMapDevice;

}	TASK_SET, *PTASK_SET;

typedef struct __CHECK_STATUS_PARAM
{
	char szStatus[924];					//设定的显示信息的内容或者返回的状态字符串
	char szError[194];					//返回的错误描述
	_int8 iStatus;						//设备状态
	_int8 iAlarmLevel;					//故障等级
	_int8 bEmergent;					//是否处于紧急状态
										//硬盘状态
	_int8 bHDProperty;				//是否具备硬盘空间属性
	_int8 iHDStatus;				//硬盘检测结果：CMD_RESULT_OK或者CMD_RESULT_ERROR
	char szHDStatus[256];				//硬盘逻辑驱动器的空间描述
	ULONGLONG uHDSumSpace;				//放置素材的硬盘总空间，单位为节
	ULONGLONG uHDFreeSpace;				//放置素材的硬盘剩余空间，单位为字节
										//对应的受控屏幕状态
	_int8 bScreenProperty;				//是否具备受控屏幕属性
	char szPDPName[32];					//受控屏幕名称，例如电源控制器，PDP屏
	_int8 iScreenDllOk;					//屏幕控制DLL的加载状态，由DLL_LOAD_STATUS描述
	_int8 iPDPNum;						//受控屏幕数量
	_int8 iPDPStatus[MAX_PDP_NUM];		//由SCREEN_STATUS枚举结构描述

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
	__int8	iReserve1;				//预留
	__int8	iReserve2;				//预留
	__int16 iReserve;				//预留
	union
	{
		DATE dtUpdateTime;
		int  iTimeOffset;
	};
	CHECK_STATUS_PARAM csParam;		//状态检测参数结构
	NET_ES_PARAM emParam;			//紧急状态控制参数结构
	PLAY_CONTENT PlayContent;		//播出内容结构

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

