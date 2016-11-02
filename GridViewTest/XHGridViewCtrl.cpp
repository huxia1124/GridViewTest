//
//Copyright(c) 2007-2016. Huan Xia
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files(the "Software"), to deal in the Software without restriction, including without limitation
//the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
//and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions
//of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
//THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.

#include "stdafx.h"
#include "XHGridViewCtrl.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXHGridViewBandCtrl
BEGIN_MESSAGE_MAP(CXHGridViewBandCtrl,CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CXHGridViewBandCtrl::CXHGridViewBandCtrl()
{
	m_BandItem.SetItemType(XHGRIDVIEW_ITEMTYPE_BAND);

	ZeroMemory(&_m_BandCtrlInfo, sizeof(_m_BandCtrlInfo));
	_m_BandCtrlInfo.bLButtonPress = FALSE;
	_m_BandCtrlInfo.pPreHitSideBaseItem = NULL;
}

CXHGridViewBandCtrl::~CXHGridViewBandCtrl()
{
}

CXHGridViewItemStructure* CXHGridViewBandCtrl::GetBandItemStructure()
{
	return &m_BandItem;
}

BOOL CXHGridViewBandCtrl::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
	LPCTSTR pszCreateClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));
	CWnd *pThisWnd = this;
	m_BandItem.SetParentGridView((CXHGridView*)((CXHGridViewHeaderCtrl*)pParentWnd)->GetParent());
	return pThisWnd->Create(pszCreateClass,lpszWindowName,dwStyle,rect,pParentWnd,0);
}

void CXHGridViewBandCtrl::ResetWindowSize()
{
	if(((CXHGridViewHeaderCtrl*)GetParent())->GetHeaderItemStructure()->IsBand())
	{
		CRect rcWholeItem;
		m_BandItem.GetItemRect(rcWholeItem);
		SetWindowPos(NULL,0,0,rcWholeItem.Width(),rcWholeItem.Height(),SWP_NOMOVE|SWP_NOREDRAW);
//		TRACE1("*CXHGridViewBandCtrl::ResetWindowSize() - Banded - Width = %d\n",rcWholeItem.Width());
		CXHGridView *pGridView = ((CXHGridView*)GetParent()->GetParent());
		pGridView->ResetHScrollBar();
	}
	else
	{
		MoveWindow(CRect(-1,-1,-1,-1));
	}
//	TRACE1("CXHGridViewBandCtrl::ResetWindowSize() - End  %d\n",GetTickCount());
}

int CXHGridViewBandCtrl::GetHeight()
{
	if(((CXHGridView*)(GetParent()->GetParent()))->GetViewType() == XHGRIDVIEW_DETAIL)
		return m_BandItem.GetHeight();
	else
		return 0;
}

int CXHGridViewBandCtrl::GetWidth()
{
	CRect rcWindow;
	GetWindowRect(rcWindow);
	return rcWindow.Width();
}

CXHGridView* CXHGridViewBandCtrl::GetParentGirdView()
{
	return (CXHGridView*)GetParent()->GetParent();
}

/* Message Handler for CXHGridViewBandCtrl */
void CXHGridViewBandCtrl::OnPaint()
{
//	TRACE1("CXHGridViewBandCtrl::OnPaint()  %d\n",GetTickCount());
	CPaintDC dc(this);
	CXHRect rcBand;
	m_BandItem.GetItemRect(rcBand);
	CXHMemDC mdc(&dc,rcBand);
	m_BandItem.Draw(&mdc,0,0);
	mdc.DrawTo(&dc,0,0);
}

void CXHGridViewBandCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXHGridView *pParentGridView = GetParentGirdView();
	pParentGridView->SetFocus();
	
	_m_BandCtrlInfo.ptLButtonDown.x = point.x;
	_m_BandCtrlInfo.ptLButtonDown.y = point.y;
	_m_BandCtrlInfo.ptPreMove.x = point.x;
	_m_BandCtrlInfo.ptPreMove.y = point.y;
	_m_BandCtrlInfo.bLButtonPress = TRUE;

	CXHGridViewBaseItem *pBaseR = NULL;
	CXHGridViewLeafItem *pLeafR = NULL;
	if(m_BandItem.BaseSideHitTest(&_m_BandCtrlInfo.pPreHitSideBaseItem,&pBaseR,point))	//Hit the splitter
	{
		SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		SetCapture();

		CXHGridView *pGridView = ((CXHGridView*)(GetParent()->GetParent()));
		//Clip cursor
		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		CRect rcClip(ptScreen.x - _m_BandCtrlInfo.pPreHitSideBaseItem->GetWidth() + pGridView->GetMinLeafWidth() * _m_BandCtrlInfo.pPreHitSideBaseItem->GetMaxBaseItemLeafCount()
			,0
			,GetSystemMetrics(SM_CXSCREEN)
			,GetSystemMetrics(SM_CYSCREEN));
		if(rcClip.right < rcClip.left)
			rcClip.right = rcClip.left;
		ClipCursor(rcClip);
		
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		dc.MoveTo(point.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),0);
		dc.LineTo(point.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);
	}
	else if(m_BandItem.BotomLeafSideHitTest(&_m_BandCtrlInfo.pPreHitSideBottomLeafItem,&_m_BandCtrlInfo.pPreHitSideBottomLeafItemRight,point))	//hit the leaf splitter
	{
		SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		SetCapture();

		_m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->RefactAllLeafItemWidth();

		//Clip cursor
		CPoint ptScreen = point;
		ClientToScreen(&ptScreen);
		CRect rcClip(ptScreen.x - _m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetWidth() + ((CXHGridView*)(GetParent()->GetParent()))->GetMinLeafWidth()
			,0
			,ptScreen.x + _m_BandCtrlInfo.pPreHitSideBottomLeafItemRight->GetWidth() - ((CXHGridView*)(GetParent()->GetParent()))->GetMinLeafWidth()
			,GetSystemMetrics(SM_CYSCREEN));
		if(rcClip.right < rcClip.left)
			rcClip.right = rcClip.left;
		ClipCursor(rcClip);
		
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(point.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),iTop);
		dc.LineTo(point.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);
	}
}

void CXHGridViewBandCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bRedraw = FALSE;
	if(_m_BandCtrlInfo.pPreHitSideBaseItem != NULL)
	{
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),0);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);
		
		int iWidthLeft = _m_BandCtrlInfo.pPreHitSideBaseItem->GetWidth() + (point.x - _m_BandCtrlInfo.ptLButtonDown.x);
		_m_BandCtrlInfo.pPreHitSideBaseItem->SetWidth(iWidthLeft);

		bRedraw = TRUE;

	}
	else if(_m_BandCtrlInfo.pPreHitSideBottomLeafItem != NULL)
	{
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),iTop);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);
		
		int iWidthLeft = _m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetWidth() + (point.x - _m_BandCtrlInfo.ptLButtonDown.x);
		_m_BandCtrlInfo.pPreHitSideBottomLeafItem->SetWidth(iWidthLeft);
		int iWidthRight = _m_BandCtrlInfo.pPreHitSideBottomLeafItemRight->GetWidth() - (point.x - _m_BandCtrlInfo.ptLButtonDown.x);
		_m_BandCtrlInfo.pPreHitSideBottomLeafItemRight->SetWidth(iWidthRight);

		bRedraw = TRUE;
	}
	
	if(bRedraw)
	{
		ResetWindowSize();
		((CXHGridViewHeaderCtrl*)GetParent())->ResetWindowSize();
		RedrawWindow();
		GetParent()->RedrawWindow();
		GetParent()->GetParent()->RedrawWindow();
	}
	
	_m_BandCtrlInfo.bLButtonPress = FALSE;
	_m_BandCtrlInfo.pPreHitSideBaseItem = NULL;
	_m_BandCtrlInfo.pPreHitSideBottomLeafItem = NULL;
	ClipCursor(NULL);
	ReleaseCapture();
}

void CXHGridViewBandCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(_m_BandCtrlInfo.pPreHitSideBaseItem == NULL)
	{
		CXHGridViewBaseItem *pBaseL = NULL;
		CXHGridViewBaseItem *pBaseR = NULL;
		if(m_BandItem.BaseSideHitTest(&pBaseL,&pBaseR,point))	//Hit the splitter
		{
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}
	}
	else
	{
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),0);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());

		_m_BandCtrlInfo.ptPreMove.x = point.x;
		_m_BandCtrlInfo.ptPreMove.y = point.y;

		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),0);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		
		dc.SelectObject(pOldPen);
	}


	if(_m_BandCtrlInfo.pPreHitSideBottomLeafItem == NULL)
	{
		CXHGridViewLeafItem *pLeafL = NULL;
		CXHGridViewLeafItem *pLeafR = NULL;
		if(m_BandItem.BotomLeafSideHitTest(&pLeafL,&pLeafR,point))	//Hit the splitter
		{
			SetCursor(LoadCursor(NULL,IDC_SIZEWE));
		}
	}
	else
	{
		CRect rcParent;
		((CXHGridView*)GetParent()->GetParent())->GetClientRect(rcParent);
		CWindowDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_BandCtrlInfo.pPreHitSideBottomLeafItem->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),iTop);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());

		_m_BandCtrlInfo.ptPreMove.x = point.x;
		_m_BandCtrlInfo.ptPreMove.y = point.y;

		dc.MoveTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),iTop);
		dc.LineTo(_m_BandCtrlInfo.ptPreMove.x - GetParent()->GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		
		dc.SelectObject(pOldPen);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewHeaderCtrl


BEGIN_MESSAGE_MAP(CXHGridViewHeaderCtrl,CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

CXHGridViewHeaderCtrl::CXHGridViewHeaderCtrl()
{
	m_HeaderItem.SetItemType(XHGRIDVIEW_ITEMTYPE_HEADER);

	_m_HeaderCtrlInfo.pPreHitLeafItem = NULL;
	_m_HeaderCtrlInfo.pPreHitSideLeafLeft = NULL;
	_m_HeaderCtrlInfo.pPreHitSideLeafRight = NULL;
	_m_HeaderCtrlInfo.iBandHeight = 0;
	m_pParentGridView = NULL;
}

CXHGridViewHeaderCtrl::~CXHGridViewHeaderCtrl()
{
}

BOOL CXHGridViewHeaderCtrl::Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd)
{
	LPCTSTR pszCreateClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW, ::LoadCursor(NULL, IDC_ARROW));
	CWnd *pThisWnd = this;
	m_pParentGridView = (CXHGridView*)pParentWnd;
	m_HeaderItem.SetParentGridView(m_pParentGridView);
	return pThisWnd->Create(pszCreateClass,lpszWindowName,dwStyle,rect,pParentWnd,0);
}

void CXHGridViewHeaderCtrl::ResetWindowSize()
{
	ReCalcLocation();
	CRect rcWholeItem;
	m_HeaderItem.GetItemRect(rcWholeItem);
	rcWholeItem.bottom += _m_HeaderCtrlInfo.iBandHeight;
	OffsetRect(rcWholeItem, - (m_pParentGridView->GetScrollPos(SB_HORZ)),0);
//	SetWindowPos(NULL,0,0,rcWholeItem.Width(),rcWholeItem.Height(),SWP_NOMOVE);
	MoveWindow(rcWholeItem);
}
void CXHGridViewHeaderCtrl::ReCalcLocation()
{
	if(m_HeaderItem.IsBand())
		_m_HeaderCtrlInfo.iBandHeight = m_BandCtrl.GetBandItemStructure()->GetHeight();
	else
		_m_HeaderCtrlInfo.iBandHeight = 0;
}

CXHGridViewItemStructure* CXHGridViewHeaderCtrl::GetHeaderItemStructure()
{
	return &m_HeaderItem;
}

CXHGridViewItemStructure* CXHGridViewHeaderCtrl::GetBandItemStructure()
{
	return m_BandCtrl.GetBandItemStructure();
}

BOOL CXHGridViewHeaderCtrl::Band()
{
	if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
	{
		BOOL bBand =  m_HeaderItem.SetBand(m_BandCtrl.GetBandItemStructure());
		m_BandCtrl.ResetWindowSize();
		ResetWindowSize();
		m_BandCtrl.ShowWindow(SW_SHOW);
		m_pParentGridView->ResetHScrollBar();
		return bBand;
	}
	else
	{
		m_HeaderItem.SetBand(NULL);
		m_BandCtrl.ShowWindow(SW_HIDE);
		return FALSE;
	}
}

void CXHGridViewHeaderCtrl::ProcessMouseDown(CPoint point)
{
	_m_HeaderCtrlInfo.ptLButtonDown.x = point.x;
	_m_HeaderCtrlInfo.ptLButtonDown.y = point.y;
	_m_HeaderCtrlInfo.ptPreMove.x = point.x;
	_m_HeaderCtrlInfo.ptPreMove.y = point.y;
	_m_HeaderCtrlInfo.bLButtonPress = TRUE;

	if(m_HeaderItem.LeafSideHitTest(&_m_HeaderCtrlInfo.pPreHitSideLeafLeft,&_m_HeaderCtrlInfo.pPreHitSideLeafRight,CPoint(point.x,point.y - _m_HeaderCtrlInfo.iBandHeight)))
	{
		SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
		SetCapture();

		_m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->RefactAllLeafItemWidth();

		//Clip cursor
		CRect rcClip;
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		{
			CPoint ptScreen = point;
			ClientToScreen(&ptScreen);
			rcClip.SetRect(ptScreen.x - _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetWidth() + ((CXHGridView*)(GetParent()))->GetMinLeafWidth()
				,0
				,ptScreen.x + _m_HeaderCtrlInfo.pPreHitSideLeafRight->GetWidth() - ((CXHGridView*)(GetParent()))->GetMinLeafWidth()
				,GetSystemMetrics(SM_CYSCREEN));
			if(rcClip.right < rcClip.left)
				rcClip.right = rcClip.left;
		}
		else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
		{
			CPoint ptScreen = point;
			ClientToScreen(&ptScreen);
			rcClip.SetRect(ptScreen.x - _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetWidth() + ((CXHGridView*)(GetParent()))->GetMinLeafWidth()
				,0
				,GetSystemMetrics(SM_CXSCREEN)
				,GetSystemMetrics(SM_CYSCREEN));
		}
		ClipCursor(rcClip);
		
		//Draw the line
		CRect rcParent;
		((CXHGridView*)GetParent())->GetClientRect(rcParent);
		CClientDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = 0;
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
			pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(point.x - GetParent()->GetScrollPos(SB_HORZ),iTop + m_BandCtrl.GetHeight());
		dc.LineTo(point.x - GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);

//		TRACE2("Down %d,%d\n",point.x - GetParent()->GetScrollPos(SB_HORZ),point.y);
		return;
	}
	if(_m_HeaderCtrlInfo.pPreHitLeafItem = m_HeaderItem.LeafHitTest(CPoint(point.x,point.y - _m_HeaderCtrlInfo.iBandHeight)))
	{
		_m_HeaderCtrlInfo.pPreHitLeafItem->ModifyState(0,XHGRIDVIEW_LEAF_STATE_PUSHED);
		RedrawWindow();
		SetCapture();
	}
}

void CXHGridViewHeaderCtrl::ProcessMouseMove(CPoint point)
{
	if(_m_HeaderCtrlInfo.pPreHitSideLeafLeft == NULL)
	{
		if(_m_HeaderCtrlInfo.pPreHitLeafItem == NULL)
		{
			
			CXHGridViewLeafItem *pLeafL = NULL;
			CXHGridViewLeafItem *pLeafR = NULL;
			if(m_HeaderItem.LeafSideHitTest(&pLeafL,&pLeafR,CPoint(point.x,point.y - _m_HeaderCtrlInfo.iBandHeight)))
				SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
		}
	}
	else
	{
		//Draw the line.
		CRect rcParent;
		((CXHGridView*)GetParent())->GetClientRect(rcParent);
		CClientDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = 0;
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
			iTop = pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),iTop + m_BandCtrl.GetHeight());
		dc.LineTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());

//		TRACE2("M- %d,%d     ",_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),_m_HeaderCtrlInfo.ptPreMove.y);

		_m_HeaderCtrlInfo.ptPreMove.x = point.x;
		_m_HeaderCtrlInfo.ptPreMove.y = point.y;

		dc.MoveTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),iTop + m_BandCtrl.GetHeight());
		dc.LineTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		
		dc.SelectObject(pOldPen);
//		TRACE2("M+ %d,%d\n",_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),_m_HeaderCtrlInfo.ptPreMove.y);
		return;
	}
	if(_m_HeaderCtrlInfo.pPreHitLeafItem)
	{
		DWORD dwOldState = _m_HeaderCtrlInfo.pPreHitLeafItem->GetState();
		if(m_HeaderItem.LeafHitTest(CPoint(point.x,point.y - _m_HeaderCtrlInfo.iBandHeight)) != _m_HeaderCtrlInfo.pPreHitLeafItem)
			_m_HeaderCtrlInfo.pPreHitLeafItem->ModifyState(XHGRIDVIEW_LEAF_STATE_PUSHED,0);
		else
			_m_HeaderCtrlInfo.pPreHitLeafItem->ModifyState(0,XHGRIDVIEW_LEAF_STATE_PUSHED);
		if((dwOldState & XHGRIDVIEW_LEAF_STATE_PUSHED) ^ (_m_HeaderCtrlInfo.pPreHitLeafItem->GetState() & XHGRIDVIEW_LEAF_STATE_PUSHED))
			RedrawWindow();
	}
}

void CXHGridViewHeaderCtrl::ProcessMouseUp(CPoint point)
{
	if(_m_HeaderCtrlInfo.pPreHitSideLeafLeft != NULL)
	{
		//Draw the line.
		CRect rcParent;
		((CXHGridView*)GetParent())->GetClientRect(rcParent);
		CClientDC dc(GetParent()->GetParent());
		CPen pen(PS_SOLID,1,RGB(255,255,255));
		CPen *pOldPen = dc.SelectObject(&pen);
		dc.SetROP2(R2_XORPEN);
		CXHGridViewSubItems *pSubItems = _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems();
		int iTop = 0;
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
			iTop = pSubItems->GetSubItemHeight(0,pSubItems->GetSubItemIndex(_m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetParentLeafItems()->GetParentSubItem()));
		dc.MoveTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),iTop + m_BandCtrl.GetHeight());
		dc.LineTo(_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),rcParent.Height());
		dc.SelectObject(pOldPen);
		
		//Adjust the Width
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		{	
			int iWidthLeft = _m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetWidth() + (point.x - _m_HeaderCtrlInfo.ptLButtonDown.x);
			_m_HeaderCtrlInfo.pPreHitSideLeafLeft->SetWidth(iWidthLeft);
			int iWidthRight = _m_HeaderCtrlInfo.pPreHitSideLeafRight->GetWidth() - (point.x - _m_HeaderCtrlInfo.ptLButtonDown.x);
			_m_HeaderCtrlInfo.pPreHitSideLeafRight->SetWidth(iWidthRight);
		}
		else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
		{
			_m_HeaderCtrlInfo.pPreHitSideLeafLeft->SetWidth(_m_HeaderCtrlInfo.pPreHitSideLeafLeft->GetWidth() + (point.x - _m_HeaderCtrlInfo.ptLButtonDown.x));
			ResetWindowSize();
			m_pParentGridView->ResetHScrollBar();
		}


//		TRACE2("Up %d,%d\n",_m_HeaderCtrlInfo.ptPreMove.x - GetParent()->GetScrollPos(SB_HORZ),_m_HeaderCtrlInfo.ptPreMove.y);
		SetRedraw(FALSE);
		GetParent()->RedrawWindow();
		SetRedraw(TRUE);
	}

	_m_HeaderCtrlInfo.bLButtonPress = FALSE;
	_m_HeaderCtrlInfo.pPreHitSideLeafLeft = NULL;
	_m_HeaderCtrlInfo.pPreHitSideLeafRight = NULL;

	if(_m_HeaderCtrlInfo.pPreHitLeafItem)
	{
		_m_HeaderCtrlInfo.pPreHitLeafItem->ModifyState(XHGRIDVIEW_LEAF_STATE_PUSHED,0);

		//Trigger an OnHeaderClick message roution
		if(_m_HeaderCtrlInfo.pPreHitLeafItem == m_HeaderItem.LeafHitTest(CPoint(point.x,point.y - _m_HeaderCtrlInfo.iBandHeight)))
			m_pParentGridView->SendMessage(WM_XHGRIDVIEW_SPECIAL_MESSAGE,(WPARAM)_m_HeaderCtrlInfo.pPreHitLeafItem,(LPARAM)XHGRIDVIEW_MESSAGE_HEADERCLICK);
	}
	_m_HeaderCtrlInfo.pPreHitLeafItem = NULL;

	CRect rcWindow;
	GetClientRect(rcWindow);

	RedrawWindow(CRect(0,GetBandHeight(),rcWindow.right,rcWindow.bottom));
	ReleaseCapture();
	ClipCursor(NULL);
}

BOOL CXHGridViewHeaderCtrl::IsBandVisible()
{
	if(!m_HeaderItem.IsBand())
		return FALSE;
	else if(m_BandCtrl.IsWindowVisible())
		return TRUE;


	//Other Conditions
	ASSERT(FALSE);
	return FALSE;
}

