// CCrossEarthInputDlg.cpp: 实现文件
//

#include "StdAfx.h"
#include "CCrossEarthInputDlg.h"
#include "afxdialogex.h"
#include "RoadDesignWorkingDatabase.h"
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;	//改移道路设计活动数据库

extern CString Cworkdir, Pro;
// CCrossEarthInputDlg 对话框

IMPLEMENT_DYNAMIC(CCrossEarthInputDlg, CDialog)

CCrossEarthInputDlg::CCrossEarthInputDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_CROSSEARTHINPUT, pParent)
	, m_LevelEidt(0)
	, m_ReferMode(0)
	, m_LevelMode(0)
{

}

CCrossEarthInputDlg::~CCrossEarthInputDlg()
{
}

void CCrossEarthInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CROEINPUT, m_ComboPile);
	DDX_Text(pDX, IDC_EDIT_CROSSLEVEL, m_LevelEidt);
	DDX_Control(pDX, IDC_LIST_CROSSEARTH_LEFT, m_EarGridCtrl_L);
	DDX_Control(pDX, IDC_LIST_CROSSEARTH_RIGHT, m_EarGridCtrl_R);
	DDX_Radio(pDX, IDC_RADIO_CROSSEARTH_BYPILE, m_ReferMode);
	DDX_Radio(pDX, IDC_RADIO_CROSSEARTH_DELTAH, m_LevelMode);
}


BEGIN_MESSAGE_MAP(CCrossEarthInputDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CCrossEarthInputDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CROEINPUT, &CCrossEarthInputDlg::OnCbnSelchangeComboCroeinput)
	ON_BN_CLICKED(IDC_RADIO_CROSSEARTH_BYPILE, &CCrossEarthInputDlg::OnBnClickedRadioCrossearthBypile)
	ON_BN_CLICKED(IDC_RADIO_CROSSEARTH_PREPT, &CCrossEarthInputDlg::OnBnClickedRadioCrossearthPrept)
	ON_BN_CLICKED(IDC_RADIO_CROSSEARTH_DELTAH, &CCrossEarthInputDlg::OnBnClickedRadioCrossearthDeltah)
	ON_BN_CLICKED(IDC_RADIO_CROSSEARTH_LEVEL, &CCrossEarthInputDlg::OnBnClickedRadioCrossearthLevel)
	ON_CBN_DROPDOWN(IDC_COMBO_CROEINPUT, &CCrossEarthInputDlg::OnCbnDropdownComboCroeinput)
	ON_BN_CLICKED(IDC_BUTTON_DRAWCROSSEARTH, &CCrossEarthInputDlg::OnBnClickedButtonDrawcrossearth)
	ON_BN_CLICKED(IDC_BUTTON_DRAWCROSSEARTH_ALL, &CCrossEarthInputDlg::OnBnClickedButtonDrawcrossearthAll)
	ON_BN_CLICKED(IDC_BUTTON_SAVECROSSEARTH, &CCrossEarthInputDlg::OnBnClickedButtonSavecrossearth)
	ON_BN_CLICKED(IDC_BUTTON_ADDROW_LEFT, &CCrossEarthInputDlg::OnBnClickedButtonAddrowLeft)
	ON_BN_CLICKED(IDC_BUTTON_DELROW_LEFT, &CCrossEarthInputDlg::OnBnClickedButtonDelrowLeft)
	ON_BN_CLICKED(IDC_BUTTON_ADDROW_RIGHT, &CCrossEarthInputDlg::OnBnClickedButtonAddrowRight)
	ON_BN_CLICKED(IDC_BUTTON_DELROW_RIGHT, &CCrossEarthInputDlg::OnBnClickedButtonDelrowRight)
END_MESSAGE_MAP()


// CCrossEarthInputDlg 消息处理程序


BOOL CCrossEarthInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//尝试读取横断面数据
	if (!ReadEarthLine())
	{
		AfxMessageBox(L"未读取到横断面地面线数据！\n请先使用【横断面设计】-【提取地面线】功能提取横断面地面线，再进行修改");
		CDialog::OnCancel();
		return FALSE;
	}
	
	//设置初始控件参数
	SetInitCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CCrossEarthInputDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteEarthLine();
	CDialog::OnOK();
}

