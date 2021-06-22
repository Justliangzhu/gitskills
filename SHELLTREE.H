#if !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
#define AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ShellTree.h : header file
//

#include <shlobj.h>

// STRUCTURES
typedef struct tagLVID
{
   LPSHELLFOLDER lpsfParent;
   LPITEMIDLIST  lpi;
   ULONG         ulAttribs;
} LVITEMDATA, *LPLVITEMDATA;

typedef struct tagID
{
   LPSHELLFOLDER lpsfParent;
   LPITEMIDLIST  lpi;
   LPITEMIDLIST  lpifq;
} TVITEMDATA, *LPTVITEMDATA;

/////////////////////////////////////////////////////////////////////////////
// CShellTree window

class CShellTree : public CTreeCtrl
{
// Construction
public:
	CShellTree();

// Attributes
public:
    BOOL m_bOutaHere;

// Operations
public:
 	static int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);
	void	PopulateTree();
	void	FolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	void	FolderPopup(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL	FolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void	EnableImages();
	BOOL	GetSelectedFolderPath(CString &szFolderPath);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellTree();

	// Generated message map functions
protected:
	void	FillTreeView(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpifq, HTREEITEM     hParent);
	int		GetIcon(LPITEMIDLIST lpi, UINT uFlags);
	void	GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
	BOOL	DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST  lpi, LPPOINT lppt);

    // Functions that deal with PIDLs
    LPITEMIDLIST	ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    LPITEMIDLIST	GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi);
    LPITEMIDLIST	CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi);
    BOOL			GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, LPSTR lpFriendlyName);
    LPITEMIDLIST	CreatePidl(UINT cbSize);
    UINT			GetSize(LPCITEMIDLIST pidl);
    LPITEMIDLIST	Next(LPCITEMIDLIST pidl);
	
	//{{AFX_MSG(CShellTree)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