/* Message Handler for CXHGridViewHeaderCtrl */
void CXHGridViewHeaderCtrl::OnPaint()
{	
//	TRACE1("CXHGridViewHeaderCtrl::OnPaint()  %d\n",GetTickCount());

	if(IsBandVisible())
	{	
		m_BandCtrl.RedrawWindow();
		
		CRect rcBand;
		m_BandCtrl.GetBandItemStructure()->GetItemRect(rcBand);
		ValidateRect(rcBand);
	}

	CPaintDC dc(this);
	
	ResetWindowSize();

	CXHRect rcHeaderItem;
	m_HeaderItem.GetItemRect(rcHeaderItem);
	CXHMemDC mdc(&dc,rcHeaderItem);
	CBrush brush(::GetSysColor(COLOR_BTNFACE));
	CPen pen;
	pen.CreateStockObject(NULL_PEN);
	CPen *pOldPen = mdc.SelectObject(&pen);
	mdc.FillRect(rcHeaderItem,&brush);
	m_HeaderItem.Draw(&mdc,0,0);
	mdc.DrawTo(&dc,0,_m_HeaderCtrlInfo.iBandHeight);
	mdc.SelectObject(pOldPen);
	((CXHGridView*)GetParent())->RedrawUnusedArea();

}

void CXHGridViewHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_pParentGridView->SetFocus();

	ProcessMouseDown(point);
	CWnd::OnLButtonDown(nFlags,point);
}

void CXHGridViewHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ProcessMouseUp(point);
	CWnd::OnLButtonUp(nFlags,point);
}

void CXHGridViewHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	ProcessMouseMove(point);
	CWnd::OnMouseMove(nFlags,point);
}

int CXHGridViewHeaderCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	VERIFY(m_BandCtrl.Create(_T("XHGridViewBandCtrl"),WS_CHILD|WS_CLIPCHILDREN,CRect(0,0,10,10),this));

	if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		m_BandCtrl.ShowWindow(SW_SHOW);
	
	return CWnd::OnCreate(lpCreateStruct);
}

int CXHGridViewHeaderCtrl::GetHeight()
{
	return m_HeaderItem.GetHeight() + m_BandCtrl.GetHeight();
}

int CXHGridViewHeaderCtrl::GetWidth()
{
	if(m_HeaderItem.IsBand())
		return m_BandCtrl.GetWidth();
	else
	{
		CRect rcWindow;
		GetWindowRect(rcWindow);
		return rcWindow.Width();
	}
}

int CXHGridViewHeaderCtrl::GetBandHeight()	//if not band,return 0
{
	if(m_HeaderItem.IsBand())
		return m_BandCtrl.GetHeight();
	else
		return 0;
}

//////////////////////////////////////////////////////////////////////
// CXHGridView

//static members
CXHGridViewLeafDrawHelperBand CXHGridView::s_BandDrawer;
CXHGridViewLeafDrawHelperHeader CXHGridView::s_HeaderDrawer;
CXHGridViewLeafDrawHelperItem CXHGridView::s_ItemDrawer;

BEGIN_MESSAGE_MAP(CXHGridView,CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_XHGRIDVIEW_SPECIAL_MESSAGE,OnSpecialMessage)
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_MESSAGE(GVM_INSERTNEWITEM, OnInsertNewItem)
	ON_MESSAGE(GVM_REDRAWITEMS, OnRedrawItems)
	ON_MESSAGE(GVM_DELETEITEM, OnDeleteItem)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

CXHGridView::CXHGridView()
{	
	m_GridView.dwFlags = XHGRIDVIEW_FLAG_GRIDLINE;
	m_GridView.iViewType = XHGRIDVIEW_DETAIL;
	m_GridView.iLineWidth = XHGRIDVIEW_LINE_WIDTH;
	m_GridView.iRowHeight = XHGRIDVIEW_ROW_HEIGHT;
	m_GridView.iLeafBorderWidth = XHGRIDVIEW_LEAF_BORDER_WIDTH;
	m_GridView.iMarkWidth = XHGRIDVIEW_MARK_WIDTH;
	m_GridView.iItemSpacing = XHGRIDVIEW_ITEMSPACING;
	m_GridView.clrBackground = ::GetSysColor(COLOR_WINDOW); 
	m_GridView.clrLineColor = ::GetSysColor(COLOR_BTNFACE);
	m_GridView.iMinLeafWidth = XHGRIDVIEW_MIN_LEAFWIDTH;
	m_GridView.pFocusedItem = NULL;
	m_GridView.dwGridViewGlobalData = 0;
	m_GridView.dwReportNumber = 0;
	m_GridView.iFocusedItemIndex = -1;
	m_GridView.ptDownPoint.x = m_GridView.ptDownPoint.y = -1;
	m_GridView.dwStatus = 0;
	m_GridView.iStartDragItemIndex = -1;
	m_GridView.iEndDragItemIndex = -1;
	m_GridView.iSelectSession = 0;	//Base Session;
	m_GridView.iFirstSelectedItemIndex = -1;
	m_GridView.iLastSelectedItemIndex = -1;
	m_GridView.iSelectedCount = 0;
	m_GridView.pMouseOverLeaf = NULL;
	m_GridView.iOldMouseOverItem = -1;

	m_pImageList = NULL;
}

CXHGridView::~CXHGridView()
{
	DeleteAllItems();
}

BOOL CXHGridView::Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, BOOL bBorder, UINT nID)
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS,::LoadCursor(NULL,IDC_ARROW));
	CWnd *pThisWnd = this;
	return pThisWnd->Create(lpszClassName,lpszWindowName,WS_CHILD|WS_CLIPCHILDREN|WS_TABSTOP|(bBorder?WS_BORDER:0)|WS_VISIBLE,rect,pParentWnd,nID);
}

int CXHGridView::GetLineWidth() const
{
	return m_GridView.iLineWidth;
}

void CXHGridView::DeleteAllItems()
{
	for(int i=0;i<m_arrpItems.GetSize();i++)
		delete m_arrpItems[i];
	m_arrpItems.RemoveAll();
}

int CXHGridView::GetRowHeight() const
{
	return m_GridView.iRowHeight;
}

void CXHGridView::SetRowHeight(int iNewHeight)
{
	m_GridView.iRowHeight = iNewHeight;
	ResetVScrollBar();
}

int CXHGridView::GetViewType() const
{
	return m_GridView.iViewType;
}

BOOL CXHGridView::SetViewType(int iViewType)
{
	m_GridView.iViewType = iViewType;
	if(m_GridView.iViewType == XHGRIDVIEW_REPORT)
		m_GridView.dwReportNumber++;
	m_HeaderCtrl.Band();
	m_HeaderCtrl.ResetWindowSize();
	ResetHScrollBar();
	RedrawWindow();
	return TRUE;
}

int CXHGridView::GetLeafBorderWidth() const
{
	return m_GridView.iLeafBorderWidth;
}

void CXHGridView::SetImageList(CImageList *pImageList)
{
	m_pImageList = pImageList;
}

CImageList* CXHGridView::GetImageList()
{
	return m_pImageList;
}

int CXHGridView::GetMinLeafWidth() const
{
	return m_GridView.iMinLeafWidth;
}

void CXHGridView::SetGridViewGlobalData(DWORD dwNewData)
{
	m_GridView.dwGridViewGlobalData = dwNewData;
}

// AFX_INLINE DWORD CXHGridView::GetGridViewGlobalData() const
// {
// 	return m_GridView.dwGridViewGlobalData;
// }

CXHGridViewHeaderCtrl* CXHGridView::GetHeaderCtrl()
{
	return &m_HeaderCtrl;
}

CXHGridViewItemStructure* CXHGridView::GetHeaderItemStructure()
{
	return m_HeaderCtrl.GetHeaderItemStructure();
}

CXHGridViewItemStructure* CXHGridView::GetBandItemStructure()
{
	return m_HeaderCtrl.GetBandItemStructure();
}

int CXHGridView::GetMarkWidth() const
{
	return m_GridView.iMarkWidth;
}

BOOL CXHGridView::SetItemSelected(int iIndex ,BOOL bSelected)
{
	if(bSelected)
	{
		if(MarkItemSelected(iIndex))
		{
			RedrawItems(iIndex);
			return TRUE;
		}
	}
	else
	{
		if(MarkItemUnselected(iIndex))
		{
			RedrawItems(iIndex);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CXHGridView::IsItemSelected(int iIndex)
{
	if(iIndex >= 0 && iIndex < GetItemCount())
		return m_arrpItems[iIndex]->IsSelected();
	else
		ASSERT(FALSE);
	
	return FALSE;
}

CXHGridViewItemStructure* CXHGridView::Internal_InsertNewItem(int iIndex, CXHGridViewItemStructure *pNewItem)
{
	if(pNewItem == NULL)
		pNewItem = GetHeaderItemStructure()->CloneStructure();

	ASSERT(pNewItem);
	pNewItem->SetParentGridView(this);
	pNewItem->SetItemType(XHGRIDVIEW_ITEMTYPE_ITEM);

	int iStart = -1, iEnd = -1;
	GetVisibleItems(&iStart, &iEnd);

	m_arrpItems.InsertAt(iIndex,pNewItem);

	ResetVScrollBar();

	if(iIndex >= iStart && iIndex <= iEnd + 1)
		RedrawItems(iIndex, iEnd + 1 - iIndex + 1);

	return pNewItem;
}

int CXHGridView::Internal_DeleteItem(int iIndex)
{
	if(iIndex < 0 || iIndex > m_arrpItems.GetCount() - 1)
		return -1;

	m_arrpItems.RemoveAt(iIndex);

	int iStart = -1, iEnd = -1;
	GetVisibleItems(&iStart, &iEnd);

	ResetVScrollBar();

	if(iIndex >= iStart && iIndex <= iEnd + 1)
		RedrawItems(iIndex, iEnd + 1 - iIndex + 1);

	return 1;
}

CXHGridViewItemStructure* CXHGridView::InsertNewItem(int iIndex)
{
	return (CXHGridViewItemStructure*)SendMessage(GVM_INSERTNEWITEM, (WPARAM)iIndex, NULL);
}

CXHGridViewItemStructure* CXHGridView::InsertNewItem(int iIndex, CXHGridViewItemStructure *pNewItem)
{
	return (CXHGridViewItemStructure*)SendMessage(GVM_INSERTNEWITEM, (WPARAM)iIndex, (LPARAM)pNewItem);
}

void CXHGridView::RedrawVisibleItems(CDC *pDC)
{
	int iStart = -1,iEnd = -1;
	GetVisibleItems(&iStart,&iEnd);
	
	if(iStart >= 0)
	{
		for(int i=iStart;i<=iEnd;i++)
		{
			RedrawItems(i);
		}
	}
}

void CXHGridView::RedrawVisibleItemsForSelected(int iExclude)
{
	int iStart = -1,iEnd = -1;
	GetVisibleItems(&iStart,&iEnd);
	
	if(iStart >= 0)
	{
		for(int i=iStart;i<=iEnd;i++)
		{
			if(i != iExclude && m_arrpItems[i]->GetStatus() & XHGRIDVIEW_ITEM_STATE_SELECTED)
			{
				RedrawItems(i);
				m_arrpItems[i]->RefreshSelected(m_GridView.iSelectSession);
			}
		}
	}
}

int CXHGridView::GetVisibleItems(int *piStart,int *piEnd)
{
	ASSERT(piStart);
	ASSERT(piEnd);

	int iValidHeight = GetItemViewHeight();
	int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
	int iItemCount = m_arrpItems.GetSize();

	//TRACE2("---\n*CXHGridView::GetVisibleItems  iValidHeight=%d H=%d\n",iValidHeight,iItemHeight*iItemCount);
	CRect rc1,rc2;
	GetClientRect(rc1);
	GetWindowRect(rc2);
	//TRACE2("Client Height = %d , Window Height = %d\n",rc1.Height(),rc2.Height());
	//TRACE2("Header Height = %d , Band Height = %d\n---\n",m_HeaderCtrl.GetHeight(),m_HeaderCtrl.GetBandHeight());
	if(iValidHeight >= iItemHeight*iItemCount - GetItemSpacing())	//All items are visible , no vscroll of course
	{
		ASSERT((GetStyle() & WS_VSCROLL) == 0);
		*piStart = 0;
		*piEnd = iItemCount - 1;
		return iItemCount;
	}

	
	//ASSERT((GetStyle() & WS_VSCROLL) == WS_VSCROLL);	//has VScrollBar
	SCROLLINFO siv;
	siv.cbSize = sizeof(siv);
	GetScrollInfo(SB_VERT,&siv);
	int iVScrollPos = GetScrollPos(SB_VERT);
	*piStart = iVScrollPos;
	*piEnd = min((int)(iVScrollPos + siv.nPage - 1) , iItemCount - 1);

	return (*piEnd) - (*piStart) + 1;

}

int CXHGridView::GetItemViewHeight()
{
	CRect rcClient;
	GetClientRect(rcClient);

	return rcClient.Height() - GetHeaderCtrl()->GetHeight();
}

void CXHGridView::GetItemViewRect(LPRECT lprcRect)
{
	CXHClientRect rcClient(this);
	SetRect(lprcRect,0,m_HeaderCtrl.GetHeight(),rcClient.Width(),rcClient.Height());
}

void CXHGridView::ResetVScrollBar()
{
	int iCurPos = 0;
	BOOL bVScrollOld;
	if((GetStyle() & WS_VSCROLL) == WS_VSCROLL)
	{
		bVScrollOld = TRUE;
		iCurPos = GetScrollPos(SB_VERT);
	}
	else
		bVScrollOld = FALSE;

	int iViewHeight = GetItemViewHeight();
//	iViewHeight -= (GetItemSpacing());
	int iCount = m_arrpItems.GetSize();
	int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();

	// use iPatch here to reserve enough space at the bottom
	//int iPatch = ((iViewHeight % iItemHeight) > 0 ? 1 : 0);
	//int iPatch = 0;

	//TRACE("*------\nResetVScrollBar(),%d/%d\n*------\n",iViewHeight,iItemHeight * iCount);
	if(iItemHeight * iCount - GetItemSpacing() > iViewHeight)	//Need V-ScrollBar
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		si.nMin = 0;
		si.nMax = iCount;
		si.nPos = min(iCurPos,si.nMax);
		si.nPage = (iViewHeight / iItemHeight);
		if(iViewHeight % iItemHeight)
			si.nPage++;
		SetScrollInfo(SB_VERT,&si);
		EnableScrollBarCtrl(SB_VERT, TRUE);
		ModifyStyle(0,WS_VSCROLL);
//		ShowScrollBar(SB_VERT);
	}
	else
	{
		SetScrollPos(SB_VERT,0);
//		ShowScrollBar(SB_VERT,FALSE);
		EnableScrollBarCtrl(SB_VERT, FALSE);
		ModifyStyle(WS_VSCROLL,0);
	}
	BOOL bVScrollNew = ((GetStyle() & WS_VSCROLL) == WS_VSCROLL);

	if(bVScrollNew != bVScrollOld)
	{
		ResetHScrollBar();
	}
}

void CXHGridView::ResetHScrollBar()
{
	int iCurPos = 0;
	BOOL bHScrollOld;
	if((GetStyle() & WS_HSCROLL) == WS_HSCROLL)
	{
		iCurPos = GetScrollPos(SB_HORZ);
		bHScrollOld = TRUE;
	}
	else
		bHScrollOld = FALSE;

	int iHeaderWidth = m_HeaderCtrl.GetWidth();
	CRect rcGridView;
	GetClientRect(rcGridView);

	int iOldPos = GetScrollPos(SB_HORZ);

	if(iHeaderWidth > rcGridView.Width())	//Need H-ScrollBar
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE|SIF_POS|SIF_RANGE;
		si.nPage = rcGridView.Width();
		si.nMin = 0;
		si.nMax = iHeaderWidth;
		si.nPos = min(iCurPos,si.nMax);

		SetScrollPos(SB_HORZ,si.nPos);
		SetScrollInfo(SB_HORZ,&si);
		EnableScrollBarCtrl(SB_HORZ, TRUE);
		ModifyStyle(0,WS_HSCROLL);
//		ShowScrollBar(SB_HORZ);
	}
	else
	{
		int iCurPos = GetScrollPos(SB_HORZ);
		ScrollWindow(iCurPos,0);
		SetScrollPos(SB_HORZ,0);
		EnableScrollBarCtrl(SB_HORZ, FALSE);
//		ShowScrollBar(SB_HORZ,FALSE);
		ModifyStyle(WS_HSCROLL,0);
	}

	GetParent()->Invalidate(FALSE);

	BOOL bHScrollNew = ((GetStyle() & WS_HSCROLL) == WS_HSCROLL);

	ResetVScrollBar();
}

BOOL CXHGridView::MarkItemSelected(int iIndex)
{
	BOOL bMarked = m_arrpItems[iIndex]->MarkSelected(m_GridView.iSelectSession);
	if(bMarked)
	{
		if(m_GridView.iFirstSelectedItemIndex == -1 || m_GridView.iFirstSelectedItemIndex > iIndex)
			m_GridView.iFirstSelectedItemIndex = iIndex;
		if(m_GridView.iLastSelectedItemIndex == -1 || m_GridView.iLastSelectedItemIndex < iIndex)
			m_GridView.iLastSelectedItemIndex = iIndex;
		m_GridView.iSelectedCount++;
	}
	//TRACE1(_T("Select Count = %d  (MarkItemSelected)\r\n"),m_GridView.iSelectedCount);
	return bMarked;
}

BOOL CXHGridView::MarkItemUnselected(int iIndex)
{
	BOOL bUnMarked = m_arrpItems[iIndex]->MarkUnselected(m_GridView.iSelectSession);
	if(bUnMarked)
	{
		m_GridView.iSelectedCount--;
	}
	//TRACE1(_T("Select Count = %d  (MarkItemUnselected)\r\n"),m_GridView.iSelectedCount);
	return bUnMarked;
}

BOOL CXHGridView::ChangeItemSelected(int iIndex)
{
	BOOL bChanged = m_arrpItems[iIndex]->ChangeSelected(m_GridView.iSelectSession);
	if(bChanged)
	{
		if(m_arrpItems[iIndex]->IsSelected())
		{
			if(m_GridView.iFirstSelectedItemIndex == -1 || m_GridView.iFirstSelectedItemIndex > iIndex)
				m_GridView.iFirstSelectedItemIndex = iIndex;
			if(m_GridView.iLastSelectedItemIndex == -1 || m_GridView.iLastSelectedItemIndex < iIndex)
				m_GridView.iLastSelectedItemIndex = iIndex;
			m_GridView.iSelectedCount++;
		}
		else
		{
			m_GridView.iSelectedCount--;
		}
	}
	//TRACE1(_T("Select Count = %d  (ChangeItemSelected)\r\n"),m_GridView.iSelectedCount);
	return bChanged;
}

CXHGridViewItemStructure* CXHGridView::GetItem(int iItemIndex)
{
	return m_arrpItems[iItemIndex];
}

void CXHGridView::RedrawUnusedArea()
{	
	CClientDC dc(this);
	CRect rcHeaderRight;
	GetHeaderRightArea(rcHeaderRight);
	CBrush brush;
	brush.CreateSolidBrush(m_GridView.clrBackground);

	dc.FillRect(rcHeaderRight,&brush);

	CRect rcItemUR,rcItemUB;
	WORD wResult = GetItemUnusedArea(rcItemUR,rcItemUB);
	if(LOBYTE(wResult) == 0)
		dc.FillRect(rcItemUR,&brush);
	if(HIBYTE(wResult) == 0)
		dc.FillRect(rcItemUB,&brush);

	//Then fill the space between items
	CXHClientRect rcClient(this);
	int iViewHeight = GetItemViewHeight();
	int iHeaderHeight = m_HeaderCtrl.GetHeight();
	int iItemFactHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight();
	int iViewCount = iViewHeight / ( iItemFactHeight + GetItemSpacing());
	int iYOffset = iHeaderHeight;
	for(int i=0;i<iViewCount;i++)
	{
		iHeaderHeight += iItemFactHeight;
		CRect rcSpacing(0,iHeaderHeight,rcClient.Width() + i * GetItemSpacing(),iHeaderHeight + GetItemSpacing());
		dc.FillRect(rcSpacing,&brush);
		iHeaderHeight += GetItemSpacing();
	}
}

void CXHGridView::GetHeaderRightArea(LPRECT rcArea)
{
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcHeader;
	m_HeaderCtrl.GetClientRect(rcHeader);
	SetRect(rcArea,rcHeader.right,rcClient.top,rcClient.right,rcHeader.bottom);
}

void CXHGridView::ProcessMouseDown(CPoint point,int iMouseButton)
{
	int iNewHit = ItemHitTest(point);

	//去除前面选中项的 Focus 属性
	if(m_GridView.pFocusedItem && iNewHit >= 0 && m_arrpItems[iNewHit] != m_GridView.pFocusedItem)
	{
		m_GridView.pFocusedItem->ModifyState(XHGRIDVIEW_ITEM_STATE_FOCUSED,0);
		int iRedrawIndex = GetItemIndexSearchVisible(m_GridView.pFocusedItem);
		if(iRedrawIndex >= 0)
			RedrawItems(iRedrawIndex);
	}

	if(iMouseButton == 2)	//Right Button
	{
		if(iNewHit == -1 || (!m_arrpItems[iNewHit]->IsSelected() && !IsControlKeyPressed()))
		{
			IncreaseSelectSession();
			RedrawVisibleItemsForSelected();
		}
	}

	//设置新的 Focus 项
	if(iNewHit >= 0)
	{
		if(IsControlKeyPressed())
		{
			if(ChangeItemSelected(iNewHit))
				RedrawItems(iNewHit);
		}
		else
		{
			if(MarkItemSelected(iNewHit))
				RedrawItems(iNewHit);
		}
		
		m_GridView.pFocusedItem = m_arrpItems[iNewHit];
		//根据 LeafItem 来改变 Header 中对应 Leaf 的外观
		CXHGridViewLeafItem *pLeaf = m_GridView.pFocusedItem->LeafHitTest(CPoint(
			point.x + GetScrollPos(SB_HORZ)
			,point.y - m_HeaderCtrl.GetHeight() - (iNewHit - GetScrollPos(SB_VERT)) * (m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing())));

		SetHeaderFocusLeaf(pLeaf);
		
		m_GridView.pFocusedItem->ModifyState(0,XHGRIDVIEW_ITEM_STATE_FOCUSED);
		RedrawItems(iNewHit);

		if(iMouseButton == 1)	//Left button
			OnItemClick(iNewHit, pLeaf);
		else if(iMouseButton == 2)
			OnItemRClick(iNewHit, pLeaf);

		m_GridView.iFocusedItemIndex = iNewHit;
	}
	else
	{
		m_GridView.iShiftStart = -1;
		IncreaseSelectSession();
		if(m_HeaderCtrl.GetHeaderItemStructure()->GetLastFocusedLeafItem())
		{
			m_HeaderCtrl.GetHeaderItemStructure()->GetLastFocusedLeafItem()->SetFocused(FALSE);
			m_HeaderCtrl.GetHeaderItemStructure()->SetLastFocusedLeafItem(NULL);
			m_HeaderCtrl.RedrawWindow();
		}
	}
}

WORD CXHGridView::GetItemUnusedArea(LPRECT rcAreaR,LPRECT rcAreaB)
{
	WORD wResult = 0;
	CRect rcClient;
	GetClientRect(rcClient);
	CRect rcHeader;
	m_HeaderCtrl.GetClientRect(rcHeader);
	int iScrollPos = 0;
	if((GetStyle() & WS_VSCROLL) == WS_VSCROLL)
		iScrollPos = GetScrollPos(SB_VERT);
	SetRect(rcAreaR,rcHeader.right,rcClient.top,rcClient.right,rcClient.bottom);
	if(rcAreaR->right < rcAreaR->left)
		wResult |= 0x0001;

	int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
	int iStart = -1 ,iEnd = -1;
	GetVisibleItems(&iStart,&iEnd);
	SetRect(rcAreaB,rcClient.left,rcHeader.Height() + iItemHeight * (iEnd - iStart + 1),rcClient.right,rcClient.bottom);
	if(rcAreaB->bottom < rcAreaB->top)
		wResult |= 0x0100;

	return wResult;
}

int CXHGridView::ItemHitTest(POINT point,BOOL bOutOfScale)
{
	CRect rcHeader;
	m_HeaderCtrl.GetClientRect(rcHeader);
	if(point.x + GetScrollPos(SB_HORZ) > rcHeader.Width())
		return -1;
	point.y -= GetHeaderCtrl()->GetHeight();
	int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();

	int iPatch = 0;
	if(point.y < 0)
	{
		int yDelta = abs(point.y);
		iPatch = yDelta / iItemHeight;
		if( yDelta % iItemHeight )
			iPatch++;

		point.y = iItemHeight - (yDelta - (iPatch - 1) * iItemHeight);
		iPatch = -iPatch;
	}

	int iIndex = point.y / iItemHeight;
	
	if(bOutOfScale || point.y - (iIndex) * iItemHeight > 0)
	{
		int iResult = -1;
		if(bOutOfScale || iIndex + GetScrollPos(SB_VERT) < m_arrpItems.GetSize())
		{
			iResult = iIndex + GetScrollPos(SB_VERT) + iPatch;
			if(bOutOfScale && iResult < 0)
				return 0;
			if(iResult < m_arrpItems.GetSize())
				return iResult;
			else
				return -1;
		}
		
		if(bOutOfScale)		//Allow out-of-scale hit
		{
			if(iResult < 0)
				iResult = 0;
			else if(iResult > m_arrpItems.GetSize() - 1)
				iResult = m_arrpItems.GetSize() - 1;
			return iResult;
		}
		else
		{
			if(iResult >= 0)
				return iResult;
			else
				return -1;
		}
	}

	if(point.y % iItemHeight >= m_HeaderCtrl.GetHeaderItemStructure()->GetHeight())	//Hit the space between items
		return -1;
	
	return -1;
}

void CXHGridView::RedrawItems(int iItemIndexStart, int iCount)
{
	SendMessage(GVM_REDRAWITEMS, iItemIndexStart, iCount);
}

void CXHGridView::Internal_RedrawItems(int iItemIndexStart, int iCount)
{
	for(int i=iItemIndexStart;i<iItemIndexStart + iCount;i++)
	{
		if(IsItemVisible(i))
		{
			//Get client DC to draw to
			CClientDC clientDC(this);
			
			//Set the Correct Org
			SCROLLINFO sih,siv;
			sih.cbSize = sizeof(sih);
			siv.cbSize = sizeof(siv);
			GetScrollInfo(SB_HORZ,&sih);
			GetScrollInfo(SB_VERT,&siv);
			sih.nPos = GetScrollPos(SB_HORZ);
			siv.nPos = GetScrollPos(SB_VERT);

			int iHH = GetHeaderCtrl()->GetHeight();
			
			clientDC.SetViewportOrg(0,GetHeaderCtrl()->GetHeight());		
			clientDC.SetViewportOrg(-sih.nPos,GetHeaderCtrl()->GetHeight());
			
			//Prepare the colors
			CXHColor clrBackcolor = ::GetSysColor(COLOR_WINDOW);
			if(m_arrpItems[i]->IsSelected())
			{
				clrBackcolor = ::GetSysColor(COLOR_HIGHLIGHT);
				clrBackcolor.MixedWith(RGB(255,255,255));
			}
			CBrush backBrush(clrBackcolor);
			
			int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
			CXHClientRect rcClient(this);
/*		
//原始代码，没有进行优化。	
			CXHRect rcItem;
			m_HeaderCtrl.GetHeaderItemStructure()->GetItemRect(rcItem);
			CXHMemDC mdc(&clientDC,rcItem);
			mdc.FillRect(rcItem,&backBrush);		//Background
			m_arrpItems[i]->Draw(&mdc,0,0);			//Content

			if(m_arrpItems[i]->IsFocused())
				mdc.DrawFocusRect(rcItem);

			mdc.DrawTo(&clientDC,0,(i - GetScrollPos(SB_VERT)) * iItemHeight);
*/

			CXHRect rcItem,rcItemFrame;
			//获得 ItemStructure 的区域
			m_HeaderCtrl.GetHeaderItemStructure()->GetItemRect(rcItem);
			//保存一个副本，在画 Focus Frame 时使用
			rcItemFrame = rcItem;
			//截取 ItemStructure 区域，使之仅仅保留客户区宽度，目的是优化绘图效率
			rcItem.right = rcItem.left + min(rcItem.Width(),rcClient.Width());

			CXHMemDC mdc(&clientDC,rcItem);
			mdc.FillRect(rcItem,&backBrush);		//Background
			m_arrpItems[i]->Draw(&mdc,-GetScrollPos(SB_HORZ),0);			//Content

			if(IsFocused() && m_arrpItems[i]->IsFocused())
			{
				rcItemFrame.OffsetRect(-GetScrollPos(SB_HORZ),0);
				mdc.DrawFocusRect(rcItemFrame);
			}

			mdc.DrawTo(&clientDC,GetScrollPos(SB_HORZ),(i - GetScrollPos(SB_VERT)) * iItemHeight);
		}
	}
}

BOOL CXHGridView::IsItemVisible(int iItemIndex,BOOL bPartial)
{
	if(iItemIndex < GetScrollPos(SB_VERT))
		return FALSE;
	int iViewHeight = GetItemViewHeight();
	int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
	int iViewCount = iViewHeight / iItemHeight;
	int iScrollMin,iScrollMax;
	GetScrollRange(SB_VERT,&iScrollMin,&iScrollMax);
	if((iViewHeight % iItemHeight) > 0 && GetScrollPos(SB_VERT) != iScrollMax)
		iViewCount++;
	if(iItemIndex >= GetScrollPos(SB_VERT) + iViewCount)
		return FALSE;

	return TRUE;
}

void CXHGridView::Sort(PFNGRIDVIEWCOMPARE pSortFunc,DWORD dwUserData)
{
	qsort_s(m_arrpItems.GetData(), m_arrpItems.GetSize(), sizeof(CXHGridViewItemStructure*), (int(__cdecl*)(void*, const void*,const void*))pSortFunc, (void*)dwUserData);
	int iLow,iHigh;
	if(GetVisibleItems(&iLow,&iHigh) > 0)
		RedrawItems(iLow,iHigh - iLow + 1);

	m_GridView.iFocusedItemIndex = -1;
	if(m_GridView.iFirstSelectedItemIndex >= 0)
		m_GridView.iFirstSelectedItemIndex = 0;
	if(m_GridView.iLastSelectedItemIndex >= 0)
		m_GridView.iLastSelectedItemIndex = m_arrpItems.GetSize() - 1;

}

/* Message Handler for  */

int CXHGridView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	VERIFY(m_HeaderCtrl.Create(_T("XHGridViewHeaderCtrl"),WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE,CRect(0,0,10,10),this));
	return CWnd::OnCreate(lpCreateStruct);
}