bool CCrossEarthInputDlg::ReadEarthLine()
{
	FILE* fpr = NULL;
	int NCRS = 0;
	if(!BAS_DRAW_FUN::ReadWorkdir())
		return false;

	CString RoadName = L"主线";
	CString DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + RoadName;
	CString MdbName = BAS_DRAW_FUN::GetCurProMdbPath();
	JD_CENTER* pLine = g_RoadWorkingDatabase.CreateHorizontalRoad(MdbName, RoadName);
	if (!pLine)
		return false;

	fpr = _wfopen(DmxFname, L"r");
	if (fpr)
	{
		fwscanf(fpr, L"%d", &NCRS);

		if (NCRS <= 0)
		{
			return false;
		}

		m_CrossEarthLineArray.clear();
		m_CrossEarthLineArray.resize(NCRS);

		AcGePoint2d pt;
		for (int j = 0; j < NCRS; j++)
		{
			fwscanf(fpr, L"%lf%lf%d%d", &m_CrossEarthLineArray[j].Cml, &m_CrossEarthLineArray[j].EarthLevel, &m_CrossEarthLineArray[j].LeftEarthNum, &m_CrossEarthLineArray[j].RightEarthNum);
			m_CrossEarthLineArray[j].Ckml = pLine->Trs_ProjMl_To_Ckml(m_CrossEarthLineArray[j].Cml);
			m_CrossEarthLineArray[j].LeftPtArray.clear();
			int k;
			for (k = 0; k < m_CrossEarthLineArray[j].LeftEarthNum; k++)
			{
				fwscanf(fpr, L"%lf%lf", &pt.x, &pt.y);
				pt.y = pt.y + m_CrossEarthLineArray[j].EarthLevel;
				m_CrossEarthLineArray[j].LeftPtArray.push_back(pt);
			}
			m_CrossEarthLineArray[j].RightPtArray.clear();
			for (k = 0; k < m_CrossEarthLineArray[j].RightEarthNum; k++)
			{
				fwscanf(fpr, L"%lf%lf", &pt.x, &pt.y);
				pt.y = pt.y + m_CrossEarthLineArray[j].EarthLevel;
				m_CrossEarthLineArray[j].RightPtArray.push_back(pt);
			}
		}
		fclose(fpr);
		return true;
	}
	else
	{
		ads_printf(L"打开横断面地面线文件失败!\n");
		return false;
	}
		
}

void CCrossEarthInputDlg::WriteEarthLine()
{
	ReadData();

	CString RoadName = L"主线";
	CString DmxFname = Cworkdir + "\\data\\" + "hdmdmx." + Pro + "." + RoadName;
	CString MdbName = BAS_DRAW_FUN::GetCurProMdbPath();

	FILE* fpr = NULL;
	fpr = _wfopen(DmxFname, L"w");
	if (fpr)
	{
		fwprintf(fpr, L"%d\n", m_CrossEarthLineArray.size());

		AcGePoint2d pt;
		for (int j = 0; j < m_CrossEarthLineArray.size(); j++)
		{
			fwprintf(fpr, L"%lf %lf %d %d\n", m_CrossEarthLineArray[j].Cml, m_CrossEarthLineArray[j].EarthLevel,
				m_CrossEarthLineArray[j].LeftPtArray.size(), m_CrossEarthLineArray[j].RightPtArray.size());
			int k;
			for (k = 0; k < m_CrossEarthLineArray[j].LeftPtArray.size(); k++)
			{
				pt = m_CrossEarthLineArray[j].LeftPtArray[k];
				pt.y = pt.y - m_CrossEarthLineArray[j].EarthLevel;
				fwprintf(fpr, L"%lf %lf ", pt.x, pt.y);
			}
			fwprintf(fpr, L"\n");
			for (k = 0; k < m_CrossEarthLineArray[j].RightPtArray.size(); k++)
			{
				pt = m_CrossEarthLineArray[j].RightPtArray[k];
				pt.y = pt.y - m_CrossEarthLineArray[j].EarthLevel;
				fwprintf(fpr, L"%lf %lf ", pt.x, pt.y);
			}
			fwprintf(fpr, L"\n");
		}
		fclose(fpr);
		AfxMessageBox(L"保存记录成功！");
		return;
	}
	else
	{
		AfxMessageBox(L"打开横断面地面线文件失败!未能保存记录\n");
		return;
	}
}

