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

//���ݿ����
typedef struct 
{
	CString name;			//���ݿ���
	CString usrName;	  //�û���
	CString passwordStr; //����
	CString addressStr;  //��ַ

	long timeOut; //���ӵĳ�ʱʱ��

}OC_DB_PARAMS;

//����̨����
enum WRITETPYE
{
	WRITETPYE_INFO, //������Ϣ
	WRITETPYE_WARN, //������Ϣ
	WRITETPYE_ERROR //������Ϣ
};




