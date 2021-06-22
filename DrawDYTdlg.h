#pragma once

#include "resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"

// CDrawDYTdlg 对话框

class CDrawDYTdlg : public CDialog, public BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(CDrawDYTdlg)

public:
	CDrawDYTdlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawDYTdlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DYT };
	XLDataBase DBS;
	JD_CENTER *pm;
	CComboBox	m_RoadNameCrl;
	CString	m_RoadName;
	CString	m_PTX;
	CString	m_PTY;
	CString	m_Dbs;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

public:
	afx_msg void OnBnClickedButtonLiulan();
	afx_msg void OnBnClickedButtonSelectxl();
	afx_msg void OnBnClickedButtonPickpt();
	afx_msg void OnCbnSelchangeComboDlname();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//绘制大样图
	void DrawDYT();
};