void CCrossEarthInputDlg::SetInitCtrl()
{
	m_ComboPile.Clear();
	for (int i = 0; i < m_CrossEarthLineArray.size(); i++)
	{
		m_ComboPile.AddString(m_CrossEarthLineArray[i].Ckml);
	}
	m_ComboPile.SetCurSel(0);

	m_LevelEidt = m_CrossEarthLineArray[0].EarthLevel;

	m_ReferMode = 0;
	m_LevelMode = 0;

	m_EarGridCtrl_L.EnableScrollBar(SB_VERT);
	m_EarGridCtrl_R.EnableScrollBar(SB_VERT);

	UpdateData(FALSE);

	WriteGridCtrl(LEFT);
	WriteGridCtrl(RIGHT);
}

int CCrossEarthInputDlg::GetCurPileIndex()
{
	UpdateData(TRUE);

	int CurSel = m_ComboPile.GetCurSel();
	CString CurCkml = L"";
	if (CurSel != CB_ERR)
		m_ComboPile.GetLBText(CurSel, CurCkml);
	else
		return -1;

	for (int i = 0; i < m_CrossEarthLineArray.size(); i++)
	{
		if (m_CrossEarthLineArray[i].Ckml == CurCkml)
			return i;
	}

	return -1;
}

void CCrossEarthInputDlg::ReadData(bool IsXNormal, bool IsYNormal)
{
	UpdateData(TRUE);
	int CurIndex = GetCurPileIndex();
	if (CurIndex < 0 || CurIndex > m_CrossEarthLineArray.size())
		return;

	m_CrossEarthLineArray[CurIndex].EarthLevel = m_LevelEidt;

	ReadGridCtrl(LEFT, IsXNormal, IsYNormal);
	ReadGridCtrl(RIGHT, IsXNormal, IsYNormal);
}

void CCrossEarthInputDlg::WriteData()
{
	int CurIndex = GetCurPileIndex();
	if (CurIndex < 0 || CurIndex > m_CrossEarthLineArray.size())
		return;

	m_LevelEidt = m_CrossEarthLineArray[CurIndex].EarthLevel;

	UpdateData(FALSE);

	WriteGridCtrl(LEFT);
	WriteGridCtrl(RIGHT);
}

void CCrossEarthInputDlg::ReadGridCtrl(int LorR, bool IsXNormal, bool IsYNormal)
{
	UpdateData(TRUE);
	int CurIndex = GetCurPileIndex();
	if (CurIndex < 0 || CurIndex > m_CrossEarthLineArray.size())
		return;

	CGridCtrl& GridCtrl = (LorR == LEFT) ? m_EarGridCtrl_L : m_EarGridCtrl_R;
	CString tempCStr = L"";
	std::vector<AcGePoint2d> ptArray;
	ptArray.clear();
	AcGePoint2d tempPt(0.0, 0.0);

	int ReferMode_Read = IsXNormal ? m_ReferMode : ((m_ReferMode == 0) ? 1 : 0);
	int LevelMode_Read = IsYNormal ? m_LevelMode : ((m_LevelMode == 0) ? 1 : 0);


	for (int row = 1; row < GridCtrl.GetRowCount(); row++)
	{
		//获取X坐标
		tempCStr = GridCtrl.GetItemText(row, 1);
		if(tempCStr.IsEmpty())
			continue;
		double X = _wtof(tempCStr);
		if (ReferMode_Read == 0)	//相对中桩，不修改
		{
			tempPt.x = X;
		}
		else		//相对前点
		{
			if (row == 1)
				tempPt.x = X;
			else
			{
				tempPt.x = (ptArray.end() - 1)->x + X;
			}
		}

		//获取Y坐标
		tempCStr = GridCtrl.GetItemText(row, 2);
		if (tempCStr.IsEmpty())
			continue;
		double Y = _wtof(tempCStr);
		if (LevelMode_Read == 0)	//相对高差
		{
			tempPt.y = m_LevelEidt + Y;
		}
		else
		{
			tempPt.y = Y;
		}

		ptArray.push_back(tempPt);
	}

	//传入数据到后台数组中
	if (LorR == LEFT)
		m_CrossEarthLineArray[CurIndex].LeftPtArray = ptArray;
	else
		m_CrossEarthLineArray[CurIndex].RightPtArray = ptArray;
}

