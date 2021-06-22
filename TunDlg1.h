#if !defined(AFX_TUNDLG1_H__E9A76E49_D021_4DA3_9E44_12BBB8C20BB5__INCLUDED_)
#define AFX_TUNDLG1_H__E9A76E49_D021_4DA3_9E44_12BBB8C20BB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TunDlg1.h : header file
//
#include "BTGridCtrl.h"
#include "resource.h"
#include "cdxCSizingDialog.h"
#include "tun.h"
/////////////////////////////////////////////////////////////////////////////
// TunDlg dialog

class TunDlg : public cdxCSizingDialog
{
// Construction
public:
	TunDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(TunDlg)
	enum { IDD = IDD_DIALOG_TUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	BTGridCtrl	m_grid;
	GV_ITEM Item;

	AcDbObjectId m_eId;
    int m_TunNum;
	int DXorRXD;
    int rxdno;
	GTZDM_ROAD *zdm;
	TUN_ROAD *pTun;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TunDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TunDlg)
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

#endif // !defined(AFX_TUNDLG1_H__E9A76E49_D021_4DA3_9E44_12BBB8C20BB5__INCLUDED_)
