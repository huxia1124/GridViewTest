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

	//��� LeafItem ���������򣬲��� LeafItem ����ϵ
	void GetLeafRect(LPRECT lprcRect);

	//��� LeafItem �Ŀɻ�ͼ���򣬲��� LeafItem ����ϵ���˷����� GetLeafRect �Ļ����ϳ�ȥ���߿�
	void GetLeafDrawingRect(LPRECT lprcRect);

	//��� LeafItem �ĳ�ȥ����ͼ�������򣬲��� LeafItem ����ϵ
	void GetLeafClientRect(LPRECT lprcRect);

	//��� LeafItem �ĳ�ȥ����ͼ�����������ǻ������ Detail Mode �µ����򡣲��� LeafItem ����ϵ
	void GetLeafClientFixedRect(LPRECT lprcRect);

	//��� LeafItem �����ͼ�������򣬲��� LeafItem ����ϵ
	BOOL GetLeafImageRect(LPRECT lprcRect);

	//��� LeafItem �� Sort Image ͼ��������(�ұ�)������ LeafItem ����ϵ
	BOOL GetLeafSortImageRect(LPRECT lprcRect);
	
	DWORD GetState() const;

	void ModifyState(DWORD dwRemove,DWORD dwAdd);

	//���� LeafItem ��ʹ�õ�ͼƬ�� ImageList �е�ͼƬ������ -1 ��ʾ��ʹ��ͼƬ
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
	//�¼�����
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
	CXHGridViewLeafItem* LeafHitTest(POINT point);	//point: SubItem �еľֲ�����,�� X ��Ч
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

	//���� _m_LeafItemsInfo �� iTotalWidth ��ֵ��ͨ���� LeafItem ����
	void AddTotalWidth(int iAdd);

	//������ LeafItem �� Width ��ת��Ϊʵ��������
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
	CXHGridViewLeafItem* LeafHitTest(POINT point);	//point: SubItem �еľֲ�����,�� X ��Ч

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	int GetHeight();
	void Draw(CDC *pDC);

	//�Ե�ǰ SubItem �Ľṹ����һ���µ� SubItem
	CXHGridViewSubItem* CloneSubItem();

	int GetLeafCount();

	void SetLeafItem(int iLeafIndex,CXHGridViewLeafItem *pNew);

	CXHGridViewLeafItem* FindLeafByID(int iLeafID);

