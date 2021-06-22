#if !defined(AFX_PICKXDMDMX_H__1BD534C7_8FF9_4234_8641_77D9336F84D2__INCLUDED_)
#define AFX_PICKXDMDMX_H__1BD534C7_8FF9_4234_8641_77D9336F84D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PickXdmDmx.h : header file
//
#include "resource.h"
#include "NEWGRIDCTRL\GridCtrl.h"
#include "BTGridCtrl.h"
#include "BAS_DRAW_FUN.h"
#include "cdxCSizingDialog.h"
#include "JD_Center.h"
#include "XLDATABASE.h"
/////////////////////////////////////////////////////////////////////////////
// PickXdmDmx dialog

class PickXdmDmx : public cdxCSizingDialog,public BAS_DRAW_FUN
{
// Construction
public:
	PickXdmDmx(CWnd* pParen,JD_CENTER *pmLine);   // standard constructor

	BTGridCtrl	m_grid;
	GV_ITEM Item;
	int m_NDmd;
	JD_CENTER *m_pm;
    AcGePoint2dArray DmxArray;//存储地面点数据
	CString PrjName,workdir;
	int m_IfUpdateAll;
	ACHAR m_ckml[80];
	double m_dml;
	double m_GC;
	
//	int m_irxd;//绕行段 <0左绕,>0右绕,0一线
    CString m_RoadName;//道路名
	
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
// Dialog Data
	//{{AFX_DATA(PickXdmDmx)
	enum { IDD = IDD_DIALOG_DMX };
	int		m_RowSum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PickXdmDmx)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
    int AttachMdb(CString TabName);//与数据库连接
	void Set_Value();
	void Get_Value();
	void queue(double a[] , int nrow, int ncol,int inf );
	int Sort();
	void Input();
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PickXdmDmx)
	afx_msg void OnBUTTONPickMl();
	afx_msg void OnSave();
//	afx_msg void OnInput();
	afx_msg void OnButtonadd();
	afx_msg void OnButtondel();
//	afx_msg void OnBUTTONSort();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICKXDMDMX_H__1BD534C7_8FF9_4234_8641_77D9336F84D2__INCLUDED_)
