#if !defined(AFX_JDCENTERINPUTDLG_H__E16D918D_6319_426B_86C7_B325D87B7CDC__INCLUDED_)
#define AFX_JDCENTERINPUTDLG_H__E16D918D_6319_426B_86C7_B325D87B7CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// JdCenterInputDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// JdCenterInputDlg dialog
//平面数据录入对话框
#include "JdCenGridCtrl.h"
#include "resource.h"
#include "cdxCSizingDialog.h"
#include "JD_CENTER.H"
#include "EXMANAGER.H"
class JdCenterInputDlg : public cdxCSizingDialog
{
// Construction
public:
	void INIT();
	JdCenterInputDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	CString m_FenLiDuan;
	bool ShowTestDrawing;
	bool DrawFlag;//控制自定义实体与对话框同步刷新
	CString m_rname;
private:
	
	int RowNum;
	GV_ITEM Item;	
	
// Dialog Data
	//{{AFX_DATA(JdCenterInputDlg)
	enum { IDD = IDD_DIALOG_JDCENTERINPUT };
	BOOL	m_GZXS;
	BOOL	m_DRAWJD;
	BOOL	m_ISLo;
	CString	m_StartLC;
	CString	m_JDGH;
	int		m_JD0;
	CButton	m_TestDrawing;
	int		m_addrows;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JdCenterInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	JD_CENTER* JdCenObj;
	AcDbObjectId JdCenId;
	/*virtual void OnBnClickedOk();*/
	
/*
	EXMANAGER *EXObj;
	AcDbObjectId EXId;
*/
// Implementation
protected:
	void UpdateGrid();
		
	// Generated message map functions
	//{{AFX_MSG(JdCenterInputDlg)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonInsert();
	afx_msg void OnButtonGetpt();
	afx_msg void OnCheckGzxs();
	afx_msg void OnButtonInData();
	afx_msg void OnButtonOutData();
	afx_msg void OnPaint();
	afx_msg void OnCHECKLo();
///	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	JdCenGridCtrl m_grid;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JDCENTERINPUTDLG_H__E16D918D_6319_426B_86C7_B325D87B7CDC__INCLUDED_)
