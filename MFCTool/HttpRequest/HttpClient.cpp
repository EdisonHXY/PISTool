
#include "stdafx.h"
#include "HttpClient.h"
////////////////////////////////////////////////////////////////////////////////////////////
#include <string>   
#include <stdio.h>  
#include <io.h>
#include <windows.h>


char *CHttpClient::pRetBuf = NULL;
int CHttpClient::m_iRetPos = 0;

char *CHttpClient::pRetBuf_gets = NULL;
int CHttpClient::m_iRetPos_gets = 0;

CHttpClient::CHttpClient(void) :   
m_bDebug(false)  
{  
	pRetBuf = new char [1024 * 1024 * 2];
	m_iRetPos = 0;
	m_iRetPos_gets = 0;
	pRetBuf_gets = new char[1024 * 1024 * 2];
	curl_global_init(CURL_GLOBAL_DEFAULT);
}  
  
CHttpClient::~CHttpClient(void)  
{  
  if(pRetBuf != NULL)
	  delete pRetBuf;

  if (pRetBuf_gets != NULL)
  {
	  delete pRetBuf_gets;
  }

  curl_global_cleanup();
}  
  
static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)  
{  
    if(itype == CURLINFO_TEXT)  
    {  
        //printf("[TEXT]%s\n", pData);   
    }  
    else if(itype == CURLINFO_HEADER_IN)  
    {  
        printf("[HEADER_IN]%s\n", pData);  
    }  
    else if(itype == CURLINFO_HEADER_OUT)  
    {  
        printf("[HEADER_OUT]%s\n", pData);  
    }  
    else if(itype == CURLINFO_DATA_IN)  
    {  
        printf("[DATA_IN]%s\n", pData);  
    }  
    else if(itype == CURLINFO_DATA_OUT)  
    {  
        printf("[DATA_OUT]%s\n", pData);  
    }  
    return 0;  
}  
 

char *CHttpClient::GetRetValue( bool bisGets)
{
	return bisGets ? pRetBuf_gets: pRetBuf;
}



/*读取上传文件 */
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = (FILE *)stream;
	size_t n;
	
	if (ferror(f))
		return CURL_READFUNC_ABORT;
	
	n = fread(ptr, size, nmemb, f) * size;
	
	return n;
}


void CHttpClient::SetAuthentication(const CString &strUser, const CString &strPass)
{
	m_strUser.Format("AgentKey: %s", "MTIzIUAjZnNh");
	m_strPass.Format("AgentAccount: %s", "mpis");
}

size_t CHttpClient::OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)  
{ 
	memcpy(&pRetBuf[m_iRetPos], (char *)buffer, nmemb);
	m_iRetPos += nmemb;

    return nmemb;  
}  
  

size_t CHttpClient::OnWriteData_Gets(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	memcpy(&pRetBuf_gets[m_iRetPos_gets], (char *)buffer, nmemb);
	m_iRetPos_gets += nmemb;

	return nmemb;
}

int CHttpClient::Put(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
	curl_global_init(CURL_GLOBAL_ALL);

	CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
	
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
	


	struct curl_slist * headers = NULL; /* init to NULL is important */
	headers = curl_slist_append(headers, (LPCSTR)m_strUser);
	headers = curl_slist_append(headers, (LPCSTR)m_strPass);
	headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
	
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */
	
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	

    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);  

	curl_slist_free_all(headers);
	
    return res;  
}

int CHttpClient::Puts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string > &headerList /*= vector<string>()*/, const char * pCaPath /*= NULL*/)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if (NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}

	if (m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	if (NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM，所以无需设置，另外支持DER   
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");   
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}

	struct curl_slist * headers = NULL; /* init to NULL is important */

	headers = curl_slist_append(headers, "Accept: application/json");
	headers = curl_slist_append(headers, "Content-Type: application/json");
	for (int i = 0; i < headerList.size();++i)
	{
		string tmp = headerList[i];
		headers = curl_slist_append(headers, tmp.data());

	}

	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT"); /* !!! */

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	//curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);

	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if (res == CURLE_OK)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}
	curl_slist_free_all(headers);
	return res;
}

