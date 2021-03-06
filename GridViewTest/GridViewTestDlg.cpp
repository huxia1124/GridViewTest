
// GridViewTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GridViewTest.h"
#include "GridViewTestDlg.h"

#include "XHBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////////

int __cdecl SortFunc(void *pUserData, CXHGridViewItemStructure **p1,CXHGridViewItemStructure **p2)
{
	return _tcscmp((*p1)->GetLeafByID((*p1)->GetParentGridView()->GetGridViewGlobalData())->GetText(),(*p2)->GetLeafByID((*p2)->GetParentGridView()->GetGridViewGlobalData())->GetText());
}

BOOL g_bTerminateEnum = FALSE;

typedef BOOL (CALLBACK*PFNENUMFILEPROC)(LPCTSTR, DWORD_PTR);

BOOL CALLBACK FileEnumFunc(LPCTSTR lpszFile, DWORD_PTR dwUserData)
{
	if(g_bTerminateEnum)
		return FALSE;

	CString strFileName = lpszFile;

	CString strExt = strFileName.Right(4);
	strExt.MakeUpper();
	int iType = 0;

	if(strExt == _T(".JPG"))
		iType = 1;
	else if(strExt == _T(".PNG"))
		iType = 2;
	else if(strExt == _T(".GIF"))
		iType = 3;
	else if(strExt == _T(".BMP"))
		iType = 4;

	if(iType == 0)
		return TRUE;

	COLORREF clrItem = GetSysColor(COLOR_WINDOW);
	switch(iType)
	{
	case 1: clrItem = RGB(255,192,192); break;
	case 2: clrItem = RGB(192,192,255); break;
	case 3: clrItem = RGB(192,255,192); break;
	case 4: clrItem = RGB(192,128,192); break;
	}

	CGridViewTestDlg *pDlg = (CGridViewTestDlg*)dwUserData;

	int i = pDlg->m_GridView.GetItemCount();
	//int i = 0;
	//CXHGridViewItemStructure *pNewItem = pDlg->m_GridView.InsertNewItem(i);

	CXHGridViewItemStructure *pNewItem = pDlg->m_GridView.CreateNewItem();

	CFileStatus fs;
	CString strTemp;
	CImage img;
	if (SUCCEEDED(img.Load(lpszFile)))
	{
		BOOL bResize = FALSE;
		CSize sizeBmp(img.GetWidth(), img.GetHeight());
		if (img.GetWidth() > 192 || img.GetHeight() > 192)
			bResize = TRUE;

		HBITMAP hBitmap = img.Detach();

		CXHBitmap bmp;
		bmp.Attach(hBitmap);

		if (bResize)
			bmp.Resize(128, 128);

		strTemp.Format(_T("%d x %d"), sizeBmp.cx, sizeBmp.cy);
		pNewItem->GetLeafItem(2, 1, 0)->SetText(strTemp);
		pNewItem->SetLeafItem(1, 0, 0, new CXHGridViewLeafItemImage((HBITMAP)bmp.Detach()));

	}
	else
	{
		strTemp.Format(_T("Error"));
		pNewItem->GetLeafItem(2, 1, 0)->SetText(strTemp);
	}

	CFile::GetStatus(lpszFile,fs);
	strTemp.Format(_T("%.1f k"),(double)fs.m_size/1024);
	pNewItem->GetLeafItem(2,0,1)->SetText(strTemp);
	pNewItem->GetLeafItem(2,2,0)->SetText(lpszFile);
	
	strTemp.Format(_T("%d"), rand());
	pNewItem->GetLeafItem(2,0,0)->SetText(strTemp);
	strTemp.Format(_T("%d"), pDlg->m_GridView.GetItemCount());
	pNewItem->GetLeafItem(0,0,0)->SetText(strTemp);

	pNewItem->GetLeafItem(0, 1, 0)->SetBackColor(clrItem);
	pNewItem->GetLeafItem(0, 1, 0)->SetText(strExt);

	pDlg->m_GridView.InsertNewItem(i, pNewItem);
	//pDlg->m_GridView.RedrawItems(i);

	return TRUE;
}

