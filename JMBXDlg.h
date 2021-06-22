#if !defined(AFX_JMBXDLG_H__7E0A3ECB_DCB4_481B_947F_17AC80018B07__INCLUDED_)
#define AFX_JMBXDLG_H__7E0A3ECB_DCB4_481B_947F_17AC80018B07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JMBXDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// JMBXDlg dialog

class JMBXDlg : public CDialog
{
// Construction
public:
	JMBXDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(JMBXDlg)
	enum { IDD = IDD_DIALOG_JMBX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JMBXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(JMBXDlg)
	afx_msg void OnBUTTONXYtoJD();
	afx_msg void OnBUTTONline();
	afx_msg void OnButtonHhqx();
	afx_msg void OnBUTTONarc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JMBXDLG_H__7E0A3ECB_DCB4_481B_947F_17AC80018B07__INCLUDED_)
