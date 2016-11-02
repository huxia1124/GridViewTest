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

#if !defined(AFX_XHGRIDVIEWITEM_H__DC8B13E2_6921_4CB1_A29C_5965C5042FF6__INCLUDED_)
#define AFX_XHGRIDVIEWITEM_H__DC8B13E2_6921_4CB1_A29C_5965C5042FF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "XHCommon.h"

#define DEFAULT_BAND_WIDTH			80

#define GVM_INSERTNEWITEM		(WM_USER + 100)
#define GVM_REDRAWITEMS			(WM_USER + 101)
#define GVM_DELETEITEM			(WM_USER + 102)

//////////////////////////////////////////////////////////////////////////

class CXHGridView;

class CXHGridViewItemStructure;
class CXHGridViewLeafItems;
class CXHGridViewLeafItem;
class CXHGridViewSubItems;
class CXHGridViewSubItem;
class CXHGridViewBaseItems;
class CXHGridViewBaseItem;
class CXHGridViewLeafDrawHelper;
class CXHGridViewLeafDrawHelperBand;
class CXHGridViewLeafDrawHelperHeader;
class CXHGridViewLeafDrawHelperItem;

//////////////////////////////////////////////////////////////////////////

#define XHGRIDVIEW_LEAF_STYLE_NORMAL		0x0000
#define XHGRIDVIEW_LEAF_STYLE_BUTTON		0x0001

#define XHGRIDVIEW_LEAF_STATE_SELECTED		0x0001
#define XHGRIDVIEW_LEAF_STATE_FOCUS			0x0002
#define XHGRIDVIEW_LEAF_STATE_MOUSEOVER		0x0004
#define XHGRIDVIEW_LEAF_STATE_PUSHED		DFCS_PUSHED		//0x0200

#define XHGRIDVIEW_ITEM_STATE_SELECTED		0x0001
#define XHGRIDVIEW_ITEM_STATE_FOCUSED		0x0002

#define XHGRIDVIEW_LEAF_FLAG_USELEAFBACKCOLOR		0x0001	//Use back color of the leaf

#define XHGRIDVIEW_DETAIL			1
#define XHGRIDVIEW_REPORT			2

//constant
#define XHGRIDVIEW_LINE_WIDTH			1
#define XHGRIDVIEW_LEAF_BORDER_WIDTH	2
#define XHGRIDVIEW_ROW_HEIGHT			20
#define XHGRIDVIEW_LEAF_WIDTH			60
#define XHGRIDVIEW_MARK_WIDTH			12
#define XHGRIDVIEW_ITEMSPACING			4
#define XHGRIDVIEW_MIN_LEAFWIDTH		24
#define XHGRIDVIEW_DRAG_MIN				8		//the distance to be considered as begin-drag
#define XHGRIDVIEW_MAX_SELECT_SESSION	32000

#define XHGRIDVIEW_ITEMTYPE_BAND		1
#define XHGRIDVIEW_ITEMTYPE_HEADER		2
#define XHGRIDVIEW_ITEMTYPE_ITEM		3

#define XHGRIDVIEW_BASEITEM_FLAG_STRETCH		0x0001

#define XHGRIDVIEW_IMAGELIST_CX			16
#define XHGRIDVIEW_IMAGELIST_CY			16

#define XHGRIDVIEW_IMAGELEAF_NULL			0
#define XHGRIDVIEW_IMAGELEAF_BITMAP			1

#define XHGRIDVIEW_IMAGELEAF_FLAG_DELAY_LOAD		0x0001

#define XHGRIDVIEW_IMAGELEAF_STATUS_LOADED			0x0001

#define WM_XHGRIDVIEW_SPECIAL_MESSAGE		(WM_USER + 10)
#define XHGRIDVIEW_MESSAGE_HEADERCLICK		1
#define XHGRIDVIEW_MESSAGE_BANDCLICK		2

#define XHGRIDVIEW_FLAG_NONE				0x0000
#define XHGRIDVIEW_FLAG_GRIDLINE			0x0001

#define XHGRIDVIEW_STATUS_LBUTTONDOWN		0x0001
#define XHGRIDVIEW_STATUS_RBUTTONDOWN		0x0002
#define XHGRIDVIEW_STATUS_DRAGGING			0x0004



//////////////////////////////////////////////////////////////////////////

typedef struct tagXHGRIDVIEWLEAFITEM
{
	int iStyle;
	int iID;
	DWORD dwState;
	DWORD dwLeafItemData;
	DWORD dwFlags;
	int iWidth;
	int iImage;			//ImageIndex
	int iSortImage;		//default -1
	BYTE lfItalic;
	BYTE lfUnderline;
	LONG lfWeight;
	COLORREF clrBackground;
	COLORREF clrForeground;
}XHGRIDVIEWLEAFITEM,*LPXHGRIDVIEWLEAFITEM;

