#if !defined(AFX_QBTMLDLG_H__833D70F2_D667_45D7_8ECA_A5F992D40508__INCLUDED_)
#define AFX_QBTMLDLG_H__833D70F2_D667_45D7_8ECA_A5F992D40508__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QBTMLDlg.h : header file
//
#include "resource.h"
#include "XLDataBase.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
/////////////////////////////////////////////////////////////////////////////
// QBTMLDlg dialog

class QBTMLDlg : public CDialog
{
// Construction
public:
	QBTMLDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(QBTMLDlg)
	enum { IDD = IDD_DIALOG_QBTML };
	CComboBox	m_RoadName;
	CString	m_mdbname;
	XLDataBase mdb;
	CString RoadName;

	CGridCtrl m_grid;
	GV_ITEM Item;

	int NFD;
	double *FDML;
	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(QBTMLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(QBTMLDlg)
	afx_msg void OnButtonInsert();
	afx_msg void OnBUTTONqbtdel();
	afx_msg void OnBUTTONqbtInput();
	afx_msg void OnBUTTONqbtsave();
	virtual void OnOK();
	afx_msg void OnBUTTONbrowserMdb();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QBTMLDLG_H__833D70F2_D667_45D7_8ECA_A5F992D40508__INCLUDED_)
