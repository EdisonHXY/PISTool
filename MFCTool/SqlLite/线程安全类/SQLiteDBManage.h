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

	BOOL InitDBAdo(const string &strSourceFile);					//初始化数据库连接设置，并打开数据库连接
	void CloseDB(void);						//关闭数据库连接

	BOOL QueryDBASCI(const CHAR *lpszSQL);	//执行数据库的查询操作,并记录结果
	BOOL Execute(const CHAR *lpszFormat, ...); 

	BOOL IsEOF(void);


	void GetFieldValueMap(DBRECORDMAP &dbMap,UINT &nRecordCount);

 	CString GetStringFieldValue(int iPos, const string& strField, DBRECORDMAP &recordMap);	//获取数据库中指定字段的值，并转换成字符串输出
 	int GetIntFieldValue(int iPos, const string& strField,  DBRECORDMAP &recordMap);			//获取数据库中指定字段的值，并转换成int类型输出
 	UINT GetUIntFieldValue(int iPos, const string& strField,  DBRECORDMAP &recordMap);			//获取数据库中指定字段的值，并转换成UInt类型输出
 	BOOL GetStringFieldValue(int iPos, const string& strField, char *szBuf,  DBRECORDMAP &recordMap);


	string UTF_82ASCII(const string &strUtf8Code);
	string ASCII2UTF_8(const string &strAsciiCode);

	std::wstring Utf82Unicode(const std::string& utf8string);		//UTF-8转Unicode 
	string WideByte2Acsi(const wstring& wstrcode);						//unicode 转为 ascii
	wstring Acsi2WideByte(const string& strascii);						//ascii 转 Unicode  
	std::string Unicode2Utf8(const std::wstring& widestring);		//Unicode 转 Utf8 


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
