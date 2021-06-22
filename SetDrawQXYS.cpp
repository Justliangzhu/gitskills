// SetDrawQXYS.cpp : implementation file
//

#include "stdafx.h"
#include "SetDrawQXYS.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern ads_point ZOOM_P1,ZOOM_P2,ZOOM_P3,ZOOM_P4;
/////////////////////////////////////////////////////////////////////////////
// SetDrawQXYS dialog
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]
//struct ptt_node{
//	ads_point pt0;
//	ads_point pt;
//	ads_point pt1;
//	struct ptt_node *next;
//};
//struct arc_lst{
//	ads_real R;
//	ads_real xo;
//	ads_real yo;
//};
//struct point_lk{
//	ads_point pt;
//	struct point_lk *next;
//};

struct brk_node *heaptr;
struct egpt_node *frontptr;

struct ptt_node *getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
struct resbuf *get_resbuf(int code,struct resbuf *p);
int INTERS(double *XX,double *YY,double X1,double Y1,double X2,
		   double Y2,double X3,double Y3,double X4,double Y4);
int getrbpoint(ads_point pt,struct resbuf *ebuf,int code);
struct ptt_node *get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
struct ptt_node *getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
struct ptt_node *getArcpt(struct resbuf *Entlist,ads_point p0, ads_point p1);
struct ptt_node *getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1);
struct point_lk *findroot(ads_point ps, ads_point pe, struct arc_lst *pxyr,ads_real bulge,ads_point ps1, ads_point ps2);
struct arc_lst *findxyr(ads_point ps,ads_point pe,ads_real bulge);
struct ptt_node *get_Arcpt(struct resbuf *Entlist,ads_point p0, ads_point p1);

SetDrawQXYS::SetDrawQXYS(CWnd* pParent /*=NULL*/)
	: CDialog(SetDrawQXYS::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetDrawQXYS)
	m_Angle = _T("0");
	m_scale = _T("2000");
	m_EndJD = _T("");
	m_StaJD = _T("");
	m_InputAng = _T("0");
	m_X = _T("0");
	m_Y = _T("0");
	m_Dbs = _T("");
	m_RoadName = _T("");
	tempSJD = -100, tempEJD = -100;
	//}}AFX_DATA_INIT
}


void SetDrawQXYS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetDrawQXYS)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_Control(pDX, IDC_COMBOANGLE, m_ZJANGLE);
	DDX_Control(pDX, IDC_COMBOSTJD, m_SjdCtrl);
	DDX_Control(pDX, IDC_COMBOEDJD, m_EjdCtl);
	DDX_CBString(pDX, IDC_COMBOANGLE, m_Angle);
	DDX_CBString(pDX, IDC_COMBOBLCSCALE, m_scale);
	DDX_CBString(pDX, IDC_COMBOEDJD, m_EndJD);
	DDX_CBString(pDX, IDC_COMBOSTJD, m_StaJD);
	DDX_Text(pDX, IDC_EDITANG, m_InputAng);
	DDX_Text(pDX, IDC_EDITX, m_X);
	DDX_Text(pDX, IDC_EDITY, m_Y);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_Dbs);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SetDrawQXYS, CDialog)
	//{{AFX_MSG_MAP(SetDrawQXYS)
	ON_BN_CLICKED(IDC_BUTTONSETWZ, OnButtonsetwz)
	ON_BN_CLICKED(IDC_BUTTONSETANG, OnButtonsetang)
	ON_BN_CLICKED(IDC_RADIOBCBSJ, OnRadiobcbsj)
	ON_BN_CLICKED(IDC_RADIOSGTSJ, OnRadiosgtsj)
	ON_CBN_SELCHANGE(IDC_COMBOSTJD, OnSelchangeCombostjd)
	ON_CBN_SELCHANGE(IDC_COMBOEDJD, OnSelchangeComboedjd)
	ON_CBN_SELCHANGE(IDC_COMBOANGLE, OnSelchangeComboangle)
	ON_BN_CLICKED(IDC_BUTTON_BROW, OnButtonBrow)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetDrawQXYS message handlers

BOOL SetDrawQXYS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//AfxMessageBox(L"1");

	// TODO: Add extra initialization here
	pm = NULL;
	int i;
	CString str;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir+"\\DATA\\";
	m_Dbs = m_Dbs + Pro+".mdb";

	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");
	//	DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");			
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
		CDialog::OnCancel();
		return 0;
	}
	DBS.ReadAllRoadName(m_Dbs);
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	m_RoadName = "主线";
	for(i=0;i<pm->JdCenNum;i++)
	{
		if (i==0)
		{
			str=L"QD";
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_SjdCtrl.AddString(str);
		}
		else if (i == pm->JdCenNum-1)
		{
			str = L"ZD";
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_SjdCtrl.AddString(str);
		}
		else
		{
			str.Format(L"%0.0lf", pm->JdCenArray[i].JdNo);
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_SjdCtrl.AddString(str);
		}
		
	}
	m_StaJD = L"QD";
	//m_StaJD.Format(L"%0.0lf",pm->JdCenArray[0].JdNo);
	m_SjdCtrl.SetCurSel(0);
	for(i=0;i<pm->JdCenNum;i++)
	{
		if (i == 0)
		{
			str = L"QD";
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_EjdCtl.AddString(str);
		}
		else if (i == pm->JdCenNum - 1)
		{
			str = L"ZD";
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_EjdCtl.AddString(str);
		}
		else
		{
			str.Format(L"%0.0lf", pm->JdCenArray[i].JdNo);
			//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
			m_EjdCtl.AddString(str);
		}
		/*str.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
		m_EjdCtl.AddString(str);
		m_EndJD.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);*/
	}
	m_EndJD = L"ZD";
	m_EjdCtl.SetCurSel(pm->JdCenNum-1);
	DrawMode = 0;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOBCBSJ);
	pbtn->SetCheck(1); 	
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetDrawQXYS::ptAddx(ads_point p1,double angx,double x,ads_point pt)
{
	ads_polar(p1,angx,x,pt);
}
void SetDrawQXYS::ptAddy(ads_point p1,double angx,double y,ads_point pt)
{
	ads_polar(p1,angx+0.5*PI,y,pt);
}

