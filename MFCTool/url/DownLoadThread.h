#pragma once
#include "CurlManager.h"
#include <queue>
typedef struct
{
	string url; //urls
	string file_name;
	string saveFilePath;// 保存的文件路径
	string checkMd5Value;//校验MD5
	string fileSize; //文件大小
}ThreadParams; //下载视频线程的参数

class CDownLoadThread
{
public:
	CDownLoadThread();
	~CDownLoadThread();

	void AddVideoFileToList(ThreadParams &tp);
	HANDLE GetThreadHandle();
	void AddFinished();
	void SetThreadTag(int index);
	int GetThreadTag();
	void GetSuccessFileList(vector<string> &list);
	void GetFailFileList(vector<string> &list);
private:
	CCurlManager m_mc;
	int m_thread_tag;
	bool m_isStopDownLoad;
	std::queue<ThreadParams> m_threadParams;
	static DWORD WINAPI DownLoadThreadProc(LPVOID lpParameter);
	HANDLE m_downLoadThread;
	void DownLoadFile();
	vector<string> m_successFileList;
	vector<string> m_failFileList;
};

