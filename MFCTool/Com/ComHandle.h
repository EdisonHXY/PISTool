#pragma once
#include "PCOMM.H"


typedef VOID(CALLBACK *fReceiveDataFromComCallBack)(char* buf, UINT nLeng,void *lParam);

class CComHandle
{
public:
	CComHandle();
	~CComHandle();

	typedef struct ComParamTag
	{
		//波特率
		int nBtSpeed;
		//数据位
		int nData;
		//停止位
		int nStop;


		//校验位 不允许设置 只支持 PNone
		//int nCheck;

		ComParamTag()
		{
			nBtSpeed = 9600;
			nData = 8;
			nStop = 1;
		}

	}ComParam;

	//打开端口 设置参数
	bool OpenComPort(int iComPort, ComParam *param = NULL);

	//关闭端口
	bool CloseComPort();

	//写入消息
	int WriteDataToCom(char *byCmd, UINT nWriteCount);

	//设置接收消息的回调
	void SetReceiveDataCallBack(fReceiveDataFromComCallBack cb, void *lparam);

	//读取
	int ReadBuf(char* buf, UINT nLeng);


private:
	int m_nComPort;
	bool SetComPortParam(ComParam *param);
	void GetRealParam(ComParam *param,int &baud, int &mode);
	static void CALLBACK ReceiveDataInfo(int iPort);

	fReceiveDataFromComCallBack m_receiveCB;
	void *m_cbParam;

};

