#if !defined(AFX_WXPLMDLG_H__FBAD97C6_004C_4912_8FCD_EC2A3E7681A6__INCLUDED_)
#define AFX_WXPLMDLG_H__FBAD97C6_004C_4912_8FCD_EC2A3E7681A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WxpLmDlg.h : header file
//
#include "BTGridCtrl.h"
#include "resource.h"
#include "Struct.h"
//-------------------------------------
/////////////////////////////////////////////////////////////////////////////
// WxpLmDlg dialog

class WxpLmDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(WxpLmDlg)

// Construction
public:
	WxpLmDlg();
	~WxpLmDlg();
   void UpdateWxp();
   bool Ispaint;
// Dialog Data
	//{{AFX_DATA(WxpLmDlg)
	enum { IDD = IDD_DIALOG_WXPLM };
	GV_ITEM Item;
	BTGridCtrl	m_wxp;
	BOOL	m_infP1;
	BOOL	m_infP2;
	BOOL	m_infP3;
	BOOL	m_infP4;
	BOOL	m_infP5;
	BOOL	m_infP6;
	BOOL	m_infP7;
	BOOL	m_infP8;
	CString	m_p1name;
	CString	m_p2name;
	CString	m_p3name;
	CString	m_p4name;
	CString	m_p5name;
	CString	m_p6name;
	CString	m_p7name;
	CString	m_p8name;
	//}}AFX_DATA

    WXP wxp[10];
	int wxpnum;
    bool gxinf;//栏目更新
    int WayorRamp;//匝道或公路
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(WxpLmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	   void NumberToString(int xz , ACHAR str[]);
	   double  StringToNumber( CString  str);     
protected:
	// Generated message map functions
	//{{AFX_MSG(WxpLmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONdel();
	afx_msg void OnPaint();
	afx_msg void OnRadioHighway();
	afx_msg void OnRadioRamp();
	afx_msg void OnCheckP4();
	afx_msg void OnCheckP3();
	afx_msg void OnCheckP2();
	afx_msg void OnCheckP1();
	afx_msg void OnCheckP5();
	afx_msg void OnCheckP6();
	afx_msg void OnCheckP7();
	afx_msg void OnCheckP8();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WXPLMDLG_H__FBAD97C6_004C_4912_8FCD_EC2A3E7681A6__INCLUDED_)