void SetDrawQXYS::OnButtonsetwz() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	double d1,d2,d3,h1,h2;
	struct resbuf result;
	int type;
	ads_point pt,ptold,tmpt,LDpt,LUpt,RDpt,RUpt,LDptold,LUptold,RDptold,RUptold;
	pt[X]=pt[Y]=pt[Z]=0;
	ptold[X]=ptold[Y]=ptold[Z]=0;
	LDpt[X]=LDpt[Y]=LDpt[Z]=0;
	LUpt[X]=LUpt[Y]=LUpt[Z]=0;
	RDpt[X]=RDpt[Y]=LUpt[Z]=0;
	LDptold[X]=LDptold[Y]=LDptold[Z]=0;
	LUptold[X]=LUptold[Y]=LUptold[Z]=0;
	RDptold[X]=RDptold[Y]=RDptold[Z]=0;
	RUptold[X]=RUptold[Y]=RUptold[Z]=0;

	DFACTOR = _wtof(m_scale) / 1000;
	JDn = _wtof(m_EndJD)-_wtof(m_StaJD)+1;
	d1 = 20*DFACTOR;
	d2 = 15*DFACTOR;
	d3 = 10*DFACTOR;
	h1 = 8*DFACTOR , h2 = 5*DFACTOR;
	if(DrawMode==0)
	{
		Wtab = d3+3*d1+7*d2;
		Htab = 2*h1+JDn*h2;
	}
	else
	{
		Wtab = d3+10*d1+7*d2;
		Htab = 2*h1+JDn*h2;		
	}

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	for(;;)
	{
		if(ads_grread(1,&type,&result)!=RTNORM)
		{ break ;}
		if(type == 3)
		{
			pt[X] = result.resval.rpoint[X];
			pt[Y] = result.resval.rpoint[Y];
			pt[Z] = result.resval.rpoint[Z];
			m_X.Format(L"%0.3lf",pt[X]);
			m_Y.Format(L"%0.3lf",pt[Y]);
			break;
		}
		if(type != 5 && type != 12)//||(type != 3 ))
		{ 
			ads_printf(L"\n必须用鼠标! ");         
		}
		if(type == 5)
		{
			pt[X] = result.resval.rpoint[X];
			pt[Y] = result.resval.rpoint[Y];
			pt[Z] = result.resval.rpoint[Z];
			ads_polar(pt,0.0,-3.0*DFACTOR,tmpt);
			ads_polar(tmpt,0.5*PI,9.0*DFACTOR,LUpt);
			ptAddx(LUpt , 0.0 , Wtab+6.0*DFACTOR , RUpt);
			ptAddy(RUpt , 0.0 , -Htab-10.0*DFACTOR , RDpt);
			ptAddy(LUpt , 0.0 , -Htab-10.0*DFACTOR , LDpt);
			if(ptold[X] < 0.1)
			{
				ads_grdraw(LUpt,RUpt,-1,0);	
				ads_grdraw(RUpt,RDpt,-1,0);	
				ads_grdraw(RDpt,LDpt,-1,0);	
				ads_grdraw(LDpt,LUpt,-1,0);	
			}
			else if(caldistance(pt,ptold)>0.1)
			{
				/*
				ads_grdraw(LUptold,RUptold,-1,0);	
				ads_grdraw(RUptold,RDptold,-1,0);	
				ads_grdraw(RDptold,LDptold,-1,0);	
				ads_grdraw(LDptold,LUptold,-1,0);	*/
				acedRedraw(NULL, 0);

				ads_grdraw(LUpt,RUpt,-1,0);	
				ads_grdraw(RUpt,RDpt,-1,0);	
				ads_grdraw(RDpt,LDpt,-1,0);	
				ads_grdraw(LDpt,LUpt,-1,0);	
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

void SetDrawQXYS::OnButtonsetang() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	ads_point pt;
	double Pang;
	UpdateData();
	pt[0]=_wtof(m_X);
	pt[1]=_wtof(m_Y);

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.


	ads_initget(1, L"");

	ads_getangle(pt,L"\n采点以输入角度",&Pang);
	m_InputAng.Format(L"%0.2lf",Pang*180/PI);

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	UpdateData(false);
}

void SetDrawQXYS::OnOK() 
{
	// TODO: Add extra validation here

	double d1,d2,d3,h1,h2;
	struct resbuf result;
	int type;
	ads_point pt,ptold,tmpt,LDpt,LUpt,RDpt,RUpt,LDptold,LUptold,RDptold,RUptold;
	if (pm)
	{
		tempSJD = pm->JdCenArray[0].JdNo;
		tempEJD = pm->JdCenArray[pm->JdCenNum-1].JdNo;
	}
	
	CDialog::OnOK();

	DFACTOR = _wtof(m_scale) / 1000;
	if (m_StaJD==L"QD")
	{
		SIP = tempSJD;
	}
	else
	{
		SIP = _wtoi(m_StaJD);
	}
	if (m_EndJD == L"ZD")
	{
		EIP = tempEJD;
	}
	else
	{
		EIP = _wtoi(m_EndJD);
	}
	JDn = EIP - SIP + 1;

	d1 = 20*DFACTOR;
	d2 = 15*DFACTOR;
	d3 = 10*DFACTOR;
	h1 = 8*DFACTOR , h2 = 5*DFACTOR;
	if(DrawMode==0)
	{
		Wtab = d3+3*d1+7*d2;
		Htab = 2*h1+JDn*h2;
	}
	else
	{
		Wtab = d3+10*d1+7*d2;
		Htab = 2*h1+JDn*h2;		
	}

	INPTX = _wtof(m_X);
	INPTY = _wtof(m_Y);
	Ppt[X] = INPTX , Ppt[Y]=INPTY , Ppt[Z]=0.0;
	ALPHA = _wtof(m_InputAng);
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	del_ent(Wtab,Htab);
	//生成直曲转角表	
	pm->CreateZQZJB();

	//再标曲线表;
	if(DrawMode==0)
		wr_cmf_c();
	else
		wr_cmf_s();

	acDocManager->unlockDocument(acDocManager->curDocument());
	if(pm->ZQZJB)
	{
		delete []pm->ZQZJB;
		pm->ZQZJB = NULL;
	}	
}

void SetDrawQXYS::LineLR(ads_point p1,ads_point p2,double angx,double dist,double lw,int color)
{
	ads_point pt1,pt2;

	ptAddx(p1,angx,dist,pt1);
	ptAddx(p2,angx,dist,pt2);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pt2,RTSTR,L"",RTNONE);

	AcGePoint3d ptb,pte;
	ptb.x = pt1[X];
	ptb.y = pt1[Y];
	ptb.z =0;
	pte.x = pt2[X];
	pte.y = pt2[Y];
	pte.z =0;
	makeline(ptb,pte,color,lw,L"曲线要素");

}
void SetDrawQXYS::LineUD(ads_point p1,ads_point p2,double angx,double dist,double lw)
{
	ads_point pt1,pt2;

	ptAddy(p1,angx,dist,pt1);
	ptAddy(p2,angx,dist,pt2);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pt2,RTSTR,L"",RTNONE);
	AcGePoint3d ptb,pte;
	ptb.x = pt1[X];
	ptb.y = pt1[Y];
	ptb.z =0;
	pte.x = pt2[X];
	pte.y = pt2[Y];
	pte.z =0;
	makeline(ptb,pte,2,lw,L"曲线要素");
}
//-----------------------------------------------------------------------------------------
void  SetDrawQXYS::MidPoint(ads_point pt,double w,double h,double angx,ads_point Mpt)
{
	ads_point p1,p2;

	ads_polar(pt,angx,w,p1);
	ads_polar(p1,angx+0.5*PI,h,p2);
	Mpt[X] = 0.5*(pt[X] + p2[X]);
	Mpt[Y] = 0.5*(pt[Y] + p2[Y]);
}
void SetDrawQXYS::wr_text(ads_point pt,double angx,double d,double wk,double ht, ACHAR *str)
{
	double l;
	ads_point ps;

	//	l = 2.0*DFACTOR;
	ptAddx(pt,angx,d,ps);
	//	ptAddy(ps,angx,wk,ps);
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,ps,RTREAL,ht,RTREAL,angx*180/PI,RTSTR,str,RTNONE);
	AcGePoint3d ptb;
	ptb.x = ps[X];
	ptb.y = ps[Y];
	ptb.z =0;
	maketext(ptb,str,angx,ht,2,3,L"宋体",L"曲线要素");
	//	acedCommandS(RTSTR,L"Text",RTPOINT,ps,RTREAL,ht,RTREAL,angx*180/PI,RTSTR,str,RTNONE);
}

