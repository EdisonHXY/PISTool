#include "stdafx.h"
#include "RequestUrlTool.h"


CRequestUrlTool::CRequestUrlTool()
{
	
}


CRequestUrlTool::~CRequestUrlTool()
{
	if (m_curlHelper.IsAvailable())
	{
		m_curlHelper.curl_global_cleanup();
	}

}

bool CRequestUrlTool::InitData(const string & dllPath)
{
	//没有传入路径 加载运行目录里的dll
	string realPath = dllPath;
	if (dllPath.empty())
	{
		TCHAR szFilePath[MAX_PATH + 1];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);

		 _tcsrchr(szFilePath, '\\');
		(_tcsrchr(szFilePath, '\\'))[0] = 0;
		string dir  = szFilePath;
		realPath = dir + "\\libcurl.dll";
	}

	bool bRet = m_curlHelper.Load(realPath.data());
	if (bRet)
	{
		//初始化
		m_curlHelper.curl_global_init(CURL_GLOBAL_ALL);
		
	}
	return bRet;
}

bool CRequestUrlTool::FtpDownLoad(const FTP_PARAMS &params,int &errorCode, bool isSFtp)
{
	if (!CheckUrlStatus())
	{
		errorCode = -99;
		return false;
	}

	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, false);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, false);

	char szTmp[256] = { 0 };
	sprintf_s(szTmp, "%s://%s/%s",isSFtp ? "sftp":"ftp", params.ip, params.remoteFileName);
	CURLcode code = m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, szTmp);
	char up[80] = { 0 };
	sprintf_s(up, "%s:%s", params.userName, params.password);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_USERPWD, up);

	WriteParam writeP;
	strcpy(writeP.filename, params.localFilePath);
	writeP.nType = WriteType_FTPDown;

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &writeP);

	//开始执行请求
	errorCode = m_curlHelper.curl_easy_perform(pCurl);

	if (writeP.stream)
	{
		fclose(writeP.stream);
	}
	m_curlHelper.curl_easy_cleanup(pCurl);

	return CURLE_OK == errorCode;
}

bool CRequestUrlTool::FtpUpLoad(const FTP_PARAMS &params, int &errorCode,bool isSFtp)
{
	if (!CheckUrlStatus())
	{
		errorCode = -99;
		return false;
	}
	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, false);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, false);

	struct stat file_info;
	curl_off_t fsize;

	/* get the file size of the local file */
	if (stat(params.localFilePath, &file_info)) {
		printf("Couldn't open '%s': %s\n", params.localFilePath, strerror(errno));
		errorCode = -88;
		return false;
	}
	fsize = (curl_off_t)file_info.st_size;


	char acFtpForm[256] = "";
	sprintf_s(acFtpForm, "%s://%s/%s", isSFtp ? "sftp" : "ftp", params.ip,params.remoteFileName);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, acFtpForm);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
	
	char up[80] = { 0 };
	sprintf_s(up, "%s:%s", params.userName, params.password);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_USERPWD, up);

	WriteParam writeP;
	strcpy(writeP.filename, params.localFilePath);
	writeP.nType = WriteType_FTPUPL0AD;
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READDATA, &writeP);

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_INFILESIZE_LARGE,(curl_off_t)fsize);
	
	/* enable uploading */
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_UPLOAD, 1L);

	errorCode = m_curlHelper.curl_easy_perform(pCurl);

	if (writeP.stream)
	{
		fclose(writeP.stream);
	}
	m_curlHelper.curl_easy_cleanup(pCurl);

	return CURLE_OK == errorCode;
}

bool CRequestUrlTool::HttpGet(const std::string & strUrl, std::string & strResponse, int &nStatueCode,vector<string > &headerList, const char * pCaPath)
{
	strResponse.clear();
	nStatueCode = 0;
	if (!CheckUrlStatus())
	{
		nStatueCode = -99;
		return false;
	}

	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	struct curl_slist * headers = SetHttpParam(pCurl,headerList, pCaPath);


	//设置接收参数
	WriteParam wp;
	wp.nType = WriteType_HttpGet;
	char szTmpBuffer[4096] = { 0 };
	wp.szBuffer = szTmpBuffer;
	wp.szBufferPos = 0;
	 m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	 m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, NULL);
	 m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &wp);
	

	 CURLcode  res = m_curlHelper.curl_easy_perform(pCurl);
	 strResponse = szTmpBuffer;
	 if (res == CURLE_OK)
	 {
		 m_curlHelper.curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	 }
	 m_curlHelper.curl_slist_free_all(headers);
	 m_curlHelper.curl_easy_cleanup(pCurl);
	 return res == CURLE_OK;
}

bool CRequestUrlTool::HttpPost(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string > &headerList /*= vector<string>()*/, const char * pCaPath /*= NULL*/)
{
	strResponse.clear();
	nStatueCode = 0;
	if (!CheckUrlStatus())
	{
		nStatueCode = -99;
		return false;
	}

	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	struct curl_slist * headers = SetHttpParam(pCurl,headerList, pCaPath);

	//设置接收参数
	WriteParam wp;
	wp.nType = WriteType_HttpPost;
	char szTmpBuffer[4096] = { 0 };
	wp.szBuffer = szTmpBuffer;
	wp.szBufferPos = 0;
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, NULL);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &wp);

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_POST, 1);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, strPost.c_str());


	CURLcode  res = m_curlHelper.curl_easy_perform(pCurl);
	strResponse = szTmpBuffer;
	if (res == CURLE_OK)
	{
		m_curlHelper.curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}
	m_curlHelper.curl_slist_free_all(headers);
	m_curlHelper.curl_easy_cleanup(pCurl);

	return res == CURLE_OK;
}

