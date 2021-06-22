#if !defined(AFX_SETJKNORM_H__77A2B330_CDED_4FCA_8AD2_632BE450BAD9__INCLUDED_)
#define AFX_SETJKNORM_H__77A2B330_CDED_4FCA_8AD2_632BE450BAD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetJKNorm.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "BAS_DRAW_FUN.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CSetJKNorm dialog

class CSetJKNorm : public CDialog,BAS_DRAW_FUN
{
// Construction
public:
	CSetJKNorm(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetJKNorm)
	enum { IDD = IDD_DIALOGJKNORM };
	CComboBox	m_JKLBCrl;
	CString	m_GLDJ;
	int		m_DV;
	CString	m_JKLB;
	//}}AFX_DATA
	int NJK;
	double JKData[10][2];
	
	GV_ITEM Item;
	CString mdbname;
	int MdborNorm; //0 mdb 1 norm;
	CString xlxz;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetJKNorm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetJKNorm)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	virtual void OnOK();
	afx_msg void OnRadioJkmdb();
	afx_msg void OnRadioJknorm();
	afx_msg void OnSelchangeCombojklb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioBushejiakuan();
	CGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETJKNORM_H__77A2B330_CDED_4FCA_8AD2_632BE450BAD9__INCLUDED_)