int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)  
{  
    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  

    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  

	//curl_easy_setopt(curl, CURLOPT_, 1);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  

	//strResponse =	gstrText;
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  

	struct curl_slist * headers = NULL; /* init to NULL is important */
	headers = curl_slist_append(headers, (LPCSTR)m_strUser);
	headers = curl_slist_append(headers, (LPCSTR)m_strPass);
	headers = curl_slist_append(headers, "Accept: application/json");
	//headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	headers = curl_slist_append(headers, "Content-Type:application/json");

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl);  
	curl_slist_free_all(headers);

    return res;  
}  
  
size_t WriteToString(void *buffer, size_t size, size_t nmemb, void *userp)
{
    *((std::string*)userp) += (char*)buffer;               // 不是说不是以0结尾的吗？这里用这种方式不会访问越界么？

    return size * nmemb;
}

int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)  
{ 
	m_iRetPos = 0;
	memset(pRetBuf, 0, 1024 * 1024 * 2);

    CURLcode res;  
	std::string strData;
    CURL *curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  

    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
	
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strData);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	
  //  strResponse.insert(gstrText.length(), &gstrText);
    /** 
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。 
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。 
    */  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3000);  


	struct curl_slist * headers = NULL; /* init to NULL is important */
	headers = curl_slist_append(headers, (LPCSTR)m_strUser);
	headers = curl_slist_append(headers, (LPCSTR)m_strPass);
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);  
    curl_easy_cleanup(curl); 
	curl_slist_free_all(headers);

    return res;  
}  
  

int CHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string> &headerList, const char * pCaPath)
{  
	m_iRetPos = 0;
	memset(pRetBuf, 0, 1024 * 1024 * 2);

    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  

    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  

	//设置头
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	//headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	headers = curl_slist_append(headers, "Content-Type:application/json");

	for (int i = 0 ; i < headerList.size();++i)
	{
		string tmp = headerList[i];
		headers = curl_slist_append(headers, tmp.data());

	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_POST, 1);  
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);  
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        //缺省情况就是PEM，所以无需设置，另外支持DER   
        //curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");   
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    }  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);  
    res = curl_easy_perform(curl);  

	if (res == CURLE_OK)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}
	

    curl_easy_cleanup(curl);  
	curl_slist_free_all(headers);
    return res;  
}  
  

int CHttpClient::Gets(const std::string & strUrl, std::string & strResponse, int &nStatueCode, vector<string > &headerList , const char * pCaPath)
{  
	m_iRetPos_gets = 0;
	memset(pRetBuf_gets, 0, 1024 * 1024 * 2);

    CURLcode res;  
    CURL* curl = curl_easy_init();  
    if(NULL == curl)  
    {  
        return CURLE_FAILED_INIT;  
    }  
    if(m_bDebug)  
    {  
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);  
        curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);  
    }  
    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());  
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData_Gets);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);  
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    if(NULL == pCaPath)  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);  
    }  
    else  
    {  
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);  
        curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);  
    }  
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);  
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3); 


// 	struct curl_slist *headers = NULL; /* init to NULL is important */
// 	headers = curl_slist_append(headers, (LPCSTR)m_strUser);
// 	headers = curl_slist_append(headers, (LPCSTR)m_strPass);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Accept: application/json");
	//headers = curl_slist_append(headers, "Content-Type:application/json;charset=UTF-8");
	headers = curl_slist_append(headers, "Content-Type:application/json");
	for (int i = 0; i < headerList.size();++i)
	{
		string tmp = headerList[i];
		headers = curl_slist_append(headers, tmp.data());

	}


	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);


    res = curl_easy_perform(curl);  

	if (res == CURLE_OK)
	{
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}

    curl_easy_cleanup(curl);  

	curl_slist_free_all(headers);

    return res;  
}  
  
