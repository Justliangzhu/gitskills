#pragma once
#include "StdAfx.h"
#include "resource.h"
#include "BAS_DRAW_FUN.h"
#include "GridCtrl.h"

// CCrossEarthInputDlg 对话框

class CCrossEarthInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CCrossEarthInputDlg)

public:
	CCrossEarthInputDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCrossEarthInputDlg();

	struct CorssEarthLine
	{
		double Cml;
		CString Ckml;
		double EarthLevel;
		int LeftEarthNum, RightEarthNum;
		std::vector<AcGePoint2d> LeftPtArray, RightPtArray;

		CorssEarthLine()
		{
			Cml = 0.0;
			Ckml = L"";
			EarthLevel = 0.0;
			LeftEarthNum = RightEarthNum = 0;
			LeftPtArray.clear();
			RightPtArray.clear();
		}
	};

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CROSSEARTHINPUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//成员变量
	std::vector<CorssEarthLine> m_CrossEarthLineArray;
	// 桩号combobox控件
	CComboBox m_ComboPile;
	// 中桩高程控件值
	double m_LevelEidt;
	//表格控件
	GV_ITEM m_Item;		//单元格
	CGridCtrl m_EarGridCtrl_L;
	CGridCtrl m_EarGridCtrl_R;
	// 参照模式 0-相对中桩 1-相对前点
	int m_ReferMode;
	// 高程类型 0-高差 1-高程
	int m_LevelMode;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	//读取地面线数据
	bool ReadEarthLine();
	//写入地面线数据
	void WriteEarthLine();
	//设置控件初始值
	void SetInitCtrl();
	//获取当前桩号
	int GetCurPileIndex();
	
	//读取数据
	void ReadData(bool IsXNormal = true, bool IsYNormal = true);
	//写入数据
	void WriteData();

	//读表格控件
	void ReadGridCtrl(int LorR, bool IsXNormal = true, bool IsYNormal = true);
	//写表格控件
	void WriteGridCtrl(int LorR);

	//绘制单个横断面地面线
	double DrawSingleCrossEarthLine(AcGePoint3d PtOri, int CrossIndex);

	//响应函数
	afx_msg void OnCbnSelchangeComboCroeinput();
	afx_msg void OnBnClickedRadioCrossearthBypile();
	afx_msg void OnBnClickedRadioCrossearthPrept();
	afx_msg void OnBnClickedRadioCrossearthDeltah();
	afx_msg void OnBnClickedRadioCrossearthLevel();
	afx_msg void OnCbnDropdownComboCroeinput();
	afx_msg void OnBnClickedButtonDrawcrossearth();
	afx_msg void OnBnClickedButtonDrawcrossearthAll();
	afx_msg void OnBnClickedButtonSavecrossearth();
	afx_msg void OnBnClickedButtonAddrowLeft();
	afx_msg void OnBnClickedButtonDelrowLeft();
	afx_msg void OnBnClickedButtonAddrowRight();
	afx_msg void OnBnClickedButtonDelrowRight();
};
