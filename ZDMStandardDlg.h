#if !defined(AFX_ZDMSTANDARDDLG_H__A9D558AF_3815_4269_B1DD_C5A486B1E937__INCLUDED_)
#define AFX_ZDMSTANDARDDLG_H__A9D558AF_3815_4269_B1DD_C5A486B1E937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDMStandardDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ZDMStandardDlg dialog

class ZDMStandardDlg : public CDialog
{
// Construction
public:
	ZDMStandardDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(ZDMStandardDlg)
	enum { IDD = IDD_DIALOG_ZDMStandardDlg };
	CString	m_A_JXRshuMin;
	CString	m_A_YBRshu;
	CString	m_MAXPD;
	CString	m_MINPC;
	CString	m_T_JXRShu;
	CString	m_T_YBRshuMin;
	int m_DV;
	CString	m_YBLShuMin;
	CString	m_YBPCMin;
	CString	m_JXPCMin;
	CString	m_JXLRShuMin;
	CString mdbname;
	//}}AFX_DATA

	BOOL ReadRoadProperty(CString mdbname);
    BOOL ReadZmbzFromMdb(CString mdbname);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZDMStandardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZDMStandardDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDMSTANDARDDLG_H__A9D558AF_3815_4269_B1DD_C5A486B1E937__INCLUDED_)
