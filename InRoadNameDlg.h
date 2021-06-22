#if !defined(AFX_INROADNAMEDLG_H__3E8D369B_C585_4F74_B98D_8D934654FF25__INCLUDED_)
#define AFX_INROADNAMEDLG_H__3E8D369B_C585_4F74_B98D_8D934654FF25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InRoadNameDlg.h : header file
//
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// InRoadNameDlg dialog

class InRoadNameDlg : public CDialog
{
// Construction
public:
	InRoadNameDlg(AcDbObjectId pmId,CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(InRoadNameDlg)
	enum { IDD = IDD_DIALOG_roadname };
	CString	m_RoadName;
	CString	m_path;
	JD_CENTER *INpm;
	AcDbObjectId eId;
	double BLC,ELC;//起点断前里程，终点断后里程
	CString BGH,EGH;//起点断前冠号，终点断后冠号
	int mode;//0主线，-1，左线，1 右线
	bool IfSetRoadName;
		
		
		
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InRoadNameDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InRoadNameDlg)
	afx_msg void OnRADIOLftLine();
	afx_msg void OnRADIOMainLine();
	afx_msg void OnRADIORtLine();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONbrowser();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INROADNAMEDLG_H__3E8D369B_C585_4F74_B98D_8D934654FF25__INCLUDED_)
