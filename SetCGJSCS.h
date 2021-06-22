#if !defined(AFX_SETCGJSCS_H__A442908B_BF93_433F_A574_8109E09AEB7B__INCLUDED_)
#define AFX_SETCGJSCS_H__A442908B_BF93_433F_A574_8109E09AEB7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCGJSCS.h : header file
//
#include "BAS_DRAW_FUN.h"
#include "JD_CENTER.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// SetCGJSCS dialog

class SetCGJSCS : public CDialog , public BAS_DRAW_FUN
{
// Construction
public:
	SetCGJSCS(JD_CENTER *pm,CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(SetCGJSCS)
	enum { IDD = IDD_DIALOG_CGCS };
	CComboBox	m_CgMode;
	CComboBox	m_CgLenMode;
	CString	m_HxCgJbl;	//���Ͷ���С���߽�����
	CString	m_ZxCgJbl;	//ֱ�߶γ��߽�����
	CString	m_JKMode;	//�ӿ�ʽ
	CString	m_flblen;
	CString	m_hlblen;
	JD_CENTER *m_pm;
	//}}AFX_DATA
    int CgMode;//0 1 2 3 4 5;
	int CgLenMode;//0-��ȡ��;1-1��������";"2-5��������	���߻��Ͷγ���ȡ��
    int Vj;//����
	CString mdbname;
	CString roadname;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetCGJSCS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SetCGJSCS)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombocglen();
	afx_msg void OnSelchangeCombocgmode();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCGJSCS_H__A442908B_BF93_433F_A574_8109E09AEB7B__INCLUDED_)