class CXHGridViewLeafItem:public CObject
{
	DECLARE_DYNAMIC(CXHGridViewLeafItem)
public:
	CXHGridViewLeafItem();
	CXHGridViewLeafItem(LPCTSTR lpszText);
	virtual ~CXHGridViewLeafItem();
public:

	LONG Release();
	LONG AddRef();

	void SetWidth(int iNewWidth);
	int GetWidth() const;
	void SetParentLeafItems(CXHGridViewLeafItems *pParent);
	CXHGridViewLeafItems *GetParentLeafItems();
	int GetFactWidth();
	int GetHeight();
	int GetFixedWidth();
	int GetFixedHeight();

	//获得 LeafItem 的整个区域，采用 LeafItem 坐标系
	void GetLeafRect(LPRECT lprcRect);

	//获得 LeafItem 的可绘图区域，采用 LeafItem 坐标系。此方法在 GetLeafRect 的基础上除去了线宽
	void GetLeafDrawingRect(LPRECT lprcRect);

	//获得 LeafItem 的除去两边图案的区域，采用 LeafItem 坐标系
	void GetLeafClientRect(LPRECT lprcRect);

	//获得 LeafItem 的除去两边图案的区域，总是获得其在 Detail Mode 下的区域。采用 LeafItem 坐标系
	void GetLeafClientFixedRect(LPRECT lprcRect);

	//获得 LeafItem 的左边图案的区域，采用 LeafItem 坐标系
	BOOL GetLeafImageRect(LPRECT lprcRect);

	//获得 LeafItem 的 Sort Image 图案的区域(右边)，采用 LeafItem 坐标系
	BOOL GetLeafSortImageRect(LPRECT lprcRect);
	
	DWORD GetState() const;

	void ModifyState(DWORD dwRemove,DWORD dwAdd);

	//设置 LeafItem 所使用的图片在 ImageList 中的图片索引， -1 表示不使用图片
	void SetImageIndex(int iImageIndex);
	void SetSortImageIndex(int iImageIndex);

	CXHGridViewItemStructure* GetParentStructure();

	void SetID(int iID);
	int GetID();

	void SetFocused(BOOL bFocused = TRUE);
	BOOL IsFocused();

	COLORREF GetTextColor() const;
	void SetTextColor(COLORREF clrText);
	COLORREF GetBackColor() const;
	void SetBackColor(COLORREF clrBackground);
	void SetTextBold(BOOL bBold = TRUE);
	void SetTextItalic(BOOL bItalic = TRUE);
	void SetTextUnderline(BOOL bUnderline = TRUE);


	void CopyLeafInfo(CXHGridViewLeafItem *pSrcLeaf);

	void SetLeafItemData(DWORD dwData);
	DWORD GetLeafItemData() const;

	BOOL MouseEnter();
	BOOL MouseLeave();

	void TriggerClickEvent();

public:
	virtual CString GetText() const;
	virtual void SetText(LPCTSTR lpszText);
	virtual void Draw(CDC *pDC);
	virtual void DrawContent(CDC *pDC);
	virtual CXHGridViewLeafItem* CloneLeafItem();
	virtual void DrawLeftImage(CDC *pDC);
	virtual void DrawSortImage(CDC *pDC);

protected:
	//事件处理
	virtual void OnMouseEnter();
	virtual void OnMouseLeave();
	virtual void OnClick();

protected:
	virtual void Initialize();
protected:
	LONG m_nRef;
	CString m_strText;
	XHGRIDVIEWLEAFITEM m_LeafItemInfo;
	CXHGridViewLeafItems *m_pParentLeafItems;
};

//////////////////////////////////////////////////////////////////////////

#define XH_BITMAP_FILE_EXTENSION	(_T(".BMP"))
#define XH_DIB_FILE_EXTENSION	(_T(".DIB"))

typedef struct tag_XHGRIDVIEWIMAGELEAFINFO
{
	LPVOID pImageObject;	//All sorts of image use this
	SIZE sizeImage;			//Image Size
	int iImageType;			//Type of the image object
	DWORD dwFlags;
	DWORD dwStatus;
}XHGRIDVIEWIMAGELEAFINFO,*LPXHGRIDVIEWIMAGELEAFINFO;

class CXHGridViewLeafItemImage:public CXHGridViewLeafItem
{
	
