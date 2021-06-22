#if !defined(AFX_CALSTA_H__DA5ECFB3_9C96_4680_9CC2_913059ED8CDC__INCLUDED_)
#define AFX_CALSTA_H__DA5ECFB3_9C96_4680_9CC2_913059ED8CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalSta.h : header file
//
#include "resource.h"
#include "GridCtrl.h"
#include "Inplaceedit.h"
#include "GridCellCombo.h"
#include "BAS_DRAW_FUN.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// CCalSta dialog

class CCalSta : public CPropertyPage,public BAS_DRAW_FUN
{
// Construction
	DECLARE_DYNCREATE(CCalSta)
public:
	CCalSta(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCalSta)
	enum { IDD = IDD_DIALOG_Sta };
	BOOL	m_IfBri;
	BOOL	m_IfQX;
	BOOL	m_IfTun;
	double	m_JG;

	
	//}}AFX_DATA
	
	GV_ITEM Item;


	CString roadname;
    JD_CENTER *pm;
	int NSTA;
	CArray<LCBdata,LCBdata>lcb;
	
	int NBRI;
	double BriAry[200][3];
	int NTUN;
	double TunAry[200][3];
	int NGLSTA;
	double GLSTA[50][3];

	void DMXZtoStr(int BTxz,CString &tmp);
	void StrtoDMXZ(CString tmp,int &BTxz);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalSta)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalSta)
	virtual void OnOK();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonCal();
//	afx_msg void OnBUTTONadjust();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALSTA_H__DA5ECFB3_9C96_4680_9CC2_913059ED8CDC__INCLUDED_)
