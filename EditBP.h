#if !defined(AFX_EDITBP_H__C3FABBAF_C74A_4331_AEB3_174B9AE440F2__INCLUDED_)
#define AFX_EDITBP_H__C3FABBAF_C74A_4331_AEB3_174B9AE440F2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBP.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CEditBP dialog
class CEditBP : public CDialog
{
// Construction
public:
	CEditBP(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditBP)
	enum { IDD = IDD_EDITBP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGridCtrl m_grid;
	GV_ITEM Item;
	int iCols;
	long lRows;
	double m_M[30],m_H[30],m_PTW[30];
	int BPJS;
	CButton * m_Button_Cur;
	CButton * m_Button_Part;
	CButton * m_Button_All;
	bool ifCur,ifPart,ifAll;
	
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditBP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditBP)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	afx_msg void OnAll();
	afx_msg void OnCur();
	afx_msg void OnSomeLCD();
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDElETE();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITBP_H__C3FABBAF_C74A_4331_AEB3_174B9AE440F2__INCLUDED_)
