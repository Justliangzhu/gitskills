// DRAWXYTZD.cpp : implementation file
//

#include "stdafx.h"
#include "DRAWXYTZD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DRAWXYTZD dialog
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]

DRAWXYTZD::DRAWXYTZD(CWnd* pParent /*=NULL*/)
	: CDialog(DRAWXYTZD::IDD, pParent)
{
	//{{AFX_DATA_INIT(DRAWXYTZD)
	m_RoadName = L"主线";
	m_PTX = L"0";
	m_PTY = L"0";
	m_Dbs = L"";
	pm=NULL;
	//}}AFX_DATA_INIT
}


void DRAWXYTZD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DRAWXYTZD)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_PTX, m_PTX);
	DDX_Text(pDX, IDC_EDIT_PTY, m_PTY);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_Dbs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DRAWXYTZD, CDialog)
	//{{AFX_MSG_MAP(DRAWXYTZD)
	ON_BN_CLICKED(IDC_BUTTON_browser, OnBUTTONbrowser)
	ON_BN_CLICKED(IDC_BUTTON_SETBYPT, OnButtonSetbypt)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL, OnButtonPickxl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DRAWXYTZD message handlers

BOOL DRAWXYTZD::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir+"\\DATA\\";
	m_Dbs = m_Dbs + Pro+".mdb";

	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");	
	DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");		
	DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	else
	{
		CDialog::OnCancel();
		return 0;
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,L"主线");
	DBS.m_RoadSum = 0;
	DBS.ReadAllRoadName(m_Dbs);
	for(int i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DRAWXYTZD::OnBUTTONbrowser() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256];
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

	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");
	DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");		
	DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,L"主线");
	DBS.m_RoadSum = 0;
	DBS.ReadAllRoadName(m_Dbs);
	int i;
	m_RoadNameCrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);	
	UpdateData(false);
}

void DRAWXYTZD::OnButtonSetbypt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	double d1,d2,d3,h1;
	struct resbuf result;
	int type;
	ads_point pt,ptold,LDpt,LUpt,RDpt,RUpt,LDptold,LUptold,RDptold,RUptold;
	pt[X]=pt[Y]=pt[Z]=0;
	ptold[X]=ptold[Y]=ptold[Z]=0;
	LDpt[X]=LDpt[Y]=LDpt[Z]=0;
	LUpt[X]=LUpt[Y]=LUpt[Z]=0;
	RDpt[X]=RDpt[Y]=LUpt[Z]=0;
	LDptold[X]=LDptold[Y]=LDptold[Z]=0;
	LUptold[X]=LUptold[Y]=LUptold[Z]=0;
	RDptold[X]=RDptold[Y]=RDptold[Z]=0;
	RUptold[X]=RUptold[Y]=RUptold[Z]=0;

	int DFACTOR = 1;
	int JDn = pm->XYNum;
	d1 = 10*DFACTOR;
	d2 = 30*DFACTOR;
	d3 = 25*DFACTOR;
	h1 = 5*DFACTOR;

	double Wtab = d1+3*d2+2*d3;
	double Htab = 2*h1+JDn*h1;

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
			m_PTX.Format(L"%0.3lf",pt[X]);
			m_PTY.Format(L"%0.3lf",pt[Y]);
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
			LUpt[X]=pt[X],LUpt[Y]=pt[Y];
			ads_polar(LUpt,0.0,Wtab,RUpt);
			ads_polar(RUpt,0.5*PI,-Htab,RDpt);
			ads_polar(LUpt,0.5*PI,-Htab,LDpt);

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

void DRAWXYTZD::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i,m_RoadName);
	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",m_RoadName);
	DBS.Read_XLDbs(m_Dbs,L"线元表",m_RoadName);		
	DBS.Read_XLDbs(m_Dbs,L"断链表",m_RoadName);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,m_RoadName);

	UpdateData(false);	
}

