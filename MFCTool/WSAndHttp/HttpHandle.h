#pragma once

#include "ServerHeader.h"

class CHttpHandle
{
public:
	CHttpHandle();
	~CHttpHandle();
	
	void RegisterApi(const char* relativePath, http_api_handler handlerFunc, HTTP_METHOD method = HTTP_METHOD_POST);
	int GetApi(const string& strUri, string& relativePath, HTTP_METHOD method, http_api_handler* handler);
	string& GetDocRoot();
	string& GetHomePage();
	string& GetBaseUrl();
	string& GetErrorPage();
	void SetConfig(const string &baseUrl, const string &docRoot, const string &homePage,const string  &errorPage);
	int ParseQueryParams(const char* query_string, QueryParams& query_params);
private:
	string m_baseUrl;
	string m_docRoot;
	string m_homePage;
	string m_errorPage;
	map<string, std::shared_ptr<http_method_handlers> > m_apiMapList;
private:
	void Handle(HTTP_METHOD httpMethod, const char* relativePath, http_api_handler handlerFunc) {
		AddApi(relativePath, httpMethod, handlerFunc);
	}
	void AddApi(const char* path, HTTP_METHOD method, http_api_handler handler);
	
};

