#if !defined(AFX_BDCALDLG_H__22366CBC_CDF4_421A_B181_0EA6D037D1D1__INCLUDED_)
#define AFX_BDCALDLG_H__22366CBC_CDF4_421A_B181_0EA6D037D1D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BDCALDlg.h : header file
//
#include "cdxCSizingDialog.h"
/////////////////////////////////////////////////////////////////////////////
// BDCALDlg dialog
#include "BTGridCtrl.h"
#include "resource.h"
#include "JD_CENTER.h"
#include "XLDataBase.h"

class BDCALDlg : public cdxCSizingDialog,public BAS_DRAW_FUN
{
// Construction
public:
	BDCALDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	void GetDataOnJM();
	void ShowDataOnJM();
// Dialog Data
	//{{AFX_DATA(BDCALDlg)
	enum { IDD = IDD_DIALOG_CALBD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CString m_Dbs;
	XLDataBase DBS;
   struct BDData BDArr[100];
    int NBIDUAN;//±Ç¶Ë¸öÊý
	JD_CENTER *pm1;
	JD_CENTER *pm2;
	BTGridCtrl m_grid;
	GV_ITEM Item;	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BDCALDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BDCALDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelxl();
	afx_msg void OnButtonCalbd();
	afx_msg void OnButtonDrawbd();
	afx_msg void OnButtonAddbd();
	afx_msg void OnButtonDelbd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BDCALDLG_H__22366CBC_CDF4_421A_B181_0EA6D037D1D1__INCLUDED_)
