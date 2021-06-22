#if !defined(AFX_DIRDLGDLG_H__2C4F6C82_03DE_439C_867F_EF0BCAD7FAEA__INCLUDED_)
#define AFX_DIRDLGDLG_H__2C4F6C82_03DE_439C_867F_EF0BCAD7FAEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DirdlgDlg.h : header file
//
#include "resource.h"
#include "Shelltree.h"
/////////////////////////////////////////////////////////////////////////////
// CDirdlgDlg dialog

class CDirdlgDlg : public CDialog 
{
// Construction
public:
	CDirdlgDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	void CreateDir();
	void SelectDir();
	bool IsRightDir();
	void NullInStrto$(CString& text);
	void InStr$toNull(CString& text);
	BOOL ReadRoadProperty(CString mdbname);
	BOOL WriteRoadProperty(CString mdbname);
	CString GetRailNameFromPath(CString MdbName);
	int DirMode;//0新建项目 1选择项目
	CString InsertRail;
// Dialog Data
	//{{AFX_DATA(CDirdlgDlg)
	enum { IDD = IDD_DIALOG_DIRDLG };
	CShellTree	m_treeExplorer;
	CString	m_dirstr;
	CString	m_ProNo;
	CString	m_Grade;
	CString	m_TerrainType;
	CString	m_DV;
	CString	m_DXLB;
	CString m_RailMdbPath;	//20200522新增	铁路方案路径
	//}}AFX_DATA
	
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirdlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDirdlgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//afx_msg void OnItemexpandingTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnSelchangedTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnRclickTreeExplorer(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONBrowser();	//浏览道路方案
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonbrowser2();	//浏览铁路方案
	afx_msg void OnBnClickedButtonpickup();

	afx_msg void OnBnClickedButtonLlgcml();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRDLGDLG_H__2C4F6C82_03DE_439C_867F_EF0BCAD7FAEA__INCLUDED_)
