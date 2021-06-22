#if !defined(AFX_NOSEDLG_H__1166274A_98D5_4047_A43F_4E175E2D9D7F__INCLUDED_)
#define AFX_NOSEDLG_H__1166274A_98D5_4047_A43F_4E175E2D9D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NoseDlg.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "BAS_DRAW_FUN.h"
#include "cdxCSizingDialog.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// NoseDlg dialog

class NoseDlg : public cdxCSizingDialog
{
// Construction
public:
	NoseDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(NoseDlg)
	enum { IDD = IDD_DIALOG_NOSE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGridCtrl	m_grid;
	GV_ITEM Item;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NoseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NoseDlg)
	afx_msg void OnButtonCal();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NOSEDLG_H__1166274A_98D5_4047_A43F_4E175E2D9D7F__INCLUDED_)
