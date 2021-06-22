#pragma once

#include "StdAfx.h"

// COutpRoadCollTabDlg 对话框

class COutpRoadCollTabDlg : public CDialog
{
	DECLARE_DYNAMIC(COutpRoadCollTabDlg)

public:
	COutpRoadCollTabDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutpRoadCollTabDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OUTPROADCOLLTAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// X坐标
	double m_Coor_X;
	// Y坐标
	double m_Coor_Y;
	afx_msg void OnBnClickedButtonOutproadcolltabPick();
	afx_msg void OnBnClickedButtonOutproadcolltabDraw();
	afx_msg void OnBnClickedButtonOutproadcolltabExcel();
};