protected:
	//��� SubItem ������SubItem �ֲ�����(��left��top����0)
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

	//��� BaseItem ��ʵ�ʿ�ȡ�
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

	//�Ե�ǰ BaseItem �Ľṹ����һ���µ� BaseItem
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
	CXHGridViewBaseItem* BaseHitTest(POINT point);		//point ����������
	CXHGridViewSubItem* SubItemHitTest(POINT point);
	void DeleteAllBaseItems();

	//��� BaseItem ��������������
	int GetMaxBaseItemRowCount() const;

	//���� BaseItem �������������������Զ����ã��벻Ҫ�ֹ�����
	void SetMaxBaseItemRowCount(int iMaxRowCount);

	//��� pBaseItem �� BaseItem �����е�������,��û��, ���� -1
	int GetBaseItemIndex(CXHGridViewBaseItem *pBaseItem) const;

	//����� BaseItem �����д� iIndexStart ��ʼ�� iCount �� BaseItem ��ȵ��ܺ�,�� iCount = -1 ���򷵻ش� iIndexStart ��ʼ������ BaseItem ���֮��
	int GetBaseItemWidth(int iIndexStart,int iCount = 1) const;

	void RearrangeBottomLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);
	void RearrangeLeafList(CArray<CXHGridViewLeafItem*,CXHGridViewLeafItem*> *pLeafList);

	void Draw(CDC *pDC);

	//��Ŀ�� BaseItems �������� BaseItems �ṹ��������������������µĶ���
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
	int iItemType;		//�������ͣ�Band / Header / Item
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

	//�������ɵײ�Ҷ�ӽڵ��б�
	void RearrangeBottomLeafList();

	//������������Ҷ�ӽڵ��б�����ת���� Report ģʽʱ����
	void RearrangeLeafList();

	//��������ṹ�ĸ߶�(���߿�),�˸߶ȸ��� GridView �����Ͳ�ͬ����ͬ
	int GetHeight() const;

	//��������ṹ�Ŀ��(���߿�),�˿�ȸ��� GridView �����Ͳ�ͬ����ͬ
	int GetWidth();

	int GetBaseItemCount() const;

	//���� 5 �������У�point ��Ϊ ItemStructure ȫ������
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

	//�Ƿ��� Band ������˶������
	BOOL IsBand();

	//���ӳ�䵽 Band �� BaseItem �Ŀ��
	int GetMapedBaseItemWidth(const CXHGridViewBaseItem *pKey) const;

	void Draw(CDC *pDC,int x,int y);
	void Draw(CDC *pDC,POINT point);

	//������� ItemStructure �ľ���,���û��� ItemStructure ���������,Ҳ����˵, left �� top ��Ϊ 0
	void GetItemRect(LPRECT lpRect);

	//��û�ͼ����
	CXHGridViewLeafDrawHelper* GetDrawer();

	//������� ItemStructure �� Detail ģʽ�µĸ߶ȣ����۴�ʱ���� ItemStructure ��״̬��Σ����� Detail ģʽ�µĸ߶� (�����߿�)
	int GetFixHeight() const;

	//��������ߵı����
	void DrawMarker(CDC *pDC);

	//�Ե�ǰ ItemStructure �ĽṹΪ��׼������һ���µ� ItemStructure
	CXHGridViewItemStructure* CloneStructure();

	//ͨ�� ID �������� LeafItem
	CXHGridViewLeafItem* GetLeafByID(int iLeafID);

	//Generate a new ID
	int NewID();

	//�������� LeafItem ��ֵ����������Ϊ��������
	void SetLeafItem(int iBaseIndex,int iSubIndex,int iLeafIndex,CXHGridViewLeafItem *pNew);

	//���ÿ�� SubItem �к��� LeafItem ���ĸ���,��ָ���� pBaseItemRoot ,����� pBaseItemRoot ����ʼ����
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

	//���ݵ�ǰ SelectSession ��ֵ���������� Flags (Selected)
	BOOL RefreshSelected(int iSelectSession);