void CXHGridView::OnPaint()
{
//	TRACE1("CXHGridView::OnPaint()  %d\n",GetTickCount());
	
	CPaintDC dc(this);	

	GetHeaderCtrl()->RedrawWindow();
	
	if(m_arrpItems.GetSize() > 0)
		RedrawUnusedArea();

	//Set the Correct Org
	SCROLLINFO sih,siv;
	sih.cbSize = sizeof(sih);
	siv.cbSize = sizeof(siv);
	GetScrollInfo(SB_HORZ,&sih);
	GetScrollInfo(SB_VERT,&siv);
	sih.nPos = GetScrollPos(SB_HORZ);
	siv.nPos = GetScrollPos(SB_VERT);
	dc.SetViewportOrg(0,GetHeaderCtrl()->GetHeight());

	CRect rcClient;
	GetClientRect(rcClient);

	dc.SetViewportOrg(-sih.nPos,GetHeaderCtrl()->GetHeight());

	//Calculate the invalid area

	//Draw Items needed to be drawn
	RedrawVisibleItems(&dc);
}

void CXHGridView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetFocus();
	
	if((GetStyle() & WS_VSCROLL) == 0)
		return;

	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	GetScrollRange(SB_VERT, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_VERT);
	
	// Get the current position of scroll box.
	int curpos = GetScrollPos(SB_VERT);
	int iOldPos = curpos;
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;
		
	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINELEFT:      // Scroll left.
		if (curpos > minpos)
			curpos--;
		break;
		
	case SB_LINERIGHT:   // Scroll right.
		if (curpos < maxpos)
			curpos++;
		break;
		
	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);
			
			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_VERT, &info, SIF_ALL);
			
			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	

	int iScrolled = iOldPos - curpos;

	if(m_GridView.dwFlags & XHGRIDVIEW_STATUS_LBUTTONDOWN
		|| m_GridView.dwFlags & XHGRIDVIEW_STATUS_RBUTTONDOWN)
	{
		m_GridView.ptDownPoint.y += (iScrolled * (GetHeaderItemStructure()->GetHeight() + GetItemSpacing()));
	}

	if(iScrolled != 0)
	{
		CXHClientRect rcClient(this);

		int iItemHeight = m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
		m_HeaderCtrl.ShowWindow(SW_HIDE);
		m_HeaderCtrl.SetParent(NULL);
		ScrollWindow(0, iItemHeight * iScrolled, NULL, rcClient);
		m_HeaderCtrl.SetParent(this);
		m_HeaderCtrl.ShowWindow(SW_SHOW);
		
		//ValidateRect(rcClient);
		
		int iLow,iHigh;
		int nVisible = GetVisibleItems(&iLow,&iHigh);
		
		// Set the new position of the thumb (scroll box).
		SetScrollPos(SB_VERT, curpos);

		if(abs(iScrolled) < nVisible - 1)
		{
			if(iScrolled > 0)	//Scroll up
			{
				iHigh = iLow - 1;
				iLow -= (nVisible - iScrolled);
			}
			else if(iScrolled < 0)
			{
				iLow = iHigh - 1;
				iHigh += (nVisible + iScrolled);
			}

			iHigh = min(iHigh,m_arrpItems.GetSize() - 1);
			iLow = max(iLow,0);
			if(iHigh >= iLow)
				RedrawItems(iLow ,iHigh - iLow + 1);
		}
		else
		{
			GetVisibleItems(&iLow,&iHigh);
			iHigh = min(iHigh,m_arrpItems.GetSize() - 1);
			iLow = max(iLow,0);
			RedrawItems(iLow ,iHigh - iLow + 1);
		}
		
		if(curpos == maxpos)
			RedrawUnusedArea();
	} 
	
	SetScrollPos(SB_VERT, curpos);
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CXHGridView::GetItemSpacing() const
{
	if(GetViewType() == XHGRIDVIEW_DETAIL)
		return m_GridView.iItemSpacing;
	else if(GetViewType() == XHGRIDVIEW_REPORT)
		return 0;

	ASSERT(FALSE);	//Other mode
	return 0;
}

COLORREF CXHGridView::GetLineColor() const
{
	return m_GridView.clrLineColor;
}

void CXHGridView::SetLineColor(COLORREF clrLineColor)
{
	m_GridView.clrLineColor = clrLineColor;
}

COLORREF CXHGridView::GetBackColor() const
{
	return m_GridView.clrBackground;
}

void CXHGridView::SetBackColor(COLORREF clrBackColor)
{
	m_GridView.clrBackground = clrBackColor;
	RedrawUnusedArea();
}

void CXHGridView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SetFocus();

	if((GetStyle() & WS_HSCROLL) == 0)
		return;

	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	GetScrollRange(SB_HORZ, &minpos, &maxpos); 
	maxpos = GetScrollLimit(SB_HORZ);
	
	// Get the current position of scroll box.
	int curpos = GetScrollPos(SB_HORZ);
	int iOldPos = curpos;
	
	// Determine the new position of scroll box.
	switch (nSBCode)
	{
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;
		
	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;
		
	case SB_ENDSCROLL:   // End scroll.
		break;
		
	case SB_LINELEFT:      // Scroll left.
		if (curpos > minpos)
			curpos-= min(5,iOldPos);
		break;
		
	case SB_LINERIGHT:   // Scroll right.
		if (curpos < maxpos)
			curpos+= min(5,maxpos - iOldPos);
		break;
		
	case SB_PAGELEFT:    // Scroll one page left.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);
			
			if (curpos > minpos)
				curpos = max(minpos, curpos - (int) info.nPage);
		}
		break;
		
	case SB_PAGERIGHT:      // Scroll one page right.
		{
			// Get the page size. 
			SCROLLINFO   info;
			GetScrollInfo(SB_HORZ, &info, SIF_ALL);
			
			if (curpos < maxpos)
				curpos = min(maxpos, curpos + (int) info.nPage);
		}
		break;
		
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
		
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	
	// Set the new position of the thumb (scroll box).
	SetScrollPos(SB_HORZ, curpos);
	
	ScrollWindow(- (curpos - iOldPos) ,0);
	
	if(m_GridView.dwFlags & XHGRIDVIEW_STATUS_LBUTTONDOWN
		|| m_GridView.dwFlags & XHGRIDVIEW_STATUS_RBUTTONDOWN)
	m_GridView.ptDownPoint.x += (iOldPos - curpos);

	UpdateWindow();
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CXHGridView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int iNewHit = ItemHitTest(point);

	if(iNewHit >= 0)
	{
		CXHGridViewLeafItem *pLeaf = m_GridView.pFocusedItem->LeafHitTest(CPoint(
			point.x + GetScrollPos(SB_HORZ)
			,point.y - m_HeaderCtrl.GetHeight() - (iNewHit - GetScrollPos(SB_VERT)) * (m_HeaderCtrl.GetHeaderItemStructure()->GetHeight() + GetItemSpacing())));

		if(pLeaf)
		{

		}
		OnItemDblClick(iNewHit, pLeaf);
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CXHGridView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(GetItemCount() == 0)
		return;

	SetFocus();
	m_GridView.ptDownPoint = point;
	m_GridView.ptPreMovePoint = point;
	m_GridView.dwStatus |= XHGRIDVIEW_STATUS_LBUTTONDOWN;
	m_GridView.iStartDragItemIndex = -1;
	m_GridView.iEndDragItemIndex = -1;
	m_GridView.iFocusedItemIndex = GetFocusedItemIndex();	//Just refresh;
	if(IsShiftKeyPressed())
	{
		if(m_GridView.iShiftStart == -1)
		{
			m_GridView.iShiftStart = (m_GridView.iFocusedItemIndex == -1?0:m_GridView.iFocusedItemIndex);
		}
	}
	else
	{
		m_GridView.iShiftStart = -1;
		m_GridView.iPreShiftEnd = -1;
		if(!IsControlKeyPressed())
			IncreaseSelectSession();
	}
	SetCapture();
	ProcessMouseDown(point,1);

	if(m_GridView.iShiftStart != -1)
	{
		if(m_GridView.iPreShiftEnd != -1 && m_GridView.iFocusedItemIndex != -1)
		{
			if(m_GridView.iPreShiftEnd - m_GridView.iFocusedItemIndex != 0)
			{
				int iStep = (m_GridView.iFocusedItemIndex - m_GridView.iPreShiftEnd) / abs(m_GridView.iPreShiftEnd - m_GridView.iFocusedItemIndex);
				int i=m_GridView.iPreShiftEnd;
				do
				{
//					TRACE1("i = %d ",i);
					if(m_arrpItems[i]->MarkUnselected(m_GridView.iSelectSession))
						RedrawItems(i);
					i+=iStep;
				}while(i != m_GridView.iFocusedItemIndex);
			}
		}

		if(m_GridView.iFocusedItemIndex != -1 && abs(m_GridView.iFocusedItemIndex - m_GridView.iShiftStart) > 0)
		{
			int iStep = (m_GridView.iFocusedItemIndex - m_GridView.iShiftStart) / abs(m_GridView.iFocusedItemIndex - m_GridView.iShiftStart);
			int i=m_GridView.iShiftStart;
			do
			{
				if(m_arrpItems[i]->MarkSelected(m_GridView.iSelectSession))
					RedrawItems(i);
				i+=iStep;
			}while(i != m_GridView.iFocusedItemIndex);
		}

		m_GridView.iPreShiftEnd = m_GridView.iFocusedItemIndex;
	}
	else
	{
		RedrawVisibleItemsForSelected();
	}

	SetFocus();
}

void CXHGridView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_GridView.dwStatus & XHGRIDVIEW_STATUS_DRAGGING)
		DrawDragRect(m_GridView.ptDownPoint,point);
	m_GridView.dwStatus &= (~(XHGRIDVIEW_STATUS_LBUTTONDOWN|XHGRIDVIEW_STATUS_DRAGGING));
	m_GridView.ptDownPoint.x = 	m_GridView.ptDownPoint.y = -1;
	m_GridView.iStartDragItemIndex = -1;
	m_GridView.iEndDragItemIndex = -1;
	ReleaseCapture();
}

void CXHGridView::OnMouseMove(UINT nFlags, CPoint point)
{
	CXHClientRect rcClient(this);
	if(m_GridView.dwStatus & XHGRIDVIEW_STATUS_LBUTTONDOWN)
	{
		if(abs(point.x - m_GridView.ptDownPoint.x) + abs(point.y - m_GridView.ptDownPoint.y) >= XHGRIDVIEW_DRAG_MIN
			|| m_GridView.dwStatus & XHGRIDVIEW_STATUS_DRAGGING)	//Begin Drag
		{
			if(m_GridView.dwStatus & XHGRIDVIEW_STATUS_DRAGGING)
			{
				DrawDragRect(m_GridView.ptDownPoint,m_GridView.ptPreMovePoint);
				ProcessDragRect(m_GridView.ptDownPoint,point,point - m_GridView.ptPreMovePoint);
				if(point.y < m_HeaderCtrl.GetHeight())
				{
					OnVScroll(SB_LINEUP,0,NULL);
				}
				else if(point.y > rcClient.Height())
				{
					OnVScroll(SB_LINEDOWN,0,NULL);
				}
				if(point.x < 0)
				{
					OnHScroll(SB_LINELEFT,0,NULL);
				}
				else if(point.x > rcClient.Width())
				{
					OnHScroll(SB_LINERIGHT,0,NULL);
				}
				UpdateWindow();
			}
			DrawDragRect(m_GridView.ptDownPoint,point);
			m_GridView.dwStatus |= XHGRIDVIEW_STATUS_DRAGGING;
		}
	}
	else
	{
		ProcessLeafHot(point);
	}
	m_GridView.ptPreMovePoint = point;
}

