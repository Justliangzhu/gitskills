// DrawJWGRID.cpp : implementation file
//

#include "stdafx.h"
#include "DrawJWGRID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]
/////////////////////////////////////////////////////////////////////////////
// DrawJWGRID dialog


DrawJWGRID::DrawJWGRID(CWnd* pParent /*=NULL*/)
	: CDialog(DrawJWGRID::IDD, pParent)
{
	//{{AFX_DATA_INIT(DrawJWGRID)
	pm = NULL;
	m_anxl = true;
	m_prex = _T("N");
	m_prey = _T("E");
	m_CrossLen = 40.0;
	m_Hspacer = 200.0;
	m_Lbx = 0.0;
	m_Lby = 0.0;
	m_Rux = 0.0;
	m_Ruy = 0.0;
	m_Vspacer = 200.0;
	m_Wordht = 3.0;
	m_Distoxl = 300.0;
	pm=NULL;
	//}}AFX_DATA_INIT
}


void DrawJWGRID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DrawJWGRID)
	DDX_Check(pDX, IDC_CHECK_ANXL, m_anxl);
	DDX_CBString(pDX, IDC_COMBO_PRX, m_prex);
	DDX_CBString(pDX, IDC_COMBO_PRY, m_prey);
	DDX_Text(pDX, IDC_EDIT_CROSSLEN, m_CrossLen);
	DDX_Text(pDX, IDC_EDIT_HSPACER, m_Hspacer);
	DDX_Text(pDX, IDC_EDIT_LBX, m_Lbx);
	DDX_Text(pDX, IDC_EDIT_LBY, m_Lby);
	DDX_Text(pDX, IDC_EDIT_RUX, m_Rux);
	DDX_Text(pDX, IDC_EDIT_RUY, m_Ruy);
	DDX_Text(pDX, IDC_EDIT_VSPACER, m_Vspacer);
	DDX_Text(pDX, IDC_EDIT_WORDHT, m_Wordht);
	DDX_Text(pDX, IDC_EDIT_DISTOXL, m_Distoxl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DrawJWGRID, CDialog)
	//{{AFX_MSG_MAP(DrawJWGRID)
	ON_BN_CLICKED(IDC_BUTTON_pick, OnBUTTONpick)
	ON_BN_CLICKED(IDC_CHECK_ANXL, OnCheckAnxl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DrawJWGRID message handlers

BOOL DrawJWGRID::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DrawJWGRID::OnBUTTONpick() 
{
	// TODO: Add your control notification handler code here
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	if(acedGetPoint(NULL,L"请在屏幕上拾取经纬网格标注的左下角点!\n",resPt)==RTNORM)
	{
		m_Lbx = resPt[X];
		m_Lby = resPt[Y];
	}
	struct resbuf result;
	int type;
	ads_point pt,ptold,LDpt,LUpt,RDpt,RUpt,LDptold,LUptold,RDptold,RUptold;
	ptold[X]=m_Lbx;
	ptold[Y]=m_Lby;
	ptold[Z]=0.0;
	LDpt[X]=m_Lbx;
	LDpt[Y]=m_Lby;
	LDpt[Z]=0.0;
	for(;;)
	{
		ads_printf(L"请在屏幕上拾取经纬网格标注的右上角点!\n");
		if(ads_grread(1,&type,&result)!=RTNORM)
		{ break ;}
		if(type == 3)
		{
			m_Rux = result.resval.rpoint[X];
			m_Ruy = result.resval.rpoint[Y];
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
			LUpt[X]=m_Lbx;
			LUpt[Y]=pt[Y];
			LUpt[Z]=0.0;
			RUpt[X]=pt[X];
			RUpt[Y]=pt[Y];
			RUpt[Z]=0.0;
			RDpt[X]=pt[X];
			RDpt[Y]=m_Lby;
			RDpt[Z]=0.0;
			/*			if(ptold[X] < 0.1)
			{
			ads_grdraw(LUpt,RUpt,-1,0);	
			ads_grdraw(RUpt,RDpt,-1,0);	
			ads_grdraw(RDpt,LDpt,-1,0);	
			ads_grdraw(LDpt,LUpt,-1,0);	
			}*/

			if(caldistance(pt,ptold)>0.1)
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
				//		ads_printf(L"%lf %lf %lf %lf\n",LUpt[X],LUpt[Y],LDpt[X],LDpt[Y]);
				//		ads_printf(L"%lf %lf %lf %lf\n",RUpt[X],RUpt[Y],RDpt[X],RDpt[Y]);
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

void DrawJWGRID::OnCheckAnxl() 
{
	// TODO: Add your control notification handler code here
	m_anxl = !m_anxl;
	if(m_anxl)
		GetDlgItem(IDC_EDIT_DISTOXL)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_DISTOXL)->EnableWindow(false);
}

void DrawJWGRID::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void DrawJWGRID::DRAWGRID()
{
	int SUM,count;
	AcGePoint3d spt,ept;
	ACHAR str[80];
	double cml;
	BAS_DRAW_FUN::xlpoint PZ;
	ads_point pt1,pt2;

	double BaseX,BaseY,StepY,StepX;

	BaseX = floor(m_Lbx/m_Hspacer) * m_Hspacer;
	BaseY = floor(m_Lby/m_Vspacer) * m_Vspacer;
	SUM = int((m_Rux-m_Lbx)/m_Hspacer*(m_Ruy-m_Lby)/m_Vspacer);
	StepY = BaseY;
	ACHAR pszLabel[80];
	_tcscpy(pszLabel,L"绘制经纬网格");
	acedSetStatusBarProgressMeter(pszLabel,0,100);
	int Pos ;
	count =0;
	if(m_anxl)
	{
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		if(Pro!="")
		{
			m_Dbs = Cworkdir+"\\DATA\\";
			m_Dbs = m_Dbs + Pro+".mdb";		
			//		   DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");	
			DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");				   
			DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
			if(DBS.XYNum>0)
			{
				pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//			  pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					pm->setDLB(DBS.DLArray,DBS.NDL);
			}
			_tcscpy(pm->mdbname,m_Dbs);
			_tcscpy(pm->RoadName,L"主线");
		}
	}
	while (StepY < m_Ruy)
	{
		StepX = BaseX;
		while(StepX < m_Rux)
		{
			if(m_anxl&&pm)
			{
				cml = pm->PROJ_ML(StepY,StepX);
				PZ.lc = cml;
				pm->xlpoint_pz(&PZ);
				pt1[X] = StepY;pt1[Y] = StepX;
				pt2[X] = PZ.x;pt2[Y] = PZ.y;
				if(caldistance(pt1,pt2)<m_Distoxl)
				{
					spt.x = StepX-0.5*m_CrossLen;
					spt.y = StepY;
					ept.x = StepX+0.5*m_CrossLen;
					ept.y = StepY;
					BAS_DRAW_FUN::makeline(spt,ept,0,0,L"经纬网格");
					spt.x = StepX;
					spt.y = StepY-0.5*m_CrossLen;
					ept.x = StepX;
					ept.y = StepY+0.5*m_CrossLen;
					BAS_DRAW_FUN::makeline(spt,ept,0,0,L"经纬网格");
					_stprintf(str,L"%s%0.0lf",m_prex,StepY);
					spt.x = StepX+0.25*m_CrossLen;
					spt.y = StepY+m_Wordht;
					BAS_DRAW_FUN::maketext(spt,str,0,m_Wordht,0,3,L"宋体",L"经纬网格");
					spt.x = StepX-m_Wordht;
					spt.y = StepY+0.25*m_CrossLen;
					_stprintf(str,L"%s%0.0lf",m_prey,StepX);
					BAS_DRAW_FUN::maketext(spt,str,0.5*PI,m_Wordht,0,3,L"宋体",L"经纬网格");
				}
			}
			else
			{
				spt.x = StepX-0.5*m_CrossLen;
				spt.y = StepY;
				ept.x = StepX+0.5*m_CrossLen;
				ept.y = StepY;
				BAS_DRAW_FUN::makeline(spt,ept,0,0,L"经纬网格");
				spt.x = StepX;
				spt.y = StepY-0.5*m_CrossLen;
				ept.x = StepX;
				ept.y = StepY+0.5*m_CrossLen;
				BAS_DRAW_FUN::makeline(spt,ept,0,0,L"经纬网格");
				_stprintf(str,L"%s%0.0lf",m_prex,StepY);
				spt.x = StepX+0.25*m_CrossLen;
				spt.y = StepY+m_Wordht;
				BAS_DRAW_FUN::maketext(spt,str,0,m_Wordht,0,3,L"宋体",L"经纬网格");
				spt.x = StepX-m_Wordht;
				spt.y = StepY+0.25*m_CrossLen;
				_stprintf(str,L"%s%0.0lf",m_prey,StepX);
				BAS_DRAW_FUN::maketext(spt,str,0.5*PI,m_Wordht,0,3,L"宋体",L"经纬网格");
			}
			Pos = (int)(count*100.0/SUM);
			acedSetStatusBarProgressMeterPos(Pos);
			StepX += m_Hspacer;
			count++;
		}
		StepY += m_Vspacer;
	}
	acedSetStatusBarProgressMeterPos(100);
}
