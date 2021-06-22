#if !defined(AFX_SAVEZDMDLG_H__AA173EC7_03EF_44AD_A21E_4454AC865CFF__INCLUDED_)
#define AFX_SAVEZDMDLG_H__AA173EC7_03EF_44AD_A21E_4454AC865CFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SAVEZDMDLG.h : header file
//
#include "resource.h"
#include "XLDataBase.h"
/////////////////////////////////////////////////////////////////////////////
// SAVEZDMDLG dialog

class SAVEZDMDLG : public CDialog
{
// Construction
public:
	SAVEZDMDLG(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    	XLDataBase  DBS;
// Dialog Data
	//{{AFX_DATA(SAVEZDMDLG)
	enum { IDD = IDD_DIALOG_SAVEZDM };
	CComboBox	m_ZPXZCrl;
	CComboBox	m_ZDMTKCrl;
	CComboBox	m_RoadNameCrl;
	BOOL	m_infbri;
	BOOL	m_infdmx;
	BOOL	m_inftun;
	BOOL	m_infpd;//ÆÂ¶ÈÐÅÏ¢
	CString	m_RoadName;
	CString	m_ZDMTKNO;
	CString	m_ZPXZ;
	CString	m_Dbs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SAVEZDMDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SAVEZDMDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPickzdm();
	afx_msg void OnSelchangeComboZdmprono();
	afx_msg void OnBUTTONbrowser();
	afx_msg void OnCheckZdmpd();
	afx_msg void OnCheckDmx();
	afx_msg void OnCheckBri();
	afx_msg void OnCheckTun();
//	afx_msg void OnCheckPm();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEZDMDLG_H__AA173EC7_03EF_44AD_A21E_4454AC865CFF__INCLUDED_)