void EnumFiles(LPCTSTR lpszFilter, PFNENUMFILEPROC pfnEnumFunc, DWORD_PTR dwUserData)
{
	if(pfnEnumFunc == NULL)
		return;

	TCHAR szStartPath[MAX_PATH];
	int nLen = 0;
	LPCTSTR pszLastFlash = _tcsrchr(lpszFilter, _T('\\'));
	LPCTSTR pszFilterOnly = _T("*");
	if(pszLastFlash != NULL)
	{
		pszFilterOnly = pszLastFlash + 1;
		_tcsncpy_s(szStartPath, MAX_PATH, lpszFilter, pszFilterOnly - lpszFilter);
		nLen = _tcslen(szStartPath);
	}
	else
	{
		_tcscpy_s(szStartPath, MAX_PATH, lpszFilter);
		nLen = _tcslen(szStartPath);
		if(nLen > 0 && szStartPath[nLen - 1] != _T('\\'))
		{
			szStartPath[nLen] =  _T('\\');
			nLen++;
		}
	}

	TCHAR szTempPathName[MAX_PATH];
	_tcscpy_s(szTempPathName, MAX_PATH, szStartPath);
	LPTSTR pszTempPathNameFileName = szTempPathName + nLen;


	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	TCHAR szStartFilter[MAX_PATH];
	_tcscpy_s(szStartFilter, MAX_PATH, szStartPath);
	_tcscat_s(szStartFilter, MAX_PATH, pszFilterOnly);
	TCHAR szSubFilter[MAX_PATH];

	// Find the first file in the directory.
	hFind = FindFirstFile(szStartFilter, &FindFileData);

	if (hFind == INVALID_HANDLE_VALUE) 
	{
		_tprintf (TEXT("Invalid file handle. Error is %u.\n"), GetLastError());
		return;
	} 
	else 
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(_tcscmp(FindFileData.cFileName, _T(".")) != 0 && _tcscmp(FindFileData.cFileName, _T("..")))
			{
				_tcscpy_s(szSubFilter, MAX_PATH, szStartPath);
				_tcscat_s(szSubFilter, MAX_PATH, FindFileData.cFileName);
				_tcscat_s(szSubFilter, MAX_PATH, _T("\\"));
				_tcscat_s(szSubFilter, MAX_PATH, pszFilterOnly);
				EnumFiles(szSubFilter, pfnEnumFunc, dwUserData);
			}
		}
		else
		{
			_tcscpy_s(pszTempPathNameFileName, MAX_PATH - nLen, FindFileData.cFileName);
			if(!pfnEnumFunc(szTempPathName, dwUserData))
			{
				FindClose(hFind);
				return;
			}
		}

		// List all the other files in the directory.
		while (FindNextFile(hFind, &FindFileData) != 0) 
		{
			if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(_tcscmp(FindFileData.cFileName, _T(".")) != 0 && _tcscmp(FindFileData.cFileName, _T("..")))
				{
					_tcscpy_s(szSubFilter, MAX_PATH, szStartPath);
					_tcscat_s(szSubFilter, MAX_PATH, FindFileData.cFileName);
					_tcscat_s(szSubFilter, MAX_PATH, _T("\\"));
					_tcscat_s(szSubFilter, MAX_PATH, pszFilterOnly);
					EnumFiles(szSubFilter, pfnEnumFunc, dwUserData);
				}
			}
			else
			{
				_tcscpy_s(pszTempPathNameFileName, MAX_PATH - nLen, FindFileData.cFileName);
				if(!pfnEnumFunc(szTempPathName, dwUserData))
				{
					FindClose(hFind);
					return;
				}
			}
		}
		FindClose(hFind);
	}
}


UINT WINAPI ListFileThreadProc(LPVOID lpParameter)
{
	srand((UINT)time(NULL));

	TCHAR chWinDir[MAX_PATH];
	GetWindowsDirectory(chWinDir,MAX_PATH); 
	EnumFiles(CString(chWinDir) + _T("\\*.*"), FileEnumFunc, (DWORD_PTR)lpParameter);


	((CGridViewTestDlg*)lpParameter)->GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T("Finished."));


	return 0;

}

void CMyGridView::OnItemDblClick(int iIndex, CXHGridViewLeafItem *pClickedLeafItem)
{
	g_bTerminateEnum = 0;

	CXHGridViewItemStructure *pItem = GetItem(iIndex);
	CXHGridViewLeafItem *pLeaf = pItem->GetLeafItem(2, 2, 0);
	if(pLeaf)
	{
		//ShellExecute(NULL, _T("open"), pLeaf->GetText(), NULL, NULL, SW_SHOW);
	}

	FileEnumFunc(pLeaf->GetText(), (DWORD_PTR)AfxGetMainWnd());
}

//////////////////////////////////////////////////////////////////////////

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGridViewTestDlg 对话框




CGridViewTestDlg::CGridViewTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridViewTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThread = INVALID_HANDLE_VALUE;
	m_pAnchor = NULL;
}

void CGridViewTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGridViewTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CGridViewTestDlg::OnBnClickedButtonStop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CHANGE, &CGridViewTestDlg::OnBnClickedButtonChange)
END_MESSAGE_MAP()


