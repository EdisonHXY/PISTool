#pragma once
#include "CmdTools.h"
#include "Icmp.h"
class NetTools
{
public:
	//ping
	static bool pingIp(const string &strIp,int nTimeout = 1200);
	
	//��ȡmac��ַ
	static string GetMacByIp(const string &strIp);
	
	//��ȡ����mac
	static bool GetAllMacAndIp(vector<pair<string, string> > &ipMacList);

};

