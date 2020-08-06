#pragma once

#include "DataBaseManager.h"

//节目单信息
typedef struct LIST_INFO_Old_tag
{
	//属性
	char szListID[11];
	char szListName[65];
	char szListTitle[32];
	int  iListStatus;
	//int  iPlayFlag;
	char szUpdateTime[20];
	char szDeleteDate[20];
	char szStartDate[20];
	char szEndDate[20];
	int  iWeek;
	char szEditorID[50];		//含用户ID和用户名，类似"888888 lhl"
	char szEditTime[20];
	char szScopeEditTime[20];
	int  iAudit_Num;
	int  iAudit_Level;
	char szImportLine[5];

	//char szMd5[80];

	//应用范围
	char szApplyScopLine[5];
	vector<int> selectedGroupList;
	

	LIST_INFO_Old_tag()
	{
		selectedGroupList.clear();
	}


}	LIST_INFO_Old, *PLIST_INFO_Old;

//下载的状态
typedef struct DownLoadListOld_Tag
{
	CString nameStr;
	CString device_CodeStr;
	CString finishTimeStr;
	int iStatus;

} DownLoadStatusList_Old;

class CDataBaseOldManager : public CDataBaseManager
{
public:
	CDataBaseOldManager();
	~CDataBaseOldManager();

	//更新节目单/播表 数据库字段都一样
	BOOL UpdatePlayListRecord(LIST_INFO_Old_tag &listInfo, int iUpdate, bool isDefault);
	BOOL UpdatePlayListRecordZone(_ConnectionPtr pConn, LIST_INFO_Old_tag &listInfo, int iUpdate, bool isDefault);

	virtual int IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType);

	int isExistStation(_ConnectionPtr pConn, CString listID, int deviceGroup, CString lineCode);

	//获取下载的状态
	int GetListDownloadStatus(vector<DownLoadStatusList_Old> &statusList);

};