void CXHGridView::ProcessLeafHot(CPoint point)
{
	//处理 LeafItem 的 Enter 和 Leave 消息
	if(::GetCapture() != m_hWnd)
		SetCapture();
	POINT ptMouse = point;
	ClientToScreen(&ptMouse);
	if(::WindowFromPoint(ptMouse) != GetSafeHwnd())
	{
		if(m_GridView.pMouseOverLeaf)
		{
			if(m_GridView.pMouseOverLeaf->MouseLeave())
				if(m_GridView.iOldMouseOverItem != -1)
					RedrawItems(m_GridView.iOldMouseOverItem);
			m_GridView.pMouseOverLeaf = NULL;
			m_GridView.iOldMouseOverItem = -1;
		}
		ReleaseCapture();
		return;
	}

	CXHClientRect rcClient(this);
	rcClient.top += m_HeaderCtrl.GetHeight();
	if(rcClient.PtInRect(point))
	{
		int iItemHit = ItemHitTest(point);
		CXHGridViewItemStructure *pItem = NULL;
		if(iItemHit != -1)
			pItem = m_arrpItems[iItemHit];
		
		if(pItem)
		{
			CXHGridViewLeafItem *pOldOverLeaf = m_GridView.pMouseOverLeaf;
			CPoint ptInner = point;
			ASSERT(iItemHit != -1);
			ClientToItem(iItemHit,&ptInner);
			m_GridView.pMouseOverLeaf = pItem->LeafHitTest(ptInner);

			if(m_GridView.pMouseOverLeaf)
			{
				if(pOldOverLeaf != m_GridView.pMouseOverLeaf)
				{
					m_GridView.pMouseOverLeaf->AddRef();
					if(pOldOverLeaf)
					{
						if(pOldOverLeaf->MouseLeave())
						{
							if(iItemHit != m_GridView.iOldMouseOverItem && m_GridView.iOldMouseOverItem != -1)
								RedrawItems(m_GridView.iOldMouseOverItem);
						}
						pOldOverLeaf->Release();
					}
					if(m_GridView.pMouseOverLeaf->MouseEnter())
					{
						RedrawItems(iItemHit);
					}

				}
			}
			else
			{
				if(pOldOverLeaf)
				{
					if(pOldOverLeaf->MouseLeave())
					{
						if(m_GridView.iOldMouseOverItem != -1)
							RedrawItems(m_GridView.iOldMouseOverItem);
					}
					pOldOverLeaf->Release();
				}
			}
			m_GridView.iOldMouseOverItem = iItemHit;
		}
		else
		{
			if(m_GridView.pMouseOverLeaf)
			{
				if(m_GridView.pMouseOverLeaf->MouseLeave())
					if(m_GridView.iOldMouseOverItem != -1)
						RedrawItems(m_GridView.iOldMouseOverItem);
			}
			m_GridView.pMouseOverLeaf = NULL;
		}


	}
	else
	{
		if(m_GridView.pMouseOverLeaf)
		{
			if(m_GridView.pMouseOverLeaf->MouseLeave())
				if(m_GridView.iOldMouseOverItem != -1)
					RedrawItems(m_GridView.iOldMouseOverItem);
		}
		m_GridView.pMouseOverLeaf = NULL;
		m_GridView.iOldMouseOverItem = -1;
		ReleaseCapture();
	}
}

BOOL CXHGridView::ClientToItem(int iItemIndex,LPPOINT lpPoint,int nCount)
{
	if(iItemIndex < 0 || iItemIndex > m_arrpItems.GetSize() - 1)
		return FALSE;

	int iVScrollPos = GetScrollPos(SB_VERT);
	int iHScrollPos = GetScrollPos(SB_HORZ);
	int iItemHeight = GetHeaderItemStructure()->GetHeight() + GetItemSpacing();

	int iYPatch = iItemHeight * (iItemIndex - iVScrollPos) + m_HeaderCtrl.GetHeight();
	for(int i=0;i<nCount;i++)
	{
		lpPoint->y -= iYPatch;
		lpPoint->x += (iHScrollPos);
		lpPoint++;
	}

	return TRUE;
}

BOOL CXHGridView::ItemToClient(int iItemIndex,LPPOINT lpPoint,int nCount)
{
	if(iItemIndex < 0 || iItemIndex > m_arrpItems.GetSize() - 1)
		return FALSE;
	
	int iVScrollPos = GetScrollPos(SB_VERT);
	int iHScrollPos = GetScrollPos(SB_HORZ);
	int iItemHeight = GetHeaderItemStructure()->GetHeight() + GetItemSpacing();

	int iYPatch = iItemHeight * (iItemIndex - iVScrollPos) + m_HeaderCtrl.GetHeight();
	for(int i=0;i<nCount;i++)
	{
		lpPoint->y += iYPatch;
		lpPoint->x -= (iHScrollPos);
		lpPoint++;
	}

	return TRUE;
}

BOOL CXHGridView::GetLeafRect(int iItemIndex,int iBaseIndex,int iSubIndex,int iLeafIndex,LPRECT lprcArea)
{
	if(iItemIndex < 0 || iItemIndex > m_arrpItems.GetSize() - 1)
		return FALSE;
	CXHGridViewItemStructure *pItem = m_arrpItems[iItemIndex];
	ASSERT(pItem);
	CXHGridViewBaseItem *pBase = pItem->GetBaseItem(iBaseIndex);
	CXHGridViewSubItem *pSubItem = pBase->GetSubItem(iSubIndex);
	CXHGridViewLeafItem *pLeaf = pSubItem->GetLeafItem(iLeafIndex);

	lprcArea->left = pBase->GetParentBaseItems()->GetBaseItemWidth(0,iBaseIndex);
	lprcArea->left += pLeaf->GetParentLeafItems()->GetLeafItemWidth(0,iLeafIndex);
	lprcArea->left += GetMarkWidth();
	lprcArea->right = lprcArea->left + pLeaf->GetFactWidth();

	if(GetViewType() == XHGRIDVIEW_DETAIL)
	{
		lprcArea->top = pSubItem->GetParentSubItems()->GetSubItemHeight(0,iSubIndex);
		lprcArea->bottom = lprcArea->top + pSubItem->GetHeight();
	}
	else
	{
		lprcArea->top = 0;
		lprcArea->bottom = GetRowHeight();
	}

	ItemToClient(iItemIndex,(LPPOINT)lprcArea,2);
	return TRUE;
}

void CXHGridView::ProcessDragRect(CPoint point1,CPoint point2,CSize sizeDelta)
{
	CXHClientRect rcClient(this);

	int iHitStart = ItemHitTest(CPoint(min(point1.x,point2.x),point1.y),TRUE);
	int iHitEnd = ItemHitTest(CPoint(min(point1.x,point2.x),point2.y),TRUE);

	int iItemHeight = GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
	if(iHitStart == -1 && iHitEnd == -1)
	{
		if(m_GridView.iEndDragItemIndex != -1)
		{
			IncreaseSelectSession();
			RedrawVisibleItemsForSelected();
		}
		m_GridView.iEndDragItemIndex = iHitEnd;	//-1
		m_GridView.iStartDragItemIndex = iHitStart;	//-1
	}
	else if((iHitEnd != -1 || iHitStart != -1 || abs(point2.y - point1.y) > iItemHeight)
		&& (point1.x + GetScrollPos(SB_HORZ) < m_HeaderCtrl.GetWidth() || point2.x + GetScrollPos(SB_HORZ) < m_HeaderCtrl.GetWidth()))
	{
		if(iHitStart == -1)
		{
			if(point2.y > point1.y)
			{
				point1.y += ((iItemHeight) / 2);
				while((iHitStart = ItemHitTest(CPoint(min(point1.x,point2.x),point1.y),TRUE)) == -1)
					point1.y += ((iItemHeight) / 2);
			}
			else if(point2.y <= point1.y)
			{
				point1.y -= ((iItemHeight) / 2);
				while((iHitStart = ItemHitTest(CPoint(min(point1.x,point2.x),point1.y),TRUE)) == -1)
					point1.y -= ((iItemHeight) / 2);
			}
		}
		if(iHitEnd == -1)
		{
			int iItemHeight = GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
			if(point2.y > point1.y)
			{
				point2.y -= ((iItemHeight) / 2);
				while((iHitEnd = ItemHitTest(CPoint(min(point1.x,point2.x),point2.y),TRUE)) == -1)
					point2.y -= ((iItemHeight) / 2);
			}
			else if(point2.y <= point1.y)
			{
				point2.y += ((iItemHeight) / 2);
				while((iHitEnd = ItemHitTest(CPoint(min(point1.x,point2.x),point2.y),TRUE)) == -1)
					point2.y += ((iItemHeight) / 2);
			}
		}

		ASSERT(iHitStart != -1);
		ASSERT(iHitEnd != -1);

		if(iHitStart>iHitEnd)
		{
			int iTemp = iHitStart;
			iHitStart = iHitEnd;
			iHitEnd = iTemp;
		}
		ASSERT(iHitStart <= iHitEnd);
		for(int i=iHitStart;i!=iHitEnd;i++)
		{
			if(MarkItemSelected(i))
				RedrawItems(i);
		}
		if(m_arrpItems[iHitEnd])
			if(MarkItemSelected(iHitEnd))
				RedrawItems(iHitEnd);

		if(!IsControlKeyPressed())
		{
			int i = iHitStart - 1;
			while(i >= 0 && i < m_arrpItems.GetSize() && m_GridView.iStartDragItemIndex != -1 && i >= m_GridView.iStartDragItemIndex)
			{
				if(MarkItemUnselected(i))
					RedrawItems(i);
				i--;
			}
			i = iHitEnd + 1;
			while(i >= 0 && i < m_arrpItems.GetSize() && m_GridView.iEndDragItemIndex != -1 && i >= m_GridView.iEndDragItemIndex)
			{
				if(MarkItemUnselected(i))
					RedrawItems(i);
				i++;
			}
		}

		m_GridView.iEndDragItemIndex = iHitEnd;
		m_GridView.iStartDragItemIndex = iHitStart;
	}
}

BOOL CXHGridView::IsControlKeyPressed() const
{
	return ((GetKeyState(VK_CONTROL) & 0xF0) != 0);
}

BOOL CXHGridView::IsShiftKeyPressed() const
{
	return ((GetKeyState(VK_SHIFT) & 0xF0) != 0);
}

int CXHGridView::GetCurrentSelectSession() const
{
	return m_GridView.iSelectSession;
}


void CXHGridView::DrawDragRect(POINT point1, POINT point2)
{
	CClientDC dc(this);
	CPen pen,*pOldPen;
	pen.CreatePen(PS_DOT,1,RGB(0,0,0));
	pOldPen = dc.SelectObject(&pen);
	CBrush brush,*pOldBrush;
	brush.CreateStockObject(NULL_BRUSH);
	pOldBrush = dc.SelectObject(&brush);
	dc.SetROP2(R2_XORPEN);
	dc.Rectangle(CRect(point1,point2));
	dc.SelectObject(pOldBrush);
	dc.SelectObject(pOldPen);
}


void CXHGridView::OnRButtonDown(UINT nFlags, CPoint point)
{
	if((m_GridView.dwStatus & XHGRIDVIEW_STATUS_LBUTTONDOWN) == XHGRIDVIEW_STATUS_LBUTTONDOWN)
		OnLButtonUp(nFlags,point);
	SetCapture();
	ProcessMouseDown(point,2);
	SetFocus();
}

void CXHGridView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
}

int CXHGridView::GetItemIndexSearchVisible(CXHGridViewItemStructure *pItem)
{
	int iLow,iHigh;
	GetVisibleItems(&iLow,&iHigh);
	for(int i=iLow;i<=iHigh;i++)
	{
		if(m_arrpItems[i] == pItem)
			return i;
	}
	return -1;
}

DWORD CXHGridView::ModifyFlags(DWORD dwRemove,DWORD dwAdd)
{
	DWORD dwOldFlags = m_GridView.dwFlags;
	m_GridView.dwFlags &= (~dwRemove);
	m_GridView.dwFlags |= (dwAdd);
	return dwOldFlags;
}

void CXHGridView::EnsureVisible(int iIndex,BOOL bPartialOK)
{
	int iScrollPos = GetScrollPos(SB_VERT);
	int iViewHeight = GetItemViewHeight();
	int iItemHeight = GetHeaderItemStructure()->GetHeight() + GetItemSpacing();
	int nVisible = iViewHeight / iItemHeight;
	if(bPartialOK)
		if((iViewHeight % iItemHeight) > 0)
			nVisible++;
	if(iIndex >= iScrollPos && iIndex <= iScrollPos + nVisible - 1)		//Visible
	{
	}
	else
	{
		if(iIndex < iScrollPos)
			OnVScroll(SB_THUMBPOSITION,iIndex,NULL);
		else if(iIndex > iScrollPos + nVisible - 1)
			OnVScroll(SB_THUMBPOSITION,iIndex - nVisible + 1,NULL);
	}
}

BOOL CXHGridView::IsGridLines()
{
	return ((m_GridView.dwFlags & XHGRIDVIEW_FLAG_GRIDLINE) == XHGRIDVIEW_FLAG_GRIDLINE);
}

DWORD CXHGridView::GetReportNumber() const
{
	return m_GridView.dwReportNumber;
}

BOOL CXHGridView::SetHeaderFocusLeaf(CXHGridViewLeafItem *pItemLeafItem)
{
//	TRACE1("CXHGridView::SetHeaderFocusLeaf() - Begin  %d\n",GetTickCount());
	if(pItemLeafItem)
	{
		CXHGridViewLeafItem *pNewFocus = m_HeaderCtrl.GetHeaderItemStructure()->GetLeafByID(pItemLeafItem->GetID());
		if(m_HeaderCtrl.GetHeaderItemStructure()->GetLastFocusedLeafItem() != pNewFocus)
		{
			pNewFocus->SetFocused();
			m_HeaderCtrl.RedrawWindow();
		}
		return TRUE;
	}
	else
	{
		if(m_HeaderCtrl.GetHeaderItemStructure()->GetLastFocusedLeafItem())
		{				
			m_HeaderCtrl.GetHeaderItemStructure()->GetLastFocusedLeafItem()->SetFocused(FALSE);
			m_HeaderCtrl.GetHeaderItemStructure()->SetLastFocusedLeafItem(NULL);
			m_HeaderCtrl.RedrawWindow();
			return TRUE;
		}
	}
//	TRACE1("CXHGridView::SetHeaderFocusLeaf() - END  %d\n",GetTickCount());
	return FALSE;
}

int CXHGridView::GetFocusedItemIndex()
{
	if(m_GridView.iFocusedItemIndex >= 0)
		return m_GridView.iFocusedItemIndex;

	if(m_GridView.pFocusedItem)
		return (m_GridView.iFocusedItemIndex = GetItemIndex(m_GridView.pFocusedItem));

	return -1;
}

BOOL CXHGridView::IsFocused()
{
	return (::GetFocus() == m_hWnd);
}

DWORD CXHGridView::GetItemData(int iItemIndex)
{
	return m_arrpItems[iItemIndex]->GetItemData();
}

int CXHGridView::GetItemCount()
{
	return m_arrpItems.GetSize();
}

BOOL CXHGridView::SetItemData(int iItemIndex,DWORD dwItemData)
{
	if(iItemIndex < 0 || iItemIndex > m_arrpItems.GetSize() - 1)
		return FALSE;

	m_arrpItems[iItemIndex]->SetItemData(dwItemData);
	return TRUE;
}

int CXHGridView::GetItemIndex(CXHGridViewItemStructure *pItem)
{
	int iCount = m_arrpItems.GetSize();
	for(int i=0;i<iCount;i++)
		if(m_arrpItems[i] == pItem)
			return i;

	return -1;
}

void CXHGridView::OnItemClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit)
{
	if(pLeafHit)
		pLeafHit->TriggerClickEvent();

	// Derived classes must call OnItemClick of the base class
}

void CXHGridView::OnItemRClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit)
{
	if(pLeafHit)
		pLeafHit->TriggerClickEvent();

	// Derived classes must call OnItemClick of the base class
}

void CXHGridView::OnItemDblClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit)
{
	// Derived classes must call OnItemDblClick of the base class
}


LRESULT CXHGridView::OnSpecialMessage(WPARAM wParam,LPARAM lParam)
{
	if(lParam == XHGRIDVIEW_MESSAGE_HEADERCLICK)
	{
		OnHeaderClick((CXHGridViewLeafItem*)wParam);
		return 0;
	}
	
	ASSERT(FALSE);	//More message ID goes here
	return 0;
}

void CXHGridView::OnHeaderClick(CXHGridViewLeafItem *pClickedLeafItem)
{
}


BOOL CXHGridView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta < 0)
		OnVScroll(SB_LINEDOWN, abs(zDelta) ,NULL);
	else
		OnVScroll(SB_LINEUP, abs(zDelta) ,NULL);

	return CWnd::OnMouseWheel(nFlags,zDelta,pt);
}

void CXHGridView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType,cx,cy);

	ResetHScrollBar();
}

BOOL CXHGridView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

BOOL CXHGridView::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE;
		GetScrollInfo(SB_VERT,&si);
		switch(pMsg->wParam)
		{
		case VK_UP:
			MoveFocus(-1);
			break;
		case VK_DOWN:
			MoveFocus(1);
			break;
		case VK_PRIOR:
			MoveFocus(-((int)si.nPage));
			break;
		case VK_NEXT:
			MoveFocus(si.nPage);
			break;
		case VK_HOME:
			MoveFocus(-m_arrpItems.GetSize());
			break;
		case VK_END:
			MoveFocus(m_arrpItems.GetSize());
			break;
		case VK_LEFT:
			OnHScroll(SB_LINELEFT,0,NULL);
			break;
		case VK_RIGHT:
			OnHScroll(SB_LINERIGHT,0,NULL);
			break;
		case VK_SPACE:
			if(GetFocusedItemIndex() >= 0)
				if(MarkItemSelected(GetFocusedItemIndex()))
					RedrawItems(GetFocusedItemIndex());
			break;
		case VK_TAB:
			return FALSE;
		}
		return TRUE;	//Processed.
	}
	return FALSE;
}

void CXHGridView::OnKillFocus(CWnd* pNewWnd)
{
	if(m_GridView.iFocusedItemIndex != -1)
		RedrawItems(m_GridView.iFocusedItemIndex);
}

void CXHGridView::OnSetFocus(CWnd* pOldWnd)
{
	if(m_GridView.iFocusedItemIndex != -1)
		RedrawItems(m_GridView.iFocusedItemIndex);
}

LRESULT CXHGridView::OnInsertNewItem(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)Internal_InsertNewItem((int)wParam, (CXHGridViewItemStructure*)lParam);
}

LRESULT CXHGridView::OnDeleteItem(WPARAM wParam, LPARAM lParam)
{
	return (LRESULT)Internal_DeleteItem((int)wParam);
}

LRESULT CXHGridView::OnRedrawItems(WPARAM wParam, LPARAM lParam)
{
	Internal_RedrawItems(wParam, lParam);
	return 0;
}

CXHGridViewItemStructure* CXHGridView::CreateNewItem()
{
	CXHGridViewItemStructure *pNewItem = GetHeaderItemStructure()->CloneStructure();

	return pNewItem;
}

BOOL CXHGridView::MoveFocus(int iIndexDelta)
{
	int iFocused = GetFocusedItemIndex();
	int iFocusOld = iFocused;
	if(iFocused >= 0)
	{
		iFocused += iIndexDelta;
		iFocused = min(iFocused,m_arrpItems.GetSize() - 1);
		iFocused = max(0,iFocused);
	}
	else
	{
		iFocused = 0;
	}
	m_GridView.iFocusedItemIndex = iFocused;
	m_GridView.pFocusedItem = m_arrpItems[m_GridView.iFocusedItemIndex];
	if(iFocusOld != iFocused)
	{
		m_arrpItems[iFocused]->ModifyState(0, XHGRIDVIEW_LEAF_STATE_FOCUS);
		if(IsShiftKeyPressed())
		{
			MarkItemSelected(iFocused);
		}
		else if(!IsControlKeyPressed())
		{
			IncreaseSelectSession();
			RedrawVisibleItemsForSelected(iFocusOld);
			MarkItemSelected(iFocused);
		}
		if(iFocusOld >= 0)
		{
			m_arrpItems[iFocusOld]->ModifyState(XHGRIDVIEW_LEAF_STATE_FOCUS, 0);
			RedrawItems(iFocusOld);
		}
		EnsureVisible(m_GridView.iFocusedItemIndex);
		RedrawItems(iFocused);
		return TRUE;
	}
	else
		return FALSE;
}

void CXHGridView::IncreaseSelectSession()
{
	m_GridView.iSelectSession++;
	m_GridView.iSelectSession = (m_GridView.iSelectSession % XHGRIDVIEW_MAX_SELECT_SESSION);
	m_GridView.iFirstSelectedItemIndex = -1;
	m_GridView.iLastSelectedItemIndex = -1;
	m_GridView.iSelectedCount = 0;
//	TRACE1(_T("Select Count = %d  (IncreaseSelectSession)\r\n"),m_GridView.iSelectedCount);
}

int CXHGridView::GetFirstSelectedItemIndex()
{
	ASSERT(m_GridView.iSelectedCount >=0);	//iSelectedCount can not be negative.
	if(m_GridView.iSelectedCount == 0)
		return -1;
	ASSERT(m_GridView.iFirstSelectedItemIndex != -1);
	if(m_GridView.iFirstSelectedItemIndex == -1)
		return -1;
	while(!m_arrpItems[m_GridView.iFirstSelectedItemIndex]->IsSelected())
		m_GridView.iFirstSelectedItemIndex++;
	return m_GridView.iFirstSelectedItemIndex;
}