bool CRequestUrlTool::HttpPut(const std::string & strUrl, const std::string & strPost, std::string & strResponse, int &nStatueCode, vector<string > &headerList /*= vector<string>()*/, const char * pCaPath /*= NULL*/)
{
	strResponse.clear();
	nStatueCode = 0;
	if (!CheckUrlStatus())
	{
		nStatueCode = -99;
		return false;
	}

	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	struct curl_slist * headers = SetHttpParam(pCurl,headerList, pCaPath);

	//设置接收参数
	WriteParam wp;
	wp.nType = WriteType_HttpPost;
	char szTmpBuffer[4096] = { 0 };
	wp.szBuffer = szTmpBuffer;
	wp.szBufferPos = 0;
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, NULL);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &wp);

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_CUSTOMREQUEST, "PUT");
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, strPost.c_str());


	CURLcode  res = m_curlHelper.curl_easy_perform(pCurl);
	strResponse = szTmpBuffer;
	if (res == CURLE_OK)
	{
		m_curlHelper.curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}
	m_curlHelper.curl_slist_free_all(headers);
	m_curlHelper.curl_easy_cleanup(pCurl);
	return res == CURLE_OK;
}

bool CRequestUrlTool::HttpDownLoad(const std::string & strUrl, const std::string & strSavePath, int &nStatueCode, vector<string > &headerList /*= vector<string>()*/, const char * pCaPath /*= NULL*/)
{
	
	nStatueCode = 0;
	if (!CheckUrlStatus())
	{
		nStatueCode = -99;
		return false;
	}

	CURL *pCurl = m_curlHelper.curl_easy_init();
	if (!pCurl)
	{
		errno = -88;
		return false;
	}
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, FileCallback);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, FileCallback);

	//设置请求头
	struct curl_slist * headers = NULL;
	for (int i = 0; i < headerList.size();++i)
	{
		string tmp = headerList[i];
		headers = m_curlHelper.curl_slist_append(headers, tmp.data());
	}

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);

	
	//设置证书
	if (NULL == pCaPath)
	{
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, false);
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM，所以无需设置，另外支持DER   
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");   
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, true);
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_CAINFO, pCaPath);
	}

	WriteParam writeP;
	strcpy(writeP.filename, strSavePath.data());
	writeP.nType = WriteType_HttpDownload;
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_URL, strUrl.c_str());
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, NULL);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &writeP);

	CURLcode  res = m_curlHelper.curl_easy_perform(pCurl);

	if (res == CURLE_OK)
	{
		m_curlHelper.curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &nStatueCode);
	}
	m_curlHelper.curl_slist_free_all(headers);

	if (writeP.stream)
	{
		fclose(writeP.stream);
	}

	m_curlHelper.curl_easy_cleanup(pCurl);

	return CURLE_OK == res;

}

size_t CRequestUrlTool::FileCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	WriteParam *param = (WriteParam*)pParam;
	size_t nRet = 0;
	

	switch (param->nType)
	{
	case  WriteType_FTPDown:
	case WriteType_HttpDownload:
		if (!param->stream)
		{
			param->stream = fopen(param->filename, "wb");
			if (!param->stream)
				return -1;
		}
		nRet = fwrite(pBuffer, nSize, nMemByte, param->stream);
		break;
	case WriteType_FTPUPL0AD:
		if (!param->stream)
		{
			param->stream = fopen(param->filename, "rb");
			if (!param->stream)
				return -1;
		}
		nRet = fread(pBuffer, nSize, nMemByte, param->stream);
		break;
	case WriteType_HttpGet:
	case WriteType_HttpPost:

		memcpy(&param->szBuffer[param->szBufferPos], (char *)pBuffer, nMemByte);
		param->szBufferPos += nMemByte;
		nRet = nMemByte;
		break;

	default:
		break;
	}
	
	return nRet;
}

bool CRequestUrlTool::CheckUrlStatus()
{
	return m_curlHelper.IsAvailable();
}

struct curl_slist *  CRequestUrlTool::SetHttpParam(CURL*pCurl, vector<string > &headerList, const char * pCaPath)
{
	//设置请求头
	struct curl_slist * headers = NULL;
	for (int i = 0; i < headerList.size();++i)
	{
		string tmp = headerList[i];
		headers = m_curlHelper.curl_slist_append(headers, tmp.data());
	}

	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, headers);


	//设置证书
	if (NULL == pCaPath)
	{
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, false);
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM，所以无需设置，另外支持DER   
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");   
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, true);
		m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_CAINFO, pCaPath);
	}

	//设置超时
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_NOSIGNAL, 1);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 3);
	m_curlHelper.curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 3);

	return headers;

}