void CCrossEarthInputDlg::WriteGridCtrl(int LorR)
{
	UpdateData(TRUE);
	int CurIndex = GetCurPileIndex();

	if (CurIndex < 0 || CurIndex > m_CrossEarthLineArray.size())
		return;

	std::vector<AcGePoint2d>& ptArray = (LorR == LEFT) ?
		m_CrossEarthLineArray[CurIndex].LeftPtArray : m_CrossEarthLineArray[CurIndex].RightPtArray;
	CGridCtrl& GridCtrl = (LorR == LEFT) ? m_EarGridCtrl_L : m_EarGridCtrl_R;
	double MidLevel = m_CrossEarthLineArray[CurIndex].EarthLevel;

	//设置行和列
	GridCtrl.SetRowCount(ptArray.size() + 1);
	GridCtrl.SetFixedRowCount(1);
	GridCtrl.SetColumnCount(3);
	GridCtrl.SetFixedColumnCount(1);

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
	for (int k = 0; k < GridCtrl.GetColumnCount(); k++)
	{
		for (int i = 0; i < GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			GridCtrl.SetItem(&m_Item);
		}
	}

	//设置标题
	GridCtrl.SetItemText(0, 0, L"序号");
	GridCtrl.SetItemText(0, 1, L"距离");
	GridCtrl.SetItemText(0, 2, L"高程");

	//设置序列号
	CString tmp = L"";
	double HoriDis = 0.0, Level = 0.0;
	for (int i = 1; i < GridCtrl.GetRowCount(); i++)
	{
		tmp.Format(L"%d", i);
		GridCtrl.SetItemText(i, 0, tmp);

		//设置水平距离值
		if (m_ReferMode == 0)
			HoriDis = ptArray[i - 1].x;
		else
		{
			if (i == 1)
				HoriDis = 0;
			else
				HoriDis = ptArray[i - 1].x - ptArray[i - 2].x;
		}

		tmp.Format(L"%.3lf", HoriDis);
		GridCtrl.SetItemText(i, 1, tmp);

		//设置高程
		if (m_LevelMode == 0)
		{
			Level = ptArray[i - 1].y - MidLevel;
		}
		else
			Level = ptArray[i - 1].y;

		tmp.Format(L"%.3lf", Level);
		GridCtrl.SetItemText(i, 2, tmp);
	}

	GridCtrl.AutoSize();

	UpdateData(FALSE);
	GridCtrl.Invalidate();
}

