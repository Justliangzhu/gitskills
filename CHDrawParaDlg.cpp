// CHDrawParaDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CHDrawParaDlg.h"
#include "afxdialogex.h"


// CHDrawParaDlg 对话框

IMPLEMENT_DYNAMIC(CHDrawParaDlg, CDialog)

CHDrawParaDlg::CHDrawParaDlg()
	: CDialog(IDD_DIALOG_HDRAWPARA)
	, m_TextHeight(0)
	, m_IsDelEnt(FALSE)
	, m_IsDrawSeparation(FALSE)
	, m_IsDrawRAL(FALSE)
	, m_IsNoteJD(FALSE)
	, m_LineWeight(0)
	, m_IsDrawBri(FALSE)
	, m_IsDrawBM(FALSE)
	, m_IsDrawSmallBri(FALSE)
	, m_IsDrawTun(FALSE)
	, m_IsDrawDX(FALSE)
	, m_EarthLen_L(0)
	, m_EarthLen_R(0)
	, m_EarthLineWeight(0)
	, m_EarthNoteMode(0)
	, m_FrameLen(0)
	, m_FrameHeight(0)
	, m_TopSpace(0)
	, m_BottomSpace(0)
	, m_MoveX(0)
	, m_MoveY(0)
	, m_RoadLen(700.0)
	, m_CenterMode(0)
	, m_DrawCenterX(0)
	, m_DrawCenterY(0)
	, m_IsBaseMapGray(FALSE)
	, m_IntersectionTextH(0)
	, m_MileTextH(0)
	, m_PileTextH(0)
	, m_EarthInforTextH(0)
{

}

CHDrawParaDlg::~CHDrawParaDlg()
{
}

void CHDrawParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SCALE, m_Combo_Scale);
	DDX_Control(pDX, IDC_COMBO_LCBZ, m_Combo_MileStyle);
	DDX_Text(pDX, IDC_EDIT_WHT, m_TextHeight);
	DDV_MinMaxDouble(pDX, m_TextHeight, 0.0, 1000.0);
	DDX_Control(pDX, IDC_COMBO_XSWS, m_Combo_Decimals);
	DDX_Check(pDX, IDC_CHECK_DELENT, m_IsDelEnt);
	DDX_Check(pDX, IDC_CHECK_FLDZDK, m_IsDrawSeparation);
	DDX_Check(pDX, IDC_CHECK_SHOWDBS, m_IsDrawRAL);
	DDX_Check(pDX, IDC_CHECK_DRAWJD, m_IsNoteJD);
	DDX_Text(pDX, IDC_EDIT_ZXWid, m_LineWeight);
	DDX_Check(pDX, IDC_CHECK_BMBRIDGE, m_IsDrawBri);
	DDX_Check(pDX, IDC_CHECK_BM, m_IsDrawBM);
	DDX_Check(pDX, IDC_CHECK_SMALLBRIDGE, m_IsDrawSmallBri);
	DDX_Check(pDX, IDC_CHECK_TUN, m_IsDrawTun);
	DDX_Check(pDX, IDC_CHECK_DX, m_IsDrawDX);
	DDX_Text(pDX, IDC_EDIT_ZCLen, m_EarthLen_L);
	DDV_MinMaxDouble(pDX, m_EarthLen_L, 0.0, DBL_MAX);
	DDX_Text(pDX, IDC_EDIT_YCLen, m_EarthLen_R);
	DDV_MinMaxDouble(pDX, m_EarthLen_R, 0.0, DBL_MAX);
	DDX_Text(pDX, IDC_EDIT_YDXWID, m_EarthLineWeight);
	DDV_MinMaxDouble(pDX, m_EarthLineWeight, 0.0, DBL_MAX);
	DDX_Radio(pDX, IDC_RADIOLCWID, m_EarthNoteMode);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_FRLEN, m_FrameLen);
	DDV_MinMaxDouble(pDX, m_FrameLen, 0.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_FRHEIGHT, m_FrameHeight);
	DDV_MinMaxDouble(pDX, m_FrameHeight, 0.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT_HDARAPARA_TOPB, m_TopSpace);
	DDV_MinMaxDouble(pDX, m_TopSpace, 0.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_BOTTOMB, m_BottomSpace);
	DDV_MinMaxDouble(pDX, m_BottomSpace, 0.0, 100000.0);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_MOVEX, m_MoveX);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_MOVEY, m_MoveY);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_RLEN, m_RoadLen);
	DDV_MinMaxDouble(pDX, m_RoadLen, 100.0, DBL_MAX);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_RLEN, m_RoadLen);
	DDX_Radio(pDX, IDC_RADIO_HDRAWPARA_SETBYMOVE, m_CenterMode);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_CENTERX, m_DrawCenterX);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_CENTERY, m_DrawCenterY);
	DDX_Control(pDX, IDC_EDIT_HDRAWPARA_MOVEX, m_MoveCenterX_Ctrl);
	DDX_Control(pDX, IDC_EDIT_HDRAWPARA_MOVEY, m_MoveCenterY_Ctrl);
	DDX_Control(pDX, IDC_EDIT_HDRAWPARA_CENTERX, m_DrawCenterX_Ctrl);
	DDX_Control(pDX, IDC_EDIT_HDRAWPARA_CENTERY, m_DrawCenterY_Ctrl);
	DDX_Control(pDX, IDC_BUTTON_HDRAWPARA_PICKCENTER, m_PickButton);
	DDX_Check(pDX, IDC_CHECK_HDRAWPARA_ISBASEGRAY, m_IsBaseMapGray);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_INTERINFOR, m_IntersectionTextH);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_MILE, m_MileTextH);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_PILE, m_PileTextH);
	DDX_Text(pDX, IDC_EDIT_HDRAWPARA_EARTHINFOR, m_EarthInforTextH);
}


