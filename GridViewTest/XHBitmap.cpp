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
#include "XHBitmap.h"


#pragma comment(lib,"Msimg32.lib")

//////////////////////////////////////////////////////////////////////
// CXHBitmap
//////////////////////////////////////////////////////////////////////

CXHBitmap::CXHBitmap()
{

}

CXHBitmap::~CXHBitmap()
{

}

CSize CXHBitmap::GetBitmapSize()
{
	ASSERT(m_hObject);
	BITMAP bmp;
	CBitmap::GetBitmap(&bmp);
	return CSize(bmp.bmWidth,bmp.bmHeight);
}

CSize CXHBitmap::Resize(int cx,int cy,BOOL bStretch)
{
	return Resize(CSize(cx,cy),bStretch);
}

CSize CXHBitmap::Resize(SIZE sizeDest,BOOL bStretch)
{
	ASSERT(m_hObject);
	BITMAP bmp;
	CBitmap::GetBitmap(&bmp);
	if(bmp.bmWidth == sizeDest.cx && bmp.bmHeight == sizeDest.cy)
		return sizeDest;

	HDC hdcScreen = ::GetDC(NULL);
	if(!bStretch)
	{
		if((double)sizeDest.cx / sizeDest.cy > (double)bmp.bmWidth / bmp.bmHeight)
			sizeDest.cx = sizeDest.cy * bmp.bmWidth / bmp.bmHeight;
		else
			sizeDest.cy = sizeDest.cx * bmp.bmHeight / bmp.bmWidth;
	}

	HBITMAP hBmpNew = ::CreateCompatibleBitmap(hdcScreen,sizeDest.cx,sizeDest.cy);
	if(hBmpNew)
	{
		HDC hdcBmpNew;
		hdcBmpNew = ::CreateCompatibleDC(hdcScreen);
		HDC hdcBmpOld;
		hdcBmpOld = ::CreateCompatibleDC(hdcScreen);
		if(hdcBmpNew && hdcBmpOld)
		{
			
			HBITMAP hBmpOldOld = (HBITMAP)::SelectObject(hdcBmpOld,(HBITMAP)m_hObject);
			HBITMAP hBmpOldNew = (HBITMAP)::SelectObject(hdcBmpNew,hBmpNew);
			::SetStretchBltMode(hdcBmpNew,HALFTONE);
			::StretchBlt(hdcBmpNew,0,0,sizeDest.cx,sizeDest.cy,hdcBmpOld,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			::SelectObject(hdcBmpOld,hBmpOldOld);
			::SelectObject(hdcBmpNew,hBmpOldNew);
		}

		::DeleteDC(hdcBmpNew);
		::DeleteDC(hdcBmpOld);

		DeleteObject();
		Attach(hBmpNew);
		ReleaseDC(NULL,hdcScreen);
		return GetBitmapSize();
	}
	
	ReleaseDC(NULL,hdcScreen);
	return CSize(0,0);
}

BOOL CXHBitmap::BitBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidth,  // width of destination rectangle
		int nHeight, // height of destination rectangle
		int nXSrc,   // x-coordinate of source upper-left corner
		int nYSrc,   // y-coordinate of source upper-left corner
		DWORD dwRop)  // raster operation code
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = pDestDC->BitBlt(nXDest,nYDest,nWidth,nHeight,&dcBmp,nXSrc,nYSrc,dwRop);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::BitBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nXSrc,   // x-coordinate of source upper-left corner
		int nYSrc,   // y-coordinate of source upper-left corner
		DWORD dwRop)  // raster operation code
{
	ASSERT(m_hObject);
	CSize sizeBitmap = GetBitmapSize();
	return BitBlt(pDestDC,nXDest,nYDest,sizeBitmap.cx,sizeBitmap.cy,nXSrc,nYSrc,dwRop);
}

BOOL CXHBitmap::BitBlt(CDC *pDestDC,
		LPCRECT lprcDest,  // destination area
		int nXSrc,   // x-coordinate of source upper-left corner
		int nYSrc,   // y-coordinate of source upper-left corner
		DWORD dwRop)  // raster operation code
{
	ASSERT(m_hObject);
	return BitBlt(pDestDC,lprcDest->left,lprcDest->top,lprcDest->right - lprcDest->left,lprcDest->bottom - lprcDest->top
		,nXSrc,nYSrc,dwRop);
}


