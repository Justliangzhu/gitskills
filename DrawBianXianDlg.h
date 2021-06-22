// DrawBianXianDlg.h : header file
// 绘制线路偏置边线对话框 2005.07.12 ldf
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWBIANXIANDLG_H__E4213BC6_D45E_4A40_AB70_1ABD4C5E1073__INCLUDED_)
#define AFX_DRAWBIANXIANDLG_H__E4213BC6_D45E_4A40_AB70_1ABD4C5E1073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JD_CENTER.H"

/////////////////////////////////////////////////////////////////////////////
// 绘制线路偏置边线对话框

class CDrawBianXianDlg : public CDialog
{
// Construction
public:
	CDrawBianXianDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawBianXianDlg)
	enum { IDD = IDD_DRAW_BX };
	CString	m_strStyle;  // 偏线类型
	CString	m_strMethod; // 偏置方式
	double	m_dScml;     // 起始里程
	double	m_dEcml;     // 终止里程
	CString	m_strSWidth; // 边线起点偏置宽度
	CString	m_strEWidth; // 边线终点偏置宽度		
	double	m_dStep;     // 绘制步长
	//}}AFX_DATA

	JD_CENTER*   m_pOffsetRoad;  // 偏线实体指针
	AcDbObjectId m_OffsetRoadID; // 偏线ID
	double       m_dBaseProMile; // 基点(偏线拾取点)投影里程
	CArray<double,double> m_SWidth, m_EWidth; // 边线偏置起终宽度序列


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawBianXianDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool GetOffsetRoad();      // 实时获得偏线
	void GetCml(double& dCml); // 拾取桩号
	bool GetOffSetWidth(CString strWidth, CArray<double,double>& arrays); // 获得偏置宽度

	// Generated message map functions
	//{{AFX_MSG(CDrawBianXianDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGetRoad();
	afx_msg void OnButtonGetSml();
	afx_msg void OnButtonGetEml();
	virtual void OnOK();
	afx_msg void OnSelendokComboMethod();
	afx_msg void OnSelendokComboSw();
	afx_msg void OnSelendokComboStyle();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWBIANXIANDLG_H__E4213BC6_D45E_4A40_AB70_1ABD4C5E1073__INCLUDED_)
