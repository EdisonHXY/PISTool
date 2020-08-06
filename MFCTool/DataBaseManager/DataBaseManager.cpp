#include "stdafx.h"
#include "DataBaseManager.h"


CDataBaseManager::CDataBaseManager()
{
	m_strUserName = "sa";
	m_strPwd = "ghyt2001";
	m_iDBType = DB_SQLSERVER;
	m_connectTimeOut = 5;
	
}


CDataBaseManager::~CDataBaseManager()
{
}

int CDataBaseManager::GetIntField(_RecordsetPtr pRs, _variant_t vtField)
{
	_variant_t vtValue;
	int iValue = 0;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		if (vtValue.vt == VT_I1)
		{
			iValue = (int)vtValue.cVal;
		}
		else if (vtValue.vt == VT_UI1)
		{
			iValue = (int)vtValue.bVal;
		}
		else if (vtValue.vt == VT_I2)
		{
			iValue = (int)vtValue.iVal;
		}
		else if (vtValue.vt == VT_UI2)
		{
			iValue = (int)vtValue.uiVal;
		}
		else if (vtValue.vt == VT_I4)
		{
			iValue = (int)vtValue.lVal;
		}
		else if (vtValue.vt == VT_UI4)
		{
			iValue = (int)vtValue.ulVal;
		}
		else if (vtValue.vt == VT_INT)
		{
			iValue = (int)vtValue.intVal;
		}
		else if (vtValue.vt == VT_UINT)
		{
			iValue = (int)vtValue.uintVal;
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), (long *)&iValue);
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}
	return iValue;
}

CString CDataBaseManager::GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize)
{

	long lValue;
	_variant_t vtValue;
	CString strValue;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		strValue.Empty();
		if (vtValue.vt == VT_I1)
		{
			strValue.Format("%d", vtValue.cVal);
		}
		else if (vtValue.vt == VT_UI1)
		{
			strValue.Format("%d", vtValue.bVal);
		}
		else if (vtValue.vt == VT_I2)
		{
			strValue.Format("%d", vtValue.iVal);
		}
		else if (vtValue.vt == VT_UI2)
		{
			strValue.Format("%d", vtValue.uiVal);
		}
		else if (vtValue.vt == VT_I4)
		{
			strValue.Format("%d", vtValue.lVal);
		}
		else if (vtValue.vt == VT_UI4)
		{
			strValue.Format("%d", vtValue.ulVal);
		}
		else if (vtValue.vt == VT_INT)
		{
			strValue.Format("%d", vtValue.intVal);
		}
		else if (vtValue.vt == VT_UINT)
		{
			strValue.Format("%d", vtValue.uintVal);
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), &lValue);
			strValue.Format("%d", lValue);
		}
		else if (vtValue.vt == VT_DATE)
		{
			COleDateTime tm = vtValue.date;
			strValue = (iReturnSize == 0) ? tm.Format("%Y-%m-%d") : tm.Format("%Y-%m-%d %H:%M:%S");
		}
		else if (vtValue.vt == VT_BSTR)
		{
			strValue = vtValue.bstrVal;
			strValue.TrimRight();
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}
	return strValue;
}

DATE CDataBaseManager::GetDateField(_RecordsetPtr pRs, _variant_t vtField)
{
	_variant_t vtValue;
	DATE dateValue = 0;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		if (vtValue.vt == VT_DATE)
		{
			dateValue = vtValue.date;
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}

	return dateValue;
}

BOOL CDataBaseManager::SetDownloadStatus(CString listIDStr, CString DeviceCodeStr, int iDlStatus, CString strTime,int listType)
{
	BOOL bRet = false;
	CString strConn, strCmd, strTable, strField, strListDBName;

	if (listType == 0)
	{
		strTable = "Play_List_Download_Status";
		strField = "List_ID";
		strListDBName = "Play_List";

	}
	else if (listType == 1)
	{
		strTable = "Video_List_Download_Status";
		strField = "List_ID";
		strListDBName = "Video_List";
	}
	else if (listType == 2)
	{
		strTable = "Default_List_Download_Status";
		strField = "List_ID";
		strListDBName = "Default_List";

	}
	else
	{
		return false;
	}

	_ConnectionPtr pConn;
	try
	{
		pConn = GetConnectPtr(m_dbIP);
		//begin trans
		pConn->BeginTrans();
		//delete 
		strCmd.Format("delete from PIS.dbo.%s where Device_Code = '%s' and %s = '%s'",
			strTable, DeviceCodeStr, strField, listIDStr);
		pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
		//insert
		strCmd.Format("insert into PIS.dbo.%s (%s, Device_Code, Status, Finish_Time) "
			"values ('%s', '%s', %d, '%s')", strTable, strField, listIDStr, DeviceCodeStr, iDlStatus, strTime);
		pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
		pConn->CommitTrans();
		bRet = true;
		pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		CString errorStr;
		errorStr.Format("读取数据库PIS.%s出错(%s)\n", strTable, strMsg);
		//AfxMessageBox(errorStr);
	}
	return bRet;
}




