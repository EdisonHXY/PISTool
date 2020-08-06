#include "stdafx.h"
#include "DataBaseOldManager.h"


CDataBaseOldManager::CDataBaseOldManager()
{
	
}


CDataBaseOldManager::~CDataBaseOldManager()
{
}

BOOL CDataBaseOldManager::UpdatePlayListRecord(LIST_INFO_Old_tag &listInfo,int iUpdate, bool isDefault)
{
	CString dbListName = isDefault ? _T("Default_List") :_T("Play_List") ;

	int i, j, iSize, iNum;
	BOOL bRet = FALSE, bGetIDOk = false;
	CString strCmd, strTime;
	_ConnectionPtr pConn;

	try
	{
		pConn = GetConnectPtr(m_dbIP);
		//begin trans
		pConn->BeginTrans();
		//process list record
		if (iUpdate == 0)			//add new record
		{
			CString strListID;
			bool bRet = isDefault ? GetDefaultPlayListID(pConn, listInfo.szImportLine, strListID, 2) : GetPlayListID(pConn, listInfo.szImportLine, strListID,  0);
			if (bRet)
			{
				sprintf(listInfo.szListID, "%s", strListID.GetBuffer());

				bGetIDOk = true;
				if (IsExistTitleEx(pConn, listInfo.szListTitle,  isDefault ? 2 : 0) != 0)
				{
					//strTitle = strListID;

					sprintf(listInfo.szListTitle, listInfo.szListID);

				}

				if (isDefault)
				{

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
						"Scope_Edit_Time, Audit_Num, Audit_Level, Import_Line) "
						"values ('%s', '%s', %d, '%s', '%s', '%s', '%s', "
						" '%s', %d, %d, '%s')", dbListName.GetBuffer(),
						listInfo.szListID, listInfo.szListTitle, listInfo.iListStatus, listInfo.szUpdateTime,
						listInfo.szDeleteDate, listInfo.szEditorID, listInfo.szEditTime,
						listInfo.szScopeEditTime, listInfo.iAudit_Num, listInfo.iAudit_Level, listInfo.szImportLine);
				}
				else {
					strCmd.Format("Insert Into %s "
						"(List_ID, List_Name, List_Title, List_Status, Update_Time, "
						"Delete_Date, Start_Date, End_Date, Week, Editor_ID, Edit_Time, "
						"Scope_Edit_Time, Audit_Num, Audit_Level, Import_Line) "
						"values ('%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', %d, "
						"'%s', '%s', '%s', %d,%d, '%s')", dbListName.GetBuffer(),
						listInfo.szListID, listInfo.szListName, listInfo.szListTitle, listInfo.iListStatus, listInfo.szUpdateTime,
						listInfo.szDeleteDate, listInfo.szStartDate, listInfo.szEndDate, listInfo.iWeek, listInfo.szEditorID, listInfo.szEditTime,
						listInfo.szScopeEditTime, listInfo.iAudit_Num, listInfo.iAudit_Level, listInfo.szImportLine);
				}
				
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
			}
			
		}
		else if (iUpdate == 1)	//update old record
		{
			bGetIDOk = true;
			//check update time
			if (isDefault)
			{
				strCmd.Format("Update %s Set List_Status = %d, Update_Time = '%s', Editor_ID = '%s', "
					"Edit_Time = '%s', Scope_Edit_Time = '%s',Audit_Level =  %d where List_ID = '%s'", dbListName.GetBuffer(),
					listInfo.iListStatus, listInfo.szUpdateTime, listInfo.szEditorID, listInfo.szEditTime, listInfo.szScopeEditTime, listInfo.iAudit_Level, listInfo.szListID);
			}
			else
			{
				strCmd.Format("delete from List_Download_Status where List_ID = '%s'", listInfo.szListID);
				pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);

				//update
				strCmd.Format("Update %s Set List_Status = %d, Update_Time = '%s', Editor_ID = '%s', "
					"Start_Date ='%s', End_Date ='%s', Week = %d,  "
					"Edit_Time = '%s', Scope_Edit_Time = '%s',Audit_Level =  %d where List_ID = '%s'", dbListName.GetBuffer(),
					LIST_STATUS_VALID, listInfo.szUpdateTime, listInfo.szEditorID, listInfo.szStartDate, listInfo.szEndDate,
					listInfo.iWeek, listInfo.szEditTime, listInfo.szScopeEditTime, listInfo.iAudit_Level, listInfo.szListID);

			}
			
			pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);

		}


		//查询
		UpdatePlayListRecordZone(pConn, listInfo, iUpdate,isDefault);
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