//----------------------------------------------------------------
int SetDrawQXYS::wr_cmf_c()
{
	double ang,d1,d2,d3,h1,h2,lw,d,ht,di,angd,hh,wk;
	ads_point p1,p2,tmpt,pr1,pr2,pl1,pl2,pl3,pt,pj,p0,Mpt;
	int i,Dip,du;
	ACHAR str[20],ZjStr[30];
	ACHAR mm[4],ss[4];
	ads_point pt1,pt2;
	bool HasSInfor = false, HasEInfor = false, GoToEnd = false;
	if (tempSJD == SIP)
	{
		HasSInfor = true;
	}
	if (tempEJD == EIP )
	{
		HasEInfor = true;
	}

	ang = ALPHA*PI/180;
	pl1[X]=Ppt[X],pl1[Y]=Ppt[Y],pl1[Z]=Ppt[Z];
	d1 = 20*DFACTOR;
	d2 = 15*DFACTOR;
	d3 = 10*DFACTOR;
	h1 = 8*DFACTOR , h2 = 5*DFACTOR;
	Wtab = d3+3*d1+9*d2;
	Htab = 2*h1+JDn*h2;
	lw = 0.5*DFACTOR;
	ptAddx(pl1 , ang , Wtab , pr1);
	ptAddy(pr1 , ang , -Htab , pr2);
	ptAddy(pl1 , ang , -Htab , pl2);
	ptAddy(pl1 , ang , -h1 , pl3);

	AcGePoint2dArray Pt2dAy;

	Pt2dAy.append(AcGePoint2d(pl1[X],pl1[Y])); 
	Pt2dAy.append(AcGePoint2d(pr1[X],pr1[Y])); 
	Pt2dAy.append(AcGePoint2d(pr2[X],pr2[Y])); 
	Pt2dAy.append(AcGePoint2d(pl2[X],pl2[Y])); 
	Pt2dAy.append(AcGePoint2d(pl1[X],pl1[Y]));
	makepolyline(Pt2dAy,2,lw,L"曲线要素");						
	//   acedCommandS(RTSTR,L"Pline",RTPOINT,pl1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pr1,RTPOINT,pr2,RTPOINT,pl2,RTSTR,L"C",RTNONE);

	lw = 0.0;
	ptAddx(pl1 , ang , d3+d1 , p1);
	ptAddx(pl1 , ang , d3+3*d1 , p2);

	LineUD(p2,p1,ang,-h1,0);
	LineUD(pl1,pr1,ang,-2.0*h1,0);

	LineLR(pl1,pl2,ang,d3,0);
	LineLR(pl1,pl2,ang,d3+d1,0);
	LineLR(pl1,pl2,ang,d3+3*d1,0);
	LineLR(pl3,pl2,ang,d3+2*d1,0);

	i=1;
	d = d3 + 3*d1;
	while(i<10)
	{
		di = d+i*d2;
		LineLR(pl1,pl2,ang,di,0);
		i++;
	}

	i=1;
	d = 2*h1;
	while(i<JDn)
	{
		di = d + i*h2;
		LineUD(pl1,pr1,ang,-di,0);
		i++;
	}

	angd = ang;
	ht = 4.0*DFACTOR;
	wk = 0.5*(h2-ht);
	ptAddx(pl1 , ang , 0.5*Wtab , Mpt);
	ptAddy(Mpt , ang , 8*DFACTOR , Mpt);
	wr_text(pj, ang,0.0,wk,ht,str);
	wr_text(Mpt, angd,0.0,wk,ht,L"平  曲  线  要  素  表");
	//    acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,Mpt,RTREAL,ht,RTREAL,angd,RTSTR,L"平  曲  线  要  素  表",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ptAddx(pl1 , ang , 0.5*Wtab , Mpt);
	ptAddy(Mpt , ang , 2*DFACTOR , Mpt);
	ptAddx(Mpt , ang , -47.5*0.5*DFACTOR  , pt1);
	ptAddx(Mpt , ang , 47.5*0.5*DFACTOR , pt2);
	LineLR(pt1,pt2,0,0,0.6,7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.6,RTREAL,0.6,RTPOINT,pt2,RTSTR,L"",RTNONE);
	ptAddy(pt1 , ang , 1.5*DFACTOR , pt1);
	ptAddy(pt2 , ang , 1.5*DFACTOR , pt2);
	LineLR(pt1,pt2,0,0,0.0,7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.0,RTREAL,0.0,RTPOINT,pt2,RTSTR,L"",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,2,RTNONE);

	MidPoint(pl1,d3,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"JD");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"JD",RTNONE);

	ptAddx(pl1,ang,d3,tmpt);
	MidPoint(tmpt,d1,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"角桩桩号");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"角桩桩号",RTNONE);

	ptAddx(pl1,ang,d3+d1,tmpt);
	MidPoint(tmpt,2.0*d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"转   角");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"转   角",RTNONE);

	ptAddx(pl3,ang,d3+d1,tmpt);
	MidPoint(tmpt,d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"左");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"左",RTNONE);

	ptAddx(pl3,ang,d3+2.0*d1,tmpt);
	MidPoint(tmpt,d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"右");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"右",RTNONE);


	ptAddx(pl1,ang,d3+3*d1,p1);
	hh = h1+h1;
	MidPoint(p1,d2,-hh,ang,pt);
	
	wr_text(pt, angd,0.0,0,ht,L"R");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"R",RTNONE);

	ptAddx(p1,ang,d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"Ls1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls1",RTNONE);

	ptAddx(p1,ang,2*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"Ls2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls2",RTNONE);

	ptAddx(p1,ang,3*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"T1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T1",RTNONE);

	ptAddx(p1,ang,4*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,0,ht,L"T2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T2",RTNONE);

	ptAddx(p1,ang,5*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	/*if (HasSInfor|| HasEInfor)
	{
		wr_text(pt, angd, 0.0, 0, ht, L"L(X)");
	}
	else
	{
		wr_text(pt, angd, 0.0, 0, ht, L"L");
	}*/
	wr_text(pt, angd, 0.0, 0, ht, L"L");
	
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"L",RTNONE);


	ptAddx(p1,ang,6*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	/*if (HasSInfor || HasEInfor)
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E(Y)");
	}
	else
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E");
	}*/
	wr_text(pt, angd, 0.0, 0, ht, L"E");

	ptAddx(p1, ang, 7 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	/*if (HasSInfor || HasEInfor)
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E(Y)");
	}
	else
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E");
	}*/
	wr_text(pt, angd, 0.0, 0, ht, L"X");

	ptAddx(p1, ang, 8 * d2, tmpt);
	MidPoint(tmpt, d2, -hh, ang, pt);
	/*if (HasSInfor || HasEInfor)
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E(Y)");
	}
	else
	{
		wr_text(pt, angd, 0.0, 0, ht, L"E");
	}*/
	wr_text(pt, angd, 0.0, 0, ht, L"Y");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"E",RTNONE);
	//--------------------------------------------------------------------------------------------
	ptAddx(p1,ang,9*d2,p1);
	i=0;
	ht = 2.0*DFACTOR;
	wk = 0.5*(h2-ht);

	ptAddy(pl1,ang,-2.0*h1,pl3);
	Dip = SIP-pm->JD0+1;
	while(i < JDn)
	{
		ptAddy(pl3,ang,-1.0*i*h2,p0);
		MidPoint(p0,d3,-h2,ang,pj);
		//		_stprintf(str,L"%d",CmfTab[i+Dip].JdNo);
		if (HasSInfor)
		{
			_stprintf(str, L"QD");
			HasSInfor = false;
		}
		else if (HasEInfor&&i == JDn - 1)
		{
			_stprintf(str, L"ZD");
			HasEInfor = false;
		}
		else
		{
			_stprintf(str, L"%s", pm->ZQZJB[i + Dip].JDNO);
		}
			
		wr_text(pj, ang,0.0,wk,ht,str);

		//		wr_text(pj, ang,0.5*(d3+d1),wk,ht,CmfTab[i+Dip].jdml);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].JDZH);
		wr_text(pj, ang,0.5*(d3+d1),wk,ht,str);
		
		atodmss(dms_rad(fabs(pm->ZQZJB[i+Dip].AFA)), du, mm,ss);
		GoToEnd = false;
		if (pm->ZQZJB[i + Dip].AFA ==0)
		{
			GoToEnd = true;
		}
		//125612
		if (!GoToEnd)
		{
			_stprintf(ZjStr, L"%d°%s%s", du, mm, ss);
			if (pm->ZQZJB[i + Dip].AFA < 0.0)
				wr_text(pj, ang, 0.5*d3 + 1.5*d1, wk, ht, ZjStr);
			else
				wr_text(pj, ang, 0.5*d3 + 2.5*d1, wk, ht, ZjStr);

			ads_rtos(pm->ZQZJB[i + Dip].R2, 2, 3, str);
			//		_stprintf(str,L"%0.3lf",pm->ZQZJB[i+Dip].R2);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 0.5*d2, wk, ht, str);

			ads_rtos(pm->ZQZJB[i + Dip].Ls1, 2, 3, str);
			//		_stprintf(str,L"%0.3lf",pm->ZQZJB[i+Dip].Ls1);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 1.5*d2, wk, ht, str);

			ads_rtos(pm->ZQZJB[i + Dip].Ls2, 2, 3, str);
			//		_stprintf(str,L"%0.3lf",pm->ZQZJB[i+Dip].Ls2);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 2.5*d2, wk, ht, str);

			ads_rtos(pm->ZQZJB[i + Dip].T1, 2, 3, str);
			//		_stprintf(str,L"%0.3lf",pm->ZQZJB[i+Dip].T1);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 3.5*d2, wk, ht, str);

			ads_rtos(pm->ZQZJB[i + Dip].T2, 2, 3, str);
			//		_stprintf(str,L"%0.3lf",pm->ZQZJB[i+Dip].T2);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 4.5*d2, wk, ht, str);


			
			ads_rtos(pm->ZQZJB[i + Dip].L, 2, 3, str);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 5.5*d2, wk, ht, str);

			ads_rtos(pm->ZQZJB[i + Dip].OutDist, 2, 3, str);
			wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 6.5*d2, wk, ht, str);
		}
		
		

		ads_rtos(pm->ZQZJB[i + Dip].JDN, 2, 3, str);
		wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 7.5*d2, wk, ht, str);

		ads_rtos(pm->ZQZJB[i + Dip].JDE, 2, 3, str);
		wr_text(pj, ang, 0.5*d3 + 3.0*d1 + 8.5*d2, wk, ht, str);


		
		i++;
	}

	return RTNORM;
}
int SetDrawQXYS::wr_cmf_s()
{
	double ang,d1,d2,d3,h1,h2,lw,d,ht,di,angd,hh,wk;
	ads_point p1,p2,tmpt,pr1,pr2,pl1,pl2,pl3,pt,pj,p0,Mpt;
	int i,Dip,du;
	ACHAR str[256],ZjStr[256];
	ACHAR mm[4],ss[4];
	ads_point pt1,pt2;
	bool HasSInfor = false, HasEInfor = false;
	if (tempSJD == SIP)
	{
		HasSInfor = true;
	}
	if (tempEJD == EIP)
	{
		HasEInfor = true;
	}

	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ang = ALPHA*PI/180;
	pl1[X]=Ppt[X],pl1[Y]=Ppt[Y],pl1[Z]=Ppt[Z];
	d1 = 20*DFACTOR;
	d2 = 15*DFACTOR;
	d3 = 10*DFACTOR;
	h1 = 8*DFACTOR , h2 = 5*DFACTOR;
	Wtab = d3+10*d1+7*d2;
	Htab = 2*h1+JDn*h2;
	lw = 0.5*DFACTOR;
	ptAddx(pl1 , ang , Wtab , pr1);
	ptAddy(pr1 , ang , -Htab , pr2);
	ptAddy(pl1 , ang , -Htab , pl2);
	ptAddy(pl1 , ang , -h1 , pl3);
	//	acedCommandS(RTSTR,L"color",RTSTR,L"Yellow",RTNONE);
	AcGePoint2dArray Pt2dAy;

	Pt2dAy.append(AcGePoint2d(pl1[X],pl1[Y])); 
	Pt2dAy.append(AcGePoint2d(pr1[X],pr1[Y])); 
	Pt2dAy.append(AcGePoint2d(pr2[X],pr2[Y])); 
	Pt2dAy.append(AcGePoint2d(pl2[X],pl2[Y])); 
	Pt2dAy.append(AcGePoint2d(pl1[X],pl1[Y]));
	makepolyline(Pt2dAy,2,lw,L"曲线要素");	
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pl1,RTSTR,L"W",RTREAL,lw,RTREAL,lw,RTPOINT,pr1,RTPOINT,pr2,RTPOINT,pl2,RTSTR,L"C",RTNONE);

	lw = 0.0;
	ptAddx(pl1 , ang , d3+5*d1 , p1);
	ptAddx(pl1 , ang , d3+3*d1 , p2);
	LineUD(p2,p1,ang,-h1,0);
	LineUD(pl1,pr1,ang,-2.0*h1,0);

	LineLR(pl1,pl2,ang,d3,0);
	LineLR(pl1,pl2,ang,d3+d1,0);
	LineLR(pl1,pl2,ang,d3+2*d1,0);
	LineLR(pl1,pl2,ang,d3+3*d1,0);
	LineLR(pl3,pl2,ang,d3+4*d1,0);
	LineLR(pl1,pl2,ang,d3+5*d1,0);

	i=1;
	d = d3 + 5*d1;
	while(i<8)
	{
		di = d+i*d2;
		LineLR(pl1,pl2,ang,di,0);
		i++;
	}

	i=1;
	d = d3 + 5*d1 + 7*d2;
	while(i<5)
	{
		di = d+i*d1;
		LineLR(pl1,pl2,ang,di,0);
		i++;
	}

	i=1;
	d = 2*h1;
	while(i<JDn)
	{
		di = d + i*h2;
		LineUD(pl1,pr1,ang,-di,0);
		i++;
	}


	angd = ang;
	ht = 4.0*DFACTOR;
	//    acedCommandS(RTSTR,L"-Style",RTSTR,L"hz",RTSTR,L"宋体",RTREAL,0.0,RTREAL,0.75,RTREAL,0.0,RTSTR,L"N",RTSTR,L"N",RTNONE);

	ptAddx(pl1 , ang , 0.5*Wtab , Mpt);
	ptAddy(Mpt , ang , 8*DFACTOR , Mpt);

	wk = 0.0;	//20190716	暂时没用上
	wr_text(Mpt, angd,0.0,wk,ht,L"平  曲  线  要  素  表");
	//    acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,Mpt,RTREAL,ht,RTREAL,angd,RTSTR,L"平  曲  线  要  素  表",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,7,RTNONE);
	ptAddx(pl1 , ang , 0.5*Wtab , Mpt);
	ptAddy(Mpt , ang , 2*DFACTOR , Mpt);
	ptAddx(Mpt , ang , -47.5*0.5*DFACTOR , pt1);
	ptAddx(Mpt , ang , 47.5*0.5*DFACTOR , pt2);
	LineLR(pt1,pt2,0,0,0.6,7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.6,RTREAL,0.6,RTPOINT,pt2,RTSTR,L"",RTNONE);
	ptAddy(pt1 , ang , 1.5*DFACTOR , pt1);
	ptAddy(pt2 , ang , 1.5*DFACTOR , pt2);
	LineLR(pt1,pt2,0,0,0.0,7);
	//	acedCommandS(RTSTR,L"Pline",RTPOINT,pt1,RTSTR,L"W",RTREAL,0.0,RTREAL,0.0,RTPOINT,pt2,RTSTR,L"",RTNONE);
	//	acedCommandS(RTSTR,L"Color",RTSHORT,2,RTNONE);

	MidPoint(pl1,d3,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"JD");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"JD",RTNONE);

	ptAddx(pl1,ang,d3,tmpt);
	MidPoint(tmpt,d1,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"角桩桩号");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"角桩桩号",RTNONE);

	ptAddx(pl1,ang,d3+d1,tmpt);
	MidPoint(tmpt,d1,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"X");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"X",RTNONE);

	ptAddx(pl1,ang,d3+2.0*d1,tmpt);
	MidPoint(tmpt,d1,-2.0*h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"Y");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Y",RTNONE);

	ptAddx(pl1,ang,d3+3.0*d1,tmpt);
	MidPoint(tmpt,2.0*d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"转   角");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"转   角",RTNONE);

	ptAddx(pl3,ang,d3+3.0*d1,tmpt);
	MidPoint(tmpt,d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"左");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"左",RTNONE);

	ptAddx(pl3,ang,d3+4.0*d1,tmpt);
	MidPoint(tmpt,d1,-h1,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"右");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"右",RTNONE);


	ptAddx(pl1,ang,d3+5*d1,p1);
	hh = h1+h1;
	MidPoint(p1,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"R");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"R",RTNONE);

	ptAddx(p1,ang,d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"Ls1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls1",RTNONE);

	ptAddx(p1,ang,2*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"Ls2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"Ls2",RTNONE);

	ptAddx(p1,ang,3*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"T1");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T1",RTNONE);

	ptAddx(p1,ang,4*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"T2");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"T2",RTNONE);

	ptAddx(p1,ang,5*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd, 0.0, wk, ht, L"L");
	/*if (HasSInfor || HasEInfor)
	{
		wr_text(pt, angd, 0.0, wk, ht, L"L(X)");
	}
	else
	{
		wr_text(pt, angd, 0.0, wk, ht, L"L");
	}*/
	//wr_text(pt, angd,0.0,wk,ht,L"L");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"L",RTNONE);

	ptAddx(p1,ang,6*d2,tmpt);
	MidPoint(tmpt,d2,-hh,ang,pt);
	wr_text(pt, angd, 0.0, wk, ht, L"E");
	/*if (HasSInfor || HasEInfor)
	{
		wr_text(pt, angd, 0.0, wk, ht, L"E(Y)");
	}
	else
	{
		wr_text(pt, angd, 0.0, wk, ht, L"E");
	}*/
	//wr_text(pt, angd,0.0,wk,ht,L"E");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"E",RTNONE);
	//--------------------------------------------------------------------------------------------
	ptAddx(p1,ang,7*d2,tmpt);
	p1[X] = tmpt[X],p1[Y] = tmpt[Y];
	MidPoint(p1,d1,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"ZH");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"ZH",RTNONE);

	ptAddx(p1,ang,d1,tmpt);
	MidPoint(tmpt,d1,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"HY(ZY)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"HY(ZY)",RTNONE);

	ptAddx(p1,ang,2.0*d1,tmpt);
	MidPoint(tmpt,d1,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"QZ");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"QZ",RTNONE);

	ptAddx(p1,ang,3.0*d1,tmpt);
	MidPoint(tmpt,d1,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"YH(YZ)");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR,L"YH(YZ)",RTNONE);

	ptAddx(p1,ang,4.0*d1,tmpt);
	MidPoint(tmpt,d1,-hh,ang,pt);
	wr_text(pt, angd,0.0,wk,ht,L"HZ");
	//	acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pt,RTREAL,ht,RTREAL,angd,RTSTR, "HZ",RTNONE);

	i=0;
	ht = 2.0*DFACTOR;
	wk = 0.5*(h2-ht);
	ptAddy(pl1,ang,-2.0*h1,pl3);
	Dip = SIP-pm->JD0+1;
	while(i < JDn)
	{
		ptAddy(pl3,ang,-1.0*i*h2,p0);
		MidPoint(p0,d3,-h2,ang,pj);
		//		_stprintf(str,L"%d",CmfTab[i+Dip].JdNo);
		if (HasSInfor)
		{
			_stprintf(str, L"QD");
		}
		else if (HasEInfor&&i == JDn - 1)
		{
			_stprintf(str, L"ZD");
		}
		else
		{
			_stprintf(str, L"%s", pm->ZQZJB[i + Dip].JDNO);
		}
		//_stprintf(str,L"%s",pm->ZQZJB[i+Dip].JDNO);		
		wr_text(pj, ang,0.0,wk,ht,str);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].JDZH);
		wr_text(pj, ang,0.5*(d3+d1),wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].JDN,2,3,str);
		wr_text(pj, ang,0.5*d3+1.5*d1,wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].JDE,2,3,str);
		wr_text(pj, ang,0.5*d3+2.5*d1,wk,ht,str);

		if (HasSInfor)
		{
			HasSInfor=false;
			i++;
			continue;
		}
		else if (HasEInfor&&i == JDn - 1)
		{
			HasEInfor=false;
			i++;
			continue;
		}
		
		atodmss(dms_rad(fabs(pm->ZQZJB[i+Dip].AFA)), du, mm,ss);
		//125612
		_stprintf(ZjStr,L"%d°%s%s",du,mm,ss);
		//	_stprintf(ZjStr,L"%d.%s%s",du,mm,ss);
		if(pm->ZQZJB[i+Dip].AFA < 0.0)
			wr_text(pj, ang,0.5*d3+3.5*d1,wk,ht,ZjStr);
		else
			wr_text(pj, ang,0.5*d3+4.5*d1,wk,ht,ZjStr);

		ads_rtos(pm->ZQZJB[i+Dip].R2,2,3,str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].R);
		wr_text(pj, ang,0.5*d3+5.0*d1+0.5*d2,wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].Ls1,2,3,str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].Lo1);
		wr_text(pj, ang,0.5*d3+5.0*d1+1.5*d2,wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].Ls2,2,3,str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].Lo2);
		wr_text(pj, ang,0.5*d3+5.0*d1+2.5*d2,wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].T1,2,3,str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].T1);
		wr_text(pj, ang,0.5*d3+5.0*d1+3.5*d2,wk,ht,str);

		ads_rtos(pm->ZQZJB[i+Dip].T2,2,3,str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].T2);
		wr_text(pj, ang,0.5*d3+5.0*d1+4.5*d2,wk,ht,str);

		ads_rtos(pm->ZQZJB[i + Dip].L, 2, 3, str);
		//		_stprintf(str,L"%0.3lf",CmfTab[i+Dip].K);
		wr_text(pj, ang, 0.5*d3 + 5.0*d1 + 5.5*d2, wk, ht, str);
		
		if (pm->ZQZJB[i + Dip].OutDist > 0.0)
		{
			ads_rtos(pm->ZQZJB[i + Dip].OutDist, 2, 3, str);
			//          _stprintf(str,L"%0.3lf",CmfTab[i+Dip].E);
			wr_text(pj, ang, 0.5*d3 + 5.0*d1 + 6.5*d2, wk, ht, str);
		}
		
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].ZHZH);		
		wr_text(pj, ang,0.5*d3+5.5*d1+7.0*d2,wk,ht,str);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].HYZH);		
		wr_text(pj, ang,0.5*d3+6.5*d1+7.0*d2,wk,ht,str);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].YoZH);	
		wr_text(pj, ang,0.5*d3+7.5*d1+7.0*d2,wk,ht,str);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].YHZH);
		wr_text(pj, ang,0.5*d3+8.5*d1+7.0*d2,wk,ht,str);
		_stprintf(str,L"%s",pm->ZQZJB[i+Dip].HZZH);
		wr_text(pj, ang,0.5*d3+9.5*d1+7.0*d2,wk,ht,str);
		i++;
	}
	return RTNORM;
}
//-----------------------------------------------------------------
int SetDrawQXYS::del_ent(double Wtab,double Htab)
{
	double ang,h1;
	ads_point tmpt,pr1,pr2,pl1,pl2,pl3,LDpt,LUpt,RDpt,RUpt;
	h1 = Wtab;
	ang = ALPHA*PI/180;
	pl1[X]=Ppt[X],pl1[Y]=Ppt[Y],pl1[Z]=Ppt[Z];

	ptAddx(pl1 , ang , Wtab , pr1);
	ptAddy(pr1 , ang , -Htab , pr2);
	ptAddy(pl1 , ang , -Htab , pl2);
	ptAddy(pl1 , ang , -h1 , pl3);
	//acedCommandS(RTSTR,L"Text",RTSTR,L"J",RTSTR,L"M",RTPOINT,pl1,RTREAL,1.0,RTREAL,0.0,RTSTR,L"pl1",RTNONE);
	//pl1,pl2,pr1,pr2四点
	//切断
	ads_polar(pl1,ang,-3.0*DFACTOR,tmpt);
	ads_polar(tmpt,ang+0.5*PI,9.0*DFACTOR,LUpt);
	ptAddx(LUpt , ang , Wtab+6.0*DFACTOR , RUpt);
	ptAddy(RUpt , ang , -Htab-10.0*DFACTOR , RDpt);
	ptAddy(LUpt , ang , -Htab-10.0*DFACTOR , LDpt);

	BAS_DRAW_FUN::AutoEntBK(LUpt,RUpt,RDpt,LDpt);
	/*
	AutoEntBK1(LUpt,RUpt);
	AutoEntBK1(RUpt,RDpt);
	AutoEntBK1(RDpt,LDpt);
	AutoEntBK1(LDpt,LUpt);*/

	//筐选
	ads_polar(pl1,ang,-4.0*DFACTOR,tmpt);
	ads_polar(tmpt,ang+0.5*PI,10.0*DFACTOR,LUpt);
	ptAddx(LUpt , ang , Wtab+8.0*DFACTOR , RUpt);
	ptAddy(RUpt , ang , -Htab-12.0*DFACTOR , RDpt);
	ptAddy(LUpt , ang , -Htab-12.0*DFACTOR , LDpt);

#ifdef VERSION_CAD2016
	acedCommandS(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, LDpt, RTPOINT, LUpt, RTPOINT, RUpt, RTPOINT, RDpt, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
	ads_command(RTSTR, L"erase", RTSTR, L"WP", RTPOINT, LDpt, RTPOINT, LUpt, RTPOINT, RUpt, RTPOINT, RDpt, RTSTR, L"", RTSTR, L"", 0);
#endif // VERSION_CAD2010	
	

	return RTNORM;
}
//------------------------------------------------------
//        enttype= "F" or "C"
//------------------------------------------------------
int SetDrawQXYS::AutoEntBK(ads_point p0, ads_point p1)
{
	struct resbuf *e,*elst,*p,*hdptr,*tlptr;
	ads_name ssname,ename,last;
	struct ptt_node *pttab,*newptr;
	struct resbuf *pointlist;
	ads_point pt0,pt1,mpt;
	int i,fit_type;
	long n;
	ACHAR a[80];


	pointlist = ads_buildlist(RTPOINT,p0,RTPOINT,p1,0);
	//   acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
	ads_grdraw(p0,p1,-1,0);
	//   acedCommandS(RTSTR,L"line",RTPOINT,p0,RTPOINT,p1,RTSTR,L"",0);
	ads_ssget(L"F",pointlist,NULL,NULL,ssname);
	if(ads_sslength(ssname,&n)==RTNORM)
	{
		ads_printf(L"Number of entity is %ld\n",n);

L60:
		while(n > 0)
		{
			n = n - 1;

			if(ads_ssname(ssname,n,ename)==RTNORM)
			{
				e = ads_entget(ename);
			}
			else 
				goto L60;
			elst = get_resbuf(0,e);
			/*	   if(!elst)
			return RTERROR; */
			_tcscpy(a,elst->resval.rstring);
			//       ads_printf(L"a=%s\n",a);

			if(_tcscmp(a,L"LINE")==0)
			{
				getrbpoint(pt0,e,10);
				getrbpoint(pt1,e,11);
				//		   i = ads_inters(p0,p1,pt0,pt1,1,mpt);
				i = INTERS(&mpt[X],&mpt[Y],p0[X],p0[Y],p1[X],p1[Y],pt0[X],pt0[Y],pt1[X],pt1[Y]);		     
				mpt[Z] = pt0[Z];
				if (i == 0 && (fabs(mpt[X] - pt0[X]) > 0.001 || fabs(mpt[Y] - pt0[Y]) > 0.001 || fabs(mpt[Z] - pt0[Z]) > 0.001)
					&& (fabs(mpt[X] - pt1[X]) > 0.001 || fabs(mpt[Y] - pt1[Y]) > 0.001 || fabs(mpt[Z] - pt1[Z]) > 0.001))
				{
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2010	
				}
			}
			if(_tcscmp(a,L"LWPOLYLINE")==0)
			{

				elst = get_resbuf(70,e);
				fit_type = elst->resval.rint;
				if(fit_type == 1 || fit_type == 129)
				{
					elst->resval.rint = 0;
					elst = get_resbuf(10,e);
					if(elst)
					{
						mpt[X] = elst->resval.rpoint[X];
						mpt[Y] = elst->resval.rpoint[Y];			 
					}
					p = ads_newrb(RTPOINT);
					p->restype = 10;
					p->resval.rpoint[X] = mpt[X];
					p->resval.rpoint[Y] = mpt[Y];
					hdptr = p;
					tlptr = p;
					p = ads_newrb(RTREAL);
					p->restype = 40;
					p->resval.rreal = 0.0;
					tlptr->rbnext = p;
					tlptr = p;
					p = ads_newrb(RTREAL);
					p->restype = 41;
					p->resval.rreal = 0.0;
					tlptr->rbnext = p;
					tlptr = p;
					p = ads_newrb(RTREAL);
					p->restype = 42;
					p->resval.rreal = 0.0;
					tlptr->rbnext = p;
					tlptr = p;
					p = e;
					while(p->rbnext->restype != 210)
						p = p->rbnext;
					tlptr->rbnext = p->rbnext;
					p->rbnext = hdptr;
					ads_entmod(e);			 

					/*			 p = hdptr;
					while(p != tlptr)
					{
					p = p->rbnext;
					ads_relrb(hdptr);
					hdptr = p;
					}
					ads_relrb(p); */
				}

				pttab = get_brkpt(e,p0,p1);
				newptr = pttab->next;
				if(newptr)
				{
					ads_point_set(newptr->pt,mpt);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
#endif // VERSION_CAD2010	
					

					pttab->next = newptr->next;
					free(newptr);
					newptr = pttab;
				}
				else
				{
					if(pttab) free(pttab);
					//			  ads_alert(L"s3");
					ads_relrb(e);
					//			  ads_alert(L"s4");
					ads_ssfree(ename);  
					goto L60;
				}
				while(newptr->next != NULL)
				{
					newptr = newptr->next;
					ads_point_set(newptr->pt,mpt);
					ads_entlast(last);
					//			ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2010	
					
					pttab->next = newptr->next;
					free(newptr);
					ads_ssfree(last);
					newptr = pttab;
				}
				if(pttab) free(pttab);
			}

			if(_tcscmp(a,L"POLYLINE")==0)
			{
				pttab = getbrkpt(e,p0,p1);
				newptr = pttab->next;
				if(newptr)
				{
					ads_point_set(newptr->pt,mpt);
					//			  ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, ename, RTPOINT, mpt, RTSTR, L"@", 0);
#endif // VERSION_CAD2010	
					
					/*			  {
					tpt[X] = mpt[X] + 0.5; tpt[Y] = mpt[Y] + 0.5; tpt[Z] = mpt[Z];
					i=acedCommandS(RTSTR,L"break",RTENAME,ename,RTPOINT,mpt,RTPOINT,tpt,0);				  
					ads_printf(L"i=%d",i);
					} */
					pttab->next = newptr->next;
					free(newptr);
					newptr = pttab;
				}
				else
				{
					if(pttab) free(pttab);
					ads_relrb(e);
					//			  ads_alert(L"s4");
					ads_ssfree(ename);  
					goto L60;
				}
				while(newptr->next != NULL)
				{
					newptr = newptr->next;
					ads_point_set(newptr->pt,mpt);
					ads_entlast(last);
					//			ads_printf(L"mpt[X]=%lf,mpt[Y]=%lf,mpt[Z]=%lf\n",mpt[X],mpt[Y],mpt[Z]);
#ifdef VERSION_CAD2016
					acedCommandS(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
					ads_command(RTSTR, L"break", RTENAME, last, RTPOINT, mpt, RTPOINT, mpt, 0);
#endif // VERSION_CAD2010	
					
					pttab->next = newptr->next;
					free(newptr);
					ads_ssfree(last);
					newptr = pttab;
				}
				if(pttab) free(pttab);
			}  
			ads_relrb(e); 
			ads_ssfree(ename);          
		}
	}
	ads_relrb(pointlist);
	ads_ssfree(ssname);

	return 1;
}     
/*------------------------------------------------------------------
;   get the intersection point xy between polyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node *Verlst;

	Verlst = getArcpt(Entlist,p0,p1);
	//   ads_alert(L"ok2");   
	if(Verlst->next==NULL)
		Verlst = getplinept(Entlist,p0,p1);

	return Verlst;
}
/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *getArcpt(struct resbuf *Entlist,ads_point p0, ads_point p1)
{
	struct resbuf *elst,*tlst;
	struct arc_lst *xyr;
	struct ptt_node *headptr,*tailptr,*newptr;
	struct point_lk *xy,*txy;
	int fit_type;
	double Zelev,bulge;
	bool loop;
	ads_name tname,result;
	ads_point p00,pt0,pt1,pt;

	headptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	getrbpoint(pt0,elst,10);
	/*   plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;
	ads_printf(L"zz=%lf\n",Zelev);
	plst = get_resbuf(10,elst);

	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	pt0[Z] = Zelev;
	} */

	tlst = get_resbuf(70,elst);
	fit_type = tlst->resval.rint;

	p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = Zelev;

	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
		tlst = get_resbuf(-1,elst);
		ads_entnext(tlst->resval.rlname,result);

		//	 ads_alert(L"1"); 
		elst = ads_entget(result); 
		//	 plst = plst->rbnext;
		/*     plst = get_resbuf(10,plst);
		if(plst)
		{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = plst->resval.rpoint[Z];
		} */
		getrbpoint(pt0,elst,10);

		Zelev = pt0[Z];
	}

	p0[Z] = Zelev; p1[Z] = Zelev;
	//   ads_printf(L"fit_type=%d\n",fit_type);*/

	if(fit_type < 2) 
	{
		/*     tlst = get_resbuf(0,elst);
		ads_printf(L"string=%s\n",tlst->resval.rstring);*/
		tlst = get_resbuf(42,elst);
		/*	 if(!tlst)
		{
		ads_alert(L"q");
		return headptr;
		} */
		bulge = tlst->resval.rreal;
	}

	//   ads_printf(L"bulge=%lf\n",bulge);
	if(fit_type < 2 && fabs(bulge) > 0.00001)
	{
		tlst = get_resbuf(-1,elst);
		ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tname,result);
		elst = ads_entget(result);
		getrbpoint(pt1,elst,10);  
		/*     plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt1[X] = plst->resval.rpoint[X];
		pt1[Y] = plst->resval.rpoint[Y];
		pt1[Z] = Zelev;
		}  */


		xyr = findxyr(pt0,pt1,bulge);
		xy = findroot(pt0,pt1,xyr,bulge,p0,p1);

		if(xyr) free(xyr);

		while(xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if((fabs(pt0[X]-pt[X])<0.001 && fabs(pt0[Y]-pt[Y])<0.001 && fabs(pt0[Z]-pt[Z])<0.001)
				|| (fabs(pt1[X]-pt[X])<0.001 && fabs(pt1[Y]-pt[Y])<0.001 && fabs(pt1[Z]-pt[Z])<0.001))        
				loop = true;
			else
			{
				newptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
				newptr->next = NULL;
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(pt,newptr->pt);
				ads_point_set(pt1,newptr->pt1); 

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if(txy) free(txy);
		}
	}

	return(headptr);
}          

