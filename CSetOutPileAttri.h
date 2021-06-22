#pragma once
#include "stdafx.h"
#include "BTGridCtrl.h"

// CSetOutPileAttri 对话框

class CSetOutPileAttri : public CDialog
{
	DECLARE_DYNAMIC(CSetOutPileAttri)

public:
	CSetOutPileAttri(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetOutPileAttri();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETOUTPILEATTRI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 表格控件
	BTGridCtrl m_GridCtrl;
	//单元格
	GV_ITEM m_Item;
	//项目名
	CString m_MdbName;
	CString m_RoadName;	
	//初始化表格
	void InitGridCtrl();
	//写表格
	void WriteGridCtrl();
	//读表格
	void ReadGridCtrl();
	//初始化函数
	virtual BOOL OnInitDialog();
	//增删一行
	afx_msg void OnBnClickedButtonSetoutpileattriAdd();
	afx_msg void OnBnClickedButtonSetoutpileattriDel();
	afx_msg void OnBnClickedOk();
	bool CheckDataValidate();

	std::vector<CrossType> m_CrossTypeArray;
};
