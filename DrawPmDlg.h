#if !defined(AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_)
#define AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawPmDlg.h : header file
//
#include "Resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
/////////////////////////////////////////////////////////////////////////////
// CDrawPmDlg dialog

class CDrawPmDlg : public CDialog,public BAS_DRAW_FUN
{
// Construction
public:
	CDrawPmDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	CDrawPmDlg(CString MdbName);
	XLDataBase DBS;
	JD_CENTER *pm;
	int DrawMode;//绘图方式 0  线位图 1 平面总体图 2 用地图
	int m_DrawAll;//是否绘所有线路		0-绘制所有线路	1-绘制单条线路	20190706	这样看上去很迷惑
	int m_ZDKMode;////绘制占地图方式
	bool WriteCSFile(CString FName);//写平面出图参数文件
	bool ReadCSFile(CString FName);//读平面出图参数文件
	bool ReadCSFileForDraw();//读取平面参数文件创建相关指针。
// Dialog Data
	//{{AFX_DATA(CDrawPmDlg)
	enum { IDD = IDD_DIALOG_DRAWPM };
	CComboBox	m_RoadName;
	CString	m_ROAD;
	double	m_ZD;
	double	m_WHT;
	double	m_QD;
	CString	m_Dbs;//数据库
	CString	m_SCALE;//比例尺
	CString	m_XSW;//里程保留小数位数
	CString	m_LCBZ;//里程标注
	BOOL	m_DrawBM;	//绘水准点
	BOOL	m_DrawDX;	//绘导线
	BOOL	m_DrawTun;	//绘隧道
	BOOL	m_DrawCulvet;	//小桥涵
	BOOL	m_DrawDZQ;	//绘大中桥
	double	m_ZcLen;	//左侧用地标注线长
	double	m_YcLen;	//右侧用地标注线长
	BOOL	m_DelEntInf;//删除界内实体
	BOOL	m_BZRAL;	//标注半径缓长
	CString	m_ZxWid;	//线路中线宽度
	BOOL	m_drawfldzdk;//分离段占地宽
	CString	m_YDXWid;	//用地桩线宽度
	BOOL m_DrawJD;		//是否标注交点
	//}}AFX_DATA
    
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawPmDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDrawPmDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONOpenDbs();
	afx_msg void OnRADIODrawALL();
	afx_msg void OnRADIODrawSingle();
	virtual void OnOK();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnRadioxwt();
	afx_msg void OnRadiopmztt();
	afx_msg void OnRadiozdt();
	virtual void OnCancel();
	afx_msg void OnRadiolcwid();
	afx_msg void OnRadionecor();
	afx_msg void OnRadioboth();
	afx_msg void OnRadionone();
	afx_msg void OnCheckBmbridge();
	afx_msg void OnCheckBm();
	afx_msg void OnCheckSmallbridge();
	afx_msg void OnCheckTun();
	afx_msg void OnCheckDx();
	afx_msg void OnCheckDelent();
	afx_msg void OnCheckShowdbs();
	afx_msg void OnCheckFldzdk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWPMDLG_H__AE45B1F4_8C6D_4C1E_8D5A_7F181CF6860A__INCLUDED_)
