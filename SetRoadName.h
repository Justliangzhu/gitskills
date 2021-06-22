#if !defined(AFX_SETROADNAME_H__A85D9454_D71D_4842_967D_1CDF7F966973__INCLUDED_)
#define AFX_SETROADNAME_H__A85D9454_D71D_4842_967D_1CDF7F966973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetRoadName.h : header file
//
#include "XLDataBase.h"
/////////////////////////////////////////////////////////////////////////////
// CSetRoadName dialog

class CSetRoadName : public CDialog
{
// Construction
public:
	CSetRoadName(CString name,CWnd* pParent = acedGetAcadFrame());   // standard constructor

   CString mdbname,RoadName;
   XLDataBase DBS;
// Dialog Data
	//{{AFX_DATA(CSetRoadName)
	enum { IDD = IDD_DIALOG_ZDRoadName };
	CComboBox	m_ZPXZCrl;
	CComboBox	m_RoadName;
	CString	m_ZPXZ;
	//}}AFX_DATA

    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetRoadName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetRoadName)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOSetRoadName();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETROADNAME_H__A85D9454_D71D_4842_967D_1CDF7F966973__INCLUDED_)
