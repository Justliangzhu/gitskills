#if !defined(AFX_HDMTDLG_H__14D4F136_FD9F_4FEE_A806_1FBF515C0A98__INCLUDED_)
#define AFX_HDMTDLG_H__14D4F136_FD9F_4FEE_A806_1FBF515C0A98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HDMTdlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// HDMTdlg dialog

class HDMTdlg : public CDialog
{
// Construction
public:
	HDMTdlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(HDMTdlg)
	enum { IDD = IDD_DIALOG_HDMT };
	CComboBox	m_RoadName;
	CComboBox	m_SKML;
	CComboBox	m_EKML;
	CString	m_TKNAME;
	double	m_YK;
	double	m_ZK;
	CString	m_Lnum;
	CString roadname;
	int lnum;
    CString CKSml;
	CString CKEml;
	double	m_SX;
	double	m_SY;
	BOOL	m_IfOut;
	int		m_startpage;
	BOOL	m_IfBZ;
	CString	m_scale;
	//}}AFX_DATA
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HDMTdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(HDMTdlg)
	afx_msg void OnBUTTONbrowser();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBORoadName();
	virtual void OnOK();
	afx_msg void OnBUTTONpick();
	afx_msg void OnCHECKIfBZ();
	afx_msg void OnChangeEditHdmscale();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HDMTDLG_H__14D4F136_FD9F_4FEE_A806_1FBF515C0A98__INCLUDED_)