double CCrossEarthInputDlg::DrawSingleCrossEarthLine(AcGePoint3d PtOri, int CrossIndex)
{
	//检测索引号是否正常
	if (CrossIndex < 0 || CrossIndex >= m_CrossEarthLineArray.size())
		return 0.0;

	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);

	//设置参数
	AcGePoint2dArray PtArray;
	AcGePoint2d tempPt(0.0, 0.0);
	AcGePoint3d TextPt(0.0, 0.0, 0.0);
	ACHAR LineText[256];
	double MaxLevel = 0.0;

	//计算绘制地面线，分左右侧
	PtArray.removeAll();
	
	for (int i = 0; i < m_CrossEarthLineArray[CrossIndex].LeftPtArray.size(); i++)
	{
		tempPt.x = PtOri.x - m_CrossEarthLineArray[CrossIndex].LeftPtArray[i].x;
		tempPt.y = PtOri.y + (m_CrossEarthLineArray[CrossIndex].LeftPtArray[i].y - m_CrossEarthLineArray[CrossIndex].EarthLevel);
		MaxLevel = max(m_CrossEarthLineArray[CrossIndex].LeftPtArray[i].y - m_CrossEarthLineArray[CrossIndex].EarthLevel, MaxLevel);
		PtArray.append(tempPt);

		//绘制小圆
		TextPt.set(tempPt.x, tempPt.y, 0.0);
		BAS_DRAW_FUN::makecircle(TextPt, 0.2, 5, L"横断面地面线示意图");

		//绘制高程信息
		TextPt.set(tempPt.x, tempPt.y - 2, 0.0);
		_stprintf(LineText, L"%.3lf", m_CrossEarthLineArray[CrossIndex].LeftPtArray[i].y);
		BAS_DRAW_FUN::maketext(TextPt, LineText, 0.0, 1, 0, 3, L"宋体", L"横断面地面线示意图");
	}
	BAS_DRAW_FUN::makepolyline(PtArray, 0, 0, L"横断面地面线示意图");

	PtArray.removeAll();
	for (int i = 0; i < m_CrossEarthLineArray[CrossIndex].RightPtArray.size(); i++)
	{
		tempPt.x = PtOri.x + m_CrossEarthLineArray[CrossIndex].RightPtArray[i].x;
		tempPt.y = PtOri.y + (m_CrossEarthLineArray[CrossIndex].RightPtArray[i].y - m_CrossEarthLineArray[CrossIndex].EarthLevel);
		MaxLevel = max(m_CrossEarthLineArray[CrossIndex].RightPtArray[i].y - m_CrossEarthLineArray[CrossIndex].EarthLevel, MaxLevel);
		PtArray.append(tempPt);

		//绘制小圆
		TextPt.set(tempPt.x, tempPt.y, 0.0);
		BAS_DRAW_FUN::makecircle(TextPt, 0.2, 5, L"横断面地面线示意图");

		//绘制高程信息
		TextPt.set(tempPt.x, tempPt.y - 2, 0.0);
		_stprintf(LineText, L"%.3lf", m_CrossEarthLineArray[CrossIndex].RightPtArray[i].y);
		BAS_DRAW_FUN::maketext(TextPt, LineText, 0.0, 1, 0, 3, L"宋体", L"横断面地面线示意图");
	}
	BAS_DRAW_FUN::makepolyline(PtArray, 0, 0, L"横断面地面线示意图");

	//绘制中心处竖线
	AcGePoint3d ptOriHei = PtOri;
	ptOriHei.y += 5;
	BAS_DRAW_FUN::makeline(PtOri, ptOriHei, 1, 0.0, L"横断面地面线示意图");

	//绘制里程信息
	PtOri.y -= 5;
	_tcscpy(LineText, m_CrossEarthLineArray[CrossIndex].Ckml);
	BAS_DRAW_FUN::maketext(PtOri, LineText, 0.0, 3, 1, 3, L"宋体", L"横断面地面线示意图");

	acDocManager->unlockDocument(acDocManager->curDocument());

	return MaxLevel;
}


void CCrossEarthInputDlg::OnCbnSelchangeComboCroeinput()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteData();
}


void CCrossEarthInputDlg::OnBnClickedRadioCrossearthBypile()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData(false, true);
	WriteData();
}


void CCrossEarthInputDlg::OnBnClickedRadioCrossearthPrept()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData(false, true);
	WriteData();
}


void CCrossEarthInputDlg::OnBnClickedRadioCrossearthDeltah()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData(true, false);
	WriteData();
}


void CCrossEarthInputDlg::OnBnClickedRadioCrossearthLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData(true, false);
	WriteData();
}

void CCrossEarthInputDlg::OnCbnDropdownComboCroeinput()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData();
}


