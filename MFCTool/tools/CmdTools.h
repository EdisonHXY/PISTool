#pragma once
class CmdTools
{
public:
	static bool ExecuteCmd(const string& strCmd,string &responStr)
	{
		responStr.clear();
		SECURITY_ATTRIBUTES sa;
		HANDLE hRead, hWrite;
		sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = NULL;
		sa.bInheritHandle = TRUE;
		if (!CreatePipe(&hRead, &hWrite, &sa, 0))
		{
			return false;
		}
		STARTUPINFO si = { sizeof(si) };
		PROCESS_INFORMATION pi;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;
		si.wShowWindow = SW_HIDE;
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		if (!CreateProcess(NULL, (char*)strCmd.data(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
		{
			return false;
		}
		CloseHandle(hWrite);
		char buffer[4096];
		memset(buffer, 0, 4096);
	
		DWORD byteRead;
		while (true)
		{
			if (ReadFile(hRead, buffer, 4095, &byteRead, NULL) == NULL)
			{
				break;
			}
			responStr += buffer;
		}
		CloseHandle(hRead);
		return true;
	}

};