int CXHGridView::GetSelectedItemCount() const
{
	ASSERT(m_GridView.iSelectedCount >=0);	//iSelectedCount can not be negative.
	return m_GridView.iSelectedCount;
}

int CXHGridView::GetNextSelectedItemIndex(int &pos)
{
	if(m_GridView.iLastSelectedItemIndex == -1)
		return -1;
	pos++;
	while((!m_arrpItems[pos]->IsSelected()) && pos < m_GridView.iLastSelectedItemIndex)
		pos++;

	if(m_arrpItems[pos]->IsSelected())
		return pos;

	return -1;

}


//////////////////////////////////////////////////////////////////////
// CXHGridViewItem

CXHGridViewItem::CXHGridViewItem()
{

}

CXHGridViewItem::~CXHGridViewItem()
{

}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewItemStructure

CXHGridViewItemStructure::CXHGridViewItemStructure()
{
	_m_ItemStructure.iItemType = XHGRIDVIEW_ITEMTYPE_ITEM;
	_m_ItemStructure.iBaseID = 0;
	_m_ItemStructure.pLastFocusedLeafItem = NULL;
	_m_ItemStructure.dwItemData = 0;
	_m_ItemStructure.dwReportNumber = 0;
	_m_ItemStructure.iSelectSession = 0;
	_m_ItemStructure.dwState = 0;

	m_pParentGridView = NULL;
	m_pBand = NULL;
	m_BaseItems.SetParentStructure(this);
	m_pDrawer = &CXHGridView::s_ItemDrawer;
}

CXHGridViewItemStructure::~CXHGridViewItemStructure()
{

}

CXHGridViewBaseItem* CXHGridViewItemStructure::InsertBaseItem(int iIndex,int iWidth)
{
	return m_BaseItems.InsertBaseItem(iIndex,iWidth);
}

CXHGridViewBaseItem* CXHGridViewItemStructure::GetBaseItem(int iIndex)
{
	return m_BaseItems.GetBaseItem(iIndex);
}

CXHGridViewSubItem* CXHGridViewItemStructure::GetSubItem(int iBase,int iSub)
{
	return m_BaseItems.GetBaseItem(iBase)->GetSubItem(iSub);
}

CXHGridViewLeafItem* CXHGridViewItemStructure::GetLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex)
{
	return m_BaseItems.GetBaseItem(iBaseIndex)->GetSubItem(iSubIndex)->GetLeafItem(iLeafIndex);
}

int CXHGridViewItemStructure::GetBaseItemCount() const
{
	return m_BaseItems.GetCount();
}

CXHGridViewBaseItem* CXHGridViewItemStructure::BaseHitTest(POINT point)
{
	return m_BaseItems.BaseHitTest(point);
}

void CXHGridViewItemStructure::SetParentGridView(CXHGridView *pParent)
{
	m_pParentGridView = pParent;
}

CXHGridView* CXHGridViewItemStructure::GetParentGridView()
{
	return m_pParentGridView;
}

int CXHGridViewItemStructure::GetHeight() const
{
	if(_m_ItemStructure.iItemType == XHGRIDVIEW_ITEMTYPE_ITEM
		|| _m_ItemStructure.iItemType == XHGRIDVIEW_ITEMTYPE_HEADER)
	{
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		{
			return GetFixHeight();
		}
		
		//Default : XHGRIDVIEW_REPORT
		return m_pParentGridView->GetRowHeight();
	}
	else // XHGRIDVIEW_ITEMTYPE_BAND or XHGRIDVIEW_ITEMTYPE_HEADER
	{
		return GetFixHeight();
	}
}

int CXHGridViewItemStructure::GetFixHeight() const
{
	int nMaxRow = m_BaseItems.GetMaxBaseItemRowCount();
	return m_pParentGridView->GetRowHeight() * nMaxRow;
}

CXHGridViewLeafItem* CXHGridViewItemStructure::GetLastFocusedLeafItem()
{
	return _m_ItemStructure.pLastFocusedLeafItem;
}

void CXHGridViewItemStructure::SetLastFocusedLeafItem(CXHGridViewLeafItem *pLeafFocused)
{
	_m_ItemStructure.pLastFocusedLeafItem = pLeafFocused;
}

int CXHGridViewItemStructure::GetWidth()
{
//	TRACE(_T("----- CXHGridViewItemStructure::GetWidth() -----\n"));
	if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		return m_BaseItems.GetBaseItemWidth(0,-1) + m_pParentGridView->GetMarkWidth();
	else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
		return GetAllLeafTotalWidth();

	//Default : XHGRIDVIEW_DETAIL ?
	ASSERT(FALSE);
	return 0;
}

int CXHGridViewItemStructure::GetAllLeafTotalWidth()
{
	if(_m_ItemStructure.dwReportNumber != m_pParentGridView->GetReportNumber())
	{
		RearrangeLeafList();
		_m_ItemStructure.dwReportNumber = m_pParentGridView->GetReportNumber();
	}

	int iTotalWidth = 0;
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		iTotalWidth += m_arrpLeafItems[i]->GetFactWidth();

	iTotalWidth += m_pParentGridView->GetMarkWidth();

	return iTotalWidth;
}

CXHGridViewLeafDrawHelper* CXHGridViewItemStructure::GetDrawer()
{
	return m_pDrawer;
}

void CXHGridViewItemStructure::SetItemData(DWORD dwData)
{
	_m_ItemStructure.dwItemData = dwData;
}

DWORD CXHGridViewItemStructure::GetItemData() const
{
	return _m_ItemStructure.dwItemData;
}

void CXHGridViewItemStructure::GetItemRect(LPRECT lpRect)
{
	SetRect(lpRect,0,0,GetWidth(),GetHeight());
}

CXHGridViewLeafItem* CXHGridViewItemStructure::GetLeafByID(int iLeafID)
{
	return m_BaseItems.FindLeafByID(iLeafID);
}

BOOL CXHGridViewItemStructure::IsFocused() const
{
	return ((_m_ItemStructure.dwState & XHGRIDVIEW_ITEM_STATE_FOCUSED) == XHGRIDVIEW_ITEM_STATE_FOCUSED);
}

CXHGridViewLeafItem* CXHGridViewItemStructure::LeafHitTest(POINT point)
{
	switch(m_pParentGridView->GetViewType())
	{
	case XHGRIDVIEW_DETAIL:
		return Detail_LeafHitTest(point);
		break;
	case XHGRIDVIEW_REPORT:
		return Report_LeafHitTest(point);
		break;
	default:
		ASSERT(FALSE);
	}
	return NULL;
}

CXHGridViewLeafItem* CXHGridViewItemStructure::Detail_LeafHitTest(POINT point)
{
	CXHGridViewBaseItem *pBase = BaseHitTest(point);		//ItemStructure 全局坐标,只检测 X
	if(pBase == NULL)
		return NULL;

	CXHGridViewSubItem *pSub = pBase->SubItemHitTest(point);	//ItemStructure 全局坐标,只检测 Y
	if(pSub == NULL)
		return NULL;

	//转换成 SubItem 局部坐标,注意,只转换了 X 坐标
	int iBaseIndex = m_BaseItems.GetBaseItemIndex(pBase);
	ASSERT(iBaseIndex >= 0);
	point.x -= m_BaseItems.GetBaseItemWidth(0,iBaseIndex);
	point.x -= m_pParentGridView->GetMarkWidth();

	return pSub->LeafHitTest(point);	//SubItem 局部坐标,只检测 X
}

CXHGridViewLeafItem* CXHGridViewItemStructure::Report_LeafHitTest(POINT point)
{
	point.x -= m_pParentGridView->GetMarkWidth();
	
	int iCurXPos = 0;
	int iLeafWidth = 0;
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
	{
		iLeafWidth = m_arrpLeafItems[i]->GetFactWidth();
		if(point.x > iCurXPos && point.x < iCurXPos + iLeafWidth)
			if(point.y > 0 && point.y < m_arrpLeafItems[i]->GetHeight())
				return m_arrpLeafItems[i];

		iCurXPos += iLeafWidth;
	}
	return NULL;
}

CXHGridViewSubItem* CXHGridViewItemStructure::InsertSubItem(int iBase,int iSub)
{
	return m_BaseItems.GetBaseItem(iBase)->InsertSubItem(iSub);
}

CXHGridViewLeafItem* CXHGridViewItemStructure::InsertLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex,LPCTSTR lpszText,int iWidth)
{
	return m_BaseItems.GetBaseItem(iBaseIndex)->GetSubItem(iSubIndex)->InsertLeafItem(iLeafIndex,lpszText,iWidth);
}

void CXHGridViewItemStructure::SetItemType(int iNewType)
{
	_m_ItemStructure.iItemType = iNewType;
	switch(_m_ItemStructure.iItemType)
	{
	case XHGRIDVIEW_ITEMTYPE_HEADER:
		m_pDrawer = &CXHGridView::s_HeaderDrawer;
		break;
	case XHGRIDVIEW_ITEMTYPE_BAND:
		m_pDrawer = &CXHGridView::s_BandDrawer;
		break;
	case XHGRIDVIEW_ITEMTYPE_ITEM:
		m_pDrawer = &CXHGridView::s_ItemDrawer;
		break;
	}
}

int CXHGridViewItemStructure::GetItemType() const
{
	return _m_ItemStructure.iItemType;
}

void CXHGridViewItemStructure::RearrangeBottomLeafList()
{
	m_arrpBottomLeafItems.RemoveAll();
	m_BaseItems.RearrangeBottomLeafList(&m_arrpBottomLeafItems);
}

void CXHGridViewItemStructure::RearrangeLeafList()
{
	m_arrpLeafItems.RemoveAll();
	m_BaseItems.RearrangeLeafList(&m_arrpLeafItems);
}

BOOL CXHGridViewItemStructure::SetBand(CXHGridViewItemStructure *pBandItem)
{
	ASSERT(GetItemType() == XHGRIDVIEW_ITEMTYPE_HEADER);

	if(pBandItem == NULL)
	{
		m_pBand = NULL;
		return FALSE;
	}
	
	ASSERT(pBandItem->GetItemType() == XHGRIDVIEW_ITEMTYPE_BAND);

	m_pBand = pBandItem;
	if(!ResetBandMap())
	{
		m_pBand = NULL;
		return FALSE;
	}
	return TRUE;
}

CXHGridViewItemStructure* CXHGridViewItemStructure::GetBand()
{
	return m_pBand;
}

BOOL CXHGridViewItemStructure::ResetBandMap()
{
	ASSERT(GetItemType() == XHGRIDVIEW_ITEMTYPE_HEADER);

	if(!m_pBand)
		return FALSE;

	m_mapToBand.RemoveAll();
	m_pBand->RearrangeBottomLeafList();

	if(m_pBand->GetLeafListSize() != m_BaseItems.GetCount())	//Leaf count not fit
		return FALSE;

	for(int i=0;i<m_pBand->GetLeafListSize();i++)
		m_mapToBand.SetAt(m_BaseItems.GetBaseItem(i),m_pBand->m_arrpBottomLeafItems[i]);

	return TRUE;
}

int CXHGridViewItemStructure::GetLeafListSize() const
{
	return m_arrpBottomLeafItems.GetSize();
}

BOOL CXHGridViewItemStructure::IsBand()
{
	if(GetItemType() == XHGRIDVIEW_ITEMTYPE_HEADER)
		if(m_pBand)
			return TRUE;
		
	return FALSE;
}

int CXHGridViewItemStructure::NewID()
{
	return _m_ItemStructure.iBaseID++;
}

int CXHGridViewItemStructure::GetMapedBaseItemWidth(const CXHGridViewBaseItem *pKey) const
{
	CXHGridViewLeafItem *pFound = NULL;
	VERIFY(m_mapToBand.Lookup(pKey,pFound));
	return pFound->GetFactWidth();
}

void CXHGridViewItemStructure::Draw(CDC *pDC,int x,int y)
{
	Draw(pDC,CPoint(x,y));
}

void CXHGridViewItemStructure::Draw(CDC *pDC,POINT point)
{
	CPoint ptOld = pDC->GetViewportOrg();

	pDC->SetViewportOrg(CPoint(ptOld.x + point.x,ptOld.y + point.y));

	DrawMarker(pDC);

	pDC->SetViewportOrg(CPoint(ptOld.x + point.x + m_pParentGridView->GetMarkWidth() ,ptOld.y + point.y));

	//(4L) draw top and bottom line of the item
	if(m_pParentGridView->GetItemSpacing() > 0)
		m_pDrawer->DrawLine(m_pParentGridView,pDC,0,0,GetWidth() - m_pParentGridView->GetMarkWidth(),0);
	m_pDrawer->DrawLine(m_pParentGridView,pDC,0,GetHeight() - 1,GetWidth() - m_pParentGridView->GetMarkWidth(),GetHeight() - 1);
//	pDC->MoveTo(0,0);
//	pDC->LineTo(GetWidth() - m_pParentGridView->GetMarkWidth(),0);
//	pDC->MoveTo(0,GetHeight() - 1);
//	pDC->LineTo(GetWidth() - m_pParentGridView->GetMarkWidth(),GetHeight() - 1);

	DrawItemStructure(pDC);

	pDC->SetViewportOrg(ptOld);
}

void CXHGridViewItemStructure::DrawItemStructure(CDC *pDC)
{
	if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		m_BaseItems.Draw(pDC);
	else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
		DrawReport(pDC);
	else
		ASSERT(FALSE);// Other ViewType
}

void CXHGridViewItemStructure::DrawReport(CDC *pDC)
{
	CPoint ptOldVOrg = pDC->GetViewportOrg();
	int iCurXPos = 0;
	int iLeafWidth = 0;
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
	{
		iLeafWidth = m_arrpLeafItems[i]->GetFactWidth();
		pDC->SetViewportOrg(ptOldVOrg.x + iCurXPos, ptOldVOrg.y);
		m_pDrawer->Draw(pDC,m_arrpLeafItems[i]);
		m_pDrawer->DrawLine(m_pParentGridView,pDC,iLeafWidth - m_pParentGridView->GetLineWidth(),0,iLeafWidth - m_pParentGridView->GetLineWidth(),m_arrpLeafItems[i]->GetHeight());
		iCurXPos += iLeafWidth;
	}
}

BOOL CXHGridViewItemStructure::BaseSideHitTest(CXHGridViewBaseItem **ppBaseItemLeft,CXHGridViewBaseItem **ppBaseItemRight,CPoint point)
{

	CXHGridViewBaseItem *pHit1 = BaseHitTest(CPoint(point.x - 2,point.y));
	CXHGridViewBaseItem *pHit2 = BaseHitTest(CPoint(point.x + 2,point.y));
	if(pHit1 != pHit2 && pHit1 != NULL)
	{
		*ppBaseItemLeft = pHit1;
		*ppBaseItemRight = pHit2;
		return TRUE;
	}
	*ppBaseItemLeft = NULL;
	*ppBaseItemRight = NULL;
	return FALSE;
}

void CXHGridViewItemStructure::DrawMarker(CDC *pDC)
{
	m_pDrawer->DrawMarker(pDC, this);
}

int CXHGridViewItemStructure::GetMaxBaseItemLeafCount(CXHGridViewBaseItem *pBaseItemRoot)
{
	if(pBaseItemRoot)
		return pBaseItemRoot->GetMaxBaseItemLeafCount();

	return m_BaseItems.GetMaxBaseItemLeafCount();
}

BOOL CXHGridViewItemStructure::LeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point)
{
	CXHGridViewLeafItem *pHit1 = LeafHitTest(CPoint(point.x - 2,point.y));
	CXHGridViewLeafItem *pHit2 = LeafHitTest(CPoint(point.x + 2,point.y));
	
	if(pHit1 != pHit2)
	{
		if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
		{
			if(pHit1 != NULL && pHit2 != NULL
				&& pHit1->GetParentLeafItems() == pHit2->GetParentLeafItems())
			{
				*ppLeafItemLeft = pHit1;
				*ppLeafItemRight = pHit2;
				return TRUE;
			}
		}
		else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
		{
			if(pHit1 != NULL)
			{
				*ppLeafItemLeft = pHit1;
				*ppLeafItemRight = pHit2;
				return TRUE;
			}
		}
	}
	
	*ppLeafItemLeft = NULL;
	*ppLeafItemRight = NULL;
	return FALSE;
	
//	if(m_pParentGridView->GetViewType() == XHGRIDVIEW_DETAIL)
//		return Detail_LeafSideHitTest(ppLeafItemLeft,ppLeafItemRight,point);
//	else if(m_pParentGridView->GetViewType() == XHGRIDVIEW_REPORT)
//		return Report_LeafSideHitTest(ppLeafItemLeft,ppLeafItemRight,point);
//
}

//
//BOOL CXHGridViewItemStructure::Detail_LeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point)
//{
//	CXHGridViewLeafItem *pHit1 = LeafHitTest(CPoint(point.x - 2,point.y));
//	CXHGridViewLeafItem *pHit2 = LeafHitTest(CPoint(point.x + 2,point.y));
//	if(pHit1 != pHit2 && pHit1 != NULL && pHit2 != NULL
//		&& pHit1->GetParentLeafItems() == pHit2->GetParentLeafItems())
//	{
//		*ppLeafItemLeft = pHit1;
//		*ppLeafItemRight = pHit2;
//		return TRUE;
//	}
//	*ppLeafItemLeft = NULL;
//	*ppLeafItemRight = NULL;
//	return FALSE;
//}
//
//BOOL CXHGridViewItemStructure::Report_LeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point)
//{
//	int iCurXPos = 0;
//	int iLeafWidth = 0;
//	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
//	{
//		iLeafWidth = m_arrpLeafItems[i]->GetFactWidth();
//		if(point.x > iCurXPos && point.x < iCurXPos + iLeafWidth)
//			return TRUE;
//		iCurXPos += iLeafWidth;
//	}
//}

BOOL CXHGridViewItemStructure::MarkSelected(int iSelectSession)
{
	BOOL bSet = ((_m_ItemStructure.dwState & XHGRIDVIEW_ITEM_STATE_SELECTED) == 0 || _m_ItemStructure.iSelectSession != iSelectSession);
	_m_ItemStructure.dwState |= XHGRIDVIEW_ITEM_STATE_SELECTED;
	_m_ItemStructure.iSelectSession = iSelectSession;
//	if(bSet)
//		TRACE(("Marked!\r\n"));
	return bSet;
}

BOOL CXHGridViewItemStructure::ChangeSelected(int iSelectSession)
{
	if(IsSelected())
		return MarkUnselected(iSelectSession);
	else
		return MarkSelected(iSelectSession);
	
	/*
	if(_m_ItemStructure.dwState & XHGRIDVIEW_ITEM_STATE_SELECTED)
		_m_ItemStructure.dwState &= (~XHGRIDVIEW_ITEM_STATE_SELECTED);
	else
		_m_ItemStructure.dwState |= XHGRIDVIEW_ITEM_STATE_SELECTED;

	_m_ItemStructure.iSelectSession = iSelectSession;

	return TRUE;
	*/
}

BOOL CXHGridViewItemStructure::RefreshSelected(int iSelectSession)
{
	if(_m_ItemStructure.iSelectSession == iSelectSession)
	{
		return MarkSelected(iSelectSession);
	}
	else
	{
		return MarkUnselected(iSelectSession);
	}
}



BOOL CXHGridViewItemStructure::MarkUnselected(int iSelectSession)
{
	BOOL bSet = ((_m_ItemStructure.dwState & XHGRIDVIEW_ITEM_STATE_SELECTED) == XHGRIDVIEW_ITEM_STATE_SELECTED && _m_ItemStructure.iSelectSession == iSelectSession);
	_m_ItemStructure.dwState &= (~XHGRIDVIEW_ITEM_STATE_SELECTED);
//	if(bSet)
//		TRACE(("Un-Marked!\r\n"));
	return bSet;
}

BOOL CXHGridViewItemStructure::IsSelected()
{
	if((_m_ItemStructure.dwState & XHGRIDVIEW_ITEM_STATE_SELECTED) == XHGRIDVIEW_ITEM_STATE_SELECTED
		&& _m_ItemStructure.iSelectSession == GetParentGridView()->GetCurrentSelectSession())
		return TRUE;
	return FALSE;
}

DWORD CXHGridViewItemStructure::GetStatus() const
{
	return _m_ItemStructure.dwState;
}

BOOL CXHGridViewItemStructure::BotomLeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point)
{
	CXHGridViewLeafItem *pHit1 = LeafHitTest(CPoint(point.x - 2,point.y));
	CXHGridViewLeafItem *pHit2 = LeafHitTest(CPoint(point.x + 2,point.y));
	if(pHit1 != pHit2 && pHit1 != NULL && pHit2 != NULL)
	{
		if(pHit1->GetParentLeafItems()->GetParentSubItem()->GetParentSubItems()->IsLastSubItem(pHit1->GetParentLeafItems()->GetParentSubItem()))
		{			
			*ppLeafItemLeft = pHit1;
			*ppLeafItemRight = pHit2;
			return TRUE;
		}
	}
	return FALSE;
}

