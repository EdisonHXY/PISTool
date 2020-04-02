#pragma once

#include <map>

//设置数据库地址 需要有序的进行设置
DllExport int OC_SetDataBaseParams(int nIndex, OC_DB_PARAMS params);

//销毁所有创建的数据库类
DllExport void OC_DeleteAllDataBase();

//执行 增 删 改 语句
DllExport bool OC_ExectSql(int nIndex, CString sqlStr, CString &errorStr);

//执行查语句
DllExport bool OC_ExectSelectSql(int nIndex, CString sqlStr, const CStringArray &keyList, map< CString, CStringArray > &valueList, CString &errorStr);

