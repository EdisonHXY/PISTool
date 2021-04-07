#include "stdafx.h"
#include "WSAndHttpManager.h"


CWSAndHttpManager::CWSAndHttpManager()
{
	m_wsServer = 0;
	m_httpServer = 0;
	m_preProcessor = 0;
	m_preProcessorParam = 0;
}


CWSAndHttpManager::~CWSAndHttpManager()
{
	StopAction();

}

//开启服务
DWORD WINAPI CWSAndHttpManager::WsLoopThread(LPVOID lpParameter) {

	CWSAndHttpManager *cen = (CWSAndHttpManager *)lpParameter;
	cen->m_wsServer->StartRunServer(cen->m_acParam.nWebSocketPort);
	return 0;
}

//开启服务
DWORD WINAPI CWSAndHttpManager::HttpLoopThread(LPVOID lpParameter) {

	CWSAndHttpManager *cen = (CWSAndHttpManager *)lpParameter;
	cen->m_httpServer->StartRunServer(cen->m_acParam.nHttpPort);

	return 0;
}


void CWSAndHttpManager::ReceiveHttpMsgCallBack(const websocketpp::http::parser::request  &req, string &responBody, int &nStatusCode, HTTP_CONTENTTYPE &nConentType, void *lParam)
{
	CWSAndHttpManager *cen = (CWSAndHttpManager *)lParam;
	cen->HandleHttpMsg(req, responBody, nStatusCode,nConentType);
}

bool CWSAndHttpManager::ReadFileContent(const string &filePath, string &content)
{
	content = "";
	
	FILE *f = fopen(filePath.c_str(), "rb");
	bool bRet = false;
	if (f)
	{
		bRet = true;
		fseek(f, 0, SEEK_END);//将文件内部的指针指向文件末尾
		long lsize = ftell(f);//获取文件长度
		rewind(f);//将文件内部的指针重新指向一个流的开头
		char *szBuffer = new char[lsize + 1];
		memset(szBuffer, 0, lsize + 1);
		fread(szBuffer, 1, lsize, f);
		content = szBuffer;
		delete szBuffer;
		szBuffer = 0;
		fclose(f);
	}

	return bRet;
}

void CWSAndHttpManager::HandleHttpMsg(const websocketpp::http::parser::request &req, string &responBody, int &nStatusCode, HTTP_CONTENTTYPE &nConentType)
{
	const string& strUri = req.get_uri();
	const string& strMethod = req.get_method();

	//获取url对应的回调
	HTTP_METHOD method =  GetMethodWithStr(strMethod);
	http_api_handler apiHand = 0;
	string relativePath;
	int nRet = m_httpHandle.GetApi(strUri, relativePath, method, &apiHand);
	
	//执行预处理操作
	if (m_preProcessor)
	{
		m_preProcessor(req, relativePath, m_preProcessorParam);
	}
	//执行回调
	if (apiHand)
	{
		//解析url 获取url中的参数
		map<string, string> listParam;
		m_httpHandle.ParseQueryParams(strUri.c_str(), listParam);
		apiHand(req, listParam, responBody, nStatusCode, nConentType,0);
	}
	else
	{
		// Not Implemented
		nStatusCode = 501;
	}
	

}


HTTP_METHOD CWSAndHttpManager::GetMethodWithStr(const string &strMedthod)
{
	if (strMedthod == "GET")
	{
		return HTTP_METHOD_GET;
	}
	else if(strMedthod == "POST")
	{
		return HTTP_METHOD_POST;
	}
	else if (strMedthod == "PUT")
	{
		return HTTP_METHOD_PUT;
	}
	return HTTP_METHOD_POST;
}


