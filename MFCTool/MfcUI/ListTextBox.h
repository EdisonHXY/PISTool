#if !defined(AFX_LISTTEXTBOX_H__4A763700_B594_444A_9F09_A7E07AFF9280__INCLUDED_)
#define AFX_LISTTEXTBOX_H__4A763700_B594_444A_9F09_A7E07AFF9280__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define LVS_EX_DOUBLEBUFFER 0x00010000
/////////////////////////////////////////////////////////////////////////////
class CListTextBox : public CListBox
{
public:
	CListTextBox();
	virtual ~CListTextBox();
	
	int AddString(LPCTSTR lpszItem, COLORREF itemColor = RGB(10, 200, 10));
	void SetCenter(bool bCenter);
private:
	CString GetString(int iIndex);
	void ShowInfo(CListBox *pListBox, CString infoStr, COLORREF itemColor);
private:
	CFont m_ftText;
	COLORREF m_colBk;
	CBrush m_brBk;
	bool m_isCenter;
	int m_maxWidth;
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListTextBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CListTextBox)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
#endif 