	DECLARE_DYNAMIC(CXHGridViewLeafItemImage)

public:
	CXHGridViewLeafItemImage();
	CXHGridViewLeafItemImage(LPCTSTR lpszImageFilePath, BOOL bDelayLoad = TRUE);
	CXHGridViewLeafItemImage(HBITMAP hBitmap);
	virtual ~CXHGridViewLeafItemImage();
public:
	virtual void DrawContent(CDC *pDC);
public:
	BOOL LoadImageFile(LPCTSTR lpszImageFilePath, BOOL bDelayLoad);
	virtual BOOL LoadImageFile(LPCTSTR lpszImageFilePath);
	void GetImageRect(LPRECT lprcImage);	// Rect (0,0,image_width,image_height)
	BOOL LoadImage(UINT nIDBitmap);

protected:
	
protected:
	XHGRIDVIEWIMAGELEAFINFO m_ImageLeafInfo;
	CString m_strImageFilePath;

protected:
	virtual void DeleteImageObject();

private:
	void DrawBitmapContent(CDC *pDC);
	void GetBitmapRect(LPRECT lprcBitmap);
	BOOL LoadBitmapFile(LPCTSTR lpszBmpFilePath);
	int RecognizeImageFileType(LPCTSTR lpszImageFileName);
	BOOL GetBitmapFileInfo(LPCTSTR lpszBmpFilePath,LPBITMAPINFO pBmpInfo);

private:
	BOOL IsBitmapFile(LPCTSTR lpszImageFileName);

private:
	void Initialize();
};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWLEAFITEMSINFO
{
	int iTotalWidth;
}_XHGRIDVIEWLEAFITEMSINFO;

class CXHGridViewLeafItems
{
public:
	CXHGridViewLeafItems();
	virtual ~CXHGridViewLeafItems();
public:
	CXHGridViewLeafItem* InsertLeafItem(int iIndex,LPCTSTR lpszText,int iWidth = DEFAULT_BAND_WIDTH);
	CXHGridViewLeafItem* GetLeafItem(int iIndex);
	int GetCount() const;
	CXHGridViewLeafItem* LeafHitTest(POINT point);	//point: SubItem 中的局部坐标,仅 X 有效
	int GetLeafFactWidth(int iLeafIndex);
	int GetLeafItemFactWidth(int iLeafIndexStart,int nCount = 1);
	int GetLeafFixedWidth(int iLeafIndex);
	void SetParentSubItem(CXHGridViewSubItem *pParent);
	CXHGridViewSubItem* GetParentSubItem();
	
	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	
	int GetLeafItemIndex(const CXHGridViewLeafItem *pLeafItem) const;
	void Draw(CDC *pDC);
	void DeleteAllLeafItems();

	//修正 _m_LeafItemsInfo 中 iTotalWidth 的值，通常由 LeafItem 调用
	void AddTotalWidth(int iAdd);

	//将所有 LeafItem 的 Width 都转换为实际坐标宽度
	void RefactAllLeafItemWidth();

	int GetLeafItemWidth(int iIndexStart,int iCount);
	void SetLeafItem(int iLeafIndex,CXHGridViewLeafItem *pNew);

