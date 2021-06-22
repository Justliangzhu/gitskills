#if !defined(AFX_ZDMORIDATADLG_H__CA588735_4EF4_43D7_95F8_33C8867F2445__INCLUDED_)
#define AFX_ZDMORIDATADLG_H__CA588735_4EF4_43D7_95F8_33C8867F2445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZdmOriDataDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ZdmOriDataDlg dialog

class ZdmOriDataDlg : public CDialog
{
// Construction
public:
	ZdmOriDataDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	int ModeBpdlc;//0 1 2 3
// Dialog Data
	//{{AFX_DATA(ZdmOriDataDlg)
	enum { IDD = IDD_DIALOGZdmDrawPam };
	CString	m_LCBoxH;
	CString	m_LCTextH;
	CString	m_PQXBoxH;
	CString	m_PQXTextH;
	CString	m_VScale;
	CString	m_Xori;
	CString	m_Yori;
	CString	m_Hscale;
	CString	m_bd;
	CString VerRadius;
	CComboBox m_bpdlc;
	CString	m_TDFS;
	CString	m_desh;
	CString	m_PDTextH;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZdmOriDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZdmOriDataDlg)
	virtual void OnOK();
	afx_msg void OnBUTTONpick();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
    afx_msg void OnSelchangeComboxBpdlc();
//	afx_msg void OnRadioljlp();
//	afx_msg void OnRadiogmlp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDMORIDATADLG_H__CA588735_4EF4_43D7_95F8_33C8867F2445__INCLUDED_)
