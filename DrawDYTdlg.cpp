// DrawDYTdlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawDYTdlg.h"
#include "afxdialogex.h"


// CDrawDYTdlg 对话框

IMPLEMENT_DYNAMIC(CDrawDYTdlg, CDialog)

CDrawDYTdlg::CDrawDYTdlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawDYTdlg::IDD, pParent)
{
	m_RoadName = L"主线";
	m_PTX = L"0";
	m_PTY = L"0";
	m_Dbs = L"";
	pm = NULL;
}

CDrawDYTdlg::~CDrawDYTdlg()
{
}

void CDrawDYTdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DLName, m_RoadNameCrl);
	DDX_CBString(pDX, IDC_COMBO_DLName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_X, m_PTX);
	DDX_Text(pDX, IDC_EDIT_Y, m_PTY);
	DDX_Text(pDX, IDC_EDIT_FaPath, m_Dbs);
}


BEGIN_MESSAGE_MAP(CDrawDYTdlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_LIULAN, &CDrawDYTdlg::OnBnClickedButtonLiulan)
	ON_BN_CLICKED(IDC_BUTTON_SelectXL, &CDrawDYTdlg::OnBnClickedButtonSelectxl)
	ON_BN_CLICKED(IDC_BUTTON_PickPT, &CDrawDYTdlg::OnBnClickedButtonPickpt)
	ON_CBN_SELCHANGE(IDC_COMBO_DLName, &CDrawDYTdlg::OnCbnSelchangeComboDlname)
	ON_BN_CLICKED(IDOK, &CDrawDYTdlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDrawDYTdlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDrawDYTdlg 消息处理程序
BOOL CDrawDYTdlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if (Pro == "")
	{
		CDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir + "\\DATA\\";
	m_Dbs = m_Dbs + Pro + ".mdb";

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


void CDrawDYTdlg::OnBnClickedButtonLiulan()
{
	// TODO:  在此添加控件通知处理程序代码
	// TODO: Add your control notification handler code here
	UpdateData(true);
	struct resbuf result;
	int rc = RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256];
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
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
	if (m_Dbs == ""){ AfxMessageBox(L"未指定数据库!"); return; }

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
	UpdateData(false);
}


void CDrawDYTdlg::OnBnClickedButtonSelectxl()
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
	return;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}


void CDrawDYTdlg::OnBnClickedButtonPickpt()
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
	int DFACTOR = 2;
	d1 = 100 * DFACTOR;
	h1 = 100 * DFACTOR;
	double Wtab = 3*d1;//表长
	double Htab = 3*h1;//表高

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
			ads_polar(RUpt, 0.5*PI, Htab, RDpt);
			ads_polar(LUpt, 0.5*PI, Htab, LDpt);

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


void CDrawDYTdlg::OnCbnSelchangeComboDlname()
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


void CDrawDYTdlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnOK();
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//绘制大样图
	DrawDYT();
	acDocManager->unlockDocument(acDocManager->curDocument());
}


void CDrawDYTdlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

//绘制大样图
void CDrawDYTdlg::DrawDYT()
{
	double OriX, OriY;//左上角点
	OriX = _wtof(m_PTX);
	OriY = _wtof(m_PTY);

	//OriX += 1000.0;
	ACHAR dwgPath[256];
	//插入图框
	//_tcscpy(dwgPath, m_FramePath);
	//double m_Frame_Height, m_Frame_Width;	//图框长宽参数
	//m_Frame_Height = 554.0;
	//m_Frame_Width = 780.0;
	//AcDbObjectId pBlockId;
	//AcDbObjectId pBlockId= BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"平面图框");
	//AcGePoint3d InsertPt;
	//InsertPt.set(OriX + m_Frame_Width / 2.0, OriY + m_Frame_Height / 2.0, 0.0);
	//if (pBlockId > 0)
	//{
	//	AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
	//	pBkRr->setColorIndex(7);
	//	BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
	//}

	//插入路基大样图

	_stprintf(dwgPath, L"%s\\lib\\路基大样图.dwg", SysPath);
	AcDbObjectId pBlockId = BAS_DRAW_FUN::CreateBlockFromDwgFile(dwgPath, L"路基大样图");
	AcGePoint3d InsertPt(OriX, OriY, 0.0);
	if (pBlockId > 0)
	{
		AcDbBlockReference* pBkRr = new AcDbBlockReference(InsertPt, pBlockId);
		pBkRr->setColorIndex(7);
		BAS_DRAW_FUN::AddEntityToDbs(pBkRr);
	}
}