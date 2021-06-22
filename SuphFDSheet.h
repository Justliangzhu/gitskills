#if !defined(AFX_SUPHFDSHEET_H__F27B63BC_D515_4F72_9DE3_24D63D4F2772__INCLUDED_)
#define AFX_SUPHFDSHEET_H__F27B63BC_D515_4F72_9DE3_24D63D4F2772__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuphFDSheet.h : header file
//
#include "SuphFDDataDlg.h"
/////////////////////////////////////////////////////////////////////////////
// SuphFDSheet

class SuphFDSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(SuphFDSheet)

// Construction
public:
	SuphFDSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	SuphFDSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
	SuphFDDataDlg ZCG,YCG;
	SuphFDDataDlg ZJK,YJK;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SuphFDSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~SuphFDSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(SuphFDSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPHFDSHEET_H__F27B63BC_D515_4F72_9DE3_24D63D4F2772__INCLUDED_)