protected:
	CXHGridViewLeafItem *Detail_LeafHitTest(POINT point);
	CXHGridViewLeafItem *Report_LeafHitTest(POINT point);
	
	//���� BaseItem �� LeafItem ��ӳ�䣬�������������� BaseItem �Ŀ��
	BOOL ResetBandMap();

	//�������Ҷ�ӽڵ��ȵ��ܺ͡�
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
	CXHGridViewItemStructure *m_pBand;	//������Ϊ HEADER ʱ���˳�Ա��Ч��ָ�� Header ��Ӧ�� Band ����

	//�� BaseItem �� LeafItem ��ӳ�䡣������������ Header ���������� Band ����ʱ����Ҫ���ӳ����ȷ�� BaseItem �Ŀ��
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

	//��� GridView �ĵ�ǰģʽ
	int GetViewType() const;

	//���� GridView �ĵ�ǰģʽ
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

	//׷��һ����¼
	CXHGridViewItemStructure* InsertNewItem(int iIndex);
	CXHGridViewItemStructure* InsertNewItem(int iIndex, CXHGridViewItemStructure *pNewItem);

	//ɾ�����м�¼
	void DeleteAllItems();

	//���Ŀǰ�ڿ��ӷ�Χ�ڵļ�¼ (index)
	int GetVisibleItems(int *piStart,int *piEnd);

	//���㲢�������� H-ScrollBar �� V-ScrollBar ����
	void ResetHScrollBar();
	void ResetVScrollBar();

	//���»������еĿ��Ӽ�¼
	void RedrawVisibleItems(CDC *pDC);

	//��ÿ����ڻ��Ƽ�¼������(�ͻ�����)�ĸ߶�
	int GetItemViewHeight();

	//��ÿ����ڻ��Ƽ�¼������(�ͻ�����)�Ĵ�С��
	void GetItemViewRect(LPRECT lprcRect);

	//���»���δʹ�����򣬼�δ�� Band , Header �� Item ռ�õ�����
	void RedrawUnusedArea();

	//���Ե���ļ�¼��, point �������� GridView ������ϵ
	int ItemHitTest(POINT point,BOOL bOutOfScale = FALSE);

	//���»��ƴ� iItemIndexStart ��ʼ�� iCount ����¼
	void RedrawItems(int iItemIndexStart, int iCount = 1);
	void Internal_RedrawItems(int iItemIndexStart, int iCount);

	//��¼�Ƿ��ڿ��ӷ�Χ��
	BOOL IsItemVisible(int iItemIndex,BOOL bPartial = TRUE);

	//��ü�¼��ļ���ռ䡣��ֵ���� GridView �ĵ�ǰģʽ��ͬ����ͬ��
	int GetItemSpacing() const;

	//���������ɫ
	COLORREF GetLineColor() const;
	//����������ɫ
	void SetLineColor(COLORREF clrLineColor);

	//��÷��û���������ɫ
	COLORREF GetBackColor() const;
	//���÷��û���������ɫ
	void SetBackColor(COLORREF clrBackColor);

	//����
	void Sort(PFNGRIDVIEWCOMPARE pSortFunc,DWORD dwUserData);

	//���û��ȡһ���û��Զ����ȫ��ֵ����ֵ�������ؼ�����Ч
	void SetGridViewGlobalData(DWORD dwNewData);
	DWORD GetGridViewGlobalData() const
	{
		return m_GridView.dwGridViewGlobalData;
	};

	//��� pItem �� GridView �е� Index , �������������ʵ�������ϵ��Ż��ģ�ֻ�ڿ��ӵļ�¼��Χ��Ѱ�ҡ���û���ҵ������� -1
	int GetItemIndexSearchVisible(CXHGridViewItemStructure *pItem);

	DWORD ModifyFlags(DWORD dwRemove,DWORD dwAdd);

	//GridView �Ƿ�������
	BOOL IsGridLines();

	DWORD GetReportNumber() const;

	void EnsureVisible(int iIndex,BOOL bPartialOK = FALSE);

	int GetCurrentSelectSession() const;

	int GetFirstSelectedItemIndex();
	int GetSelectedItemCount() const;
	int GetNextSelectedItemIndex(int &pos);

	BOOL ClientToItem(int iItemIndex,LPPOINT lpPoint,int nCount = 1);
	BOOL ItemToClient(int iItemIndex,LPPOINT lpPoint,int nCount = 1);

	//��� LeafItem �����򡣲��� GridView ����
	BOOL GetLeafRect(int iItemIndex,int iBaseIndex,int iSubIndex,int iLeafIndex,LPRECT lprcArea);

	//��� Focused Item ����������û�� Focused Item�� ���� -1
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

	//����ֵ��,���ֽ����λΪ 1 ��ʾ rcAreaR ��Ч�����ֽ����λΪ 1 ��ʾ rcAreaB ��Ч
	WORD GetItemUnusedArea(LPRECT rcAreaR,LPRECT rcAreaB);

	// iMouseButton: 1 - Left ; 2 - Right ; 3 - Middle
	void ProcessMouseDown(CPoint point,int iMouseButton);

	// ���� LeafItem �� Enter �� Leave ��Ϣ,point Ϊ GridView �ֲ�����
	void ProcessLeafHot(CPoint point);

	//����ѡ���������ü�¼�� Selected ���
	void ProcessDragRect(CPoint point1,CPoint point2,CSize sizeDelta);

	void RedrawVisibleItemsForSelected(int iExclude = -1);

	void IncreaseSelectSession();

	BOOL MarkItemSelected(int iIndex);
	BOOL MarkItemUnselected(int iIndex);
	BOOL ChangeItemSelected(int iIndex);

	//Ctrl ���Ƿ񱻰���
	BOOL IsControlKeyPressed() const;
	//Shift ���Ƿ񱻰���
	BOOL IsShiftKeyPressed() const;

protected:
	XHGRIDVIEW m_GridView;
	CImageList *m_pImageList;

	CArray <CXHGridViewItemStructure*,CXHGridViewItemStructure*> m_arrpItems;

protected:
	BOOL SetHeaderFocusLeaf(CXHGridViewLeafItem *pItemLeafItem);
	
	//������������ ItemStructure �������š��� Sort(����) ֮������ȷ�� Focused Item ������ʱ������ʹ�����ַ���
	int GetItemIndex(CXHGridViewItemStructure *pItem);

	//�ƶ� Focus ��־
	BOOL MoveFocus(int iIndexDelta);

	//����һ�� XOR_PEN �� Dot ���ο�
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
