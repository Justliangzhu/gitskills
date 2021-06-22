// CGutInteractDlg.cpp: 实现文件
//
#include "StdAfx.h"
#include "CGutInteractDlg.h"
#include "afxdialogex.h"
#include "DesignLine.h"
#include "HdmDes.h"
#include "RoadDesignWorkingDatabase.h"

// CGutInteractDlg 对话框
#define CG		1	//侧沟
#define LDSG	2	//路堤水沟	排水沟
#define TG		3	//天沟

IMPLEMENT_DYNAMIC(CGutInteractDlg, CDialog)
extern HdmDes HDM;
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库

CGutInteractDlg::CGutInteractDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_GUTINTERACT, pParent)
	, m_InteractMode(0)
{
	m_GutterTpye = 1;
}

CGutInteractDlg::~CGutInteractDlg()
{
}

void CGutInteractDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_GUTINTERACT_ADD, m_InteractMode);
	DDX_Control(pDX, IDC_LIST_GUTINTERACT_PARATABLE, m_GridCtrl);
	DDX_Control(pDX, IDC_COMBO_GUTINTERACT_GUTTYPE, m_GutterTypeCombo);
}


BEGIN_MESSAGE_MAP(CGutInteractDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_GUTINTERACT_PICK, &CGutInteractDlg::OnBnClickedButtonGutinteractPick)
	ON_CBN_EDITCHANGE(IDC_COMBO_GUTINTERACT_GUTTYPE, &CGutInteractDlg::OnCbnEditchangeComboGutinteractGuttype)
	ON_CBN_SELCHANGE(IDC_COMBO_GUTINTERACT_GUTTYPE, &CGutInteractDlg::OnCbnSelchangeComboGutinteractGuttype)
	ON_BN_CLICKED(IDC_RADIO_GUTINTERACT_ADD, &CGutInteractDlg::OnBnClickedRadioGutinteractAdd)
	ON_BN_CLICKED(IDC_RADIO_GUTINTERACT_DEL, &CGutInteractDlg::OnBnClickedRadioGutinteractDel)
END_MESSAGE_MAP()


// CGutInteractDlg 消息处理程序


void CGutInteractDlg::OnBnClickedButtonGutinteractPick()
{
	// TODO: 在此添加控件通知处理程序代码
	//交点聚集在CAD窗口上
	ShowWindow(SW_HIDE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	ads_name ent;
	AcDbEntity* pEnt;
	DesignLine_ROAD* desline;
	AcDbObjectId DesId;
	ads_point pt;

	ACHAR Tip[256];
	if (m_InteractMode == 0)
		_tcscpy(Tip, L"\n请选择要增加水沟的的横断面设计线实体:");
	else
		_tcscpy(Tip, L"\n请选择要删除的横断面实体:");

	if (RTNORM != acedEntSel(Tip, ent, pt))
	{
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		return;
	}
	acdbGetObjectId(DesId, ent);
	if (acdbOpenObject(pEnt, DesId, AcDb::kForRead) != Acad::eOk)
		return;


	if (m_InteractMode == 1)
	{
		if (!pEnt->isKindOf(DesignLine_ROAD::desc()) && !pEnt->isKindOf(LTGOU_ROAD::desc()) && !pEnt->isKindOf(JSGOU_ROAD::desc()))
		{
			ads_printf(L"所选不是横断面实体！");
			pEnt->close();
			ShowWindow(SW_SHOW);
			SetFocus();
			return;
		}
		pEnt->close();
		
		//删除横断面水沟
		HDM.DeleteGutter(DesId);
	}
	else
	{
		if (!pEnt->isKindOf(DesignLine_ROAD::desc()))
		{
			ads_printf(L"所选不是横断面设计线实体！");
			pEnt->close();
			ShowWindow(SW_SHOW);
			SetFocus();
			return;
		}
		pEnt->close();

		//读取用户设置的水沟参数
		ReadGridCtrl();

		AcDbObject* pObj = NULL;

		acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);

		int wErrorStatus = acdbOpenObject(pObj, DesId, AcDb::kForWrite);
		if (wErrorStatus == Acad::eOk)
		{
			if (pObj->isKindOf(DesignLine_ROAD::desc()))
			{
				DesignLine_ROAD* desline = NULL;
				desline = DesignLine_ROAD::cast(pObj);
				desline->assertWriteEnabled();//加响应器修改实体时，不可少
				desline->m_GutterType = m_GutterTpye;
				if (m_GutterTpye == CG)
					desline->SetLateralGutterPara(m_CG_GK, m_CG_GS, m_CG_GP1, m_CG_GP2, m_CG_JQHD, m_CG_GDR);
				else if (m_GutterTpye == LDSG)
					desline->SetLDGPara(m_LDG_T2W, m_LDG_T1W, m_LDG_T2H, m_LDG_GH, m_LDG_GW, m_LDG_HH, m_LDG_SP1, m_LDG_SP2, m_LDG_SP3, m_LDG_JQHD, m_LDG_GDR);
				else if (m_GutterTpye == TG)
					desline->SetTGPara(m_TG_BJ, m_TG_GW, m_TG_GS, m_TG_SP1, m_TG_SP2, m_TG_JQHD, m_TG_GDR);
				desline->close();
			}
		}
	}


	ShowWindow(SW_SHOW);
	SetFocus();	
}


