#pragma once
#include "DllParser.h"

#include "curl.h"

class CCurlHelper
{
public:
	CCurlHelper()
	{
		m_bInit = false;
	}
	~CCurlHelper()
	{
		m_dllParser.UnLoad();
	}

	//加载DLL
	bool Load(const char* strDllPath)
	{
		this->m_bInit = m_dllParser.Load(strDllPath);
		return this->m_bInit;
	}

	//判断DLL是否可用
	bool IsAvailable()
	{
		return this->m_bInit;
	}


	CURLcode curl_global_init(long flags)
	{
		auto fun = m_dllParser.GetFunction<CURLcode(long)>("curl_global_init");
		return fun(flags);
	}
	void curl_global_cleanup(void)
	{
		auto fun = m_dllParser.GetFunction<void()>("curl_global_cleanup");
		return fun();
	}
	void * curl_easy_init()
	{
		auto fun = m_dllParser.GetFunction<void *()>("curl_easy_init");
		return fun();
	}

	void curl_easy_cleanup(void *curl)
	{
		auto fun = m_dllParser.GetFunction<void (void*)>("curl_easy_cleanup");
		return fun(curl);
	}
	template<class T>
	CURLcode curl_easy_setopt(void *curl, CURLoption option, T lparam)
	{
		
		auto fun = m_dllParser.GetFunction<CURLcode(void*, CURLoption,T)>("curl_easy_setopt");
		return fun(curl, option,lparam);
	}

	CURLcode curl_easy_perform(void *curl)
	{
		auto fun = m_dllParser.GetFunction<CURLcode(void*)>("curl_easy_perform");
		return fun(curl);
	}
	struct curl_slist *curl_slist_append(struct curl_slist * s,
		const char *t)
	{
		auto fun = m_dllParser.GetFunction<struct curl_slist *(struct curl_slist *, const char *)>("curl_slist_append");
		return fun(s,t);
	}
	template<class T>
	CURLcode curl_easy_getinfo(CURL *curl, CURLINFO info, T lparam)
	{
		auto fun = m_dllParser.GetFunction<CURLcode(CURL *, CURLINFO, T)>("curl_easy_getinfo");
		return fun(curl, info, lparam);
	}
	void curl_slist_free_all(struct curl_slist * s)
	{
		auto fun = m_dllParser.GetFunction<void(struct curl_slist *)>("curl_slist_free_all");
		return fun(s);
	}

	void curl_easy_reset(CURL *curl)
	{
		auto fun = m_dllParser.GetFunction<void(CURL *)>("curl_easy_reset");
		return fun(curl);
	}

private:
	DllParser m_dllParser;
	bool m_bInit;

};

