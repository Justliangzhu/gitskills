#if !defined(AFX_PMCUTDLG_H__9C1884EA_E7FA_4260_9321_08837AD79B6A__INCLUDED_)
#define AFX_PMCUTDLG_H__9C1884EA_E7FA_4260_9321_08837AD79B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmCutDlg.h : header file
//
#include "resource.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// PmCutDlg dialog

class PmCutDlg : public CDialog
{
// Construction
public:
	PmCutDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    JD_CENTER *pm;
// Dialog Data
	//{{AFX_DATA(PmCutDlg)
	enum { IDD = IDD_DIALOG_TIP };
	CString	m_Xori;
	CString	m_Yori;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PmCutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PmCutDlg)
	afx_msg void OnButtonSet();
	afx_msg void OnBUTTONpick();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMCUTDLG_H__9C1884EA_E7FA_4260_9321_08837AD79B6A__INCLUDED_)
