#pragma once

#include <map>

//�������ݿ��ַ ��Ҫ����Ľ�������
DllExport int OC_SetDataBaseParams(int nIndex, OC_DB_PARAMS params);

//�������д��������ݿ���
DllExport void OC_DeleteAllDataBase();

//ִ�� �� ɾ �� ���
DllExport bool OC_ExectSql(int nIndex, CString sqlStr, CString &errorStr);

//ִ�в���� ִ����֮�� �������OC_ReleaseBuffer�����ͷ��ڴ棬�����´�ִ�л�������ڴ����
DllExport bool OC_ExectSelectSql(int nIndex, CString sqlStr, const CStringArray &keyList, map< CString, CStringArray* > &valueList, CString &errorStr);

//�ͷ��ڴ�
DllExport bool OC_ReleaseBuffer(int nIndex, map< CString, CStringArray* > &valueList);