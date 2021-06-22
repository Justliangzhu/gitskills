#if !defined(AFX_XLSJ_H__AA0BF3FD_0640_478A_8EBF_86899DBCE23D__INCLUDED_)
#define AFX_XLSJ_H__AA0BF3FD_0640_478A_8EBF_86899DBCE23D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XLSJ.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XLSJ dialog
#include "JD_CENTER.h"
#include "resource.h"
class XLSJ : public CDialog,public BAS_DRAW_FUN
{
	// Construction
public:
	double xjj;
	AcGePoint3d PT;//选择线路点
	
	JD_CENTER * pGtzx;
	JD_CENTER * pGtzxIN;//接入线路	
	AcDbObjectId lId,lIdIN;
	
	bool IsModifyOldGTZX;//量否修改原有方案
	int m_ModifyOldGTZX;
	XLSJ(CWnd* pParent = acedGetAcadFrame());   // standard constructor
	// Dialog Data
	//{{AFX_DATA(XLSJ)
	enum { IDD = IDD_addJD };
	CButton	m_ButtonInXL;
	CButton	m_ButtonOutXL;
	double	m_A2L2;
	double	m_A1L1;
	double	m_R;
	bool m_IfA;
	int m_curActiveJD;
	CString	m_LinkRelation;
	//}}AFX_DATA
	//为了刷新对话框需要重载
	double GetRFromT_P_P(double x1,double y1,double x2,double y2,
		double &xs,double &ys,double &x0,double &y0,
		double &xe,double &ye,double &cenX,double &cenY,
		AcDbObjectIdArray &eraseId,bool IsStandard, double DV=350);
	double GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, bool IsStandard, double DV,int INTRadius,int JUDGET012);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XLSJ)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void ButtonUpdate();
	// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(XLSJ)
	afx_msg void OnSelCir();
	afx_msg void OnSelQx();
	afx_msg void OnInxl();
	afx_msg void OnOutxl();
	afx_msg void OnRadioReplace();
	afx_msg void OnRadioNewgtzx();		
	virtual BOOL OnInitDialog();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnBUTTONupdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XLSJ_H__AA0BF3FD_0640_478A_8EBF_86899DBCE23D__INCLUDED_)
