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

//数据库参数
typedef struct 
{
	CString name;			//数据库名
	CString usrName;	  //用户名
	CString passwordStr; //密码
	CString addressStr;  //地址

	long timeOut; //连接的超时时间

}OC_DB_PARAMS;

//控制台类型
enum WRITETPYE
{
	WRITETPYE_INFO, //正常信息
	WRITETPYE_WARN, //警告信息
	WRITETPYE_ERROR //错误信息
};