void DRAWXYTZD::OnOK() 
{
	// TODO: Add extra validation here
	CDialog::OnOK();

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcGePoint3d  ptb,pt,pte;
	double d1,d2,d3,h1;
	double l,INDATA[5],OriX,OriY,texth;
	int i, j,k ;
	ACHAR chr[50],ch[40],tch[6],chAR[50],chtmp[40],lcchr[20];
	xlpoint  P0tmp;
	int DFACTOR = 1;
	d1 = 10*DFACTOR;
	d2 = 30*DFACTOR;
	d3 = 25*DFACTOR;
	h1 = 5*DFACTOR;	
	texth = 4*DFACTOR;	
	double Wtab = d1+3*d2+2*d3;
	double Htab = 2*h1+pm->XYNum*h1;

	OriX = _wtof(m_PTX);
	OriY = _wtof(m_PTY);

	ptb.x=OriX,ptb.y=OriY;
	pte.x=OriX+Wtab,pte.y=OriY;
	makeline(ptb,pte,0,0);
	pt.x = OriX,pt.y = OriY-Htab;
	makeline(ptb,pt,0,0);
	pt.x = pte.x; makeline(pte,pt,0,0);
	ptb.y=OriY-Htab;pte.y=OriY-Htab;
	makeline(ptb,pte,0,0);
	ptb.x=OriX+d1,ptb.y=OriY,pte.x=ptb.x,pte.y=OriY-Htab;makeline(ptb,pte,0,0);
	ptb.x=OriX+d1+d2;pte.x=ptb.x;pte.y=OriY-h1;makeline(ptb,pte,0,0);
	pte.y = OriY-2*h1,pt.x=ptb.x;pt.y=OriY-Htab;makeline(pte,pt,0,0);
	ptb.x=OriX+d1+2*d2;pte.x=ptb.x;pte.y=OriY-h1;makeline(ptb,pte,0,0);
	pte.y = OriY-2*h1,pt.x=ptb.x;pt.y=OriY-Htab;makeline(pte,pt,0,0);
	ptb.x=OriX+d1+3*d2;pte.x=ptb.x;pte.y=OriY-h1;makeline(ptb,pte,0,0);
	pte.y = OriY-2*h1,pt.x=ptb.x;pt.y=OriY-Htab;makeline(pte,pt,0,0);
	ptb.x=OriX+d1+3*d2+d3;pte.x=ptb.x;pte.y=OriY-h1;makeline(ptb,pte,0,0);
	pte.y = OriY-2*h1,pt.x=ptb.x;pt.y=OriY-Htab;makeline(pte,pt,0,0);
	ptb.x=OriX,ptb.y=OriY-h1,pte.x=OriX+Wtab,pte.y=ptb.y;makeline(ptb,pte,0,0);
	ptb.x=OriX,ptb.y=OriY-2*h1,pte.x=OriX+Wtab,pte.y=ptb.y;makeline(ptb,pte,0,0);
	pt.x=OriX+0.5*d1,pt.y=OriY-0.5*h1,maketext(pt,L"NO",0,texth,0,3);
	pt.x=OriX+d1+0.5*d2,maketext(pt,L"STA",0,texth,0,3);
	pt.x=OriX+d1+1.5*d2,maketext(pt,L"X",0,texth,0,3);
	pt.x=OriX+d1+2.5*d2,maketext(pt,L"Y",0,texth,0,3);
	pt.x=OriX+d1+3*d2+0.5*d3,maketext(pt,L"O",0,texth,0,3);
	pt.x=OriX+d1+3*d2+1.5*d3,maketext(pt,L"L",0,texth,0,3);
	pt.x=OriX+d1+0.5*d2,pt.y=OriY-1.5*h1,maketext(pt,pm->RoadName,0,texth-1,0,3);
	pt.x=OriX+d1+3*d2+0.5*d3,maketext(pt,pm->RoadName,0,texth-1,0,3);
	ptb.x=OriX+d1+0.85*d2,ptb.y =OriY-1.5*h1;
	pte.x=OriX+d1+3*d2+0.15*d3,pte.y=ptb.y;makeline(ptb,pte,0,0);
	CString str;
	for (i=1;i<=pm->XYNum;i++)
	{
		P0tmp.lc=pm->XYArray[i][6]; 
		P0tmp.x=pm->XYArray[i][8];
		P0tmp.y=pm->XYArray[i][9];
		P0tmp.a=pm->XYArray[i][5];
		P0tmp.r=pm->XYArray[i][7];

		for (k=0;k<5;k++)INDATA[k]=pm->XYArray[i][k];j=(int)(pm->XYArray[i][0]+0.01);
		l=0.0;
		if (j>=3)
		{
			_tcscpy(tch,L"H");
			_tcscpy(chAR,L"A = ");
			RToS(pm->XYArray[i][1],2,chtmp);
			_tcscat(chAR,chtmp);
		}
		if (j==1) //直线
		{
			l=INDATA[1]; 
			_tcscpy(tch,L"Z");
			_tcscpy(chAR,L"R = ");
			_tcscat(chAR,L"∞");
		}
		if (j==2) 
		{
			l=INDATA[2]; 
			_tcscpy(tch,L"Y"); 
			_tcscpy(chAR,L"R = ");
			RToS(pm->XYArray[i][1],2,chtmp);
			_tcscat(chAR,chtmp);
		}

		ACHAR QXflag[50];
		if (i>1 && i<=pm->XYNum-1)
		{
			j=(int)(pm->XYArray[i-1][0]+0.01);
			if (j>=3) { _tcscpy(ch,L"S"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch,L"H"); };
			if (j==1) { _tcscpy(ch,L"T"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch,L"Z"); };
			if (j==2) { _tcscpy(ch,L"C"); /*if (Chinese_Engbz_type==2)*/_tcscpy(ch,L"Y"); };
			_tcscpy(chr,ch);_tcscat(chr,tch);

			_tcscpy(QXflag,chr);

			if (_tcsncmp(chr,L"YY",3)==0) {  _tcscpy(chr,L"GQ");   };
			if (_tcsncmp(chr,L"C.C.",5)==0) {  _tcscpy(chr,L"P.C.C.");};
			if (_tcsncmp(chr,L"S.S.",5)==0&&(j==3||j==4) ) {  _tcscpy(chr,L"PRC");};
			if (_tcsncmp(chr,L"HH",3)==0&&(j==3||j==4))
				_tcscpy(chr,L"GQ");
		}
		else if(i==1)
			_tcscpy(chr,L"QD"); 
		else
			_tcscpy(chr,L"ZD"); 
		pt.x = OriX+0.5*d1,pt.y = OriY-2*h1-(i-1)*h1-0.5*h1; maketext(pt,chr,0,texth,0,3);
		str = pm->XLC(pm->XYArray[i][6]);
		_tcscpy(chr,str);
		pt.x= OriX+d1+0.5*d2, maketext(pt,chr,0,texth,0,3);
		_stprintf(chr,L"%0.4lf",pm->XYArray[i][8]);
		pt.x= OriX+d1+1.5*d2, maketext(pt,chr,0,texth,0,3);
		_stprintf(chr,L"%0.4lf",pm->XYArray[i][9]);
		pt.x= OriX+d1+2.5*d2, maketext(pt,chr,0,texth,0,3);
		double dfm=dms_rad(pm->XYArray[i][5]);
		_stprintf(chr,L"%0.5lf",dfm);
		pt.x= OriX+d1+3*d2+0.5*d3, maketext(pt,chr,0,texth,0,3);
		if(i<pm->XYNum)
		{
			double len= pm->XYArray[i+1][6]-pm->XYArray[i][6];
			_stprintf(chr,L"%0.3lf",len);
			pt.x= OriX+d1+3*d2+1.5*d3, pt.y= OriY-2*h1-(i-1)*h1-h1;maketext(pt,chr,0,texth,0,3);
			ptb.x= OriX,ptb.y= OriY-2*h1-i*h1,pte.x= OriX+Wtab-d3,pte.y= ptb.y,makeline(ptb,pte,0,0);		 
			ptb.x= OriX+Wtab-d3,ptb.y= OriY-2*h1-i*h1-0.5*h1,pte.x= OriX+Wtab,pte.y= ptb.y,makeline(ptb,pte,0,0);
		}
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
}

void DRAWXYTZD::OnButtonPickxl() 
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml=-100;
	//	acDocManager->setCurDocument(ZPMDOC,AcAp::kNone,Adesk::kTrue);
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

	AcDbObject *pObj=NULL;
	// acdbOpenObject(pObj, eId, AcDb::kForRead);
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)==Acad::eOk)
	{	
		//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
		if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
		{
			pObj->close();	
			//acdbOpenObject(pm, eId, AcDb::kForRead);
			JD_CENTER *pm = JD_CENTER::cast(pObj);
			//pm->close();
			m_Dbs=pm->mdbname;
			m_RoadName=pm->RoadName;
		}
		else
		{
			pObj->close();

			DBS.GetXLXdata(eId,m_Dbs,m_RoadName);
			//		ads_printf(L"%s %s\n",m_Dbs,m_RoadName);
			if(m_Dbs!=L""&&m_RoadName!=L"")
			{
				DBS.Read_XLDbs(m_Dbs,L"线元表",m_RoadName);		
				DBS.Read_XLDbs(m_Dbs,L"断链表",m_RoadName);
				if(DBS.XYNum>0)
				{
					pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
					//				pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
					if(DBS.NDL>0)
						pm->setDLB(DBS.DLArray,DBS.NDL);
					_tcscpy(pm->mdbname,m_Dbs);
					_tcscpy(pm->RoadName,m_RoadName);   
				}
			}	
		}	
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog
	UpdateData(false);
	return ;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE		
}
