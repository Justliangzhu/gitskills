// DrawZDMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawZDMDlg.h"
#include "BAS_DRAW_FUN.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DrawZDMDlg dialog


DrawZDMDlg::DrawZDMDlg()
	: CPropertyPage(DrawZDMDlg::IDD)
	, m_IsDrawFeaturePile(FALSE)
	, m_IsDrawCtrH(TRUE)
{
	//{{AFX_DATA_INIT(DrawZDMDlg)
	m_RoadName = L"主线";
	m_Hscale = L"1000";
	m_Zscale = L"100";
	m_CorX = L"0";
	m_CorY = L"0";
	m_Dbs = L"";
	m_SStCkml = L"AK0+000";
	m_SEdCkml = L"AK0+000";
	m_EdDml = 0.0;
	m_StDml = 0.0;
	m_SPage = L"1";
	m_bzym = FALSE;
	m_TKName = L"";
	m_infwgx = TRUE;	//20190710修改	默认绘制网格线
	m_dzgkname = L"";
	pm=NULL;
	//}}AFX_DATA_INIT
}


void DrawZDMDlg::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DrawZDMDlg)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCtrl);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_Hscale, m_Hscale);
	DDX_Text(pDX, IDC_EDIT_ZSCALE, m_Zscale);
	DDX_Text(pDX, IDC_EDIT_X, m_CorX);
	DDX_Text(pDX, IDC_EDIT_Y, m_CorY);
	DDX_Text(pDX, IDC_EDIT_MDB, m_Dbs);
	/*DDX_Text(pDX, IDC_EDIT_EDDML, m_EdDml);
	DDX_Text(pDX, IDC_EDIT_STDML, m_StDml);*/
	DDX_Text(pDX, IDC_EDIT_EDDML, m_SEdCkml);
	DDX_Text(pDX, IDC_EDIT_STDML, m_SStCkml);
	DDX_Text(pDX, IDC_EDIT_SPage, m_SPage);
	DDX_Check(pDX, IDC_CHECK_BZYM, m_bzym);
	DDX_Text(pDX, IDC_EDIT_TKNAME, m_TKName);
	DDX_Check(pDX, IDC_CHECK_DRAWWGX, m_infwgx);
	DDX_Text(pDX, IDC_EDIT_DZGKFNAME, m_dzgkname);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK_DRAWZDM_FEAPILE, m_IsDrawFeaturePile);
	DDX_Check(pDX, IDC_CHECK1, m_IsDrawCtrH);
}


