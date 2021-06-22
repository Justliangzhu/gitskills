#pragma once
#include "stdafx.h"

// CHDrawParaDlg 对话框

class CHDrawParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CHDrawParaDlg)

public:
	CHDrawParaDlg();
	virtual ~CHDrawParaDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HDRAWPARA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

	//平面绘图参数类
	DrawHorizontalPara m_DrawPara;
	//绘图比例
	CComboBox m_Combo_Scale;
	//里程样式
	CComboBox m_Combo_MileStyle;
	// 字高
	double m_TextHeight;
	// 小数位数
	CComboBox m_Combo_Decimals;
	// 是否删除界内实体
	BOOL m_IsDelEnt;
	// 分离段绘用地
	BOOL m_IsDrawSeparation;
	// 是否绘制RAL
	BOOL m_IsDrawRAL;
	// 是否标注交点
	BOOL m_IsNoteJD;
	// 线宽
	double m_LineWeight;
	// 是否绘制大中桥
	BOOL m_IsDrawBri;
	//是否绘制水准点
	BOOL m_IsDrawBM;
	// 是否绘制小桥涵
	BOOL m_IsDrawSmallBri;
	// 是否绘制隧道
	BOOL m_IsDrawTun;
	// 是否绘制导线
	BOOL m_IsDrawDX;
	// 用地界线左宽
	double m_EarthLen_L;
	// 用地界线右宽
	double m_EarthLen_R;
	//用地界线宽
	double m_EarthLineWeight;
	// 用地界绘制模式
	int m_EarthNoteMode;
	//图框长
	double m_FrameLen;
	//图框宽
	double m_FrameHeight;
	//顶部留白
	double m_TopSpace;
	//底部留白
	double m_BottomSpace;
	// 横向移动
	double m_MoveX;
	// 纵向移动
	double m_MoveY;
	// 每幅道路长
	double m_RoadLen;
	//线路中心指定模式
	int m_CenterMode;
	//绘图中心X、Y坐标
	double m_DrawCenterX;
	double m_DrawCenterY;
	//控件控制变量
	CEdit m_MoveCenterX_Ctrl;
	CEdit m_MoveCenterY_Ctrl;
	CEdit m_DrawCenterX_Ctrl;
	CEdit m_DrawCenterY_Ctrl;
	CButton m_PickButton;

	//参数传递
	void UpdateDrawPara(bool bSaveAndValidate);
	//初始化函数
	virtual BOOL OnInitDialog();
	//检测函数
	bool CheckParaValidate();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonHdrawparaPickcenter();
	void SetDrawModeCtrl();	//设置控件

	afx_msg void OnBnClickedRadioHdrawparaSetbymove();
	afx_msg void OnBnClickedRadioHdrawparaSetbacenter();
	// 底图是否为灰色
	BOOL m_IsBaseMapGray;
	// 交点信息字高
	double m_IntersectionTextH;
	// 里程标字高
	double m_MileTextH;
	// 特征桩字高
	double m_PileTextH;
	// 占地信息字高
	double m_EarthInforTextH;
};
