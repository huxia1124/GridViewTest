
// GridViewTestDlg.h : 头文件
//

#pragma once
#include "XHGridViewCtrl.h"
#include "STXAnchor.h"

int __cdecl SortFunc(void *pUserData, CXHGridViewItemStructure **p1,CXHGridViewItemStructure **p2);

class CMyGridView:public CXHGridView
{
protected:
	afx_msg void OnHeaderClick(CXHGridViewLeafItem *pClickedLeafItem)
	{
		SetGridViewGlobalData(pClickedLeafItem->GetID());
		Sort(SortFunc,0); 
	}
	afx_msg void OnItemDblClick(int iIndex, CXHGridViewLeafItem *pClickedLeafItem);
};

// CGridViewTestDlg 对话框
class CGridViewTestDlg : public CDialog
{
// 构造
public:
	CGridViewTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GRIDVIEWTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HANDLE m_hThread;

public:

	CMyGridView m_GridView;
	CSTXAnchor *m_pAnchor;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonChange();
};
