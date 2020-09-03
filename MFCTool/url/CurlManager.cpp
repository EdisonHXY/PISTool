#include "stdafx.h"
#include "CurlManager.h"

CCurlManager::CCurlManager()
{
	m_pCurl = NULL;
	m_nPort = 80;
	m_hFile = INVALID_HANDLE_VALUE;
	m_pCallback = NULL;
	m_pCallbackParam = NULL;
	m_curlCode = CURLE_OK;
	m_lfFlag = Lf_None;
	m_curlList = NULL;

	m_pCurl = curl_easy_init();
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(m_pCurl, CURLOPT_WRITEDATA, this);

	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);//忽略证书检查
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
}


CCurlManager::~CCurlManager()
{
	ClearHeaderList();
	if (m_pCurl)
	{
		curl_easy_cleanup(m_pCurl);
	}

	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
}

bool CCurlManager::SetPort(LONG port)
{
	if (port == m_nPort)
		return true;
	m_nPort = port;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_PORT, m_nPort);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetTimeout(int nSecond)
{
	if (nSecond < 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_TIMEOUT, nSecond);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetConnectTimeout(int nSecond)
{
	if (nSecond < 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_CONNECTTIMEOUT, nSecond);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetUserAgent(LPCSTR lpAgent)
{
	if (NULL == lpAgent)
		return false;
	int nLen = strlen(lpAgent);
	if (nLen == 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_USERAGENT, lpAgent);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetResumeFrom(LONG lPos)
{
	if (lPos < 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM, lPos);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetResumeFromLarge(LONGLONG llPos)
{
	if (llPos < 0)
		return false;
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_RESUME_FROM_LARGE, llPos);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::AddHeader(LPCSTR lpKey, LPCSTR lpValue)
{
	if (lpKey == NULL || lpValue == NULL)
	{
		return false;
	}
	
	int nLen1 = strlen(lpKey), nLen2 = strlen(lpValue);
	if (nLen1 <=0 || nLen2 <=0)
	{
		return false;
	}
	string strHeader(lpKey);
	strHeader.append(": ");
	strHeader.append(lpValue);
	m_curlList = curl_slist_append(m_curlList, strHeader.c_str());
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_HTTPHEADER, m_curlList);
	return CURLE_OK == m_curlCode;
	
}

void CCurlManager::ClearHeaderList()
{
	if (m_curlList)
	{
		curl_slist_free_all(m_curlList);
		m_curlList = NULL;
	}
}

bool CCurlManager::SetCookie(LPCSTR lpCookie)
{
	if (lpCookie == NULL)
	{
		return false;
	}
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIE, lpCookie);
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::SetCookieFile(LPCSTR lpFilePath)
{
	if (lpFilePath == NULL)
	{
		return false;
	}
	m_curlCode = curl_easy_setopt(m_pCurl, CURLOPT_COOKIEFILE, lpFilePath);
	return CURLE_OK == m_curlCode;
}

const char* CCurlManager::GetError() const
{
	return curl_easy_strerror(m_curlCode);
}

void CCurlManager::SetCallback(fProgressCallBack pCallback, void* lpParam)
{
	m_pCallbackParam = lpParam;
	m_pCallback = pCallback;
}

bool CCurlManager::DownloadToFile(LPCSTR lpUrl, LPCSTR lpFile)
{
	if (lpUrl == NULL || lpFile == NULL)
	{
		return false;
	}
	CURLcode code = curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	DeleteFileA(lpFile);
	m_hFile = CreateFileA(lpFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		return FALSE;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_NOPROGRESS, 0);
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
	curl_easy_setopt(m_pCurl, CURLOPT_PROGRESSDATA, this);
	m_lfFlag = Lf_Download;
	//开始执行请求
	m_curlCode = curl_easy_perform(m_pCurl);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
	return CURLE_OK == m_curlCode;
}

bool CCurlManager::Post(LPCSTR lpUrl, LPCSTR lpData)
{
	if (lpUrl == NULL || lpData == NULL)
	{
		return false;
	}

	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, lpData);
	//curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, lpData);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	m_lfFlag = Lf_Post;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;

}

bool CCurlManager::Post(LPCSTR lpUrl, unsigned char* lpData, unsigned int nSize)
{
	if (lpData == NULL || lpUrl == NULL || nSize <=0)
	{
		return false;
	}

	curl_easy_setopt(m_pCurl, CURLOPT_POST, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDS, lpData);
	curl_easy_setopt(m_pCurl, CURLOPT_POSTFIELDSIZE, nSize);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	m_lfFlag = Lf_Post;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;
	
}

bool CCurlManager::Get(LPCSTR lpUrl)
{
	if (lpUrl == NULL)
	{
		return false;
	}
	curl_easy_setopt(m_pCurl, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(m_pCurl, CURLOPT_URL, lpUrl);
	curl_easy_setopt(m_pCurl, CURLOPT_FOLLOWLOCATION, 1);//支持重定向
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(m_pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
	m_lfFlag = Lf_Get;
	m_strRespons.clear();
	m_curlCode = curl_easy_perform(m_pCurl);
	return CURLE_OK == m_curlCode;

}

const string& CCurlManager::GetRespons() const
{
	return m_strRespons;
}

const char* CCurlManager::GetResponsPtr() const
{
	return m_strRespons.c_str();
}
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	curl_off_t nread;
	/* in real-world cases, this would probably get this data differently
	as this fread() stuff is exactly what the library already would do
	by default internally */
	size_t retcode = fread(ptr, size, nmemb, (FILE*)stream);

	nread = (curl_off_t)retcode;

	fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
		" bytes from file\n", nread);
	return retcode;
}
#include <io.h>
#include <sys/stat.h>
bool CCurlManager::FtpUpload(const FTP_PARAMS &params)
{
	
	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;

	/* get the file size of the local file */
	if (stat(params.localFilePath, &file_info)) {
		//printf("Couldn't open '%s': %s\n", params.localFilePath, strerror(errno));
		return false;
	}
	fsize = (curl_off_t)file_info.st_size;

	//char szLog[128] = { 0 };
	//sprintf_s(szLog, "Local file size: %d bytes \n", fsize);
	//printf(szLog);
	//printf("Local file size: %d" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

	/* get a FILE * of the same file */
	hd_src = fopen(params.localFilePath, "rb");
	if (hd_src == NULL)
	{
		return false;
	}
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
	bool bRet = false;
	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {

		char acUserPwd[128] = "";
		char acFtpForm[256] = "";
		sprintf_s(acFtpForm, "ftp://%s:%d/%s", params.ip, params.port, params.upLoadFileName);


		if (strlen(params.userName) >= 1)
		{
			sprintf_s(acUserPwd, "%s:%s", params.userName, params.password);
			curl_easy_setopt(curl, CURLOPT_USERPWD, acUserPwd);
		}

		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
		/* we want to use our own read function */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

		/* enable uploading */
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* specify target */
		curl_easy_setopt(curl, CURLOPT_URL, acFtpForm);

		/* pass in that last of FTP commands to run after the transfer */
	//	curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

		/* now specify which file to upload */
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

		/* Set the size of the file to upload (optional).  If you give a *_LARGE
		option you MUST make sure that the type of the passed-in argument is a
		curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
		make sure that to pass in a type 'long' argument. */
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
			(curl_off_t)fsize);

		/* Now run off and do what you've been told! */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
// 			fprintf(stderr, "curl_easy_perform() failed: %s\n",
// 				curl_easy_strerror(res));
			bRet = false;
		}
		else
		{
			bRet = true;
		}
			

		/* clean up the FTP commands list */
		//curl_slist_free_all(headerlist);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	fclose(hd_src); /* close the local file */

	curl_global_cleanup();

	return bRet;

}

bool CCurlManager::FtpUpload(const char *url, const FTP_PARAMS &params)
{

	CURL *curl;
	CURLcode res;
	FILE *hd_src;
	struct stat file_info;
	curl_off_t fsize;

	/* get the file size of the local file */
	if (stat(params.localFilePath, &file_info)) {
		printf("Couldn't open '%s': %s\n", params.localFilePath, strerror(errno));
		return false;
	}
	fsize = (curl_off_t)file_info.st_size;

	printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);

	/* get a FILE * of the same file */
	hd_src = fopen(params.localFilePath, "rb");
	if (hd_src == NULL)
	{
		return false;
	}
	/* In windows, this will init the winsock stuff */
	curl_global_init(CURL_GLOBAL_ALL);
	bool bRet = false;
	/* get a curl handle */
	curl = curl_easy_init();
	if (curl) {

// 		char acUserPwd[128] = "";
 		char acFtpForm[256] = "";
		sprintf_s(acFtpForm, "%s/%s", url, params.upLoadFileName);


// 		if (strlen(params.userName) >= 1)
// 		{
// 			sprintf_s(acUserPwd, "%s:%s", params.userName, params.password);
// 			curl_easy_setopt(curl, CURLOPT_USERPWD, acUserPwd);
// 		}

		//curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60);
		curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);
		/* we want to use our own read function */
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
		curl_easy_setopt(curl, CURLOPT_FTP_FILEMETHOD, CURLFTPMETHOD_NOCWD);
		/* enable uploading */
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

		/* specify target */
		curl_easy_setopt(curl, CURLOPT_URL, acFtpForm);

		/* pass in that last of FTP commands to run after the transfer */
		//	curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);

		/* now specify which file to upload */
		curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);

		/* Set the size of the file to upload (optional).  If you give a *_LARGE
		option you MUST make sure that the type of the passed-in argument is a
		curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
		make sure that to pass in a type 'long' argument. */
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
			(curl_off_t)fsize);

		/* Now run off and do what you've been told! */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
		{
			// 			fprintf(stderr, "curl_easy_perform() failed: %s\n",
			// 				curl_easy_strerror(res));
			bRet = false;
		}
		else
		{
			bRet = true;
		}


		/* clean up the FTP commands list */
		//curl_slist_free_all(headerlist);

		/* always cleanup */
		curl_easy_cleanup(curl);
	}
	fclose(hd_src); /* close the local file */

	curl_global_cleanup();

	return bRet;
}