BOOL CXHBitmap::StretchBlt(CDC *pDestDC,
		int nXOriginDest, // x-coord of destination upper-left corner
		int nYOriginDest, // y-coord of destination upper-left corner
		int nWidthDest,   // width of destination rectangle
		int nHeightDest,  // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,   // height of source rectangle
		DWORD dwRop)      // raster operation code
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = pDestDC->StretchBlt(nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,&dcBmp
		,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,dwRop);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::StretchBlt(CDC *pDestDC,
		int nXOriginDest, // x-coord of destination upper-left corner
		int nYOriginDest, // y-coord of destination upper-left corner
		int nWidthDest,   // width of destination rectangle
		int nHeightDest,  // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		DWORD dwRop)      // raster operation code
{
	ASSERT(m_hObject);

	CSize sizeBitmap = GetBitmapSize();
	return StretchBlt(pDestDC,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,nXOriginSrc,nYOriginSrc,sizeBitmap.cx,sizeBitmap.cy,dwRop);
}

BOOL CXHBitmap::StretchBlt(CDC *pDestDC,
		LPCRECT lprcDest,	// destination area
		LPCRECT lprcSrc,	// source area
		DWORD dwRop)      // raster operation code
{
	ASSERT(m_hObject);
	return StretchBlt(pDestDC,lprcDest->left,lprcDest->top,lprcDest->right - lprcDest->left,lprcDest->bottom - lprcDest->top
		,lprcSrc->left,lprcSrc->top,lprcSrc->right - lprcSrc->left,lprcSrc->bottom - lprcSrc->top,dwRop);
}

BOOL CXHBitmap::MaskBlt(CDC *pDestDC,
		int nXDest,      // x-coord of destination upper-left corner
		int nYDest,      // y-coord of destination upper-left corner 
		int nWidth,      // width of source and destination
		int nHeight,     // height of source and destination
		int nXSrc,       // x-coord of upper-left corner of source
		int nYSrc,       // y-coord of upper-left corner of source
		HBITMAP hbmMask, // handle to monochrome bit mask
		int xMask,       // horizontal offset into mask bitmap
		int yMask,       // vertical offset into mask bitmap
		DWORD dwRop)     // raster operation code
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = ::MaskBlt(pDestDC->GetSafeHdc(),nXDest,nYDest,nWidth,nHeight,dcBmp.GetSafeHdc()
		,nXSrc,nYSrc,hbmMask,xMask,yMask,dwRop);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::PlgBlt(CDC *pDestDC,
		CONST POINT *lpPoint, // destination vertices
		int nXSrc,            // x-coord of source upper-left corner
		int nYSrc,            // y-coord of source upper-left corner
		int nWidth,           // width of source rectangle
		int nHeight,          // height of source rectangle
		HBITMAP hbmMask,      // handle to bitmask
		int xMask,            // x-coord of bitmask upper-left corner
		int yMask)            // y-coord of bitmask upper-left corner
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = ::PlgBlt(pDestDC->GetSafeHdc(),lpPoint,dcBmp.GetSafeHdc(),nXSrc,nYSrc,nWidth,nHeight
		,hbmMask,xMask,yMask);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::LoadBitmapFromFile(LPCTSTR lpszFilePath)
{
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,lpszFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if(!hBmp)
		return FALSE;

	DeleteObject();
	Attach(hBmp);
	return TRUE;
}

BOOL CXHBitmap::CopyBitmapFromDC(CDC *pDC,LPCRECT lprcArea)
{
	DeleteObject();
	if(!CreateCompatibleBitmap(pDC,lprcArea->right - lprcArea->left,lprcArea->top - lprcArea->bottom))
		return FALSE;

	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDC))
		return FALSE;
	
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = dcBmp.BitBlt(0,0,lprcArea->right - lprcArea->left,lprcArea->top - lprcArea->bottom
		,pDC,0,0,SRCCOPY);

	dcBmp.SelectObject(pOldBmp);
	dcBmp.DeleteDC();

	return bSuccess;
}


BOOL CXHBitmap::TransparentBlt(CDC *pDestDC,
		int nXOriginDest,   // x-coord of destination upper-left corner
		int nYOriginDest,   // y-coord of destination upper-left corner
		int nWidthDest,     // width of destination rectangle
		int nHeightDest,    // height of destination rectangle
		int nXOriginSrc,    // x-coord of source upper-left corner
		int nYOriginSrc,    // y-coord of source upper-left corner
		int nWidthSrc,      // width of source rectangle
		int nHeightSrc,     // height of source rectangle
		UINT crTransparent) // color to make transparent
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = ::TransparentBlt(pDestDC->GetSafeHdc(),nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,dcBmp.GetSafeHdc()
		,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,crTransparent);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::AlphaBlend(CDC *pDestDC,
		int nXOriginDest,            // x-coord of upper-left corner
		int nYOriginDest,            // y-coord of upper-left corner
		int nWidthDest,              // destination width
		int nHeightDest,             // destination height
		int nXOriginSrc,             // x-coord of upper-left corner
		int nYOriginSrc,             // y-coord of upper-left corner
		int nWidthSrc,               // source width
		int nHeightSrc,              // source height
		BLENDFUNCTION blendFunction)  // alpha-blending function
{
	ASSERT(m_hObject);
	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	BOOL bSuccess = ::AlphaBlend(pDestDC->GetSafeHdc(),nXOriginDest,nYOriginDest,nWidthDest,nHeightDest,dcBmp.GetSafeHdc()
		,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,blendFunction);

	dcBmp.SelectObject(pOldBmp);
	return bSuccess;
}

