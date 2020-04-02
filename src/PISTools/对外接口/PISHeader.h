#pragma once
#include <vector>
using namespace std;

typedef enum OC_ERROR_CODE
{
	OC_ERROR_CODE_NONE, //û�д���
	OC_ERROR_CODE_PARAM, //��������
	OC_ERROR_CODE_FAIL, //���ִ���
};

//�����ļ�INI
typedef struct 
{
	CString headerName;
	CString keyName;
	CString valueName;
	CString defaultName;
	
}OC_INI_INFO;
