#if !defined(AFX_HDMDAOHANG_H__11C74EFB_3047_4D1A_A2E7_158379097031__INCLUDED_)
#define AFX_HDMDAOHANG_H__11C74EFB_3047_4D1A_A2E7_158379097031__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HdmDaoHang.h : header file
//
#include "BtnST.h"
#include "FlatComboBox.h"
#include "HdmDes.h"
#include "resource.h"
#include "XlDataBase.h"
#include "BAS_DRAW_FUN.h"
#include "HdmDataBase.h"

/////////////////////////////////////////////////////////////////////////////
// CHdmDaoHang dialog

class CHdmDaoHang : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CHdmDaoHang(CWnd* pParent = NULL);   // standard constructor
	~CHdmDaoHang();
 
	int xh;

	int RoadSum;
    struct Hdmdata
	{
       ACHAR ckml[30];
	   double x1,y1,x2,y2;
    };

	Hdmdata *hdm;
	
	void Zoom(double x1,double y1,double x2,double y2);
	void GengZhong(CString xl,CString ckml);
	void Close();

	
// Dialog Data
	//{{AFX_DATA(CHdmDaoHang)
	enum { IDD = IDD_GENZHONG };
	CFlatComboBox	m_XL;
	CFlatComboBox	m_SeletLC;
	CButtonST	m_pre;
	CButtonST	m_next;
	CButtonST	m_last;
	CButtonST	m_firt;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHdmDaoHang)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHdmDaoHang)
	afx_msg void OnFist();
	afx_msg void OnLast();
	afx_msg void OnNext();
	afx_msg void OnPre();
	afx_msg void OnEditchangeLc();
	afx_msg void OnSelchangeLc();
	afx_msg void OnEditchangeXlnx();
	afx_msg void OnSelchangeXlnx();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HDMDAOHANG_H__11C74EFB_3047_4D1A_A2E7_158379097031__INCLUDED_)
