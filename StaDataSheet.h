#if !defined(AFX_STADATASHEET_H__DFBB8E46_C370_4502_B646_AC7B694A084E__INCLUDED_)
#define AFX_STADATASHEET_H__DFBB8E46_C370_4502_B646_AC7B694A084E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaDataSheet.h : header file
//
#include "CalSta.h"
/////////////////////////////////////////////////////////////////////////////
// StaDataSheet

class StaDataSheet : public CPropertySheet,BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(StaDataSheet)

// Construction
public:
	struct RoadSTAdata
	{
		CString roadname;//µÀÂ·Ãû
        JD_CENTER *pm;
	    int NSTA;
		CArray<LCBdata,LCBdata>lcb;
	};
	CArray<GLTZSTAdata,GLTZSTAdata>GLTZlcb;
	double SmlOnZx[50],EmlOnZx[50], NoseSmlonZX[50],NoseSmlonFld[50],NoseEmlonZX[50],NoseEmlonFld[50];
	
	RoadSTAdata *road;
    void AddLcToArr(CCalSta &psta, double &cml,double glcml,CString glRoadName);
	void CalTZSTA(); 
	StaDataSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	StaDataSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

	CCalSta *StaDlgAry;
	int xlnum;
	CString mdbFname;

// Attributes
public:

// Operations
public:
   BOOL INIT() ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StaDataSheet)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
    void  Adjust();
// Implementation
public:
	virtual ~StaDataSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(StaDataSheet)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STADATASHEET_H__DFBB8E46_C370_4502_B646_AC7B694A084E__INCLUDED_)
