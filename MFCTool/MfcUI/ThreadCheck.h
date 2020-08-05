#pragma once
#include <vector>
using namespace std;

//�ٽ�����
class CMyLock
{
public:
	CMyLock();
	~CMyLock();

	void lock();
	void unLock();

private:
	CRITICAL_SECTION m_lock;
};


//��һ�߳�
class CThreadCheck
{
public:
	CThreadCheck(UINT tagId);
	~CThreadCheck();

	typedef void(*fThreadExectCallBack)(UINT threadID,void *lparam);

	void StartRun(bool isLoop =true);
	void UpdateInterval(UINT nInterval);
	void StopRun();
	void SetThreadExectCallBack(fThreadExectCallBack cb,void *lparam); //�߳��ڲ�ִ�еĻص�
	

	//����ÿ���̵߳ļ�ⷶΧ  
	//nCheckTotal �ܹ���������  nThreadTotal �����˼����߳�   threadRangeList ÿ���̼߳��Ŀ�ʼλ�� ��  ����λ��
	//����ֵ ��ʵ���ܹ��õ����߳���
	static int CaclueEveryThreadCheckRange(int nCheckTotal, int nThreadTotal, vector<pair <int, int>> &threadRangeList);


private:
	bool m_isLoop;
	
	UINT m_tagID;
	UINT m_interval;
	HANDLE m_hEvent;
	HANDLE m_hThread;
	bool m_bStopCheck;
	fThreadExectCallBack  m_hThreadCB;
	void *m_hThreadCBParam;
	static DWORD WINAPI ThreadProc(LPVOID lpParameter);
	static void CalcueNum(int &everyNum, int &revelNum, int totalNum, int countNum);
};

//����߳� ִ����ͬ�Ķ���
class CThreadCheckGroup
{
public:
	CThreadCheckGroup();
	~CThreadCheckGroup();

	typedef void(*fThreadGroupCallBack)(UINT startIndex, UINT endIndex, void *lparam);

	//���ò��� nCheckTotal�ܹ���������   nMaxThread  ��࿪���˼����߳�  nInterval ѭ����� isLoop �Ƿ�ѭ��ִ��
	void SetParam(UINT nMaxThread, UINT nCheckTotal, UINT nInterval,bool isLoop =true);

	//����߳��ڲ�ִ�еĻص�
	void SetGroupThreadExectCallBack(fThreadGroupCallBack cb, void *lparam); //�߳��ڲ�ִ�еĻص�

	void Stop();
private:
	fThreadGroupCallBack m_groupCB;
	void *m_groupCBParam;
	vector<pair <int, int>> m_threadRangList; //�����ÿ���̼߳��ķ�Χ
	vector<CThreadCheck*> m_theadList; //���м����߳�

	static void ThreadExectCallBackFunc(UINT threadID, void *lparam);
	void ThreadCheckAction(UINT threadID);
	
};



