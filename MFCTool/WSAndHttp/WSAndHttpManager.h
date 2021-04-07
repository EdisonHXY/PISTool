#pragma once
#include "WebScoketServer.h"
#include "HttpHandle.h"

class CWSAndHttpManager
{
public:
	CWSAndHttpManager();
	~CWSAndHttpManager();

	typedef struct __ActionParam
	{
		unsigned int nWebSocketPort; //websocket端口
		unsigned int nHttpPort; //http端口 可以相同
		string strBaseUrl; //url的前缀
		string strDocRoot; //文档保存的位置
		string strHomePage;//主页面
		string strErrorPage;//error
		__ActionParam()
		{
			nHttpPort = 8083;
			nWebSocketPort = 8081;
			strBaseUrl = "/v1/api";
			strDocRoot = "/html";
			strHomePage = "index.html";
		}

	}ActionParam;

	//开始执行
	void StartAction(ActionParam &param);

	//停止执行
	void StopAction();

	//服务出错时的回调
	void SetErrorCallBack(CWebScoketServer::fServerErrorCallBack cb,void *lparam);

	//设置接收websocket的信息
	void SetReceiveWSMsgCallBack(CWebScoketServer::fReceiveWsMsgCallBack cb, void *lParam);

	//发送websocket信息 文本
	void SenWsMsg(const CWebScoketServer::connection_data & cd, const string &strMsg, bool isText = true);

	//设置预处理动作
	void  SetPreProcessor(http_api_prehandler hah, void *lParam);

	//注册http的请求 响应是在回调中设置
	void RegisterApi(const char* relativePath, http_api_handler handlerFunc, HTTP_METHOD method = HTTP_METHOD_POST);


	bool ReadFileContent(const string &filePath, string &content);



private:
	ActionParam m_acParam;
	CWebScoketServer *m_wsServer;
	CWebScoketServer *m_httpServer;
	static DWORD WINAPI WsLoopThread(LPVOID lpParameter);
	static DWORD WINAPI HttpLoopThread(LPVOID lpParameter);
	CHttpHandle m_httpHandle;
	http_api_prehandler m_preProcessor;
	void* m_preProcessorParam;
private:

	static void ReceiveHttpMsgCallBack(const websocketpp::http::parser::request  &req, string &responBody, int &nStatusCode, HTTP_CONTENTTYPE &nConentType, void *lParam);
	
	void HandleHttpMsg(const websocketpp::http::parser::request  &req, string &responBody, int &nStatusCode, HTTP_CONTENTTYPE &nConentType);
	HTTP_METHOD GetMethodWithStr(const string &strMedthod);
};

