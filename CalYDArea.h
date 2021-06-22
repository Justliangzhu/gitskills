#if !defined(AFX_CALYDAREA_H__B0CEEC9D_6AF5_4F6D_9076_44C474A93898__INCLUDED_)
#define AFX_CALYDAREA_H__B0CEEC9D_6AF5_4F6D_9076_44C474A93898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CalYDArea.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "OutTAB.h"
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CCalYDArea dialog

class CCalYDArea : public CDialog
{
// Construction
public:
	CCalYDArea(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    XLDataBase DBS;

// Dialog Data
	//{{AFX_DATA(CCalYDArea)
	enum { IDD = IDD_DIALOG_YDAREA };
	CComboBox	m_RoadNameCrl;
	CString	m_EndDml;
	CString	m_mdbname;
	CString	m_YdArea;
	CString	m_StartDml;
	CString	m_RoadName;
	BOOL	m_outbykm;
	//}}AFX_DATA

	struct ZDKDAT
	{
		double cml;
		double LZDK;
		double RZDK;
		int IsInsertHdm;
		//		double OutPt[2];//占地界外点用于删除实体
	};
	struct RoadData
	{
	    JD_CENTER  *pm;
		ZDKDAT *pZDK;
		int NZDK;
	}RoadArr[50];//最多49个分离段
	int NRoad;

	OutTAB tab;
/*
	struct YdByKm
	{
		double zarea;
		double yarea;
        CString lcfw;
	};
	CArray<YdByKm,YdByKm>pYdByKm;*/

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCalYDArea)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCalYDArea)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonYdbrowser();
	afx_msg void OnButtonpickxl();
	afx_msg void OnSelchangeComboYdroadname();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALYDAREA_H__B0CEEC9D_6AF5_4F6D_9076_44C474A93898__INCLUDED_)
