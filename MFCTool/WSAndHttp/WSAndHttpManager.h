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
		unsigned int nWebSocketPort; //websocket�˿�
		unsigned int nHttpPort; //http�˿� ������ͬ
		string strBaseUrl; //url��ǰ׺
		string strDocRoot; //�ĵ������λ��
		string strHomePage;//��ҳ��
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

	//��ʼִ��
	void StartAction(ActionParam &param);

	//ִֹͣ��
	void StopAction();

	//�������ʱ�Ļص�
	void SetErrorCallBack(CWebScoketServer::fServerErrorCallBack cb,void *lparam);

	//���ý���websocket����Ϣ
	void SetReceiveWSMsgCallBack(CWebScoketServer::fReceiveWsMsgCallBack cb, void *lParam);

	//����websocket��Ϣ �ı�
	void SenWsMsg(const CWebScoketServer::connection_data & cd, const string &strMsg, bool isText = true);

	//����Ԥ������
	void  SetPreProcessor(http_api_prehandler hah, void *lParam);

	//ע��http������ ��Ӧ���ڻص�������
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

