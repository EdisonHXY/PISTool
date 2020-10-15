#if !defined(AFX_COLORSTATIC_H__AC6371A8_A0AB_4ADF_A35C_811F4BACA233__INCLUDED_)
#define AFX_COLORSTATIC_H__AC6371A8_A0AB_4ADF_A35C_811F4BACA233__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();
	DECLARE_DYNAMIC(CColorStatic)

// Attributes
public:
	COLORREF m_crTextColor;
	COLORREF m_crBkColor;
	CBrush m_brBkGround;

	HCURSOR m_hCursor;

// Operations
public:
	void SetTextColor(COLORREF crTextColor);
	void SetBkColor(COLORREF crBkColor);
	COLORREF GetBkColor();
	COLORREF GetTextColor();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__AC6371A8_A0AB_4ADF_A35C_811F4BACA233__INCLUDED_)
