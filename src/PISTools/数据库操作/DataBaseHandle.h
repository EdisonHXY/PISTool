#pragma once
#pragma warning(disable:4146)
#import "C:/Program Files/Common Files/System/ADO/msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;

#include <map>

class CDataBaseHandle
{
	//���ݿ�����
	enum DB_TYPE_
	{
		DB_SQLSERVER = 0,
		DB_ORACLE,
		DB_MYSQL,
	};

public:
	CDataBaseHandle(int tag);
	~CDataBaseHandle();

	//���ò���
	void SetDataBaseParams(OC_DB_PARAMS params);

	//��ȡ��ʶ
	void GetTag(int &tag);
	
	//ִ�� �� ɾ �� ���
	bool ExectSql(CString sqlStr,CString &errorStr);

	//ִ�в����
	bool ExectSql(CString sqlStr,const CStringArray &keyList,map< CString, CStringArray *> &valueList,CString &errorStr);

	//�ͷ��ڴ�
	bool ReleaseBuffer(map< CString, CStringArray* > &valueList);


private:
	OC_DB_PARAMS m_params;
	int m_tag;
	DB_TYPE_ m_iDBType;
	bool m_isReleaseBuffer;
private:
	_ConnectionPtr GetConnectPtr();
	int GetIntField(_RecordsetPtr pRs, _variant_t vtField);
	CString GetFieldValue(_RecordsetPtr pRs, _variant_t vtField, int iReturnSize = 0);
	DATE GetDateField(_RecordsetPtr pRs, _variant_t vtField);

};


class CDataBaseHandleGroup
{
public:
	CDataBaseHandleGroup();
	~CDataBaseHandleGroup();

public:
	vector<CDataBaseHandle*> m_groupDBList; //�������д��������ݿ���
	void DeletAllDB();
};
