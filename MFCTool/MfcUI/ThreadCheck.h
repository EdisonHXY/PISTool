#pragma once
#include <vector>
using namespace std;

//临界区锁
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


//单一线程
class CThreadCheck
{
public:
	CThreadCheck(UINT tagId);
	~CThreadCheck();

	typedef void(*fThreadExectCallBack)(UINT threadID,void *lparam);

	void StartRun(bool isLoop =true);
	void UpdateInterval(UINT nInterval);
	void StopRun();
	void SetThreadExectCallBack(fThreadExectCallBack cb,void *lparam); //线程内部执行的回调
	

	//计算每个线程的检测范围  
	//nCheckTotal 总共检测的数量  nThreadTotal 开启了几个线程   threadRangeList 每个线程检测的开始位置 和  结束位置
	//返回值 是实际能够用到的线程数
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

//多个线程 执行相同的动作
class CThreadCheckGroup
{
public:
	CThreadCheckGroup();
	~CThreadCheckGroup();

	typedef void(*fThreadGroupCallBack)(UINT startIndex, UINT endIndex, void *lparam);

	//设置参数 nCheckTotal总共检测的数量   nMaxThread  最多开启了几个线程  nInterval 循环间隔 isLoop 是否循环执行
	void SetParam(UINT nMaxThread, UINT nCheckTotal, UINT nInterval,bool isLoop =true);

	//多个线程内部执行的回调
	void SetGroupThreadExectCallBack(fThreadGroupCallBack cb, void *lparam); //线程内部执行的回调

	void Stop();
private:
	fThreadGroupCallBack m_groupCB;
	void *m_groupCBParam;
	vector<pair <int, int>> m_threadRangList; //计算出每个线程检测的范围
	vector<CThreadCheck*> m_theadList; //所有检测的线程

	static void ThreadExectCallBackFunc(UINT threadID, void *lparam);
	void ThreadCheckAction(UINT threadID);
	
};



