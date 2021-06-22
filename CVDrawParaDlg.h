#pragma once
#include "stdafx.h"
#include "BTGridCtrl.h"

// CVDrawParaDlg 对话框

class CVDrawParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CVDrawParaDlg)

public:
	CVDrawParaDlg();
	virtual ~CVDrawParaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VDRAWPARA  };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	DrawProfilePara m_VDrawPara;	//纵断面绘图参数
	//参数传递
	void UpdateDrawPara(bool bSaveAndValidate);
	// 横向比例
	double m_HoriScale;
	// 纵向比例
	double m_VerScale;
	// 是否绘制竖曲线要素
	BOOL m_IsDrawCurveInf;
	// 是否绘制网格
	BOOL m_IsDrawGrid;
	// 是否绘制五线谱特征桩
	BOOL m_IsDrawFeaturePile;
	// 五线谱数组
	std::vector<WXP> m_WXPArray;

	//五线谱表格控件
	BTGridCtrl m_wxp;

	//初始化函数
	virtual BOOL OnInitDialog();

	//写入五线谱表格
	void WriteWXPGridCtrl();
	//读取五线谱表格
	void ReadWXPGridCtrl();
	//五线谱名称与int之间的转换
	double StringToNumber(CString str);
	void NumberToString(int xz, ACHAR str[]);
	//检测数据是否有效
	bool CheckParaValidate();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonVdrawparaAdd();
	afx_msg void OnBnClickedButtonVdrawparaDel();
	//是否绘制控制标高
	BOOL m_isDrawCtrHZCS;
	double m_ZMBZTextH;
	double m_YbreakTextH;
};
