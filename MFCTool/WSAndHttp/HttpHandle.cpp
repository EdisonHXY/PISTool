#include "stdafx.h"
#include "HttpHandle.h"

#ifndef IS_NUM
#define IS_NUM(c)   ((c) >= '0' && (c) <= '9')
#endif

#ifndef IS_HEX
#define IS_HEX(c) (IS_NUM(c) || ((c) >= 'a' && (c) <= 'f') || ((c) >= 'A' && (c) <= 'F'))
#endif

CHttpHandle::CHttpHandle()
{
	m_baseUrl = "";
	m_homePage = "index.html";
	m_docRoot = "";
	m_errorPage = "";

}


CHttpHandle::~CHttpHandle()
{
}

void CHttpHandle::RegisterApi(const char* relativePath, http_api_handler handlerFunc, HTTP_METHOD method /*= HTTP_METHOD_POST*/)
{
	switch (method)
	{
	case HTTP_METHOD_GET:
		Handle(HTTP_METHOD_GET, relativePath, handlerFunc);
		break;
	case HTTP_METHOD_POST:
		Handle(HTTP_METHOD_POST, relativePath, handlerFunc);
		break;
	case HTTP_METHOD_PUT:
		Handle(HTTP_METHOD_PUT, relativePath, handlerFunc);
		break;
	case HTTP_METHOD_DELET:
		Handle(HTTP_METHOD_DELET, relativePath, handlerFunc);
		break;
	case HTTP_METHOD_ANY:
		Handle(HTTP_METHOD_GET, relativePath, handlerFunc);
		Handle(HTTP_METHOD_POST, relativePath, handlerFunc);
		Handle(HTTP_METHOD_PUT, relativePath, handlerFunc);
		Handle(HTTP_METHOD_DELET, relativePath, handlerFunc);
		break;
	default:
		break;
	}
}

int CHttpHandle::GetApi(const string& strUri, string& relativePath, HTTP_METHOD method, http_api_handler* handler)
{
	const char* s = strUri.c_str();
	const char* b = m_baseUrl.c_str();
	while (*s && *b && *s == *b) { ++s;++b; }
	if (*b != '\0') {
		return -1;
	}
	const char* e = s;
	while (*e && *e != '?') ++e;

	std::string path = std::string(s, e);
	relativePath = path;
	const char *kp, *ks, *vp, *vs;
	for (auto iter = m_apiMapList.begin(); iter != m_apiMapList.end(); ++iter) {
		kp = iter->first.c_str();
		vp = path.c_str();

		if (strcmp(kp,vp) == 0) {
			auto method_handlers = iter->second;
			for (auto iter = method_handlers->begin(); iter != method_handlers->end(); ++iter) {
				if (iter->method == method) {
					*handler = iter->handler;
					return 0;
				}
			}
		}
	}
	*handler = NULL;
	return -1;
}

std::string& CHttpHandle::GetDocRoot()
{
	return m_docRoot;
}

std::string& CHttpHandle::GetHomePage()
{
	return m_homePage;
}

std::string& CHttpHandle::GetBaseUrl()
{
	return m_baseUrl;
}

std::string& CHttpHandle::GetErrorPage()
{
	return m_errorPage;
}

void CHttpHandle::SetConfig(const string &baseUrl, const string &docRoot, const string &homePage, const string  &errorPage)
{
	m_baseUrl = baseUrl;
	m_docRoot = docRoot;

	SetCurrentDirectory(docRoot.c_str());

	m_homePage = homePage;
	m_errorPage = errorPage;
}

void CHttpHandle::AddApi(const char* path, HTTP_METHOD method, http_api_handler handler)
{
	std::shared_ptr<http_method_handlers> method_handlers = NULL;
	auto iter = m_apiMapList.find(path);
	if (iter == m_apiMapList.end()) {
		// add path
		method_handlers = std::shared_ptr<http_method_handlers>(new http_method_handlers);
		m_apiMapList[path] = method_handlers;
	}
	else {
		method_handlers = iter->second;
	}
	for (auto iter = method_handlers->begin(); iter != method_handlers->end(); ++iter) {
		if (iter->method == method) {
			// update
			iter->handler = handler;
			return;
		}
	}
	// add
	method_handlers->push_back(http_method_handler(method, handler));
}

static char hex2i(char hex) {
	if (hex >= '0' && hex <= '9') {
		return hex - '0';
	}
	switch (hex) {
	case 'A': case 'a': return 10;
	case 'B': case 'b': return 11;
	case 'C': case 'c': return 12;
	case 'D': case 'd': return 13;
	case 'E': case 'e': return 14;
	case 'F': case 'f': return 15;
	default: break;
	}
	return 0;
}

static std::string url_unescape(const char* istr) {
	std::string ostr;
	const char* p = istr;
	while (*p != '\0') {
		if (*p == '%' &&
			IS_HEX(p[1]) &&
			IS_HEX(p[2])) {
			ostr += (hex2i(p[1]) << 4 | hex2i(p[2]));
			p += 3;
		}
		else {
			ostr += *p;
			++p;
		}
	}
	return ostr;
}

int CHttpHandle::ParseQueryParams(const char* query_string, QueryParams& query_params) {
	const char* p = strchr(query_string, '?');
	p = p ? p + 1 : query_string;

	enum {
		s_key,
		s_value,
	} state = s_key;

	const char* key = p;
	const char* value = NULL;
	int key_len = 0;
	int value_len = 0;
	while (*p != '\0') {
		if (*p == '&') {
			if (key_len && value_len) {
				std::string strkey = std::string(key, key_len);
				std::string strvalue = std::string(value, value_len);
				query_params[url_unescape(strkey.c_str())] = url_unescape(strvalue.c_str());
				//query_params[strkey.c_str()] = strvalue.c_str();
				key_len = value_len = 0;
			}
			state = s_key;
			key = p + 1;
		}
		else if (*p == '=') {
			state = s_value;
			value = p + 1;
		}
		else {
			state == s_key ? ++key_len : ++value_len;
		}
		++p;
	}
	if (key_len && value_len) {
		std::string strkey = std::string(key, key_len);
		std::string strvalue = std::string(value, value_len);
		query_params[url_unescape(strkey.c_str())] = url_unescape(strvalue.c_str());
		//query_params[strkey.c_str()] = strvalue.c_str();
		key_len = value_len = 0;
	}
	return query_params.size() == 0 ? -1 : 0;
}


