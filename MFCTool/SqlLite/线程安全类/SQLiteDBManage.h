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
#include <map>
#include "sqlite3.h"  
#include "../MfcUI/ThreadCheck.h"

using namespace std;  
typedef map<string, vector<string> > DBRECORDMAP;
class CSQLiteDBManage  
{
public:
	
	CSQLiteDBManage(void);
	virtual ~CSQLiteDBManage(void);

	BOOL InitDBAdo(const string &strSourceFile);					//��ʼ�����ݿ��������ã��������ݿ�����
	void CloseDB(void);						//�ر����ݿ�����

	BOOL QueryDBASCI(const CHAR *lpszSQL);	//ִ�����ݿ�Ĳ�ѯ����,����¼���
	BOOL Execute(const CHAR *lpszFormat, ...); 

	BOOL IsEOF(void);


	void GetFieldValueMap(DBRECORDMAP &dbMap,UINT &nRecordCount);

 	CString GetStringFieldValue(int iPos, const string& strField, DBRECORDMAP &recordMap);	//��ȡ���ݿ���ָ���ֶε�ֵ����ת�����ַ������
 	int GetIntFieldValue(int iPos, const string& strField,  DBRECORDMAP &recordMap);			//��ȡ���ݿ���ָ���ֶε�ֵ����ת����int�������
 	UINT GetUIntFieldValue(int iPos, const string& strField,  DBRECORDMAP &recordMap);			//��ȡ���ݿ���ָ���ֶε�ֵ����ת����UInt�������
 	BOOL GetStringFieldValue(int iPos, const string& strField, char *szBuf,  DBRECORDMAP &recordMap);


	string UTF_82ASCII(const string &strUtf8Code);
	string ASCII2UTF_8(const string &strAsciiCode);

	std::wstring Utf82Unicode(const std::string& utf8string);		//UTF-8תUnicode 
	string WideByte2Acsi(const wstring& wstrcode);						//unicode תΪ ascii
	wstring Acsi2WideByte(const string& strascii);						//ascii ת Unicode  
	std::string Unicode2Utf8(const std::wstring& widestring);		//Unicode ת Utf8 


protected:
	static int _sql_callback(void *param, int argc, char **argv, char **szColName);
	void RecvExecuteRet(int argc, char **argv, char **szColName);
	void ClearFieldValue(void);
	void AddRecordValue(const char *szColName, const char *szValue);
	UINT String2UINT(const string &strValue);



private:
	sqlite3 *m_pSQLiteConn;

	DBRECORDMAP m_verFieldValue;
	CMyLock m_mutexProcess;


};


#endif 
