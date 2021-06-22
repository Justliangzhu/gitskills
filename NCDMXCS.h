#if !defined(AFX_NCDMXCS_H__C4CF796D_E5E5_4B38_A81A_9C1CB4220894__INCLUDED_)
#define AFX_NCDMXCS_H__C4CF796D_E5E5_4B38_A81A_9C1CB4220894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NCDMXCS.h : header file
//
#include "BTGridCtrl.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CNCDMXCS dialog

class CNCDMXCS : public CDialog//内插地面线参数
{
// Construction
public:
	CNCDMXCS(double sdml, double edml,CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNCDMXCS)
	enum { IDD = IDD_DIALOG_NCDMXCS };
	CString	m_ncspacer;
	int		m_ifdtm;
	//}}AFX_DATA

	BTGridCtrl m_grid;
	GV_ITEM Item;
    
	double m_sdml,m_edml;//该线路的起终里程
	struct DMXCS{
		double ZJ;//左负右正 0 为中线
		double sdml;
		double edml;//起终里程
		int Color;//绘该地面线的颜色
	} DmxCsArr[50];
	int NDMX,izdm;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNCDMXCS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNCDMXCS)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNcadd();
	afx_msg void OnButtonNcdel();
	virtual void OnOK();
	afx_msg void OnButtonPickzdmtk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCDMXCS_H__C4CF796D_E5E5_4B38_A81A_9C1CB4220894__INCLUDED_)
