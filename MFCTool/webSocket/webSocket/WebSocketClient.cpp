#include "stdafx.h"
#include "WebSocketClient.h"


CWebSocketClient *g_client;
CWebSocketClient::CWebSocketClient()
{
	m_connectCB = 0;
	m_connectCBParam = 0;

	m_receiveCB = 0;
	m_receiveCBParm = 0;

	g_client = this;
}


CWebSocketClient::~CWebSocketClient()
{
}

void CWebSocketClient::OpenRunLoop()
{
	try {
		// Set logging to be pretty verbose (everything except message payloads)
		m_client.set_access_channels(websocketpp::log::alevel::all);
		m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);

		// Initialize ASIO
		m_client.init_asio();

		// Register our message handler
		m_client.set_message_handler(bind(&on_message, &m_client, ::_1, ::_2));

		m_client.set_close_handler(bind(&on_close, &m_client, ::_1));
		m_client.set_open_handler(bind(&on_open, &m_client, ::_1));
		m_client.set_fail_handler(bind(&on_fail, &m_client, ::_1));

		m_client.start_perpetual();

		m_client.run();
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;

		if (m_connectCB)
		{
			string str = e.what();
			m_connectCB(false, str, m_connectCBParam);
		}

	}
}

bool CWebSocketClient::ConnectToWs(string uri, fConnectStatus cb, void *lparam)
{
	bool bRet = false;
	m_connectCB = cb;
	m_connectCBParam = lparam;

	try {

		websocketpp::lib::error_code ec;
		ws_client::connection_ptr con = m_client.get_connection(uri, ec);
		if (ec) {
			std::cout << "could not create connection because: " << ec.message() << std::endl;
			return false;
		}

		m_hdl = con->get_handle();
		// Note that connect here only requests a connection. No network messages are
		// exchanged until the event loop starts running in the next line.
		m_client.connect(con);

		bRet = true;
	
	}
	catch (websocketpp::exception const & e) {
		std::cout << e.what() << std::endl;

		if (cb)
		{
			string str = e.what();
			cb(false, str, lparam);
		}

		bRet = false;
	}

	return bRet;
}

void CWebSocketClient::disConnect()
{
	if (m_hdl.expired())
	{
		return;
	}
	m_client.close(m_hdl, websocketpp::close::status::normal, "");
}

void CWebSocketClient::SetReceiveCallBack(fReceiveData cb, void *lparam)
{
	m_receiveCB = cb;
	m_receiveCBParm = lparam;
}

bool CWebSocketClient::SendMsg(string str)
{

	websocketpp::lib::error_code ec;

	if (m_hdl.expired())
	{
		return false;
	}

	m_client.send(m_hdl, str, websocketpp::frame::opcode::TEXT, ec);

	if (ec) {
		std::cout << "Echo failed because: " << ec.message() << std::endl;
		return false;
	}
	return true;
}

void CWebSocketClient::StopLoop()
{
	m_client.stop_perpetual();

}

void CWebSocketClient::on_message(ws_client* c, websocketpp::connection_hdl hdl, message_ptr msg)
{
// 	std::cout << "on_message called with hdl: " << hdl.lock().get()
// 		<< " and message: " << msg->get_payload()
// 		<< std::endl;

	if (g_client->m_receiveCB)
	{
		string s = msg->get_payload();
		g_client->m_receiveCB(s, g_client->m_receiveCBParm);
	}
	
}

void  CWebSocketClient::on_open(ws_client* s, websocketpp::connection_hdl hdl) {


	if (g_client->m_connectCB)
	{
		string s = "on_open";
		g_client->m_connectCB(true, s, g_client->m_connectCBParam);
	}


}

void  CWebSocketClient::on_close(ws_client* s, websocketpp::connection_hdl hdl) {
	
	if (g_client->m_connectCB)
	{
		string s = "on_closed";
		g_client->m_connectCB(false, s, g_client->m_connectCBParam);
	}
	
}

void  CWebSocketClient::on_fail(ws_client* s, websocketpp::connection_hdl hdl) {
	
	ws_client::connection_ptr con = s->get_con_from_hdl(hdl);
	//string str = "Fail handler: " +  con->get_ec().message();
	//std::cout << "Fail handler: " << con->get_ec() << " " << con->get_ec().message() << std::endl;

	CString str;
	str.Format("Fail handler: %s", con->get_ec().message());

	if (g_client->m_connectCB)
	{
		string s = str;
		g_client->m_connectCB(false,s , g_client->m_connectCBParam);
	}
	
}
