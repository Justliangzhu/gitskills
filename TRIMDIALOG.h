#if !defined(AFX_TRIMDIALOG_H__4886357B_14CF_4A5F_BC14_210B3EA4B851__INCLUDED_)
#define AFX_TRIMDIALOG_H__4886357B_14CF_4A5F_BC14_210B3EA4B851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TRIMDIALOG.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// TRIMDIALOG dialog

class TRIMDIALOG : public CDialog
{
// Construction
public:
	TRIMDIALOG(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(TRIMDIALOG)
	enum { IDD = IDD_DIALOG_TKPRO };
	CString	m_SCALE;
	CString	m_HEIGHT;
	CString	m_DH;
	bool IsOk;//是否选确定
	double m_dSpace;
	double m_Scale;
	double m_H;
	double m_dH;
	double m_dTK;
	CString	m_DTK;
	CString	m_SPACEW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TRIMDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TRIMDIALOG)
	afx_msg void OnEditchangeComboScale();
	afx_msg void OnEditchangeCOMBOHeight();
	afx_msg void OnEditchangeComboDh();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnEditchangeComboSpacew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIMDIALOG_H__4886357B_14CF_4A5F_BC14_210B3EA4B851__INCLUDED_)
