#if !defined(AFX_DELDBBZ_H__1B6DD288_6548_40A9_BB1B_A7F622CE894A__INCLUDED_)
#define AFX_DELDBBZ_H__1B6DD288_6548_40A9_BB1B_A7F622CE894A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DELDBBZ.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CDELDBBZ dialog

class CDELDBBZ : public CDialog
{
// Construction
public:
	CDELDBBZ(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	XLDataBase DBS;
// Dialog Data
	//{{AFX_DATA(CDELDBBZ)
	enum { IDD = IDD_DIALOG_DELDBBZ };
	CString	m_HxZh;
	BOOL	m_KdBz;
	BOOL	m_LjDesh;
	BOOL	m_LmDesh;
	BOOL	m_LmHp;
	BOOL	m_TljHp;
	BOOL	m_ZhBz;
	//}}AFX_DATA

   int m_bc;//-1 ×ó 1 ÓÒ 0 Á½²à
   CString mdbname,roadname,hxbh;
   AcDbObjectIdArray IdArr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDELDBBZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDELDBBZ)
	afx_msg void OnButtonPickzh();
	afx_msg void OnRadioLeft();
	afx_msg void OnRADIORight();
	afx_msg void OnRadioBoth();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DELDBBZ_H__1B6DD288_6548_40A9_BB1B_A7F622CE894A__INCLUDED_)
