#if !defined(AFX_CHECKTOOLBAR_H__729FCCD1_938A_468D_98AD_5233FD4A9BDA__INCLUDED_)
#define AFX_CHECKTOOLBAR_H__729FCCD1_938A_468D_98AD_5233FD4A9BDA__INCLUDED_
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "MultiCheckListBox.h"






/////////////////////////////////////////////////////////////////////////////
// CCheckToolBar dialog

class CCheckToolBar : public CDialog
{
// Construction
public:
	CCheckToolBar(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CCheckToolBar)
	enum { IDD = IDD_CheckToolBar };
	CCheckListBox	m_checklist;


	//}}AFX_DATA
    

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckToolBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
		//CCheckListBox m_list;
// Implementation

protected:

	// Generated message map functions
	//{{AFX_MSG(CCheckToolBar)
	afx_msg void OnOk();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList1();
	afx_msg void OnDblclkList1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKTOOLBAR_H__729FCCD1_938A_468D_98AD_5233FD4A9BDA__INCLUDED_)
