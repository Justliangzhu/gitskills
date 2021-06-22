// COverallDrawParaDlg.cpp: 实现文件
//
#include "stdafx.h"
#include "COverallDrawParaDlg.h"
#include "afxdialogex.h"
#include "acutmem.h"

// COverallDrawParaDlg 对话框

IMPLEMENT_DYNAMIC(COverallDrawParaDlg, CDialog)

COverallDrawParaDlg::COverallDrawParaDlg()
	: CDialog(IDD_DIALOG_OVERALLDRAWPARA)
	, m_FrameLen(0)
	, m_FrameWid(0)
	, m_FrameSizeMode(0)
	, m_PageNum(1)
	, m_IsDrawPage(FALSE)
	, m_TotalPageNum(0)
	, m_IsAssignPage(FALSE)
	, m_IsDrawName(FALSE)
	, m_OutputMode(0)
	, m_IsDrawOval(FALSE)
	, m_IsDrawHoriRoad(FALSE)
	, m_IsDrawEarth(FALSE)
	, m_IsDrawProfile(FALSE)
	, m_IsDrawCross(FALSE)
	, m_IsDrawCurInfo(FALSE)
	, m_IsDrawRoadbed(FALSE)
	, m_IsAssignTotalPageNum(FALSE)
{

}

COverallDrawParaDlg::~COverallDrawParaDlg()
{
}

void COverallDrawParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OVALPARA_TEXTSTYLE, m_Combo_TextStyle);
	DDX_Text(pDX, IDC_EDIT_OVALPARA_LEN, m_FrameLen);
	DDV_MinMaxDouble(pDX, m_FrameLen, 0.0, 50000.0);
	DDX_Text(pDX, IDC_EDIT_OVALPARA_WID, m_FrameWid);
	DDV_MinMaxDouble(pDX, m_FrameWid, 0.0, 50000.0);
	DDX_Control(pDX, IDC_EDIT_OVALPARA_LEN, m_FrameLenCtrl);
	DDX_Control(pDX, IDC_EDIT_OVALPARA_WID, m_FrameWidCtrl);
	DDX_Radio(pDX, IDC_RADIO_OVALPARA_A4, m_FrameSizeMode);
	DDX_Control(pDX, IDC_EDIT_OVALPARA_STARTPAGE, m_PageNumCtrl);
	DDX_Text(pDX, IDC_EDIT_OVALPARA_STARTPAGE, m_PageNum);
	DDV_MinMaxInt(pDX, m_PageNum, 0, INT_MAX);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_DRAWPAGE, m_IsDrawPage);
	DDX_Text(pDX, IDC_EDIT_OVALPARA_TOTALPAGE, m_TotalPageNum);
	DDX_Control(pDX, IDC_EDIT_OVALPARA_TOTALPAGE, m_TotalPageNumCtrl);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_ASSIGNPAGE, m_IsAssignPage);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_NOTENAME, m_IsDrawName);
	DDX_Radio(pDX, IDC_RADIO_OVALPARA_BREAKOUTPUT, m_OutputMode);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_OVALROAD, m_IsDrawOval);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_HORIROAD, m_IsDrawHoriRoad);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_EARTH, m_IsDrawEarth);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_PROFILE, m_IsDrawProfile);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_CROSS, m_IsDrawCross);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_CURINFO, m_IsDrawCurInfo);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_ROADBED, m_IsDrawRoadbed);
	DDX_Check(pDX, IDC_CHECK_OVALPARA_ASSIGNTOTALPAGE, m_IsAssignTotalPageNum);
}


