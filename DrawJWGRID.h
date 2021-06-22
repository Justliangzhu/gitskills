#if !defined(AFX_DRAWJWGRID_H__93B8F43F_2E21_4C77_8757_F577C5EED3ED__INCLUDED_)
#define AFX_DRAWJWGRID_H__93B8F43F_2E21_4C77_8757_F577C5EED3ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawJWGRID.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// DrawJWGRID dialog

class DrawJWGRID : public CDialog,BAS_DRAW_FUN
{
// Construction
public:
	DrawJWGRID(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    JD_CENTER *pm;
	CString m_Dbs;
	XLDataBase DBS;
// Dialog Data
	//{{AFX_DATA(DrawJWGRID)
	enum { IDD = IDD_DIALOG_BJWGRID };
	BOOL	m_anxl;
	CString	m_prex;
	CString	m_prey;
	double	m_CrossLen;
	double	m_Hspacer;
	double	m_Lbx;
	double	m_Lby;
	double	m_Rux;
	double	m_Ruy;
	double	m_Vspacer;
	double	m_Wordht;
	double	m_Distoxl;
	//}}AFX_DATA

   void DRAWGRID();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DrawJWGRID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DrawJWGRID)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONpick();
	afx_msg void OnCheckAnxl();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWJWGRID_H__93B8F43F_2E21_4C77_8757_F577C5EED3ED__INCLUDED_)
