#pragma once

#pragma pack(1)

#define VSZIP_EXPORT extern "C" _declspec(dllexport)

enum VSZ_CODE
{
	VSZ_OK = 0,
	VSZ_SRC_ERROR,
	VSZ_DST_ERROR,
	VSZ_MEM_ERROR,
};

typedef struct
{
	char szFile[MAX_PATH];
	ULONGLONG ullFileSize;
	FILETIME  ftEditTime;
	ULONGLONG ullStartPos;
	ULONGLONG ullStopPos;

}	FILE_DESC, * PFILE_DESC;

//ѹ�����
typedef void * HVSZ;

//���Ȼص�����
//iPercent: 0��100
typedef void (* lpProgressFunc)(LPVOID pVoid, const char * szFile, int iPercent);

#pragma pack()

//ѹ��һ��Ŀ¼�������szOutFile�ļ�
//szPath����Ҫѹ����Ŀ¼������·��
//szOutFile��ѹ������ļ�����·�������ΪNULL����ѹ������ļ�λ��szPath��ͬ��Ŀ¼�£���չ��Ϊvsz
//����0����ʾѹ���ɹ��������ʾ�������
VSZIP_EXPORT int  VS_Compress(const char * szPath, const char * szOutFile, lpProgressFunc ProgressFunc, LPVOID pVoid);

//ѹ�������ļ��������szOutFile�ļ�
//szInFile����Ҫѹ���ĵ����ļ�������·��
//szOutFile��ѹ������ļ�����·�������ΪNULL����ѹ������ļ�λ��szInFile��ͬ��Ŀ¼�£���չ��Ϊvsz
//����0����ʾѹ���ɹ��������ʾ�������
VSZIP_EXPORT int  VS_CompressFile(const char * szInFile, const char * szOutFile, lpProgressFunc ProgressFunc, LPVOID pVoid);

//��ѹ��һ���ļ��������szOutPathĿ¼
//szInFile����Ҫ��ѹ�����ļ�������·��
//szOutPath�����Ŀ¼�����ΪNULL��ֱ��ʹ��ѹ���ļ����ڵ�Ŀ¼
//����0����ʾ��ѹ���ɹ��������ʾ�������
VSZIP_EXPORT int  VS_Decompress(const char * szInFile, const char * szOutPath, lpProgressFunc ProgressFunc, LPVOID pVoid);

//��ʼ��ѹ���ļ��ڵ��ļ��б��Ի�ȡ�ļ��б�
//szInFile��ѹ���ļ�������·��
//iFileNum������ѹ���ļ��ڵ��ļ�����
//����ѹ����������������ȡ�ļ��б�����NULL����ʾʧ��
VSZIP_EXPORT HVSZ VS_ParseVSZ(const char * szInFile, int & iFileNum);
//��ȡѹ���ļ��ڰ������ļ���
//hVSZ����Ҫ�ͷŵ�ѹ�����
//iIndex����Ҫ��ȡ�ļ������ļ���ţ���0��ʼ��һֱ����
//pFileDesc��ѹ���ļ���Ԫ����Ϣ�������ļ�����ԭʼ�ļ���С��ѹ���ļ���С
//����true����ʾ��ȡ�ɹ��������ʾ�Ѿ�û���ļ�
VSZIP_EXPORT BOOL VS_GetNextFile(HVSZ hVSZ, int iIndex, PFILE_DESC pFileDesc);
//�ͷ�ѹ�����
//hVSZ����Ҫ�ͷŵ�ѹ�����
VSZIP_EXPORT void VS_FreeVSZ(HVSZ hVSZ);

//�����ļ���md5
//szInFile����Ҫ����MD5���ļ���
//szMD5���������õ�32���ֽڵ�MD5��
//����true��ʾ�ɹ�������ʧ��
VSZIP_EXPORT BOOL VS_GetMD5(const char * szInFile, char szMD5[36]);
