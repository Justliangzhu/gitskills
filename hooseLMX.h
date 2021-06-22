#if !defined(AFX_HOOSELMX_H__2036BAE5_E396_47A4_9FCA_B9513EB65CE2__INCLUDED_)
#define AFX_HOOSELMX_H__2036BAE5_E396_47A4_9FCA_B9513EB65CE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// hooseLMX.h : header file
//
#include "JD_CENTER.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ChooseLMX dialog

class ChooseLMX : public CDialog
{
// Construction
public:
	ChooseLMX(CWnd* pParent = acedGetAcadFrame());   // standard constructor

	JD_CENTER *m_pm;
// Dialog Data
	//{{AFX_DATA(ChooseLMX)
	enum { IDD = IDD_DIALOG_CheckLmx };
	BOOL	m_L_CXDX;
	BOOL	m_L_FGDX;
	BOOL	m_L_LYDX;
	BOOL	m_L_TLJX;
	BOOL	m_L_YLJX;
	BOOL	m_Y_TLJX;
	BOOL	m_Y_CXDX;
	BOOL	m_Y_LYDX;
	BOOL	m_Y_YLJX;
	BOOL	m_Y_FGDX;
	double	m_sdml;
	double	m_edml;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ChooseLMX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ChooseLMX)
	afx_msg void OnButtonSelxl();
	afx_msg void OnBUTTONPickSdml();
	afx_msg void OnBUTTONPickEdml();
	virtual void OnOK();
	afx_msg void OnBUTTONdraw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOOSELMX_H__2036BAE5_E396_47A4_9FCA_B9513EB65CE2__INCLUDED_)