	void CopyFrom(CXHGridViewLeafItems *pLeafItems);
	CXHGridViewLeafItem* FindLeafByID(int iLeafID);

protected:
	CArray <CXHGridViewLeafItem*,CXHGridViewLeafItem*> m_arrpLeafItems;
	CXHGridViewSubItem *m_pParentSubItem;
	_XHGRIDVIEWLEAFITEMSINFO _m_LeafItemsInfo;
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewSubItem
{
public:
	CXHGridViewSubItem();
	virtual ~CXHGridViewSubItem();
public:
	CXHGridViewLeafItem* GetLeafItem(int iIndex);
	void SetParentSubItems(CXHGridViewSubItems *pParent);
	CXHGridViewSubItems* GetParentSubItems();
	CXHGridViewLeafItem* InsertLeafItem(int iLeafIndex,LPCTSTR lpszText,int iWidth = DEFAULT_BAND_WIDTH);
	CXHGridViewLeafItem* LeafHitTest(POINT point);	//point: SubItem 中的局部坐标,仅 X 有效

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	int GetHeight();
	void Draw(CDC *pDC);

	//以当前 SubItem 的结构产生一个新的 SubItem
	CXHGridViewSubItem* CloneSubItem();

	int GetLeafCount();

	void SetLeafItem(int iLeafIndex,CXHGridViewLeafItem *pNew);

	CXHGridViewLeafItem* FindLeafByID(int iLeafID);

protected:
	//获得 SubItem 的区域。SubItem 局部坐标(即left和top总是0)
	void GetSubItemRect(LPRECT lprcSubItem);

protected:
	virtual void OnDrawSubItem(CDC *pDC,LPCRECT lprcSubItem);

protected:
	CXHGridViewLeafItems m_LeafItems;
	CXHGridViewSubItems *m_pParentSubItems;
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewSubItems
{
public:
	CXHGridViewSubItems();
	virtual ~CXHGridViewSubItems();
public:
	CXHGridViewSubItem* InsertSubItem(int iIndex);
	CXHGridViewSubItem* GetSubItem(int iIndex);
	int GetCount() const;
	void SetParentBaseItem(CXHGridViewBaseItem* pParent);
	CXHGridViewBaseItem* GetParentBaseItem();
	CXHGridViewSubItem* SubItemHitTest(POINT point);

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	void Draw(CDC *pDC);
	void CopyFrom(CXHGridViewSubItems *pSubItems);
	void DeleteAllSubItems();
	CXHGridViewLeafItem* FindLeafByID(int iLeafID);
	BOOL IsLastSubItem(CXHGridViewSubItem *pSubItem);
	int GetSubItemIndex(CXHGridViewSubItem *pSubItem);
	int GetSubItemHeight(int iIndexStart,int iCount);
	int GetMaxBaseItemLeafCount();	//See CXHGridViewItemStructure for details

protected:
	CArray <CXHGridViewSubItem*,CXHGridViewSubItem*> m_arrpSubItems;
	CXHGridViewBaseItem *m_pParentBaseItem;
};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWBASEITEMINFO
{
	DWORD dwFlags;
}_XHGRIDVIEWBASEITEMINFO;

class CXHGridViewBaseItem
{
public:
	CXHGridViewBaseItem();
	virtual ~CXHGridViewBaseItem();
	
public:
	CXHGridViewSubItem* InsertSubItem(int iIndex);
	CXHGridViewSubItem* GetSubItem(int iIndex);
	void SetWidth(int iNewWidth);

	//获得 BaseItem 的实际宽度。
	int GetWidth();
	void SetParentBaseItems(CXHGridViewBaseItems *pParent);
	CXHGridViewBaseItems* GetParentBaseItems();
	CXHGridViewSubItem* SubItemHitTest(POINT point);
	int GetSubItemCount() const;

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	void Draw(CDC *pDC);
	BOOL IsStretch() ;
	int GetHeight();

	//以当前 BaseItem 的结构产生一个新的 BaseItem
	CXHGridViewBaseItem* CloneBaseItem();

	int GetMaxBaseItemLeafCount();	//See CXHGridViewItemStructure for details

	CXHGridViewLeafItem* FindLeafByID(int iLeafID);

protected:
	int m_iWidth;
	CXHGridViewSubItems m_SubItems;
	CXHGridViewBaseItems *m_pParentBaseItems;
	_XHGRIDVIEWBASEITEMINFO _m_BaseItemInfo;
};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWBASEITEMSINFO
{
	int iMaxRowCount;
}_XHGRIDVIEWBASEITEMSINFO;

class CXHGridViewBaseItems
{
public:
	CXHGridViewBaseItems();
	virtual ~CXHGridViewBaseItems();

public:
	CXHGridViewBaseItem* InsertBaseItem(int iIndex,int iWidth);
	CXHGridViewBaseItem* GetBaseItem(int iIndex);
	int GetCount() const;
	void SetParentStructure(CXHGridViewItemStructure *pParent);
	CXHGridViewItemStructure* GetParentStructure();
	CXHGridViewBaseItem* BaseHitTest(POINT point);		//point 是相对坐标点
	CXHGridViewSubItem* SubItemHitTest(POINT point);
	void DeleteAllBaseItems();

	//获得 BaseItem 集合中最大的行数
	int GetMaxBaseItemRowCount() const;

	//设置 BaseItem 集合中最大的行数，类自动调用，请不要手工调用
	void SetMaxBaseItemRowCount(int iMaxRowCount);

	//获得 pBaseItem 在 BaseItem 集合中的索引号,若没有, 返回 -1
	int GetBaseItemIndex(CXHGridViewBaseItem *pBaseItem) const;

	//获得索 BaseItem 集合中从 iIndexStart 开始的 iCount 个 BaseItem 宽度的总和,若 iCount = -1 ，则返回从 iIndexStart 开始的所有 BaseItem 宽度之和
	int GetBaseItemWidth(int iIndexStart,int iCount = 1) const;

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	void Draw(CDC *pDC);

	//从目标 BaseItems 复制整个 BaseItems 结构，不共享对象，重新生成新的对象
	void CopyFrom(CXHGridViewBaseItems *pBaseItems);

	int GetMaxBaseItemLeafCount();	//See CXHGridViewItemStructure for details

	CXHGridViewLeafItem* FindLeafByID(int iLeafID);

protected:
	CArray <CXHGridViewBaseItem*,CXHGridViewBaseItem*> m_arrpBaseItems;
	CXHGridViewItemStructure *m_pParentStructure;
	_XHGRIDVIEWBASEITEMSINFO _m_BaseItemsInfo;
};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWITEMSTRUCTURE
{
	int iItemType;		//对象类型：Band / Header / Item
	int iBaseID;
	DWORD dwState;
	CXHGridViewLeafItem *pLastFocusedLeafItem;
	DWORD dwItemData;
	DWORD dwReportNumber;
	int iSelectSession;
}_XHGRIDVIEWITEMSTRUCTURE;

class CXHGridViewItemStructure
{
public:
	CXHGridViewItemStructure();
	virtual ~CXHGridViewItemStructure();

public:
	CXHGridViewBaseItem* InsertBaseItem(int iIndex,int iWidth);		//Main band /
	CXHGridViewSubItem* InsertSubItem(int iBase,int iSub);		//Band row /
	CXHGridViewLeafItem* InsertLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex,LPCTSTR lpszText,int iWidth = DEFAULT_BAND_WIDTH);		//Sub band /

	CXHGridViewBaseItem* GetBaseItem(int iIndex);		//Main band /
	CXHGridViewSubItem* GetSubItem(int iBase,int iSub);		//Band row /
	CXHGridViewLeafItem* GetLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex);		//Sub band /

