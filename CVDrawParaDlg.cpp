// CVDrawParaDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CVDrawParaDlg.h"
#include "afxdialogex.h"


// CVDrawParaDlg 对话框

IMPLEMENT_DYNAMIC(CVDrawParaDlg, CDialog)

CVDrawParaDlg::CVDrawParaDlg()
	: CDialog(IDD_DIALOG_VDRAWPARA)
	, m_HoriScale(1000)
	, m_VerScale(100)
	, m_IsDrawCurveInf(TRUE)
	, m_IsDrawGrid(TRUE)
	,m_isDrawCtrHZCS(TRUE)
	, m_IsDrawFeaturePile(FALSE)
	, m_ZMBZTextH(5.0)
	, m_YbreakTextH(5.0)
{

}

CVDrawParaDlg::~CVDrawParaDlg()
{
}

void CVDrawParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_Hscale, m_HoriScale);
	DDV_MinMaxDouble(pDX, m_HoriScale, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_ZSCALE, m_VerScale);
	DDV_MinMaxDouble(pDX, m_VerScale, 1, 1000000);
	DDX_Check(pDX, IDC_CHECK_VDRAWPARA_ISDRAWCURVEINF, m_IsDrawCurveInf);
	DDX_Check(pDX, IDC_CHECK_DRAWWGX, m_IsDrawGrid);
	DDX_Check(pDX, IDC_CHECK_DRAWZDM_FEAPILE, m_IsDrawFeaturePile);
	DDX_Control(pDX, IDC_LIST_VDRAWPARA_WXP, m_wxp);
	DDX_Check(pDX, IDC_CHECK_DRAWCTRHZCS, m_isDrawCtrHZCS);
	DDX_Text(pDX, IDC_EDIT_ZMBZtextH, m_ZMBZTextH);
	DDX_Text(pDX, IDC_EDIT_YbreakH, m_YbreakTextH);
}

//更新数据
void CVDrawParaDlg::UpdateDrawPara(bool bSaveAndValidate)
{
	UpdateData(TRUE);
	

	if (bSaveAndValidate)
	{
		m_VDrawPara.Hscale = m_HoriScale;
		m_VDrawPara.Zscale = m_VerScale;
		m_VDrawPara.IsDrawFeaturePile = m_IsDrawFeaturePile;
		m_VDrawPara.infwgx = m_IsDrawGrid;
		m_VDrawPara.ifDrawCtrH = m_isDrawCtrHZCS;
		m_VDrawPara.IsDrawCurveInf = m_IsDrawCurveInf;
		m_VDrawPara.WXPArray = m_WXPArray;
		m_VDrawPara.DefTexhH = m_ZMBZTextH;
		m_VDrawPara.YbreakTextH = m_YbreakTextH;
	}
	else
	{
		m_HoriScale = m_VDrawPara.Hscale;
		m_VerScale = m_VDrawPara.Zscale;
		m_IsDrawFeaturePile = m_VDrawPara.IsDrawFeaturePile;
		m_IsDrawGrid = m_VDrawPara.infwgx;
		m_isDrawCtrHZCS= m_VDrawPara.ifDrawCtrH;
		m_IsDrawCurveInf = m_VDrawPara.IsDrawCurveInf;
		m_WXPArray = m_VDrawPara.WXPArray;
		m_ZMBZTextH=m_VDrawPara.DefTexhH ;
		m_YbreakTextH=m_VDrawPara.YbreakTextH;
		UpdateData(FALSE);
	}
}


BEGIN_MESSAGE_MAP(CVDrawParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CVDrawParaDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_VDRAWPARA_ADD, &CVDrawParaDlg::OnBnClickedButtonVdrawparaAdd)
	ON_BN_CLICKED(IDC_BUTTON_VDRAWPARA_DEL, &CVDrawParaDlg::OnBnClickedButtonVdrawparaDel)
END_MESSAGE_MAP()


// CVDrawParaDlg 消息处理程序