void CDataBaseManager::SetDBIp(CString ipAddress)
{
	m_dbIP = ipAddress;
}

CString CDataBaseManager::GetDBIP()
{
	return m_dbIP;
}

int CDataBaseManager::GetAllVideoListData(vector<LIST_INFO> &list, int listType, int dayInterval)
{
	list.clear();
	//CString dbListName = bIsVideoList ? _T("Video_List") : _T("Play_List");

	CString dbListName;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return -1;
	}
	//select * from tb where 时间字段 >= convert(varchar(10),getdate() - 7,120)
	CString strCmd;
	if (listType == 2)
	{
		strCmd = "select * from " + dbListName;
	}
	else
	{
		strCmd.Format(_T("select * from %s where  End_Date  >= convert(varchar(10),getdate() - %d,120)"), dbListName, dayInterval);

	}

	
	
	
	return Search(strCmd, list, listType);
}

int CDataBaseManager::GetDeviceDownLoadStatus(char *listID, vector<Device_DownLoadInfo> &listDeviceDown, int listType)
{

	CString dbListName;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return -1;
	}
	listDeviceDown.clear();
	_RecordsetPtr pRs;
	_ConnectionPtr pConn;

	try
	{
		pConn = GetConnectPtr(m_dbIP);
		pRs.CreateInstance(__uuidof(Recordset));
		CString strCmd;
		strCmd.Format("select a.*, b.Device_Code, b.Device_Name, c.Status, c.Finish_Time from "+dbListName+"_Scope a "
			"inner join Device_List b on (a.Device_Pos=b.Device_Pos and a.Station_Code=b.Station_Code) left join "
			+dbListName+"_Download_Status c on (a.List_ID=c.List_ID and b.Device_Code=c.Device_Code) "
			"where a.List_ID='%s' order by b.Device_Code ASC", listID);
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
	
		while (!pRs->adoEOF)
		{
			Device_DownLoadInfo info;
			//station code and name
			info.strCode = GetFieldValue(pRs, "Device_Code");
			//strItem = GetStationName(strCode);
		
			//device name
			info.strName = GetFieldValue(pRs, "Device_Name");
			//status
			info.iStatus = GetIntField(pRs, "Status");
			if (info.iStatus < 0 || info.iStatus >= DL_MAX)
				info.iStatus = 0;

			//finish time
			info.finishName = GetFieldValue(pRs, "Finish_Time", 2);
			listDeviceDown.push_back(info);
			//next
			pRs->MoveNext();
		}
		pRs->Close();
		pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		return -1;
	}
	

	return 0;
}

_ConnectionPtr CDataBaseManager::GetConnectPtr(LPCTSTR szIP)
{
	CString strConn;
	_bstr_t strCmd;
	_ConnectionPtr pConn;		//连接对象


	//open
	pConn.CreateInstance(__uuidof(Connection));
	pConn->ConnectionTimeout = m_connectTimeOut;
	if (m_iDBType == DB_ORACLE)
	{
		strConn.Format("Provider=MSDAORA; Data Source=%s:1521/PIS; "
			"User ID=%s; Password=%s", szIP, m_strUserName, m_strPwd);
	}
	else if (m_iDBType == DB_MYSQL)
	{
		strConn.Format("Driver=MySQL ODBC 8.0 Unicode Driver; Server=%s; Database=PIS; "
			"User ID=%s; Password=%s", szIP, m_strUserName, m_strPwd);
	}
	else	//default is SQLSERVER
	{
		strConn.Format("Provider=SQLOLEDB; Data Source=%s; Initial Catolog=PIS; "
			"User ID=%s; Password=%s", szIP, m_strUserName, m_strPwd);
	}
	pConn->Open((_bstr_t)strConn, "", "", -1);
	//init session
	if (m_iDBType == DB_ORACLE)
	{
		strCmd = "alter session set nls_date_format = 'yyyy-mm-dd HH24:MI:SS'";
		pConn->Execute(strCmd, NULL, adExecuteNoRecords);
	}
	else if (m_iDBType == DB_MYSQL)
	{
	}
	else	//default is SQLSERVER
	{
		strCmd = "Use PIS";
		pConn->Execute(strCmd, NULL, adExecuteNoRecords);
	}
	return pConn;
}