BEGIN_MESSAGE_MAP(CHDrawParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CHDrawParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHDrawParaDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_HDRAWPARA_PICKCENTER, &CHDrawParaDlg::OnBnClickedButtonHdrawparaPickcenter)
	ON_BN_CLICKED(IDC_RADIO_HDRAWPARA_SETBYMOVE, &CHDrawParaDlg::OnBnClickedRadioHdrawparaSetbymove)
	ON_BN_CLICKED(IDC_RADIO_HDRAWPARA_SETBACENTER, &CHDrawParaDlg::OnBnClickedRadioHdrawparaSetbacenter)
END_MESSAGE_MAP()


// CHDrawParaDlg 消息处理程序




void CHDrawParaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (CheckParaValidate())
	{
		UpdateDrawPara(true);
		CDialog::OnOK();
	}
	else
		return;
}

/*===================================================================
函数名:UpdateDrawPara
------------------------------------------------------------
函数功能:在m_DrawPara和成员变量间相互传递数据
------------------------------------------------------------
输入:	bool bSaveAndValidate	true-传递到m_DrawPara中
								false-传递到各成员变量中
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190902
------------------------------------------------------------
备注：
=====================================================================*/
void CHDrawParaDlg::UpdateDrawPara(bool bSaveAndValidate)
{
	UpdateData(TRUE);

	if (bSaveAndValidate)
	{
		int CurSel = m_Combo_Scale.GetCurSel();
		if (CurSel != CB_ERR)
			m_Combo_Scale.GetLBText(CurSel, m_DrawPara.SCALE);
		else
			AfxMessageBox(L"更新绘图比例时出现错误！");

		CurSel = m_Combo_MileStyle.GetCurSel();
		if (CurSel != CB_ERR)
			m_Combo_MileStyle.GetLBText(CurSel, m_DrawPara.LCBZ);
		else
			AfxMessageBox(L"更新里程标注样式时出现错误！");

		CurSel = m_Combo_Decimals.GetCurSel();
		if (CurSel != CB_ERR)
			m_Combo_Decimals.GetLBText(CurSel, m_DrawPara.XSW);
		else
			AfxMessageBox(L"更新小数位数时出现错误！");

		m_DrawPara.WHT = m_TextHeight;
		m_DrawPara.DelEntInf = m_IsDelEnt;
		m_DrawPara.drawfldzdk = m_IsDrawSeparation;
		m_DrawPara.BZRAL = m_IsDrawRAL;
		m_DrawPara.DrawJD = m_IsNoteJD;
		m_DrawPara.DrawDZQ = m_IsDrawBri;
		m_DrawPara.DrawTun = m_IsDrawTun;
		m_DrawPara.DrawBM = m_IsDrawBM;
		m_DrawPara.DrawCulvet = m_IsDrawSmallBri;
		m_DrawPara.DrawDX = m_IsDrawDX;
		m_DrawPara.ZcLen = m_EarthLen_L;
		m_DrawPara.YcLen = m_EarthLen_R;
		m_DrawPara.ZxWid.Format(L"%lf", m_LineWeight);
		m_DrawPara.YDXWid.Format(L"%lf", m_EarthLineWeight);
		m_DrawPara.FrameLength = m_FrameLen;
		m_DrawPara.FrameHeight = m_FrameHeight;
		m_DrawPara.TopSpace = m_TopSpace;
		m_DrawPara.BottomSpace = m_BottomSpace;
		m_DrawPara.MoveCenterX = m_MoveX;
		m_DrawPara.MoveCenterY = m_MoveY;
		m_DrawPara.OneMapRoadLen = m_RoadLen;
		if (m_CenterMode == 1)
			m_DrawPara.CenterMode = ByCoordinate;
		else
			m_DrawPara.CenterMode = ByMoveDistance;
		m_DrawPara.CenterX = m_DrawCenterX;
		m_DrawPara.CenterY = m_DrawCenterY;
		m_DrawPara.IsBaseMapGray = m_IsBaseMapGray;

		//20191231新增
		m_DrawPara.TextHPara.EarthLineTextH = m_EarthInforTextH;
		m_DrawPara.TextHPara.IntersectionInforTextH = m_IntersectionTextH;
		m_DrawPara.TextHPara.MileTextH = m_MileTextH;
		m_DrawPara.TextHPara.PileTextH = m_PileTextH;
	}
	else
	{
		int GetSel = m_Combo_Scale.FindString(-1, m_DrawPara.SCALE);
		if (GetSel != CB_ERR)
			m_Combo_Scale.SetCurSel(GetSel);
		else
			AfxMessageBox(L"绘图比例设置错误！");

		GetSel = m_Combo_MileStyle.FindString(-1, m_DrawPara.LCBZ);
		if (GetSel != CB_ERR)
			m_Combo_MileStyle.SetCurSel(GetSel);
		else
			AfxMessageBox(L"里程标注样式设置错误！");

		GetSel = m_Combo_Decimals.FindString(-1, m_DrawPara.XSW);
		if (GetSel != CB_ERR)
			m_Combo_Decimals.SetCurSel(GetSel);
		else
			AfxMessageBox(L"小数位数样式设置错误！");

		m_TextHeight = m_DrawPara.WHT;
		m_IsDelEnt = m_DrawPara.DelEntInf;
		m_IsDrawSeparation = m_DrawPara.drawfldzdk;
		m_IsDrawRAL = m_DrawPara.BZRAL;
		m_IsNoteJD = m_DrawPara.DrawJD;
		m_IsDrawBri = m_DrawPara.DrawDZQ;
		m_IsDrawTun = m_DrawPara.DrawTun;
		m_IsDrawBM = m_DrawPara.DrawBM;
		m_IsDrawSmallBri = m_DrawPara.DrawCulvet;
		m_IsDrawDX = m_DrawPara.DrawDX;
		m_EarthLen_L = m_DrawPara.ZcLen;
		m_EarthLen_R = m_DrawPara.YcLen;
		m_LineWeight = _wtof(m_DrawPara.ZxWid);
		m_EarthLineWeight = _wtof(m_DrawPara.YDXWid);
		m_FrameLen = m_DrawPara.FrameLength;
		m_FrameHeight = m_DrawPara.FrameHeight;
		m_TopSpace = m_DrawPara.TopSpace;
		m_BottomSpace = m_DrawPara.BottomSpace;
		m_MoveX = m_DrawPara.MoveCenterX;
		m_MoveY = m_DrawPara.MoveCenterY;
		m_RoadLen = m_DrawPara.OneMapRoadLen;

		if (m_DrawPara.CenterMode == ByCoordinate)
			m_CenterMode = 1;
		else
			m_CenterMode = 0;
		SetDrawModeCtrl();

		m_DrawCenterX = m_DrawPara.CenterX;
		m_DrawCenterY = m_DrawPara.CenterY;

		m_IsBaseMapGray = m_DrawPara.IsBaseMapGray;

		//20191231新增
		m_EarthInforTextH = m_DrawPara.TextHPara.EarthLineTextH;
		m_IntersectionTextH = m_DrawPara.TextHPara.IntersectionInforTextH;
		m_MileTextH = m_DrawPara.TextHPara.MileTextH;
		m_PileTextH = m_DrawPara.TextHPara.PileTextH;

		UpdateData(FALSE);
	}
}