/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *get_plinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0,pt1,mptn,mpto,PTS;
	struct resbuf *elst,*plst;
	//  ads_name tname,result;
	struct ptt_node *newptr,*headptr,*tailptr;
	double Zelev;
	int i;

	headptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	//  getrbpoint(pt0,elst,10);
	plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10,elst);
	//      ads_alert(L"1");
	if(plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	else
		return headptr;
	ads_point_set(pt0,PTS);
	ads_point_set(pt0,mpto); 
	/*   p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = pt0[Z];
	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
	tlst = get_resbuf(-1,elst);
	ads_name_set(tname,tlst->resval.rlname);
	ads_entnext(tname,result);
	ads_entdel(result); 
	elst = ads_entget(result); 
	getrbpoint(pt0,elst,10); 
	plst = plst->rbnext;
	plst = get_resbuf(10,plst);
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	//      pt0[Z] = plst->resval.rpoint[Z];
	}
	}  */   
	/*   tlst = get_resbuf(0,elst);
	_tcscpy(Zo,tlst->resval.rstring);*/
	//   ads_point_set(pt0,PTS);
	//   ads_point_set(pt0,mpto);

	p0[Z] = Zelev; p1[Z] = Zelev;

	//   ads_alert(L"ii");
	while(plst != NULL)
	{
		//     getrbpoint(pt1,elst,10);
		plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;

			i = ads_inters(p0,p1,pt0,pt1,1,mptn);

			if(i==RTNORM)
			{
				mptn[Z] = Zelev;

				if(fabs(mptn[X]-mpto[X])>0.001 || fabs(mptn[Y]-mpto[Y])>0.001 || fabs(mptn[Z]-mpto[Z])>0.001)
				{
					newptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
					ads_point_set(pt0,newptr->pt0);
					ads_point_set(mptn,newptr->pt); 
					//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
					ads_point_set(pt1,newptr->pt1);
					newptr->next = NULL;
					tailptr->next = newptr;
					tailptr = newptr;
				}
			}
			ads_point_set(pt1,pt0);
			ads_point_set(mptn,mpto); 

			/*     tlst = get_resbuf(-1,elst);
			ads_name_set(tname,tlst->resval.rlname);
			ads_entnext(tname,result);
			elst = ads_entget(result);
			tlst = get_resbuf(0,elst);
			_tcscpy(Zo,tlst->resval.rstring);*/
		}
	}
	//   ads_alert(L"kk");
	if(headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt,pt0);
		if(fabs(PTS[X]-pt0[X])<0.0001 && fabs(PTS[Y]-pt0[Y])<0.0001 && fabs(PTS[Z]-pt0[Z])<0.0001)
		{
			headptr->next = newptr->next;
			if(newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if(fabs(mptn[X]-pt1[X])<0.0001 && fabs(mptn[Y]-pt1[Y])<0.0001 && fabs(mptn[Z]-pt1[Z])<0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if(newptr==tailptr)
				return headptr;
			while(newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if(tailptr) free(tailptr);
		}
	}

	return(headptr);
}

/*------------------------------------------------------------------
;   get the intersection point xy between polyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	ads_point pt0,pt1,p00,mptn,mpto,PTS;
	struct resbuf *elst,*tlst;
	ads_name result;
	struct ptt_node *newptr,*headptr,*tailptr;
	double Zelev;
	ACHAR Zo[20];
	int i;

	headptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;

	elst = Entlist;
	getrbpoint(pt0,elst,10);
	/*   plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;
	ads_printf(L"k=%d,Ze=%lf\n",plst->restype,Zelev); 
	plst = get_resbuf(10,elst);
	ads_alert(L"1");
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	pt0[Z] = Zelev;
	}
	else
	return headptr; */

	p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = pt0[Z];
	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
		tlst = get_resbuf(-1,elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname,result);

		elst = ads_entget(result); 
		getrbpoint(pt0,elst,10); 
		Zelev = pt0[Z]; 
		/*	 plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		//      pt0[Z] = plst->resval.rpoint[Z];
		} */
	}     
	tlst = get_resbuf(0,elst);
	_tcscpy(Zo,tlst->resval.rstring);
	ads_point_set(pt0,PTS);
	ads_point_set(pt0,mpto);

	p0[Z] = Zelev; p1[Z] = Zelev;

	//   ads_alert(L"ii");
	while(_tcscmp(Zo,L"SEQEND") != 0)
	{
		getrbpoint(pt1,elst,10);
		/*	 plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
		pt1[X] = plst->resval.rpoint[X];
		pt1[Y] = plst->resval.rpoint[Y];*/
		pt1[Z] = Zelev;

		i = ads_inters(p0,p1,pt0,pt1,1,mptn);

		if(i==RTNORM)
		{
			mptn[Z] = Zelev;
			//	     ads_printf(L"i=%d,tn[X]=%lf,tn[Y]=%lf,tn[Z]=%lf\n",i,mptn[X],mptn[Y],mptn[Z]);
			if(fabs(mptn[X]-mpto[X])>0.001 || fabs(mptn[Y]-mpto[Y])>0.001 || fabs(mptn[Z]-mpto[Z])>0.001)
			{
				newptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(mptn,newptr->pt); 
				//		   ads_printf(L"xx=%lf\n",newptr->pt[X]);
				ads_point_set(pt1,newptr->pt1);
				newptr->next = NULL;
				tailptr->next = newptr;
				tailptr = newptr;
			}
		}
		ads_point_set(pt1,pt0);
		ads_point_set(mptn,mpto); 

		tlst = get_resbuf(-1,elst);
		//     ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tlst->resval.rlname,result);
		elst = ads_entget(result);
		tlst = get_resbuf(0,elst);
		_tcscpy(Zo,tlst->resval.rstring);
	}
	//   ads_alert(L"kk");
	if(headptr->next != NULL)
	{
		newptr = headptr->next;
		ads_point_set(newptr->pt,pt0);
		if(fabs(PTS[X]-pt0[X])<0.0001 && fabs(PTS[Y]-pt0[Y])<0.0001 && fabs(PTS[Z]-pt0[Z])<0.0001)
		{
			headptr->next = newptr->next;
			if(newptr) free(newptr);
		}
		//	 ads_alert(L"jj");
		if(fabs(mptn[X]-pt1[X])<0.0001 && fabs(mptn[Y]-pt1[Y])<0.0001 && fabs(mptn[Z]-pt1[Z])<0.0001)
		{
			//		 ads_alert(L"j");
			newptr = headptr;
			if(newptr==tailptr)
				return headptr;
			while(newptr->next != tailptr)
				newptr = newptr->next;
			newptr->next = NULL;
			if(tailptr) free(tailptr);
		}
	}

	return(headptr);
}

