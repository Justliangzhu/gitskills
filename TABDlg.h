#if !defined(AFX_TABDLG_H__09DBAF69_C3E9_43CE_B454_A1C3B8DD8488__INCLUDED_)
#define AFX_TABDLG_H__09DBAF69_C3E9_43CE_B454_A1C3B8DD8488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TABDlg.h : header file
//
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CTABDlg dialog

class CTABDlg : public CDialog
{
// Construction
public:
	CTABDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTABDlg)
	enum { IDD = IDD_DIALOG_TAB };
	CComboBox	m_RoadName;
	CString	m_DuanName;
	double	m_Edml;
	CString	m_PrjMdb;
	CString	m_RoadStr;
	double	m_Sdml;
	int		m_Spage;
	int		m_PageSum;
	XLDataBase mdb;
	CString RoadName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTABDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTABDlg)
	afx_msg void OnBUTTONbrowser();
	afx_msg void OnSelchangeCOMBORoadName();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABDLG_H__09DBAF69_C3E9_43CE_B454_A1C3B8DD8488__INCLUDED_)
