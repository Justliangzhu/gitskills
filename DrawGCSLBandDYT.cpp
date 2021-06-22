// DrawGCSLBandDYT.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawGCSLBandDYT.h"
#include "afxdialogex.h"
#include "CSetOutPileAttri.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]

// CDrawGCSLBandDYT 对话框

IMPLEMENT_DYNAMIC(CDrawGCSLBandDYT, CDialog)

CDrawGCSLBandDYT::CDrawGCSLBandDYT(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawGCSLBandDYT::IDD, pParent)
	, m_IsToExcel(FALSE)
	, m_PLTab(FALSE)
{
	m_RoadName = L"主线";
	m_PTX = L"0";
	m_PTY = L"0";
	m_Dbs = L"";
	pm = NULL;
	DFACTOR = 1;
}

CDrawGCSLBandDYT::~CDrawGCSLBandDYT()
{
}

void CDrawGCSLBandDYT::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DLName, m_RoadNameCrl);
	DDX_CBString(pDX, IDC_COMBO_DLName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_X, m_PTX);
	DDX_Text(pDX, IDC_EDIT_Y, m_PTY);
	DDX_Text(pDX, IDC_EDIT_FaPath, m_Dbs);
	DDX_Check(pDX, IDC_CHECK_QUANTITY_ISTOEXCEL, m_IsToExcel);
	DDX_Check(pDX, IDC_CHECK_PL, m_PLTab);
}


BEGIN_MESSAGE_MAP(CDrawGCSLBandDYT, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LIULAN, &CDrawGCSLBandDYT::OnBnClickedButtonLiulan)
	ON_BN_CLICKED(IDC_BUTTON_SelectXL, &CDrawGCSLBandDYT::OnBnClickedButtonSelectxl)
	ON_BN_CLICKED(IDC_BUTTON_PickPT, &CDrawGCSLBandDYT::OnBnClickedButtonPickpt)
	ON_CBN_SELCHANGE(IDC_COMBO_DLName, &CDrawGCSLBandDYT::OnCbnSelchangeComboDlname)
	ON_BN_CLICKED(IDOK, &CDrawGCSLBandDYT::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDrawGCSLBandDYT::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_SETOUTPILEATTRI, &CDrawGCSLBandDYT::OnBnClickedButtonSetoutpileattri)
	ON_BN_CLICKED(IDC_CHECK_PL, &CDrawGCSLBandDYT::OnBnClickedCheckPl)
END_MESSAGE_MAP()


