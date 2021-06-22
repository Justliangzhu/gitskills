#if !defined(AFX_ZDMKZBG_H__8E9D0F3D_420E_469A_BB68_5B04CA8FCF80__INCLUDED_)
#define AFX_ZDMKZBG_H__8E9D0F3D_420E_469A_BB68_5B04CA8FCF80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDMKZBG.h : header file
//
#include "NEWGRIDCTRL\GridCtrl.h"
#include "NEWGRIDCTRL\Inplaceedit.h"
#include "NEWGRIDCTRL\GridCellCombo.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ZDMKZBG dialog
#include "JdCenGridCtrl.h"

class ZDMKZBG : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	ZDMKZBG(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(ZDMKZBG)
	enum { IDD = IDD_DIALOG_XZBG };
	BOOL	m_gzxs;
	//}}AFX_DATA
  	JdCenGridCtrl m_grid;
	double	X0,Y0;
	double K0,H0;
    double  HBVSCALE;
//	ept[X] = (cml-K0)/HBVSCALE+X0;
//	ept[Y] = rdes_level(cml)-+Y0;
	CString m_RoadName;
	CString mdbname;
	XLDataBase  DBS;
	GV_ITEM Item;
	JD_CENTER *pm;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ZDMKZBG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ZDMKZBG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPickzdm();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	afx_msg void OnButtonDraw();
	virtual void OnOK();
	afx_msg void OnCheckGzxs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDMKZBG_H__8E9D0F3D_420E_469A_BB68_5B04CA8FCF80__INCLUDED_)
