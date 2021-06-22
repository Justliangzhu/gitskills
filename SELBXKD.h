#if !defined(AFX_SELBXKD_H__3FE79396_9E0B_4418_8FF7_DF339C46BDC4__INCLUDED_)
#define AFX_SELBXKD_H__3FE79396_9E0B_4418_8FF7_DF339C46BDC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SELBXKD.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSELBXKD dialog

class CSELBXKD : public CDialog
{
// Construction
public:
	CSELBXKD(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSELBXKD)
	enum { IDD = IDD_DIALOG_SELBXOVERLAP };
	CString	m_inf;
	CString	m_newinf;
	CString	m_yyinf;
	//}}AFX_DATA
    int selinf;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSELBXKD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSELBXKD)
	afx_msg void OnRadioNew();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioOld();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELBXKD_H__3FE79396_9E0B_4418_8FF7_DF339C46BDC4__INCLUDED_)
