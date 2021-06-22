// CCrossDrawParaDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CCrossDrawParaDlg.h"
#include "afxdialogex.h"


// CCrossDrawParaDlg 对话框

IMPLEMENT_DYNAMIC(CCrossDrawParaDlg, CDialog)

CCrossDrawParaDlg::CCrossDrawParaDlg()
	: CDialog(IDD_DIALOG_CROSSDRAWPARA)
	, m_Scale(200)
	, m_IsDrawGrid(FALSE)
	, m_IsDrawEarthPile(FALSE)
	, m_EarthWidL(5)
	, m_EarthWidR(5)
	, m_TabTextH(0)
	, m_DrawTextH(0)
{

}

CCrossDrawParaDlg::~CCrossDrawParaDlg()
{
}

void CCrossDrawParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_HDMSCALE, m_Scale);
	DDV_MinMaxDouble(pDX, m_Scale, 1, 10000000);
	DDX_Check(pDX, IDC_CHECK_IsDrawHengDuanMianWangGe, m_IsDrawGrid);
	DDX_Check(pDX, IDC_CHECK_AUTODES_DRAWEARTHPI, m_IsDrawEarthPile);
	DDX_Text(pDX, IDC_EDIT_ZK, m_EarthWidL);
	DDX_Text(pDX, IDC_EDIT_YK, m_EarthWidR);
	DDX_Text(pDX, IDC_EDIT_TabTextH, m_TabTextH);
	DDX_Text(pDX, IDC_EDIT_DrawTextH, m_DrawTextH);
}


BEGIN_MESSAGE_MAP(CCrossDrawParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCrossDrawParaDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCrossDrawParaDlg 消息处理程序



BOOL CCrossDrawParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDrawPara(false);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
//参数传递
void CCrossDrawParaDlg::UpdateDrawPara(bool bSaveAndValidate)
{
	UpdateData(TRUE);

	if (bSaveAndValidate)
	{
		m_DrawPara.Scale = m_Scale;
		m_DrawPara.IsDrawGrid = m_IsDrawGrid;
		m_DrawPara.IsDrawEarthPile = m_IsDrawEarthPile;
		m_DrawPara.EarthWidL = m_EarthWidL;
		m_DrawPara.EarthWidR = m_EarthWidR;

		m_DrawPara.TabTextH = m_TabTextH;
		m_DrawPara.BZTextH = m_DrawTextH;
	}
	else
	{
		m_Scale = m_DrawPara.Scale;
		m_IsDrawGrid = m_DrawPara.IsDrawGrid;
		m_IsDrawEarthPile = m_DrawPara.IsDrawEarthPile;
		m_EarthWidL = m_DrawPara.EarthWidL;
		m_EarthWidR = m_DrawPara.EarthWidR;

		m_TabTextH = m_DrawPara.TabTextH;
		m_DrawTextH = m_DrawPara.BZTextH;

		UpdateData(FALSE);
	}
}
//检测函数
bool CCrossDrawParaDlg::CheckParaValidate()
{
	if (m_Scale < 1)
	{
		AfxMessageBox(L"比例尺设置错误，请检查！");
		return false;
	}

	if (m_EarthWidL < 0.0)
	{
		AfxMessageBox(L"左地面线宽设置错误，请检查！");
		return false;
	}

	if (m_EarthWidR < 0.0)
	{
		AfxMessageBox(L"右地面线宽设置错误，请检查！");
		return false;
	}

	return true;
}


void CCrossDrawParaDlg::OnBnClickedOk()
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
