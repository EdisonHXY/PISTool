/***********************************************************************************
*	版权所有(c)2014 冠华天视软件部
*	文件名称:	CtrlLib.h
*	文件说明:	设备控制库管理类头文件
*	当前版本:	1.0.20140327
*	作者:		贾巍云
*	创建日期:	2014-03-27
*************************************************************************************/

#ifndef	_CTRLLIB_H
#define	_CTRLLIB_H

#include <afxtempl.h>


#define MAX_PDP_NUM			32

enum CMD_RESULT
{	
	CMD_RESULT_LOCAL_ERROR = -10,		//本地错误，例如参数非法
	CMD_RESULT_CONN_ERROR,				//网络故障
	CMD_RESULT_SERVER_ERROR,			//网络服务未启动，或者服务程序已经死机

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

enum NET_ES_TYPE
{
	NET_ET_CONTENT = 0,					//采用紧急内容，通过紧急模块实现，szContent维护紧急内容
	NET_ET_LAYOUT,						//采用紧急版式，szContent维护版式文件名
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
	SCREEN_STATUS_OFF = 0,		//关闭状态
	SCREEN_STATUS_ON,			//开启状态
	SCREEN_STATUS_ERROR,		//故障状态
	SCREEN_STATUS_UNKNOWN,		//未知状态
	MAX_SCREEN_STATUS
};

enum CTRL_CMD
{
	CTRL_CMD_SWITCH_DEVICE = 0,			//设备开关机控制命令，iParam为0表示关机，为1表示开机。
	CTRL_CMD_SWITCH_SCREEN,				//设备屏幕开关机控制命令，iParam为0表示关闭，为1表示开启。
	CTRL_CMD_END_CONTENT,				//结束紧急状态命令，iParam无效
	CTRL_CMD_SWITCH_RUNNING,			//版式运行及停止控制命令，iParam为0表示停止播放，为1表示开始播放
	CTRL_CMD_TAKE,						//TAKE命令，iParam无效
	CTRL_CMD_RESET_DEVICE,				//复位命令，iParam无效
	CTRL_CMD_SWITCH_SOUND,				//声音开启关闭命令，iParam为0表示关闭，为1表示开启
	CTRL_CMD_SET_VOLUME,				//设置音量大小命令，iParam为音量大小，0～255
	CTRL_CMD_SWITCH_LIVE,				//直播控制命令，iParam为0表示结束直播，为1表示开始直播
	CTRL_CMD_WAKEUP_DEVICE = 100,		//唤醒其他设备，要唤醒的设备IP及MAC通过ctrl_ExecuteCmd函数的szError传递进来（以NULL分隔）
	CTRL_CMD_HD_SPACE = 200,			//是否支持硬盘空间属性
};


typedef struct
{
	char	szDeviceCode[9];
	__int16 iExecResult;
	DATE    dtExecTime;

}	TASK_LIST, *PTASK_LIST;

#pragma pack(1)

//const	int		CTRLTYPE_NONE		= 0;	//未知
//const	int		CTRLTYPE_OK			= 1;	//支持
//const	int		CTRLTYPE_NO			= 2;	//不支持


//typedef struct
//{	
//	int  iRightLevel;					//权限级别码，数字越大级别越高
//
//	int  iEsType;						//紧急状态类型，由enum NET_ES_TYPE结构描述
//	char szContent[1025];				//表示需要设置的紧急状态内容或者紧急版式名称，以NULL结束。
//	BOOL bFullScreen;					//如果是设置紧急内容，表示是否使用全屏显示。
//										//若为true，则使用全屏实现紧急内容显示，否则只使用视频区域
//	UINT uTimeLength;					//紧急状态持续时间，若为0，表示一直处于紧急状态，直到手动停止
//
//}	NET_ES_PARAM, * PNET_ES_PARAM;



typedef struct
{	
	int  iRightLevel;					//权限级别码，数字越大级别越高

	int  iEsType;						//紧急状态类型，由enum NET_ES_TYPE结构描述
	char szContent[1022];				//表示需要设置的紧急状态内容或者紧急版式名称，以NULL结束。
	char iSoundType;					//发布紧急信息时声音处理类型，0表示不处理，1表示关闭声音，2表示设置音量
	BYTE buLeftVolume;					//发布紧急信息时调整的左声道音量
	BYTE buRightVolume;					//发布紧急信息时调整的右声道音量
	BOOL bFullScreen;					//如果是设置紧急内容，表示是否使用全屏显示。
										//若为true，则使用全屏实现紧急内容显示，否则只使用视频区域
	WORD wTimeLength;					//紧急状态持续时间，若为0，表示一直处于紧急状态，直到手动停止
	WORD wReplaceInfo;					//紧急信息的播出模式，1表示替换之前的，0表示追加到最后
//	UINT uTimeLength;					//紧急状态持续时间，若为0，表示一直处于紧急状态，直到手动停止

}	NET_ES_PARAM, * PNET_ES_PARAM;


typedef struct
{
	char szStatus[924];					//设定的显示信息的内容或者返回的状态字符串
										//对于CHECK_STATUS命令，返回内容见下面的描述
	char szError[196];					//返回的错误描述
										//对于CHECK_STATUS命令，当硬盘空间告警时，应给出返回的告警信息
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

}	CHECK_STATUS_PARAM, * PCHECK_STATUS_PARAM;

typedef struct
{	
	int  iModuleNum;					//模块个数
	char szPlayContent[1280];			//播出内容描述字符串

}	PLAY_CONTENT, * PPLAY_CONTENT;


//文字对象属性参数，长度2字节
typedef struct tagSTRPARAM
{
	WORD chCmd;			//命令类型
	WORD uCmdParam;	    //命令参数

}STRPARAM, *PSTRPARAM;

//更新显示内容命令数据包头
typedef struct tagLEDFONTPARAM
{
	  DWORD command;	//数据包命令标识，更新节目命令(=0x55AA5501)，
	  DWORD dst_count;  //目标设备数量
	  //目标设备地址，用于标识显示屏，全线所有显示屏要进行统一编号
	  WORD  dst_addr[256];
	  DWORD param_count;  //参数个数
	  STRPARAM param[16];  //文字对象属性
} LEDFONTPARAM, *PLEDFONTPARAM;


#pragma pack()

typedef void *	HWINDIB;		//用于windows的DIB数据


//模块合法性函数
//主程序通过调用该函数，用于判断一个DLL是否是合法的模块。如果用户将无关的DLL放到模块的目录下，通过该函数就可以避免把该DLL错认为系统的模块。主程序只需要在进入程序加载模块时调用一次该函数即可。函数原型如下：
//返回模块的版本号，如果大于等于主程序的版本号，则表示DLL是合法的模块，否则不合法，不应该再进行其他的函数调用。
typedef UINT (* lpctrl_GetDllVersion)();									//get dll version

//模块初始化函数
//主程序通过调用该函数，使得各个模块可以初始化各自的参数，创建各个对象等等。主程序只需要在进入程序加载模块时调用一次该函数即可。函数原型如下：
//函数返回一个无类型指针，需要主程序加以维护。如果返回NULL，表示初始化不成功。
//szComOrIP表示要控制的设备的IP地址，szMac为其物理地址，iRightLevel为操作者和用户的权限
typedef void * (* lpctrl_InitDll)(LPCTSTR szComOrIP, LPCTSTR szMac, int iRightLevel);		//initial dll, such as creating network conection, open com.

//模块非初始化函数
//主程序通过调用该函数，使得各个模块可以释放各自的内存等等。主程序只需要在退出程序释放模块时调用一次该函数即可。函数原型如下：
//如果返回true，表示非初始化成功。
typedef BOOL (* lpctrl_UninitDll)(void * lpVoid);							//Uninitial dll, such as stop running

//显示参数设置函数
//主程序通过调用该函数，调出各个模块的设置对话框，调整各种参数。当用户需要设置一个模块的参数时，主程序都需要调用该模块的参数设置函数。函数原型如下：
//如果返回IDOK，表示模块的参数进行了改动。
typedef int  (* lpctrl_ShowConfigDlg)();									//show config dialog

//获取模块名称函数
//主程序通过调用该函数，获取模块的名称，使得用户能够识别模块。函数原型如下：
//该函数返回模块名称的实际长度，单位字节。
typedef int  (* lpctrl_GetName)(char * szName, int iSize);					//get dll module name


//获取当前紧急状态内容
//pES用于维护返回的紧急状态数据。返回结果用枚举类型CMD_RESULT描述
typedef int  (* lpctrl_GetContent)(void* lpVoid, PNET_ES_PARAM pES);

//设置紧急状态
//pES表示要设置的紧急状态数据。返回结果用枚举类型CMD_RESULT描述
typedef int  (* lpctrl_SetContent)(void* lpVoid, PNET_ES_PARAM pES);		//set content

//状态检测函数
//主程序通过调用该函数，检测受监控设备的状态。函数原型如下：
//返回值为int类型，采用枚举类型描述，pParam维护返回的状态描述
//参数szStatus采用"参数1\t值\n参数2\t值\n"的编码方式，使得主程序可以采用ListCtrl控件以列表的方式进行状态显示。
typedef int  (* lpctrl_CheckStatus)(void * lpVoid, PCHECK_STATUS_PARAM pParam);		//The param is the value returned by InitDll function

//调度函数，返回结果用枚举类型CMD_RESULT描述，iCmd表示执行的命令类型，iParam为命令参数，
//szError维护返回的错误描述，iLen表示缓冲区的长度，这两个参数只针对CTRL_CMD_SWITCH_SCREEN命令有效
typedef int  (* lpctrl_ExecuteCmd)(void * lpVoid, int iCmd, int iParam, PCHAR szError, int iLen);
//判断是否存在指定命令
typedef BOOL (* lpctrl_SupportCmd)(int iCmd);
//获取当前播出内容描述
//pParam用于维护返回的内容数据。返回结果用枚举类型CMD_RESULT描述
typedef int(*lpctrl_GetPlayContent)(void* lpVoid, PPLAY_CONTENT pParam);	// 获得播放模块的内容

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