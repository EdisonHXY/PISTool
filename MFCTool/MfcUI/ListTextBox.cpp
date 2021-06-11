#include "stdafx.h"
#include "ListTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
CListTextBox::CListTextBox()
{
	m_ftText.CreateFont(14, 0, 0, 0, 0, FALSE, FALSE, 0, 0, 0, 0, 0, 0, _TEXT("ËÎÌו"));
	m_colBk = RGB(25, 0, 0);
	m_brBk.CreateSolidBrush(m_colBk);
	m_isCenter = true;
	m_maxWidth = 0;

}

CListTextBox::~CListTextBox()
{

}


afx_msg LRESULT CListTextBox::OnAddstringdraw(WPARAM wParam, LPARAM lParam)
{
	LISTTEXTBOXTRSFERDATRA data = *(LISTTEXTBOXTRSFERDATRA*)lParam;
	SetCenter(data.bCenter);

	SetRedraw(FALSE);
	if (GetCount() > 300)
		DeleteString(0);

	AddString(data.strText.data(), data.itemColor);
	SetRedraw(TRUE);

	return 0;
}

void  CListTextBox::ShowInfo(CListBox *pListBox, CString infoStr, COLORREF itemColor)
{
	int len = infoStr.GetLength();
	
	CDC *pDC = pListBox->GetDC();
	if (!pDC)
	{
		return;
	}
	CFont *pFont = pListBox->GetFont();
	if (!pFont)
	{
		return;
	}
	CFont *pOldFont = pDC->SelectObject(pFont);

	CSize sz = pDC->GetTextExtent(infoStr, len);

	while (sz.cx > m_maxWidth)
	{
		len--;
		sz = pDC->GetTextExtent(infoStr, len);
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
	int nIndex = pListBox->AddString(infoStr.Left(len));
	if (nIndex >= 0)
		SetItemData(nIndex, itemColor);

	int strLen = infoStr.GetLength();
	if (len != strLen)
	{
		ShowInfo(pListBox, infoStr.Right(strLen - len),itemColor);
	}
}
int CListTextBox::AddString(LPCTSTR lpszItem, COLORREF itemColor /*= RGB(10, 200, 10)*/)
{
	if (m_maxWidth <=0)
	{
		CRect clientRect;
		GetClientRect(&clientRect);
		m_maxWidth = clientRect.Width();
	}


	CString infoStr = lpszItem;
	int len = infoStr.GetLength();
	if (len > 256)
	{
		infoStr = infoStr.Left(256);
		infoStr += "....";
	}


	ShowInfo(this, infoStr, itemColor);
	// Add the string to the list box
// 	int nIndex = CListBox::AddString(lpszItem);
// 
// 	// save color data
// 	if (nIndex >= 0)
// 		SetItemData(nIndex, itemColor);
// 
// 	return nIndex;
	return 0;
}

void CListTextBox::SetCenter(bool bCenter)
{
	m_isCenter = bCenter;
}

BEGIN_MESSAGE_MAP(CListTextBox, CListBox)
	//{{AFX_MSG_MAP(CListTextBox)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_ADDSTRINGDRAW, &CListTextBox::OnAddstringdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CListTextBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	if(lpDrawItemStruct->itemID >= (UINT)GetCount())
		return;

	CString strText;
	GetText(lpDrawItemStruct->itemID, strText);

//	LPCTSTR lpszText = (LPCTSTR)lpDrawItemStruct->itemData;
//	if(lpszText == NULL)
//		return;

	if(strText.Compare("") == 0)
		return;
	

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	COLORREF clrOld;

	// get color info from item data
	COLORREF clrNew = (COLORREF)(lpDrawItemStruct->itemData);

	// item selected ?
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush brush(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item deselected ?
	if (!(lpDrawItemStruct->itemState & ODS_SELECTED) &&
		(lpDrawItemStruct->itemAction & ODA_SELECT))
	{
		CBrush brush(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&lpDrawItemStruct->rcItem, &brush);
	}

	// item has focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	// lost focus ?
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
	}

	// set the background mode to TRANSPARENT
	int nBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		clrOld = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else
		if (lpDrawItemStruct->itemState & ODS_DISABLED)
			clrOld = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		else
			clrOld = pDC->SetTextColor(clrNew);

	// get item text
	GetText(lpDrawItemStruct->itemID, strText);
	CRect rect = lpDrawItemStruct->rcItem;

	// text format
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (m_isCenter)
	{
		nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	}
	
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;

	// draw the text
	pDC->DrawText(strText, -1, &rect, nFormat);

	// restore old values
	pDC->SetTextColor(clrOld);
	pDC->SetBkMode(nBkMode);
}

void CListTextBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	lpMeasureItemStruct->itemHeight = 28;
}


BOOL CListTextBox::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcClient;
	GetClientRect(rcClient);
	pDC->FillRect(rcClient, &CBrush(m_colBk));
	return TRUE;
	
	return CListBox::OnEraseBkgnd(pDC);
}


HBRUSH CListTextBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetBkColor(m_colBk);
	return (HBRUSH)m_brBk.GetSafeHandle();
}

CString CListTextBox::GetString(int iIndex)
{
	CString strText;
	ASSERT(::IsWindow(m_hWnd));
	GetText(iIndex, strText.GetBufferSetLength(GetTextLen(iIndex)));

	return strText;
}


