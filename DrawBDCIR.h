#if !defined(AFX_DRAWBDCIR_H__C6A1CF2D_4A36_4564_8E72_788A589983CA__INCLUDED_)
#define AFX_DRAWBDCIR_H__C6A1CF2D_4A36_4564_8E72_788A589983CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawBDCIR.h : header file
//
#include "resource.h"
#include "JD_CENTER.h"

/////////////////////////////////////////////////////////////////////////////
// DrawBDCIR dialog

class DrawBDCIR : public CDialog,BAS_DRAW_FUN
{
// Construction
public:
	DrawBDCIR(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    bool ReadBDFile();
	int FindBDXH();
	JD_CENTER *m_pm;
	struct BDData BDArr[100];
	int NBIDUAN;
	int InorOut;//0·ÖÁ÷ 1 »æÁ÷
// Dialog Data
	//{{AFX_DATA(DrawBDCIR)
	enum { IDD = IDD_DIALOG_BDYHZ };
	CString	m_XL1;
	CString	m_XL2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DrawBDCIR)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DrawBDCIR)
	afx_msg void OnButtonPickxl1();
	afx_msg void OnButtonPickxl2();
	afx_msg void OnRadioDrawflb();
	afx_msg void OnRadioDrawhlb();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWBDCIR_H__C6A1CF2D_4A36_4564_8E72_788A589983CA__INCLUDED_)
