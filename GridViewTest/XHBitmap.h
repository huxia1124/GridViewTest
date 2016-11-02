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

#if !defined(AFX_XHBITMAP_H__55E68466_B531_4765_BFFB_5FB14EBB293D__INCLUDED_)
#define AFX_XHBITMAP_H__55E68466_B531_4765_BFFB_5FB14EBB293D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef AC_SRC_ALPHA
	#define AC_SRC_ALPHA	0
#endif

class CXHBitmap : public CBitmap  
{
public:
	CXHBitmap();
	virtual ~CXHBitmap();

public:
	CSize GetBitmapSize();
	
	//Resize to a given size.the smaller in size,the less memory required to store the bitmap,but also the lower in quality
	//bStretch = FALSE to keep the ratio of width/height,otherwise stretch to given size
	CSize Resize(SIZE sizeDest,BOOL bStretch = FALSE);
	CSize Resize(int cx,int cy,BOOL bStretch = FALSE);

//Drawing Wrappers
public:
	BOOL LoadBitmapFromFile(LPCTSTR lpszFilePath);
	BOOL CopyBitmapFromDC(CDC *pDC,LPCRECT lprcArea);

	BOOL FlowBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidthDest,  // width of destination rectangle
		int nHeightDest, // height of destination rectangle
		int nXOriginSrc = 0,  // x-coord of source upper-left corner
		int nYOriginSrc = 0,  // y-coord of source upper-left corner
		int nWidthSrc = 0,    // width of source rectangle
		int nHeightSrc = 0,		// height of source rectangle
		DWORD dwRop = SRCCOPY);	// raster operation code
	BOOL FlowBlt(CDC *pDestDC,
		LPCRECT lprcDest,	//destination area
		int nXOriginSrc = 0,  // x-coord of source upper-left corner
		int nYOriginSrc = 0,  // y-coord of source upper-left corner
		int nWidthSrc = 0,    // width of source rectangle
		int nHeightSrc = 0,		// height of source rectangle
		DWORD dwRop = SRCCOPY);	// raster operation code
	BOOL TransparentFlowBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidthDest,  // width of destination rectangle
		int nHeightDest, // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,		// height of source rectangle
		UINT crTransparent);	// color to make transparent

	//BitBlt
	BOOL BitBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nWidth,  // width of destination rectangle
		int nHeight, // height of destination rectangle
		int nXSrc,   // x-coordinate of source upper-left corner
		int nYSrc,   // y-coordinate of source upper-left corner
		DWORD dwRop = SRCCOPY);  // raster operation code
	BOOL BitBlt(CDC *pDestDC,
		int nXDest,  // x-coord of destination upper-left corner
		int nYDest,  // y-coord of destination upper-left corner
		int nXSrc = 0,   // x-coordinate of source upper-left corner
		int nYSrc = 0,   // y-coordinate of source upper-left corner
		DWORD dwRop = SRCCOPY);  // raster operation code
	BOOL BitBlt(CDC *pDestDC,
		LPCRECT lprcDest,  // destination area
		int nXSrc = 0,   // x-coordinate of source upper-left corner
		int nYSrc = 0,   // y-coordinate of source upper-left corner
		DWORD dwRop = SRCCOPY);  // raster operation code

	//StretchBlt
	BOOL StretchBlt(CDC *pDestDC,
		int nXOriginDest, // x-coord of destination upper-left corner
		int nYOriginDest, // y-coord of destination upper-left corner
		int nWidthDest,   // width of destination rectangle
		int nHeightDest,  // height of destination rectangle
		int nXOriginSrc,  // x-coord of source upper-left corner
		int nYOriginSrc,  // y-coord of source upper-left corner
		int nWidthSrc,    // width of source rectangle
		int nHeightSrc,   // height of source rectangle
		DWORD dwRop = SRCCOPY);      // raster operation code
	BOOL StretchBlt(CDC *pDestDC,
		int nXOriginDest, // x-coord of destination upper-left corner
		int nYOriginDest, // y-coord of destination upper-left corner
		int nWidthDest,   // width of destination rectangle
		int nHeightDest,  // height of destination rectangle
		int nXOriginSrc = 0,  // x-coord of source upper-left corner
		int nYOriginSrc = 0,  // y-coord of source upper-left corner
		DWORD dwRop = SRCCOPY);      // raster operation code
	BOOL StretchBlt(CDC *pDestDC,
		LPCRECT lprcDest,	// destination area
		LPCRECT lprcSrc,	// source area
		DWORD dwRop = SRCCOPY);      // raster operation code
	
	//MaskBlt
	BOOL MaskBlt(CDC *pDestDC,
		int nXDest,      // x-coord of destination upper-left corner
		int nYDest,      // y-coord of destination upper-left corner 
		int nWidth,      // width of source and destination
		int nHeight,     // height of source and destination
		int nXSrc,       // x-coord of upper-left corner of source
		int nYSrc,       // y-coord of upper-left corner of source
		HBITMAP hbmMask, // handle to monochrome bit mask
		int xMask,       // horizontal offset into mask bitmap
		int yMask,       // vertical offset into mask bitmap
		DWORD dwRop);     // raster operation code
	
	//PlgBlt
	BOOL PlgBlt(CDC *pDestDC,
		CONST POINT *lpPoint, // destination vertices
		int nXSrc,            // x-coord of source upper-left corner
		int nYSrc,            // y-coord of source upper-left corner
		int nWidth,           // width of source rectangle
		int nHeight,          // height of source rectangle
		HBITMAP hbmMask,      // handle to bitmask
		int xMask,            // x-coord of bitmask upper-left corner
		int yMask);            // y-coord of bitmask upper-left corner

	//TransparentBlt
	BOOL TransparentBlt(CDC *pDestDC,
		int nXOriginDest,   // x-coord of destination upper-left corner
		int nYOriginDest,   // y-coord of destination upper-left corner
		int nWidthDest,     // width of destination rectangle
		int hHeightDest,    // height of destination rectangle
		int nXOriginSrc,    // x-coord of source upper-left corner
		int nYOriginSrc,    // y-coord of source upper-left corner
		int nWidthSrc,      // width of source rectangle
		int nHeightSrc,     // height of source rectangle
		UINT crTransparent); // color to make transparent
	
	//AlphaBlend
	BOOL AlphaBlend(CDC *pDestDC,
		int nXOriginDest,            // x-coord of upper-left corner
		int nYOriginDest,            // y-coord of upper-left corner
		int nWidthDest,              // destination width
		int nHeightDest,             // destination height
		int nXOriginSrc,             // x-coord of upper-left corner
		int nYOriginSrc,             // y-coord of upper-left corner
		int nWidthSrc,               // source width
		int nHeightSrc,              // source height
		BLENDFUNCTION blendFunction);  // alpha-blending function
	BOOL AlphaBlend(CDC *pDestDC,
		int nXOriginDest,            // x-coord of upper-left corner
		int nYOriginDest,            // y-coord of upper-left corner
		int nWidthDest,              // destination width
		int nHeightDest,             // destination height
		int nXOriginSrc,             // x-coord of upper-left corner
		int nYOriginSrc,             // y-coord of upper-left corner
		int nWidthSrc,               // source width
		int nHeightSrc,              // source height
		int SourceConstantAlpha);  // alpha transparency value used on the entire source bitmap

public:
	static BOOL GetBitmap(LPCTSTR lpszFilePath,LPBITMAP pBitmap);

};

#endif // !defined(AFX_XHBITMAP_H__55E68466_B531_4765_BFFB_5FB14EBB293D__INCLUDED_)
