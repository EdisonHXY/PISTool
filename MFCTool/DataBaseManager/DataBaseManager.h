#pragma once
#pragma warning(disable : 4146)
#import "c:\program files\common files\system\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#include <vector>
using namespace std;


class CStationInfo
{
public:
	CStationInfo();
	~CStationInfo();

	// Attributes
public:
	char szStationCode[6];
	char szStationName[41];
	char szLineCode[5];
	BOOL bIsTrain;

	// Operations
public:
	void ResetData();
	void CopyInfo(CStationInfo & src);			//拷贝车站信息
	void ReadInfo(_RecordsetPtr pRs);	//读取车站信息

	int GetIntField(_RecordsetPtr pRs, _variant_t vtField);

	CString GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize = 0);
	DATE GetDateField(_RecordsetPtr pRs, _variant_t vtField);

};

class CLineInfo
{
public:
	CLineInfo();
	~CLineInfo();

	// Attributes
public:
	char szLineCode[5];
	char szLineName[41];

	CStringArray m_arrDeviceCode; //设备列表

	//车站列表
	CTypedPtrArray<CPtrArray, CStationInfo *> m_ptrStationList;
	//列车列表
	CTypedPtrArray<CPtrArray, CStationInfo *> m_ptrTrainList;

	// Operations
public:
	void ResetData();
	void ReadInfo(_RecordsetPtr pRs);	//读取线路信息

	CString GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize = 0);
	
};


//节目单信息
typedef struct LIST_INFO_tag
{
	//属性
	char szListID[11];
	char szListName[65];
	char szListTitle[32];
	int  iListStatus;
	int  iPlayFlag;
	char szUpdateTime[20];
	char szDeleteDate[20];
	char szStartDate[20];
	char szEndDate[20];
	int  iWeek;
	char szEditorID[50];		//含用户ID和用户名，类似"888888 lhl"
	char szEditTime[20];
	char szScopeEditTime[20];
	int  iAudit;
	char szImportLine[5];

	char szMd5[80];

	//应用范围
	CStringArray * pArrayPos;
	CStringArray * pArrayStation;

	CTypedPtrArray<CPtrArray, CStationInfo *> *ptrSelectedStation;
	CStringArray *strArraySelectedPos;

	LIST_INFO_tag()
	{
		pArrayStation = NULL;
		pArrayPos = NULL;
		ptrSelectedStation = NULL;
		strArraySelectedPos = NULL;
	}


}	LIST_INFO, *PLIST_INFO;

//节目单状态
enum LIST_STATUS_INDEX
{
	LIST_STATUS_INVALID = 0,
	LIST_STATUS_VALID,
	LIST_STATUS_DELETED,
	LIST_STATUS_NO,
	MAX_LIST_STATUS
};

//下载状态
enum
{
	DL_NOT = 0,
	DL_DOING,
	DL_FINISHED,
	DL_ERROR,
	DL_MAX
};

//数据库类型
enum DB_TYPE_
{
	DB_SQLSERVER = 0,
	DB_ORACLE,
	DB_MYSQL,
};

//设备下载状态的信息
typedef struct 
{
	CString strCmd, strCode, strName, finishName;
	int iStatus;
}Device_DownLoadInfo;

typedef struct 
{
	CString stationCode; //站点编号
	CString startTimeStr; //开始时间
	CString stopTimeStr;  //停止时间

}Station_RunTime;


typedef struct
{
	CString stationCode; //站点编号
	CString treminateStaion1;
	CString firstTrain1;
	CString lastTrain1;

	CString treminateStaion2;
	CString firstTrain2;
	CString lastTrain2;

	CString lineStr;

}TainInfo_RunTime;

class CDataBaseManager
{
public:
	CDataBaseManager();
	virtual ~CDataBaseManager();


	//设置数据库地址
	virtual void SetDBIp(CString ipAddress);
	virtual CString GetDBIP();

	//获取所有节目单数据
	 int GetAllVideoListData(vector<LIST_INFO>  &list, int listType,int dayInterval);

	//根据节目单szListID信息 获取设备下载的状态
	 int GetDeviceDownLoadStatus(char *listID,vector<Device_DownLoadInfo> &listDeviceDown, int listType);
	
	//检测节目单名称是否有重复  -1 : 错误, 0 : 没有记录, 1: 存在相同的记录
	 virtual int IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType);

	//更新节目单/播表 数据库字段都一样
	 BOOL UpdatePlayListRecord(LIST_INFO_tag &listInfo, CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate,bool bIsVideoList);

	//获取所有线路
	 void GetLineList(CTypedPtrArray<CPtrArray, CLineInfo *> &lineList, CStringArray &strArrayPos);

	//更新节目单 的应用区域
	 BOOL UpdatePlayListRecordZone(_ConnectionPtr pConn, LIST_INFO_tag &listInfo, int iUpdate, int listType);

	//更新默认播表
	 BOOL UpdateDefaultPlayListRecord(LIST_INFO_tag &listInfo,CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate);

	 //设置下载状态
	 BOOL SetDownloadStatus(CString listIDStr, CString DeviceCodeStr, int iDlStatus, CString strTime, int listType);

	 //获取运营时间
	 bool GetStationTimeInfo(vector<Station_RunTime> &staionTimeInfo);

	 //获取列车运营时间 TrainInfo
	 bool GetTrainInfoTime(vector<TainInfo_RunTime> &trainTimeInfoList, CString strLine);
	 //设置列车的运营时间 TrainInfo
	 bool SetTrainInfoTime(TainInfo_RunTime &trainTimeInfo, CString strLine);

public:
	//连接数据表
	_ConnectionPtr GetConnectPtr(LPCTSTR szIP);

	//获取节目单
	int Search(CString strCmd, vector<LIST_INFO> &list, int listType);
	void ReadApplyScope(_ConnectionPtr pConn, PLIST_INFO pListInfo, int listType);
	void GetListInfo(_RecordsetPtr pRs, PLIST_INFO pListInfo);
	CString GetMaxIDCmd(LPCTSTR szTable, CString strServerLineCode, LPCTSTR szField);
	CString IncreaseCode(CString & strCode, char cFlag);
	BOOL GetPlayListID(_ConnectionPtr pConn, CString strServerLineCode, CString & strListID, int listType);
	BOOL GetDefaultPlayListID(_ConnectionPtr pConn, CString strServerLineCode, CString & strListID, int listType);

	int IsExistTitleEx(_ConnectionPtr pConn, CString strTitle, int listType);
	
	int isExistStation(_ConnectionPtr pConn, CString listID,CString devicePos,CString stationCode,CString lineCode, int listType);

	int GetIntField(_RecordsetPtr pRs, _variant_t vtField);

	CString GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize = 0);
	DATE GetDateField(_RecordsetPtr pRs, _variant_t vtField);
	
public:
	int  m_iDBType;   //数据库类型
	CString m_strUserName, m_strPwd;				//数据库登录用户名和密码
	CString m_dbIP;  //数据库地址
	UINT m_connectTimeOut; //数据库连接超时时间
};