BOOL CGutInteractDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_InteractMode = 1;
	SetInitGutterPara();

	m_GutterTypeCombo.AddString(L"侧沟");
	m_GutterTypeCombo.AddString(L"路堤水沟");
	m_GutterTypeCombo.AddString(L"天沟");

	m_GutterTypeCombo.EnableWindow(FALSE);
	m_GridCtrl.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CGutInteractDlg::SetInitGutterPara()
{
	BAS_DRAW_FUN BasFun;
	CString MdbName = BasFun.GetCurProMdbPath();

	//获取侧沟参数
	std::vector<LQSGdata> LQSGArray;
	if (g_RoadWorkingDatabase.GetLQSG(MdbName, L"主线", LEFT, LQSGArray) && LQSGArray.size())
	{
		m_CG_GK = LQSGArray[0].gw;
		m_CG_GS = LQSGArray[0].gs; 
		m_CG_GP1 = LQSGArray[0].nbp;
		m_CG_GP2 = LQSGArray[0].wbp;
		m_CG_JQHD = LQSGArray[0].Jh;
		m_CG_GDR = LQSGArray[0].GDR;
	}
	else
	{
		m_CG_GK = 0.4;
		m_CG_GS = 0.6;
		m_CG_GP1 = 0.0;
		m_CG_GP2 = 0.0;
		m_CG_JQHD = 0.3;
		m_CG_GDR = 0.0;
	}

	//获取路堤水沟参数	均取起点参数
	std::vector<LDSGdata> LDSGArray;
	if (g_RoadWorkingDatabase.GetLDSG(MdbName, L"主线", LEFT, LDSGArray) && LDSGArray.size())
	{
		m_LDG_T2W = LDSGArray[0].S_DSYK;
		m_LDG_T1W = LDSGArray[0].S_HPDK;
		m_LDG_T2H = LDSGArray[0].S_DSYH;
		m_LDG_GH = LDSGArray[0].S_GS;
		m_LDG_GW = LDSGArray[0].S_GK;
		m_LDG_HH = LDSGArray[0].S_HPDH;
		m_LDG_SP1 = LDSGArray[0].nbp;
		m_LDG_SP2 = LDSGArray[0].wbp;
		m_LDG_SP3 = LDSGArray[0].DSYP; 
		m_LDG_JQHD = LDSGArray[0].jh;
		m_LDG_GDR = LDSGArray[0].LDSG_GDR;
	}
	else
	{
		m_LDG_T2W = 1.0;
		m_LDG_T1W = 2.0;
		m_LDG_T2H = 0.5;
		m_LDG_GH = 0.6;
		m_LDG_GW = 0.4;
		m_LDG_HH =0.5;
		m_LDG_SP1 = 1.0;
		m_LDG_SP2 = 1.0;
		m_LDG_SP3 = 1.0;
		m_LDG_JQHD = 0.2;
		m_LDG_GDR = 0.0;
	}

	//获取天沟参数
	std::vector<TGdata> TGArray;
	if (g_RoadWorkingDatabase.GetTG(MdbName, L"主线", LEFT, TGArray) && TGArray.size())
	{
		m_TG_BJ = TGArray[0].sbj;
		m_TG_GW = TGArray[0].sgw;
		m_TG_GS = TGArray[0].sgs;
		m_TG_SP1 = TGArray[0].nbp;
		m_TG_SP2 = TGArray[0].wbp;
		m_TG_JQHD = TGArray[0].jqhd;
		m_TG_GDR = TGArray[0].TG_GDR;
	}
	else
	{
		m_TG_BJ = 2.0;
		m_TG_GW = 0.4;
		m_TG_GS = 0.6;
		m_TG_SP1 = 1.0;
		m_TG_SP2 = 1.0;
		m_TG_JQHD = 0.3;
		m_TG_GDR = 0.0;
	}
}

