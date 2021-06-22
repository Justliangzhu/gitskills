#if !defined(AFX_GOUPDSHEET_H__0AEC43CB_4AE2_424D_B3DA_5199BBDB2483__INCLUDED_)
#define AFX_GOUPDSHEET_H__0AEC43CB_4AE2_424D_B3DA_5199BBDB2483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GouPDSheet.h : header file
//
#include "GouPDPage.h"
/////////////////////////////////////////////////////////////////////////////
// GouPDSheet

class GouPDSheet : public CPropertySheet,BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(GouPDSheet)

// Construction
public:
	GouPDSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	GouPDSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

    struct RoadGouPDdata
	{
		CString roadname,ZGouPDName,YGouPDName;//µÀÂ·Ãû
        JD_CENTER *pm;
	    int NZGouPD,NYGouPD;
		CArray<GouPDdata,GouPDdata>ZGouPD;
		CArray<GouPDdata,GouPDdata>YGouPD;
	} ;
	RoadGouPDdata *road;

	GouPDPage *GouPDDlgAry;
	int xlnum;
	CString mdbFname;
	BOOL INIT(); 
// Attributes
public:
		virtual BOOL OnInitDialog();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GouPDSheet)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GouPDSheet();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(GouPDSheet)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOUPDSHEET_H__0AEC43CB_4AE2_424D_B3DA_5199BBDB2483__INCLUDED_)
