#if !defined(AFX_BXLMK_H__E6B959C8_3BB9_4D2D_AD14_367DF46767D2__INCLUDED_)
#define AFX_BXLMK_H__E6B959C8_3BB9_4D2D_AD14_367DF46767D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BXLMK.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CBXLMK dialog

class CBXLMK : public CDialog
{
// Construction
public:
	CBXLMK(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBXLMK)
	enum { IDD = IDD_DIALOG_BXLMK };
	CString	m_bxedml;
	CString	m_bxsdml;
	//}}AFX_DATA

    int bxinf;//边线类型 0 中分带 1路缘带 2车行道 3硬路肩 4土路肩
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBXLMK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBXLMK)
	afx_msg void OnRadioZfd();
	afx_msg void OnRadioLyd();
	afx_msg void OnRadioCxd();
	afx_msg void OnRadioYlj();
	afx_msg void OnRadioTlj();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BXLMK_H__E6B959C8_3BB9_4D2D_AD14_367DF46767D2__INCLUDED_)
