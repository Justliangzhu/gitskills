#if !defined(AFX_PMDIALOG_H__1DDBB354_FD3F_48CD_9023_E18AAE603C23__INCLUDED_)
#define AFX_PMDIALOG_H__1DDBB354_FD3F_48CD_9023_E18AAE603C23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PMDIALOG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PMDIALOG dialog
//平面数据录入对话框（含基线和所有分离段）
#include "cdxCSizingDialog.h"
#include "JdCenterInputDlg.h"
#include "JD_CENTER.H"
#include "EXMANAGER.H"

class PMDIALOG : public cdxCSizingDialog
{
// Construction
public:
	PMDIALOG(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	~PMDIALOG();   // standard constructor
	
// Dialog Data
	//{{AFX_DATA(PMDIALOG)
	enum { IDD = IDD_DIALOG_PM };
	CTabCtrl	m_Tab;
	CString	m_FLDName;
	//}}AFX_DATA

	JD_CENTER* JdCenObj;//基线
	AcDbObjectId JdCenId;	

	EXMANAGER *EXObj;//分离段
	AcDbObjectId EXId;
	bool MainLineEnable;
	JdCenterInputDlg *MainLineDlg;
	CArray<JdCenterInputDlg*,JdCenterInputDlg*> FLD;
	CStringArray FLDNameArray;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PMDIALOG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PMDIALOG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNewfld();
	afx_msg void OnButtonRename();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonDelfld();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnBUTTONInPmFromMdb();
	afx_msg void OnBUTTONOutputMdb();
	afx_msg void OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMDIALOG_H__1DDBB354_FD3F_48CD_9023_E18AAE603C23__INCLUDED_)
