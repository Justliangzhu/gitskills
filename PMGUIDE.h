#if !defined(AFX_PMGUIDE_H__10F1CE25_B41D_43B0_AF2C_1A65FDA808F2__INCLUDED_)
#define AFX_PMGUIDE_H__10F1CE25_B41D_43B0_AF2C_1A65FDA808F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PMGUIDE.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// PMGUIDE dialog

class PMGUIDE : public CDialog
{
// Construction
public:
	PMGUIDE(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(PMGUIDE)
	enum { IDD = IDD_DIALOG_Guide };
	CSliderCtrl	m_sld;
	CComboBox	m_RoadNameCrl;
	CString	m_RoadName;
	CString	m_mdbname;
	CString	m_startdml;
	CString	m_enddml;
	CString	m_ZdDml;
	//}}AFX_DATA

	JD_CENTER  *pm;
    XLDataBase DBS;
	int m_mode;//平面方案方式 0 文件方式 new GTZX 要释放；  1 图选方式
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PMGUIDE)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PMGUIDE)
	afx_msg void OnButtonZdmdb();
	afx_msg void OnSelchangeCOMBORoadName();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPickpmxl();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMGUIDE_H__10F1CE25_B41D_43B0_AF2C_1A65FDA808F2__INCLUDED_)
