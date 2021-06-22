// CAutoDrawGutterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "CAutoDrawGutterDlg.h"
#include "afxdialogex.h"
#include "BAS_DRAW_FUN.h"
#include "RoadAutoDesign.h"
#include "RoadDesignWorkingDatabase.h"

// CAutoDrawGutterDlg 对话框
extern CString Cworkdir, Pro;
extern int isCT;//1表示只自动出图，不设计 0表示先自动设计，后出图
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;

IMPLEMENT_DYNAMIC(CAutoDrawGutterDlg, CDialog)

CAutoDrawGutterDlg::CAutoDrawGutterDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_AUTODRAWGUTTER, pParent)
	, m_ProPath(_T(""))
	, m_Coor_X(0)
	, m_Coor_Y(0)
	, m_FramePath(_T(""))
	, m_IsFromMdb(false)
	, m_GutterPath(_T(""))
	, m_GutterThick(0)
{

}

CAutoDrawGutterDlg::~CAutoDrawGutterDlg()
{
}

void CAutoDrawGutterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AUTODES_PATH, m_ProPath);
	DDX_Text(pDX, IDC_EDIT_AUTODES_XCOOR, m_Coor_X);
	DDX_Text(pDX, IDC_EDIT_AUTODES_YCOOR, m_Coor_Y);
	DDX_Text(pDX, IDC_EDIT4, m_FramePath);
	DDX_Text(pDX, IDC_EDIT_AUTODRAW_GUTTERTHICK, m_GutterThick);
	DDV_MinMaxDouble(pDX, m_GutterThick, 0.0, 10000.0);
	DDX_Text(pDX, IDC_EDIT_DRAWGUT_GUTPATH, m_GutterPath);
}


BEGIN_MESSAGE_MAP(CAutoDrawGutterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_BRO, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesBro)
	ON_BN_CLICKED(IDOK, &CAutoDrawGutterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_PICK, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesPick)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_BROFRAME, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesBroframe)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_BRRGUTTER, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesBrrgutter)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_HDRAWPARA, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesHdrawpara)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_VDRAWPARA, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesVdrawpara)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_SETCROSSPARA, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesSetcrosspara)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_OVALPARA, &CAutoDrawGutterDlg::OnBnClickedButtonAutodesOvalpara)
END_MESSAGE_MAP()


// CAutoDrawGutterDlg 消息处理程序
void CAutoDrawGutterDlg::OnBnClickedButtonAutodesBro()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString curpm = m_ProPath;
	CFileDialog fname(true, NULL, curpm);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER;
	fname.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0";

	if (fname.DoModal() == IDOK)
	{
		m_ProNo = fname.GetFileName();
		m_ProNo.MakeUpper();
		m_ProNo.Replace(L".MDB", NULL);

		m_ProPath = fname.GetPathName();
		CString str = fname.GetPathName();
		str.MakeUpper();

		CString str1 = L"\\data\\" + m_ProNo + L".mdb";
		str1.MakeUpper();
		str.Replace(str1, NULL);
		m_dirstr = str;

		//向workdir写项目路径
		ACHAR wfname[1024];
		FILE* fp = NULL;
		CString  tmpname;

		_stprintf(wfname, L"%s\\workdir", SysPath);

		if ((fp = _wfopen(wfname, L"w")) != NULL)
		{
			tmpname = m_dirstr;
			NullInStrto$(tmpname);
			fwprintf(fp, L"%s", tmpname);
			fclose(fp);
		}

		str = L"当前工程项目:" + m_dirstr + L"  [方案号: " + m_ProNo + L"]";
		acedGetAcadFrame()->SetWindowText(str);

		UpdateData(false);
	}
}

BOOL CAutoDrawGutterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	BAS_DRAW_FUN Bas_Fun;
	Bas_Fun.ReadWorkdir();

	m_ProPath = Cworkdir + L"\\data\\" + Pro + L".mdb";

	if (isCT == 1)//只出图，不设计
	{
		GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(FALSE);
	}

	//读取平面绘图参数
	if (!g_RoadWorkingDatabase.GetHorizontalDarwPara(m_ProPath, L"主线", m_HDrawPara))
	{
		AfxMessageBox(L"读取平面绘图参数失败，请检查arx放置位置是否正确！");
	}

	//设置纵面绘图参数
	SetProfilePara();

	UpdateData(FALSE);

	return TRUE;
}

void CAutoDrawGutterDlg::NullInStrto$(CString& text)
{
	int len, i;

	len = text.GetLength();
	i = text.Find(L" ");

	while (i < len && i >= 0)
	{
		text.SetAt(i, '$');
		i = text.Find(L" ");
	}
	if (len == 0)
		text = "$";
}

void CAutoDrawGutterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);

	CDialog::OnOK();

	RoadAutoDesign AuDes;
	AuDes.SetMdbName(m_ProPath);
	AuDes.SetRoadName(L"主线");
	AuDes.SetFramePath(m_FramePath);
	AuDes.SetDrawOriCoor(m_Coor_X, m_Coor_Y);
	AuDes.SetDataSource(m_IsFromMdb);
	AuDes.SetGutterPath(m_GutterPath);
	AuDes.SetGutterThick(m_GutterThick);
	AuDes.SetHorizontalDrawPara(m_HDrawPara);
	AuDes.SetProfileDrawPara(m_VDrawPara);
	AuDes.SetCrossDrawPara(m_CrossDrawPara);
	AuDes.AutoDrawGutter();

}