	void SetParentGridView(CXHGridView *pParent);
	CXHGridView* GetParentGridView();

	//重新生成底层叶子节点列表。
	void RearrangeBottomLeafList();

	//重新生成所有叶子节点列表。请在转换到 Report 模式时调用
	void RearrangeLeafList();

	//获得整个结构的高度(含线宽),此高度根据 GridView 的类型不同而不同
	int GetHeight() const;

	//获得整个结构的宽度(含线宽),此宽度根据 GridView 的类型不同而不同
	int GetWidth();

	int GetBaseItemCount() const;

	//以下 5 个函数中，point 均为 ItemStructure 全局坐标
	CXHGridViewBaseItem* BaseHitTest(POINT point);
	CXHGridViewLeafItem* LeafHitTest(POINT point);
	BOOL BaseSideHitTest(CXHGridViewBaseItem **ppBaseItemLeft,CXHGridViewBaseItem **ppBaseItemRight,CPoint point);
	BOOL LeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point);
	BOOL BotomLeafSideHitTest(CXHGridViewLeafItem **ppLeafItemLeft,CXHGridViewLeafItem **ppLeafItemRight,CPoint point);

	void SetItemType(int iNewType);
	int GetItemType() const;

	BOOL SetBand(CXHGridViewItemStructure *pBandItem);
	CXHGridViewItemStructure* GetBand();
	int GetLeafListSize() const;

	//是否有 Band 对象与此对象关联
	BOOL IsBand();

	//获得映射到 Band 的 BaseItem 的宽度
	int GetMapedBaseItemWidth(const CXHGridViewBaseItem *pKey) const;

	void Draw(CDC *pDC,int x,int y);
	void Draw(CDC *pDC,POINT point);

	//获得整个 ItemStructure 的矩形,采用基于 ItemStructure 自身的坐标,也就是说, left 和 top 都为 0
	void GetItemRect(LPRECT lpRect);

	//获得绘图对象
	CXHGridViewLeafDrawHelper* GetDrawer();

	//获得整个 ItemStructure 在 Detail 模式下的高度，无论此时，该 ItemStructure 的状态如何，返回 Detail 模式下的高度 (不含线宽)
	int GetFixHeight() const;

	//绘制最左边的标记条
	void DrawMarker(CDC *pDC);

	//以当前 ItemStructure 的结构为标准，产生一个新的 ItemStructure
	CXHGridViewItemStructure* CloneStructure();

	//通过 ID 号来查找 LeafItem
	CXHGridViewLeafItem* GetLeafByID(int iLeafID);

	//Generate a new ID
	int NewID();

	//重新设置 LeafItem 的值，可以设置为其它类型
	void SetLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex,CXHGridViewLeafItem *pNew);

	//获得每个 SubItem 中含有 LeafItem 最多的个数,若指定了 pBaseItemRoot ,则仅从 pBaseItemRoot 处开始查找
	int GetMaxBaseItemLeafCount(CXHGridViewBaseItem *pBaseItemRoot = NULL);

	void ModifyState(DWORD dwRemove,DWORD dwAdd);
	DWORD GetState();


	CXHGridViewLeafItem* GetLastFocusedLeafItem();
	void SetLastFocusedLeafItem(CXHGridViewLeafItem *pLeafFocused);

	void SetItemData(DWORD dwData);
	DWORD GetItemData() const;

	BOOL IsFocused() const;

	BOOL MarkSelected(int iSelectSession);
	BOOL MarkUnselected(int iSelectSession);
	BOOL ChangeSelected(int iSelectSession);
	BOOL IsSelected();

	DWORD GetStatus() const;

	//根据当前 SelectSession 的值来重新设置 Flags (Selected)
	BOOL RefreshSelected(int iSelectSession);

protected:
	CXHGridViewLeafItem *Detail_LeafHitTest(POINT point);
	CXHGridViewLeafItem *Report_LeafHitTest(POINT point);
	
	//重置 BaseItem 到 LeafItem 的映射，并重新设置所有 BaseItem 的宽度
	BOOL ResetBandMap();

	//获得所有叶子节点宽度的总和。
	int GetAllLeafTotalWidth();

	void DrawItemStructure(CDC *pDC);
	void DrawReport(CDC *pDC);

protected:
	_XHGRIDVIEWITEMSTRUCTURE _m_ItemStructure;
	CArray <CXHGridViewLeafItem*,CXHGridViewLeafItem*> m_arrpBottomLeafItems;
	CArray <CXHGridViewLeafItem*,CXHGridViewLeafItem*> m_arrpLeafItems;
	CXHGridViewBaseItems m_BaseItems;
	CXHGridView *m_pParentGridView;

