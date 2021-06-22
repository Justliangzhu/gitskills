#if !defined(AFX_SETLINEDLG1_H__320ECD81_7666_4604_99DD_C1A7D8DFDF8E__INCLUDED_)
#define AFX_SETLINEDLG1_H__320ECD81_7666_4604_99DD_C1A7D8DFDF8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetLineDlg1.h : header file
//
#include "geassign.h"
#include "BAS_DRAW_FUN.h"
#include "jd_center.h"
#include "BlockComputerFun.h"
/////////////////////////////////////////////////////////////////////////////
// CSetLineDlg dialog

class CSetLineDlg : public CDialog,public BlockComputerFun
{
// Construction
public:
	CSetLineDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	CSetLineDlg(AcDbObjectId EnId,CWnd* pParent = acedGetAcadFrame());
	// Dialog Data
	//{{AFX_DATA(CSetLineDlg)
	enum { IDD = IDD_DIALOGSETLINE };
	CButton	m_CheckFreeset;
	double	m_pianjiao;
	double	m_pianjv;
	double	m_slcd;
	double	m_slex;
	double	m_sley;
	double	m_slfwj;
	double	m_slsx;
	double	m_slsy;
	int		m_cixiang;
	int		m_pianxiang;
	BOOL	m_FreeOrNot;
	//}}AFX_DATA	
	int Vcexiang,Vpianxiang;
	//
	AcDbObjectId JCId;
	//2005.5.31
	BOOL CreatTOrModifyF;
	double ClickCout;
	// 定义计算线元变量;
	RampItem ComItem,InterItem;
	// 定义被选中单元的ID;
	AcDbObjectId InputEntId;
	// 定义被选中单元的类型变量;
	int SelItem;
	// 定义直线实体指针及ID;
	AcDbLine * pLine;
	AcDbObjectId LineId;
	AcGePoint3d point;
	// 被添入匝道实体中的线元数组;
	double RouteArray[3][10];
	// 是否能创建匝道单元;
	BOOL CanCreatRampItem;

	int m_XYNum;
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetLineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetLineDlg)
	afx_msg void OnSlok();
	afx_msg void OnSlcancel();
	afx_msg void OnButtonsme();
	afx_msg void OnButtonsms();
	afx_msg void OnRadioyouce();
	afx_msg void OnRadioyoupian();
	afx_msg void OnRadiozouce();
	afx_msg void OnRadiozoupian();
	afx_msg void OnButtonslsd();
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonupdate();
	afx_msg void OnCheckfreeset();
	afx_msg void OnButsetsdot();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETLINEDLG1_H__320ECD81_7666_4604_99DD_C1A7D8DFDF8E__INCLUDED_)
