#if !defined(AFX_ZDMTBX_H__699A3630_AA4F_4BD2_8A15_4731A0C191F1__INCLUDED_)
#define AFX_ZDMTBX_H__699A3630_AA4F_4BD2_8A15_4731A0C191F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ZDMTBX.h : header file
//
#include "BTGridCtrl.h"
#include "XLDataBase.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CZDMTBX dialog

class CZDMTBX : public CPropertyPage
{
	DECLARE_DYNCREATE(CZDMTBX)

// Construction
public:
	CZDMTBX();
	~CZDMTBX();
    
// Dialog Data
	//{{AFX_DATA(CZDMTBX)
	enum { IDD = IDD_DIALOG_ZDMTBX };
	BOOL	m_drawbx;
	//}}AFX_DATA
	BTGridCtrl m_grid;
	GV_ITEM Item;

	struct DMXCS{
		double ZJ;//左负右正 0 为中线
		double sdml;
		double edml;//起终里程
		int Color;//绘该地面线的颜色
	} DmxCsArr[50];
	int NDMX;
	CString mdbname;
	CString RoadName;
	XLDataBase DBS;
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CZDMTBX)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CZDMTBX)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDrawbx();
	afx_msg void OnButtonBxdel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ZDMTBX_H__699A3630_AA4F_4BD2_8A15_4731A0C191F1__INCLUDED_)
