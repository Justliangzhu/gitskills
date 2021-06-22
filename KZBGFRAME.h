#if !defined(AFX_KZBGFRAME_H__25AA16DB_0D2A_4CA2_A8DF_C4DE70C33F98__INCLUDED_)
#define AFX_KZBGFRAME_H__25AA16DB_0D2A_4CA2_A8DF_C4DE70C33F98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KZBGFRAME.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// KZBGFRAME dialog
#include "resource.h"
class KZBGFRAME : public CDialog
{
// Construction
public:
	KZBGFRAME(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(KZBGFRAME)
	enum { IDD = IDD_DIALOG_FRAME };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(KZBGFRAME)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(KZBGFRAME)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KZBGFRAME_H__25AA16DB_0D2A_4CA2_A8DF_C4DE70C33F98__INCLUDED_)