void CCrossEarthInputDlg::OnBnClickedButtonDrawcrossearth()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData();

	ShowWindow(SW_HIDE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	ads_point resPt;

	if (acedGetPoint(NULL, L"请在屏幕上拾取一点!\n", resPt) == RTNORM)
	{
		AcGePoint3d ptOri(resPt[X], resPt[Y], resPt[Z]);
		DrawSingleCrossEarthLine(ptOri, GetCurPileIndex());
	}

	ShowWindow(SW_SHOW);
	SetFocus(); // Reset the focus back to ourselves
}


void CCrossEarthInputDlg::OnBnClickedButtonDrawcrossearthAll()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadData();

	ShowWindow(SW_HIDE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	ads_point resPt;

	if (acedGetPoint(NULL, L"请在屏幕上拾取一点!\n", resPt) == RTNORM)
	{
		//设置参数
		int CurNum = 0;
		double MaxLevel = 0.0;
		AcGePoint3d DrawOriPt(resPt[X], resPt[Y], resPt[Z]);

		for (int i = 0; i < m_CrossEarthLineArray.size(); i++)
		{
			if (CurNum >= 5)
			{
				DrawOriPt.x += 250.0;
				DrawOriPt.y = resPt[Y];
				CurNum = 0;
				MaxLevel = 0.0;
			}

			DrawOriPt.y = DrawOriPt.y + MaxLevel + 10.0;

			MaxLevel = DrawSingleCrossEarthLine(DrawOriPt, i);
			CurNum++;
		}
	}

	ShowWindow(SW_SHOW);
	SetFocus(); // Reset the focus back to ourselves
}


void CCrossEarthInputDlg::OnBnClickedButtonSavecrossearth()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteEarthLine();
}


void CCrossEarthInputDlg::OnBnClickedButtonAddrowLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_EarGridCtrl_L.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Item.row = nRow;
		m_EarGridCtrl_L.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_EarGridCtrl_L.GetColumnCount(); i++)
		{
			m_Item.col = i;
			m_EarGridCtrl_L.SetItem(&m_Item);
		}
		for (i = nRow; i < m_EarGridCtrl_L.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_EarGridCtrl_L.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_EarGridCtrl_L.GetRowCount(); i++)
			m_EarGridCtrl_L.SetRowHeight(i, 25);

		m_EarGridCtrl_L.SetFocusCell(nRow, 1);
	}

	UpdateData(FALSE);
	m_EarGridCtrl_L.Invalidate();
}


void CCrossEarthInputDlg::OnBnClickedButtonDelrowLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_EarGridCtrl_L.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_EarGridCtrl_L.DeleteRow(nRow);
		int i;
		for (i = nRow; i < m_EarGridCtrl_L.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_EarGridCtrl_L.SetItemText(i, 0, str);
		}
		m_EarGridCtrl_L.SetFocusCell(nRow, 1);
		m_EarGridCtrl_L.Invalidate();
	}
}


void CCrossEarthInputDlg::OnBnClickedButtonAddrowRight()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_EarGridCtrl_R.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Item.row = nRow;
		m_EarGridCtrl_R.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_EarGridCtrl_R.GetColumnCount(); i++)
		{
			m_Item.col = i;
			m_EarGridCtrl_R.SetItem(&m_Item);
		}
		for (i = nRow; i < m_EarGridCtrl_R.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_EarGridCtrl_R.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_EarGridCtrl_R.GetRowCount(); i++)
			m_EarGridCtrl_R.SetRowHeight(i, 25);

		m_EarGridCtrl_R.SetFocusCell(nRow, 1);
	}

	UpdateData(FALSE);
	m_EarGridCtrl_R.Invalidate();
}




void CCrossEarthInputDlg::OnBnClickedButtonDelrowRight()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_EarGridCtrl_R.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_EarGridCtrl_R.DeleteRow(nRow);
		int i;
		for (i = nRow; i < m_EarGridCtrl_R.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_EarGridCtrl_R.SetItemText(i, 0, str);
		}
		m_EarGridCtrl_R.SetFocusCell(nRow, 1);
		m_EarGridCtrl_R.Invalidate();
	}
}