BEGIN_MESSAGE_MAP(COverallDrawParaDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_OVALPARA_A4, &COverallDrawParaDlg::OnBnClickedRadioOvalparaA4)
	ON_BN_CLICKED(IDC_RADIO_OVALPARA_A3, &COverallDrawParaDlg::OnBnClickedRadioOvalparaA3)
	ON_BN_CLICKED(IDC_RADIO_OVALPARA_A2, &COverallDrawParaDlg::OnBnClickedRadioOvalparaA2)
	ON_BN_CLICKED(IDC_RADIO_OVALPARA_A1, &COverallDrawParaDlg::OnBnClickedRadioOvalparaA1)
	ON_BN_CLICKED(IDC_RADIO_OVALPARA_CUSTOM, &COverallDrawParaDlg::OnBnClickedRadioOvalparaCustom)
	ON_BN_CLICKED(IDC_CHECK_OVALPARA_DRAWPAGE, &COverallDrawParaDlg::OnBnClickedCheckOvalparaDrawpage)
	ON_BN_CLICKED(IDC_CHECK_OVALPARA_ASSIGNPAGE, &COverallDrawParaDlg::OnBnClickedCheckOvalparaAssignpage)
	ON_BN_CLICKED(IDC_CHECK_OVALPARA_ASSIGNTOTALPAGE, &COverallDrawParaDlg::OnBnClickedCheckOvalparaAssigntotalpage)
	ON_BN_CLICKED(IDOK, &COverallDrawParaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COverallDrawParaDlg 消息处理程序



BOOL COverallDrawParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitComboBox();
	OnBnClickedRadioOvalparaA3();

	m_PageNumCtrl.SetReadOnly(TRUE);
	m_TotalPageNumCtrl.SetReadOnly(TRUE);

	UpdateDrawPara(false);

	CRect TempRect;
	GetWindowRect(TempRect);
	MoveWindow(TempRect.TopLeft().x, TempRect.TopLeft().y, 230, TempRect.Height());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void COverallDrawParaDlg::InitComboBox()
{
	//初始化文字样式
	m_Combo_TextStyle.ResetContent();
	AcDbTextStyleTable* pTextStyleTbl = NULL;
	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTextStyleTbl, AcDb::kForRead);
	AcDbTextStyleTableIterator* pIt = NULL;
	pTextStyleTbl->newIterator(pIt);
	for (; !pIt->done(); pIt->step())
	{
		AcDbTextStyleTableRecord* pRcd = NULL;
		if (pIt->getRecord(pRcd, AcDb::kForRead) == Acad::eOk)
		{
			TCHAR* szName = NULL;
			pRcd->getName(szName);
			if (_tcslen(szName) > 0)
			{
				m_Combo_TextStyle.AddString(szName);
			}
			acutDelString(szName);
			pRcd->close();
		}
	}
	delete pIt;
	pTextStyleTbl->close();

	//设置默认文字样式，如果有Standard则默认选择Standard，否则在有文字样式的情况下，默认选择第一项
	int Sel = m_Combo_TextStyle.FindString(-1, L"Standard");
	if (Sel != CB_ERR)
		m_Combo_TextStyle.SetCurSel(Sel);
	else if (m_Combo_TextStyle.GetCount())
		m_Combo_TextStyle.SetCurSel(0);

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedRadioOvalparaA4()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_FrameSizeMode = 0;

	m_FrameLenCtrl.SetReadOnly(TRUE);
	m_FrameWidCtrl.SetReadOnly(TRUE);

	m_FrameLen = 297.0;
	m_FrameWid = 210.0;

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedRadioOvalparaA3()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_FrameSizeMode = 1;

	m_FrameLenCtrl.SetReadOnly(TRUE);
	m_FrameWidCtrl.SetReadOnly(TRUE);

	m_FrameLen = 420.0;
	m_FrameWid = 297.0;

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedRadioOvalparaA2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_FrameSizeMode = 2;

	m_FrameLenCtrl.SetReadOnly(TRUE);
	m_FrameWidCtrl.SetReadOnly(TRUE);

	m_FrameLen = 594.0;
	m_FrameWid = 420.0;

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedRadioOvalparaA1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	m_FrameLenCtrl.SetReadOnly(TRUE);
	m_FrameWidCtrl.SetReadOnly(TRUE);

	m_FrameSizeMode = 3;

	m_FrameLen = 841.0;
	m_FrameWid = 594.0;

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedRadioOvalparaCustom()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	m_FrameSizeMode = 4;

	m_FrameLenCtrl.SetReadOnly(FALSE);
	m_FrameWidCtrl.SetReadOnly(FALSE);

	UpdateData(FALSE);
}