struct resbuf *get_resbuf(int code,struct resbuf *p)
{
	struct resbuf *rb;
	for(rb=p; (rb != NULL) && (rb->restype != code); rb = rb->rbnext);
	return(rb);
}

int getrbpoint(ads_point pt,struct resbuf *ebuf,int code)
{
	struct resbuf *rb;

	if((rb = get_resbuf(code,ebuf)) == NULL)
		return RTERROR;
	pt[X] = rb->resval.rpoint[X];
	pt[Y] = rb->resval.rpoint[Y];
	pt[Z] = rb->resval.rpoint[Z];
	return 1;
}
/*------------------------------------------------------------------
c  find the intersection point between two stright lines.         |
c  (x,y) -- intersective point coordinates                        |
c  INF   -- flag: =0, IP point lies in stright line section       |
c                 =1, IP point lies in out of stright line section|
c                 =-1,there is no intersection point              |
c------------------------------------------------------------------*/
int  INTERS(double *XX,double *YY,double X1,double Y1,double X2,double Y2,
			double X3,double Y3,double X4,double Y4)
{
	double A11,A12,A21,A22,B1,B2,D,D1,D2;
	int INF;
	double X1min,Y1min,X2min,Y2min,X1max,Y1max,X2max,Y2max;


	A11 = Y2-Y1;
	A12 = X1-X2;
	A21 = Y4-Y3;
	A22 = X3-X4;
	B1 = X1*Y2 - X2*Y1;
	B2 = X3*Y4 - X4*Y3;
	D = A11*A22-A21*A12;
	if(fabs(D)<1.0E-5)
		INF = -1;
	else
	{
		D1 = B1*A22-B2*A12;
		D2 = B2*A11-B1*A21;
		*XX  = D1/D;
		*YY  = D2/D;
		X1min=min(X1,X2)-0.000001;
		X1max=max(X1,X2)+0.000001;
		Y1min=min(Y1,Y2)-0.000001;
		Y1max=max(Y1,Y2)+0.000001;
		X2min=min(X3,X4)-0.000001;
		X2max=max(X3,X4)+0.000001;
		Y2min=min(Y3,Y4)-0.000001;
		Y2max=max(Y3,Y4)+0.000001;
		if(X1min<=*XX && *XX<=X1max && Y1min<=*YY && *YY<=Y1max
			&& X2min<=*XX && *XX<=X2max
			&& Y2min<=*YY && *YY<=Y2max)
			INF = 0;
		else
			INF = 1;
	}
	return INF;
}    
/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1)
{
	struct ptt_node *Verlst;

	Verlst = get_Arcpt(Entlist,p0,p1);
	//   ads_alert(L"ok2");   
	if(Verlst->next==NULL)
		Verlst = get_plinept(Entlist,p0,p1);

	return Verlst;
}
/*------------------------------------------------------------------
;   get the intersection point xy between wppolyline and line p0 p1
;   return  coordinate of intersection point list (pt0 mpt pt1)
;           if have intersection point,  else, nil
;   Entlist: list of entity
;---------------------------------------------------*/
struct ptt_node *get_Arcpt(struct resbuf *Entlist,ads_point p0, ads_point p1)
{
	struct resbuf *elst,*tlst,*plst;
	struct arc_lst *xyr;
	struct ptt_node *headptr,*tailptr,*newptr;
	struct point_lk *xy,*txy;
	int fit_type;
	double Zelev,bulge;
	bool loop;
	ads_point pt0,pt1,pt;

	headptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
	headptr->next = NULL;
	tailptr = headptr;
	elst = Entlist;
	//   getrbpoint(pt0,elst,10);
	plst = get_resbuf(38,elst);
	Zelev = plst->resval.rreal;

	plst = get_resbuf(10,elst);

	if(plst)
	{
		pt0[X] = plst->resval.rpoint[X];
		pt0[Y] = plst->resval.rpoint[Y];
		pt0[Z] = Zelev;
	}
	tlst = get_resbuf(70,elst);
	fit_type = tlst->resval.rint;

	/*   p00[X] = pt0[X]; p00[Y] = pt0[Y]; p00[Z] = Zelev;

	if(fabs(pt0[X]-p00[X])<0.001 && fabs(pt0[Y]-p00[Y])<0.001 && fabs(pt0[Z]-p00[Z])<0.001)
	{
	tlst = get_resbuf(-1,elst);
	ads_entnext(tlst->resval.rlname,result);
	ads_printf(L"name=%ld,result=%ld\n",tlst->resval.rlname,result);
	ads_alert(L"1"); 
	plst = ads_entget(result); 
	plst = plst->rbnext;
	plst = get_resbuf(10,plst);
	if(plst)
	{
	pt0[X] = plst->resval.rpoint[X];
	pt0[Y] = plst->resval.rpoint[Y];
	//      pt0[Z] = plst->resval.rpoint[Z];
	}
	//	 getrbpoint(pt0,elst,10);

	Zelev = pt0[Z];
	}

	p0[Z] = Zelev; p1[Z] = Zelev;
	//   ads_printf(L"fit_type=%d\n",fit_type);*/

