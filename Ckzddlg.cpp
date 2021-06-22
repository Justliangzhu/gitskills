﻿// Ckzddlg.cpp: 实现文件
//
#include "stdafx.h" 

//#include "pch.h"
#include "Ckzddlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "XLDataBase.h"
#include "CSetLevelCtrlDlg.h"

// Ckzddlg 对话框

IMPLEMENT_DYNAMIC(Ckzddlg, CDialog)

Ckzddlg::Ckzddlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_Ckzddlg, pParent)
{
	m_iterpoint = _T("0.00");
	m_XZlevel = _T("0.0");
	pline = NULL;
	ModeBiterstyle = 0;
	m_clearance = _T("7.0");
}

Ckzddlg::~Ckzddlg()
{
}

void Ckzddlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1_JCDLC, m_iterpoint);
	DDX_Control(pDX, IDC_COMBO_JCYS, m_iterstyle);
	DDX_Text(pDX, IDC_EDIT3_JCDGC, m_clearance);
	DDX_Text(pDX, IDC_EDIT_JSKZBG, m_XZlevel);
}


BEGIN_MESSAGE_MAP(Ckzddlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_JCDKZ, &Ckzddlg::OnBnClickedButtonJcdkz)
	ON_CBN_SELCHANGE(IDC_COMBO_JCYS, &Ckzddlg::OnCbnSelchangeComboJcys)
	ON_BN_CLICKED(IDOK, &Ckzddlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Ckzddlg 消息处理程序


void Ckzddlg::OnBnClickedButtonJcdkz()//计算控制标高
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	double temp;
	temp=_wtof (m_clearance);
	double templevel;
	if (ModeBiterstyle==0)
		templevel = pline->m_InterSectData.InterDesH - temp;
	else
		templevel = pline->m_InterSectData.InterDesH +temp;
	m_XZlevel.Format(L"%.3f",templevel);
	UpdateData(FALSE);
}


BOOL Ckzddlg::OnInitDialog()//初始化
{
	m_LevelCtrlPtArray.clear();
	bool JCD;
	JCD = FALSE;
	CDialog::OnInitDialog();
	BAS_DRAW_FUN Bas_Fun;
	XLDataBase DBS;
	CString MdfRoadNmae;
	MdfRoadNmae = Pro;
	CString temp1;
	DBS.Read_XLDbs(m_MdbName, "高程控制点表", pline->RoadName);

	if (DBS.m_LevelCtrlPtArray.size())
	{
		m_iterstyle.AddString(L"下穿铁路");
		m_iterstyle.AddString(L"上跨铁路");
		for (int i = 0; i < DBS.m_LevelCtrlPtArray.size(); i++)
		{
			if (DBS.m_LevelCtrlPtArray[i].Note.Find(L"交叉点")>=0)
			{
				if (DBS.m_LevelCtrlPtArray[i].Style ==2)
				{
					/*m_iterstyle.AddString(L"上跨铁路");
					m_iterstyle.AddString(L"下穿铁路");*/
					m_iterstyle.SetCurSel(1);
					m_XZlevel.Format(L"%.3f", DBS.m_LevelCtrlPtArray[i].DesHXX);
				}
				else
				{
					/*m_iterstyle.AddString(L"上跨铁路");
					m_iterstyle.AddString(L"下穿铁路");*/
					m_iterstyle.SetCurSel(0);
					m_XZlevel.Format(L"%.3f", DBS.m_LevelCtrlPtArray[i].DesHSX);
				}
				m_iterpoint = pline->LCchr(DBS.m_LevelCtrlPtArray[i].GH, DBS.m_LevelCtrlPtArray[i].XCLc, 3);
				m_clearance.Format(L"%.3f", DBS.m_LevelCtrlPtArray[i].clearrence);
				JCD = TRUE;

			}
				
			
		}
	}
	if (JCD ==FALSE)
	{
		temp1 = pline->XLC(pline->m_InterSectData.CurDml);
		m_iterpoint = temp1;
		m_iterstyle.AddString(L"下穿铁路");
		m_iterstyle.AddString(L"上跨铁路");
		m_iterstyle.SetCurSel(0);
		double templevelcs;
		double temp;
		temp = _wtof(m_clearance);
		templevelcs = pline->m_InterSectData.InterDesH -temp;
		m_XZlevel.Format(L"%.3f", templevelcs);
	}
	
	
	UpdateData(FALSE);
	return TRUE;
}

void Ckzddlg::OnCbnSelchangeComboJcys()//选择样式
{
	// TODO: 在此添加控件通知处理程序代码
	ModeBiterstyle = m_iterstyle.GetCurSel();
}


void Ckzddlg::OnBnClickedOk()//加入控制点表
{
	// TODO: 在此添加控件通知处理程序代码)
	CDialog::OnOK();
	GCKZPt tempgckz;
	tempgckz.RoadName = pline->RoadName;
	_tcscpy(tempgckz.GH, L"K");
	double temp_LC;
	temp_LC = pline->TYLC(m_iterpoint);
	tempgckz.XCLc = temp_LC;
	if (ModeBiterstyle == 0)
	{
		tempgckz.DesHSX = _wtof(m_XZlevel);
		tempgckz.Style = 1;
	}
	else
	{
		tempgckz.DesHXX = _wtof(m_XZlevel);
		tempgckz.Style = 2;
	}
	tempgckz.Note = L"交叉点";
	XLDataBase DBS;
	m_LevelCtrlPtArray.clear();
	tempgckz.clearrence = _wtof(m_clearance);
	m_LevelCtrlPtArray.push_back(tempgckz);

	/*DBS.Write_XLDbs(m_MdbName, "高程控制点表", pline->RoadName);*/
}