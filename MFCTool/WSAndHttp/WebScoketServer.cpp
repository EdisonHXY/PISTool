#include "stdafx.h"
#include "WebScoketServer.h"


CWebScoketServer::CWebScoketServer():m_next_sessionid(1)
{
	m_httpCB = 0;
	m_wsCB = 0;
	m_wsCBParam = m_httpCBParm = 0;

	m_errCB = 0;
	m_errCBParam = 0;

	m_server.clear_access_channels(websocketpp::log::alevel::all);
	m_server.clear_error_channels(websocketpp::log::alevel::all);

	m_server.set_access_channels(websocketpp::log::alevel::all);
	
	m_log.open("output.log");
	m_server.get_alog().set_ostream(&m_log);
	m_server.get_elog().set_ostream(&m_log);
	//m_server.register_ostream(&std::cout);


	m_server.init_asio();
	m_server.set_reuse_addr(true);
	m_server.set_http_handler(bind(&CWebScoketServer::on_http, this, ::_1));


	m_server.set_message_handler(bind(&CWebScoketServer::on_message, this, ::_1, ::_2));
	m_server.set_open_handler(bind(&CWebScoketServer::on_open, this, ::_1));
	m_server.set_close_handler(bind(&CWebScoketServer::on_close, this, ::_1));
	m_server.set_listen_backlog(81920);
}

CWebScoketServer::~CWebScoketServer()
{
	m_log.close();
}

void CWebScoketServer::StartRunServer(int port)
{
	//m_server.set_socket_init_handler(bind(&on_socket_init, ::_1, ::_2));
	try
	{
		m_server.listen(port);
		m_server.start_accept();
		m_server.run();
	}
	catch (const std::exception&)
	{
		
		if (m_errCB)
		{
			m_errCB(-1, "端口可能被占用", m_errCBParam);
		}
	}
	
}

void CWebScoketServer::StopRun()
{
	m_server.stop();
}

void CWebScoketServer::SetErrorCallBack(fServerErrorCallBack cb, void *lparam)
{
	m_errCB = cb;
	m_errCBParam = lparam;
}

void CWebScoketServer::SetReceiveWSMsgCallBack(fReceiveWsMsgCallBack cb, void *lParam)
{
	m_wsCB = cb;
	m_wsCBParam = lParam;
}

void CWebScoketServer::SetHttpMsgCallBack(fReceiveHttpMsgCallBack cb, void *lParam)
{
	m_httpCB = cb;
	m_httpCBParm = lParam;
}

void CWebScoketServer::SendMsgToHandle(const connection_data & cd, const string& strMsg, int nFlag /*= 0*/)
{
	m_server.send(cd.hdl, strMsg, nFlag == 0 ?  websocketpp::frame::opcode::TEXT: websocketpp::frame::opcode::binary);
}

void CWebScoketServer::on_open(connection_hdl hdl)
{
	connection_data data;

	data.sessionid = m_next_sessionid++;
	data.name.clear();
	data.hdl = hdl;

	m_connections[hdl] = data;


}

void CWebScoketServer::on_close(connection_hdl hdl)
{
	connection_data data;
	bool bRet = get_data_from_hdl(hdl, data);
	if (!bRet)
	{
		return;
	}
	m_connections.erase(hdl);
}

void CWebScoketServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg)
{
	if (msg->get_opcode() == websocketpp::frame::opcode::text) {
		m_server.get_alog().write(websocketpp::log::alevel::app,
			"Text Message Received: " + msg->get_payload());
	}
	else {
		m_server.get_alog().write(websocketpp::log::alevel::app,
			"Binary Message Received: " + websocketpp::utility::to_hex(msg->get_payload()));
	}

	connection_data data;
	bool bRet = get_data_from_hdl(hdl, data);
	if (!bRet)
	{
		return;
	}

	if (m_wsCB)
	{
		const string &str = msg->get_payload();
		m_wsCB(data, str, m_wsCBParam);
	}

	
	
}

void CWebScoketServer::on_http( connection_hdl hdl)
{ 
	ws_server::connection_ptr con = m_server.get_con_from_hdl(hdl);
	websocketpp::http::parser::request rt = con->get_request();


	string strBody = "";
	int nStatusCode = 400;
	
	HTTP_CONTENTTYPE nContType = HTTP_CONTENTTYPE_APPLICATION_JSON;
	if (m_httpCB)
	{
		m_httpCB(rt, strBody, nStatusCode, nContType,m_httpCBParm);
	}
	else
	{

		strBody = string_To_UTF8("错误的请求");
	}
	string strCont = GetContentText(nContType);
	con->replace_header("Content-Type", strCont);
	con->set_body(strBody);
	con->set_status(websocketpp::http::status_code::value(nStatusCode));
}

bool CWebScoketServer::get_data_from_hdl(connection_hdl hdl, connection_data& data)
{
	auto it = m_connections.find(hdl);

	if (it == m_connections.end()) {
		return false;
	}

	data = it->second;
	return true;
}

std::string CWebScoketServer::GetContentText(HTTP_CONTENTTYPE tp)
{
	string str;
	switch (tp)
	{
	case HTTP_CONTENTTYPE_APPLICATION_JSON:
		str = "application/json";
		break;
	case HTTP_CONTENTTYPE_APPLICATION_XML:
		str = "application/xml";
		break;
	case HTTP_CONTENTTYPE_MULTIPART_FORM_DATA:
		str = "multipart/form-data";
		break;
	case HTTP_CONTENTTYPE_TEXT_PLAIN:
		str = "text/plain";
		break;
	case HTTP_CONTENTTYPE_TEXT_HTML:
		str = "text/html";
		break;
	case HTTP_CONTENTTYPE_IMAGE_JPEG:
		str = "image/jpeg";
		break;
	case HTTP_CONTENTTYPE_IMAGE_PNG:
		str = "image/png";
		break;
	case HTTP_CONTENTTYPE_IMAGE_GIF:
		str = "image/gif";
		break;
	case HTTP_CONTENTTYPE_IMAGE_BMP:
		str = "image/bmp";
		break;
	case HTTP_CONTENTTYPE_IMAGE_SVG:
		str = "image/svg";
		break;
	default:
		str = "application/json";
		break;
	}
	return str;
}

std::string CWebScoketServer::UTF8_To_string(const std::string & str)
{
	int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	memset(pwBuf, 0, nwLen * 2 + 2);

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	memset(pBuf, 0, nLen + 1);

	WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr = pBuf;

	delete[]pBuf;
	delete[]pwBuf;

	pBuf = NULL;
	pwBuf = NULL;

	return retStr;
}
std::string CWebScoketServer::string_To_UTF8(const std::string & str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴  
	ZeroMemory(pwBuf, nwLen * 2 + 2);

	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);

	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);

	char * pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);

	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);

	std::string retStr(pBuf);

	delete[]pwBuf;
	delete[]pBuf;

	pwBuf = NULL;
	pBuf = NULL;

	return retStr;
}