BOOL CVDrawParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	UpdateDrawPara(false);
	WriteWXPGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CVDrawParaDlg::WriteWXPGridCtrl()
{
	int i, j;
	ACHAR tmp[50];
	CRect rect;
	GetClientRect(rect);
	rect.left += 1;
	rect.top += 10;
	rect.right -= 1;
	rect.bottom -= 260;
	COLORREF txtbkclr = RGB(0XFF, 0XFF, 0XE0);
	COLORREF fixbkclr = RGB(155, 188, 0);

	int wxpnum = m_WXPArray.size();
	m_wxp.SetRowCount(wxpnum + 2);
	m_wxp.SetFixedRowCount(1);
	m_wxp.SetColumnCount(4);
	m_wxp.SetFixedColumnCount(1);
	m_wxp.SetTextBkColor(txtbkclr);
	m_wxp.SetFixedBkColor(fixbkclr);

	GV_ITEM Item;
	LOGFONT font = { 12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial" };
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	Item.lfFont = font;

	for (int k = 0; k < m_wxp.GetColumnCount(); k++)
	{
		for (i = 0; i < m_wxp.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_wxp.SetItem(&Item);
		}
	}
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;


	m_wxp.SetItemText(0, 0, L"序 号");
	m_wxp.SetItemText(0, 1, L"栏 目");
	m_wxp.SetItemText(0, 2, L"栏 高(mm)");
	m_wxp.SetItemText(0, 3, L"字 高(mm)");

	_tcscpy(tmp, L"第1栏");
	m_wxp.SetItemText(1, 0, tmp);

	CRect Rect;
	int width;

	m_wxp.GetClientRect(&Rect);
	width = Rect.Width();

	m_wxp.SetColumnWidth(0, int(54 * width / 350));
	m_wxp.SetColumnWidth(1, int(110 * width / 350));
	m_wxp.SetColumnWidth(2, int(85 * width / 350));
	m_wxp.SetColumnWidth(3, int(85 * width / 350));

	for (i = 0; i < wxpnum; i++)
	{
		//		ads_printf(L"%d %lf %lf\n",wxp[i].wxpinf,wxp[i].Lht,wxp[i].texth);
		NumberToString(m_WXPArray[i].wxpinf, tmp);
		m_wxp.SetItemText(i + 1, 1, tmp);
		_stprintf(tmp, L"%0.1lf", m_WXPArray[i].Lht);
		m_wxp.SetItemText(i + 1, 2, tmp);
		_stprintf(tmp, L"%0.1lf", m_WXPArray[i].texth);
		m_wxp.SetItemText(i + 1, 3, tmp);

		_stprintf(tmp, L"第%d栏", i + 1);
		m_wxp.SetItemText(i + 1, 0, tmp);
	}

	_stprintf(tmp, L"第%d栏", i + 1);
	m_wxp.SetItemText(i + 1, 0, tmp);
	m_wxp.SetRowHeight(0, DEFAULTCOLHEI);
	for (i = 1; i < m_wxp.GetRowCount(); i++)
		m_wxp.SetRowHeight(i, DEFAULTCOLHEI);

	m_wxp.ResetScrollBars();
	m_wxp.SetFocusCell(1, 1);

	UpdateData(FALSE);
	m_wxp.Invalidate();
}

void CVDrawParaDlg::ReadWXPGridCtrl()
{
	UpdateData(TRUE);

	int i;
	ACHAR tmp[30];
	int wxpnum = m_wxp.GetRowCount() - 2;
	if (wxpnum > 14)
		wxpnum = 14;
	m_WXPArray.clear();
	m_WXPArray.resize(wxpnum);
	for (i = 1; i <= wxpnum; i++)
	{
		m_WXPArray[i - 1].wxpinf = StringToNumber(m_wxp.GetItemText(i, 1));
		_tcscpy(tmp, m_wxp.GetItemText(i, 2));
		m_WXPArray[i - 1].Lht = _wtof(tmp);
		_tcscpy(tmp, m_wxp.GetItemText(i, 3));
		m_WXPArray[i - 1].texth = _wtof(tmp);
	}
}

void CVDrawParaDlg::NumberToString(int xz, ACHAR str[])
{
	switch (xz)
	{
	case 0:
		_tcscpy(str, L"0-地质概况");
		break;
	case 1:
		_tcscpy(str, L"1-设计高程");
		break;
	case 2:
		_tcscpy(str, L"2-地面高程");
		break;
	case 3:
		_tcscpy(str, L"3-坡度/坡长");
		break;
	case 4:
		_tcscpy(str, L"4-里程桩号");
		break;
	case 5:
		_tcscpy(str, L"5-直线及平曲线");
		break;
	case 6:
		_tcscpy(str, L"6-超高");
		break;
	case 7:
		_tcscpy(str, L"7-填挖高");
		break;
	case 8:
		_tcscpy(str, L"8-竖曲线");
		break;
	default:
		_tcscpy(str, L"0-地质概况");
		break;
	}
}

//
bool CVDrawParaDlg::CheckParaValidate()
{
	if (m_HoriScale < 1)
	{
		AfxMessageBox(L"横向比例尺设置错误！");
		return false;
	}
	if (m_VerScale < 1)
	{
		AfxMessageBox(L"纵向比例尺设置错误！");
		return false;
	}
	
	CString ErrorMess;
	for (int i = 0; i < m_WXPArray.size(); i++)
	{
		if (m_WXPArray[i].Lht < 0.0)
		{
			ErrorMess.Format(L"第%d行栏高设置错误，请检查！");
			AfxMessageBox(ErrorMess);
			return false;
		}

		if (m_WXPArray[i].texth < 0.0)
		{
			ErrorMess.Format(L"第%d行字高设置错误，请检查！");
			AfxMessageBox(ErrorMess);
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------
double CVDrawParaDlg::StringToNumber(CString str)
{
	int xz;

	if (str == "0-地质概况")
		xz = 0;
	else if (str == "1-设计高程")
		xz = 1;
	else if (str == "2-地面高程")
		xz = 2;
	else if (str == "3-坡度/坡长")
		xz = 3;
	else if (str == "4-里程桩号")
		xz = 4;
	else if (str == "5-直线及平曲线")
		xz = 5;
	else if (str == "6-超高")
		xz = 6;
	else if (str == "7-填挖高")
		xz = 7;
	else if (str == "8-竖曲线")
		xz = 8;
	else
		xz = 0;

	return xz;
}

void CVDrawParaDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadWXPGridCtrl();
	if (CheckParaValidate())
	{
		UpdateDrawPara(true);
		CDialog::OnOK();
	}
	else
		return;
}


void CVDrawParaDlg::OnBnClickedButtonVdrawparaAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	GV_ITEM Item;
	LOGFONT font = { 12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial" };
	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	Item.lfFont = font;

	int nRow = m_wxp.GetFocusCell().row;
	if (nRow >= 0)
	{
		Item.row = nRow;
		m_wxp.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_wxp.GetColumnCount(); i++)
		{
			Item.col = i;
			m_wxp.SetItem(&Item);
		}
		for (i = nRow; i < m_wxp.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"第%d栏", i);
			m_wxp.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_wxp.GetRowCount(); i++)
			m_wxp.SetRowHeight(i, 20);

		if (m_wxp.GetRowCount() > 3)
		{
			if (nRow == m_wxp.GetRowCount() - 2)
			{
				for (i = 1; i < m_wxp.GetColumnCount(); i++)
					m_wxp.SetItemText(nRow, i, m_wxp.GetItemText(nRow - 1, i));
			}
			else
				for (i = 1; i < m_wxp.GetColumnCount(); i++)
					m_wxp.SetItemText(nRow, i, m_wxp.GetItemText(nRow + 1, i));
		}
		m_wxp.SetFocusCell(nRow, 1);
	}

	UpdateData(FALSE);
	m_wxp.Invalidate();
}


void CVDrawParaDlg::OnBnClickedButtonVdrawparaDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_wxp.GetFocusCell().row;
	if (nRow == m_wxp.GetRowCount() - 1) return;
	if (nRow >= 0)
	{
		m_wxp.DeleteRow(nRow);
		int i;
		for (i = nRow; i < m_wxp.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"第%d栏", i);
			m_wxp.SetItemText(i, 0, str);
		}
		m_wxp.SetFocusCell(nRow, 1);
		m_wxp.Invalidate();
	}
}