void CXHGridViewItemStructure::ModifyState(DWORD dwRemove,DWORD dwAdd)
{
	_m_ItemStructure.dwState &= (~dwRemove);
	_m_ItemStructure.dwState |= dwAdd;
}

DWORD CXHGridViewItemStructure::GetState()
{
	return _m_ItemStructure.dwState;
}

CXHGridViewItemStructure* CXHGridViewItemStructure::CloneStructure()
{
	CXHGridViewItemStructure *pNewStructure = new CXHGridViewItemStructure();
	ASSERT(pNewStructure);
	pNewStructure->m_BaseItems.CopyFrom(&m_BaseItems);
	pNewStructure->m_BaseItems.SetParentStructure(pNewStructure);
	return pNewStructure;
}

void CXHGridViewItemStructure::SetLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex,CXHGridViewLeafItem *pNew)
{
	ASSERT(pNew);
	CXHGridViewLeafItem *pOldLeafItem = GetLeafItem(iBaseIndex, iSubIndex, iLeafIndex);

	for(int i=0;i<m_arrpLeafItems.GetCount();i++)
	{
		if(m_arrpLeafItems[i] == pOldLeafItem)
		{
			m_arrpLeafItems[i] = pNew;
			break;
		}
	}

	GetBaseItem(iBaseIndex)->GetSubItem(iSubIndex)->SetLeafItem(iLeafIndex, pNew);
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewBaseItems

CXHGridViewBaseItems::CXHGridViewBaseItems()
{
	_m_BaseItemsInfo.iMaxRowCount = 0;
	m_pParentStructure = NULL;
}

CXHGridViewBaseItems::~CXHGridViewBaseItems()
{
	DeleteAllBaseItems();
}

CXHGridViewBaseItem* CXHGridViewBaseItems::InsertBaseItem(int iIndex,int iWidth)
{
	CXHGridViewBaseItem *pNew = new CXHGridViewBaseItem();
	ASSERT(pNew);


	//下面两行顺序不能改变！
	pNew->SetWidth(iWidth);
	pNew->SetParentBaseItems(this);

	m_arrpBaseItems.InsertAt(iIndex,pNew);

	if(pNew->GetSubItemCount() > _m_BaseItemsInfo.iMaxRowCount)
		_m_BaseItemsInfo.iMaxRowCount = pNew->GetSubItemCount();

	return pNew;
}

void CXHGridViewBaseItems::DeleteAllBaseItems()
{
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		if(m_arrpBaseItems[i])
			delete m_arrpBaseItems[i];
	m_arrpBaseItems.RemoveAll();
}

CXHGridViewBaseItem* CXHGridViewBaseItems::GetBaseItem(int iIndex)
{
	return m_arrpBaseItems[iIndex];
}

int CXHGridViewBaseItems::GetCount() const
{
	return m_arrpBaseItems.GetSize();
}

void CXHGridViewBaseItems::SetParentStructure(CXHGridViewItemStructure *pParent)
{
	m_pParentStructure = pParent;
}

int CXHGridViewBaseItems::GetMaxBaseItemLeafCount()
{
	int iMax = 0;
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		iMax = max(iMax,m_arrpBaseItems[i]->GetMaxBaseItemLeafCount());
	return iMax;
}

CXHGridViewItemStructure* CXHGridViewBaseItems::GetParentStructure()
{
	return m_pParentStructure;
}

CXHGridViewBaseItem* CXHGridViewBaseItems::BaseHitTest(POINT point)
{
	int iCurPos = m_pParentStructure->GetParentGridView()->GetMarkWidth();
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
	{
		if(point.x > iCurPos && point.x < iCurPos + m_arrpBaseItems[i]->GetWidth())
			if(point.y > 0 && point.y < m_pParentStructure->GetHeight())
				return m_arrpBaseItems[i];
		iCurPos += m_arrpBaseItems[i]->GetWidth();
	}
	return NULL;
}
CXHGridViewSubItem* CXHGridViewBaseItems::SubItemHitTest(POINT point)
{
	CXHGridViewBaseItem *pBase = BaseHitTest(point);
	if(pBase == NULL)
		return NULL;

	return pBase->SubItemHitTest(point);
}

int CXHGridViewBaseItems::GetMaxBaseItemRowCount() const
{
	return _m_BaseItemsInfo.iMaxRowCount;
}

void CXHGridViewBaseItems::SetMaxBaseItemRowCount(int iMaxRowCount)
{
	_m_BaseItemsInfo.iMaxRowCount = iMaxRowCount;
}
int CXHGridViewBaseItems::GetBaseItemIndex(CXHGridViewBaseItem *pBaseItem) const
{
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		if(m_arrpBaseItems[i] == pBaseItem)
			return i;
	return -1;
}

int CXHGridViewBaseItems::GetBaseItemWidth(int iIndexStart,int iCount) const
{
	int iTotalWidth = 0;
	if(iCount == -1)
		iCount = m_arrpBaseItems.GetSize() - iIndexStart;
	for(int i=0;i<iCount;i++)
		iTotalWidth += m_arrpBaseItems[iIndexStart + i]->GetWidth();
	return iTotalWidth;
}

void CXHGridViewBaseItems::RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		m_arrpBaseItems[i]->RearrangeBottomLeafList(pLeafList);
}

void CXHGridViewBaseItems::RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		m_arrpBaseItems[i]->RearrangeLeafList(pLeafList);
}

void CXHGridViewBaseItems::Draw(CDC *pDC)
{
	CPoint ptOld;
	int iCurXPos = 0;
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
	{
		ptOld = pDC->GetViewportOrg();
		pDC->SetViewportOrg(CPoint(ptOld.x + iCurXPos, ptOld.y));
		m_arrpBaseItems[i]->Draw(pDC);
		pDC->SetViewportOrg(ptOld);
		iCurXPos += m_arrpBaseItems[i]->GetWidth();
	}
}

void CXHGridViewBaseItems::CopyFrom(CXHGridViewBaseItems *pBaseItems)
{
	DeleteAllBaseItems();
	m_arrpBaseItems.SetSize(pBaseItems->GetCount());
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
	{
		m_arrpBaseItems[i] = pBaseItems->m_arrpBaseItems[i]->CloneBaseItem();
		ASSERT(m_arrpBaseItems[i]);
		m_arrpBaseItems[i]->SetParentBaseItems(this);
	}
	memcpy(&_m_BaseItemsInfo,&pBaseItems->_m_BaseItemsInfo,sizeof(_m_BaseItemsInfo));
}

CXHGridViewLeafItem* CXHGridViewBaseItems::FindLeafByID(int iLeafID)
{
	CXHGridViewLeafItem *pFind = NULL;
	for(int i=0;i<m_arrpBaseItems.GetSize();i++)
		if((pFind = m_arrpBaseItems[i]->FindLeafByID(iLeafID)))
			return pFind;
	ASSERT(FALSE);
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
// CXHGridViewBaseItem

CXHGridViewBaseItem::CXHGridViewBaseItem()
{
	m_SubItems.SetParentBaseItem(this);
	m_pParentBaseItems = NULL;
	_m_BaseItemInfo.dwFlags = XHGRIDVIEW_BASEITEM_FLAG_STRETCH;
//	VERIFY(m_SubItems.InsertSubItem(0));
}

CXHGridViewBaseItem::~CXHGridViewBaseItem()
{

}

void CXHGridViewBaseItem::SetWidth(int iNewWidth)
{
	m_iWidth = iNewWidth;
}

int CXHGridViewBaseItem::GetWidth()
{
	if(m_pParentBaseItems->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_ITEM)
		return m_pParentBaseItems->GetParentStructure()->GetParentGridView()->GetHeaderItemStructure()->GetBaseItem(m_pParentBaseItems->GetBaseItemIndex(this))->GetWidth();

	if(m_pParentBaseItems->GetParentStructure()->IsBand())
		return m_pParentBaseItems->GetParentStructure()->GetMapedBaseItemWidth(this);
	return m_iWidth;
}

CXHGridViewSubItem* CXHGridViewBaseItem::GetSubItem(int iIndex)
{
	return m_SubItems.GetSubItem(iIndex);
}

void CXHGridViewBaseItem::SetParentBaseItems(CXHGridViewBaseItems *pParent)
{
	m_pParentBaseItems = pParent;
}

CXHGridViewBaseItems* CXHGridViewBaseItem::GetParentBaseItems()
{
	return m_pParentBaseItems;
}

CXHGridViewSubItem* CXHGridViewBaseItem::SubItemHitTest(POINT point)
{
	return m_SubItems.SubItemHitTest(point);
}

int CXHGridViewBaseItem::GetMaxBaseItemLeafCount()
{
	return m_SubItems.GetMaxBaseItemLeafCount();
}

CXHGridViewSubItem* CXHGridViewBaseItem::InsertSubItem(int iIndex)
{
	CXHGridViewSubItem *pInserted = m_SubItems.InsertSubItem(iIndex);
	 
	if(GetSubItemCount() > m_pParentBaseItems->GetMaxBaseItemRowCount())
		m_pParentBaseItems->SetMaxBaseItemRowCount(GetSubItemCount());

	return pInserted;
}

int CXHGridViewBaseItem::GetSubItemCount() const
{
	return m_SubItems.GetCount();
}

void CXHGridViewBaseItem::RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	m_SubItems.RearrangeBottomLeafList(pLeafList);
}

void CXHGridViewBaseItem::RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	m_SubItems.RearrangeLeafList(pLeafList);
}

void CXHGridViewBaseItem::Draw(CDC *pDC)
{
	CXHGridView *pGridView = GetParentBaseItems()->GetParentStructure()->GetParentGridView();
	CXHGridViewLeafDrawHelper *pDrawer = GetParentBaseItems()->GetParentStructure()->GetDrawer();
	pDrawer->DrawLine(pGridView,pDC,GetWidth() - 1,0,GetWidth() - 1,GetHeight());
	
//	pDC->MoveTo(GetWidth() - 1,0);
//	pDC->LineTo(GetWidth() - 1,GetHeight());

	m_SubItems.Draw(pDC);
}

int CXHGridViewBaseItem::GetHeight()
{
	return m_pParentBaseItems->GetParentStructure()->GetHeight();
}

BOOL CXHGridViewBaseItem::IsStretch()
{
	//Item 类型，返回其 HeaderItemStructure 中对应 BaseItem 的 IsStretch() 值
	if(m_pParentBaseItems->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_ITEM)
		return m_pParentBaseItems->GetParentStructure()->GetParentGridView()->GetHeaderItemStructure()->GetBaseItem(GetParentBaseItems()->GetBaseItemIndex(this))->IsStretch();
	else if(m_pParentBaseItems->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_BAND)
		return ((_m_BaseItemInfo.dwFlags & XHGRIDVIEW_BASEITEM_FLAG_STRETCH) == XHGRIDVIEW_BASEITEM_FLAG_STRETCH);
	else if(m_pParentBaseItems->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_HEADER)
		return ((_m_BaseItemInfo.dwFlags & XHGRIDVIEW_BASEITEM_FLAG_STRETCH) == XHGRIDVIEW_BASEITEM_FLAG_STRETCH);

	ASSERT(FALSE);
	return FALSE;
}

CXHGridViewBaseItem* CXHGridViewBaseItem::CloneBaseItem()
{
	CXHGridViewBaseItem *pNewBaseItem = new CXHGridViewBaseItem();
	ASSERT(pNewBaseItem);
	
	pNewBaseItem->m_SubItems.CopyFrom(&m_SubItems);
	pNewBaseItem->m_SubItems.SetParentBaseItem(pNewBaseItem);
	pNewBaseItem->m_iWidth = m_iWidth;
	
	return pNewBaseItem;
}

CXHGridViewLeafItem* CXHGridViewBaseItem::FindLeafByID(int iLeafID)
{
	return m_SubItems.FindLeafByID(iLeafID);
}


//////////////////////////////////////////////////////////////////////////
// CXHGridViewSubItems

CXHGridViewSubItems::CXHGridViewSubItems()
{
	m_pParentBaseItem = NULL;
}

CXHGridViewSubItems::~CXHGridViewSubItems()
{
	DeleteAllSubItems();
}

CXHGridViewSubItem* CXHGridViewSubItems::InsertSubItem(int iIndex)
{
	CXHGridViewSubItem *pNew = new CXHGridViewSubItem();
	ASSERT(pNew);

	pNew->SetParentSubItems(this);
	m_arrpSubItems.InsertAt(iIndex,pNew);

	return pNew;
}

CXHGridViewSubItem* CXHGridViewSubItems::GetSubItem(int iIndex)
{
	return m_arrpSubItems[iIndex];
}

int CXHGridViewSubItems::GetCount() const
{
	return m_arrpSubItems.GetSize();
}

void CXHGridViewSubItems::CopyFrom(CXHGridViewSubItems *pSubItems)
{
	DeleteAllSubItems();
	m_arrpSubItems.SetSize(pSubItems->GetCount());
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
	{
		m_arrpSubItems[i] = pSubItems->m_arrpSubItems[i]->CloneSubItem();
		ASSERT(m_arrpSubItems[i]);
		m_arrpSubItems[i]->SetParentSubItems(this);
	}
}

void CXHGridViewSubItems::DeleteAllSubItems()
{
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
		if(m_arrpSubItems[i])
			delete m_arrpSubItems[i];
	m_arrpSubItems.RemoveAll();
}

void CXHGridViewSubItems::SetParentBaseItem(CXHGridViewBaseItem* pParent)
{
	m_pParentBaseItem = pParent;
}

CXHGridViewBaseItem* CXHGridViewSubItems::GetParentBaseItem()
{
	return m_pParentBaseItem;
}

int CXHGridViewSubItems::GetMaxBaseItemLeafCount()
{
	int iMax = 0;
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
		iMax = max(iMax,m_arrpSubItems[i]->GetLeafCount());
	return iMax;
}

CXHGridViewSubItem* CXHGridViewSubItems::SubItemHitTest(POINT point)
{
	if(GetParentBaseItem()->GetParentBaseItems()->GetParentStructure()->GetParentGridView()->GetViewType() == XHGRIDVIEW_DETAIL)
	{
		int iCurYPos = 0;
		int iRowHeight = m_pParentBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView()->GetRowHeight();
		if(m_pParentBaseItem->IsStretch())
			iRowHeight = m_pParentBaseItem->GetParentBaseItems()->GetMaxBaseItemRowCount() * iRowHeight / GetCount();
		for(int i=0;i<m_arrpSubItems.GetSize();i++)
		{
			if(point.y > iCurYPos && point.y < iCurYPos + iRowHeight)
				return m_arrpSubItems[i];
			iCurYPos += iRowHeight;
		}

		//Not found!
		return NULL;
	}

	//for other view type,not implemented
	ASSERT(FALSE);
	return NULL;
}

void CXHGridViewSubItems::RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	m_arrpSubItems[m_arrpSubItems.GetSize() - 1]->RearrangeBottomLeafList(pLeafList);
}

void CXHGridViewSubItems::RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
		m_arrpSubItems[i]->RearrangeLeafList(pLeafList);
}

void CXHGridViewSubItems::Draw(CDC *pDC)
{
	CPoint ptOld;
	int iCurYPos = 0;
	CXHGridView *pGridView = m_pParentBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView();
	CXHGridViewLeafDrawHelper *pDrawer = m_pParentBaseItem->GetParentBaseItems()->GetParentStructure()->GetDrawer();
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
	{
		ptOld = pDC->GetViewportOrg();
		pDC->SetViewportOrg(CPoint(ptOld.x, ptOld.y + iCurYPos));

		//(5L) Draw lines
		if(i != m_arrpSubItems.GetSize() - 1)
		{
			pDrawer->DrawLine(pGridView,pDC,0, m_arrpSubItems[i]->GetHeight(),m_pParentBaseItem->GetWidth(), m_arrpSubItems[i]->GetHeight());
//			pDC->MoveTo(0, m_arrpSubItems[i]->GetHeight());
//			pDC->LineTo(m_pParentBaseItem->GetWidth(), m_arrpSubItems[i]->GetHeight());
		}

		m_arrpSubItems[i]->Draw(pDC);
		pDC->SetViewportOrg(ptOld);
		iCurYPos += m_arrpSubItems[i]->GetHeight();
	}
}

CXHGridViewLeafItem* CXHGridViewSubItems::FindLeafByID(int iLeafID)
{
	CXHGridViewLeafItem *pFind = NULL;
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
		if((pFind = m_arrpSubItems[i]->FindLeafByID(iLeafID)))
			return pFind;
	return NULL;
}

BOOL CXHGridViewSubItems::IsLastSubItem(CXHGridViewSubItem *pSubItem)
{
	return (pSubItem == m_arrpSubItems[m_arrpSubItems.GetSize() - 1]);
}

int CXHGridViewSubItems::GetSubItemIndex(CXHGridViewSubItem *pSubItem)
{
	for(int i=0;i<m_arrpSubItems.GetSize();i++)
		if(m_arrpSubItems[i] == pSubItem)
			return i;
	return -1;
}

int CXHGridViewSubItems::GetSubItemHeight(int iIndexStart,int iCount)
{
	CXHGridView *pGridView = m_pParentBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView();
	int iViewType = pGridView->GetViewType();
	if(iViewType == XHGRIDVIEW_DETAIL)
	{
		int iTotalHeight = 0;
		for(int i=0;i<iCount;i++)
			iTotalHeight += m_arrpSubItems[iIndexStart + i]->GetHeight();
		return iTotalHeight;
	}
	else if(iViewType == XHGRIDVIEW_REPORT)
		return  pGridView->GetRowHeight();

	ASSERT(FALSE);	//Other View Type
	return pGridView->GetRowHeight();
}


//////////////////////////////////////////////////////////////////////////
// CXHGridViewSubItem

CXHGridViewSubItem::CXHGridViewSubItem()
{
	m_LeafItems.SetParentSubItem(this);
	m_pParentSubItems = NULL;
//	VERIFY( m_LeafItems.InsertLeafItem(0,_T("")) );
}

CXHGridViewSubItem::~CXHGridViewSubItem()
{

}

CXHGridViewLeafItem* CXHGridViewSubItem::GetLeafItem(int iIndex)
{
	return m_LeafItems.GetLeafItem(iIndex);
}

void CXHGridViewSubItem::SetParentSubItems(CXHGridViewSubItems *pParent)
{
	m_pParentSubItems = pParent;
}

CXHGridViewSubItems* CXHGridViewSubItem::GetParentSubItems()
{
	return m_pParentSubItems;
}

CXHGridViewLeafItem* CXHGridViewSubItem::LeafHitTest(POINT point)
{
	return m_LeafItems.LeafHitTest(point);
}

CXHGridViewLeafItem* CXHGridViewSubItem::InsertLeafItem(int iLeafIndex,LPCTSTR lpszText,int iWidth)
{
	return m_LeafItems.InsertLeafItem(iLeafIndex,lpszText,iWidth);
}

int CXHGridViewSubItem::GetLeafCount()
{
	return m_LeafItems.GetCount();
}

void CXHGridViewSubItem::RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	m_LeafItems.RearrangeBottomLeafList(pLeafList);
}

void CXHGridViewSubItem::RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	m_LeafItems.RearrangeLeafList(pLeafList);
}

int CXHGridViewSubItem::GetHeight()
{
	CXHGridViewBaseItem *pBaseItem = GetParentSubItems()->GetParentBaseItem();
	int iRowHeight = pBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView()->GetRowHeight();
	if(pBaseItem->IsStretch())
		return pBaseItem->GetParentBaseItems()->GetMaxBaseItemRowCount() * iRowHeight / pBaseItem->GetSubItemCount();
	return iRowHeight;
}

void CXHGridViewSubItem::Draw(CDC *pDC)
{
	if(m_LeafItems.GetCount() > 0)
		m_LeafItems.Draw(pDC);
	else
	{
		CRect rcSubItem;
		GetSubItemRect(rcSubItem);
		OnDrawSubItem(pDC,rcSubItem);
	}
}

void CXHGridViewSubItem::GetSubItemRect(LPRECT lprcSubItem)
{
	SetRect(lprcSubItem,0,0,m_pParentSubItems->GetParentBaseItem()->GetWidth(),m_pParentSubItems->GetSubItemHeight(m_pParentSubItems->GetSubItemIndex(this),1));
}

CXHGridViewSubItem* CXHGridViewSubItem::CloneSubItem()
{
	CXHGridViewSubItem *pNewSubItem = new CXHGridViewSubItem();
	ASSERT(pNewSubItem);

	pNewSubItem->m_LeafItems.CopyFrom(&m_LeafItems);
	pNewSubItem->m_LeafItems.SetParentSubItem(pNewSubItem);

	return pNewSubItem;
}

CXHGridViewLeafItem* CXHGridViewSubItem::FindLeafByID(int iLeafID)
{
	return m_LeafItems.FindLeafByID(iLeafID);
}

void CXHGridViewSubItem::SetLeafItem(int iLeafIndex,CXHGridViewLeafItem *pNew)
{
	m_LeafItems.SetLeafItem(iLeafIndex,pNew);
}

void CXHGridViewSubItem::OnDrawSubItem(CDC *pDC,LPCRECT lprcSubItem)
{
	//[M] Do nothing or call GridView to perform drawing
}


//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafItems

CXHGridViewLeafItems::CXHGridViewLeafItems()
{
	_m_LeafItemsInfo.iTotalWidth = 0;
	m_pParentSubItem = NULL;
}

