#if !defined(AFX_REWRITEFILE_H__77D2A783_CC49_4325_ACA6_546FAAC76812__INCLUDED_)
#define AFX_REWRITEFILE_H__77D2A783_CC49_4325_ACA6_546FAAC76812__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReWriteFILE.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CReWriteFILE dialog

class CReWriteFILE : public CDialog
{
// Construction
public:
	CReWriteFILE(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    CString MES;
// Dialog Data
	//{{AFX_DATA(CReWriteFILE)
	enum { IDD = IDD_DIALOG_CHECKFILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReWriteFILE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CReWriteFILE)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REWRITEFILE_H__77D2A783_CC49_4325_ACA6_546FAAC76812__INCLUDED_)
