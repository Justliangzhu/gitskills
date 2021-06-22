#if !defined(AFX_SETSPRIALDLG_H__5972586B_5FB0_4B41_8F36_4127A0D06D0F__INCLUDED_)
#define AFX_SETSPRIALDLG_H__5972586B_5FB0_4B41_8F36_4127A0D06D0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetSprialdlg.h : header file
//
#include "geassign.h"
#include "BAS_DRAW_FUN.h"
#include "jd_center.h"
#include "BlockComputerFun.h"
/////////////////////////////////////////////////////////////////////////////
// SetSprialdlg dialog

class SetSprialdlg : public CDialog,public BlockComputerFun
{
// Construction
public:
	SetSprialdlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	SetSprialdlg(AcDbObjectId EntityId,CWnd* pParent = acedGetAcadFrame());   	
// Dialog Data
	//{{AFX_DATA(SetSprialdlg)
	enum { IDD = IDD_DLGSETSPRIAL };
	double	m_qdpj;
	double	m_qxcd;
	double	m_qdpianjv;
	double	m_qxzdbj;
	double	m_qxqdbj;
	double	m_qxcsaz;
	double	m_qxpzj;
	int		m_pazypian;
	int		m_qdzyce;
	int		m_qdzypian;
	//}}AFX_DATA
	int Vpazypian;
	int Vqdzyce;
	int Vqdzypian;
	
	AcDb2dPolyline * p2dline;
	AcDbObjectId entid;
	AcDbObjectId JDid;
	AcDbObjectId GottnEntId;
	RampItem InterItem;
	RampItem ComItem;
	//定义显示样式实体是否已创建的变量;
	BOOL CreatOrnot;
	//定义引缓时缓曲的起点变量;
	double SprSX,SprSY,SprOA,SprSR;
	//定义缓曲的源点坐标;
	double OrX,OrY,OrO;
	//定义缓曲的曲长及终点半径,偏向,A值;
	double SPrialER,SPrialLen,ValueA;
	int SprDef;
	//定义被选中的实体类型变量;
	int SelItem;
	BOOL ResultS;
	AcGePoint3d point;
	//定义当前布设的缓曲线的样式;
	int RadForWard;
	double arrxy[3][10];
	
	int m_XYNum;
	CArray<BAS_DRAW_FUN::XYarray,BAS_DRAW_FUN::XYarray>pXYArr;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetSprialdlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SetSprialdlg)
	afx_msg void OnSetsprialok();
	afx_msg void OnSetsprialcancel();
	afx_msg void OnRadiosszouce();
	afx_msg void OnRadiossyouce();
	afx_msg void OnRadiosspazoupian();
	afx_msg void OnRadiosszoupian();
	afx_msg void OnRadiossyoupian();
	afx_msg void OnRadiosspayoupian2();
	afx_msg void OnButtonDragenddot();
	afx_msg void OnButtonUpdate();
	virtual BOOL OnInitDialog();
//	afx_msg void OnChangeEditssre();
//	afx_msg void OnChangeEditssva();
//	afx_msg void OnChangeEditsslen();
	afx_msg LRESULT OnAcadKeepFocus(WPARAM wParam,LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETSPRIALDLG_H__5972586B_5FB0_4B41_8F36_4127A0D06D0F__INCLUDED_)