// CGridViewTestDlg 消息处理程序

BOOL CGridViewTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	CRect rcArea;
	GetDlgItem(IDC_STATIC_GRID_VIEW_AREA)->GetWindowRect(rcArea);
	ScreenToClient(rcArea);


	CRect rcClient;
	GetClientRect(rcClient);
	if(!m_GridView.Create(_T("MyGrid"), rcArea,this,TRUE,1002))
	{
		//Error.
	}

	m_pAnchor = new CSTXAnchor(GetSafeHwnd());

	m_pAnchor->AddItem(m_GridView.GetSafeHwnd(), STXANCHOR_ALL);
	m_pAnchor->AddItem(IDC_BUTTON_STOP, STXANCHOR_RIGHT | STXANCHOR_TOP);
	m_pAnchor->AddItem(IDC_BUTTON_CHANGE, STXANCHOR_RIGHT | STXANCHOR_TOP);
	m_pAnchor->AddItem(IDC_STATIC_MESSAGE, STXANCHOR_RIGHT | STXANCHOR_TOP);

	//m_GridView.SetImageList(&g_ImgList);



	CXHGridViewItemStructure *pBand = m_GridView.GetBandItemStructure();

	pBand->InsertBaseItem(0,100)->InsertSubItem(0)->InsertLeafItem(0,_T("Band (0,0,0)"));

	// The line above can be written like this:
	//////////////////////////////////////////////////////////////////////////
	//
	// CXHGridViewBaseItem *pBase0 = pBand->InsertBaseItem(0,100);	// Width = 100px, Height is hard-coded
	// CXHGridViewSubItem *pSub0 =  pBase0->InsertSubItem(0);
	// pSub0->InsertLeafItem(0,_T("Band (0,0,0)"))
	//
	//////////////////////////////////////////////////////////////////////////

	// Because we already have Base Item at index 0, here we call InsertSubItem directly to insert a SubItem into BaseItem 0
	// And there is already an SubItem, we insert the new SubItem after it, use index=1 to specify the location.
	pBand->InsertSubItem(0,1)->InsertLeafItem(0,_T("Band (0,1,0)"));


 	pBand->InsertBaseItem(1,120)->InsertSubItem(0)->InsertLeafItem(0,_T("Band (1,0,0)"));
 	pBand->InsertSubItem(1,1)->InsertLeafItem(0,_T("Band (1,1,0)"));
 	pBand->InsertBaseItem(2,260)->InsertSubItem(0)->InsertLeafItem(0,_T("Band (2,0,0)"));

	//Do remember to insert at least one LeafItem into each SubItem


	CXHGridViewItemStructure *pHeader = m_GridView.GetHeaderItemStructure();
	pHeader->InsertBaseItem(0,100)->InsertSubItem(0)->InsertLeafItem(0,_T("ID"))->SetImageIndex(0);
	pHeader->InsertSubItem(0,1)->InsertLeafItem(0, _T("File Type"));              

	pHeader->InsertBaseItem(1,100)->InsertSubItem(0)->InsertLeafItem(0,_T("Image"))->SetImageIndex(0);

	pHeader->InsertBaseItem(2,170)->InsertSubItem(0)->InsertLeafItem(0,_T("Random Integer"));
	pHeader->InsertLeafItem(2,0,1,_T("Image File Size"));
	pHeader->InsertSubItem(2,1)->InsertLeafItem(0,_T("Image Size"));              
	pHeader->InsertSubItem(2,2)->InsertLeafItem(0,_T("File Name"));
	m_GridView.GetHeaderCtrl()->Band();


	GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T("Searching for images..."));

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, ListFileThreadProc, this, 0, NULL);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGridViewTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGridViewTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGridViewTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGridViewTestDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	g_bTerminateEnum = TRUE;

	GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText(_T("Stopped!"));
}

void CGridViewTestDlg::OnDestroy()
{
	if (m_pAnchor)
		delete m_pAnchor;

	// TODO: 在此处添加消息处理程序代码
	if(m_hThread != INVALID_HANDLE_VALUE)
	{
		OnBnClickedButtonStop();
		WaitForSingleObject(m_hThread, 2000);
	}

	CDialog::OnDestroy();
}

void CGridViewTestDlg::OnBnClickedButtonChange()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_GridView.GetViewType() == XHGRIDVIEW_REPORT)
	{
		m_GridView.SetViewType(XHGRIDVIEW_DETAIL);
	}
	else
	{
		m_GridView.SetViewType(XHGRIDVIEW_REPORT);
	}
}
