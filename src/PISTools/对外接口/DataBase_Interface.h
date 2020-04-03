#pragma once

#include <map>

//设置数据库地址 需要有序的进行设置
DllExport int OC_SetDataBaseParams(int nIndex, OC_DB_PARAMS params);

//销毁所有创建的数据库类
DllExport void OC_DeleteAllDataBase();

//执行 增 删 改 语句
DllExport bool OC_ExectSql(int nIndex, CString sqlStr, CString &errorStr);

//执行查语句 执行完之后 必须配合OC_ReleaseBuffer进行释放内存，否则下次执行会给给予内存错误
DllExport bool OC_ExectSelectSql(int nIndex, CString sqlStr, const CStringArray &keyList, map< CString, CStringArray* > &valueList, CString &errorStr);

//释放内存
DllExport bool OC_ReleaseBuffer(int nIndex, map< CString, CStringArray* > &valueList);