#if !defined(AFX_DRAWXYTZD_H__82933978_3FA2_479A_BB75_532B75DEA17F__INCLUDED_)
#define AFX_DRAWXYTZD_H__82933978_3FA2_479A_BB75_532B75DEA17F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DRAWXYTZD.h : header file
//
#include "resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// DRAWXYTZD dialog

class DRAWXYTZD : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	DRAWXYTZD(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	XLDataBase DBS;
	JD_CENTER *pm;
// Dialog Data
	//{{AFX_DATA(DRAWXYTZD)
	enum { IDD = IDD_DIALOG_DRAWXYTZD };
	CComboBox	m_RoadNameCrl;
	CString	m_RoadName;
	CString	m_PTX;
	CString	m_PTY;
	CString	m_Dbs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DRAWXYTZD)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DRAWXYTZD)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONbrowser();
	afx_msg void OnButtonSetbypt();
	afx_msg void OnSelchangeCOMBORoadName();
	virtual void OnOK();
	afx_msg void OnButtonPickxl();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWXYTZD_H__82933978_3FA2_479A_BB75_532B75DEA17F__INCLUDED_)
