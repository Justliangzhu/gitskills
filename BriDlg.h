#if !defined(AFX_BRIDLG_H__4CE0C55C_1886_470C_BF11_56C2FA35E366__INCLUDED_)
#define AFX_BRIDLG_H__4CE0C55C_1886_470C_BF11_56C2FA35E366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BriDlg.h : header file
//
#include "BTGridCtrl.h"
#include "resource.h"
#include"cdxCSizingDialog.h"
#include "Bri.h"
/////////////////////////////////////////////////////////////////////////////
// BriDlg dialog

class BriDlg : public cdxCSizingDialog
{
// Construction
public:
	BriDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	BTGridCtrl	m_grid;
	GV_ITEM Item;
// Dialog Data
	//{{AFX_DATA(BriDlg)
	enum { IDD = IDD_DIALOG_Bri };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

   AcDbObjectId m_eId;
   int m_BriNum;
   int DXorRXD;
   int rxdno;
   GTZDM_ROAD *zdm;
   BRI_ROAD *pBri;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BriDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BriDlg)
	afx_msg void OnButtonInDatabase();
	afx_msg void OnButtonadd();
	afx_msg void OnButtondel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONInBase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRIDLG_H__4CE0C55C_1886_470C_BF11_56C2FA35E366__INCLUDED_)
