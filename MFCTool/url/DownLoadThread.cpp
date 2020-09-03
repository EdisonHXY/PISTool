#include "stdafx.h"
#include "DownLoadThread.h"


CDownLoadThread::CDownLoadThread()
{
	m_downLoadThread = NULL;
	m_isStopDownLoad = false;
	m_successFileList.clear();
	m_failFileList.clear();
}


CDownLoadThread::~CDownLoadThread()
{
	for (size_t i = 0; i < m_threadParams.size(); i++)
	{
		m_threadParams.pop();
	}
	if (m_downLoadThread)
	{
		WaitForSingleObject(m_downLoadThread, 10);
	}

}

void CDownLoadThread::AddVideoFileToList(ThreadParams &tp)
{
	if (m_downLoadThread == NULL)
	{
		m_isStopDownLoad = false;
		m_downLoadThread = CreateThread(NULL, 0, DownLoadThreadProc, this, 0, NULL);
	}
	m_threadParams.push(tp);
}

HANDLE CDownLoadThread::GetThreadHandle()
{
	return m_downLoadThread;
}

void CDownLoadThread::AddFinished()
{
	m_isStopDownLoad = true;

}

void CDownLoadThread::SetThreadTag(int index)
{
	m_thread_tag = index;
}

int CDownLoadThread::GetThreadTag()
{
	return m_thread_tag;
}

void CDownLoadThread::GetSuccessFileList(vector<string> &list)
{
	list = m_successFileList;
}

void CDownLoadThread::GetFailFileList(vector<string> &list)
{
	list = m_failFileList;
}

DWORD WINAPI CDownLoadThread::DownLoadThreadProc(LPVOID lpParameter)
{
	CDownLoadThread *ps = (CDownLoadThread*)lpParameter;
	if (ps)
	{
		ps->DownLoadFile();
	}
	return 0;
}

void CDownLoadThread::DownLoadFile()
{
	FTP_PARAMS fp;
	CCenterManager::GetInstance()->GetFtpParams(fp);

	while (!m_isStopDownLoad || (m_threadParams.size() > 0))
	{
		ThreadParams tp = m_threadParams.front();

		const char * videoPath = tp.saveFilePath.c_str();
		char fileName[256] = { 0 };
		char fileExt[20] = { 0 };
		_splitpath(videoPath, NULL, NULL, fileName, fileExt);

		strcat_s(fileName, fileExt);

	

		bool bRet = m_mc.DownloadToFile(tp.url.c_str(), videoPath);
	
		if (bRet)
		{

			vector<string> savedList;
			CCenterManager::GetInstance()->GetSavedVideoConfig(savedList);

			bool isExit = false;
			for (unsigned int i = 0; i < savedList.size(); ++i)
			{
				string p = savedList[i];
				if (p == fileName)
				{
					isExit = true;
					break;
				}

			}

			if (isExit)
			{
				m_threadParams.pop();
				continue;
			}

			CCenterManager::GetInstance()->RecordVideoFileName(fileName);

			m_successFileList.push_back(tp.saveFilePath);

			strcpy_s(fp.localFilePath, videoPath);
		
			//strcpy_s(fp.upLoadFileName, fileName);
			sprintf_s(fp.upLoadFileName, "\\MetaData\\Media\\%s", fileName);
			bRet = m_mc.FtpUpload(fp);
			
		}
		else
		{
			m_failFileList.push_back(tp.saveFilePath);
		}

		m_threadParams.pop();
	}
}
