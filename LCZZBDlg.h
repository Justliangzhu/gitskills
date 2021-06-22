#if !defined(AFX_LCZZBDLG_H__7D27BF81_29E3_45B8_8F9B_D1340E612361__INCLUDED_)
#define AFX_LCZZBDLG_H__7D27BF81_29E3_45B8_8F9B_D1340E612361__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LCZZBDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CLCZZBDlg dialog
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "resource.h"
#include "OutTAB.h"

class CLCZZBDlg : public CDialog
{
	// Construction
public:
	CLCZZBDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	void Release();
	CString pmFname;
	JD_CENTER  *pm;
    XLDataBase DBS;
	int m_mode;//平面方案方式 0 文件方式 new GTZX 要释放；  1 图选方式
	int SingleorPL;//单个或批量 0 单个 1 批量
	AcDbObjectId GTZXId;//GTZXId

	struct LCZB{
		ACHAR  Ckml[25];
		double x,y;
	};
	
	CArray<LCZB,LCZB>LCZBArr;
	int NLCZB;
	
	_Application m_app;
	Workbooks    m_workbooks;
	Worksheets   m_worksheets;
	_Workbook    m_workbook;
	_Worksheet   m_worksheet;
    Sheets       m_sheets;
	
	void  FillData();
	//往EXCLE的CELL中写数据
	bool WriteInExcel(Range rgMyRge,long Line,long Col,_variant_t Value);
	
	// Dialog Data
	//{{AFX_DATA(CLCZZBDlg)
	enum { IDD = IDD_DIALOGZM_LCTOZB };
	CComboBox	m_RoadName;
	CString	m_ROAD;
	CString mdbname;
	CString	m_XCLC;
	CString	m_ENDCKML;
	CString	m_STARTCKML;
	double	m_SPACER;
	BOOL	m_infdrawlc;
	//}}AFX_DATA
	double PtN,PtE;
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCZZBDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CLCZZBDlg)
	afx_msg void OnButtonMdb();
	afx_msg void OnBUTTONSet();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGetpoint();
	afx_msg void OnRadiosingle();
	afx_msg void OnRadioplout();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnCheckDrawlc();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_LCZZBDLG_H__7D27BF81_29E3_45B8_8F9B_D1340E612361__INCLUDED_)
