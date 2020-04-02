#include "stdafx.h"
#include "../����ӿ�/DataBase_Interface.h"

#include "DataBaseHandle.h"
static vector<CDataBaseHandle*> g_groupDBList; //�������д��������ݿ���

int OC_SetDataBaseParams(int nIndex,  OC_DB_PARAMS params)
{
	if (nIndex < 0)
	{
		return OC_ERROR_CODE_PARAM;
	}

	int nSize = g_groupDBList.size();
	if (nIndex >= nSize)
	{
		CDataBaseHandle *db = new CDataBaseHandle(nIndex);
		if (db == NULL)
		{
			return OC_ERROR_CODE_FAIL;
		}
		db->SetDataBaseParams(params);
		g_groupDBList.push_back(db);
	}

	return OC_ERROR_CODE_NONE;
}

 void OC_DeleteAllDataBase()
{
	 for (unsigned int i = 0 ; i < g_groupDBList.size();++i)
	 {
		 CDataBaseHandle *db = g_groupDBList[i];
		 if (db)
		 {
			 delete db;
			 db = NULL;
		 }
	 }
}

 bool OC_ExectSql(int nIndex, CString sqlStr, CString &errorStr)
 {
	 if (nIndex >= g_groupDBList.size())
	 {
		 errorStr = _T("δ�������ݿ�Ĳ��� ��ҪOC_SetDataBaseParams");
		 return false;
	 }
	 return g_groupDBList[nIndex]->ExectSql(sqlStr, errorStr);
 }

 bool OC_ExectSelectSql(int nIndex, CString sqlStr, const CStringArray &keyList, map< CString, CStringArray > &valueList, CString &errorStr)
 {
	 if (nIndex >= g_groupDBList.size())
	 {
		 errorStr = _T("δ�������ݿ�Ĳ��� ��ҪOC_SetDataBaseParams");
		 return false;
	 }
	 return g_groupDBList[nIndex]->ExectSql(sqlStr, keyList,valueList, errorStr);
 }
