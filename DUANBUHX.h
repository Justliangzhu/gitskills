#if !defined(AFX_DUANBUHX_H__E4A5B06B_F6F5_4FCD_90DB_C9D67181459B__INCLUDED_)
#define AFX_DUANBUHX_H__E4A5B06B_F6F5_4FCD_90DB_C9D67181459B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DUANBUHX.h : header file
//
#include "JD_CENTER.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// DUANBUHX dialog

class DUANBUHX : public CDialog
{
// Construction
public:
	DUANBUHX(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    JD_CENTER *m_pm;
	AcGePoint2dArray LcArr;
	AcDbObjectId ZBXID[5];
	AcDbObjectId YBXID[5];
	int NZID,NYID;
// Dialog Data
	//{{AFX_DATA(DUANBUHX)
	enum { IDD = IDD_DIALOG_LJDBHX };
	CString	m_Edml;
	CString	m_Sdml;
	CString	m_ZhSerial;
	CString	m_ZhSpacer;
	CString m_HxAng;
	CString m_Show;
	CString m_HxBh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DUANBUHX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DUANBUHX)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPickst();
	afx_msg void OnButtonPicked();
	afx_msg void OnButtonPickadd();
	virtual void OnOK();
	afx_msg void OnButtonSelxl();
	afx_msg void OnButtonPickzcbx();
	afx_msg void OnButtonPickycbx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DUANBUHX_H__E4A5B06B_F6F5_4FCD_90DB_C9D67181459B__INCLUDED_)
