// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_crTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBkColor = ::GetSysColor(COLOR_WINDOW);
	m_brBkGround.DeleteObject();
	m_brBkGround.CreateSolidBrush(m_crBkColor);

	//m_hCursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
}

CColorStatic::~CColorStatic()
{
}

IMPLEMENT_DYNAMIC(CColorStatic, CStatic)

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	CRect rect;
	pDC->SetTextColor(m_crTextColor);
	pDC->SetBkColor(m_crBkColor);
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->FillSolidRect(&rect, m_crBkColor);
	m_brBkGround.DeleteObject();
	m_brBkGround.CreateSolidBrush(m_crBkColor);

	return (HBRUSH)m_brBkGround.m_hObject;
}

void CColorStatic::SetTextColor(COLORREF crTextColor)
{
	m_crTextColor = crTextColor;
	Invalidate();
}

void CColorStatic::SetBkColor(COLORREF crBkColor)
{
	m_crBkColor = crBkColor;
	Invalidate();
}

COLORREF CColorStatic::GetBkColor()
{
	return m_crBkColor;
}

COLORREF CColorStatic::GetTextColor()
{
	return m_crTextColor;
}

BOOL CColorStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
    //::SetCursor( m_hCursor );
	return true;
}
