#if !defined(AFX_PPCDSJ_H__F28233B2_23DD_4C9B_B2CF_DEB78111DF4B__INCLUDED_)
#define AFX_PPCDSJ_H__F28233B2_23DD_4C9B_B2CF_DEB78111DF4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PPCDsj.h : header file
//
#include "BAS_DRAW_FUN.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "resource.h"
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

/////////////////////////////////////////////////////////////////////////////
// PPCDsj dialog

class PPCDsj : public CPropertyPage, BAS_DRAW_FUN 
{
	DECLARE_DYNCREATE(PPCDsj)

// Construction
public:
	PPCDsj();
	~PPCDsj();

// Dialog Data
	//{{AFX_DATA(PPCDsj)
	enum { IDD = IDD_DIALOG_PPCD };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
	CGridCtrl m_grid;
	GV_ITEM Item;

	int NPPCD;//爬坡车道分段数
	PPCDdata *PPCD;//爬坡车道数
	int ZorY;

	void SetGrid(); 
	void SetPPCD(int nppcd,PPCDdata ppcd[]) ;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PPCDsj)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PPCDsj)
	afx_msg void OnBUTTONAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONdel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PPCDSJ_H__F28233B2_23DD_4C9B_B2CF_DEB78111DF4B__INCLUDED_)
