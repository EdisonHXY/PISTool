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
	void CopyInfo(CStationInfo & src);			//������վ��Ϣ
	void ReadInfo(_RecordsetPtr pRs);	//��ȡ��վ��Ϣ

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

	CStringArray m_arrDeviceCode; //�豸�б�

	//��վ�б�
	CTypedPtrArray<CPtrArray, CStationInfo *> m_ptrStationList;
	//�г��б�
	CTypedPtrArray<CPtrArray, CStationInfo *> m_ptrTrainList;

	// Operations
public:
	void ResetData();
	void ReadInfo(_RecordsetPtr pRs);	//��ȡ��·��Ϣ

	CString GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize = 0);
	
};


//��Ŀ����Ϣ
typedef struct LIST_INFO_tag
{
	//����
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
	char szEditorID[50];		//���û�ID���û���������"888888 lhl"
	char szEditTime[20];
	char szScopeEditTime[20];
	int  iAudit;
	char szImportLine[5];

	char szMd5[80];

	//Ӧ�÷�Χ
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

//��Ŀ��״̬
enum LIST_STATUS_INDEX
{
	LIST_STATUS_INVALID = 0,
	LIST_STATUS_VALID,
	LIST_STATUS_DELETED,
	LIST_STATUS_NO,
	MAX_LIST_STATUS
};

//����״̬
enum
{
	DL_NOT = 0,
	DL_DOING,
	DL_FINISHED,
	DL_ERROR,
	DL_MAX
};

//���ݿ�����
enum DB_TYPE_
{
	DB_SQLSERVER = 0,
	DB_ORACLE,
	DB_MYSQL,
};

//�豸����״̬����Ϣ
typedef struct 
{
	CString strCmd, strCode, strName, finishName;
	int iStatus;
}Device_DownLoadInfo;

typedef struct 
{
	CString stationCode; //վ����
	CString startTimeStr; //��ʼʱ��
	CString stopTimeStr;  //ֹͣʱ��

}Station_RunTime;


typedef struct
{
	CString stationCode; //վ����
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


	//�������ݿ��ַ
	virtual void SetDBIp(CString ipAddress);
	virtual CString GetDBIP();

	//��ȡ���н�Ŀ������
	 int GetAllVideoListData(vector<LIST_INFO>  &list, int listType,int dayInterval);

	//���ݽ�Ŀ��szListID��Ϣ ��ȡ�豸���ص�״̬
	 int GetDeviceDownLoadStatus(char *listID,vector<Device_DownLoadInfo> &listDeviceDown, int listType);
	
	//����Ŀ�������Ƿ����ظ�  -1 : ����, 0 : û�м�¼, 1: ������ͬ�ļ�¼
	 virtual int IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType);

	//���½�Ŀ��/���� ���ݿ��ֶζ�һ��
	 BOOL UpdatePlayListRecord(LIST_INFO_tag &listInfo, CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate,bool bIsVideoList);

	//��ȡ������·
	 void GetLineList(CTypedPtrArray<CPtrArray, CLineInfo *> &lineList, CStringArray &strArrayPos);

	//���½�Ŀ�� ��Ӧ������
	 BOOL UpdatePlayListRecordZone(_ConnectionPtr pConn, LIST_INFO_tag &listInfo, int iUpdate, int listType);

	//����Ĭ�ϲ���
	 BOOL UpdateDefaultPlayListRecord(LIST_INFO_tag &listInfo,CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate);

	 //��������״̬
	 BOOL SetDownloadStatus(CString listIDStr, CString DeviceCodeStr, int iDlStatus, CString strTime, int listType);

	 //��ȡ��Ӫʱ��
	 bool GetStationTimeInfo(vector<Station_RunTime> &staionTimeInfo);

	 //��ȡ�г���Ӫʱ�� TrainInfo
	 bool GetTrainInfoTime(vector<TainInfo_RunTime> &trainTimeInfoList, CString strLine);
	 //�����г�����Ӫʱ�� TrainInfo
	 bool SetTrainInfoTime(TainInfo_RunTime &trainTimeInfo, CString strLine);

public:
	//�������ݱ�
	_ConnectionPtr GetConnectPtr(LPCTSTR szIP);

	//��ȡ��Ŀ��
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
	int  m_iDBType;   //���ݿ�����
	CString m_strUserName, m_strPwd;				//���ݿ��¼�û���������
	CString m_dbIP;  //���ݿ��ַ
	UINT m_connectTimeOut; //���ݿ����ӳ�ʱʱ��
};


