#pragma once
#include "StdAfx.h"
#include <vector>
#include "GridCtrl.h"

// CSetLevelCtrlDlg 对话框

class CSetLevelCtrlDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetLevelCtrlDlg)

public:
	CSetLevelCtrlDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetLevelCtrlDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SetLevelCtrlData };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//数据
	std::vector<GCKZPt> m_LevelCtrlPtArray;
	//路径
	CString m_MdbName;
	//道路名
	CString m_RoadName;
	CComboBox m_ComboxRoadName;
	// 表格控件
	CGridCtrl m_GridCtrl;
	//单元格
	GV_ITEM m_Item;
	int NumOfTK;
	//初始化函数
	virtual BOOL OnInitDialog();
	//初始化表格控件
	void InitGridCtrl();
	//读表格
	void ReadGridCtrl();
	//写表格
	void WriteGridCtrl();

	//保存数据
	afx_msg void OnBnClickedButtonSetlevelctrlSave();
	//增加行
	afx_msg void OnBnClickedButtonSetlevelctrlAddrow();
	//删除行
	afx_msg void OnBnClickedButtonSetlevelctrlDelrow();
	//平面拾取
	afx_msg void OnBnClickedButtonSetlevelctrlHpick();
	int GetNumOfTK();
	JD_CENTER* ptempRoad;
	//纵面拾取
	afx_msg void OnBnClickedButtonSetlevelctrlVpick();
	afx_msg void OnBnClickedButtonSetlevelctrlDraw();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeComboRoadname();
};
