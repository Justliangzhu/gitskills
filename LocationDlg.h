#if !defined(AFX_LOCATIONDLG_H__3A9787E6_2EC4_452F_887D_A994B4E7C055__INCLUDED_)
#define AFX_LOCATIONDLG_H__3A9787E6_2EC4_452F_887D_A994B4E7C055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocationDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LocationDlg dialog
#include "resource.h"
class LocationDlg : public CDialog
{
// Construction
public:
	LocationDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(LocationDlg)
	enum { IDD = IDD_DIALOG_LOCATION2 };
	double	m_A1;
	double	m_A2;
	int m_AorL;
	double	m_Radius;
	AcDbObjectId xlId;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LocationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	double Rmax;
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LocationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonQxb();
	afx_msg void OnButtonTtr();
	afx_msg void OnButtonTpr();
	afx_msg void OnButtonTpp();
	afx_msg void OnButtonPpp();
	afx_msg void OnButtonInvert();
	afx_msg void OnButtonLink();
	afx_msg void OnRadioA();
	afx_msg void OnRADIOLo();
	afx_msg void OnBUTTONrefresh();
	afx_msg void OnMENUITEMRoadXZ();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOCATIONDLG_H__3A9787E6_2EC4_452F_887D_A994B4E7C055__INCLUDED_)