int CDataBaseManager::Search(CString strCmd, vector<LIST_INFO> &list, int listType)
{
	int nRet = 0;
	CString strItem;
	_ConnectionPtr pConn;
	_RecordsetPtr pRs;
	try
	{
		pConn = GetConnectPtr(m_dbIP);
		//read list info
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			LIST_INFO  info;
			memset(&info, 0, sizeof(LIST_INFO));
			
			GetListInfo(pRs, &info);
			ReadApplyScope(pConn, &info, listType);
			list.push_back(info);
			pRs->MoveNext();
		}
		pRs->Close();
		pConn->Close();
	}
	catch (_com_error & e)
	{

		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		nRet = -1;
	}
	return nRet;
}

void CDataBaseManager::ReadApplyScope(_ConnectionPtr pConn, PLIST_INFO pListInfo, int listType)
{
	CString dbListName;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return ;
	}
	CString strCmd;
	_RecordsetPtr pRs;

	pListInfo->pArrayPos = new CStringArray;
	pListInfo->pArrayStation = new CStringArray;
	try
	{
		pRs.CreateInstance(__uuidof(Recordset));
		//get device pos
		strCmd.Format("Select Distinct Device_Pos From "+ dbListName +"_Scope where List_ID = '%s'", pListInfo->szListID);
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			pListInfo->pArrayPos->Add(GetFieldValue(pRs, "Device_Pos"));
			pRs->MoveNext();
		}
		pRs->Close();
		//get station
		strCmd.Format("Select Station_Code, Line_Code From "+ dbListName +"_Scope where List_ID = '%s'", pListInfo->szListID);
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			pListInfo->pArrayStation->Add(GetFieldValue(pRs, "Station_Code"));
			pRs->MoveNext();
		}
		pRs->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
	}
}

void CDataBaseManager::GetListInfo(_RecordsetPtr pRs, PLIST_INFO pListInfo)
{
	memset(pListInfo, 0, sizeof(LIST_INFO));
	sprintf(pListInfo->szListID, GetFieldValue(pRs, "List_ID"));
	sprintf(pListInfo->szListName, GetFieldValue(pRs, "List_Name"));
	sprintf(pListInfo->szListTitle, GetFieldValue(pRs, "List_Title"));
	pListInfo->iListStatus = GetIntField(pRs, "List_Status");
	if (pListInfo->iListStatus > LIST_STATUS_DELETED || pListInfo->iListStatus < LIST_STATUS_INVALID)
	{
		pListInfo->iListStatus = LIST_STATUS_INVALID;
	}
	sprintf(pListInfo->szStartDate, GetFieldValue(pRs, "Start_Date"));
	sprintf(pListInfo->szEndDate,GetFieldValue(pRs, "End_Date"));
	pListInfo->iWeek = GetIntField(pRs, "Week");
	pListInfo->iPlayFlag = GetIntField(pRs, "Play_Flag");
	sprintf(pListInfo->szUpdateTime, GetFieldValue(pRs, "Update_Time", 2));
	sprintf(pListInfo->szDeleteDate, GetFieldValue(pRs, "Delete_Date"));
	sprintf(pListInfo->szEditorID, "%s %s", GetFieldValue(pRs, "Editor_ID"), GetFieldValue(pRs, "User_Name"));
	sprintf(pListInfo->szEditTime, GetFieldValue(pRs, "Edit_Time", 2));
	sprintf(pListInfo->szScopeEditTime, GetFieldValue(pRs, "Scope_Edit_Time", 2));
	pListInfo->iAudit = GetIntField(pRs, "Audit");
	sprintf(pListInfo->szImportLine, GetFieldValue(pRs, "Import_Line"));
}


// -1 : 错误, 0 : 没有记录, 1: 存在相同的记录
int CDataBaseManager::IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType)
{

	CString dbListName;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return -1;
	}
	int iRet = 0;
	_bstr_t strCmd;
	_RecordsetPtr pRs;
	_ConnectionPtr pConn;

	//检测节目单名称是否有重复
	//WritLog(L"检测节目单名称是否有重复");
	try
	{
		pConn = GetConnectPtr(m_dbIP);
		pRs.CreateInstance(__uuidof(Recordset));
		strCmd = "Select List_ID, List_Title, Edit_Time, \"Audit\" From "+ dbListName +" where List_Name = '" + strName + L"'";

		if (listType == 2)
		{
			strCmd = "Select List_ID, List_Title, Edit_Time, \"Audit\" From " + dbListName + " where List_Title = '" + strName + L"'";
		}

		pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			bAudit = GetIntField(pRs, "Audit");
			strListID = GetFieldValue(pRs, "List_ID");
			strTitle = GetFieldValue(pRs, "List_Title");
			strUpdateTime = GetFieldValue(pRs, "Edit_Time", 2);
			iRet = 1;
		}
		pRs->Close();
		pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		iRet = -1;
	}
	return iRet;
}

