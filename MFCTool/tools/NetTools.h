#pragma once
#include "CmdTools.h"
#include "Icmp.h"
class NetTools
{
public:
	//ping
	static bool pingIp(const string &strIp,int nTimeout = 1200);
	
	//获取mac地址
	static string GetMacByIp(const string &strIp);
	
	//获取所有mac
	static bool GetAllMacAndIp(vector<pair<string, string> > &ipMacList);

};

