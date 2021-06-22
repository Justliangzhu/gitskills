#if !defined(AFX_DLBDLG_H__F4301573_0F05_49CD_801F_0F620EEB2095__INCLUDED_)
#define AFX_DLBDLG_H__F4301573_0F05_49CD_801F_0F620EEB2095__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DLBdlg.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "BAS_DRAW_FUN.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// DLBdlg dialog

class DLBdlg : public CDialog,public BAS_DRAW_FUN 
{
// Construction
public:
	//DLBdlg(CWnd* pParent = NULL);   // standard constructor
	DLBdlg(CWnd* pParent = acedGetAcadFrame());
// Dialog Data
	//{{AFX_DATA(DLBdlg)
	enum { IDD = IDD_DIALOG_DLBdlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGridCtrl m_grid;
	GV_ITEM Item;
	Adesk::Int16 m_NDL;

	AcDbObjectId m_eId;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DLBdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DLBdlg)
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	afx_msg void OnBUTTONselLine();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLBDLG_H__F4301573_0F05_49CD_801F_0F620EEB2095__INCLUDED_)
