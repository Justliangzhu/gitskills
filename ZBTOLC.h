#if !defined(AFX_ZBTOLC_H__27402773_88F3_4460_999E_5667A6954D01__INCLUDED_)
#define AFX_ZBTOLC_H__27402773_88F3_4460_999E_5667A6954D01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZBTOLC.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ZBTOLC dialog

class ZBTOLC : public CDialog
{
// Construction
public:
	ZBTOLC(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	JD_CENTER  *pm;
    XLDataBase DBS;
	int m_mode;//平面方案方式 0 文件方式 new GTZX 要释放；  1 图选方式
// Dialog Data
	//{{AFX_DATA(ZBTOLC)
	enum { IDD = IDD_DIALOG_ZBTOLC };
	CComboBox	m_RoadNameCrl;
	BOOL	m_infdrawlc;
	CString	m_RoadName;
	CString	m_mdbname;
	CString	m_Xcor;
	CString	m_Ycor;
	CString	m_CKML;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZBTOLC)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZBTOLC)
	afx_msg void OnButtonZbbrowser();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnButtonPickxl();
	afx_msg void OnBUTTONPickPT();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDrawlc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZBTOLC_H__27402773_88F3_4460_999E_5667A6954D01__INCLUDED_)
