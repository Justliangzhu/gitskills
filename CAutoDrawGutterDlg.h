﻿#pragma once
#include "stdafx.h"
#include "COverallDrawParaDlg.h"
#include "CHDrawParaDlg.h"
#include "CVDrawParaDlg.h"
#include "CCrossDrawParaDlg.h"


// CAutoDrawGutterDlg 对话框

class CAutoDrawGutterDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoDrawGutterDlg)

public:
	CAutoDrawGutterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoDrawGutterDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AUTODRAWGUTTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	//基本参数
	CString m_ProPath;		//方案路径
	double m_Coor_X;		//X坐标
	double m_Coor_Y;		//Y坐标
	CString m_FramePath;	//图框路径
	CString m_ProNo;		//方案名
	CString m_dirstr;		//方案文件夹
	afx_msg void OnBnClickedButtonAutodesBro();
	virtual BOOL OnInitDialog();
	void NullInStrto$(CString& text);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonAutodesPick();
	afx_msg void OnBnClickedButtonAutodesBroframe();
	BOOL m_IsFromMdb;
	afx_msg void OnBnClickedButtonAutodesBrrgutter();
	CString m_GutterPath;
	double m_GutterThick;

	//总体绘图参数
	COverallDrawParaDlg m_OvalDrawParaDlg;	//总体绘图参数对话框
	DrawOverallPara m_OvalDrawPara;			//总体绘图参数
	afx_msg void OnBnClickedButtonAutodesOvalpara();

	//平面绘图参数
	CHDrawParaDlg m_HDrawParaDlg;	//平面绘图参数对话框
	DrawHorizontalPara m_HDrawPara;	//平面绘图参数
	afx_msg void OnBnClickedButtonAutodesHdrawpara();

	//纵面绘图参数
	CVDrawParaDlg m_VDrawParaDlg;	//纵面绘图参数对话框
	DrawProfilePara m_VDrawPara;	//纵面绘图参数
	void SetProfilePara();			//设置初始纵断面参数
	afx_msg void OnBnClickedButtonAutodesVdrawpara();

	//横断面绘图参数
	CCrossDrawParaDlg m_CrossDrawParaDlg;	//横断面绘图参数对话框
	DrawCrossPara m_CrossDrawPara;			//横断面绘图参数
	void SetCrossPara();			//设置初始横断面参数
	afx_msg void OnBnClickedButtonAutodesSetcrosspara();

};
