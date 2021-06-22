// PPCDSheet.h: interface for the CPPCDSheet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PPCDSHEET_H__75E0C2A3_DBCA_4114_B2C1_94C6CB84CBD6__INCLUDED_)
#define AFX_PPCDSHEET_H__75E0C2A3_DBCA_4114_B2C1_94C6CB84CBD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JD_CENTER.h"
#include "PPCDsj.h"
#include "xldatabase.h"
class CPPCDSheet : public CPropertySheet,BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(CPPCDSheet)
		
		// Construction
public:
	CPPCDSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPPCDSheet(JD_CENTER *pm,LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	
	// Attributes
public:
    PPCDsj Zppcd,Yppcd;
	JD_CENTER *m_pm;
	void SetZYPPCDInitData();//设置左右爬坡车道数据
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPPCDSheet)
public:
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal();
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CPPCDSheet();
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CPPCDSheet)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PPCDSHEET_H__75E0C2A3_DBCA_4114_B2C1_94C6CB84CBD6__INCLUDED_)
