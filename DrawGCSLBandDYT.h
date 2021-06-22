#pragma once

#include "resource.h"
#include "XLDataBase.h"
#include "JD_CENTER.h"
#include "CSetOutPileAttri.h"

// CDrawGCSLBandDYT 对话框

class CDrawGCSLBandDYT : public CDialog,public BAS_DRAW_FUN
{
	DECLARE_DYNAMIC(CDrawGCSLBandDYT)

public:
	CDrawGCSLBandDYT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDrawGCSLBandDYT();

// 对话框数据
	enum { IDD = IDD_DIALOG_DrawGCSLBandDYT };
	XLDataBase DBS;
	JD_CENTER *pm;
	CComboBox	m_RoadNameCrl;
	CString	m_RoadName;
	CString	m_PTX;
	CString	m_PTY;
	CString	m_Dbs;
	int DFACTOR;//比例
	//20200303梁柱
	std::vector<CString> PathArray;
	std::vector<CrossType> m_CrossTypeArray;	//手动设置指定桩号数组
		// 是否是输出到Excel
	BOOL m_IsToExcel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
public:

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonLiulan();
	afx_msg void OnBnClickedButtonSelectxl();
	afx_msg void OnBnClickedButtonPickpt();
	afx_msg void OnCbnSelchangeComboDlname();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	
	afx_msg void OnBnClickedButtonSetoutpileattri();

	// //20200303梁柱批量输出表格
	BOOL m_PLTab;
	afx_msg void OnBnClickedCheckPl();
};
