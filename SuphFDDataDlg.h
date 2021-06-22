#if !defined(AFX_SUPHFDDATADLG_H__F2CA3E13_5EA1_41FA_98DA_78E3CAF4F8CC__INCLUDED_)
#define AFX_SUPHFDDATADLG_H__F2CA3E13_5EA1_41FA_98DA_78E3CAF4F8CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SuphFDDataDlg.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "resource.h"
#include "BAS_DRAW_FUN.h"
/////////////////////////////////////////////////////////////////////////////
// SuphFDDataDlg dialog

class SuphFDDataDlg : public CPropertyPage,BAS_DRAW_FUN
{
	DECLARE_DYNCREATE(SuphFDDataDlg)

// Construction
public:
	SuphFDDataDlg();
	~SuphFDDataDlg();

// Dialog Data
	//{{AFX_DATA(SuphFDDataDlg)
	enum { IDD = IDD_DIALOG_CGFDDATA };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
   
    
	GV_ITEM Item;
	int NCG;
	CgSZ CG[1000];
	int CgJkInf; //0 ³¬¸ß 1¼Ó¿í
	int flag1;
	CString mdbname,roadname;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(SuphFDDataDlg)
	public:
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(SuphFDDataDlg)
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPHFDDATADLG_H__F2CA3E13_5EA1_41FA_98DA_78E3CAF4F8CC__INCLUDED_)
