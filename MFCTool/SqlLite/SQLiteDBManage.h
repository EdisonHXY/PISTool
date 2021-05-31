/***********************************************************************************
*	��Ȩ����(c) 2016 �ڻ���������Ƽ����޹�˾
*	�ļ�����:	SQLiteDBManage.h
*	�ļ�˵��:	SQLite�������ݿ���������ļ�
*	��ǰ�汾:	1.0.20160404
*	����:		��Ρ��
*	��������:	2016-04-04
*************************************************************************************/
#ifndef	_SQLITEDBMANAGE_H
#define	_SQLITEDBMANAGE_H
#define SQLITE_HAS_CODEC

#define PISDB  "PisDB.db"

#pragma warning(disable : 4786)

#include <iostream> 
#include <vector>
#include "sqlite3.h"  
#include "../MfcUI/ThreadCheck.h"

using namespace std;  

class CSQLiteDBManage  
{
public:
	typedef struct RECORD
	{
		char szField[MAX_PATH];
		vector<string> verFieldValue;
		
	} RECORD, *LPRECORD;
	
	CSQLiteDBManage(void);
	virtual ~CSQLiteDBManage(void);

	void InitDBParam(CString strSourceFile, CString strPass);
	
	void SetPass(char *szPass);
	BOOL InitDBAdo(void);					//��ʼ�����ݿ��������ã��������ݿ�����
	void CloseDB(void);						//�ر����ݿ�����

	BOOL QueryDB(CString strSql);	//ִ�����ݿ�Ĳ�ѯ����,����¼���
	BOOL QueryDBASCI(const CHAR *lpszSQL);	//ִ�����ݿ�Ĳ�ѯ����,����¼���
	BOOL QueryDB(const CHAR *lpszFormat, ...);
	BOOL Execute(const CString &strSql);
	BOOL Execute(const CHAR *lpszFormat, ...); 

	BOOL IsEOF(void);
	void ResMoveNext(void);
	void ResMoveFirst(void);
	void ResMoveLast(void);
	void ResMovePrevious(void);

	CString GetStringFieldValue(CString strField); //��ȡ���ݿ���ָ���ֶε�ֵ����ת�����ַ������
	int GetIntFieldValue(CString strField);			//��ȡ���ݿ���ָ���ֶε�ֵ����ת����int�������
	UINT GetUIntFieldValue(CString strField);		//��ȡ���ݿ���ָ���ֶε�ֵ����ת����UInt�������
	BOOL GetStringFieldValue(CString strField, char *szBuf);
	
	CString GetStringFieldValue(int iPos, CString strField);	//��ȡ���ݿ���ָ���ֶε�ֵ����ת�����ַ������
	int GetIntFieldValue(int iPos, CString strField);			//��ȡ���ݿ���ָ���ֶε�ֵ����ת����int�������
	UINT GetUIntFieldValue(int iPos, CString strField);			//��ȡ���ݿ���ָ���ֶε�ֵ����ת����UInt�������
	BOOL GetStringFieldValue(int iPos, CString strField, char *szBuf);


	void GetAllSchemaName(CStringArray *strArray);   //��ѯ���ݿ����û����������б������
	long GetFieldCount(void);						//�� ȡ��ѯ���������¼���ı����ֶε�����
	int GetRecordsetCount(void);					//��ȡ��ѯ�����¼���м�¼������
	

	string UTF_82ASCII(string &strUtf8Code);		
	string ASCII2UTF_8(string &strAsciiCode);

	std::wstring Utf82Unicode(const std::string& utf8string);		//UTF-8תUnicode 
	string WideByte2Acsi(wstring& wstrcode);						//unicode תΪ ascii
	wstring Acsi2WideByte(string& strascii);						//ascii ת Unicode  
	std::string Unicode2Utf8(const std::wstring& widestring);		//Unicode ת Utf8 


protected:
	static int _sql_callback(void *param, int argc, char **argv, char **szColName);
	void RecvExecuteRet(int argc, char **argv, char **szColName);
	void ClearFieldValue(void);
	void AddRecordValue(const char *szColName, const char *szValue);
	UINT String2UINT(const string &strValue);

	void Lock_Start(void);
	void Lock_End(void);

private:
	string m_strDBFileName;
	sqlite3 *m_pSQLiteConn;

	int m_iRecordPos;
	vector<LPRECORD> m_verFieldValue;

	CMyLock m_mutexProcess;

};


#endif 
