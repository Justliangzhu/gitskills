#if !defined(AFX_CXLMBG_H__3C8F2C04_B128_48BC_809C_9803953776EF__INCLUDED_)
#define AFX_CXLMBG_H__3C8F2C04_B128_48BC_809C_9803953776EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CXLMBG.h : header file
//
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCXLMBG dialog

class CCXLMBG : public CDialog
{
// Construction
public:
	CCXLMBG(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	JD_CENTER *pm1;
	JD_CENTER *pm2;
	 XLDataBase DBS;
	CString mdbname,RoadName,HxBh;
// Dialog Data
	//{{AFX_DATA(CCXLMBG)
	enum { IDD = IDD_DIALOG_CXLMBG };
	CString	m_lmbgxzb;
	CString	m_lmbgyzb;
	CString	m_desh1;
	CString	m_desh2;
	CString	m_dist1;
	CString	m_dist2;
	CString	m_lmbg1;
	CString	m_lmbg2;
	CString	m_lmbgc;
	CString	m_lmhp1;
	CString	m_lmhp2;
	CString	m_prjckml1;
	CString	m_prjckml2;
	CString	m_xl1;
	CString	m_xl2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCXLMBG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCXLMBG)
	afx_msg void OnBUTTONLMBGPickPT();
	afx_msg void OnButtonPickxl1();
	afx_msg void OnButtonPickxl2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CXLMBG_H__3C8F2C04_B128_48BC_809C_9803953776EF__INCLUDED_)
