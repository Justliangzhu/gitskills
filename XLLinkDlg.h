#if !defined(AFX_XLLINKDLG_H__DB8484BD_02D3_4872_A5C9_BED998A05800__INCLUDED_)
#define AFX_XLLINKDLG_H__DB8484BD_02D3_4872_A5C9_BED998A05800__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XLLinkDlg.h : header file
//
#include "BAS_DRAW_FUN.h"
/////////////////////////////////////////////////////////////////////////////
// XLLinkDlg dialog
//线路连接对话框
#include "JD_CENTER.h"
class XLLinkDlg : public CDialog , BAS_DRAW_FUN
{
// Construction
public:
	XLLinkDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(XLLinkDlg)
	enum { IDD = IDD_DIALOG_XLLINK };
	CButton	m_ButtonNoL;
	CButton	m_ButtonHasL;
	CEdit	m_EditA2;
	CEdit	m_EditA1;
	CButton	m_ButtonA;
	CButton	m_ButtonL;
	CButton	m_ButtonLRLModel;
	CButton	m_ButtonTUModel;
	CButton	m_ButtonZYModel;
	CButton	m_ButtonCModel;
	CButton	m_ButtonSModel;
	CButton	m_ButtonLuanModel;
	int		m_LinkModel;
	float	m_AorL1;
	float	m_AorL2;
	int		m_AorL;
	int		m_NotL;
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XLLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	//两个线路对象
	JD_CENTER *XLObj1,*XLObj2;
	//两个线路对象的ID
	AcDbObjectId XLId1,XLId2;
	
private:
	//选择线路的两点
	AcGePoint3d PT1,PT2;
	//投影到对应线路上的统一里程
	double tylc1x,tylc2x;
	//线元序号
	int XYNo1,XYNo2;
	//线元类型
	int XYClass1,XYClass2;
	int JDNo1,JDNo2;//两点分别对应的交点号
	
	double startN1,startE1,endN1,endE1;//直线起终点
	double startN2,startE2,endN2,endE2;//直线起终点
	double CenN1,CenE1,R1;//圆心及半径
	double CenN2,CenE2,R2;//圆心及半径
	double DistO1O2;//圆心距
	int LeftOrRight1,LeftOrRight2;//左右转,左转为-1，右转为+1
	void AutoModel();//计算连接方式......
	// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(XLLinkDlg)
	afx_msg void OnButtonSel1();
	afx_msg void OnButtonSel2();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLLINKDLG_H__DB8484BD_02D3_4872_A5C9_BED998A05800__INCLUDED_)