// CDrawGCSLBandDYT 消息处理程序
BOOL CDrawGCSLBandDYT::OnInitDialog()
{
	//m_PLTab;
	CDialog::OnInitDialog();
	//20200322梁柱，初始化隐藏
	GetDlgItem(IDC_BUTTON_LIULAN)->ShowWindow(false);
	DFACTOR = 1;
	// TODO: Add extra initialization here
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if (Pro == "")
	{
		AfxMessageBox(L"未选择方案！无法输出工程数量表");
		CDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir + "\\DATA\\";
	m_Dbs = m_Dbs + Pro + ".mdb";
	//20200303梁柱
	PathArray.clear();
	PathArray.push_back(m_Dbs);
	DBS.Read_XLDbs(m_Dbs, L"线元表", L"主线");
	DBS.Read_XLDbs(m_Dbs, L"断链表", L"主线");
	if (DBS.XYNum > 0)
	{
		pm = new JD_CENTER(DBS.XYArray, DBS.XYNum);//主线对象
		if (DBS.NDL > 0)
			pm->setDLB(DBS.DLArray, DBS.NDL);
	}
	else
	{
		AfxMessageBox(L"方案无平面设计！无法输出工程数量表");
		CDialog::OnCancel();
		return 0;
	}
	_tcscpy(pm->mdbname, m_Dbs);
	_tcscpy(pm->RoadName, L"主线");
	DBS.m_RoadSum = 0;
	DBS.ReadAllRoadName(m_Dbs);
	for (int i = 0; i < DBS.m_RoadSum; i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDrawGCSLBandDYT::OnBnClickedButtonLiulan()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	UpdateData(true);
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
		
	if (m_PLTab == 0)
	{
		struct resbuf result;
		int rc = RTNORM;
		ACHAR fname[256];

		//////////////////////////////////////////////////////////////////////////
		ACHAR DefaultPath[256];
		_tcscpy(DefaultPath, Cworkdir);
		_tcscat(DefaultPath, L"\\DATA\\");
		_tcscat(DefaultPath, Pro);
		_tcscat(DefaultPath, L".mdb");
		//获到缺省图框名
		//////////////////////////////////////////////////////////////////////////


		if ((ads_getfiled(L"请输入数据库名 *.mdb", DefaultPath, L"mdb", 2 + 4, &result)) != RTNORM) return;
		if ((result.resval.rint == 1) && (result.restype == RTSHORT)) return;
		_tcscpy(fname, result.resval.rstring);
		m_Dbs = fname;
		if (m_Dbs == "") { AfxMessageBox(L"未指定数据库!"); return; }

		//	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");
		DBS.Read_XLDbs(m_Dbs, L"线元表", L"主线");
		DBS.Read_XLDbs(m_Dbs, L"断链表", L"主线");
		if (DBS.XYNum > 0)
		{
			if (pm)
				delete pm;
			pm = new JD_CENTER(DBS.XYArray, DBS.XYNum);//主线对象
			if (DBS.NDL > 0)
				pm->setDLB(DBS.DLArray, DBS.NDL);
		}
		_tcscpy(pm->mdbname, m_Dbs);
		_tcscpy(pm->RoadName, L"主线");
		DBS.m_RoadSum = 0;
		DBS.ReadAllRoadName(m_Dbs);
		int i;
		m_RoadNameCrl.ResetContent();
		for (i = 0; i < DBS.m_RoadSum; i++)
			m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
		m_RoadNameCrl.SetCurSel(0);
	}
	//20200303梁柱
	else
	{
		CString tempProPath = Cworkdir + L"\\data\\" + Pro + L".mdb";

		CFileDialog fname(true,NULL, tempProPath);
		fname.m_ofn.lpstrDefExt = L"mdb";
		fname.m_ofn.lpstrInitialDir = Cworkdir+"\\data\\";
		fname.m_ofn.Flags = OFN_LONGNAMES | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
		fname.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0";
		if (fname.DoModal()==IDOK)
		{
			PathArray.clear();
			CString Path;
			POSITION pos = fname.GetStartPosition();
			while (pos)
			{
				Path = fname.GetNextPathName(pos);
				PathArray.push_back(Path);
			}
			Path.Format(L"共计%d个方案：",PathArray.size());
			m_Dbs = Path;
			for (int i = 0; i < PathArray.size(); i++)
			{
				m_Dbs += PathArray.at(i);
				m_Dbs += L";";
			}
		}

	}

	UpdateData(false);
}


void CDrawGCSLBandDYT::OnBnClickedButtonSelectxl()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml = -100;
	rc = ads_entsel(L"\nSelect a  线路 : ", en, asDblArray(PT));
	if (rc != RTNORM)
	{
		ads_alert(L"所选实体非线路实体!");
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog
		return;
	}
	acdbGetObjectId(eId, en);

	AcDbObject *pObj = NULL;
	if (acdbOpenObject(pObj, eId, AcDb::kForRead) == Acad::eOk)
	{
		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{
			pObj->close();
			//acdbOpenObject(pm, eId, AcDb::kForRead);
			JD_CENTER *pm = JD_CENTER::cast(pObj);
			//pm->close();
			m_Dbs = pm->mdbname;
			m_RoadName = pm->RoadName;
		}
		else
		{
			pObj->close();

			DBS.GetXLXdata(eId, m_Dbs, m_RoadName);
			//		ads_printf(L"%s %s\n",m_Dbs,m_RoadName);
			if (m_Dbs != L""&&m_RoadName != L"")
			{
				DBS.Read_XLDbs(m_Dbs, L"线元表", m_RoadName);
				DBS.Read_XLDbs(m_Dbs, L"断链表", m_RoadName);
				if (DBS.XYNum > 0)
				{
					pm = new JD_CENTER(DBS.XYArray, DBS.XYNum);//主线对象
					if (DBS.NDL > 0)
						pm->setDLB(DBS.DLArray, DBS.NDL);
					_tcscpy(pm->mdbname, m_Dbs);
					_tcscpy(pm->RoadName, m_RoadName);
				}
			}
		}
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog
	UpdateData(false);
	return; 
	// return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}


void CDrawGCSLBandDYT::OnBnClickedButtonPickpt()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	UpdateData(true);
	double d1, d2, d3, d4, h1;
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
	d1 = 10 * DFACTOR;
	d2 = 20 * DFACTOR;
	d3 = 60 * DFACTOR;
	d4 = 30 * DFACTOR;
	h1 = 5 * DFACTOR;
	double Wtab = d1 + d2 + d3 + d2 + 2 * d4;//表长
	double Htab = 20 * h1;//表高

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

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
			m_PTX.Format(L"%0.3lf", pt[X]);
			m_PTY.Format(L"%0.3lf", pt[Y]);
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
			LUpt[X] = pt[X], LUpt[Y] = pt[Y];
			ads_polar(LUpt, 0.0, Wtab, RUpt);
			ads_polar(RUpt, 0.5*PI, -Htab, RDpt);
			ads_polar(LUpt, 0.5*PI, -Htab, LDpt);

			if (ptold[X] < 0.1)
			{
				ads_grdraw(LUpt, RUpt, -1, 0);
				ads_grdraw(RUpt, RDpt, -1, 0);
				ads_grdraw(RDpt, LDpt, -1, 0);
				ads_grdraw(LDpt, LUpt, -1, 0);
			}
			else if (caldistance(pt, ptold) > 0.1)
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

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/
	UpdateData(false);
}


void CDrawGCSLBandDYT::OnCbnSelchangeComboDlname()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i, m_RoadName);
	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",m_RoadName);
	DBS.Read_XLDbs(m_Dbs, L"线元表", m_RoadName);
	DBS.Read_XLDbs(m_Dbs, L"断链表", m_RoadName);
	if (DBS.XYNum > 0)
	{
		if (pm)
			delete pm;
		pm = new JD_CENTER(DBS.XYArray, DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if (DBS.NDL > 0)
			pm->setDLB(DBS.DLArray, DBS.NDL);
	}
	_tcscpy(pm->mdbname, m_Dbs);
	_tcscpy(pm->RoadName, m_RoadName);

	UpdateData(false);
}


void CDrawGCSLBandDYT::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代
	CDialog::OnOK();

}


void CDrawGCSLBandDYT::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void CDrawGCSLBandDYT::OnBnClickedButtonSetoutpileattri()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetOutPileAttri PileAttriDlg;
	PileAttriDlg.m_MdbName = m_Dbs;
	PileAttriDlg.m_RoadName = m_RoadName;
	PileAttriDlg.m_CrossTypeArray = m_CrossTypeArray;
	if (PileAttriDlg.DoModal() == IDOK)
		m_CrossTypeArray = PileAttriDlg.m_CrossTypeArray;
}


void CDrawGCSLBandDYT::OnBnClickedCheckPl()
{
	// TODO: 在此添加控件通知处理程序代码
	//20200322梁柱，隐藏浏览按钮
	if (m_PLTab==0)
	{
		m_PLTab = 1;
		GetDlgItem(IDC_BUTTON_LIULAN)->ShowWindow(true);
	}
	else
	{
		m_PLTab = 0;
		GetDlgItem(IDC_BUTTON_LIULAN)->ShowWindow(false);
	}
}
