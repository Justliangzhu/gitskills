#if !defined(AFX_INPUTZDM_H__1D8CF4F3_9EC7_4F24_B310_AFF83ED767C9__INCLUDED_)
#define AFX_INPUTZDM_H__1D8CF4F3_9EC7_4F24_B310_AFF83ED767C9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputZDM.h : header file
//
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CInputZDM dialog

class CInputZDM : public CDialog
{
// Construction
public:
	CInputZDM(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    int  DrawMode;//0 全部  1 部分
	XLDataBase  DBS;
// Dialog Data
	//{{AFX_DATA(CInputZDM)
	enum { IDD = IDD_DIALOG_DRZDMPRO };
	CComboBox	m_ZPXZCrl;
	CComboBox	m_ZtfNoCrl;
	CComboBox	m_RoadNameCrl;
	BOOL	m_BriInf;
	BOOL	m_PdxInf;
	BOOL	m_TunInf;
	CString	m_RoadName;
	CString	m_ZPXZ;
	CString	m_ZtfNo;
	CString	m_MdbName;
	double	m_OriX;
	double	m_OriY;
	BOOL	m_DmxInf;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputZDM)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputZDM)
	afx_msg void OnButtonScanzdm();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnRadioallzm();
	afx_msg void OnRadioPart();
	afx_msg void OnSelchangeCOMBOZTFNo();
	afx_msg void OnBUTTONpick();
	afx_msg void OnCheckPdx();
	afx_msg void OnCheckTun();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckBri();
	virtual void OnOK();
	afx_msg void OnCheckDmx();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTZDM_H__1D8CF4F3_9EC7_4F24_B310_AFF83ED767C9__INCLUDED_)
