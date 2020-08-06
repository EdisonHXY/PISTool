#pragma once

#include "DataBaseManager.h"

//��Ŀ����Ϣ
typedef struct LIST_INFO_Old_tag
{
	//����
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
	char szEditorID[50];		//���û�ID���û���������"888888 lhl"
	char szEditTime[20];
	char szScopeEditTime[20];
	int  iAudit_Num;
	int  iAudit_Level;
	char szImportLine[5];

	//char szMd5[80];

	//Ӧ�÷�Χ
	char szApplyScopLine[5];
	vector<int> selectedGroupList;
	

	LIST_INFO_Old_tag()
	{
		selectedGroupList.clear();
	}


}	LIST_INFO_Old, *PLIST_INFO_Old;

//���ص�״̬
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

	//���½�Ŀ��/���� ���ݿ��ֶζ�һ��
	BOOL UpdatePlayListRecord(LIST_INFO_Old_tag &listInfo, int iUpdate, bool isDefault);
	BOOL UpdatePlayListRecordZone(_ConnectionPtr pConn, LIST_INFO_Old_tag &listInfo, int iUpdate, bool isDefault);

	virtual int IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType);

	int isExistStation(_ConnectionPtr pConn, CString listID, int deviceGroup, CString lineCode);

	//��ȡ���ص�״̬
	int GetListDownloadStatus(vector<DownLoadStatusList_Old> &statusList);

};