BEGIN_MESSAGE_MAP(DrawZDMDlg, CPropertyPage)
	//{{AFX_MSG_MAP(DrawZDMDlg)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, OnButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_PickPT, OnBUTTONPickPT)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_RADIO_CBSJ, OnRadioCbsj)
	ON_BN_CLICKED(IDC_RADIO_SGTSJ, OnRadioSgtsj)
	ON_BN_CLICKED(IDC_BUTTON_SELTKFILE, OnButtonSeltkfile)
	ON_BN_CLICKED(IDC_CHECK_BZYM, OnCheckBzym)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_DRAWWGX, OnCheckDrawwgx)
	ON_BN_CLICKED(IDC_RADIO_FYSC, OnRadioFysc)
	ON_BN_CLICKED(IDC_RADIO_DZZFSC, OnRadioDzzfsc)
	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_QF, OnRadioQf)
	ON_BN_CLICKED(IDC_RADIO_ZF, OnRadioZf)
	ON_BN_CLICKED(IDC_RADIO_YF, OnRadioYf)
	ON_BN_CLICKED(IDC_BUTTON_DZGK, OnButtonDzgk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DrawZDMDlg message handlers

BOOL DrawZDMDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pbtn;
	DrawMode = 1;
	pm=NULL;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_SGTSJ);
	pbtn->SetCheck(1); 	
	GetDlgItem(IDC_EDIT_SPage)->EnableWindow(false);

	FForZF = 1;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DZZFSC);
	pbtn->SetCheck(1); 	

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CPropertyPage::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir+"\\DATA\\";
	m_Dbs = m_Dbs + Pro+".mdb";

	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");		
	DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
	if(DBS.NJdCen>0)
	{
		pm=new JD_CENTER();//主线对象
		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"主线");
	}
	else
	{
		DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");	
		if(DBS.XYNum>0)
		{
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pm->mdbname,m_Dbs);
			_tcscpy(pm->RoadName,L"主线");
		}
		else
		{
			CPropertyPage::OnCancel();
			return 0;
		}
	}
	zdmbx.mdbname = m_Dbs;
	zdmbx.RoadName = "主线";
	DBS.ReadAllRoadName(m_Dbs);
	int i;
	for( i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCtrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCtrl.SetCurSel(0);
	pzLinep = pm;
	DBS.ReadAllXZ(pm->mdbname,pm->RoadName);
	ZPInf = 0;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_ALL);
	pbtn->SetCheck(1); 	
	GetDlgItem(IDC_RADIO_ZF)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_YF)->EnableWindow(false);	
	if(DBS.RoadZ=="左幅")
		GetDlgItem(IDC_RADIO_ZF)->EnableWindow(true);
	if(DBS.RoadY=="右幅")
		GetDlgItem(IDC_RADIO_YF)->EnableWindow(true);		
	/*
	m_ZPXZCrl.AddString("全幅");
	if(DBS.RoadZ=="左幅")
	m_ZPXZCrl.AddString("左幅");
	if(DBS.RoadY=="右幅")
	m_ZPXZCrl.AddString("右幅");
	m_ZPXZCrl.SetCurSel(0);*/


	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);

	m_SStCkml=BAS_DRAW_FUN::LCchr(pm->DLArray[0].EGH, pm->DLArray[0].ELC,3);
	//m_SStCkml.Format(L"%s",);
	m_StDml = _wtof(str);
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_SEdCkml = BAS_DRAW_FUN::LCchr(pm->DLArray[pm->DLNum - 1].EGH, pm->DLArray[pm->DLNum - 1].ELC, 3);
	m_EdDml = _wtof(str);

	//设置五线谱默认值	20190710	把简图默认的地质概况去掉
	Init_WXP_Data(false);

	CFileFind Find;         //先定义类对象,才能用里面的涵数 
	CString tkname;
	tkname = Cworkdir+"\\DATA\\纵断面图框.dwg";
	if(Find.FindFile(tkname,0))
		m_TKName = tkname;
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DrawZDMDlg::OnButtonScan() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256],wordir[256],pro[256];
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	_tcscpy(DefaultPath,Cworkdir);
	_tcscat(DefaultPath,L"\\DATA\\");
	_tcscat(DefaultPath,Pro);
	_tcscat(DefaultPath,L".mdb");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////	

	if ( (ads_getfiled( L"请输入数据库名 *.mdb",DefaultPath,L"mdb",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_Dbs=fname;
	if(m_Dbs==""){AfxMessageBox(L"未指定数据库!");return;}

	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");		
	DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
	if(DBS.NJdCen>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER();//主线对象
		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"主线");
	}
	else
	{
		DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");	
		if(DBS.XYNum>0)
		{
			if(pm)
				delete pm;
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pm->mdbname,m_Dbs);
			_tcscpy(pm->RoadName,L"主线");
		}
	}
	pzLinep = pm;
	zdmbx.mdbname = m_Dbs;
	zdmbx.RoadName = "主线";
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_Dbs);
	int i;
	/*
	int num=m_RoadNameCtrl.GetCount();
	for(i=0;i<num;i++)
	m_RoadNameCtrl.DeleteString(i);*/
	m_RoadNameCtrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCtrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCtrl.SetCurSel(0);

	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_StDml = _wtof(str);
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_EdDml = _wtof(str);

	if (DrawMode == 1)
		Init_WXP_Data(false);
	else
		Init_WXP_Data(true);
	UpdateData(false);
	wxplm.gxinf=true;
	wxplm.Invalidate();
}

