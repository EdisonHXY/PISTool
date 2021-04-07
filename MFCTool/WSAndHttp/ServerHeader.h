#pragma once
#include <iostream>
#include <map>
#include <list>
#include <memory>
using namespace std;
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>


enum HTTP_CONTENTTYPE
{
	HTTP_CONTENTTYPE_APPLICATION_JSON,
	HTTP_CONTENTTYPE_APPLICATION_XML,
	HTTP_CONTENTTYPE_MULTIPART_FORM_DATA,
	HTTP_CONTENTTYPE_TEXT_PLAIN,
	HTTP_CONTENTTYPE_TEXT_HTML,
	HTTP_CONTENTTYPE_IMAGE_JPEG,
	HTTP_CONTENTTYPE_IMAGE_PNG,
	HTTP_CONTENTTYPE_IMAGE_GIF,
	HTTP_CONTENTTYPE_IMAGE_BMP,
	HTTP_CONTENTTYPE_IMAGE_SVG
};

//请求类型
enum HTTP_METHOD
{
	HTTP_METHOD_GET,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELET,
	HTTP_METHOD_ANY
};

//请求的回调函数 req.get_body() req.get_header()
typedef int(*http_api_handler)(const websocketpp::http::parser::request &req, const map<string, string> &queryParams, string &responBody, int &nStatusCode, HTTP_CONTENTTYPE &nConentType, void *lParam);
typedef int(*http_api_prehandler)(const websocketpp::http::parser::request &req,const string &relativePath, void *lParam);

//请求和回调的集合
struct http_method_handler {
	HTTP_METHOD         method;
	http_api_handler    handler;
	http_method_handler(HTTP_METHOD m = HTTP_METHOD_POST, http_api_handler h = NULL) {
		method = m;
		handler = h;
	}
};
//集合的列表
typedef std::list<http_method_handler> http_method_handlers;
typedef map<std::string, std::string>   QueryParams;