bool CCurlManager::FtpDownload(const FTP_PARAMS &params)
{
	CURLcode code = curl_easy_setopt(m_pCurl, CURLOPT_URL, params.ip);
	char up[80] = { 0 };
	sprintf_s(up, "%s:%s", params.userName, params.password);
	curl_easy_setopt(m_pCurl, CURLOPT_USERPWD, up);

	DeleteFileA(params.localFilePath);
	m_hFile = CreateFileA(params.localFilePath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == m_hFile)
	{
		return FALSE;
	}

	m_lfFlag = Lf_Download;
	//开始执行请求
	m_curlCode = curl_easy_perform(m_pCurl);
	
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}

	if (m_curlCode != CURLE_OK)
	{
		DeleteFileA(params.localFilePath);
	}

	return CURLE_OK == m_curlCode;

}

size_t CCurlManager::WriteCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	//把下载到的数据以追加的方式写入文件(一定要有a，否则前面写入的内容就会被覆盖了)
	CCurlManager* pThis = (CCurlManager*)pParam;
	if (pThis == NULL)
	{
		return 0;
	}
	DWORD dwWritten = 0;
	switch (pThis->m_lfFlag)
	{
	case Lf_Download://下载
	{
						 if (pThis->m_hFile == INVALID_HANDLE_VALUE)
							 return 0;
						 if (!WriteFile(pThis->m_hFile, pBuffer, nSize*nMemByte, &dwWritten, NULL))
							 return 0;
	}
		break;
	case Lf_Post://Post数据
	case Lf_Get://Get数据
	{
					pThis->m_strRespons.append((const char*)pBuffer, nSize*nMemByte);
					dwWritten = nSize*nMemByte;
	}
		break;
	case Lf_None://未定义
		break;
	}
	return dwWritten;
}

size_t CCurlManager::HeaderCallback(void* pBuffer, size_t nSize, size_t nMemByte, void* pParam)
{
	CCurlManager* pThis = (CCurlManager*)pParam;
	return 0;
}

int CCurlManager::ProgressCallback(void *pParam, double dltotal, double dlnow, double ultotal, double ulnow)
{
	CCurlManager* pThis = (CCurlManager*)pParam;
	if (pThis == NULL)
	{
		return -1;
	}
	if (pThis->m_pCallback)
	{
		pThis->m_pCallback(pThis->m_pCallbackParam, dltotal, dlnow);
	}
	return 0;

}