void CGutInteractDlg::WriteGridCtrl(std::vector<CString>& TitleArray, std::vector<double>& ParaArray)
{
	ACHAR tmp[50];

	int ColumnNum = TitleArray.size() + 1;

	//设置行和列
	m_GridCtrl.SetRowCount(2);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetColumnCount(ColumnNum);
	m_GridCtrl.SetFixedColumnCount(1);

	//设置单元格属性

	LOGFONT font = { 12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial" };
	m_Item.mask = GVIF_TEXT | GVIF_FORMAT;
	m_Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	m_Item.lfFont = font;

	//传入单元格属性
	for (int k = 0; k < m_GridCtrl.GetColumnCount(); k++)
	{
		for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			m_GridCtrl.SetItem(&m_Item);
		}
	}

	//设置标题
	m_GridCtrl.SetItemText(0, 0, L"序号");
	for (int i = 0; i < TitleArray.size(); i++)
	{
		m_GridCtrl.SetItemText(0, i + 1, TitleArray[i]);
		_stprintf(tmp, L"%.3lf", ParaArray[i]);
		m_GridCtrl.SetItemText(1, i + 1, tmp);
	}
		

	//设置宽度与长度
	for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		m_GridCtrl.SetRowHeight(i, DEFAULTCOLHEI);

	//设置序列号
	for (int i = 1; i < m_GridCtrl.GetRowCount(); i++)
	{
		_stprintf(tmp, L"%d", i);
		m_GridCtrl.SetItemText(i, 0, tmp);
	}

	//额外设置
	m_GridCtrl.ResetScrollBars();
	m_GridCtrl.SetFocusCell(1, 1);
	m_GridCtrl.AutoSize();

	UpdateData(FALSE);
	m_GridCtrl.Invalidate();
}

//读取表格数据
void CGutInteractDlg::ReadGridCtrl()
{
	UpdateData(TRUE);
	CString tempCStr;

	if (m_GutterTpye == CG)
	{
		tempCStr = m_GridCtrl.GetItemText(1, 1);
		m_CG_GK = _wtof(tempCStr);
		
		tempCStr = m_GridCtrl.GetItemText(1, 2);
		m_CG_GS = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 3);
		m_CG_GP1 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 4);
		m_CG_GP2 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 5);
		m_CG_JQHD = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 6);
		m_CG_GDR = _wtof(tempCStr);
	}
	else if (m_GutterTpye == LDSG)
	{
		tempCStr = m_GridCtrl.GetItemText(1, 1);
		m_LDG_JQHD = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 2);
		m_LDG_T1W = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 3);
		m_LDG_HH = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 4);
		m_LDG_GW = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 5);
		m_LDG_GH = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 6);
		m_LDG_T2W = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 7);
		m_LDG_T2H = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 8);
		m_LDG_SP1 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 9);
		m_LDG_SP2 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 10);
		m_LDG_SP3 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 11);
		m_LDG_GDR = _wtof(tempCStr);
	}
	else if (m_GutterTpye == TG)
	{
		tempCStr = m_GridCtrl.GetItemText(1, 1);
		m_TG_BJ = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 2);
		m_TG_GS = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 3);
		m_TG_GW = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 4);
		m_TG_SP1 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 5);
		m_TG_SP2 = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 6);
		m_TG_JQHD = _wtof(tempCStr);

		tempCStr = m_GridCtrl.GetItemText(1, 7);
		m_TG_GDR = _wtof(tempCStr);
	}
}

void CGutInteractDlg::OnCbnEditchangeComboGutinteractGuttype()
{

}


