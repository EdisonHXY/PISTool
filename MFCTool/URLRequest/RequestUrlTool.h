#pragma once
/*!
 * \class CRequestUrlTool 
 *
 * \brief ��װLibCurl ����ʹ��
 *
 * \author ��ѩԴ
 * \date ���� 2020
 */
#include "curl/CurlHelper.h"
#include <string>
#include <vector>
using namespace std;

class CRequestUrlTool
{
public:
	typedef struct __FTP_PARAMS
	{
		char userName[50];
		char password[50];
		char ip[50];
		int port;
		char localFilePath[256];
		char remoteFileName[128];

		__FTP_PARAMS()
		{
			memset(userName, 0, 50);
			memset(password, 0, 50);
			memset(ip, 0, 50);
			memset(localFilePath, 0, 256);
			memset(remoteFileName, 0, 128);
			port = 0;
		}

	}FTP_PARAMS;


public:
	CRequestUrlTool();
	~CRequestUrlTool();

	/**
	    *  @date        2020/09/14 15:54
	    *  @brief       ��ʼ������ ����libcurl.dll��    
		*  Ĭ�ϲ����Ǽ��ر���Ŀ¼�µ�libcurl.dll 
	 **/
	bool InitData(const string &dllPath = string(""));
	/**
	    *  @date        2020/09/14 17:26
	    *  @brief       FTP����     
	 **/
	bool FtpDownLoad(const FTP_PARAMS &ftpParam,int &errorCode,bool isSFtp = false);
	/**
	    *  @date        2020/09/14 17:26
	    *  @brief       FTP�ϴ�     
	 **/
	bool FtpUpLoad(const FTP_PARAMS &ftpParam, int &errorCodebool, bool isSFtp = false);

	/**
	    *  @date        2020/09/15 09:19
	    *  @brief       Get      
		* headerList ������� ��Accept: application/json��ͷ��
		* pCaPath      ����httpsʱ ����֤��ĵ�ַ��Ĭ���ǲ���֤֤��
	 **/
	bool HttpGet(const std::string & strUrl, std::string & strResponse, int &nStatueCode,vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

	/**
	*  @date        2020/09/15 09:19
	*  @brief       Post
	**/
	bool HttpPost(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode,vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

	/**
	*  @date        2020/09/15 09:19
	*  @brief       Put
	**/
	bool HttpPut(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode,vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

	/**
	*  @date        2020/09/15 09:19
	*  @brief       �����ļ�
	**/
	bool HttpDownLoad(const std::string & strUrl,const std::string & strSavePath, int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);


private:
	enum WriteType
	{
		WriteType_UnKnow,
		WriteType_FTPDown,
		WriteType_FTPUPL0AD,
		WriteType_HttpGet,
		WriteType_HttpPost,
		WriteType_HttpDownload
	};

	//�ص�ʹ�õĲ���
	typedef struct __WriteParam
	{
		char filename[256]; 
		FILE *stream; 
		WriteType nType;
		char *szBuffer; //http ����ʱʹ��
		int szBufferPos;
		__WriteParam()
		{
			memset(filename, 0, 256);
			stream = NULL;
			nType = WriteType_UnKnow;
			szBuffer = NULL;
			szBufferPos = 0;
		}

	}WriteParam;


private:
	CCurlHelper m_curlHelper; //dll
	static size_t	FileCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam);
	bool CheckUrlStatus();
	struct curl_slist *  SetHttpParam(CURL*pCurl, vector<string > &headerList, const char * pCaPath); //����ͳһ�Ĳ���
};