	if(fit_type < 2) 
	{
		/*     tlst = get_resbuf(0,elst);
		ads_printf(L"string=%s\n",tlst->resval.rstring);*/
		tlst = get_resbuf(42,tlst);
		/*	 if(!tlst)
		{
		ads_alert(L"q");
		return headptr;
		} */
		bulge = tlst->resval.rreal;
	}

	//   ads_printf(L"bulge=%lf\n",bulge);
	if(fit_type < 2 && fabs(bulge) > 0.00001)
	{
		/*     tlst = get_resbuf(-1,elst);
		ads_name_set(tname,tlst->resval.rlname);
		ads_entnext(tname,result);
		elst = ads_entget(result);
		getrbpoint(pt1,elst,10);  */
		plst = plst->rbnext;
		plst = get_resbuf(10,plst);
		if(plst)
		{
			pt1[X] = plst->resval.rpoint[X];
			pt1[Y] = plst->resval.rpoint[Y];
			pt1[Z] = Zelev;
		}


		xyr = findxyr(pt0,pt1,bulge);
		xy = findroot(pt0,pt1,xyr,bulge,p0,p1);

		if(xyr) free(xyr);

		while(xy->next != NULL)
		{
			txy = xy->next;
			xy->next = txy->next;

			pt[X] = txy->pt[X]; pt[Y] = txy->pt[Y];

			if((fabs(pt0[X]-pt[X])<0.001 && fabs(pt0[Y]-pt[Y])<0.001 && fabs(pt0[Z]-pt[Z])<0.001)
				|| (fabs(pt1[X]-pt[X])<0.001 && fabs(pt1[Y]-pt[Y])<0.001 && fabs(pt1[Z]-pt[Z])<0.001))        
				loop = true;
			else
			{
				newptr = (struct ptt_node *)calloc(1,sizeof(struct ptt_node));
				newptr->next = NULL;
				ads_point_set(pt0,newptr->pt0);
				ads_point_set(pt,newptr->pt);
				ads_point_set(pt1,newptr->pt1); 

				tailptr->next = newptr;
				tailptr = newptr;
			}

			if(txy) free(txy);
		}
	}

