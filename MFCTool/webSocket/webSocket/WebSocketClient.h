#pragma once
#define _WEBSOCKETPP_NO_CPP11_SYSTEM_ERROR_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include <string>
#include <iostream>
using namespace std;

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;
// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

class CWebSocketClient
{
public:
	CWebSocketClient();
	~CWebSocketClient();

	typedef void(*fConnectStatus)(bool isConnected, string &desStr, void *lparam);
	typedef void(*fReceiveData)(string &data, void *lparam);

	//打开运行环境
	void OpenRunLoop();

	//连接  uri = "ws://localhost:9002";
	bool ConnectToWs(string url, fConnectStatus cb = NULL, void *lparam = NULL);

	//断开连接
	void disConnect();

	//接收
	void SetReceiveCallBack(fReceiveData cb, void *lparam);
	//发送
	bool SendMsg(string str);

	//停止
	void StopLoop();

private:
	ws_client m_client;
	fConnectStatus m_connectCB;
	void *m_connectCBParam;
	fReceiveData m_receiveCB;
	void *m_receiveCBParm;
	websocketpp::connection_hdl m_hdl;
	static void on_message(ws_client* c, websocketpp::connection_hdl hdl, message_ptr msg);
	static void on_open(ws_client* s, websocketpp::connection_hdl hdl);
	static void on_close(ws_client* s, websocketpp::connection_hdl hdl);
	static void on_fail(ws_client* s, websocketpp::connection_hdl hdl);
};

