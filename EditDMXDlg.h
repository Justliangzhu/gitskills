#if !defined(AFX_EDITDMXDLG_H__CDD40941_F801_11D7_AF19_006067772FF4__INCLUDED_)
#define AFX_EDITDMXDLG_H__CDD40941_F801_11D7_AF19_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDMXDlg.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "BAS_DRAW_FUN.h"
#include "cdxCSizingDialog.h"
#include "DMX.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// EditDMXDlg dialog

class EditDMXDlg : public CDialog
{
// Construction
public:
	EditDMXDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditDMXDlg)
	enum { IDD = IDD_DIALOG_EDITDMD };
	
	GV_ITEM Item;
	Adesk::Int32 m_NDmd;
	AcDbObjectId m_eId;	//}}AFX_DATA
	CString DmxFname;
	DMX_ROAD  *pdmx;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditDMXDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditDMXDlg)
	afx_msg void OnButtonadd();
	afx_msg void OnButtondel();
	afx_msg void OnBUTTONSelDMX();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDMXDLG_H__CDD40941_F801_11D7_AF19_006067772FF4__INCLUDED_)