CString CDataBaseManager::GetMaxIDCmd(LPCTSTR szTable, CString strServerLineCode, LPCTSTR szField)
{
	CString strCmd;

	if (m_iDBType == DB_ORACLE)
	{
		if (strServerLineCode.IsEmpty())
		{
			strCmd.Format("Select max(%s) as Max_ID from %s where Import_Line is null",
				szField, szTable);
		}
		else
		{
			strCmd.Format("Select max(%s) as Max_ID from %s where Import_Line = '%s'",
				szField, szTable, strServerLineCode);
		}
	}
	else if (m_iDBType == DB_MYSQL)
	{
		strCmd.Format("Select max(%s) as Max_ID from %s where Import_Line = '%s'",
			szField, szTable, strServerLineCode);
	}
	else	//default is SQLSERVER
	{
		strCmd.Format("Select max(%s) as Max_ID from %s where Import_Line = '%s'",
			szField, szTable, strServerLineCode);
	}
	return strCmd;
}

CString CDataBaseManager::IncreaseCode(CString & strCode, char cFlag)
{
	UINT uNum;
	char szValue[16], szTemp[16];

	if (strCode.GetLength() != 10)
	{
		sprintf(szValue, "%c000000001", cFlag);
	}
	else
	{
		uNum = strtoul(strCode.Mid(1), NULL, 36);
		_ultoa(uNum + 1, szTemp, 36);
		_strupr(szTemp);
		sprintf(szValue, "%c%09s", cFlag, szTemp);
	}
	return szValue;
}

BOOL CDataBaseManager::GetPlayListID(_ConnectionPtr pConn, CString strServerLineCode, CString & strListID,int listType)
{
	CString dsListName;
	if (listType == 0)
	{
		dsListName = _T("Play_List");
	}else if (listType == 1)
	{
		dsListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dsListName = _T("Default_List");
	}
	else
	{
		return false;
	}
	BOOL bRet = false;
	CString strCmd, strCode;
	char cFlag;
	_RecordsetPtr pRs;			//记录集对象

	cFlag = '0';
	if (strServerLineCode.GetLength() > 1)
	{
		cFlag += (UINT)strtoul(strServerLineCode.Mid(1), NULL, 36);
	}
	try
	{
		pRs.CreateInstance(__uuidof(Recordset));
		strCmd = GetMaxIDCmd(dsListName, strServerLineCode, "List_ID");
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenDynamic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			strCode = GetFieldValue(pRs, "Max_ID");
		}
		pRs->Close();
		strListID = IncreaseCode(strCode, cFlag);

		bRet = true;
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//MessageBox(strMsg, "确定", MB_OK | MB_ICONINFORMATION);
	}
	return bRet;
}

BOOL CDataBaseManager::GetDefaultPlayListID(_ConnectionPtr pConn, CString strServerLineCode, CString & strListID, int listType)
{
	CString dsListName = _T("Default_List");

	BOOL bRet = false;
	CString strCmd, strCode;
	char cFlag;
	_RecordsetPtr pRs;			//记录集对象

	cFlag = '0';
	if (strServerLineCode.GetLength() > 1)
	{
		cFlag += (UINT)strtoul(strServerLineCode.Mid(1), NULL, 36);
	}
	try
	{
		pRs.CreateInstance(__uuidof(Recordset));
		strCmd = GetMaxIDCmd(dsListName, strServerLineCode, "List_ID");
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenDynamic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			strCode = GetFieldValue(pRs, "Max_ID");
		}
		pRs->Close();
	
		UINT uNum;
		char szValue[16], szTemp[16];

		if (strCode.GetLength() != 5)
		{
			sprintf(szValue, "%c0001", cFlag);
		}
		else
		{
			uNum = strtoul(strCode.Mid(1), NULL, 36);
			_ultoa(uNum + 1, szTemp, 36);
			_strupr(szTemp);
			sprintf(szValue, "%c%04s", cFlag, szTemp);
		}
		strListID = szValue;
		bRet = true;
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//MessageBox(strMsg, "确定", MB_OK | MB_ICONINFORMATION);
	}
	return bRet;
}

int CDataBaseManager::IsExistTitleEx(_ConnectionPtr pConn, CString strTitle, int listType)
{
	CString dbListName;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return false;
	}
	int iRet = 0;
	CString strCmd;
	_RecordsetPtr pRs;

	//检测文件标题是否有重复
	try
	{
		strCmd.Format("Select List_ID From "+ dbListName + " where List_Title = '%s'", strTitle);
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			iRet = 1;
		}
		pRs->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		iRet = -1;
	}
	return iRet;
}

