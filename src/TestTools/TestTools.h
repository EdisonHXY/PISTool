
// TestTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestToolsApp: 
// �йش����ʵ�֣������ TestTools.cpp
//

class CTestToolsApp : public CWinApp
{
public:
	CTestToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestToolsApp theApp;