void COverallDrawParaDlg::OnBnClickedCheckOvalparaDrawpage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	//if (m_IsDrawPage)
	//{
	//	m_PageNumCtrl.SetReadOnly(FALSE);
	//}
	//else
	//{
	//	m_PageNumCtrl.SetReadOnly(TRUE);
	//}
	UpdateData(FALSE);
}

void COverallDrawParaDlg::OnBnClickedCheckOvalparaAssignpage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_IsAssignPage)
	{
		m_PageNumCtrl.SetReadOnly(FALSE);
	}
	else
	{
		m_PageNumCtrl.SetReadOnly(TRUE);
	}

	UpdateData(FALSE);
}

//传递参数
void COverallDrawParaDlg::UpdateDrawPara(bool bSaveAndValidate)
{
	UpdateData(TRUE);

	if (bSaveAndValidate)
	{
		m_DrawOvalPara.IsDrawHoriOvalDrawing = m_IsDrawOval;
		m_DrawOvalPara.IsDrawHoriRoad = m_IsDrawHoriRoad;
		m_DrawOvalPara.IsDrawHoriEarth = m_IsDrawEarth;
		m_DrawOvalPara.IsDrawProfile = m_IsDrawProfile;
		m_DrawOvalPara.IsDrawCross = m_IsDrawCross;
		m_DrawOvalPara.IsDrawCurveInfo = m_IsDrawCurInfo;
		m_DrawOvalPara.IsDrawRoadbed = m_IsDrawRoadbed;

		if (m_OutputMode == 0)
			m_DrawOvalPara.OutMode = OutputDrawingMode::Break;
		else
			m_DrawOvalPara.OutMode = OutputDrawingMode::Layout;

		m_DrawOvalPara.IsNotePage = m_IsDrawPage;
		m_DrawOvalPara.IsNoteDrawingName = m_IsDrawName;

		if (m_IsAssignPage)
			m_DrawOvalPara.StartPageNum = m_PageNum;
		else
			m_DrawOvalPara.StartPageNum = 1;

		if(m_IsAssignTotalPageNum)
			m_DrawOvalPara.TotalPageNum = m_TotalPageNum;
		else
			m_DrawOvalPara.TotalPageNum = 0;
	}
	else
	{
		m_IsDrawOval = m_DrawOvalPara.IsDrawHoriOvalDrawing;
		m_IsDrawHoriRoad = m_DrawOvalPara.IsDrawHoriRoad;
		m_IsDrawEarth = m_DrawOvalPara.IsDrawHoriEarth;
		m_IsDrawProfile = m_DrawOvalPara.IsDrawProfile;
		m_IsDrawCross = m_DrawOvalPara.IsDrawCross;
		m_IsDrawCurInfo = m_DrawOvalPara.IsDrawCurveInfo;
		m_IsDrawRoadbed = m_DrawOvalPara.IsDrawRoadbed;

		if (m_DrawOvalPara.OutMode == OutputDrawingMode::Break)
			m_OutputMode = 0;
		else
			m_OutputMode = 1;

		m_IsDrawPage = m_DrawOvalPara.IsNotePage;
		m_IsDrawName = m_DrawOvalPara.IsNoteDrawingName;

		if (m_IsAssignPage)
			m_PageNum = m_DrawOvalPara.StartPageNum;

		if (m_IsAssignTotalPageNum)
			m_TotalPageNum = m_DrawOvalPara.TotalPageNum;

		UpdateData(FALSE);
	}
}


void COverallDrawParaDlg::OnBnClickedCheckOvalparaAssigntotalpage()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);

	if (m_IsAssignTotalPageNum)
	{
		m_TotalPageNumCtrl.SetReadOnly(FALSE);
	}
	else
	{
		m_TotalPageNumCtrl.SetReadOnly(TRUE);
	}

	UpdateData(FALSE);
}


void COverallDrawParaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDrawPara(true);
	CDialog::OnOK();
}
