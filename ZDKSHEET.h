#if !defined(AFX_ZDKSHEET_H__BD335E8C_2FB3_42A2_B327_5519730E10CE__INCLUDED_)
#define AFX_ZDKSHEET_H__BD335E8C_2FB3_42A2_B327_5519730E10CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDKSHEET.h : header file
//
#include "ZDKINPUT.h"
/////////////////////////////////////////////////////////////////////////////
// CZDKSHEET

class CZDKSHEET : public CPropertySheet
{
	DECLARE_DYNAMIC(CZDKSHEET)

// Construction
public:
	CZDKSHEET(GTZDK_ROAD *pzdk,AcDbObjectId ZID,UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CZDKSHEET(GTZDK_ROAD *pzdk,AcDbObjectId ZID,LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
   CZDKINPUT *Left;
//   CZDKINPUT *Right;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZDKSHEET)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CZDKSHEET();
	// Generated message map functions
protected:
	//{{AFX_MSG(CZDKSHEET)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDKSHEET_H__BD335E8C_2FB3_42A2_B327_5519730E10CE__INCLUDED_)
