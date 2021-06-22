#if !defined(AFX_PMDRAWDIALOG_H__A5C79A65_5ABB_4771_83A0_DF926ECAC1B0__INCLUDED_)
#define AFX_PMDRAWDIALOG_H__A5C79A65_5ABB_4771_83A0_DF926ECAC1B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PmDrawDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPmDrawDialog dialog
//平面出图设置对话框
class CPmDrawDialog : public CDialog
{
// Construction
public:
	CPmDrawDialog(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPmDrawDialog)
	enum { IDD = IDD_DIALOG_PMDRAWSETTING };
	CComboBox	m_XLCtrl;
	CString	m_StartLc;
	CString	m_Nlc;
	CString	m_QxysLR;
	CString	m_EndLc;
	int		m_Scale;
	int		m_Texth;
	CString	m_JDGH;
	int		m_JD0;
	CString	m_XLName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPmDrawDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPmDrawDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCloseupComboXlsel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PMDRAWDIALOG_H__A5C79A65_5ABB_4771_83A0_DF926ECAC1B0__INCLUDED_)
