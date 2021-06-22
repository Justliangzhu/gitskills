#if !defined(AFX_DBBZ_H__36957F1C_8B9C_431B_A0A5_8BE8D7FCC46E__INCLUDED_)
#define AFX_DBBZ_H__36957F1C_8B9C_431B_A0A5_8BE8D7FCC46E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DBBZ.h : header file
//
#include "resource.h"
#include "JD_CENTER.h"
#include "XLDataBase.h"
/////////////////////////////////////////////////////////////////////////////
// DBBZ dialog

class DBBZ : public CDialog
{
// Construction
public:
	DBBZ(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    int bzinf;//0 1 2 3 4 5 
	int dirinf;//0 1
	int xxinf;//1 2 3 4
    AcDbObjectIdArray HxIdArr;
	int NHxId;
    AcDbObjectIdArray ZxIdArr;
    int NZxId;
	JD_CENTER *m_pm;
	CString mdbname,roadname;
	XLDataBase DBS;
// Dialog Data
	//{{AFX_DATA(DBBZ)
	enum { IDD = IDD_DIALOG_DBBZ };
	CString	m_qhdis;
	CString	m_zydis;
	CString m_zhxsw;
	CString m_gcxsw;
	CString m_kdxsw;
	CString m_hpxsw;
	BOOL	m_bzdir;
	//}}AFX_DATA
     double sml,eml;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DBBZ)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DBBZ)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioDbzh();
	afx_msg void OnRadioDbsjg();
	afx_msg void OnRadioDbljg();
	afx_msg void OnRadioDbwid();
	afx_msg void OnRadioDbhp();
	afx_msg void OnRadioDbljhp();
	afx_msg void OnRadioDbpxhx();
	afx_msg void OnRadioDbczhx();
	afx_msg void OnRadioDbxx1();
	afx_msg void OnRadioDbxx2();
	afx_msg void OnRadioDbxx3();
	afx_msg void OnRadioDbxx4();
	afx_msg void OnButtonDbpickhx();
	afx_msg void OnButtonDbpickzx();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DBBZ_H__36957F1C_8B9C_431B_A0A5_8BE8D7FCC46E__INCLUDED_)
