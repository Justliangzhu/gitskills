#pragma once
#include "StdAfx.h"
#include "GridCtrl.h"

// CGutInteractDlg 对话框

class CGutInteractDlg : public CDialog
{
	DECLARE_DYNAMIC(CGutInteractDlg)

public:
	CGutInteractDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGutInteractDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_GUTINTERACT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonGutinteractPick();
	virtual BOOL OnInitDialog();
	// 交互模式，目前暂时提供两种，0-增加水沟，1-删除水沟
	int m_InteractMode;

	//侧沟参数	沟宽	沟深	内坡率	外坡率	浆砌厚度	沟底半径
	double m_CG_GK, m_CG_GS, m_CG_GP1, m_CG_GP2, m_CG_JQHD, m_CG_GDR;
	//路堤水沟参数	挡水堰宽	护坡道宽	挡水堰厚	水沟厚	水沟宽	护坡道厚	内边坡	外边坡	挡水堰外侧边坡	浆砌厚度	沟底半径
	double m_LDG_T2W, m_LDG_T1W, m_LDG_T2H, m_LDG_GH, m_LDG_GW, m_LDG_HH, m_LDG_SP1, m_LDG_SP2, m_LDG_SP3, m_LDG_JQHD, m_LDG_GDR;	
	//截水沟（天沟）参数	边距	沟宽	沟深	内坡率	外坡率	浆砌厚度	沟底半径
	double m_TG_BJ, m_TG_GW, m_TG_GS, m_TG_SP1, m_TG_SP2, m_TG_JQHD, m_TG_GDR;

	//设置水沟参数
	void SetInitGutterPara();

	// 表格控件
	CGridCtrl m_GridCtrl;
	//单元格
	GV_ITEM m_Item;

	//写表格
	void WriteGridCtrl(std::vector<CString>& TitleArray, std::vector<double>& ParaArray);
	//读取表格数据
	void ReadGridCtrl();
	afx_msg void OnCbnEditchangeComboGutinteractGuttype();

	//水沟类型
	int m_GutterTpye;
	// 水沟类型下拉框
	CComboBox m_GutterTypeCombo;
	afx_msg void OnCbnSelchangeComboGutinteractGuttype();
	afx_msg void OnBnClickedRadioGutinteractAdd();
	afx_msg void OnBnClickedRadioGutinteractDel();
};
