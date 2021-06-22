#pragma once
#include "stdafx.h"
#include "resource.h"
// COverallDrawParaDlg 对话框

class COverallDrawParaDlg : public CDialog
{
	DECLARE_DYNAMIC(COverallDrawParaDlg)

public:
	COverallDrawParaDlg();
	virtual ~COverallDrawParaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_OVERALLDRAWPARA
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CComboBox m_Combo_TextStyle;		//文字样式下拉框

	//初始化combobobx
	void InitComboBox();

	afx_msg void OnBnClickedRadioOvalparaA4();
	afx_msg void OnBnClickedRadioOvalparaA3();
	afx_msg void OnBnClickedRadioOvalparaA2();
	afx_msg void OnBnClickedRadioOvalparaA1();
	afx_msg void OnBnClickedRadioOvalparaCustom();

	double m_FrameLen;			//图框长
	double m_FrameWid;			//图框宽
	CEdit m_FrameLenCtrl;		//图框长控制
	CEdit m_FrameWidCtrl;		//图框宽控制
	int m_FrameSizeMode;		//图框尺寸模式

	afx_msg void OnBnClickedCheckOvalparaDrawpage();
	afx_msg void OnBnClickedCheckOvalparaAssignpage();

	// 是否标注页码
	BOOL m_IsDrawPage;
	// 是否标注图名
	BOOL m_IsDrawName;
	// 页码数
	int m_PageNum;
	// 总页码数
	int m_TotalPageNum;
	// 是否指定页码
	BOOL m_IsAssignPage;
	// 是否指定总页码
	BOOL m_IsAssignTotalPageNum;
	// 页码数控件
	CEdit m_PageNumCtrl;
	// 总页码数控件
	CEdit m_TotalPageNumCtrl;
	// 出图模式
	int m_OutputMode;
	// 是否绘制总体图
	BOOL m_IsDrawOval;
	// 是否绘制平面图
	BOOL m_IsDrawHoriRoad;
	// 是否绘制用地图
	BOOL m_IsDrawEarth;
	// 是否绘制纵断面图
	BOOL m_IsDrawProfile;
	// 是否绘制横断面图
	BOOL m_IsDrawCross;
	// 是否绘制曲线要素表
	BOOL m_IsDrawCurInfo;
	// 是否绘制路基大样图
	BOOL m_IsDrawRoadbed;

	// 总体绘图参数
	DrawOverallPara m_DrawOvalPara;

	//参数传递
	void UpdateDrawPara(bool bSaveAndValidate);
	afx_msg void OnBnClickedCheckOvalparaAssigntotalpage();

	afx_msg void OnBnClickedOk();
};
