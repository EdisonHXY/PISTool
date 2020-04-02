#pragma once
#include <vector>
using namespace std;

typedef enum OC_ERROR_CODE
{
	OC_ERROR_CODE_NONE, //没有错误
	OC_ERROR_CODE_PARAM, //参数错误
	OC_ERROR_CODE_FAIL, //出现错误
};

//配置文件INI
typedef struct 
{
	CString headerName;
	CString keyName;
	CString valueName;
	CString defaultName;
	
}OC_INI_INFO;