BOOL CXHBitmap::AlphaBlend(CDC *pDestDC,
		int nXOriginDest,            // x-coord of upper-left corner
		int nYOriginDest,            // y-coord of upper-left corner
		int nWidthDest,              // destination width
		int nHeightDest,             // destination height
		int nXOriginSrc,             // x-coord of upper-left corner
		int nYOriginSrc,             // y-coord of upper-left corner
		int nWidthSrc,               // source width
		int nHeightSrc,              // source height
		int SourceConstantAlpha)  // alpha transparency value used on the entire source bitmap
{
	ASSERT(m_hObject);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = SourceConstantAlpha;

	return AlphaBlend(pDestDC,nXOriginDest,nYOriginDest,nWidthDest,nHeightDest
		,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,bf);
}

BOOL CXHBitmap::FlowBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidthDest,  // width of destination rectangle
		int nHeightDest, // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,		// height of source rectangle
		DWORD dwRop)	// raster operation code
{
	ASSERT(m_hObject);
	CSize sizeBmp = GetBitmapSize();

	if(nWidthSrc == 0)
		nWidthSrc = sizeBmp.cx - nXOriginSrc;
	if(nHeightSrc == 0)
		nHeightSrc = sizeBmp.cy - nYOriginSrc;

	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	for(int x=nXDest;x<nXDest+nWidthDest;x+=nWidthSrc)
		for(int y=nYDest;y<nYDest+nHeightDest;y+=nHeightSrc)
			pDestDC->StretchBlt(x,y,min(nWidthSrc,nXDest+nWidthDest-x),min(nHeightSrc,nYDest+nHeightDest-y),&dcBmp
			,nXOriginSrc,nYOriginSrc,min(nWidthSrc,nXDest+nWidthDest-x),min(nHeightSrc,nYDest+nHeightDest-y),dwRop);

	dcBmp.SelectObject(pOldBmp);
	return TRUE;
}

BOOL CXHBitmap::FlowBlt(CDC *pDestDC,
		LPCRECT lprcDest,	//destination area
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,		// height of source rectangle
		DWORD dwRop)	// raster operation code
{
	ASSERT(m_hObject);
	return FlowBlt(pDestDC,lprcDest->left,lprcDest->top,lprcDest->right - lprcDest->left,lprcDest->bottom - lprcDest->top
		,nXOriginSrc,nYOriginSrc,nWidthSrc,nHeightSrc,dwRop);
}


BOOL CXHBitmap::TransparentFlowBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidthDest,  // width of destination rectangle
		int nHeightDest, // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,		// height of source rectangle
		UINT crTransparent)	// color to make transparent
{
	ASSERT(m_hObject);
	CSize sizeBmp = GetBitmapSize();

	if(nWidthSrc == 0)
		nWidthSrc = sizeBmp.cx - nXOriginSrc;
	if(nHeightSrc == 0)
		nHeightSrc = sizeBmp.cy - nYOriginSrc;

	CDC dcBmp;
	if(!dcBmp.CreateCompatibleDC(pDestDC))
		return FALSE;
	CBitmap *pOldBmp = dcBmp.SelectObject(this);

	for(int x=0;x<nXDest+nWidthDest;x+=nWidthSrc)
		for(int y=0;y<nYDest+nHeightDest;y+=nHeightSrc)
			::TransparentBlt(pDestDC->GetSafeHdc(),x,y,min(nWidthSrc,nXDest+nWidthDest-x),min(nHeightSrc,nYDest+nHeightDest-y),dcBmp.GetSafeHdc()
			,nXOriginSrc,nYOriginSrc,min(nWidthSrc,nXDest+nWidthDest-x),min(nHeightSrc,nYDest+nHeightDest-y),crTransparent);


	dcBmp.SelectObject(pOldBmp);
	return TRUE;
}

//static
BOOL CXHBitmap::GetBitmap(LPCTSTR lpszFilePath,LPBITMAP pBitmap)
{
	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL,lpszFilePath,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if(hBmp)
	{
		VERIFY(::GetObject(hBmp,sizeof(BITMAP),pBitmap) == sizeof(BITMAP));
		::DeleteObject(hBmp);
		return TRUE;
	}
	return FALSE;
}