protected:
	CXHGridViewItemStructure *m_pBand;	//当类型为 HEADER 时，此成员有效。指向 Header 对应的 Band 对象。

	//从 BaseItem 到 LeafItem 的映射。当对象本身用作 Header 并且设置了 Band 对象时，需要这个映射来确定 BaseItem 的宽度
	CMap <const CXHGridViewBaseItem*,const CXHGridViewBaseItem*,CXHGridViewLeafItem*,CXHGridViewLeafItem*> m_mapToBand;

	CXHGridViewLeafDrawHelper *m_pDrawer;
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewItem  
{
public:
	CXHGridViewItem();
	virtual ~CXHGridViewItem();

};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWBANDCTRL
{
	CXHGridViewBaseItem *pPreHitSideBaseItem;
	CXHGridViewLeafItem *pPreHitSideBottomLeafItem;
	CXHGridViewLeafItem *pPreHitSideBottomLeafItemRight;
	POINT ptLButtonDown;
	POINT ptPreMove;
	BOOL bLButtonPress;
}_XHGRIDVIEWBANDCTRL;

class CXHGridViewBandCtrl:public CWnd
{
public:
	CXHGridViewBandCtrl();
	virtual ~CXHGridViewBandCtrl();
public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	CXHGridViewItemStructure* GetBandItemStructure();

	//if invisible,0 will be returned
	int GetHeight();

	int GetWidth();

	CXHGridView* GetParentGirdView();

protected:
	_XHGRIDVIEWBANDCTRL _m_BandCtrlInfo;
	CXHGridViewItemStructure m_BandItem;

protected:
	
	afx_msg void OnLButtonDown( UINT, CPoint );
	afx_msg void OnLButtonUp( UINT, CPoint );
	afx_msg void OnPaint( );
	afx_msg void OnMouseMove( UINT, CPoint );

	DECLARE_MESSAGE_MAP()

public:
	void ResetWindowSize();

};

//////////////////////////////////////////////////////////////////////////

typedef struct tag_XHGRIDVIEWHEADERCTRL
{
	CXHGridViewLeafItem *pPreHitLeafItem;
	CXHGridViewLeafItem *pPreHitSideLeafLeft;
	CXHGridViewLeafItem *pPreHitSideLeafRight;
	int iBandHeight;
	POINT ptPreMove;
	POINT ptLButtonDown;
	BOOL bLButtonPress;
}_XHGRIDVIEWHEADERCTRL;

class CXHGridViewHeaderCtrl:public CWnd
{
public:
	CXHGridViewHeaderCtrl();
	virtual ~CXHGridViewHeaderCtrl();
public:
	BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd);
	CXHGridViewItemStructure* GetHeaderItemStructure();
	CXHGridViewItemStructure* GetBandItemStructure();
	BOOL IsBandVisible();

	//Get the full height of the HeaderCtrl,including band control if banded
	int GetHeight();

	BOOL Band();
	int GetBandHeight();	//If not banded,return 0

	void ResetWindowSize();
	int GetWidth();
	
protected:
	_XHGRIDVIEWHEADERCTRL _m_HeaderCtrlInfo;
	CXHGridViewItemStructure m_HeaderItem;
	
protected:
	
	afx_msg void OnLButtonDown( UINT, CPoint );
	afx_msg void OnLButtonUp( UINT, CPoint );
	afx_msg void OnPaint( );
	afx_msg void OnMouseMove( UINT, CPoint );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

protected:
	CXHGridViewBandCtrl m_BandCtrl;
	CXHGridView *m_pParentGridView;

private:
	void ReCalcLocation();
	void ProcessMouseDown(CPoint point);
	void ProcessMouseMove(CPoint point);
	void ProcessMouseUp(CPoint point);

};

//////////////////////////////////////////////////////////////////////////

typedef struct tagXHGRIDVIEW
{
	DWORD dwFlags;
	int iLineWidth;
	int iRowHeight;
	int iLeafBorderWidth;
	int iMarkWidth;
	int iItemSpacing;	//Space between items
	int iMinLeafWidth;
	int iViewType;
	CXHGridViewItemStructure *pFocusedItem;
	COLORREF clrBackground;
	COLORREF clrLineColor;
	DWORD dwGridViewGlobalData;
	DWORD dwReportNumber;	//This is for performance improvement
	int iFocusedItemIndex;
	POINT ptDownPoint;
	POINT ptPreMovePoint;
	DWORD dwStatus;
	int iStartDragItemIndex;
	int iSelectSession;
	int iEndDragItemIndex;
	int iFirstSelectedItemIndex;
	int iLastSelectedItemIndex;
	int iSelectedCount;
	int iShiftStart;
	int iPreShiftEnd;
	CXHGridViewLeafItem *pMouseOverLeaf;
	int iOldMouseOverItem;
}XHGRIDVIEW,*LPXHGRIDVIEW;