void CGutInteractDlg::OnCbnSelchangeComboGutinteractGuttype()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	int CurSel = m_GutterTypeCombo.GetCurSel();
	if (CurSel == CB_ERR)
	{
		AfxMessageBox(L"下拉框未知错误！");
		return;
	}
	CString GutName;
	m_GutterTypeCombo.GetLBText(CurSel, GutName);
	if (GutName == L"侧沟")
		m_GutterTpye = CG;
	else if (GutName == L"路堤水沟")
		m_GutterTpye = LDSG;
	else if (GutName == L"天沟")
		m_GutterTpye = TG;

	std::vector<CString> TitleArray;
	std::vector<double> ParaArray;

	if (m_GutterTpye == CG)
	{
		TitleArray.push_back(L"水沟底宽(m)");
		TitleArray.push_back(L"沟深(m)");
		TitleArray.push_back(L"内侧边坡(1:)");
		TitleArray.push_back(L"外侧边坡(1:)");
		TitleArray.push_back(L"浆砌厚度(m)");
		TitleArray.push_back(L"沟底半径(m)");

		ParaArray.push_back(m_CG_GK);
		ParaArray.push_back(m_CG_GS);
		ParaArray.push_back(m_CG_GP1);
		ParaArray.push_back(m_CG_GP2);
		ParaArray.push_back(m_CG_JQHD);
		ParaArray.push_back(m_CG_GDR);
	}
	else if (m_GutterTpye == LDSG)
	{
		TitleArray.push_back(L"浆砌厚度(m)");
		TitleArray.push_back(L"护坡道宽(m)");
		TitleArray.push_back(L"护坡道厚(m)");
		TitleArray.push_back(L"水沟底宽(m)");
		TitleArray.push_back(L"水沟深(m)");
		TitleArray.push_back(L"挡水堰宽(m)");
		TitleArray.push_back(L"挡水堰厚(m)");
		TitleArray.push_back(L"内侧边坡(1:)");
		TitleArray.push_back(L"外侧边坡(1:)");
		TitleArray.push_back(L"挡水堰边坡(1:)");
		TitleArray.push_back(L"沟底半径(m)");

		ParaArray.push_back(m_LDG_JQHD);
		ParaArray.push_back(m_LDG_T1W);
		ParaArray.push_back(m_LDG_HH);
		ParaArray.push_back(m_LDG_GW);
		ParaArray.push_back(m_LDG_GH);
		ParaArray.push_back(m_LDG_T2W);
		ParaArray.push_back(m_LDG_T2H);
		ParaArray.push_back(m_LDG_SP1);
		ParaArray.push_back(m_LDG_SP2);
		ParaArray.push_back(m_LDG_SP3);
		ParaArray.push_back(m_LDG_GDR);
	}
	else if (m_GutterTpye == TG)
	{
		TitleArray.push_back(L"边距(m)");
		TitleArray.push_back(L"水沟深(m)");
		TitleArray.push_back(L"水沟底宽(m)");
		TitleArray.push_back(L"内侧边坡(1:)");
		TitleArray.push_back(L"外侧边坡(1:)");
		TitleArray.push_back(L"浆砌厚度(m)");
		TitleArray.push_back(L"沟底半径(m)");

		ParaArray.push_back(m_TG_BJ);
		ParaArray.push_back(m_TG_GS);
		ParaArray.push_back(m_TG_GW);
		ParaArray.push_back(m_TG_SP1);
		ParaArray.push_back(m_TG_SP2);
		ParaArray.push_back(m_TG_JQHD);
		ParaArray.push_back(m_TG_GDR);
	}

	WriteGridCtrl(TitleArray, ParaArray);
}


void CGutInteractDlg::OnBnClickedRadioGutinteractAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_GutterTypeCombo.EnableWindow(TRUE);
	m_GridCtrl.EnableWindow(TRUE);
	if (m_GutterTypeCombo.GetCurSel() == CB_ERR)
		m_GutterTypeCombo.SetCurSel(0);

	OnCbnSelchangeComboGutinteractGuttype();
}


void CGutInteractDlg::OnBnClickedRadioGutinteractDel()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_GutterTypeCombo.EnableWindow(FALSE);
	m_GridCtrl.EnableWindow(FALSE);
}