int CDataBaseManager::isExistStation(_ConnectionPtr pConn,CString listID, CString devicePos, CString stationCode, CString lineCode, int listType)
{
	CString dbListName ;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return -1;
	}
	int iRet = 0;
	//_bstr_t strCmd;
	_RecordsetPtr pRs;


	//检测节目单名称是否有重复
	//WritLog(L"检测节目单名称是否有重复");
	try
	{
		//pConn = GetConnectPtr(m_dbIP);
		pRs.CreateInstance(__uuidof(Recordset));
		CString strCmd;
 		strCmd.Format("Select * From %s_Scope where List_ID = '%s' AND Device_Pos= '%s' AND Station_Code= '%s' AND Line_Code= '%s'",
			dbListName,listID, devicePos, stationCode, lineCode);
		
		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			iRet = 1;
		}
		pRs->Close();
		//pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		iRet = -1;
	}
	return iRet;

}

BOOL CDataBaseManager::UpdatePlayListRecord(LIST_INFO_tag &listInfo, CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate, bool bIsVideoList)
{
	if (listInfo.ptrSelectedStation == NULL || listInfo.ptrSelectedStation == NULL)
	{
		return -1;
	}

	CString dbListName = bIsVideoList ? _T("Video_List") : _T("Play_List");

	int i, j, iSize, iNum;
	BOOL bRet = FALSE, bGetIDOk = false;
	CString strCmd, strTime;
	_ConnectionPtr pConn;
	//CStationInfo * pStationInfo;
	COleDateTime tm = COleDateTime::GetCurrentTime();

	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	try
	{
		pConn = GetConnectPtr(m_dbIP);
		//begin trans
		pConn->BeginTrans();
		//process list record
		if (iUpdate == 0)			//add new record
		{
			CString strListID;
			
			if (GetPlayListID(pConn,listInfo.szImportLine, strListID, bIsVideoList ? 1 : 0))
			{
				sprintf(listInfo.szListID, "%s", strListID.GetBuffer());
			
				bGetIDOk = true;
				if (IsExistTitleEx(pConn, listInfo.szListTitle, bIsVideoList ? 1 : 0) != 0)
				{
					//strTitle = strListID;

					sprintf(listInfo.szListTitle, listInfo.szListID);

				}
				strCmd.Format("Insert Into %s "
					"(List_ID, List_Name, List_Title, List_Status, Play_Flag, Update_Time, "
					"Delete_Date, Start_Date, End_Date, Week, Editor_ID, Edit_Time, "
					"Scope_Edit_Time, \"Audit\", List_MD5, Import_Line) "
					"values ('%s', '%s', '%s', %d, %d, '%s', '%s', '%s', '%s', %d, "
					"'%s', '%s', '%s', %d, '%s', '%s')",dbListName.GetBuffer(),
					listInfo.szListID,listInfo.szListName,listInfo.szListTitle, LIST_STATUS_VALID, listInfo.iPlayFlag, listInfo.szUpdateTime,
					listInfo.szDeleteDate, listInfo.szStartDate, listInfo.szEndDate, listInfo.iWeek, listInfo.szEditorID, listInfo.szEditTime,
					listInfo.szScopeEditTime, listInfo.iAudit,listInfo.szMd5, listInfo.szImportLine);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
		}
		else if (iUpdate == 1)	//update old record
		{
			bGetIDOk = true;
			//check update time
			if (strUpdateTime != strOldUpdateTime)
			{
				strCmd.Format("delete from %s_Download_Status where List_ID = '%s'", dbListName.GetBuffer(), listInfo.szListID);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
			//update
			strCmd.Format("Update %s Set List_Status = %d, Update_Time = '%s', Editor_ID = '%s', "
				"Start_Date ='%s', End_Date ='%s', Week = %d, Play_Flag = %d, "
				"Edit_Time = '%s', Scope_Edit_Time = '%s' where List_ID = '%s'", dbListName.GetBuffer(),
				LIST_STATUS_VALID, listInfo.szUpdateTime, listInfo.szEditorID, listInfo.szStartDate, listInfo.szEndDate,
				listInfo.iWeek, listInfo.iPlayFlag, listInfo.szEditTime, listInfo.szScopeEditTime, listInfo.szListID);
			pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			
		}


		//查询
		UpdatePlayListRecordZone(pConn,listInfo, iUpdate, bIsVideoList ? 1 : 0);
		//process list apply scope
		
		//commit trans
		pConn->CommitTrans();
		//close connection
		pConn->Close();
		bRet = true;
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		bRet = FALSE;
	}
	return (bRet && bGetIDOk);
}


void CDataBaseManager::GetLineList(CTypedPtrArray<CPtrArray, CLineInfo *> &lineList, CStringArray &strArrayPos)
{
	lineList.RemoveAll();
	strArrayPos.RemoveAll();

	int i, iSize;
	CString strCode;
	CLineInfo * pLineInfo;
	CStationInfo * pStationInfo;
	_bstr_t strCmd;
	_ConnectionPtr pConn;
	_RecordsetPtr pRs;

	try
	{
		pConn = GetConnectPtr(m_dbIP);

		//read device pos
		pRs.CreateInstance(__uuidof(Recordset));
		strCmd = "Select * from Device_Pos order by Type_Order ASC";
		pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			strArrayPos.Add(GetFieldValue(pRs, "Type_Name"));
			pRs->MoveNext();
		}
		pRs->Close();

		//read line list
		strCmd = "Select * from Line_List order by Line_Code ASC";
		pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			pLineInfo = new CLineInfo();
			pLineInfo->ReadInfo(pRs);
			lineList.Add(pLineInfo);
			pRs->MoveNext();
		}
		pRs->Close();
		//read station list
		iSize = lineList.GetSize();
		for (i = 0; i < iSize; i++)
		{
			pLineInfo = lineList[i];
			strCmd = "Select Station_Code, Station_Name, Line_Code, Is_Train from Station_List "
				"where Line_Code = '" + (CString)pLineInfo->szLineCode + "' order by Station_Code ASC";
			pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
			while (!pRs->adoEOF)
			{
				pStationInfo = new CStationInfo();
				pStationInfo->ReadInfo(pRs);
				if (pStationInfo->bIsTrain)
				{
					pLineInfo->m_ptrTrainList.Add(pStationInfo);
				}
				else
				{
					pLineInfo->m_ptrStationList.Add(pStationInfo);
				}
				pRs->MoveNext();
			}
			pRs->Close();
		}

		//read DeviceCode list
		pLineInfo->m_arrDeviceCode.RemoveAll();
		for (i = 0; i < iSize; i++)
		{
			pLineInfo = lineList[i];
			strCmd = "Select Device_Code from Device_List "
				"where Line_Code = '" + (CString)pLineInfo->szLineCode + "'";
			pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
			while (!pRs->adoEOF)
			{
				CString str = GetFieldValue(pRs, "Device_Code");
				pLineInfo->m_arrDeviceCode.Add(str);
				pRs->MoveNext();
			}
			pRs->Close();
		}


		pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
	}
}