// context_ptr on_tls_init(tls_mode mode, websocketpp::connection_hdl hdl) {
// 	namespace asio = websocketpp::lib::asio;
// 
// 	std::cout << "on_tls_init called with hdl: " << hdl.lock().get() << std::endl;
// 	std::cout << "using TLS mode: " << (mode == MOZILLA_MODERN ? "Mozilla Modern" : "Mozilla Intermediate") << std::endl;
// 
// 	context_ptr ctx = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
// 
// 	try {
// 		if (mode == MOZILLA_MODERN) {
// 			// Modern disables TLSv1
// 			ctx->set_options(asio::ssl::context::default_workarounds |
// 				asio::ssl::context::no_sslv2 |
// 				asio::ssl::context::no_sslv3 |
// 				asio::ssl::context::no_tlsv1 |
// 				asio::ssl::context::single_dh_use);
// 		}
// 		else {
// 			ctx->set_options(asio::ssl::context::default_workarounds |
// 				asio::ssl::context::no_sslv2 |
// 				asio::ssl::context::no_sslv3 |
// 				asio::ssl::context::single_dh_use);
// 		}
// 		//这里面证书密码可以不需要验证，看生成的证书
// 		ctx->set_password_callback(bind(&get_password));
// 		ctx->use_certificate_chain_file("server.pem");
// 		ctx->use_private_key_file("server.pem", asio::ssl::context::pem);
// 
// 		// Example method of generating this file:
// 		// `openssl dhparam -out dh.pem 2048`
// 		// Mozilla Intermediate suggests 1024 as the minimum size to use
// 		// Mozilla Modern suggests 2048 as the minimum size to use.
// 		ctx->use_tmp_dh_file("dh.pem");
// 
// 		std::string ciphers;
// 
// 		if (mode == MOZILLA_MODERN) {
// 			ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!3DES:!MD5:!PSK";
// 		}
// 		else {
// 			ciphers = "ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-DSS-AES128-GCM-SHA256:kEDH+AESGCM:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA:ECDHE-ECDSA-AES128-SHA:ECDHE-RSA-AES256-SHA384:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA:ECDHE-ECDSA-AES256-SHA:DHE-RSA-AES128-SHA256:DHE-RSA-AES128-SHA:DHE-DSS-AES128-SHA256:DHE-RSA-AES256-SHA256:DHE-DSS-AES256-SHA:DHE-RSA-AES256-SHA:AES128-GCM-SHA256:AES256-GCM-SHA384:AES128-SHA256:AES256-SHA256:AES128-SHA:AES256-SHA:AES:CAMELLIA:DES-CBC3-SHA:!aNULL:!eNULL:!EXPORT:!DES:!RC4:!MD5:!PSK:!aECDH:!EDH-DSS-DES-CBC3-SHA:!EDH-RSA-DES-CBC3-SHA:!KRB5-DES-CBC3-SHA";
// 		}
// 
// 		if (SSL_CTX_set_cipher_list(ctx->native_handle(), ciphers.c_str()) != 1) {
// 			std::cout << "Error setting cipher list" << std::endl;
// 		}
// 	}
// 	catch (std::exception& e) {
// 		std::cout << "Exception: " << e.what() << std::endl;
// 	}
// 	return ctx;
// }




void CWSAndHttpManager::StartAction(ActionParam &param)
{
	m_acParam = param;
	m_httpHandle.SetConfig(param.strBaseUrl, param.strDocRoot, param.strHomePage,param.strErrorPage);
	
	//开启websocket服务
	m_wsServer = new CWebScoketServer;
	if (param.nWebSocketPort > 0)
	{
		CreateThread(NULL, 0, WsLoopThread, this, 0, NULL);
	}
	
	if (param.nHttpPort > 0 &&  param.nHttpPort != param.nWebSocketPort)
	{
		//开启Http服务
		m_httpServer = new CWebScoketServer;
		CreateThread(NULL, 0, HttpLoopThread, this, 0, NULL);
	}
	else
	{
		m_httpServer = m_wsServer;
	}

	if (m_httpServer)
	{
		m_httpServer->SetHttpMsgCallBack(ReceiveHttpMsgCallBack, this);
	}


}

void CWSAndHttpManager::StopAction()
{
	if (m_wsServer)
	{
		m_wsServer->StopRun();
		delete m_wsServer;
		m_wsServer = 0;
	}

	if (m_acParam.nHttpPort != m_acParam.nWebSocketPort && m_httpServer)
	{
		m_httpServer->StopRun();
		delete m_httpServer;
		m_httpServer = 0;
	}
}

void CWSAndHttpManager::SetErrorCallBack(CWebScoketServer::fServerErrorCallBack cb, void *lparam)
{
	if (m_wsServer)
	{
		m_wsServer->SetErrorCallBack(cb, lparam);
	}
}

void CWSAndHttpManager::SetReceiveWSMsgCallBack(CWebScoketServer::fReceiveWsMsgCallBack cb, void *lParam)
{
	if (m_wsServer)
	{
		m_wsServer->SetReceiveWSMsgCallBack(cb, lParam);
	}
	
}

void CWSAndHttpManager::SenWsMsg(const CWebScoketServer::connection_data & cd, const string &strMsg, bool isText)
{
	if (m_wsServer)
	{
		m_wsServer->SendMsgToHandle(cd, strMsg, isText ? 0 : 1);
	}
	
}

void CWSAndHttpManager::SetPreProcessor(http_api_prehandler hah, void *lParam)
{
	m_preProcessor = hah;
	m_preProcessorParam = lParam;
}

void CWSAndHttpManager::RegisterApi(const char* relativePath, http_api_handler handlerFunc, HTTP_METHOD method /*= HTTP_METHOD_POST*/)
{
	m_httpHandle.RegisterApi(relativePath, handlerFunc, method);
}