CXHGridViewLeafItems::~CXHGridViewLeafItems()
{
	DeleteAllLeafItems();
}

CXHGridViewLeafItem* CXHGridViewLeafItems::InsertLeafItem(int iIndex,LPCTSTR lpszText,int iWidth /* = DEFAULT_BAND_WIDTH */)
{
	CXHGridViewLeafItem *pNew = new CXHGridViewLeafItem();
	ASSERT(pNew);

	//下面两行代码顺序不能改变！
	pNew->SetWidth(iWidth);
	pNew->SetParentLeafItems(this);

	if(GetParentSubItem()->GetParentSubItems()->GetParentBaseItem()->GetParentBaseItems()->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_HEADER)
		pNew->SetID(pNew->GetParentStructure()->NewID());

	_m_LeafItemsInfo.iTotalWidth += iWidth;
	pNew->SetText(lpszText);

	m_arrpLeafItems.InsertAt(iIndex,pNew);

	return pNew;
}

void CXHGridViewLeafItems::CopyFrom(CXHGridViewLeafItems *pLeafItems)
{
	DeleteAllLeafItems();
	m_arrpLeafItems.SetSize(pLeafItems->GetCount());
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
	{
		m_arrpLeafItems[i] = pLeafItems->m_arrpLeafItems[i]->CloneLeafItem();
		ASSERT(m_arrpLeafItems[i]);
		m_arrpLeafItems[i]->SetParentLeafItems(this);
	}
	memcpy(&_m_LeafItemsInfo,&pLeafItems->_m_LeafItemsInfo,sizeof(_m_LeafItemsInfo));
}

void CXHGridViewLeafItems::DeleteAllLeafItems()
{
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		if(m_arrpLeafItems[i])
			delete m_arrpLeafItems[i];
	m_arrpLeafItems.RemoveAll();
}

void CXHGridViewLeafItems::AddTotalWidth(int iAdd)
{
	_m_LeafItemsInfo.iTotalWidth += iAdd;
}

CXHGridViewLeafItem* CXHGridViewLeafItems::GetLeafItem(int iIndex)
{
	return m_arrpLeafItems[iIndex];
}

int CXHGridViewLeafItems::GetCount() const
{
	return m_arrpLeafItems.GetSize();
}

CXHGridViewLeafItem* CXHGridViewLeafItems::LeafHitTest(POINT point)
{
	int iCurXPos = 0;
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
	{
		//TRACE3(_T("pt:%d  IW:%d  TW:%d\n"),point.x,GetLeafFactWidth(i),_m_LeafItemsInfo.iTotalWidth);
		if(point.x > iCurXPos && point.x < iCurXPos + GetLeafFactWidth(i))
			return m_arrpLeafItems[i];
		iCurXPos += GetLeafFactWidth(i);
	}

	return NULL;
}

int CXHGridViewLeafItems::GetLeafFactWidth(int iLeafIndex)
{
	// Report Mode
	if(m_arrpLeafItems[iLeafIndex]->GetParentStructure()->GetParentGridView()->GetViewType() == XHGRIDVIEW_REPORT)
		return m_arrpLeafItems[iLeafIndex]->GetWidth();

	// Detail Mode
	if(m_arrpLeafItems[iLeafIndex]->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_ITEM)
	{
		CXHGridViewItemStructure *pHeaderItem = m_arrpLeafItems[iLeafIndex]->GetParentStructure()->GetParentGridView()->GetHeaderItemStructure();
		CXHGridViewLeafItem *pHeaderLeaf = pHeaderItem->GetLeafByID(m_arrpLeafItems[iLeafIndex]->GetID());
		return pHeaderLeaf->GetFactWidth();
	}

	int iParentBaseWidth = m_pParentSubItem->GetParentSubItems()->GetParentBaseItem()->GetWidth();
	double fRate = (double)_m_LeafItemsInfo.iTotalWidth / iParentBaseWidth;

	if(iLeafIndex == m_arrpLeafItems.GetSize() - 1)
	{
		return iParentBaseWidth - (int)(GetLeafItemWidth(0,iLeafIndex) / fRate);
	}

	int iResult = (int)((m_arrpLeafItems[iLeafIndex]->GetWidth() / fRate));
	return iResult;
}

int CXHGridViewLeafItems::GetLeafItemFactWidth(int iLeafIndexStart,int nCount)
{
	int iTotalWidth = 0;
	for(int i=iLeafIndexStart;i<iLeafIndexStart+nCount;i++)
		iTotalWidth += GetLeafFactWidth(i);

	return iTotalWidth;
}

int CXHGridViewLeafItems::GetLeafFixedWidth(int iLeafIndex)
{
	//Always calculate in Detail Mode
	if(m_arrpLeafItems[iLeafIndex]->GetParentStructure()->GetItemType() == XHGRIDVIEW_ITEMTYPE_ITEM)
	{
		CXHGridViewItemStructure *pHeaderItem = m_arrpLeafItems[iLeafIndex]->GetParentStructure()->GetParentGridView()->GetHeaderItemStructure();
		CXHGridViewLeafItem *pHeaderLeaf = pHeaderItem->GetLeafByID(m_arrpLeafItems[iLeafIndex]->GetID());
		return pHeaderLeaf->GetFactWidth();
	}

	int iParentBaseWidth = m_pParentSubItem->GetParentSubItems()->GetParentBaseItem()->GetWidth();
	double fRate = (double)_m_LeafItemsInfo.iTotalWidth / iParentBaseWidth;

	if(iLeafIndex == m_arrpLeafItems.GetSize() - 1)
	{
		return iParentBaseWidth - (int)(GetLeafItemWidth(0,iLeafIndex) / fRate);
	}

	int iResult = (int)((m_arrpLeafItems[iLeafIndex]->GetWidth() / fRate));
	return iResult;
}

void CXHGridViewLeafItems::SetParentSubItem(CXHGridViewSubItem *pParent)
{
	m_pParentSubItem = pParent;
}

CXHGridViewSubItem* CXHGridViewLeafItems::GetParentSubItem()
{
	return m_pParentSubItem;
}

void CXHGridViewLeafItems::RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	pLeafList->Append(m_arrpLeafItems);
}

void CXHGridViewLeafItems::RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList)
{
	pLeafList->Append(m_arrpLeafItems);
}

int CXHGridViewLeafItems::GetLeafItemIndex(const CXHGridViewLeafItem *pLeafItem) const
{
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		if(m_arrpLeafItems[i] == pLeafItem)
			return i;

	return -1;
}

void CXHGridViewLeafItems::Draw(CDC *pDC)
{
	CPoint ptOld;
	int iCurXPos = 0;
	CXHGridView *pGridView = m_pParentSubItem->GetParentSubItems()->GetParentBaseItem()->GetParentBaseItems()->GetParentStructure()->GetParentGridView();
	CXHGridViewLeafDrawHelper *pDrawer = m_pParentSubItem->GetParentSubItems()->GetParentBaseItem()->GetParentBaseItems()->GetParentStructure()->GetDrawer();
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
	{
		ptOld = pDC->GetViewportOrg();
		pDC->SetViewportOrg(CPoint(ptOld.x + iCurXPos, ptOld.y));

		//(5L) Draw lines
		if(i != m_arrpLeafItems.GetSize() - 1)
		{
			pDrawer->DrawLine(pGridView,pDC,m_arrpLeafItems[i]->GetFactWidth(),0,m_arrpLeafItems[i]->GetFactWidth(),m_pParentSubItem->GetHeight());
//			pDC->MoveTo(m_arrpLeafItems[i]->GetFactWidth(),0);
//			pDC->LineTo(m_arrpLeafItems[i]->GetFactWidth(),m_pParentSubItem->GetHeight());
		}

		m_arrpLeafItems[i]->Draw(pDC);
		pDC->SetViewportOrg(ptOld);
		iCurXPos += m_arrpLeafItems[i]->GetFactWidth();
	}
}

int CXHGridViewLeafItems::GetLeafItemWidth(int iIndexStart,int iCount)
{
	int iTotalWidth = 0;
	if(iCount == -1)
		iCount = m_arrpLeafItems.GetSize() - iIndexStart;
	for(int i=0;i<iCount;i++)
		iTotalWidth += m_arrpLeafItems[iIndexStart + i]->GetWidth();
	return iTotalWidth;
}

CXHGridViewLeafItem* CXHGridViewLeafItems::FindLeafByID(int iLeafID)
{
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		if(m_arrpLeafItems[i]->GetID() == iLeafID)
			return m_arrpLeafItems[i];
	return NULL;
}

void CXHGridViewLeafItems::SetLeafItem(int iLeafIndex, CXHGridViewLeafItem *pNew)
{
	CXHGridViewLeafItem *pCurrent = m_arrpLeafItems[iLeafIndex];

	pNew->SetParentLeafItems(this);
	pNew->CopyLeafInfo(pCurrent);

	pCurrent->Release();

	m_arrpLeafItems[iLeafIndex] = pNew;
}

void CXHGridViewLeafItems::RefactAllLeafItemWidth()
{
	CArray <int,int> arrWidth;
	arrWidth.SetSize(GetCount());
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		arrWidth[i] = m_arrpLeafItems[i]->GetFactWidth();
	for(int i=0;i<m_arrpLeafItems.GetSize();i++)
		m_arrpLeafItems[i]->SetWidth(arrWidth[i]);

}


//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafItem

IMPLEMENT_DYNAMIC(CXHGridViewLeafItem,CObject)

CXHGridViewLeafItem::CXHGridViewLeafItem()
{
	Initialize();
}
CXHGridViewLeafItem::CXHGridViewLeafItem(LPCTSTR lpszText)
{
	Initialize();
	m_strText = lpszText;
}

CXHGridViewLeafItem::~CXHGridViewLeafItem()
{
}

void CXHGridViewLeafItem::Initialize()
{
	ZeroMemory(&m_LeafItemInfo, sizeof(m_LeafItemInfo));
	m_nRef = 1;
	m_LeafItemInfo.clrBackground = ::GetSysColor(COLOR_WINDOW);
	m_LeafItemInfo.clrForeground = ::GetSysColor(COLOR_WINDOWTEXT);
	m_LeafItemInfo.dwLeafItemData = 0;
	m_LeafItemInfo.dwFlags = 0;
	m_LeafItemInfo.dwState = 0;
	m_LeafItemInfo.iImage = -1;
	m_LeafItemInfo.iSortImage = -1;
	m_LeafItemInfo.iID = -1;
	m_LeafItemInfo.iWidth = XHGRIDVIEW_LEAF_WIDTH;
	m_pParentLeafItems = NULL;
}

void CXHGridViewLeafItem::SetWidth(int iNewWidth)
{
	int iDelta = iNewWidth - m_LeafItemInfo.iWidth;
	m_LeafItemInfo.iWidth = iNewWidth;
	if(GetParentLeafItems())
		GetParentLeafItems()->AddTotalWidth(iDelta);
}

int CXHGridViewLeafItem::GetWidth() const
{
	return m_LeafItemInfo.iWidth;
}

void CXHGridViewLeafItem::SetText(LPCTSTR lpszText)
{
	m_strText = lpszText;
}

CString CXHGridViewLeafItem::GetText() const
{
	return m_strText;
}
void CXHGridViewLeafItem::Draw(CDC *pDC)
{
	GetParentStructure()->GetDrawer()->Draw(pDC,this);
}
void CXHGridViewLeafItem::DrawContent(CDC *pDC)
{

	DrawLeftImage(pDC);

	// Prepare Font
	CFont *pFont = (CFont*)pDC->GetCurrentFont();
	LOGFONT lf;
	pFont->GetLogFont(&lf);

	lf.lfWeight = m_LeafItemInfo.lfWeight;
	lf.lfItalic = m_LeafItemInfo.lfItalic;
	lf.lfUnderline = m_LeafItemInfo.lfUnderline;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);
	CFont *pOldFont = pDC->SelectObject(&fontDraw);

	//Draw Text
	CRect rcArea;
	GetLeafClientRect(rcArea);
	int iOldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF clrOldBkColor = pDC->SetBkColor(m_LeafItemInfo.clrBackground);
	COLORREF clrOldTextColor = pDC->SetTextColor(m_LeafItemInfo.clrForeground);


	pDC->DrawText(m_strText,rcArea,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_END_ELLIPSIS);

	pDC->SelectObject(pOldFont);

	pDC->SetBkMode(iOldBkMode);
	pDC->SetTextColor(clrOldTextColor);
	pDC->SetBkColor(clrOldBkColor);

	DrawSortImage(pDC);
}

void CXHGridViewLeafItem::DrawLeftImage(CDC *pDC)
{
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	//Draw image
	if(pImageList)
	{
		if(m_LeafItemInfo.iImage >= 0)
		{
			CRect rcImage;
			GetLeafImageRect(rcImage);
			pImageList->Draw(pDC,m_LeafItemInfo.iImage,CPoint(rcImage.left,rcImage.top),ILD_TRANSPARENT);
		}
	}
}

void CXHGridViewLeafItem::DrawSortImage(CDC *pDC)
{
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	//Draw sort image
	if(pImageList)
	{
		if(m_LeafItemInfo.iSortImage >= 0)
		{
			CRect rcSortImage;
			GetLeafSortImageRect(rcSortImage);
			pImageList->Draw(pDC,m_LeafItemInfo.iSortImage,CPoint(rcSortImage.left,rcSortImage.top),ILD_TRANSPARENT);
		}
	}
}

CXHGridViewItemStructure* CXHGridViewLeafItem::GetParentStructure()
{
	return m_pParentLeafItems->GetParentSubItem()->GetParentSubItems()->GetParentBaseItem()->GetParentBaseItems()->GetParentStructure();
}

void CXHGridViewLeafItem::SetParentLeafItems(CXHGridViewLeafItems *pParent)
{
	m_pParentLeafItems = pParent;
}

CXHGridViewLeafItems* CXHGridViewLeafItem::GetParentLeafItems()
{
	return m_pParentLeafItems;
}

int CXHGridViewLeafItem::GetFactWidth()
{
	if(GetParentStructure()->GetItemType() != XHGRIDVIEW_ITEMTYPE_ITEM)
		return m_pParentLeafItems->GetLeafFactWidth(m_pParentLeafItems->GetLeafItemIndex(this));
	else	//XHGRIDVIEW_ITEMTYPE_ITEM
		return GetParentStructure()->GetParentGridView()->GetHeaderCtrl()->GetHeaderItemStructure()->GetLeafByID(m_LeafItemInfo.iID)->GetFactWidth();
}

int CXHGridViewLeafItem::GetFixedWidth()
{
	if(GetParentStructure()->GetItemType() != XHGRIDVIEW_ITEMTYPE_ITEM)
		return m_pParentLeafItems->GetLeafFixedWidth(m_pParentLeafItems->GetLeafItemIndex(this));
	else	//XHGRIDVIEW_ITEMTYPE_ITEM
		return GetParentStructure()->GetParentGridView()->GetHeaderCtrl()->GetHeaderItemStructure()->GetLeafByID(m_LeafItemInfo.iID)->GetFixedWidth();
}

int CXHGridViewLeafItem::GetFixedHeight()
{
	CXHGridViewBaseItem *pBaseItem = GetParentLeafItems()->GetParentSubItem()->GetParentSubItems()->GetParentBaseItem();
	CXHGridView *pGridView = pBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView();

	if(pBaseItem->IsStretch())
		return pBaseItem->GetParentBaseItems()->GetMaxBaseItemRowCount() * pGridView->GetRowHeight() / GetParentLeafItems()->GetParentSubItem()->GetParentSubItems()->GetCount();
	else
		return pGridView->GetRowHeight();
}

int CXHGridViewLeafItem::GetHeight()
{
	CXHGridViewBaseItem *pBaseItem = GetParentLeafItems()->GetParentSubItem()->GetParentSubItems()->GetParentBaseItem();
	CXHGridView *pGridView = pBaseItem->GetParentBaseItems()->GetParentStructure()->GetParentGridView();

	if(GetParentStructure()->GetParentGridView()->GetViewType() == XHGRIDVIEW_DETAIL
		&& pBaseItem->IsStretch())
		return pBaseItem->GetParentBaseItems()->GetMaxBaseItemRowCount() * pGridView->GetRowHeight() / GetParentLeafItems()->GetParentSubItem()->GetParentSubItems()->GetCount();
	else
		return pGridView->GetRowHeight();
}

void CXHGridViewLeafItem::GetLeafRect(LPRECT lprcRect)
{
	SetRect(lprcRect,0,0,GetFactWidth(),GetHeight());
}

void CXHGridViewLeafItem::GetLeafDrawingRect(LPRECT lprcRect)
{
	CXHGridView *pGridView = GetParentStructure()->GetParentGridView();
	int iLineWidth = pGridView->GetLineWidth();
	SetRect(lprcRect,0,0,GetFactWidth() - iLineWidth,GetHeight() - iLineWidth);
	if(m_pParentLeafItems->GetLeafItemIndex(this) != 0)
		lprcRect->left = iLineWidth;
	if(GetParentStructure()->GetParentGridView()->GetViewType() == XHGRIDVIEW_DETAIL)
		lprcRect->top = iLineWidth;
}

void CXHGridViewLeafItem::SetID(int iID)
{
	m_LeafItemInfo.iID = iID;
}

int CXHGridViewLeafItem::GetID()
{
	return m_LeafItemInfo.iID;
}

void CXHGridViewLeafItem::SetFocused(BOOL bFocused)
{
	if(bFocused)
	{
		CXHGridViewLeafItem *pLastFocus = GetParentStructure()->GetLastFocusedLeafItem();
		if(pLastFocus)
			pLastFocus->SetFocused(FALSE);
		m_LeafItemInfo.dwState |= XHGRIDVIEW_LEAF_STATE_FOCUS;
		GetParentStructure()->SetLastFocusedLeafItem(this);
	}
	else
	{
		m_LeafItemInfo.dwState &= (~XHGRIDVIEW_LEAF_STATE_FOCUS);
		GetParentStructure()->SetLastFocusedLeafItem(NULL);
	}
}

BOOL CXHGridViewLeafItem::IsFocused()
{
	return ((m_LeafItemInfo.dwState & XHGRIDVIEW_LEAF_STATE_FOCUS) == XHGRIDVIEW_LEAF_STATE_FOCUS);
}


void CXHGridViewLeafItem::GetLeafClientRect(LPRECT lprcRect)
{
	int iLeafBorder = GetParentStructure()->GetParentGridView()->GetLeafBorderWidth();
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	int iLeftPatch = 0, iRightPatch = 0;
	if(pImageList)
	{
		if(m_LeafItemInfo.iImage >= 0)	//Has image on the left
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(m_LeafItemInfo.iImage,&ii);
			iLeftPatch = ii.rcImage.right - ii.rcImage.left + iLeafBorder;
		}
		if(m_LeafItemInfo.iSortImage >= 0)	//Has sort image on the right
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(m_LeafItemInfo.iSortImage,&ii);
			iRightPatch = ii.rcImage.right - ii.rcImage.left + iLeafBorder;
		}
	}
	SetRect(lprcRect,iLeafBorder + iLeftPatch ,iLeafBorder,GetFactWidth() - iLeafBorder - iLeafBorder - iRightPatch,GetHeight() - iLeafBorder);
}

void CXHGridViewLeafItem::GetLeafClientFixedRect(LPRECT lprcRect)
{
	int iLeafBorder = GetParentStructure()->GetParentGridView()->GetLeafBorderWidth();
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	int iLeftPatch = 0, iRightPatch = 0;
	if(pImageList)
	{
		if(m_LeafItemInfo.iImage >= 0)	//Has image on the left
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(m_LeafItemInfo.iImage,&ii);
			iLeftPatch = ii.rcImage.right - ii.rcImage.left + iLeafBorder;
		}
		if(m_LeafItemInfo.iSortImage >= 0)	//Has sort image on the right
		{
			IMAGEINFO ii;
			pImageList->GetImageInfo(m_LeafItemInfo.iSortImage,&ii);
			iRightPatch = ii.rcImage.right - ii.rcImage.left + iLeafBorder;
		}
	}
	SetRect(lprcRect,iLeafBorder + iLeftPatch ,iLeafBorder,GetFixedWidth() - iLeafBorder - iLeafBorder - iRightPatch,GetFixedHeight() - iLeafBorder);
}

BOOL CXHGridViewLeafItem::GetLeafImageRect(LPRECT lprcRect)
{
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	if(!pImageList)
		return FALSE;
	if(m_LeafItemInfo.iImage < 0)
		return FALSE;

	int iLeafBorder = GetParentStructure()->GetParentGridView()->GetLeafBorderWidth();
	
	IMAGEINFO ii;
	pImageList->GetImageInfo(m_LeafItemInfo.iImage,&ii);
	int iImgWidth = ii.rcImage.right - ii.rcImage.left;
	int iImgHeight = ii.rcImage.bottom - ii.rcImage.top;

	int iYPatch = (GetHeight() - iLeafBorder * 2 - iImgHeight) / 2;

	SetRect(lprcRect,iLeafBorder,iLeafBorder,iLeafBorder + iImgWidth,iLeafBorder + iImgHeight);
	OffsetRect(lprcRect,0,iYPatch);
	return TRUE;
}

