#if !defined(AFX_FLOATMENU_H__BAC97CC4_A6C1_4AE7_B1F2_AC0140C0BE16__INCLUDED_)
#define AFX_FLOATMENU_H__BAC97CC4_A6C1_4AE7_B1F2_AC0140C0BE16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FLOATMENU.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FLOATMENU dialog

class FLOATMENU : public CDialog
{
// Construction
public:
	FLOATMENU(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(FLOATMENU)
	enum { IDD = IDD_DIALOG_MENU };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FLOATMENU)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FLOATMENU)
	afx_msg void OnMenuitemLocationtool();
	afx_msg void OnMenuitemNewxm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLOATMENU_H__BAC97CC4_A6C1_4AE7_B1F2_AC0140C0BE16__INCLUDED_)
