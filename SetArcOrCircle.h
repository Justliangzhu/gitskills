#if !defined(AFX_SETARCORCIRCLE_H__017512CB_8377_47B9_81DC_FF6EB8C5AF3C__INCLUDED_)
#define AFX_SETARCORCIRCLE_H__017512CB_8377_47B9_81DC_FF6EB8C5AF3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetArcOrCircle.h : header file
//
#include "geassign.h"
#include "BAS_DRAW_FUN.h"
#include "JD_CENTER.H"
#include "BlockComputerFun.h"
/////////////////////////////////////////////////////////////////////////////
// CSetArcOrCircle dialog

class CSetArcOrCircle : public CDialog,public BlockComputerFun
{
// Construction
public:
	CSetArcOrCircle(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	CSetArcOrCircle(double v1,double v2,double v3,double v4,
	int type,AcDbObjectId InputEntId=0,CWnd* pParent = acedGetAcadFrame());
	CSetArcOrCircle(AcDbObjectId EntityId,CWnd* pParent = acedGetAcadFrame());
// Dialog Data
	//{{AFX_DATA(CSetArcOrCircle)
	enum { IDD = IDD_DIALOGSETARC };
	double	m_pinajv;
	int		m_biance;
	double	m_qdpianjiao;
	double	m_qxpzj;
	double	m_qxbj;
	double	m_qxcd;
	int		m_qxzyp;
	int		m_qdzyp;
	// 起点是否滑动;
	BOOL	m_FreeOrNot;
	//}}AFX_DATA
	double Vbiance;
	double Vpianxiang;
	double Vpapianxiang;
	//
	double m_var1,m_var2,m_var3,m_var4;
	int itemtype;
	
	int m_XYNum;
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
	//
	AcDbArc *pArc;
	AcDbCircle * pCircle;
	AcDbObjectId CirId;
	AcDbObjectId ArcId;
	//
	double arrxy[3][10];
	//
	AcDbObjectId JCId;
	BOOL CreatTOrModifyF;
	double ClickCout;
	//被选中实体的ID;
	AcDbObjectId GottnEntId;
	//定义被选中类型;
	int SelItem;
	AcGePoint3d PT;
	//计算单元变量;
	RampItem CompItem;
	RampItem InterItem;
	//是否计算计算单元;
	BOOL ComputeOrnot;
	//是否能创建单元变量;
	BOOL CanCreatRampItem;
	//圆弧单元的起点数据信息;
	double ArcSX,ArcSY,ArcSOA,ArcRad,ArcLen;
	// 缓曲线终点半径
	double SprEndR;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetArcOrCircle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetArcOrCircle)
	afx_msg void OnRadiozouce();
	afx_msg void OnRadioyouce();
	afx_msg void OnRadiozoupian();
	afx_msg void OnRadioyoupian();
	afx_msg void OnRadiopazoupian();
	afx_msg void OnRadiopayoupian();
	afx_msg void OnButtonsaxiandian();
	afx_msg void OnSaok();
	afx_msg void OnSacancel();
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam,LPARAM lParam);
	afx_msg void OnChecksafreeset();
	afx_msg void OnButtonsaupdatedata();
	afx_msg void OnChangeEditsapaqxbj();
	afx_msg void OnChangeEditsapapianzhuanjiao();
	afx_msg void OnChangeEditsapaqxcd();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETARCORCIRCLE_H__017512CB_8377_47B9_81DC_FF6EB8C5AF3C__INCLUDED_)





