///////////////////////////////////////////////////////////////////////////////////////////////   
  
void CHttpClient::SetDebug(bool bDebug)  
{  
    m_bDebug = bDebug;  
}  

double CHttpClient::GetTotalFileLenth(const char* url)
{
	double downloadFileLenth = 0;
	CURL* pCurl = curl_easy_init();

	curl_easy_setopt(pCurl, CURLOPT_URL, url);
	curl_easy_setopt(pCurl, CURLOPT_HEADER, 1L);
	curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1L);
	if(curl_easy_perform(pCurl) == CURLE_OK)
	{
		curl_easy_getinfo(pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else
	{
		downloadFileLenth = -1;
	}
	curl_easy_cleanup(pCurl);
	return downloadFileLenth;
}


BOOL CHttpClient::DownloadFile(const char *urlFile, char *szFile)
{
	if(urlFile == NULL || szFile == NULL)
		return FALSE;

	long localFileLen = GetLocalFileLenth(szFile);
	long lurlFileLen = GetTotalFileLenth(urlFile);

	if(localFileLen >= lurlFileLen)
		return TRUE;


	FILE* fp = fopen(szFile,"ab+");
	if(fp == NULL) 
	{
		return FALSE;
	}

	CURL* pCurl = curl_easy_init();
	curl_easy_setopt(pCurl, CURLOPT_URL, urlFile);
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 3000);
	curl_easy_setopt(pCurl, CURLOPT_HEADER, 0L);
	curl_easy_setopt(pCurl, CURLOPT_NOBODY, 0L);
	curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(pCurl, CURLOPT_RESUME_FROM, localFileLen);
	 		
	curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, WriteFunc);
	curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, fp);	
	curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
	curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, ProgressFunc);
	curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, &m_flCurDownloadLen);
		
	BOOL bDownload = curl_easy_perform(pCurl);
	fclose(fp);
	curl_easy_cleanup(pCurl);

	return bDownload;
}


BOOL CHttpClient::CreateMultiDir(const char *pathName)
{
	if(pathName == NULL) 
		return FALSE;

	char filePath[256] = {0};
	strcpy(filePath, pathName);

	int i = 0, pathLen = strlen(pathName);
	CString curPath;
	char curFilePath[256] = {0};
	WIN32_FIND_DATA swf;
	if(filePath[pathLen - 1] != '\\')	//最后一个非0字符不是‘\\’则加上
	{
		filePath[pathLen] = '\\';
	}
	while(filePath[i] != '\0')
	{
		if(filePath[i] == ':')
		{
			i+=2;
			continue;
		}
		if(filePath[i] == '\\')
		{
			memcpy(curFilePath, filePath, i);
			curFilePath[i] = '\0';
			curPath = curFilePath;
			if(FindFirstFile(curPath, &swf) == INVALID_HANDLE_VALUE) //目录不存在就创建
			{
				if(!CreateDirectory(curPath, NULL))
				{
					return FALSE;
				}
			}
		}
		i++;
	}
	return TRUE;	

}


size_t CHttpClient::WriteFunc(char *str, size_t size, size_t nmemb, void *stream)
{
	return fwrite(str, size, nmemb, (FILE*)stream);  
}

size_t CHttpClient::ProgressFunc(
								 double* pFileLen,
								 double t,// 下载时总大小  
								 double d, // 已经下载大小  
								 double ultotal, // 上传是总大小  
								 double ulnow)   // 已经上传大小  
{
	if(t == 0) 
		return 0;

	*pFileLen = d;
	return 0;
}


long CHttpClient::GetLocalFileLenth(const char* fileName)
{
	DWORD dwFileSize = 0;

	FILE* fp = fopen(fileName, "rb");
	if(fp != NULL)
	{
		dwFileSize = filelength(fileno(fp));
		fclose(fp);
	}
	return dwFileSize;
}

