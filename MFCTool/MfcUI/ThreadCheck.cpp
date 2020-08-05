#include "stdafx.h"
#include "ThreadCheck.h"

CMyLock::CMyLock()
{
	InitializeCriticalSection(&m_lock);
}

CMyLock::~CMyLock()
{
	DeleteCriticalSection(&m_lock);
}

void CMyLock::lock()
{
	EnterCriticalSection(&m_lock);
}

void CMyLock::unLock()
{
	LeaveCriticalSection(&m_lock);
}

CThreadCheck::CThreadCheck(UINT tagId) : m_tagID(tagId)
{
	m_hEvent = NULL;
	m_hThread = NULL;
	m_bStopCheck = true;
	m_hThreadCB = NULL;
	m_hThreadCBParam = NULL;
	m_interval = 1000;
	
}


CThreadCheck::~CThreadCheck()
{
	StopRun();
}

void CThreadCheck::StartRun( bool isLoop)
{
	m_isLoop = isLoop;

	if (m_hThread)
	{
		return;
	}
	m_bStopCheck = false;
	m_hEvent = CreateEvent(false, false, NULL, NULL);
	m_hThread = CreateThread(NULL, 0, ThreadProc, this, 0, NULL);
}

void CThreadCheck::UpdateInterval(UINT nInterval)
{
	m_interval = nInterval;
}

void CThreadCheck::StopRun()
{
	m_bStopCheck = true;
	SetEvent(m_hEvent);

	WaitForSingleObject(m_hEvent, 5000);
	m_hThread = NULL;
}


void CThreadCheck::SetThreadExectCallBack(fThreadExectCallBack cb, void *lparam)
{
	m_hThreadCB = cb;
	m_hThreadCBParam = lparam;
}

DWORD WINAPI CThreadCheck::ThreadProc(LPVOID lpParameter)
{
	CThreadCheck*dlg = (CThreadCheck*)lpParameter;

	if (dlg->m_isLoop)
	{
		while (!dlg->m_bStopCheck)
		{
			if (dlg->m_hThreadCB )
			{
				dlg->m_hThreadCB(dlg->m_tagID,dlg->m_hThreadCBParam);
			}

			WaitForSingleObject(dlg->m_hEvent, dlg->m_interval);
		}
	}
	else
	{
		if (dlg->m_hThreadCB)
		{
			dlg->m_hThreadCB(dlg->m_tagID,dlg->m_hThreadCBParam);
		}
	}
	SetEvent(dlg->m_hEvent);
	return 0;
}

void CThreadCheck::CalcueNum(int &everyNum, int &revelNum, int totalNum, int countNum)
{
	if (countNum <= 0)
	{
		everyNum = revelNum = 0;
		return;
	}
	everyNum = totalNum / countNum;
	revelNum = totalNum % countNum;
}

int CThreadCheck::CaclueEveryThreadCheckRange(int nCheckTotal,int nThreadTotal,vector<pair <int,int>> &threadRangeList)
{
	//计算每个线程检测的个数
	int everyNum = 0;
	int revelNum = 0;
	CalcueNum(everyNum, revelNum, nCheckTotal, nThreadTotal);


	int preLength = 0;
	int preIndex = 0;
	threadRangeList.clear();
	for (int i = 0;i < nThreadTotal;++i)
	{
		int num = everyNum;
		if (i < revelNum)
		{
			num++;
		}

		pair <int, int > p = make_pair(preIndex, preIndex + num);
		preIndex = preIndex + num;
		threadRangeList.push_back(p);
	}


	return everyNum == 0 ? revelNum : nThreadTotal;
	
}


CThreadCheckGroup::CThreadCheckGroup()
{
	m_groupCB = NULL;
	m_groupCBParam = NULL;
}

CThreadCheckGroup::~CThreadCheckGroup()
{
	Stop();
}



void CThreadCheckGroup::SetParam(UINT nMaxThread, UINT nCheckTotal, UINT nInterval, bool isLoop /*=true*/)
{
	if (nCheckTotal <=0)
	{
		//没有要检测的
		return;
	}
	
	Stop();

	int nUseCount = CThreadCheck::CaclueEveryThreadCheckRange(nCheckTotal, nMaxThread, m_threadRangList);

	for (UINT i = 0 ; i < nUseCount;++i)
	{
		CThreadCheck *td = new CThreadCheck(i);
		td->SetThreadExectCallBack(ThreadExectCallBackFunc, this);
		td->UpdateInterval(nInterval);
		td->StartRun(isLoop);
		m_theadList.push_back(td);
	}
}

void CThreadCheckGroup::SetGroupThreadExectCallBack(fThreadGroupCallBack cb, void *lparam)
{
	m_groupCB = cb;
	m_groupCBParam = lparam;
}

void CThreadCheckGroup::ThreadExectCallBackFunc(UINT threadID, void *lparam)
{
	CThreadCheckGroup *tg = (CThreadCheckGroup*)lparam;
	tg->ThreadCheckAction(threadID);
}

void CThreadCheckGroup::ThreadCheckAction(UINT threadID)
{
	
	if (m_groupCB)
	{
		pair <int, int> pp = m_threadRangList[threadID];
		m_groupCB(pp.first, pp.second, m_groupCBParam);
	}
}

void CThreadCheckGroup::Stop()
{
	for (unsigned int i = 0 ;i<m_theadList.size();++i)
	{
		CThreadCheck *tc = m_theadList[i];
		if (tc)
		{
			tc->StopRun();
			delete tc;
			tc = NULL;
		}
	}
	m_theadList.clear();
}
