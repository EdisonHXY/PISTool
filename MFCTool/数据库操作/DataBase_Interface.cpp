#include "stdafx.h"
#include "../对外接口/DataBase_Interface.h"

#include "DataBaseHandle.h"
static CDataBaseHandleGroup g_DB_group;

int OC_SetDataBaseParams(int nIndex,  OC_DB_PARAMS params)
{
	if (nIndex < 0)
	{
		return OC_ERROR_CODE_PARAM;
	}

	int nSize = g_DB_group.m_groupDBList.size();
	if (nIndex >= nSize)
	{
		CDataBaseHandle *db = new CDataBaseHandle(nIndex);
		if (db == NULL)
		{
			return OC_ERROR_CODE_FAIL;
		}
		db->SetDataBaseParams(params);
		g_DB_group.m_groupDBList.push_back(db);
	}

	return OC_ERROR_CODE_NONE;
}

 void OC_DeleteAllDataBase()
{
	 g_DB_group.DeletAllDB();
}

 bool OC_ExectSql(int nIndex, CString sqlStr, CString &errorStr)
 {
	 if (nIndex >= g_DB_group.m_groupDBList.size())
	 {
		 errorStr = _T("未设置数据库的参数 需要OC_SetDataBaseParams");
		 return false;
	 }
	 return  g_DB_group.m_groupDBList[nIndex]->ExectSql(sqlStr, errorStr);
 }

 bool OC_ExectSelectSql(int nIndex, CString sqlStr, const CStringArray &keyList, map< CString, CStringArray *> &valueList, CString &errorStr)
 {
	 if (nIndex >= g_DB_group.m_groupDBList.size())
	 {
		 errorStr = _T("未设置数据库的参数 需要OC_SetDataBaseParams");
		 return false;
	 }
	 return  g_DB_group.m_groupDBList[nIndex]->ExectSql(sqlStr, keyList,valueList, errorStr);
 }

 bool OC_ReleaseBuffer(int nIndex, map< CString, CStringArray* > &valueList)
 {
	 if (nIndex >= g_DB_group.m_groupDBList.size())
	 {
		 //errorStr = _T("未设置数据库的参数 需要OC_SetDataBaseParams");
		 return false;
	 }
	 return g_DB_group.m_groupDBList[nIndex]->ReleaseBuffer(valueList);
 }
