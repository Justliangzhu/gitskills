#if !defined(AFX_GETHDMDMX_H__DDF00591_5AB3_48E6_BF6C_63E75A0511B3__INCLUDED_)
#define AFX_GETHDMDMX_H__DDF00591_5AB3_48E6_BF6C_63E75A0511B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetHdmDmx.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// GetHdmDmx dialog

class GetHdmDmx : public CDialog
{
// Construction
public:
	GetHdmDmx(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(GetHdmDmx)
	enum { IDD = IDD_DIALOG_HDMDMX };
	double	m_YHP;
	double	m_ZHP;
	int mode;//0-dtm,1- µ≤‚£¨2-–Èƒ‚£¨3-œ‡∂‘
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetHdmDmx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GetHdmDmx)
	afx_msg void OnRadioSc();
	afx_msg void OnRadioXd();
	afx_msg void OnRadioXn();
	afx_msg void OnRadioDtm();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETHDMDMX_H__DDF00591_5AB3_48E6_BF6C_63E75A0511B3__INCLUDED_)