BOOL CDataBaseOldManager::UpdatePlayListRecordZone(_ConnectionPtr pConn, LIST_INFO_Old_tag &listInfo, int iUpdate, bool isDefault)
{
	CString dbListName = isDefault ? _T("Default_List_Apply"):  _T("List_Apply");
	
	CString	strCmd;
	//if (iUpdate == 0)
	{
		CString lisID = listInfo.szListID;
		strCmd = "delete from " + dbListName + "_Scope where List_ID = '" + lisID + "'";
		pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
	}

	int iSize = listInfo.selectedGroupList.size();
	for (int i = 0; i < iSize; i++)
	{

		int deviceGroup = listInfo.selectedGroupList[i];

		//判断是否存在
		if (isExistStation(pConn, listInfo.szListID, deviceGroup, listInfo.szApplyScopLine) == 0)
		{
			strCmd.Format("insert into " + dbListName + "_Scope (List_ID, "
				"Device_Group, Line_Code) values ('%s', %d, '%s')",
				listInfo.szListID, deviceGroup, listInfo.szApplyScopLine);
			pConn->Execute((_bstr_t)strCmd, NULL, adCmdText);
		}

		
	}

	return TRUE;
}

int CDataBaseOldManager::IsExistName(CString strName, CString & strListID, CString & strTitle, CString & strUpdateTime, BOOL & bAudit, int listType)
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
		strCmd = "Select List_ID, List_Title, Edit_Time, \"Audit_Level\" From " + dbListName + " where List_Name = '" + strName + L"'";

		if (listType == 2)
		{
			strCmd = "Select List_ID, List_Title, Edit_Time, \"Audit_Level\" From " + dbListName + " where List_Title = '" + strName + L"'";
		}

		pRs->Open(strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		if (!pRs->adoEOF)
		{
			bAudit = GetIntField(pRs, "Audit_Level");
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

int CDataBaseOldManager::isExistStation(_ConnectionPtr pConn, CString listID, int deviceGroup, CString lineCode)
{
	CString dbListName = _T("List_Apply");
	
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
		strCmd.Format("Select * From %s_Scope where List_ID = '%s' AND Device_Group= %d AND Line_Code= '%s'",
			dbListName, listID, deviceGroup, lineCode);

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

int CDataBaseOldManager::GetListDownloadStatus(vector<DownLoadStatusList_Old> &statusList)
{
	CString dbListName = _T("List_Download_Status");
	statusList.clear();

	int iRet = 0;
	_bstr_t strCmd;
	_RecordsetPtr pRs;
	_ConnectionPtr pConn;

	try
	{
		pConn = GetConnectPtr(m_dbIP);

		//pConn = GetConnectPtr(m_dbIP);
		pRs.CreateInstance(__uuidof(Recordset));
		CString strCmd;
		strCmd.Format("Select * From %s",
			dbListName);

		pRs->Open((_bstr_t)strCmd, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		while (!pRs->adoEOF)
		{
			DownLoadStatusList_Old info;
			info.nameStr = GetFieldValue(pRs, "List_Name");


			//Device_Code
			info.device_CodeStr = GetFieldValue(pRs, "Device_Code");
			//status
			info.iStatus = GetIntField(pRs, "Download_Status");
			if (info.iStatus < 0 || info.iStatus >= DL_MAX)
				info.iStatus = 0;

			//finish time
			info.finishTimeStr = GetFieldValue(pRs, "Finish_Time", 2);

			statusList.push_back(info);

			//next
			pRs->MoveNext();
		}

		pRs->Close();
		pConn->Close();
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();

		iRet = -1;
	}
	return iRet;

}