	return(headptr);
}


/*-------------------------------------------------------------------
;  Given starting point Ps, ending point PE of a piece of arc, bulge of arc,
;  find the ridius R and center coordinates.
;   return: (R (xo yo))
;-------------------------------------------------------------------*/
struct arc_lst *findxyr(ads_point ps,ads_point pe,ads_real bulge)
{
	ads_real a,gama,cord,R,cita,angse,ang;
	ads_point cxy;
	struct arc_lst *newptr;

	a = atan(fabs(bulge));
	gama = 2.0 * a;
	cord = ads_distance(ps,pe);
	R = (0.5 * cord) / sin(gama);
	cita = 0.5 * PI - gama;
	angse = ads_angle(ps,pe);
	if(bulge > 0.0)
		ang = angse + cita;
	else
		ang = angse - cita;
	ads_polar(ps,ang,R,cxy);
	newptr = (struct arc_lst *)calloc(1,sizeof(struct arc_lst));
	newptr->R = R;
	newptr->xo = cxy[X];
	newptr->yo = cxy[Y];
	return(newptr);
}
/*-------------------------------------------------------------------
;  Given starting point Ps, ending point PE of a piece of arc, bulge of arc,
;  find the ridius R and center coordinates.
;   return: (R (xo yo))
;-------------------------------------------------------------------*/
struct point_lk *findroot(ads_point ps, ads_point pe, struct arc_lst *pxyr,ads_real bulge,
	ads_point ps1, ads_point ps2)
{
	ads_real R,xo,yo,xs1,ys1,xs2,ys2,dx,dy,b,k,a1,b1,c1,del,x1,y1,x2,y2,pi2,aps,ape,alp1,alp2;
	bool OK;
	ads_point p1,p2,cxy;
	struct point_lk *newptr,*headptr,*tailptr;

	newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
	if(newptr)
	{
		newptr->next = NULL;
		headptr = newptr;
		tailptr = newptr;
	}

	R = pxyr->R;
	xo = pxyr->xo; yo = pxyr->yo;
	xs1 = ps1[X] , ys1=ps1[Y];
	xs2 = ps2[X] , ys2=ps2[Y];
	cxy[X] = xo; cxy[Y] = yo;

	dx = ps1[X] - ps2[X];
	OK = true;
	if(fabs(dx) > 0.00000001)
	{
		dy = ps1[Y] - ps2[Y];
		k = dy / dx;
		b = ps1[Y] - k * ps1[X];
		a1 = 1.0 + k * k;
		b1 = 2.0 * k * (b - yo) - 2.0 * xo;
		c1 = xo * xo + (b-yo) * (b-yo) - R * R;
		del = b1 * b1 - 4.0 * a1 * c1;
		if(del >= 0.0)
		{
			del = sqrt(del);
			x1 = (del - b1)/(2.0*a1);
			y1 = k * x1 + b;
			x2 = (del + b1)/(-2.0*a1);
			y2 = k * x2 + b;
		}
		else
			OK = false;
	}
	else
	{
		x1 = xs1; x2 = x1;
		del = R*R - (x1 - xo) * (x1 - xo);
		if(del >= 0.0)
		{
			del = sqrt(del);
			y1 = yo + del;
			y2 = yo - del;
		}
		else
			OK = false;
	}
	p1[X] = x1; p1[Y] = y1; p2[X] = x2; p2[Y] = y2; pi2 = 2.0 * PI;

	if(OK)
	{
		aps = ads_angle(cxy,ps);
		ape = ads_angle(cxy,pe);
		alp1 = ads_angle(cxy,p1);
		alp2 = ads_angle(cxy,p2);
		if(bulge > 0.0)
		{
			if(ape > aps)
			{
				if(alp1 >= aps && alp1 <= ape)
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}				
				}
				if(alp2 >= aps && alp2 <= ape)
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								 
				}
			}
			else
			{
				if((alp1>=0.0 && alp1<=ape) || (alp1>= aps && alp1 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								 
				}
				if((alp2 >=0.0 && alp2 <= ape) || (alp2 >= aps && alp2 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								 
				}
			}
		}
		else
		{
			if(aps > ape)
			{
				if(alp1>=ape && alp1<=aps)
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								  
				}
				if(alp2>=ape && alp2<=aps)
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}				                
				}
			}
			else
			{
				if((alp1>=0.0 && alp1<=aps) || (alp1>=ape && alp1 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p1[X]; newptr->pt[Y] = p1[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								  
				}
				if((alp2>=0.0 && alp2<=aps) || (alp2>=ape && alp2 <= pi2))
				{
					newptr = (struct point_lk *)calloc(1,sizeof(struct point_lk));
					if(newptr)
					{
						newptr->next = NULL;
						newptr->pt[X] = p2[X]; newptr->pt[Y] = p2[Y];
						tailptr->next = newptr;
						tailptr = newptr;
					}								 
				}
			}
		}
	}
	return headptr; 
}

void SetDrawQXYS::OnRadiobcbsj() 
{
	// TODO: Add your control notification handler code here
	DrawMode = 0;
}

void SetDrawQXYS::OnRadiosgtsj() 
{
	// TODO: Add your control notification handler code here
	DrawMode = 1;
}

void SetDrawQXYS::OnSelchangeCombostjd() 
{
	// TODO: Add your control notification handler code here
	int i = m_SjdCtrl.GetCurSel();
	CString str;	
	m_SjdCtrl.GetLBText(i,str);
	if(_wtoi(str) <= _wtoi(m_EndJD))
		m_StaJD = str;
	else
		ads_alert(L"起始号须小于等于终止号!");
	UpdateData(false);
}

void SetDrawQXYS::OnSelchangeComboedjd() 
{
	// TODO: Add your control notification handler code here
	int i = m_EjdCtl.GetCurSel();
	CString str;	
	m_EjdCtl.GetLBText(i,str);
	if(_wtoi(str) >= _wtoi(m_StaJD))
		m_EndJD = str;
	else
		ads_alert(L"终止号须大于等于起始号!");
	UpdateData(false);	
}

void SetDrawQXYS::OnSelchangeComboangle() 
{
	// TODO: Add your control notification handler code here
	int i = m_ZJANGLE.GetCurSel();	
	m_ZJANGLE.GetLBText(i,m_InputAng);
	m_Angle = m_InputAng;
	UpdateData(false);
}

void SetDrawQXYS::OnButtonBrow() 
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
	//	DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");			
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
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_Dbs);
	int i;

	m_RoadNameCrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	m_RoadName = "主线";
	CString str;
	for(i=1;i<pm->JdCenNum-1;i++)
	{
		str.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
		//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
		m_SjdCtrl.AddString(str);
	}
	m_StaJD.Format(L"%0.0lf",pm->JdCenArray[1].JdNo);
	m_SjdCtrl.SetCurSel(0);
	for(i=1;i<pm->JdCenNum-1;i++)
	{
		str.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
		m_EjdCtl.AddString(str);
		m_EndJD.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
	}	
	m_EjdCtl.SetCurSel(pm->JdCenNum-2);

	UpdateData(false);
}

void SetDrawQXYS::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i,m_RoadName);
	DBS.Read_XLDbs(m_Dbs,L"控制点表",m_RoadName);
	//	DBS.Read_XLDbs(m_Dbs,L"线元表",m_RoadName);			
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

	CString str;
	for(i=1;i<pm->JdCenNum-1;i++)
	{
		str.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
		//	ads_printf(L"%lf\n",pm->JdCenArray[i].JdNo);
		m_SjdCtrl.AddString(str);
	}
	m_StaJD.Format(L"%0.0lf",pm->JdCenArray[1].JdNo);
	m_SjdCtrl.SetCurSel(0);
	for(i=1;i<pm->JdCenNum-1;i++)
	{
		str.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
		m_EjdCtl.AddString(str);
		m_EndJD.Format(L"%0.0lf",pm->JdCenArray[i].JdNo);
	}	
	m_EjdCtl.SetCurSel(pm->JdCenNum-2);	
	UpdateData(false);	
}