BOOL CDataBaseManager::UpdatePlayListRecordZone(_ConnectionPtr pConn,LIST_INFO_tag &listInfo, int iUpdate, int listType)
{
	CString dbListName ;
	if (listType == 0)
	{
		dbListName = _T("Play_List");
	}
	else if (listType == 1)
	{
		dbListName = _T("Video_List");
	}
	else if (listType == 2)
	{
		dbListName = _T("Default_List");
	}
	else
	{
		return false;
	}
	CString	strCmd;
	//if (iUpdate == 0)
	{
		CString lisID = listInfo.szListID;
		strCmd = "delete from "+ dbListName +"_Scope where List_ID = '" + lisID + "'";
		pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
	}
	
	int iSize = listInfo.strArraySelectedPos->GetSize();
	for (int i = 0; i < iSize; i++)
	{
		int iNum = listInfo.ptrSelectedStation->GetSize();
		for (int j = 0; j < iNum; j++)
		{
			CStationInfo *pStationInfo = (*listInfo.ptrSelectedStation)[j];

			CString zoneStr = (*listInfo.strArraySelectedPos)[i];

			//判断是否存在
			if (isExistStation(pConn,listInfo.szListID, zoneStr, pStationInfo->szStationCode, pStationInfo->szLineCode, listType) == 0)
			{
				strCmd.Format("insert into "+ dbListName +"_Scope (List_ID, "
					"Device_Pos, Station_Code, Line_Code) values ('%s', '%s', '%s', '%s')",
					listInfo.szListID, zoneStr.GetBuffer(), pStationInfo->szStationCode, pStationInfo->szLineCode);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
			
		}
	}

	return TRUE;
}

BOOL CDataBaseManager::UpdateDefaultPlayListRecord(LIST_INFO_tag &listInfo, CString & strUpdateTime, CString & strOldUpdateTime, int iUpdate)
{
	if (listInfo.ptrSelectedStation == NULL || listInfo.ptrSelectedStation == NULL)
	{
		return -1;
	}
	CString dbListName = _T("Default_List");
	int i, j, iSize, iNum;
	BOOL bRet = FALSE, bGetIDOk = false;
	CString strCmd, strTime;
	_ConnectionPtr pConn;
	//CStationInfo * pStationInfo;
	COleDateTime tm = COleDateTime::GetCurrentTime();

	strTime = tm.Format("%Y-%m-%d %H:%M:%S");
	try
	{
		pConn = GetConnectPtr(m_dbIP);
		//begin trans
		pConn->BeginTrans();
		//process list record
		if (iUpdate == 0)			//add new record
		{
			CString strListID;

			if (GetDefaultPlayListID(pConn, listInfo.szImportLine, strListID,2))
			{
				sprintf(listInfo.szListID, "%s", strListID.GetBuffer());

				bGetIDOk = true;
				if (IsExistTitleEx(pConn, listInfo.szListTitle, 2) != 0)
				{
					//strTitle = strListID;

					sprintf(listInfo.szListTitle, listInfo.szListID);

				}

				//ListID 最大长度是5位
				CString tmpStr = listInfo.szListID;
				if (tmpStr.GetLength() > 5)
				{
					//去除中间多余的 保留首尾
					CString headerStr = tmpStr.Left(2);
					CString endStr = tmpStr.Right(2);
					CString midStr = tmpStr.Mid(2, 1);
					CString finalStr = headerStr + midStr + endStr;
					sprintf_s(listInfo.szListID, finalStr.GetBuffer());

				}
				

				strCmd.Format("Insert Into %s "
					"(List_ID, List_Title, List_Status, Update_Time, "
					"Delete_Date,Editor_ID, Edit_Time, "
					"Scope_Edit_Time, \"Audit\", List_MD5, Import_Line) "
					"values ('%s', '%s', %d, '%s', '%s', '%s', '%s', "
					" '%s', %d, '%s', '%s')", dbListName.GetBuffer(),
					listInfo.szListID, listInfo.szListTitle, LIST_STATUS_VALID, listInfo.szUpdateTime,
					listInfo.szDeleteDate, listInfo.szEditorID, listInfo.szEditTime,
					listInfo.szScopeEditTime, listInfo.iAudit, listInfo.szMd5, listInfo.szImportLine);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
		}
		else if (iUpdate == 1)	//update old record
		{
			bGetIDOk = true;
			//check update time
			if (strUpdateTime != strOldUpdateTime)
			{
				strCmd.Format("delete from %s_Download_Status where List_ID = '%s'", dbListName.GetBuffer(), listInfo.szListID);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
			//update
			strCmd.Format("Update %s Set List_Status = %d, Update_Time = '%s', Editor_ID = '%s', "
				"Edit_Time = '%s', Scope_Edit_Time = '%s' where List_ID = '%s'", dbListName.GetBuffer(),
				LIST_STATUS_VALID, listInfo.szUpdateTime, listInfo.szEditorID,listInfo.szEditTime, listInfo.szScopeEditTime, listInfo.szListID);
			pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);

		}


		//查询
		UpdatePlayListRecordZone(pConn, listInfo, iUpdate, 2);
		//process list apply scope

		//commit trans
		pConn->CommitTrans();
		//close connection
		pConn->Close();
		bRet = true;
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		//AfxMessageBox(strMsg, MB_OK | MB_ICONINFORMATION);
		bRet = FALSE;
	}
	return (bRet && bGetIDOk);

}

//class CLineInfo
CLineInfo::CLineInfo()
{
	ResetData();
}

CLineInfo::~CLineInfo()
{

}

void CLineInfo::ResetData()
{
	memset(szLineCode, 0, sizeof(szLineCode));
	memset(szLineName, 0, sizeof(szLineName));
	m_arrDeviceCode.RemoveAll();
}

void CLineInfo::ReadInfo(_RecordsetPtr pRs)
{
	sprintf(szLineCode, "%s", GetFieldValue(pRs, "Line_Code"));
	sprintf(szLineName, "%s", GetFieldValue(pRs, "Line_Name"));
}

CString CLineInfo::GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize )
{

	long lValue;
	_variant_t vtValue;
	CString strValue;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		strValue.Empty();
		if (vtValue.vt == VT_I1)
		{
			strValue.Format("%d", vtValue.cVal);
		}
		else if (vtValue.vt == VT_UI1)
		{
			strValue.Format("%d", vtValue.bVal);
		}
		else if (vtValue.vt == VT_I2)
		{
			strValue.Format("%d", vtValue.iVal);
		}
		else if (vtValue.vt == VT_UI2)
		{
			strValue.Format("%d", vtValue.uiVal);
		}
		else if (vtValue.vt == VT_I4)
		{
			strValue.Format("%d", vtValue.lVal);
		}
		else if (vtValue.vt == VT_UI4)
		{
			strValue.Format("%d", vtValue.ulVal);
		}
		else if (vtValue.vt == VT_INT)
		{
			strValue.Format("%d", vtValue.intVal);
		}
		else if (vtValue.vt == VT_UINT)
		{
			strValue.Format("%d", vtValue.uintVal);
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), &lValue);
			strValue.Format("%d", lValue);
		}
		else if (vtValue.vt == VT_DATE)
		{
			COleDateTime tm = vtValue.date;
			strValue = (iReturnSize == 0) ? tm.Format("%Y-%m-%d") : tm.Format("%Y-%m-%d %H:%M:%S");
		}
		else if (vtValue.vt == VT_BSTR)
		{
			strValue = vtValue.bstrVal;
			strValue.TrimRight();
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}
	return strValue;
}





