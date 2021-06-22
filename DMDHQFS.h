#if !defined(AFX_DMDHQFS_H__A4F62B81_F75B_11D7_AF19_006067772FF4__INCLUDED_)
#define AFX_DMDHQFS_H__A4F62B81_F75B_11D7_AF19_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMDHQFS.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// DMDHQFS dialog

class DMDHQFS : public CDialog
{
// Construction
public:
	DMDHQFS(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	
   CButton  *m_buttonFile,*m_buttonDmx;
   CString DMXFname;
    int NCinf;//数模内插还是从地面线表读取
// Dialog Data
	//{{AFX_DATA(DMDHQFS)
	enum { IDD = IDD_DIALOGDMD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMDHQFS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMDHQFS)
	afx_msg void OnRadiofile();
	afx_msg void OnRadiodtm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMDHQFS_H__A4F62B81_F75B_11D7_AF19_006067772FF4__INCLUDED_)
