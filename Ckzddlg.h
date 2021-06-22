#pragma once
#include "StdAfx.h"

// Ckzddlg 对话框

class Ckzddlg : public CDialog
{
	DECLARE_DYNAMIC(Ckzddlg)

public:
	Ckzddlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Ckzddlg();
	std::vector<GCKZPt> m_LevelCtrlPtArray;
	int ModeBiterstyle;//1-1
	CString m_iterpoint;
	CString m_clearance;
	CComboBox m_iterstyle;
	CString m_XZlevel;
	GCKZPt itergckz;
	JD_CENTER* pline;
	double temp_clearance;
	CString m_MdbName;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Ckzddlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonJcdkz();
	afx_msg void OnCbnSelchangeComboJcys();
	afx_msg void OnBnClickedOk();
};
