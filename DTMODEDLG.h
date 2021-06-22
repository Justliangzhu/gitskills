#if !defined(AFX_DTMODEDLG_H__881E13A9_6C8C_487A_ACF9_39DE8967514F__INCLUDED_)
#define AFX_DTMODEDLG_H__881E13A9_6C8C_487A_ACF9_39DE8967514F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DTMODEDLG.h : header file
//
#include "resource.h"
#include "BAS_DRAW_FUN.h"
/////////////////////////////////////////////////////////////////////////////
// DTMODEDLG dialog

class DTMODEDLG : public CDialog ,BAS_DRAW_FUN
{
// Construction
public:
	DTMODEDLG(CWnd* pParent = acedGetAcadFrame());   // standard constructor
    int ShqDir;//起始缓曲方向 -1 逆时针 1顺时针
	int DragPos;//拖动位置 0 点 1圆心
	int CsType;//参数类型 0 长度 1 A
	int qQXType;//前双曲类型 1 C 0 S -1直圆或卵型 
	int hQXType;//后双曲类型 1 C 0 S -1直圆或卵型
    AcDbObjectId xyid[5],CirID,xyid2[5];
	double XY[5][10],XY2[5][10];
	double XYOLD[5][10],XYOLD2[5][10];
	int XLNUM;
	int xynum,xynum2;

	//zl 为传入拾取点
	AcGePoint3d PT, PT1, PT2;
	bool changeid;
	BOOL changeAcDbObjectId(AcDbObjectId& Id1);
	//zl
    AcDbObjectId IDarr[3];
	AcDbObject *pEnt1,*pEnt2,*pEnt3;
	int IDNum;
	double MCenX,MCenY;
// Dialog Data
	//{{AFX_DATA(DTMODEDLG)
	enum { IDD = IDD_DIALOG_DTMODE };
	BOOL	m_ChangeDYR;
	BOOL	m_ChangeREnd;
	BOOL	m_ChangeRStart;
	BOOL	m_DelEnt;
	BOOL	m_DragBC;
	BOOL	m_LjEnd;
	BOOL	m_LjStart;
	CString	m_A1L1;
	CString	m_A2L2;
	CString	m_A3L3;
	CString	m_A4L4;
	CString	m_EndR;
	CString	m_MoveR;
	CString	m_StartR;
	CString	m_BC;
	CString	m_Rcha;
	CString	m_A1L1SHOW;
	CString	m_A2L2SHOW;
	CString	m_A3L3SHOW;
	CString	m_A4L4SHOW;
	int		m_QTYPE;
	int		m_HTYPE;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DTMODEDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
	void CalXYData(); 
	void UpdateEnt(); 
	int FindWichDY(AcGePoint3d pt);
	void GRDrawXY(int num,double XYarr[][10]);
protected:

	// Generated message map functions
	//{{AFX_MSG(DTMODEDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioSelpoint();
	afx_msg void OnRadioCirpoint();
	afx_msg void OnRadioLength();
	afx_msg void OnRadioAvalue();
	afx_msg void OnRadioUnclock();
	afx_msg void OnRadioClock();
	afx_msg void OnRadioCtypeh();
	afx_msg void OnRadioStypeh();
	afx_msg void OnRadioCtype();
	afx_msg void OnRadioStype();
	afx_msg void OnCheckDragbc();
	afx_msg void OnCheckDelent();
	afx_msg void OnCheckLjstart();
	afx_msg void OnCheckLjend();
	afx_msg void OnCheckChangestr();
	afx_msg void OnCheckChangedyr();
	afx_msg void OnCheckChangerend();
	afx_msg void OnButtonDrag();
	virtual void OnOK();
	virtual void OnCancel();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DTMODEDLG_H__881E13A9_6C8C_487A_ACF9_39DE8967514F__INCLUDED_)
