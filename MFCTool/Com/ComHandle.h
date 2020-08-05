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
		//������
		int nBtSpeed;
		//����λ
		int nData;
		//ֹͣλ
		int nStop;


		//У��λ ���������� ֻ֧�� PNone
		//int nCheck;

		ComParamTag()
		{
			nBtSpeed = 9600;
			nData = 8;
			nStop = 1;
		}

	}ComParam;

	//�򿪶˿� ���ò���
	bool OpenComPort(int iComPort, ComParam *param = NULL);

	//�رն˿�
	bool CloseComPort();

	//д����Ϣ
	int WriteDataToCom(char *byCmd, UINT nWriteCount);

	//���ý�����Ϣ�Ļص�
	void SetReceiveDataCallBack(fReceiveDataFromComCallBack cb, void *lparam);

	//��ȡ
	int ReadBuf(char* buf, UINT nLeng);


private:
	int m_nComPort;
	bool SetComPortParam(ComParam *param);
	void GetRealParam(ComParam *param,int &baud, int &mode);
	static void CALLBACK ReceiveDataInfo(int iPort);

	fReceiveDataFromComCallBack m_receiveCB;
	void *m_cbParam;

};

