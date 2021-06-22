#if !defined(AFX_SETDRAWQXYS_H__1A919F54_5AA2_4147_918F_CD2D4F85AA28__INCLUDED_)
#define AFX_SETDRAWQXYS_H__1A919F54_5AA2_4147_918F_CD2D4F85AA28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDrawQXYS.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// SetDrawQXYS dialog

class SetDrawQXYS : public CDialog , BAS_DRAW_FUN
{
// Construction
public:
    JD_CENTER *pm;
	XLDataBase DBS;
	int tempSJD ,tempEJD;
	int DrawMode;//0 按初步设计绘 1按施工图绘
	int del_ent(double Wtab,double Htab);
	int AutoEntBK(ads_point p0, ads_point p1);
	int wr_cmf_c();
	int wr_cmf_s();
	void  MidPoint(ads_point pt,double w,double h,double angx,ads_point Mpt);
	void LineLR(ads_point p1,ads_point p2,double angx,double dist,double lw,int color=2);
	void LineUD(ads_point p1,ads_point p2,double angx,double dist,double lw);
	void wr_text(ads_point pt,double angx,double d,double wk,double ht, ACHAR *str);
	void draw();
	SetDrawQXYS(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    double DFACTOR,INPTX,INPTY,ALPHA,Wtab,Htab;
	ads_point Ppt;
	int SIP,EIP,JDn;
// Dialog Data
	//{{AFX_DATA(SetDrawQXYS)
	enum { IDD = IDD_DIALOGQXYS };
	CComboBox	m_RoadNameCrl;
	CComboBox	m_ZJANGLE;
	CComboBox	m_SjdCtrl;
	CComboBox	m_EjdCtl;
	CString	m_Angle;
	CString	m_scale;
	CString	m_EndJD;
	CString	m_StaJD;
	CString	m_InputAng;
	CString	m_X;
	CString	m_Y;
	CString	m_Dbs;
	CString	m_RoadName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetDrawQXYS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void ptAddx(ads_point p1,double angx,double x,ads_point pt);
	void ptAddy(ads_point p1,double angx,double y,ads_point pt);
protected:

	// Generated message map functions
	//{{AFX_MSG(SetDrawQXYS)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonsetwz();
	afx_msg void OnButtonsetang();
	virtual void OnOK();
	afx_msg void OnRadiobcbsj();
	afx_msg void OnRadiosgtsj();
	afx_msg void OnSelchangeCombostjd();
	afx_msg void OnSelchangeComboedjd();
	afx_msg void OnSelchangeComboangle();
	afx_msg void OnButtonBrow();
	afx_msg void OnSelchangeCOMBORoadName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDRAWQXYS_H__1A919F54_5AA2_4147_918F_CD2D4F85AA28__INCLUDED_)
