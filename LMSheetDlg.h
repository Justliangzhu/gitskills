#if !defined(AFX_LMSHEETDLG_H__E2C40B01_5464_4FA9_8390_0E9BEA59FE34__INCLUDED_)
#define AFX_LMSHEETDLG_H__E2C40B01_5464_4FA9_8390_0E9BEA59FE34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LMSheetDlg.h : header file
//
#include "lmsj.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// LMSheetDlg

class LMSheetDlg : public CPropertySheet,BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(LMSheetDlg)

// Construction
public:
	LMSheetDlg(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	LMSheetDlg(JD_CENTER *pm,LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    LMSJ zlmk,ylmk;
	JD_CENTER *m_pm;
	void SetDefaultLmk(int &nlmk,LMKdata &lmk);
	void SetZYLmkInitData();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LMSheetDlg)
	public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~LMSheetDlg();

	// Generated message map functions
protected:
	//{{AFX_MSG(LMSheetDlg)
	afx_msg void OnButtonCopy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMSHEETDLG_H__E2C40B01_5464_4FA9_8390_0E9BEA59FE34__INCLUDED_)