typedef int (__cdecl *PFNGRIDVIEWCOMPARE)(void *pUserData, CXHGridViewItemStructure **ppItem1,CXHGridViewItemStructure **ppItem2);

class CXHGridView : public CWnd  
{
public:
	CXHGridView();
	virtual ~CXHGridView();

public:

	BOOL Create(LPCTSTR lpszWindowName, const RECT& rect, CWnd* pParentWnd, BOOL bBorder, UINT nID);

	int GetLineWidth() const;
	int GetRowHeight() const;
	void SetRowHeight(int iNewHeight);

	//获得 GridView 的当前模式
	int GetViewType() const;

	//设置 GridView 的当前模式
	BOOL SetViewType(int iViewType);

	int GetLeafBorderWidth() const;
	int GetMarkWidth() const;
	int GetMinLeafWidth() const;
	void SetImageList(CImageList *pImageList);
	CImageList* GetImageList();
	CXHGridViewHeaderCtrl* GetHeaderCtrl();
	CXHGridViewItemStructure* GetHeaderItemStructure();
	CXHGridViewItemStructure* GetBandItemStructure();
	CXHGridViewItemStructure* GetItem(int iItemIndex);
	CXHGridViewItemStructure* CreateNewItem();

	//追加一条记录
	CXHGridViewItemStructure* InsertNewItem(int iIndex);
	CXHGridViewItemStructure* InsertNewItem(int iIndex, CXHGridViewItemStructure *pNewItem);

	//删除所有记录
	void DeleteAllItems();

	//获得目前在可视范围内的记录 (index)
	int GetVisibleItems(int *piStart,int *piEnd);

	//计算并重新设置 H-ScrollBar 和 V-ScrollBar 参数
	void ResetHScrollBar();
	void ResetVScrollBar();

	//重新绘制所有的可视记录
	void RedrawVisibleItems(CDC *pDC);

	//获得可用于绘制记录的区域(客户区域)的高度
	int GetItemViewHeight();

	//获得可用于绘制记录的区域(客户区域)的大小。
	void GetItemViewRect(LPRECT lprcRect);

	//重新绘制未使用区域，即未被 Band , Header 或 Item 占用的区域
	void RedrawUnusedArea();

	//测试点击的记录号, point 采用整个 GridView 的坐标系
	int ItemHitTest(POINT point,BOOL bOutOfScale = FALSE);

	//重新绘制从 iItemIndexStart 开始的 iCount 个记录
	void RedrawItems(int iItemIndexStart, int iCount = 1);
	void Internal_RedrawItems(int iItemIndexStart, int iCount);

	//记录是否在可视范围内
	BOOL IsItemVisible(int iItemIndex,BOOL bPartial = TRUE);

	//获得记录间的间隔空间。此值根据 GridView 的当前模式不同而不同。
	int GetItemSpacing() const;

	//获得线条颜色
	COLORREF GetLineColor() const;
	//设置线条颜色
	void SetLineColor(COLORREF clrLineColor);

	//获得非用户区背景颜色
	COLORREF GetBackColor() const;
	//设置非用户区背景颜色
	void SetBackColor(COLORREF clrBackColor);

	//排序
	void Sort(PFNGRIDVIEWCOMPARE pSortFunc,DWORD dwUserData);

	//设置或获取一个用户自定义的全局值，该值在整个控件内有效
	void SetGridViewGlobalData(DWORD dwNewData);
	DWORD GetGridViewGlobalData() const
	{
		return m_GridView.dwGridViewGlobalData;
	};

	//获得 pItem 在 GridView 中的 Index , 这个函数是用来实现性能上的优化的，只在可视的记录范围中寻找。若没有找到，返回 -1
	int GetItemIndexSearchVisible(CXHGridViewItemStructure *pItem);

	DWORD ModifyFlags(DWORD dwRemove,DWORD dwAdd);

	//GridView 是否有线条
	BOOL IsGridLines();

	DWORD GetReportNumber() const;

	void EnsureVisible(int iIndex,BOOL bPartialOK = FALSE);

	int GetCurrentSelectSession() const;

	int GetFirstSelectedItemIndex();
	int GetSelectedItemCount() const;
	int GetNextSelectedItemIndex(int &pos);

	BOOL ClientToItem(int iItemIndex,LPPOINT lpPoint,int nCount = 1);
	BOOL ItemToClient(int iItemIndex,LPPOINT lpPoint,int nCount = 1);

	//获得 LeafItem 的区域。采用 GridView 坐标
	BOOL GetLeafRect(int iItemIndex,int iBaseIndex,int iSubIndex,int iLeafIndex,LPRECT lprcArea);

	//获得 Focused Item 的索引，若没有 Focused Item， 返回 -1
	int GetFocusedItemIndex();

	BOOL IsFocused();