//class CStationInfo
CStationInfo::CStationInfo()
{
	ResetData();
}

CStationInfo::~CStationInfo()
{

}

void CStationInfo::ResetData()
{
	memset(szStationCode, 0, sizeof(szStationCode));
	memset(szStationName, 0, sizeof(szStationName));
	memset(szLineCode, 0, sizeof(szLineCode));
	bIsTrain = false;
}

void CStationInfo::CopyInfo(CStationInfo & src)
{
	sprintf(szStationCode, "%s", src.szStationCode);
	sprintf(szStationName, "%s", src.szStationName);
	sprintf(szLineCode, "%s", src.szLineCode);
	bIsTrain = src.bIsTrain;
}

void CStationInfo::ReadInfo(_RecordsetPtr pRs)
{
	sprintf(szStationCode, "%s", GetFieldValue(pRs, "Station_Code"));
	sprintf(szStationName, "%s", GetFieldValue(pRs, "Station_Name"));
	sprintf(szLineCode, "%s", GetFieldValue(pRs, "Line_Code"));
	bIsTrain = GetIntField(pRs, "Is_Train");
}

int CStationInfo::GetIntField(_RecordsetPtr pRs, _variant_t vtField)
{
	_variant_t vtValue;
	int iValue = 0;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		if (vtValue.vt == VT_I1)
		{
			iValue = (int)vtValue.cVal;
		}
		else if (vtValue.vt == VT_UI1)
		{
			iValue = (int)vtValue.bVal;
		}
		else if (vtValue.vt == VT_I2)
		{
			iValue = (int)vtValue.iVal;
		}
		else if (vtValue.vt == VT_UI2)
		{
			iValue = (int)vtValue.uiVal;
		}
		else if (vtValue.vt == VT_I4)
		{
			iValue = (int)vtValue.lVal;
		}
		else if (vtValue.vt == VT_UI4)
		{
			iValue = (int)vtValue.ulVal;
		}
		else if (vtValue.vt == VT_INT)
		{
			iValue = (int)vtValue.intVal;
		}
		else if (vtValue.vt == VT_UINT)
		{
			iValue = (int)vtValue.uintVal;
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), (long *)&iValue);
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}
	return iValue;
}

