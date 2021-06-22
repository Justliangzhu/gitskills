// DrawBianXianDlg.h : header file
// ������·ƫ�ñ��߶Ի��� 2005.07.12 ldf
//
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWBIANXIANDLG_H__E4213BC6_D45E_4A40_AB70_1ABD4C5E1073__INCLUDED_)
#define AFX_DRAWBIANXIANDLG_H__E4213BC6_D45E_4A40_AB70_1ABD4C5E1073__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JD_CENTER.H"

/////////////////////////////////////////////////////////////////////////////
// ������·ƫ�ñ��߶Ի���

class CDrawBianXianDlg : public CDialog
{
// Construction
public:
	CDrawBianXianDlg(CWnd* pParent = acedGetAcadFrame());   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDrawBianXianDlg)
	enum { IDD = IDD_DRAW_BX };
	CString	m_strStyle;  // ƫ������
	CString	m_strMethod; // ƫ�÷�ʽ
	double	m_dScml;     // ��ʼ���
	double	m_dEcml;     // ��ֹ���
	CString	m_strSWidth; // �������ƫ�ÿ��
	CString	m_strEWidth; // �����յ�ƫ�ÿ��		
	double	m_dStep;     // ���Ʋ���
	//}}AFX_DATA

	JD_CENTER*   m_pOffsetRoad;  // ƫ��ʵ��ָ��
	AcDbObjectId m_OffsetRoadID; // ƫ��ID
	double       m_dBaseProMile; // ����(ƫ��ʰȡ��)ͶӰ���
	CArray<double,double> m_SWidth, m_EWidth; // ����ƫ�����տ������


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawBianXianDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool GetOffsetRoad();      // ʵʱ���ƫ��
	void GetCml(double& dCml); // ʰȡ׮��
	bool GetOffSetWidth(CString strWidth, CArray<double,double>& arrays); // ���ƫ�ÿ��

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
