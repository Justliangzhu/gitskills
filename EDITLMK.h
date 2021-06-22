#if !defined(AFX_EDITLMK_H__F3F69C77_23A7_4F43_8FFC_40D9BE287E5E__INCLUDED_)
#define AFX_EDITLMK_H__F3F69C77_23A7_4F43_8FFC_40D9BE287E5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EDITLMK.h : header file
//
#include "JD_CENTER.h"
#include "LMKGridCtrl.h"
#include "resource.h"
////////////////////////////////////////////////////////////////////////////
// CEDITLMK dialog

class CEDITLMK : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CEDITLMK(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEDITLMK)
	enum { IDD = IDD_DIALOG_EDITLMK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	LMKGridCtrl	m_grid;
	GV_ITEM Item;
    int BxNum;
	CArray<BXLMKD,BXLMKD>pBXArr;
	JD_CENTER *pm;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEDITLMK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEDITLMK)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONdel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLMK_H__F3F69C77_23A7_4F43_8FFC_40D9BE287E5E__INCLUDED_)
