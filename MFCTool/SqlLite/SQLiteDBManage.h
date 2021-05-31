/***********************************************************************************
*	版权所有(c) 2016 冠华天视数码科技有限公司
*	文件名称:	SQLiteDBManage.h
*	文件说明:	SQLite操作数据库的类声明文件
*	当前版本:	1.0.20160404
*	作者:		贾巍云
*	创建日期:	2016-04-04
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
	BOOL InitDBAdo(void);					//初始化数据库连接设置，并打开数据库连接
	void CloseDB(void);						//关闭数据库连接

	BOOL QueryDB(CString strSql);	//执行数据库的查询操作,并记录结果
	BOOL QueryDBASCI(const CHAR *lpszSQL);	//执行数据库的查询操作,并记录结果
	BOOL QueryDB(const CHAR *lpszFormat, ...);
	BOOL Execute(const CString &strSql);
	BOOL Execute(const CHAR *lpszFormat, ...); 

	BOOL IsEOF(void);
	void ResMoveNext(void);
	void ResMoveFirst(void);
	void ResMoveLast(void);
	void ResMovePrevious(void);

	CString GetStringFieldValue(CString strField); //获取数据库中指定字段的值，并转换成字符串输出
	int GetIntFieldValue(CString strField);			//获取数据库中指定字段的值，并转换成int类型输出
	UINT GetUIntFieldValue(CString strField);		//获取数据库中指定字段的值，并转换成UInt类型输出
	BOOL GetStringFieldValue(CString strField, char *szBuf);
	
	CString GetStringFieldValue(int iPos, CString strField);	//获取数据库中指定字段的值，并转换成字符串输出
	int GetIntFieldValue(int iPos, CString strField);			//获取数据库中指定字段的值，并转换成int类型输出
	UINT GetUIntFieldValue(int iPos, CString strField);			//获取数据库中指定字段的值，并转换成UInt类型输出
	BOOL GetStringFieldValue(int iPos, CString strField, char *szBuf);


	void GetAllSchemaName(CStringArray *strArray);   //查询数据库中用户创建的所有表的名称
	long GetFieldCount(void);						//获 取查询结果表明记录集的表中字段的总数
	int GetRecordsetCount(void);					//获取查询结果记录集中记录的总数
	

	string UTF_82ASCII(string &strUtf8Code);		
	string ASCII2UTF_8(string &strAsciiCode);

	std::wstring Utf82Unicode(const std::string& utf8string);		//UTF-8转Unicode 
	string WideByte2Acsi(wstring& wstrcode);						//unicode 转为 ascii
	wstring Acsi2WideByte(string& strascii);						//ascii 转 Unicode  
	std::string Unicode2Utf8(const std::wstring& widestring);		//Unicode 转 Utf8 


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
