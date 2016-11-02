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

#if !defined(AFX_XHCOMMON_H__1D368D2D_E5FE_41A3_8F70_5932DE4382EE__INCLUDED_)
#define AFX_XHCOMMON_H__1D368D2D_E5FE_41A3_8F70_5932DE4382EE__INCLUDED_

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// CXHColor

class CXHColor
{
public:
	CXHColor();
	CXHColor(COLORREF clrColor);
	CXHColor(const CXHColor& color);
	virtual ~CXHColor();
public:
	operator COLORREF();
	CXHColor& operator=(const CXHColor& color);
	BOOL operator ==(const CXHColor& color);
public:
	CXHColor& MixedWith(const CXHColor& color,double dRateSelf = 0.5f);
	BYTE GetRedValue() const;
	BYTE GetGreenValue() const;
	BYTE GetBlueValue() const;
	BYTE Average();
	void Reverse();
	BYTE Different(COLORREF clrCompareTo);
	COLORREF Gray();
public:
	static COLORREF MixColor(COLORREF clrColor1,COLORREF clrColor2,double dRate1 = 0.5f);
	static BYTE Average(COLORREF clrColor);
	static COLORREF Reverse(COLORREF clrColor);
	static BYTE Different(COLORREF clrColor1,COLORREF clrColor2);
	static COLORREF Gray(COLORREF clrColor);
protected:
	COLORREF m_clrColor;
};

//////////////////////////////////////////////////////////////////////////
// CXHRect
class CXHRect:public CRect
{
public:
	CXHRect();
	virtual ~CXHRect();
	CXHRect( int l, int t, int r, int b ):CRect(l,t,r,b){};
	CXHRect( const RECT& srcRect ):CRect(srcRect){};
	CXHRect( LPCRECT lpSrcRect ):CRect(lpSrcRect){};
	CXHRect( POINT point, SIZE size ):CRect(point,size){};
	CXHRect( POINT topLeft, POINT bottomRight ):CRect(topLeft,bottomRight){};

public:
	void FitRect(LPCRECT lprcFitTo);
	void Normalize();		//Normalize to (0,0,width,height)
	void CenterInRect(LPCRECT lprcOuterRect);
	void OverlapToRect(LPCRECT lprcTarget);		//Overlap at (0,0)
};

//////////////////////////////////////////////////////////////////////////
// CXHWindowRect

class CXHWindowRect : public CXHRect  
{
public:
	CXHWindowRect(CWnd *pWnd);
	virtual ~CXHWindowRect();
};

//////////////////////////////////////////////////////////////////////////
// CXHClientRect

class CXHClientRect : public CXHRect  
{
public:
	CXHClientRect(CWnd *pWnd);
	virtual ~CXHClientRect();
};

//////////////////////////////////////////////////////////////////////////
// CXHMemDC
class CXHMemDC:public CDC
{
private:
	CBitmap*	m_bitmap;
	CBitmap*	m_oldBitmap;
	CDC*		m_pDC;
	CRect		m_rcBounds;
	BOOL		m_bAutoDrawBack;
public:
	CXHMemDC(CDC* pDC, int cx, int cy, BOOL bAutoDrawBack = FALSE);
	CXHMemDC(CDC* pDC, const CRect& rcBounds, BOOL bAutoDrawBack = FALSE);
	~CXHMemDC();
	CXHMemDC* operator->();
	BOOL DrawTo(CDC* pDstDC,int x,int y);
	BOOL DrawTo(CDC* pDstDC,POINT ptDest);
};
#endif // !defined(AFX_XHCOMMON_H__1D368D2D_E5FE_41A3_8F70_5932DE4382EE__INCLUDED_)
