#pragma once
#include "ServerHeader.h"
#include <iostream>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio> ws_server;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef ws_server::message_ptr message_ptr;

using websocketpp::connection_hdl;



//typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;


class CWebScoketServer
{
public:
	CWebScoketServer();
	~CWebScoketServer();

	typedef struct __connection_data {
		connection_hdl hdl;
		int sessionid;
		std::string name;
	}connection_data;

	typedef void(*fReceiveWsMsgCallBack)(connection_data &cd, const string &strMsg, void *lParam);
	typedef void(*fReceiveHttpMsgCallBack)(const websocketpp::http::parser::request  &req,string &responBody,int &nStatusCode, HTTP_CONTENTTYPE &nConentType, void *lParam);
	typedef void(*fServerErrorCallBack)(int nErrorCode,const string &strMsg, void *lparam);

	//开始运行服务 
	void StartRunServer(int port); 
	
	void StopRun();

	void SetErrorCallBack(CWebScoketServer::fServerErrorCallBack cb, void *lparam);

	//设置接收websocket的信息
	void SetReceiveWSMsgCallBack(fReceiveWsMsgCallBack cb,void *lParam);

	//设置接收Http的信息
	void SetHttpMsgCallBack(fReceiveHttpMsgCallBack cb, void *lParam);

	//向某个连接发送消息
	void SendMsgToHandle(const connection_data & cd, const string& strMsg, int nFlag = 0);


	//数据格式转换
	static std::string string_To_UTF8(const std::string & str);
	static std::string UTF8_To_string(const std::string & str);

private:
	typedef std::map<connection_hdl, connection_data, std::owner_less<connection_hdl> > con_list;
	unsigned int  m_next_sessionid;
	ws_server m_server;
	con_list m_connections;
	fReceiveHttpMsgCallBack m_httpCB;
	void* m_httpCBParm;
	fReceiveWsMsgCallBack m_wsCB;
	void* m_wsCBParam;
	std::ofstream m_log;

	fServerErrorCallBack m_errCB;
	void *m_errCBParam;

private:
	void on_open( connection_hdl hdl);
	void on_close(connection_hdl hdl);
	void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
	void on_http(connection_hdl hdl);
	bool get_data_from_hdl(connection_hdl hdl, connection_data& data);
	string GetContentText(HTTP_CONTENTTYPE tp);
};

