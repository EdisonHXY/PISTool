// PISTools.h : PISTools DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������




// CPISToolsApp
// �йش���ʵ�ֵ���Ϣ������� PISTools.cpp
//

class CPISToolsApp : public CWinApp
{
public:
	CPISToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