void CAutoDrawGutterDlg::OnBnClickedButtonAutodesPick()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	UpdateData(true);
	AcGePoint3d PT;
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	if (acedGetPoint(NULL, L"请在图上空白处(务必远离地形图)采集一点:_\n", resPt) == RTNORM)
	{
		m_Coor_X = resPt[X];
		m_Coor_Y = resPt[Y];
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	UpdateData(false);
}


void CAutoDrawGutterDlg::OnBnClickedButtonAutodesBroframe()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString curpm = m_ProPath;
	CFileDialog fname(true, NULL, curpm);
	fname.m_ofn.lpstrDefExt = L"dwg";
	fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER;
	fname.m_ofn.lpstrFilter = L"CAD图形文件(*.dwg)\0*.dwg\0";

	if (fname.DoModal() == IDOK)
	{
		m_FramePath = fname.GetPathName();
		UpdateData(false);
	}
}

void CAutoDrawGutterDlg::OnBnClickedButtonAutodesBrrgutter()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CString curpm = m_GutterPath;
	CFileDialog fname(true, NULL, curpm);
	fname.m_ofn.lpstrDefExt = L"dwg";
	fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER;
	fname.m_ofn.lpstrFilter = L"CAD图形文件(*.dwg)\0*.dwg\0";

	if (fname.DoModal() == IDOK)
	{
		m_GutterPath = fname.GetPathName();
		UpdateData(false);
	}
}


void CAutoDrawGutterDlg::OnBnClickedButtonAutodesHdrawpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_HDrawParaDlg.m_DrawPara = m_HDrawPara;

	if (m_HDrawParaDlg.DoModal() == IDOK)
		m_HDrawPara = m_HDrawParaDlg.m_DrawPara;
	SetFocus();
}


void CAutoDrawGutterDlg::SetProfilePara()
{
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_ProPath, L"主线");
	if (!pRoad)
	{
		AfxMessageBox(L"设置纵断面绘图参数时无法获取平面信息，请确认是否有平面方案！");
		return;
	}

	//设置绘图参数
	m_VDrawPara.DrawMode = 1;
	m_VDrawPara.FForZF = 0;
	m_VDrawPara.ZPInf = 0;
	m_VDrawPara.Hscale = 2000.0;
	m_VDrawPara.Zscale = 200.0;
	m_VDrawPara.StDml = pRoad->DLArray[0].ELC;
	m_VDrawPara.EdDml = pRoad->DLArray[pRoad->DLNum - 1].BLC;
	m_VDrawPara.SPage = 0;
	m_VDrawPara.bzym = false;
	m_VDrawPara.infwgx = true;
	m_VDrawPara.dzgkname = L"";	//暂时先不考虑地质描述文件

	//设置五线谱参数
	m_VDrawPara.WXPArray.resize(4);
	for (int i = 0; i < m_VDrawPara.WXPArray.size(); i++)
		m_VDrawPara.WXPArray[i].texth = 5.0;

	m_VDrawPara.WXPArray[0].wxpinf = 1, m_VDrawPara.WXPArray[1].wxpinf = 2, m_VDrawPara.WXPArray[2].wxpinf = 7;
	m_VDrawPara.WXPArray[3].wxpinf = 3;
	m_VDrawPara.WXPArray[0].Lht = 20.0, m_VDrawPara.WXPArray[1].Lht = 20.0, m_VDrawPara.WXPArray[2].Lht = 20.0;
	m_VDrawPara.WXPArray[3].Lht = 20.0;
}

void CAutoDrawGutterDlg::OnBnClickedButtonAutodesVdrawpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VDrawParaDlg.m_VDrawPara = m_VDrawPara;

	if (m_VDrawParaDlg.DoModal() == IDOK)
		m_VDrawPara = m_VDrawParaDlg.m_VDrawPara;
	SetFocus();
}

void CAutoDrawGutterDlg::SetCrossPara()
{
	m_CrossDrawPara.EarthWidL = 5.0;
	m_CrossDrawPara.EarthWidR = 5.0;
	m_CrossDrawPara.IsDrawEarthPile = false;
	m_CrossDrawPara.IsDrawGrid = true;
	m_CrossDrawPara.Scale = 200.0;
}


void CAutoDrawGutterDlg::OnBnClickedButtonAutodesSetcrosspara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CrossDrawParaDlg.m_DrawPara = m_CrossDrawPara;

	if (m_CrossDrawParaDlg.DoModal() == IDOK)
		m_CrossDrawPara = m_CrossDrawParaDlg.m_DrawPara;
	SetFocus();
}


void CAutoDrawGutterDlg::OnBnClickedButtonAutodesOvalpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OvalDrawParaDlg.m_DrawOvalPara = m_OvalDrawPara;

	if (m_OvalDrawParaDlg.DoModal() == IDOK)
		m_OvalDrawPara = m_OvalDrawParaDlg.m_DrawOvalPara;
	SetFocus();
}
