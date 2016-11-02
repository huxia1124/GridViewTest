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
#include "XHCommon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////
// CXHColor

CXHColor::CXHColor()
{
	m_clrColor = RGB(0,0,0);
}

CXHColor::CXHColor(COLORREF clrColor)
{
	m_clrColor = clrColor;
}

CXHColor::CXHColor(const CXHColor& color)
{
	*this = color;
}

CXHColor::~CXHColor()
{
}

CXHColor& CXHColor::operator=(const CXHColor& color)
{
	m_clrColor = color.m_clrColor;
	return *this;
}

CXHColor::operator COLORREF()
{
	return m_clrColor;
}

BOOL CXHColor::operator ==(const CXHColor& color)
{
	return m_clrColor == color.m_clrColor;
}

CXHColor& CXHColor::MixedWith(const CXHColor& color,double dRateSelf)
{
	double dRateOther = 1.0f - dRateSelf;
	m_clrColor = RGB(GetRValue(m_clrColor) * dRateSelf + GetRValue(color.m_clrColor) * dRateOther,
		GetGValue(m_clrColor) * dRateSelf + GetGValue(color.m_clrColor) * dRateOther,
		GetBValue(m_clrColor) * dRateSelf + GetBValue(color.m_clrColor) * dRateOther);
	return *this;
}

BYTE CXHColor::GetRedValue() const
{
	return GetRValue(m_clrColor);
}

BYTE CXHColor::GetGreenValue() const
{
	return GetGValue(m_clrColor);
}

BYTE CXHColor::GetBlueValue() const
{
	return GetBValue(m_clrColor);
}

BYTE CXHColor::Average()
{
	return Average(m_clrColor);
}

void CXHColor::Reverse()
{
	m_clrColor = ((~m_clrColor) & 0x00FFFFFF);
}

BYTE CXHColor::Different(COLORREF clrCompareTo)
{
	return Different(m_clrColor,clrCompareTo);
}

COLORREF CXHColor::Gray()
{
	m_clrColor = Gray(m_clrColor);
	return m_clrColor;
}

//static
COLORREF CXHColor::MixColor(COLORREF clrColor1,COLORREF clrColor2,double dRate1 /* = 0.5f */)
{
	double dRate2= 1.0f - dRate1;
	return RGB(GetRValue(clrColor1) * dRate1 + GetRValue(clrColor2) * dRate2,
		GetGValue(clrColor1) * dRate1 + GetGValue(clrColor2) * dRate2,
		GetBValue(clrColor1) * dRate1 + GetBValue(clrColor2) * dRate2);
}

//static
BYTE CXHColor::Average(COLORREF clrColor)
{
	return (GetRValue(clrColor) + GetGValue(clrColor) + GetBValue(clrColor)) / 3;
}

//static
COLORREF CXHColor::Reverse(COLORREF clrColor)
{
	return (~clrColor) & 0x00FFFFFF;
}

//static
BYTE CXHColor::Different(COLORREF clrColor1,COLORREF clrColor2)
{
	return ( abs(GetRValue(clrColor1) - GetRValue(clrColor2))
		+ abs(GetGValue(clrColor1) - GetGValue(clrColor2))
		+ abs(GetBValue(clrColor1) - GetBValue(clrColor2)) ) / 3;
}

//static
COLORREF CXHColor::Gray(COLORREF clrColor)
{
	BYTE byVal =(BYTE)( GetRValue(clrColor) * 0.3f + GetRValue(clrColor) * 0.6f + GetRValue(clrColor) * 0.1f );
	return RGB(byVal,byVal,byVal);
}


//////////////////////////////////////////////////////////////////////
// CXHRect

CXHRect::CXHRect():CRect()
{

}

CXHRect::~CXHRect()
{

}
void CXHRect::OverlapToRect(LPCRECT lprcTarget)
{
	OffsetRect(lprcTarget->left - left, lprcTarget->top - top);
}