	DWORD GetItemData(int iItemIndex);
	BOOL SetItemData(int iItemIndex,DWORD dwItemData);

	BOOL SetItemSelected(int iIndex ,BOOL bSelected = TRUE);
	BOOL IsItemSelected(int iIndex);

	int GetItemCount();


protected:

	CXHGridViewItemStructure* Internal_InsertNewItem(int iIndex, CXHGridViewItemStructure *pNewItem);
	int Internal_DeleteItem(int iIndex);

	void GetHeaderRightArea(LPRECT rcArea);

	//返回值中,低字节最低位为 1 表示 rcAreaR 无效，高字节最低位为 1 表示 rcAreaB 无效
	WORD GetItemUnusedArea(LPRECT rcAreaR,LPRECT rcAreaB);

	// iMouseButton: 1 - Left ; 2 - Right ; 3 - Middle
	void ProcessMouseDown(CPoint point,int iMouseButton);

	// 处理 LeafItem 的 Enter 和 Leave 消息,point 为 GridView 局部坐标
	void ProcessLeafHot(CPoint point);

	//根据选择区域，设置记录的 Selected 标记
	void ProcessDragRect(CPoint point1,CPoint point2,CSize sizeDelta);

	void RedrawVisibleItemsForSelected(int iExclude = -1);

	void IncreaseSelectSession();

	BOOL MarkItemSelected(int iIndex);
	BOOL MarkItemUnselected(int iIndex);
	BOOL ChangeItemSelected(int iIndex);

	//Ctrl 键是否被按下
	BOOL IsControlKeyPressed() const;
	//Shift 键是否被按下
	BOOL IsShiftKeyPressed() const;

protected:
	XHGRIDVIEW m_GridView;
	CImageList *m_pImageList;

	CArray <CXHGridViewItemStructure*,CXHGridViewItemStructure*> m_arrpItems;

protected:
	BOOL SetHeaderFocusLeaf(CXHGridViewLeafItem *pItemLeafItem);
	
	//线性搜索给定 ItemStructure 的索引号。在 Sort(排序) 之后重新确定 Focused Item 的索引时，必须使用这种方法
	int GetItemIndex(CXHGridViewItemStructure *pItem);

	//移动 Focus 标志
	BOOL MoveFocus(int iIndexDelta);

	//绘制一个 XOR_PEN 的 Dot 矩形框
	void DrawDragRect(POINT point1, POINT point2);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg virtual void OnItemClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit);
	afx_msg virtual void OnItemRClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit);
	afx_msg virtual void OnItemDblClick(int iItemIndex, CXHGridViewLeafItem *pLeafHit);
	afx_msg virtual void OnHeaderClick(CXHGridViewLeafItem *pClickedLeafItem);
	afx_msg LRESULT OnSpecialMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	afx_msg LRESULT OnInsertNewItem(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedrawItems(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDeleteItem(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CXHGridViewHeaderCtrl m_HeaderCtrl;

private:

public:
	static CXHGridViewLeafDrawHelperBand s_BandDrawer;
	static CXHGridViewLeafDrawHelperHeader s_HeaderDrawer;
	static CXHGridViewLeafDrawHelperItem s_ItemDrawer;

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewLeafDrawHelper
{
public:
	CXHGridViewLeafDrawHelper();
	virtual ~CXHGridViewLeafDrawHelper();
public:
	virtual void Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem);
	virtual void DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure);
	virtual void DrawLine(CXHGridView *pGridView,CDC *pDC,int x1,int y1,int x2,int y2);
protected:
private:
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewLeafDrawHelperBand:public CXHGridViewLeafDrawHelper
{
public:
	CXHGridViewLeafDrawHelperBand();
	virtual ~CXHGridViewLeafDrawHelperBand();
public:
	virtual void Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem);
	virtual void DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure);
protected:
private:
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewLeafDrawHelperHeader:public CXHGridViewLeafDrawHelper
{
public:
	CXHGridViewLeafDrawHelperHeader();
	virtual ~CXHGridViewLeafDrawHelperHeader();
public:
	virtual void Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem);
	virtual void DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure);
	virtual void DrawLine(CXHGridView *pGridView,CDC *pDC,int x1,int y1,int x2,int y2);
protected:
private:
};

//////////////////////////////////////////////////////////////////////////

class CXHGridViewLeafDrawHelperItem:public CXHGridViewLeafDrawHelper
{
public:
	CXHGridViewLeafDrawHelperItem();
	virtual ~CXHGridViewLeafDrawHelperItem();
public:
	virtual void Draw(CDC *pDC,CXHGridViewLeafItem *pLeafItem);
	virtual void DrawMarker(CDC *pDC,CXHGridViewItemStructure *pItemStructure);
protected:
private:
};


#endif // !defined(AFX_XHGRIDVIEWITEM_H__DC8B13E2_6921_4CB1_A29C_5965C5042FF6__INCLUDED_)
