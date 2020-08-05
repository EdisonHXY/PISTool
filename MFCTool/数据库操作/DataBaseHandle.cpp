#include "stdafx.h"
#include "DataBaseHandle.h"


CDataBaseHandle::CDataBaseHandle(int tag)
{
	m_tag = tag;
	m_params.timeOut = 0;
	//memset(&m_params, 0, sizeof(m_params));
	m_iDBType = DB_SQLSERVER;
	m_isReleaseBuffer = true;
}


CDataBaseHandle::~CDataBaseHandle()
{
}

void CDataBaseHandle::SetDataBaseParams( OC_DB_PARAMS params)
{
	m_params.timeOut = params.timeOut < 0 ? 2 : params.timeOut;
	if (!params.addressStr.IsEmpty())
	{
		m_params.addressStr = params.addressStr;
	}
	if (!params.name.IsEmpty())
	{
		m_params.name = params.name;
	}
	if (!params.passwordStr.IsEmpty())
	{
		m_params.passwordStr = params.passwordStr;
	}
	if (!params.usrName.IsEmpty())
	{
		m_params.usrName = params.usrName;
	}

}

void CDataBaseHandle::GetTag(int &tag)
{
	tag = m_tag;
}

bool CDataBaseHandle::ExectSql(CString sqlStr, CString &errorStr)
{
	_ConnectionPtr pConn;
	bool bRet = false;
	errorStr.Empty();
	try
	{
		pConn = GetConnectPtr();
		//begin trans
		pConn->BeginTrans();

		pConn->Execute((_bstr_t)sqlStr, NULL, adCmdText);

		//commit trans
		pConn->CommitTrans();
		//close connection
		pConn->Close();
		bRet = true;

	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		errorStr = (LPCTSTR)strMsg;
	}

	return bRet;
}

bool CDataBaseHandle::ExectSql(CString sqlStr, const CStringArray &keyList, map< CString, CStringArray * > &valueList, CString &errorStr)
{

	if (!m_isReleaseBuffer)
	{
		errorStr = "未释放上一次的内存，请调要OC_ReleaseBuffer";
		return false;
	}

	_ConnectionPtr pConn;
	bool bRet = false;
	errorStr.Empty();
	_RecordsetPtr pRs;

	valueList.clear();

	try
	{
		pConn = GetConnectPtr();
		//read list info
		pRs.CreateInstance(__uuidof(Recordset));
		pRs->Open((_bstr_t)sqlStr, pConn.GetInterfacePtr(), adOpenStatic, adLockReadOnly, adCmdText);
		map<CString, CStringArray * > ::iterator it;
		while (!pRs->adoEOF)
		{
			for (unsigned int i = 0; i < keyList.GetSize(); i++)
			{
				CString keyStr = keyList[i];

				it = valueList.find(keyStr);
				if (it == valueList.end())
				{
					valueList[keyStr] = new CStringArray;
					m_isReleaseBuffer = false;
				}
				
				valueList[keyStr]->Add(GetFieldValue(pRs, keyStr.GetBuffer()));
			}
			pRs->MoveNext();
		}
		pRs->Close();
		pConn->Close();
		bRet = true;
	}
	catch (_com_error & e)
	{
		_bstr_t strMsg = e.Description();
		errorStr = (LPCTSTR)strMsg;
	}

	return bRet;
}

bool CDataBaseHandle::ReleaseBuffer(map< CString, CStringArray* > &valueList)
{
	map< CString, CStringArray* > ::iterator it = valueList.begin();

	while (it != valueList.end())
	{
		CStringArray *arr = it->second;
		if (arr)
		{
			delete arr;
			arr = NULL;
			
		}
		it++;
	}

	m_isReleaseBuffer = true;
	valueList.clear();
	return true;
}

_ConnectionPtr CDataBaseHandle::GetConnectPtr()
{
	CString strConn;
	_bstr_t strCmd;
	_ConnectionPtr pConn;		//连接对象


								//open
	pConn.CreateInstance(__uuidof(Connection));
	pConn->ConnectionTimeout = m_params.timeOut;
	if (m_iDBType == DB_ORACLE)
	{
		strConn.Format("Provider=MSDAORA; Data Source=%s:1521/%s; "
			"User ID=%s; Password=%s", m_params.addressStr,m_params.name, m_params.usrName, m_params.passwordStr);
	}
	else if (m_iDBType == DB_MYSQL)
	{
		strConn.Format("Driver=MySQL ODBC 8.0 Unicode Driver; Server=%s; Database=%s; "
			"User ID=%s; Password=%s", m_params.addressStr, m_params.name, m_params.usrName, m_params.passwordStr);
	}
	else	//default is SQLSERVER
	{
		strConn.Format("Provider=SQLOLEDB; Data Source=%s; Initial Catolog=%s; "
			"User ID=%s; Password=%s", m_params.addressStr, m_params.name, m_params.usrName, m_params.passwordStr,m_params.timeOut);
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
		strCmd = "Use " + m_params.name;
		pConn->Execute(strCmd, NULL, adExecuteNoRecords);
	}
	return pConn;
}

int CDataBaseHandle::GetIntField(_RecordsetPtr pRs, _variant_t vtField)
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

CString CDataBaseHandle::GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize)
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
			strValue.Format(_T("%d"), vtValue.cVal);
		}
		else if (vtValue.vt == VT_UI1)
		{
			strValue.Format(_T("%d"), vtValue.bVal);
		}
		else if (vtValue.vt == VT_I2)
		{
			strValue.Format(_T("%d"), vtValue.iVal);
		}
		else if (vtValue.vt == VT_UI2)
		{
			strValue.Format(_T("%d"), vtValue.uiVal);
		}
		else if (vtValue.vt == VT_I4)
		{
			strValue.Format(_T("%d"), vtValue.lVal);
		}
		else if (vtValue.vt == VT_UI4)
		{
			strValue.Format(_T("%d"), vtValue.ulVal);
		}
		else if (vtValue.vt == VT_INT)
		{
			strValue.Format(_T("%d"), vtValue.intVal);
		}
		else if (vtValue.vt == VT_UINT)
		{
			strValue.Format(_T("%d"), vtValue.uintVal);
		}
		else if (vtValue.vt == VT_DECIMAL)
		{
			VarI4FromDec(&(vtValue.decVal), &lValue);
			strValue.Format(_T("%d"), lValue);
		}
		else if (vtValue.vt == VT_DATE)
		{
			COleDateTime tm = vtValue.date;
			strValue = (iReturnSize == 0) ? tm.Format(_T("%Y-%m-%d")) : tm.Format(_T("%Y-%m-%d %H:%M:%S"));
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

DATE CDataBaseHandle::GetDateField(_RecordsetPtr pRs, _variant_t vtField)
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


CDataBaseHandleGroup::CDataBaseHandleGroup()
{
	m_groupDBList.clear();
}

CDataBaseHandleGroup::~CDataBaseHandleGroup()
{
	DeletAllDB();
}

void CDataBaseHandleGroup::DeletAllDB()
{
	for (unsigned int i = 0; i < m_groupDBList.size(); ++i)
	{
		CDataBaseHandle *db = m_groupDBList[i];
		if (db)
		{
			delete db;
			db = NULL;
		}
	}
	m_groupDBList.clear();
}
