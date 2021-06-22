#if !defined(AFX_DRAWZDMDLG_H__0594ADD9_76D6_477C_A629_7FCBD415FFB1__INCLUDED_)
#define AFX_DRAWZDMDLG_H__0594ADD9_76D6_477C_A629_7FCBD415FFB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawZDMDlg.h : header file
//
#include "Resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
#include "WxpLmDlg.h"
#include "ZDMTBX.h"
/////////////////////////////////////////////////////////////////////////////
// DrawZDMDlg dialog

class DrawZDMDlg : public CPropertyPage
{
// Construction
public:
	DrawZDMDlg();   // standard constructor

	WxpLmDlg wxplm;
    CZDMTBX zdmbx;
	XLDataBase DBS;
	JD_CENTER *pm;
	int DrawMode;//0 ���� 1 ʩ��
	int FForZF;//0 �ַ� 1����
	int ZPInf;//0 ȫ�� 1 ȫ�� 2 ��� 3 �ҷ�
// Dialog Data
	//{{AFX_DATA(DrawZDMDlg)
	enum { IDD = IDD_DIALOG_DRAWZDM };
	CComboBox	m_RoadNameCtrl;
	CString	m_RoadName;		//��·��
	CString	m_Hscale;		//�������
	CString	m_Zscale;		//�������
	CString	m_CorX;			//��ͼԭ��X
	CString	m_CorY;			//��ͼԭ��Y
	CString	m_Dbs;			//����·��
	CString m_SStCkml;
	CString m_SEdCkml;
	double	m_EdDml;		//��ʼ���
	double	m_StDml;		//��ֹ���
	CString	m_SPage;		//��ʼҳ��
	BOOL	m_bzym;			//�Ƿ��עҳ��
	CString	m_TKName;		//ͼ���ļ�·��
	BOOL	m_infwgx;		//�Ƿ����������
	CString	m_dzgkname;		//���ʸſ��ļ�·��
	BOOL m_IsDrawFeaturePile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DrawZDMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DrawZDMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonScan();
	afx_msg void OnBUTTONPickPT();
	afx_msg void OnSelchangeCOMBORoadName();
	afx_msg void OnRadioCbsj();
	afx_msg void OnRadioSgtsj();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnButtonSeltkfile();
	afx_msg void OnCheckBzym();
	afx_msg void OnPaint();
	afx_msg void OnCheckDrawwgx();
	afx_msg void OnRadioFysc();
	afx_msg void OnRadioDzzfsc();
	afx_msg void OnRadioAll();
	afx_msg void OnRadioQf();
	afx_msg void OnRadioZf();
	afx_msg void OnRadioYf();
	afx_msg void OnButtonDzgk();

	void Init_WXP_Data(bool Is_Prili_Des);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	BOOL m_IsDrawCtrH;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWZDMDLG_H__0594ADD9_76D6_477C_A629_7FCBD415FFB1__INCLUDED_)
