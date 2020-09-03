
#ifndef __HTTPCLIENT_H__   
#define __HTTPCLIENT_H__   
  
//#pragma comment(lib, "lib/libssh2.lib")
//Ԥ����꣺��� CURL_STATICLIB�����Ҫopenssl���ͼ� USE_OPENSSL��
//���Ӿ�̬�⣺ws2_32.lib wldap32.lib winmm.lib Advapi32.lib libcurl.lib��libcurld.lib�������openssl��Ҫ�ӣ�libeay32.lib ssleay32.lib��


#include "libcurl/Curl/x86/include/curl/curl.h"
#pragma comment(lib, "./HttpRequest/libcurl/Curl/x86/lib/libcurl.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "winmm.lib" )
#pragma comment ( lib, "wldap32.lib" )
 
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;  

class CHttpClient  
{  
public:  
    CHttpClient(void);  
    virtual ~CHttpClient(void);  
	void SetAuthentication(const CString &strUser, const CString &strPass);
	static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid);
	static size_t OnWriteData_Gets(void* buffer, size_t size, size_t nmemb, void* lpVoid);
	char *GetRetValue(bool bisGets = false);
	BOOL DownloadFile(const char *urlFile, char *szFile);

protected:
	static size_t WriteFunc(char *str, size_t size, size_t nmemb, void *stream);     //д�����ݣ��ص�������
	static size_t ProgressFunc(double* fileLen, double t, double d, double ultotal, double ulnow);   //���ؽ���

	BOOL CreateMultiDir(const char *pathName);
	double GetTotalFileLenth(const char *url);                     //��ȡ��Ҫ���ص��ļ�����
	long GetLocalFileLenth(const char *fileName);                  //��ȡ�����ʼ۳���

public:  

    /** 
    * @brief HTTP POST���� 
    * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com 
    * @param strPost �������,ʹ�����¸�ʽpara1=val1?2=val2&�� 
    * @param strResponse �������,���ص����� 
    * @return �����Ƿ�Post�ɹ� 
    */  
    int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);  
  
	int Put(const std::string & strUrl, const std::string & strPost, std::string & strResponse);  

	int Puts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

    /** 
    * @brief HTTP GET���� 
    * @param strUrl �������,�����Url��ַ,��:http://www.baidu.com 
    * @param strResponse �������,���ص����� 
    * @return �����Ƿ�Post�ɹ� 
    */  
    int Get(const std::string & strUrl, std::string & strResponse);  
  
    /** 
    * @brief HTTPS POST����,��֤��汾 
    * @param strUrl �������,�����Url��ַ,��:https://www.alipay.com 
    * @param strPost �������,ʹ�����¸�ʽpara1=val1?2=val2&�� 
    * @param strResponse �������,���ص����� 
    * @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��. 
    * @return �����Ƿ�Post�ɹ� 
    */  
    int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse,int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);
  
    /** 
    * @brief HTTPS GET����,��֤��汾 
    * @param strUrl �������,�����Url��ַ,��:https://www.alipay.com 
    * @param strResponse �������,���ص����� 
    * @param pCaPath �������,ΪCA֤���·��.�������ΪNULL,����֤��������֤�����Ч��. 
    * @return �����Ƿ�Post�ɹ� 
    */  
    int Gets(const std::string & strUrl, std::string & strResponse, int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

  
public:  
    void SetDebug(bool bDebug);  	
  
private:  
    bool m_bDebug;  
	//static std::string m_strRetJson;
	static char *pRetBuf;
	static int m_iRetPos;
	CString m_strUser, m_strPass;
	double m_flCurDownloadLen;

	static char *pRetBuf_gets;
	static int m_iRetPos_gets;

};  


#endif
