#if !defined(AFX_TRIMDIAG_H__6AF8447A_BB2B_4483_BFB3_87BB06AF5191__INCLUDED_)
#define AFX_TRIMDIAG_H__6AF8447A_BB2B_4483_BFB3_87BB06AF5191__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TRIMDIAG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TRIMDIAG dialog
#include "GridCtrl.h"
class TRIMDIAG : public CDialog
{
// Construction
public:
	void CreateGrid();
	void Tag();
	void TagCombobox();
	TRIMDIAG(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(TRIMDIAG)
	enum { IDD = IDD_DIALOG_TRIMTU };
	CComboBox	m_EndCtrl;
	CComboBox	m_StartCtrl;
	CButton	m_CANCLE;
	CButton	m_OK;
	CString	m_START;
	CString	m_END;
	int NUM;
	CString	m_NUM;
	CGridCtrl m_grid;
    GV_ITEM Item;
	int Rows;
	int Cols;
	CString	m_LC;
	int		m_TRIMFLAG;
	BOOL m_IsDrawZDM;
	CString m_MdbName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TRIMDIAG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TRIMDIAG)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditchangeCombo1();
	afx_msg void OnEditchangeCombo2();
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnSelchangeCombo2();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedCheckDrawzdm();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIMDIAG_H__6AF8447A_BB2B_4483_BFB3_87BB06AF5191__INCLUDED_)
