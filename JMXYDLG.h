#if !defined(AFX_JMXYDLG_H__4CAFA323_34E4_4B20_BF01_F96142B820C6__INCLUDED_)
#define AFX_JMXYDLG_H__4CAFA323_34E4_4B20_BF01_F96142B820C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JMXYDLG.h : header file
//
#include "BTGridCtrl.h"
#include "resource.h"
#include"cdxCSizingDialog.h"
#include "BAS_DRAW_FUN.H"
/////////////////////////////////////////////////////////////////////////////
// CJMXYDLG dialog

class CJMXYDLG : public cdxCSizingDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CJMXYDLG(CWnd* pParent = acedGetAcadFrame(),AcDbObjectId eId=NULL);   // standard constructor

	BTGridCtrl	m_grid;
	GV_ITEM Item;
	int m_XYNum;
	AcDbObjectId m_eId;
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
// Dialog Data
	//{{AFX_DATA(CJMXYDLG)
	enum { IDD = IDD_DIALOG_JMBXINPUT };
	CString	m_XYSFwj;
	CString	m_XYSX;
	CString	m_XYSY;
	CString	m_XYFNAME;
	CString	m_XYSml;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJMXYDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
   void GetXYArrFromJDCen(JD_CENTER *pm);
   void FormDrawXYarr();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CJMXYDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonXyadd();
	afx_msg void OnButtonXydel();
	virtual void OnOK();
	afx_msg void OnButtonInputfile();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JMXYDLG_H__4CAFA323_34E4_4B20_BF01_F96142B820C6__INCLUDED_)
