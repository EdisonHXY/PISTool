/***********************************************************************************
*	��Ȩ����(c)2014 �ڻ����������
*	�ļ�����:	CtrlLib.cpp
*	�ļ�˵��:	�豸���ƿ�����������ļ�
*	��ǰ�汾:	1.0.20140327
*	����:		��Ρ��
*	��������:	2014-03-27
*************************************************************************************/

#include "StdAfx.h"
#include "CtrlLib.h"

//////////////////////////////////////////////////////////////////////////////////////////////
CCtrlLib::CCtrlLib(void)
{
	m_hctrlDll = NULL;
	ctrl_InitDll = NULL;
	ctrl_UninitDll = NULL;
	ctrl_GetContent = NULL;
	ctrl_SetContent = NULL;
	ctrl_CheckStatus = NULL;
	ctrl_ExecuteCmd = NULL;
	ctrl_SupportCmd = NULL;
	ctrl_GetPlayContent = NULL;
}

CCtrlLib::~CCtrlLib(void)
{
	if(m_hctrlDll != NULL)
		FreeLibrary(m_hctrlDll);
	
}


BOOL CCtrlLib::LoadFunction(const char *szPath)
{

 	m_hctrlDll = LoadLibrary(szPath);
	if(m_hctrlDll != NULL)
	{
		ctrl_InitDll = (lpctrl_InitDll)GetProcAddress(m_hctrlDll, "ctrl_InitDll");
		ctrl_UninitDll = (lpctrl_UninitDll)GetProcAddress(m_hctrlDll, "ctrl_UninitDll");
		ctrl_GetContent = (lpctrl_GetContent)GetProcAddress(m_hctrlDll, "ctrl_GetContent");
		ctrl_SetContent = (lpctrl_SetContent)GetProcAddress(m_hctrlDll, "ctrl_SetContent");
		ctrl_CheckStatus = (lpctrl_CheckStatus)GetProcAddress(m_hctrlDll, "ctrl_CheckStatus");
		ctrl_ExecuteCmd = (lpctrl_ExecuteCmd)GetProcAddress(m_hctrlDll, "ctrl_ExecuteCmd");
		ctrl_SupportCmd = (lpctrl_SupportCmd)GetProcAddress(m_hctrlDll, "ctrl_SupportCmd");
		ctrl_GetPlayContent = (lpctrl_GetPlayContent)GetProcAddress(m_hctrlDll, "ctrl_GetPlayContent");
	}
	else
	{
		return FALSE;
	}
	

	return TRUE;
}

bool CCtrlLib::isLoadSuccess()
{
	return ctrl_InitDll && ctrl_SetContent && ctrl_CheckStatus && ctrl_ExecuteCmd && ctrl_GetPlayContent;
}