BOOL CHDrawParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDrawPara(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/*===================================================================
函数名:CheckParaValidate
------------------------------------------------------------
函数功能:检测数据是否有效
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:	bool	返回值		true-有效	false-无效
------------------------------------------------------------
算法描述:
------------------------------------------------------------
编写日期:20190902
------------------------------------------------------------
备注：
=====================================================================*/
bool CHDrawParaDlg::CheckParaValidate()
{
	UpdateData(TRUE);

	int CurSel = m_Combo_Scale.GetCurSel();
	if (CurSel == CB_ERR)
	{
		AfxMessageBox(L"绘图比例选择存在错误！");
		return false;
	}

	CurSel = m_Combo_MileStyle.GetCurSel();
	if (CurSel == CB_ERR)
	{
		AfxMessageBox(L"里程标注样式选择存在错误！");
		return false;
	}

	CurSel = m_Combo_Decimals.GetCurSel();
	if (CurSel == CB_ERR)
	{
		AfxMessageBox(L"小数位数选择存在错误！");
		return false;
	}

	if (m_TextHeight < 0.1)
	{
		AfxMessageBox(L"字高设置错误！");
		return false;
	}

	if (m_EarthLen_L < 0.0)
	{
		AfxMessageBox(L"左占地宽线长设置错误！");
		return false;
	}

	if (m_EarthLen_R < 0.0)
	{
		AfxMessageBox(L"右占地宽线长设置错误！");
		return false;
	}

	if (m_LineWeight < 0.0)
	{
		AfxMessageBox(L"道路中线线宽设置错误！");
		return false;
	}

	if (m_EarthLineWeight < 0.0)
	{
		AfxMessageBox(L"占地宽线宽设置错误！");
		return false;
	}

	return true;
}


void CHDrawParaDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CHDrawParaDlg::OnBnClickedButtonHdrawparaPickcenter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	struct resbuf result;
	int type;
	ads_point pt, ptold, LDpt, LUpt, RDpt, RUpt, LDptold, LUptold, RDptold, RUptold;
	pt[X] = pt[Y] = pt[Z] = 0;
	ptold[X] = ptold[Y] = ptold[Z] = 0;
	LDpt[X] = LDpt[Y] = LDpt[Z] = 0;
	LUpt[X] = LUpt[Y] = LUpt[Z] = 0;
	RDpt[X] = RDpt[Y] = LUpt[Z] = 0;
	LDptold[X] = LDptold[Y] = LDptold[Z] = 0;
	LUptold[X] = LUptold[Y] = LUptold[Z] = 0;
	RDptold[X] = RDptold[Y] = RDptold[Z] = 0;
	RUptold[X] = RUptold[Y] = RUptold[Z] = 0;
	double FrameWidth = m_FrameLen - 20;
	double FrameHeight = m_FrameHeight - m_TopSpace - m_BottomSpace;
	if (m_BottomSpace == 0)
		FrameHeight -= 10.0;
	else
		FrameHeight -= 20.0;


	GetParent()->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	acedGetAcadFrame()->EnableWindow(TRUE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.
	

	for (;;)
	{
		if (ads_grread(1, &type, &result) != RTNORM)
		{
			break;
		}
		if (type == 3)
		{
			pt[X] = result.resval.rpoint[X];
			pt[Y] = result.resval.rpoint[Y];
			pt[Z] = result.resval.rpoint[Z];
			m_DrawCenterX = pt[X];
			m_DrawCenterY = pt[Y];
			break;
		}
		if (type != 5 && type != 12)//||(type != 3 ))
		{
			ads_printf(L"\n必须用鼠标! ");
		}
		if (type == 5)
		{
			pt[X] = result.resval.rpoint[X];
			pt[Y] = result.resval.rpoint[Y];
			pt[Z] = result.resval.rpoint[Z];
			LUpt[X] = pt[X] - FrameWidth  /2.0, LUpt[Y] = pt[Y] + FrameHeight / 2.0;
			ads_polar(LUpt, 0.0, FrameWidth, RUpt);
			ads_polar(RUpt, 0.5 * PI, -FrameHeight, RDpt);
			ads_polar(LUpt, 0.5 * PI, -FrameHeight, LDpt);

			if (ptold[X] < 0.1)
			{
				ads_grdraw(LUpt, RUpt, -1, 0);
				ads_grdraw(RUpt, RDpt, -1, 0);
				ads_grdraw(RDpt, LDpt, -1, 0);
				ads_grdraw(LDpt, LUpt, -1, 0);
			}
			else if (BAS_DRAW_FUN::caldistance(pt, ptold) > 0.1)
			{
				acedRedraw(NULL, 0);
				ads_grdraw(LUpt, RUpt, -1, 0);
				ads_grdraw(RUpt, RDpt, -1, 0);
				ads_grdraw(RDpt, LDpt, -1, 0);
				ads_grdraw(LDpt, LUpt, -1, 0);
			}
			Cpoint(LUptold, LUpt);
			Cpoint(RUptold, RUpt);
			Cpoint(RDptold, RDpt);
			Cpoint(LDptold, LDpt);
			Cpoint(ptold, pt);
		}
	}

	GetParent()->ShowWindow(SW_SHOW);
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	acedGetAcadFrame()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/
	UpdateData(false);
}

void CHDrawParaDlg::SetDrawModeCtrl()
{
	if (m_CenterMode == 0)
	{
		//禁止坐标输入
		m_DrawCenterX_Ctrl.SetReadOnly(TRUE);
		m_DrawCenterY_Ctrl.SetReadOnly(TRUE);
		m_PickButton.EnableWindow(FALSE);

		//开放距离输入
		m_MoveCenterX_Ctrl.SetReadOnly(FALSE);
		m_MoveCenterY_Ctrl.SetReadOnly(FALSE);
	}
	else
	{
		//开放坐标输入
		m_DrawCenterX_Ctrl.SetReadOnly(FALSE);
		m_DrawCenterY_Ctrl.SetReadOnly(FALSE);
		m_PickButton.EnableWindow(TRUE);

		//禁止距离输入
		m_MoveCenterX_Ctrl.SetReadOnly(TRUE);
		m_MoveCenterY_Ctrl.SetReadOnly(TRUE);
	}

}


void CHDrawParaDlg::OnBnClickedRadioHdrawparaSetbymove()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetDrawModeCtrl();
}


void CHDrawParaDlg::OnBnClickedRadioHdrawparaSetbacenter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	SetDrawModeCtrl();
}
