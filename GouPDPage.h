#if !defined(AFX_GOUPDPAGE_H__E003D0F3_5BB3_4849_A720_FD859C4E2F5E__INCLUDED_)
#define AFX_GOUPDPAGE_H__E003D0F3_5BB3_4849_A720_FD859C4E2F5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GouPDPage.h : header file
//
#include "resource.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "BAS_DRAW_FUN.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// GouPDPage dialog

class GouPDPage : public CPropertyPage,BAS_DRAW_FUN
{
	DECLARE_DYNCREATE(GouPDPage)

// Construction
public:
	GouPDPage();
	~GouPDPage();

	CGridCtrl m_grid;
	GV_ITEM Item;

	CString mdbFname;
	CString roadname;
    JD_CENTER *pm;
	int NGouPD;
	CArray<GouPDdata,GouPDdata>GouPD;
	int ZorY;

// Dialog Data
	//{{AFX_DATA(GouPDPage)
	enum { IDD = IDD_DIALOG_GouPD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(GouPDPage)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(GouPDPage)
	afx_msg void OnButtonadd();
	afx_msg void OnBUTTONrefresh();
	afx_msg void OnButtondel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONpick();
	afx_msg void OnBUTTONsave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOUPDPAGE_H__E003D0F3_5BB3_4849_A720_FD859C4E2F5E__INCLUDED_)
