#if !defined(AFX_TRIMTUSETTINGDIALOG_H__B8B5FFDA_55E4_431A_AE74_1CB9E8471F24__INCLUDED_)
#define AFX_TRIMTUSETTINGDIALOG_H__B8B5FFDA_55E4_431A_AE74_1CB9E8471F24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrimTuSettingDialog.h : header file
//
#include "JD_CENTER.h"
#include "XLDataBase.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CTrimTuSettingDialog dialog
//平面图分幅输出设计对话框
class CTrimTuSettingDialog : public CDialog
{
// Construction
public:
	CTrimTuSettingDialog(CWnd* pParent = acedGetAcadFrame());   // standard constructor
   	XLDataBase DBS;
	JD_CENTER *pm;
// Dialog Data
	//{{AFX_DATA(CTrimTuSettingDialog)
	enum { IDD = IDD_DIALOG_TRIMTUSETTING };
	CComboBox	m_RoadNameCrl;
	float	m_Length;
	double m_Frame_Height;	//内框高度
	double m_Frame_Width;	//内框宽度
	int		m_TrimFlag;
	CString	m_TuKuangFile;
	CString	m_OutName;
	int		m_StartNo;
	CString	m_SumT;
	CString	m_RoadName;
	CString	m_Edml;
	CString	m_Dbs;
	CString	m_Sdml;
	//}}AFX_DATA
    int m_TKPos;//成图位置 1顶部 2 居中 3 底部
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrimTuSettingDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTrimTuSettingDialog)
	afx_msg void OnButtonBrowstk();
	afx_msg void OnRadioTop();
	afx_msg void OnRadioMiddle();
	afx_msg void OnRadioBottom();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonLiulan();
	afx_msg void OnSelchangeCOMBORoadName();
	void Show_Detail_Infor(bool IsShow);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_ShowInforCtrl;
	bool m_IsNowShow;
	afx_msg void OnBnClickedButtonShowDetailInfor();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRIMTUSETTINGDIALOG_H__B8B5FFDA_55E4_431A_AE74_1CB9E8471F24__INCLUDED_)
