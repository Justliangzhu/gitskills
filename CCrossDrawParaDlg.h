#pragma once
#include "stdafx.h"

// CCrossDrawParaDlg 对话框

class CCrossDrawParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CCrossDrawParaDlg)

public:
	CCrossDrawParaDlg();
	virtual ~CCrossDrawParaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CROSSDRAWPARA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	//参数传递
	void UpdateDrawPara(bool bSaveAndValidate);
	//检测函数
	bool CheckParaValidate();
	//横断面绘图参数
	DrawCrossPara m_DrawPara;
	// 比例尺
	double m_Scale;
	//是否绘制网格
	BOOL m_IsDrawGrid;
	//是否绘制占地桩
	BOOL m_IsDrawEarthPile;
	// 左地面线宽
	double m_EarthWidL;
	// 右地面线宽
	double m_EarthWidR;
	afx_msg void OnBnClickedOk();
	double m_TabTextH;
	double m_DrawTextH;
};