//20190710	统一在这里设置初始五线谱值
void DrawZDMDlg::Init_WXP_Data(bool Is_Prili_Des)
{
	if (!Is_Prili_Des)	//施工图
	{
		//设置五线谱默认值	20190710	把简图默认的地质概况和超高栏去掉
		wxplm.wxpnum = 5;
		for (int i = 0; i < wxplm.wxpnum; i++)
			wxplm.wxp[i].texth = 5.0;

		wxplm.wxp[0].wxpinf = 1, wxplm.wxp[1].wxpinf = 2, wxplm.wxp[2].wxpinf = 3;
		wxplm.wxp[3].wxpinf = 4; wxplm.wxp[4].wxpinf = 5;
		wxplm.wxp[0].Lht = 20.0, wxplm.wxp[1].Lht = 20.0, wxplm.wxp[2].Lht = 20.0;
		wxplm.wxp[3].Lht = 30.0, wxplm.wxp[4].Lht = 30.0;

		//原始默认值设置，留以备份
		//wxplm.wxpnum = 7;
		//for (int i = 0; i < wxplm.wxpnum; i++)
		//	wxplm.wxp[i].texth = 2.5;

		//wxplm.wxp[0].wxpinf = 0, wxplm.wxp[1].wxpinf = 1, wxplm.wxp[2].wxpinf = 2;
		//wxplm.wxp[3].wxpinf = 3; wxplm.wxp[4].wxpinf = 4; wxplm.wxp[5].wxpinf = 5;
		//wxplm.wxp[6].wxpinf = 6;
		//wxplm.wxp[0].Lht = 8.0, wxplm.wxp[1].Lht = 14.0, wxplm.wxp[2].Lht = 12.0;
		//wxplm.wxp[3].Lht = 10.0, wxplm.wxp[4].Lht = 12.0, wxplm.wxp[5].Lht = 16.0;
		//wxplm.wxp[6].Lht = 28.0;
	}
	else
	{
		//设置五线谱默认值	20190710	把简图默认的地质概况去掉
		wxplm.wxpnum = 3;
		for (int i = 0; i < wxplm.wxpnum; i++)
			wxplm.wxp[i].texth = 5.0;	//设置默认字高为6	20190710

		wxplm.wxp[0].wxpinf = 3, wxplm.wxp[1].wxpinf = 4, wxplm.wxp[2].wxpinf = 5;
		wxplm.wxp[0].Lht = 20.0, wxplm.wxp[1].Lht = 20.0, wxplm.wxp[2].Lht = 30.0;

		//原始默认值设置，留以备份
		//wxplm.wxpnum = 4;
		//for (int i = 0; i < wxplm.wxpnum; i++)
		//	wxplm.wxp[i].texth = 2.5;

		//wxplm.wxp[0].wxpinf = 0, wxplm.wxp[1].wxpinf = 3, wxplm.wxp[2].wxpinf = 4;
		//wxplm.wxp[3].wxpinf = 5;
		//wxplm.wxp[0].Lht = 10.0, wxplm.wxp[1].Lht = 10.0, wxplm.wxp[2].Lht = 10.0;
		//wxplm.wxp[3].Lht = 20.0;
	}
}

void DrawZDMDlg::OnBUTTONPickPT() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	ads_point resPt;
	GetParent()->GetParent()->EnableWindow(TRUE);
	GetParent()->ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->GetParent()->SetFocus();	// Give AutoCAD the focus.

	if(acedGetPoint(NULL,L"请在屏幕上拾取一点!\n",resPt)==RTNORM)
	{
		m_CorX.Format(L"%.2lf",resPt[X]);
		m_CorY.Format(L"%.2lf",resPt[Y]);
	}


	GetParent()->ShowWindow(SW_SHOW); // Display our dialog again
	GetParent()->SetFocus(); // Reset the focus back to ourselves
	GetParent()->GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	GetParent()->EnableWindow(TRUE); // Enable our dialog*/
	UpdateData(false);
}

void DrawZDMDlg::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int i = m_RoadNameCtrl.GetCurSel();
	m_RoadNameCtrl.GetLBText(i,m_RoadName);
	DBS.Read_XLDbs(m_Dbs,L"控制点表",m_RoadName);		
	DBS.Read_XLDbs(m_Dbs,L"断链表",m_RoadName);
	if(DBS.NJdCen>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER();//主线对象
		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,m_RoadName);
	}
	else
	{
		DBS.Read_XLDbs(m_Dbs,L"线元表",m_RoadName);	
		if(DBS.XYNum>0)
		{
			if(pm)
				delete pm;
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(pm->mdbname,m_Dbs);
			_tcscpy(pm->RoadName,m_RoadName);
		}
	}
	pzLinep = pm;
	zdmbx.mdbname = m_Dbs;
	zdmbx.RoadName = m_RoadName;

	DBS.ReadAllXZ(m_Dbs,m_RoadName);
	ZPInf = 0;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_ALL);
	pbtn->SetCheck(1); 	
	GetDlgItem(IDC_RADIO_ZF)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_YF)->EnableWindow(false);	
	if(DBS.RoadZ=="左幅")
		GetDlgItem(IDC_RADIO_ZF)->EnableWindow(true);
	if(DBS.RoadY=="右幅")
		GetDlgItem(IDC_RADIO_YF)->EnableWindow(true);		
	/*
	m_ZPXZCrl.AddString("全幅");
	if(DBS.RoadZ=="左幅")
	m_ZPXZCrl.AddString("左幅");
	if(DBS.RoadY=="右幅")
	m_ZPXZCrl.AddString("右幅");
	m_ZPXZCrl.SetCurSel(0);	*/


	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_StDml = _wtof(str);
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_EdDml = _wtof(str);
	UpdateData(false);
}