BOOL CXHGridViewLeafItem::GetLeafSortImageRect(LPRECT lprcRect)
{
	CImageList *pImageList = GetParentStructure()->GetParentGridView()->GetImageList();
	if(!pImageList)
		return FALSE;
	if(m_LeafItemInfo.iSortImage < 0)
		return FALSE;

	int iLeafBorder = GetParentStructure()->GetParentGridView()->GetLeafBorderWidth();
	
	IMAGEINFO ii;
	pImageList->GetImageInfo(m_LeafItemInfo.iSortImage,&ii);
	int iImgWidth = ii.rcImage.right - ii.rcImage.left;
	int iImgHeight = ii.rcImage.bottom - ii.rcImage.top;

	int iLeafWidth = GetFactWidth();
	int iYPatch = (GetHeight() - iLeafBorder * 2 - iImgHeight) / 2;

	SetRect(lprcRect,iLeafWidth - iLeafBorder - iImgWidth ,iLeafBorder,iLeafWidth - iLeafBorder,iLeafBorder + iImgHeight);
	OffsetRect(lprcRect,0,iYPatch);
	return TRUE;
}

BOOL CXHGridViewLeafItem::MouseEnter()
{
	BOOL bSet = ((m_LeafItemInfo.dwState & XHGRIDVIEW_LEAF_STATE_MOUSEOVER) == 0);
	m_LeafItemInfo.dwState |= XHGRIDVIEW_LEAF_STATE_MOUSEOVER;

	if(bSet)
		OnMouseEnter();

//	TRACE2(_T("(E) Mouse Enter - %d  (%s)\r\n"),bSet!=FALSE,m_strText);
	return bSet;
}

BOOL CXHGridViewLeafItem::MouseLeave()
{
	BOOL bSet = ((m_LeafItemInfo.dwState & XHGRIDVIEW_LEAF_STATE_MOUSEOVER));
	m_LeafItemInfo.dwState &= (~XHGRIDVIEW_LEAF_STATE_MOUSEOVER);

	if(bSet)
		OnMouseLeave();
	
//	TRACE2(_T("(L) Mouse Leave - %d  (%s)\r\n"),bSet!=FALSE,m_strText);
	return bSet;
}

DWORD CXHGridViewLeafItem::GetState() const
{
	return m_LeafItemInfo.dwState;
}

void CXHGridViewLeafItem::SetLeafItemData(DWORD dwData)
{
	m_LeafItemInfo.dwLeafItemData = dwData;
}

DWORD CXHGridViewLeafItem::GetLeafItemData() const
{
	return m_LeafItemInfo.dwLeafItemData;
}

void CXHGridViewLeafItem::ModifyState(DWORD dwRemove,DWORD dwAdd)
{
	m_LeafItemInfo.dwState &= (~dwRemove);
	m_LeafItemInfo.dwState |= dwAdd;
}

COLORREF CXHGridViewLeafItem::GetTextColor() const
{
	return m_LeafItemInfo.clrForeground;
}

void CXHGridViewLeafItem::SetTextColor(COLORREF clrText)
{
	m_LeafItemInfo.clrForeground = clrText;
}

COLORREF CXHGridViewLeafItem::GetBackColor() const
{
	return m_LeafItemInfo.clrBackground;
}

void CXHGridViewLeafItem::SetBackColor(COLORREF clrBackground)
{
	m_LeafItemInfo.clrBackground = clrBackground;
}

void CXHGridViewLeafItem::SetTextBold(BOOL bBold)
{
	m_LeafItemInfo.lfWeight = (bBold ? FW_BOLD : 0);
}

void CXHGridViewLeafItem::SetTextItalic(BOOL bItalic)
{
	m_LeafItemInfo.lfItalic = bItalic;
}

void CXHGridViewLeafItem::SetTextUnderline(BOOL bUnderline)
{
	m_LeafItemInfo.lfUnderline = bUnderline;
}

void CXHGridViewLeafItem::SetImageIndex(int iImageIndex)
{
	m_LeafItemInfo.iImage = iImageIndex;
}

void CXHGridViewLeafItem::SetSortImageIndex(int iImageIndex)
{
	m_LeafItemInfo.iSortImage = iImageIndex;
}

CXHGridViewLeafItem* CXHGridViewLeafItem::CloneLeafItem()
{
	CXHGridViewLeafItem *pNewLeafItem = new CXHGridViewLeafItem();
	ASSERT(pNewLeafItem);

	memcpy(&pNewLeafItem->m_LeafItemInfo,&m_LeafItemInfo,sizeof(m_LeafItemInfo));
	pNewLeafItem->m_LeafItemInfo.iImage = -1;
	pNewLeafItem->m_LeafItemInfo.iSortImage = -1;
	pNewLeafItem->m_strText = m_strText;

	return pNewLeafItem;
}

void CXHGridViewLeafItem::CopyLeafInfo(CXHGridViewLeafItem *pSrcLeaf)
{
	memcpy(&m_LeafItemInfo,&pSrcLeaf->m_LeafItemInfo,sizeof(m_LeafItemInfo));
}

LONG CXHGridViewLeafItem::Release()
{
	LONG nRef = InterlockedDecrement(&m_nRef);
	if(nRef == 0)
		delete this;

	return nRef;
}

LONG CXHGridViewLeafItem::AddRef()
{
	return InterlockedIncrement(&m_nRef);
}

void CXHGridViewLeafItem::TriggerClickEvent()
{
	OnClick();
}

void CXHGridViewLeafItem::OnClick()
{
	//Do something in derived classes
}

void CXHGridViewLeafItem::OnMouseEnter()
{
	//Do something in derived classes
}

void CXHGridViewLeafItem::OnMouseLeave()
{
	//Do something in derived classes
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafDrawHelper

CXHGridViewLeafDrawHelper::CXHGridViewLeafDrawHelper()
{
}

CXHGridViewLeafDrawHelper::~CXHGridViewLeafDrawHelper()
{
}

void CXHGridViewLeafDrawHelper::Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem)
{
	//Don't use base drawer directly
	ASSERT(FALSE);
}

void CXHGridViewLeafDrawHelper::DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure)
{
	//Don't use base drawer directly
	ASSERT(FALSE);	
}

void CXHGridViewLeafDrawHelper::DrawLine(CXHGridView *pGridView,CDC *pDC,int x1,int y1,int x2,int y2)
{
	if(pGridView->IsGridLines())
	{
		CPen pen(PS_SOLID,pGridView->GetLineWidth(),pGridView->GetLineColor());
		CPen *pOldPen;
		pOldPen = pDC->SelectObject(&pen);
		
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
		
		pDC->SelectObject(pOldPen);
	}
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafDrawHelperBand

CXHGridViewLeafDrawHelperBand::CXHGridViewLeafDrawHelperBand()
{
}

CXHGridViewLeafDrawHelperBand::~CXHGridViewLeafDrawHelperBand()
{
}

void CXHGridViewLeafDrawHelperBand::Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem)
{
	CRect rcLeaf;
	pLeafItem->GetLeafRect(rcLeaf);
	CRect rcClient;
	pLeafItem->GetLeafClientRect(rcClient);

	pDC->DrawFrameControl(rcLeaf,DFC_BUTTON,DFCS_BUTTONPUSH|(pLeafItem->GetState() & XHGRIDVIEW_LEAF_STATE_PUSHED));
	if((pLeafItem->GetState() & XHGRIDVIEW_LEAF_STATE_PUSHED) == XHGRIDVIEW_LEAF_STATE_PUSHED)
		rcClient.OffsetRect(1,1);

	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	CFont *pOldFont = pDC->SelectObject(&font);
	pLeafItem->DrawContent(pDC);
	pDC->SelectObject(pOldFont);
}

void CXHGridViewLeafDrawHelperBand::DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure)
{
	CRect rcMarker(0,0, pItemStructure->GetParentGridView()->GetMarkWidth(),pItemStructure->GetHeight());
	pDC->DrawFrameControl(rcMarker,DFC_BUTTON,DFCS_BUTTONPUSH);
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafDrawHelperHeader

CXHGridViewLeafDrawHelperHeader::CXHGridViewLeafDrawHelperHeader()
{
}

CXHGridViewLeafDrawHelperHeader::~CXHGridViewLeafDrawHelperHeader()
{
}

void CXHGridViewLeafDrawHelperHeader::Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem)
{
	CRect rcLeaf;
	pLeafItem->GetLeafRect(rcLeaf);
	CRect rcClient;
	pLeafItem->GetLeafClientRect(rcClient);

	pDC->DrawFrameControl(rcLeaf,DFC_BUTTON,DFCS_BUTTONPUSH|(pLeafItem->GetState() & XHGRIDVIEW_LEAF_STATE_PUSHED));
	if((pLeafItem->GetState() & XHGRIDVIEW_LEAF_STATE_PUSHED) == XHGRIDVIEW_LEAF_STATE_PUSHED)
		rcClient.OffsetRect(1,1);

	COLORREF clrOldTextColor = pLeafItem->GetTextColor();
	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);
	LOGFONT logFont;
	font.GetLogFont(&logFont);
	if(pLeafItem->IsFocused())
	{
//		pLeafItem->SetTextColor(RGB(255,0,0));
		logFont.lfWeight = FW_BOLD;
	}
	font.DeleteObject();
	font.CreateFontIndirect(&logFont);
	CFont *pOldFont = pDC->SelectObject(&font);

	pLeafItem->DrawContent(pDC);

	pDC->SelectObject(pOldFont);
	pLeafItem->SetTextColor(clrOldTextColor);
}

void CXHGridViewLeafDrawHelperHeader::DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure)
{
	CRect rcMarker(0,0, pItemStructure->GetParentGridView()->GetMarkWidth(),pItemStructure->GetHeight());
	pDC->DrawFrameControl(rcMarker,DFC_BUTTON,DFCS_BUTTONPUSH);
}
void CXHGridViewLeafDrawHelperHeader::DrawLine(CXHGridView *pGridView,CDC *pDC,int x1,int y1,int x2,int y2)
{
	//Header Control - No need to draw lines
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafDrawHelperItem

CXHGridViewLeafDrawHelperItem::CXHGridViewLeafDrawHelperItem()
{
}

CXHGridViewLeafDrawHelperItem::~CXHGridViewLeafDrawHelperItem()
{
}

void CXHGridViewLeafDrawHelperItem::Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem)
{
	CRect rcLeaf;
	pLeafItem->GetLeafDrawingRect(rcLeaf);
	CXHColor clrBack = pLeafItem->GetBackColor();
	if(pLeafItem->GetParentStructure()->IsSelected())
		clrBack.MixedWith(::GetSysColor(COLOR_HIGHLIGHT));
	CBrush brush(clrBack);
	pDC->FillRect(rcLeaf,&brush);

	CFont font;
	font.CreateStockObject(DEFAULT_GUI_FONT);

	CFont *pOldFont = pDC->SelectObject(&font);
	pLeafItem->DrawContent(pDC);
	pDC->SelectObject(pOldFont);
}

void CXHGridViewLeafDrawHelperItem::DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure)
{
	CRect rcMarker(0,0, pItemStructure->GetParentGridView()->GetMarkWidth(),pItemStructure->GetHeight());
	if(pItemStructure->GetState() & XHGRIDVIEW_ITEM_STATE_FOCUSED)
		pDC->DrawFrameControl(rcMarker,DFC_SCROLL,DFCS_SCROLLRIGHT);
	else
		pDC->DrawFrameControl(rcMarker,DFC_BUTTON,DFCS_BUTTONPUSH);
}

//////////////////////////////////////////////////////////////////////////
// CXHGridViewLeafItemImage

IMPLEMENT_DYNAMIC(CXHGridViewLeafItemImage,CXHGridViewLeafItem)

CXHGridViewLeafItemImage::CXHGridViewLeafItemImage()
{
	Initialize();
}

CXHGridViewLeafItemImage::CXHGridViewLeafItemImage(LPCTSTR lpszImageFilePath, BOOL bDelayLoad)
{
	Initialize();
	LoadImageFile(lpszImageFilePath, bDelayLoad);
}

CXHGridViewLeafItemImage::CXHGridViewLeafItemImage(HBITMAP hBitmap)
{
	ASSERT(hBitmap);
	Initialize();
	CBitmap *pNewBmp = new CBitmap();
	ASSERT(pNewBmp);
	pNewBmp->Attach(hBitmap);
	m_ImageLeafInfo.pImageObject = pNewBmp;
	m_ImageLeafInfo.iImageType = XHGRIDVIEW_IMAGELEAF_BITMAP;
	m_ImageLeafInfo.dwStatus = XHGRIDVIEW_IMAGELEAF_STATUS_LOADED;
}

void CXHGridViewLeafItemImage::Initialize()
{
	m_ImageLeafInfo.iImageType = XHGRIDVIEW_IMAGELEAF_NULL;
	m_ImageLeafInfo.pImageObject = NULL;
	m_ImageLeafInfo.sizeImage.cx = m_ImageLeafInfo.sizeImage.cy = 0;
	m_ImageLeafInfo.dwFlags = 0;
	m_ImageLeafInfo.dwStatus = 0;
}

CXHGridViewLeafItemImage::~CXHGridViewLeafItemImage()
{
	DeleteImageObject();
}

void CXHGridViewLeafItemImage::DrawContent(CDC *pDC)
{
	if(m_ImageLeafInfo.iImageType == XHGRIDVIEW_IMAGELEAF_BITMAP)
		DrawBitmapContent(pDC);
	else
		ASSERT(FALSE);
}

void CXHGridViewLeafItemImage::DrawBitmapContent(CDC *pDC)
{
	if((m_ImageLeafInfo.dwStatus & XHGRIDVIEW_IMAGELEAF_STATUS_LOADED) == 0)
		LoadImageFile(m_strImageFilePath);

	CXHRect rcClient;
	GetLeafClientRect(rcClient);
	CXHRect rcBitmap,rcBitmapOrigin;
	GetImageRect(rcBitmapOrigin);
	rcBitmap = rcBitmapOrigin;
	rcBitmap.FitRect(rcClient);

	CDC bmpDC;
	bmpDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = bmpDC.SelectObject((CBitmap*)m_ImageLeafInfo.pImageObject);

	pDC->SetStretchBltMode(HALFTONE);
	pDC->StretchBlt(rcBitmap.left,rcBitmap.top,rcBitmap.Width(),rcBitmap.Height(),&bmpDC,0,0,rcBitmapOrigin.Width(),rcBitmapOrigin.Height(),SRCCOPY);

	bmpDC.SelectObject(pOldBitmap);
}

void CXHGridViewLeafItemImage::GetImageRect(LPRECT lprcImage)
{
	if(m_ImageLeafInfo.iImageType == XHGRIDVIEW_IMAGELEAF_BITMAP)
		GetBitmapRect(lprcImage);
	else
		ASSERT(FALSE);
}

void CXHGridViewLeafItemImage::GetBitmapRect(LPRECT lprcBitmap)
{
	CBitmap *pBitmap = (CBitmap*)m_ImageLeafInfo.pImageObject;
	ASSERT(pBitmap);
	if(pBitmap->GetSafeHandle())
	{
		BITMAP bmpInfo;
		pBitmap->GetBitmap(&bmpInfo);
		SetRect(lprcBitmap,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight);
	}
}

BOOL CXHGridViewLeafItemImage::LoadBitmapFile(LPCTSTR lpszBmpFilePath)
{
	DeleteImageObject();

	CSize sizeDesire(0,0);

	if(m_ImageLeafInfo.dwFlags & XHGRIDVIEW_IMAGELEAF_FLAG_DELAY_LOAD)	//must be in Delay-Load mode 
	{
		BITMAPINFO bmpInfo;
		if(!GetBitmapFileInfo(lpszBmpFilePath,&bmpInfo))
			return FALSE;
		
		CRect rcLeaf;
		GetLeafClientFixedRect(rcLeaf);
		sizeDesire.cx = min(rcLeaf.Width() * 2,bmpInfo.bmiHeader.biWidth);
		sizeDesire.cy = min(rcLeaf.Height() * 2,bmpInfo.bmiHeader.biHeight);
		if((double)sizeDesire.cx / sizeDesire.cy > (double)bmpInfo.bmiHeader.biWidth / bmpInfo.bmiHeader.biHeight)
		{
			sizeDesire.cx = sizeDesire.cy * bmpInfo.bmiHeader.biWidth / bmpInfo.bmiHeader.biHeight;
		}
		else
		{
			sizeDesire.cy = sizeDesire.cx * bmpInfo.bmiHeader.biHeight / bmpInfo.bmiHeader.biWidth;
		}
	}
	
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL,lpszBmpFilePath,IMAGE_BITMAP,sizeDesire.cx,sizeDesire.cy,LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_VGACOLOR);
	ASSERT(m_ImageLeafInfo.pImageObject == NULL);
	if(hBitmap)
	{
		CBitmap *pNewBmp = new CBitmap();
		pNewBmp->Attach(hBitmap);
		m_ImageLeafInfo.pImageObject = pNewBmp;
		m_ImageLeafInfo.iImageType = XHGRIDVIEW_IMAGELEAF_BITMAP;
	}
	else
		return FALSE;

	return TRUE;
}

void CXHGridViewLeafItemImage::DeleteImageObject()
{
	if(m_ImageLeafInfo.pImageObject == NULL)	//No object to delete
		return;

	ASSERT(m_ImageLeafInfo.iImageType != XHGRIDVIEW_IMAGELEAF_NULL);

	if(m_ImageLeafInfo.iImageType == XHGRIDVIEW_IMAGELEAF_BITMAP)
		delete ((CBitmap*)m_ImageLeafInfo.pImageObject);
	else
		ASSERT(FALSE);


	m_ImageLeafInfo.pImageObject = NULL;
	m_ImageLeafInfo.iImageType = XHGRIDVIEW_IMAGELEAF_NULL;
}

int CXHGridViewLeafItemImage::RecognizeImageFileType(LPCTSTR lpszImageFileName)
{
	if(IsBitmapFile(lpszImageFileName))
		return XHGRIDVIEW_IMAGELEAF_BITMAP;

	return XHGRIDVIEW_IMAGELEAF_NULL;
}

BOOL CXHGridViewLeafItemImage::GetBitmapFileInfo(LPCTSTR lpszBmpFilePath,LPBITMAPINFO pBmpInfo)
{
	CFile bmpfile;
	if(!bmpfile.Open(lpszBmpFilePath,CFile::modeRead))
		return FALSE;
	
	BITMAPFILEHEADER bmFile;
	bmpfile.Read(&bmFile,sizeof(bmFile));

	BITMAPINFOHEADER bmpInfo;
	UINT uRead = bmpfile.Read(&bmpInfo,sizeof(bmpInfo));

	memcpy(&pBmpInfo->bmiHeader,&bmpInfo,uRead);
	
	bmpfile.Close();
	return TRUE;
}

BOOL CXHGridViewLeafItemImage::IsBitmapFile(LPCTSTR lpszImageFileName)
{
	CString strBmpFile = lpszImageFileName;
	CString strFileExt;
	int iDot = strBmpFile.ReverseFind(_T('.'));
	if(iDot != -1)	//There is file extension
	{
		if(_tcsicmp(lpszImageFileName + iDot, XH_BITMAP_FILE_EXTENSION) != 0
			&& _tcsicmp(lpszImageFileName + iDot, XH_DIB_FILE_EXTENSION) != 0)
			return FALSE;
	}
	else
		ASSERT(FALSE);	//you'd better provide a file name with an extension
	
	return TRUE;
}

BOOL CXHGridViewLeafItemImage::LoadImageFile(LPCTSTR lpszImageFilePath, BOOL bDelayLoad)
{
	m_strImageFilePath = lpszImageFilePath;
	m_ImageLeafInfo.iImageType = RecognizeImageFileType(lpszImageFilePath);
	if(bDelayLoad)
	{
		m_ImageLeafInfo.dwFlags |= XHGRIDVIEW_IMAGELEAF_FLAG_DELAY_LOAD;
		return TRUE;
	}
	else
	{
		m_ImageLeafInfo.dwFlags &= (~XHGRIDVIEW_IMAGELEAF_FLAG_DELAY_LOAD);
		return LoadImageFile(lpszImageFilePath);
	}

	return FALSE;
}

BOOL CXHGridViewLeafItemImage::LoadImageFile(LPCTSTR lpszImageFilePath)
{
	BOOL bLoad = FALSE;
	switch(m_ImageLeafInfo.iImageType)
	{
	case XHGRIDVIEW_IMAGELEAF_BITMAP:
		bLoad = LoadBitmapFile(lpszImageFilePath);
		break;
	default:
		ASSERT(FALSE);
	}

	if(bLoad)
		m_ImageLeafInfo.dwStatus |= XHGRIDVIEW_IMAGELEAF_STATUS_LOADED;
	else
		m_ImageLeafInfo.dwStatus &= (~XHGRIDVIEW_IMAGELEAF_STATUS_LOADED);

	return bLoad;
}

BOOL CXHGridViewLeafItemImage::LoadImage(UINT nIDBitmap)
{
	CBitmap bmp;
	if(!bmp.LoadBitmap(nIDBitmap))
		return FALSE;

	DeleteImageObject();

	CBitmap *pNewBmp = new CBitmap();
	pNewBmp->Attach(bmp.Detach());
	m_ImageLeafInfo.pImageObject = pNewBmp;
	m_ImageLeafInfo.iImageType = XHGRIDVIEW_IMAGELEAF_BITMAP;

	m_ImageLeafInfo.dwStatus |= XHGRIDVIEW_IMAGELEAF_STATUS_LOADED;

	return TRUE;
}