CString CStationInfo::GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize )
{

	long lValue;
	_variant_t vtValue;
	CString strValue;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		strValue.Empty();
		if (vtValue.vt == VT_I1)
		{
			strValue.Format("%d", vtValue.cVal);
		}
		else if (vtValue.vt == VT_UI1)
		{
			strValue.Format("%d", vtValue.bVal);
		}
		else if (vtValue.vt == VT_I2)
		{
			strValue.Format("%d", vtValue.iVal);
		}
		else if (vtValue.vt == VT_UI2)
		{
			strValue.Format("%d", vtValue.uiVal);
		}
		else if (vtValue.vt == VT_I4)
		{
			strValue.Format("%d", vtValue.lVal);
		}
		else if (vtValue.vt == VT_UI4)
		{
			strValue.Format("%d", vtValue.ulVal);
		}
		else if (vtValue.vt == VT_INT)
		{
			strValue.Format("%d", vtValue.intVal);
		}
		else if (vtValue.vt == VT_UINT)
		{
			strValue.Format("%d", vtValue.uintVal);
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), &lValue);
			strValue.Format("%d", lValue);
		}
		else if (vtValue.vt == VT_DATE)
		{
			COleDateTime tm = vtValue.date;
			strValue = (iReturnSize == 0) ? tm.Format("%Y-%m-%d") : tm.Format("%Y-%m-%d %H:%M:%S");
		}
		else if (vtValue.vt == VT_BSTR)
		{
			strValue = vtValue.bstrVal;
			strValue.TrimRight();
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}
	return strValue;
}

DATE CStationInfo::GetDateField(_RecordsetPtr pRs, _variant_t vtField)
{
	_variant_t vtValue;
	DATE dateValue = 0;

	try
	{
		vtValue = pRs->GetCollect(vtField);
		if (vtValue.vt == VT_DATE)
		{
			dateValue = vtValue.date;
		}
	}
	catch (_com_error & e)
	{
		e.Description();
	}

	return dateValue;
}