void CXHRect::CenterInRect(LPCRECT lprcOuterRect)
{
	OverlapToRect(lprcOuterRect);
	int iXOffsetPatch = (lprcOuterRect->right - right) / 2;
	int iYOffsetPatch = (lprcOuterRect->bottom - bottom) / 2;
	OffsetRect(iXOffsetPatch,iYOffsetPatch);
}

void CXHRect::Normalize()
{
	OffsetRect(-left,-top);
}

void CXHRect::FitRect(LPCRECT lprcFitTo)
{
	int iWidth = lprcFitTo->right - lprcFitTo->left;
	int iHeight = lprcFitTo->bottom - lprcFitTo->top;

	if(Height() <= iHeight && Width() <= iWidth)
	{
		CenterInRect(lprcFitTo);
		return;
	}

	if((double)Width()/Height() > (double)iWidth/iHeight)	//Normalize to Width
	{
		double dRatio = (double)Width() / iWidth;
		right = (long)(left + Width() / dRatio);
		bottom = (long)(top + Height() / dRatio);
		CenterInRect(lprcFitTo);
	}
	else if((double)Width()/Height() <= (double)iWidth/iHeight)	//Normalize to Height
	{
		double dRatio = (double)Height() / iHeight;
		bottom = (long)(top + Height() / dRatio);
		right = (long)(left + Width() / dRatio);
		CenterInRect(lprcFitTo);
	}
}

//////////////////////////////////////////////////////////////////////
// CXHWindowRect

CXHWindowRect::CXHWindowRect(CWnd *pWnd)
{
	pWnd->GetWindowRect(this);
}

CXHWindowRect::~CXHWindowRect()
{

}

//////////////////////////////////////////////////////////////////////////
// CXHClientRect

CXHClientRect::CXHClientRect(CWnd *pWnd)
{
	pWnd->GetClientRect(this);
}

CXHClientRect::~CXHClientRect()
{

}

//////////////////////////////////////////////////////////////////////////
// CXHMemDC
CXHMemDC::CXHMemDC(CDC* pDC, int cx, int cy, BOOL bAutoDrawBack) : CDC()
{
	CreateCompatibleDC(pDC);
	m_bitmap = new CBitmap;
	m_bitmap->CreateCompatibleBitmap(pDC, cx, cy);
	m_oldBitmap = SelectObject(m_bitmap);
	m_pDC = pDC;
	m_rcBounds.SetRect(0,0,cx,cy);
	m_bAutoDrawBack = bAutoDrawBack;
}

CXHMemDC::CXHMemDC(CDC* pDC, const CRect& rcBounds, BOOL bAutoDrawBack) : CDC()
{
	CreateCompatibleDC(pDC);
	m_bitmap = new CBitmap;
	m_bitmap->CreateCompatibleBitmap(pDC, rcBounds.Width(), rcBounds.Height());
	m_oldBitmap = SelectObject(m_bitmap);
	m_pDC = pDC;
	m_rcBounds = rcBounds;
	m_bAutoDrawBack = bAutoDrawBack;
}

CXHMemDC::~CXHMemDC() 
{
	if(m_bAutoDrawBack)
		m_pDC->BitBlt(m_rcBounds.left,m_rcBounds.top,m_rcBounds.Width(),m_rcBounds.Height(),this,0,0,SRCCOPY);
	SelectObject(m_oldBitmap);
	if (m_bitmap != NULL) 
		delete m_bitmap;
}

CXHMemDC* CXHMemDC::operator->() 
{
	return this;
}

BOOL CXHMemDC::DrawTo(CDC* pDstDC,int x,int y)
{
	return pDstDC->BitBlt(x,y,m_rcBounds.Width(),m_rcBounds.Height(),this,0,0,SRCCOPY);
}

BOOL CXHMemDC::DrawTo(CDC* pDstDC,POINT ptDest)
{
	return DrawTo(pDstDC, ptDest.x,ptDest.y);
}
