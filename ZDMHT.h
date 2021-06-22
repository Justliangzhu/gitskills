#if !defined(AFX_ZDMHT_H__38D1E31B_65B3_4783_8179_F7444877F96F__INCLUDED_)
#define AFX_ZDMHT_H__38D1E31B_65B3_4783_8179_F7444877F96F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDMHT.h : header file
//
#include "DrawZDMDlg.h"
/////////////////////////////////////////////////////////////////////////////
// ZDMHT

class ZDMHT : public CPropertySheet
{
	DECLARE_DYNAMIC(ZDMHT)

// Construction
public:
	ZDMHT(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ZDMHT(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	DrawZDMDlg zinidlg;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZDMHT)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ZDMHT();

	// Generated message map functions
protected:
	//{{AFX_MSG(ZDMHT)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDMHT_H__38D1E31B_65B3_4783_8179_F7444877F96F__INCLUDED_)