void DrawZDMDlg::OnRadioCbsj() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DrawMode =0;
	Init_WXP_Data(true);
	wxplm.gxinf=true;
}

void DrawZDMDlg::OnRadioSgtsj() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DrawMode =1;
	Init_WXP_Data(false);
	wxplm.gxinf=true;
}

void DrawZDMDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(wxplm.Ispaint)
		wxplm.UpdateWxp();
	CPropertyPage::OnOK();
}

void DrawZDMDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CPropertyPage::OnCancel();
}

/*
void DrawZDMDlg::OnSelchangeComboZpxz() 
{
// TODO: Add your control notification handler code here
UpdateData();
int i = m_ZPXZCrl.GetCurSel();
m_ZPXZCrl.GetLBText(i,m_ZPXZ);
DBS.Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,m_ZPXZ);
m_StDml = DBS.PDB[0].ml;
m_EdDml = DBS.PDB[DBS.NPD-1].ml;
UpdateData(false);
}*/


void DrawZDMDlg::OnButtonSeltkfile() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];
	ACHAR DefaultPath[256],wordir[256],pro[256];
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	_tcscpy(DefaultPath,Cworkdir);
	_tcscat(DefaultPath,L"\\DATA\\");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////	

	if ((ads_getfiled( L"请输入图框文件名 *.dwg",DefaultPath,L"dwg",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_TKName=fname;	
	UpdateData(false);
}

void DrawZDMDlg::OnCheckBzym() 
{
	// TODO: Add your control notification handler code here
	m_bzym = !m_bzym;
	UpdateData(false);
	if(m_bzym)
		GetDlgItem(IDC_EDIT_SPage)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_SPage)->EnableWindow(false);
}

void DrawZDMDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	if(wxplm.WayorRamp==0)
	{
		GetDlgItem(IDC_RADIO_CBSJ)->EnableWindow(true);
		GetDlgItem(IDC_RADIO_SGTSJ)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_RADIO_CBSJ)->EnableWindow(false);
		GetDlgItem(IDC_RADIO_SGTSJ)->EnableWindow(false);
	}
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void DrawZDMDlg::OnCheckDrawwgx() 
{
	// TODO: Add your control notification handler code here
	m_infwgx = !m_infwgx;
	UpdateData(false);
}

void DrawZDMDlg::OnRadioFysc() 
{
	// TODO: Add your control notification handler code here
	FForZF = 0;
}

void DrawZDMDlg::OnRadioDzzfsc() 
{
	// TODO: Add your control notification handler code here
	FForZF = 1;
}

void DrawZDMDlg::OnRadioAll() 
{
	// TODO: Add your control notification handler code here
	ZPInf = 0;
	DBS.Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,L"全幅");
	m_StDml = DBS.PDB[0].ml;
	m_EdDml = DBS.PDB[DBS.NPD-1].ml;
	UpdateData(false);	
}

void DrawZDMDlg::OnRadioQf() 
{
	// TODO: Add your control notification handler code here
	ZPInf = 1;
	DBS.Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,L"全幅");
	m_StDml = DBS.PDB[0].ml;
	m_EdDml = DBS.PDB[DBS.NPD-1].ml;
	UpdateData(false);	
}

void DrawZDMDlg::OnRadioZf() 
{
	// TODO: Add your control notification handler code here
	ZPInf = 2;
	DBS.Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,L"左幅");
	m_StDml = DBS.PDB[0].ml;
	m_EdDml = DBS.PDB[DBS.NPD-1].ml;
	UpdateData(false);		
}

void DrawZDMDlg::OnRadioYf() 
{
	// TODO: Add your control notification handler code here
	ZPInf = 3;
	DBS.Read_XLDbs(pm->mdbname,L"坡度表",pm->RoadName,L"右幅");
	m_StDml = DBS.PDB[0].ml;
	m_EdDml = DBS.PDB[DBS.NPD-1].ml;
	UpdateData(false);			
}

void DrawZDMDlg::OnButtonDzgk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];
	ACHAR DefaultPath[256],wordir[256],pro[256];
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	_tcscpy(DefaultPath,Cworkdir);
	_tcscat(DefaultPath,L"\\DATA\\");
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////	

	if ((ads_getfiled( L"请输入地质概况描述文件名 ",DefaultPath,L"*",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_dzgkname=fname;	
	UpdateData(false);	
}
