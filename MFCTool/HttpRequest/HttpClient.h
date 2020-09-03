
#ifndef __HTTPCLIENT_H__   
#define __HTTPCLIENT_H__   
  
//#pragma comment(lib, "lib/libssh2.lib")
//预编译宏：添加 CURL_STATICLIB（如果要openssl，就加 USE_OPENSSL）
//附加静态库：ws2_32.lib wldap32.lib winmm.lib Advapi32.lib libcurl.lib（libcurld.lib）；如果openssl，要加（libeay32.lib ssleay32.lib）


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
	static size_t WriteFunc(char *str, size_t size, size_t nmemb, void *stream);     //写入数据（回调函数）
	static size_t ProgressFunc(double* fileLen, double t, double d, double ultotal, double ulnow);   //下载进度

	BOOL CreateMultiDir(const char *pathName);
	double GetTotalFileLenth(const char *url);                     //获取将要下载的文件长度
	long GetLocalFileLenth(const char *fileName);                  //获取本地问价长度

public:  

    /** 
    * @brief HTTP POST请求 
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
    * @param strPost 输入参数,使用如下格式para1=val1?2=val2&… 
    * @param strResponse 输出参数,返回的内容 
    * @return 返回是否Post成功 
    */  
    int Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);  
  
	int Put(const std::string & strUrl, const std::string & strPost, std::string & strResponse);  

	int Puts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);

    /** 
    * @brief HTTP GET请求 
    * @param strUrl 输入参数,请求的Url地址,如:http://www.baidu.com 
    * @param strResponse 输出参数,返回的内容 
    * @return 返回是否Post成功 
    */  
    int Get(const std::string & strUrl, std::string & strResponse);  
  
    /** 
    * @brief HTTPS POST请求,无证书版本 
    * @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com 
    * @param strPost 输入参数,使用如下格式para1=val1?2=val2&… 
    * @param strResponse 输出参数,返回的内容 
    * @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性. 
    * @return 返回是否Post成功 
    */  
    int Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse,int &nStatueCode, vector<string > &headerList = vector<string>(), const char * pCaPath = NULL);
  
    /** 
    * @brief HTTPS GET请求,无证书版本 
    * @param strUrl 输入参数,请求的Url地址,如:https://www.alipay.com 
    * @param strResponse 输出参数,返回的内容 
    * @param pCaPath 输入参数,为CA证书的路径.如果输入为NULL,则不验证服务器端证书的有效性. 
    * @return 返回是否Post成功 
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
