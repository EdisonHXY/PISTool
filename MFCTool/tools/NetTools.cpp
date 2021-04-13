#include "stdafx.h"
#include "NetTools.h"



bool NetTools::pingIp(const string &strIp,int nTimeOut)
{
	static lpICMP_SendEcho Icmp_SendEcho = 0;
	static HANDLE hIcmp = 0;
	static bool bInit = false;
	if (!bInit)
	{
		HINSTANCE hDllIcmp = LoadLibrary("Icmp.dll");
		if (hDllIcmp == NULL)
			return false;
		lpICMP_CreateFile Icmp_CreateFile = (lpICMP_CreateFile)GetProcAddress(hDllIcmp, "IcmpCreateFile");
		if (Icmp_CreateFile == NULL)
			return false;
		lpICMP_CloseHandle Icmp_CloseHandle = (lpICMP_CloseHandle)GetProcAddress(hDllIcmp, "IcmpCloseHandle");
		if (Icmp_CloseHandle == NULL)
			return false;
		Icmp_SendEcho = (lpICMP_SendEcho)GetProcAddress(hDllIcmp, "IcmpSendEcho");
		if (Icmp_SendEcho == NULL)
			return false;
		hIcmp = Icmp_CreateFile();
		bInit = hIcmp !=0;
	}



	IP_OPTION_INFORMATION_ICMP ipInfo;
	PICMP_ECHO_REPLY_ICMP pIcmpEcho;
	int iSize = sizeof(ICMP_ECHO_REPLY_ICMP) + 8;
	char szData[sizeof(ICMP_ECHO_REPLY_ICMP) + 8];
	BOOL bRetsult = FALSE;

	pIcmpEcho = (PICMP_ECHO_REPLY_ICMP)szData;
	ipInfo.Ttl = 255;
	ipInfo.Tos = 0;
	ipInfo.Flags = 0;
	ipInfo.OptionsSize = 0;
	ipInfo.OptionsData = NULL;
	pIcmpEcho->Status = 0;
	Icmp_SendEcho(hIcmp, inet_addr(strIp.data()), NULL, 0, &ipInfo, pIcmpEcho, iSize, nTimeOut);
	bRetsult = (pIcmpEcho->Status == 0);

	

	return bRetsult;
}

// 使用字符串分割
static void Stringsplit(const string& str, const string& splits, vector<string>& res,bool bEmptyNoAdd = false)
{
	if (str == "")		return;
	//在字符串末尾也加入分隔符，方便截取最后一段
	string strs = str + splits;
	size_t pos = strs.find(splits);
	int step = splits.size();

	// 若找不到内容则字符串搜索函数返回 npos
	while (pos != strs.npos)
	{
		string temp = strs.substr(0, pos);
		if (!bEmptyNoAdd)
		{
			res.push_back(temp);
		}
		else if (temp.length() > 0)
		{
			res.push_back(temp);
		}
		
		//去掉已分割的字符串,在剩下的字符串中进行分割
		strs = strs.substr(pos + step, strs.size());
		pos = strs.find(splits);
	}
}
std::string NetTools::GetMacByIp(const string &strIp)
{
	string str = "arp -a " + strIp;
	string resPon;
	bool bRet = CmdTools::ExecuteCmd(str, resPon);

	if (bRet)
	{
		vector<string> strList;
		Stringsplit(resPon, "\r\n", strList,true);
		int nSize = strList.size();
		if (nSize < 3)
		{
			bRet = false;
		}
		else
		{
			string str = strList[nSize - 1];
			strList.clear();
			Stringsplit(str, " ", strList,true);
			nSize = strList.size();
			if (nSize < 3)
			{
				bRet = false;
			}
			else
			{
				resPon = strList[nSize - 2];
			}
			

		}
	}
	
	resPon = bRet ? resPon: "00-00-00-00-00-00";
	

	return resPon;
}

bool NetTools::GetAllMacAndIp(vector<pair<string, string> > &ipMacList)
{
	ipMacList.clear();
	string str = "arp -a ";
	string resPon;
	bool bRet = CmdTools::ExecuteCmd(str, resPon);

	if (bRet)
	{
		vector<string> strList;
		Stringsplit(resPon, "\r\n", strList, true);
		int nSize = strList.size();
		if (nSize < 3)
		{
			return false;
		}
		else
		{

			for (int i =  2; i < nSize;++i)
			{
				string str = strList[i];
				vector<string> tmpList;
				Stringsplit(str, " ", tmpList, true);
				
				if (tmpList.size() < 3)
				{
					continue;
				}
				else
				{
					
					ipMacList.push_back(make_pair(tmpList[0], tmpList[1]));
				}

			}

			


		}
	}


	return bRet;
}
