#if !defined(AFX_SETCGBZDLG_H__736BE7FA_D976_4296_BDD5_61E6B4F46C0C__INCLUDED_)
#define AFX_SETCGBZDLG_H__736BE7FA_D976_4296_BDD5_61E6B4F46C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCGBZDlg.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "BAS_DRAW_FUN.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// SetCGBZDlg dialog

class SetCGBZDlg : public CDialog,BAS_DRAW_FUN
{
// Construction
public:
	SetCGBZDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(SetCGBZDlg)
	enum { IDD = IDD_DIALOG_CGBZ };
	int		m_DV;
	CString	m_MAXSuphI;
	CString	m_IG;
	

	//}}AFX_DATA
	int NSUPH;
	double SuphData[10][2];
	
	GV_ITEM Item;
	CString mdbname;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetCGBZDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SetCGBZDlg)
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioMdb();
	afx_msg void OnRadioNorm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioBushechaogao();
	CGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCGBZDLG_H__736BE7FA_D976_4296_BDD5_61E6B4F46C0C__INCLUDED_)
