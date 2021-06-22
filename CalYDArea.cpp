// CalYDArea.cpp : implementation file
//

#include "stdafx.h"
#include "CalYDArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalYDArea dialog


CCalYDArea::CCalYDArea(CWnd* pParent /*=NULL*/)
	: CDialog(CCalYDArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCalYDArea)
	m_EndDml = _T("");
	m_mdbname = _T("");
	m_YdArea = _T("");
	m_StartDml = _T("");
	m_RoadName = _T("");
	m_outbykm = FALSE;
	//}}AFX_DATA_INIT
	for(int i=0;i<50;i++)
	{
		RoadArr[i].pm = NULL;
		RoadArr[i].pZDK = NULL;
		RoadArr[i].NZDK = 0;
	}
}


void CCalYDArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCalYDArea)
	DDX_Control(pDX, IDC_COMBO_YDROADNAME, m_RoadNameCrl);
	DDX_Text(pDX, IDC_EDIT_ENDDML, m_EndDml);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_mdbname);
	DDX_Text(pDX, IDC_EDIT_YDAREA, m_YdArea);
	DDX_Text(pDX, IDC_EDITSTARTDML, m_StartDml);
	DDX_CBString(pDX, IDC_COMBO_YDROADNAME, m_RoadName);
	DDX_Check(pDX, IDC_CHECK_OUTBYKM, m_outbykm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCalYDArea, CDialog)
	//{{AFX_MSG_MAP(CCalYDArea)
	ON_BN_CLICKED(IDC_BUTTON_YDBROWSER, OnButtonYdbrowser)
	ON_BN_CLICKED(IDC_BUTTONPICKXL, OnButtonpickxl)
	ON_CBN_SELCHANGE(IDC_COMBO_YDROADNAME, OnSelchangeComboYdroadname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalYDArea message handlers

BOOL CCalYDArea::OnInitDialog() 
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

	m_mdbname = Cworkdir+"\\DATA\\";
	m_mdbname = m_mdbname + Pro+".mdb";	

	FILE *fpr;
	CString ZDKfilename;
	int DorS,j;
	double dml;
	m_RoadName = "主线";
	DBS.ReadAllRoadName(m_mdbname);
	NRoad = DBS.m_RoadSum;		
	for(int i=0;i<DBS.m_RoadSum;i++)
	{
		DBS.Read_XLDbs(m_mdbname,L"线元表",DBS.Road[i].RoadName);		
		DBS.Read_XLDbs(m_mdbname,L"断链表",DBS.Road[i].RoadName);
		if(DBS.XYNum>0)
		{
			RoadArr[i].pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				RoadArr[i].pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(RoadArr[i].pm->mdbname,m_mdbname);
			_tcscpy(RoadArr[i].pm->RoadName,DBS.Road[i].RoadName);
		}
		ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + Pro + "." + DBS.Road[i].RoadName;
		if((fpr=_wfopen(ZDKfilename ,L"r"))!=NULL)
		{
			fwscanf(fpr,L"%d",&RoadArr[i].NZDK);
			if(RoadArr[i].NZDK>0)
			{
				RoadArr[i].pZDK = new ZDKDAT[RoadArr[i].NZDK];
				for(j=0;j<RoadArr[i].NZDK;j++)
					fwscanf(fpr ,L"%lf%lf%d%lf%lf",&RoadArr[i].pZDK[j].cml,&dml,&RoadArr[i].pZDK[j].IsInsertHdm,&RoadArr[i].pZDK[j].LZDK,&RoadArr[i].pZDK[j].RZDK);
			}
			fclose(fpr);
		}	
	}
	m_StartDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[0].cml));
	m_EndDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[RoadArr[0].NZDK-1].cml));
	UpdateData(FALSE);		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCalYDArea::OnButtonYdbrowser() 
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
	m_mdbname=fname;
	if(m_mdbname==""){AfxMessageBox(L"未指定数据库!");return;}

	FILE *fpr;
	CString ZDKfilename;
	int DorS,j;
	double dml;
	CString str;
	str=m_mdbname;
	str.MakeUpper();
	int pos,len;
	pos = str.Find(L"DATA");
	len = str.GetLength();
	str=str.Right(len-pos-5);
	str.Replace(L".MDB",L"");

	m_RoadName = "主线";
	DBS.ReadAllRoadName(m_mdbname);
	NRoad = DBS.m_RoadSum;		
	for(int i=0;i<DBS.m_RoadSum;i++)
	{
		RoadArr[i].NZDK=0;
		if(RoadArr[i].pm)
		{ 
			delete RoadArr[i].pm;
			RoadArr[i].pm = NULL;
		}
		if(RoadArr[i].pZDK)
		{
			delete RoadArr[i].pZDK;
			RoadArr[i].pZDK=NULL;
		}
		DBS.Read_XLDbs(m_mdbname,L"线元表",DBS.Road[i].RoadName);		
		DBS.Read_XLDbs(m_mdbname,L"断链表",DBS.Road[i].RoadName);
		if(DBS.XYNum>0)
		{
			RoadArr[i].pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				RoadArr[i].pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(RoadArr[i].pm->mdbname,m_mdbname);
			_tcscpy(RoadArr[i].pm->RoadName,DBS.Road[i].RoadName);
		}
		ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + str + "." + DBS.Road[i].RoadName;
		if((fpr=_wfopen(ZDKfilename ,L"r"))!=NULL)
		{
			fwscanf(fpr,L"%d",&RoadArr[i].NZDK);
			if(RoadArr[i].NZDK>0)
			{
				RoadArr[i].pZDK = new ZDKDAT[RoadArr[i].NZDK];
				for(j=0;j<RoadArr[i].NZDK;j++)
					fwscanf(fpr ,L"%lf%lf%d%lf%lf",&RoadArr[i].pZDK[j].cml,&dml,&RoadArr[i].pZDK[j].IsInsertHdm,&RoadArr[i].pZDK[j].LZDK,&RoadArr[i].pZDK[j].RZDK);
			}
			fclose(fpr);
		}	
	}
	if(RoadArr[0].NZDK>0)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		m_StartDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[0].cml));
		m_EndDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[RoadArr[0].NZDK-1].cml));
	}
	else
		GetDlgItem(IDOK)->EnableWindow(false);

	UpdateData(FALSE);		
}

void CCalYDArea::OnButtonpickxl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	acedGetAcadDwgView()->SetFocus();
	ShowWindow(SW_HIDE);		// Hide our dialog

	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml=-100;

	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	rc = ads_entsel(L"\nSelect a  线路 : ", en, asDblArray(PT));
	if (rc != RTNORM) 
	{    
		ads_alert(L"所选实体非线路实体!");
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		EnableWindow(TRUE); // Enable our dialog
		acDocManager->unlockDocument(acDocManager->curDocument());
		return;   
	}
	acdbGetObjectId(eId, en);

	AcDbObject *pObj;
	JD_CENTER *pm;
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}

	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{
		//20190722	改用cast
		pm = JD_CENTER::cast(pObj);
		m_mdbname = pm->mdbname;
	}
	else
	{
		CString roadname;
		pObj->close();
		DBS.GetXLXdata(eId,m_mdbname,roadname);
	}		 

	FILE *fpr;
	CString ZDKfilename;
	int DorS;
	double dml;
	CString str;
	str=m_mdbname;
	str.MakeUpper();
	int pos,len,j;
	pos = str.Find(L"DATA");
	len = str.GetLength();
	str=str.Right(len-pos-5);
	str.Replace(L".MDB",L"");

	m_RoadName = "主线";
	DBS.ReadAllRoadName(m_mdbname);
	NRoad = DBS.m_RoadSum;		
	for(int i=0;i<DBS.m_RoadSum;i++)
	{
		RoadArr[i].NZDK=0;
		if(RoadArr[i].pm)
		{ 
			delete RoadArr[i].pm;
			RoadArr[i].pm = NULL;
		}
		if(RoadArr[i].pZDK)
		{
			delete RoadArr[i].pZDK;
			RoadArr[i].pZDK=NULL;
		}
		DBS.Read_XLDbs(m_mdbname,L"线元表",DBS.Road[i].RoadName);		
		DBS.Read_XLDbs(m_mdbname,L"断链表",DBS.Road[i].RoadName);
		if(DBS.XYNum>0)
		{
			RoadArr[i].pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				RoadArr[i].pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(RoadArr[i].pm->mdbname,m_mdbname);
			_tcscpy(RoadArr[i].pm->RoadName,DBS.Road[i].RoadName);
		}
		ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + str + "." + DBS.Road[i].RoadName;
		if((fpr=_wfopen(ZDKfilename ,L"r"))!=NULL)
		{
			fwscanf(fpr,L"%d",&RoadArr[i].NZDK);
			if(RoadArr[i].NZDK>0)
			{
				RoadArr[i].pZDK = new ZDKDAT[RoadArr[i].NZDK];
				for(j=0;j<RoadArr[i].NZDK;j++)
					fwscanf(fpr ,L"%lf%lf%d%lf%lf",&RoadArr[i].pZDK[j].cml,&dml,&RoadArr[i].pZDK[j].IsInsertHdm,&RoadArr[i].pZDK[j].LZDK,&RoadArr[i].pZDK[j].RZDK);
			}
			fclose(fpr);
		}	
	}
	if(RoadArr[0].NZDK>0)
	{
		GetDlgItem(IDOK)->EnableWindow(true);
		m_StartDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[0].cml));
		m_EndDml.Format(L"%0.3lf",RoadArr[0].pm->XLC1(RoadArr[0].pZDK[RoadArr[0].NZDK-1].cml));
	}
	else
	{
		GetDlgItem(IDOK)->EnableWindow(false);
	}
	pm->close();

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog
	acDocManager->unlockDocument(acDocManager->curDocument());	
	UpdateData(FALSE);
}

void CCalYDArea::OnSelchangeComboYdroadname() 
{
	// TODO: Add your control notification handler code here
	/*
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i,m_RoadName);
	//	DBS.Read_XLDbs(m_mdbname,L"控制点表",m_RoadName);
	DBS.Read_XLDbs(m_mdbname,L"线元表",m_RoadName);		
	DBS.Read_XLDbs(m_mdbname,L"断链表",m_RoadName);
	if(DBS.XYNum>0)
	{
	if(pm && m_mode==0)
	delete pm;
	//		pm=new JD_CENTER;//主线对象
	//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
	pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
	if(DBS.NDL>0)
	pm->setDLB(DBS.DLArray,DBS.NDL);
	m_mode = 0;
	}
	_tcscpy(pm->mdbname,m_mdbname);
	_tcscpy(pm->RoadName,m_RoadName);

	FILE *fpr;
	CString ZDKfilename;
	int DorS;
	double dml;
	CString str;
	str=pm->mdbname;
	str.MakeUpper();
	int pos,len;
	pos = str.Find(L"DATA");
	len = str.GetLength();
	str=str.Right(len-pos-5);
	str.Replace(L".MDB",L"");
	ZDKfilename =Cworkdir + "\\data\\" + "hdmZDK." + str + "." + pm->RoadName;
	if((fpr=_wfopen(ZDKfilename ,L"r"))==NULL)
	{
	CString mes;
	mes.Format(L"无法打开%s文件!",ZDKfilename);
	ads_alert(mes);
	UpdateData(FALSE);	
	return ;
	}	
	fwscanf(fpr,L"%d",&NZDK);	
	if(NZDK>0)
	{
	if(pZDK) delete pZDK;
	pZDK = new ZDKDAT[NZDK];
	for(i=0;i<NZDK;i++)
	fwscanf(fpr ,L"%lf%lf%d%lf%lf",&pZDK[i].cml,&dml,&DorS,&pZDK[i].LZDK,&pZDK[i].RZDK);
	m_StartDml.Format(L"%0.3lf",pm->XLC1(pZDK[0].cml));
	m_EndDml.Format(L"%0.3lf",pm->XLC1(pZDK[NZDK-1].cml));
	}
	fclose(fpr);


	UpdateData(FALSE);		*/

}

void CCalYDArea::OnOK() 
{
	// TODO: Add extra validation here	
	UpdateData();

	if(RoadArr[0].pm)
	{
		double szydk,syydk,ezydk,eyydk,sml,eml;
		int i,sno=-1,eno=-1,iroad,ii;
		double zxsml = RoadArr[0].pm->TYLC(_wtof(m_StartDml));
		double zxeml = RoadArr[0].pm->TYLC(_wtof(m_EndDml));
		if(zxsml>zxeml+0.0001)
		{
			ads_alert(L"终止里程应大于起始里程!");
			return;
		}
		if(zxsml<RoadArr[0].pZDK[0].cml-0.01||zxeml>RoadArr[0].pZDK[RoadArr[0].NZDK-1].cml+0.01)
		{
			ads_alert(L"里程出界!");
			return;
		}
		double SmlZx,EmlZx;
		SmlZx = zxsml, EmlZx = zxeml;
		RoadArr[0].pm->FormLCB(1000,SmlZx,EmlZx);

		CString skml,ekml;
		tab.pYdByKm = new OutTAB::YdByKm[RoadArr[0].pm->NLCB-1];
		tab.NYdByKm=0;
		CString lcfw;
		for(ii=0;ii<RoadArr[0].pm->NLCB-1;ii++)//按公里统计用地面积
		{
			zxsml = RoadArr[0].pm->pLCB[ii].lc;
			zxeml = RoadArr[0].pm->pLCB[ii+1].lc;
			skml = RoadArr[0].pm->XLC(zxsml);
			ekml = RoadArr[0].pm->XLC(zxeml);
			lcfw =skml+"~"+ekml;
			_tcscpy(tab.pYdByKm[tab.NYdByKm].lcfw,lcfw);
			double zarea=0.0,yarea=0.0;
			BAS_DRAW_FUN::xlpoint SZxP,EZxP;
			SZxP.lc = zxsml;
			RoadArr[0].pm->xlpoint_pz(&SZxP);
			EZxP.lc = zxeml;
			RoadArr[0].pm->xlpoint_pz(&EZxP);
			for(iroad=0;iroad<NRoad;iroad++)
			{
				if(iroad==0)//是主线
				{
					sml = zxsml;
					eml = zxeml;
				}
				else//分离段
				{
					double prjsml,prjeml;

					prjsml = RoadArr[0].pm->PROJ_ML(RoadArr[iroad].pm->XYArray[1][8],RoadArr[iroad].pm->XYArray[1][9]);
					prjeml = RoadArr[0].pm->PROJ_ML(RoadArr[iroad].pm->XYArray[RoadArr[iroad].pm->XYNum][8],RoadArr[iroad].pm->XYArray[RoadArr[iroad].pm->XYNum][9]);

					if(prjsml>zxeml || prjeml<zxsml)//分离段在主线统计里程范围外
						continue;
					else
					{
						if(prjsml>=zxsml&&prjeml<=zxeml)//分离段全在主线统计里程范围
						{
							sml = RoadArr[iroad].pZDK[0].cml; 
							eml = RoadArr[iroad].pZDK[RoadArr[iroad].NZDK-1].cml;
						}
						else if(prjsml<zxsml&&prjeml<=zxeml)//分离段起端在范围外，终端在范围内
						{
							sml = RoadArr[iroad].pm->PROJ_ML(SZxP.x,SZxP.y);
							eml = RoadArr[iroad].pZDK[RoadArr[iroad].NZDK-1].cml;
						}
						else if(prjsml>=zxsml&&prjeml>zxeml)//分离段终端在范围外，起端在范围内
						{
							sml = RoadArr[iroad].pZDK[0].cml; 
							eml = RoadArr[iroad].pm->PROJ_ML(EZxP.x,EZxP.y);
						}
						else
						{
							sml = eml =0;
						}
					}
				}
				sno=-1,eno=-1;
				for(i=0;i<RoadArr[iroad].NZDK;i++)
				{
					if(RoadArr[iroad].pZDK[i].cml>=sml-0.01)
					{
						sno = i;
						if(i < RoadArr[iroad].NZDK && i>0)
						{
							szydk =  RoadArr[iroad].pZDK[i-1].LZDK + (sml-RoadArr[iroad].pZDK[i-1].cml)*
								(RoadArr[iroad].pZDK[i].LZDK - RoadArr[iroad].pZDK[i-1].LZDK)/
								(RoadArr[iroad].pZDK[i].cml - RoadArr[iroad].pZDK[i-1].cml);
							syydk =  RoadArr[iroad].pZDK[i-1].RZDK + (sml-RoadArr[iroad].pZDK[i-1].cml)*
								(RoadArr[iroad].pZDK[i].RZDK - RoadArr[iroad].pZDK[i-1].RZDK)/
								(RoadArr[iroad].pZDK[i].cml - RoadArr[iroad].pZDK[i-1].cml);
						}
						else if(i==0)
						{
							szydk =  RoadArr[iroad].pZDK[0].LZDK;
							syydk =  RoadArr[iroad].pZDK[0].RZDK;
						}
						break;
					}
				}

				for(i=0;i<RoadArr[iroad].NZDK;i++)
				{
					if(RoadArr[iroad].pZDK[i].cml>=eml-0.01)
					{
						eno = i;
						if(i < RoadArr[iroad].NZDK && i>0)
						{
							ezydk =  RoadArr[iroad].pZDK[i-1].LZDK + (eml - RoadArr[iroad].pZDK[i-1].cml)*
								(RoadArr[iroad].pZDK[i].LZDK - RoadArr[iroad].pZDK[i-1].LZDK)/
								(RoadArr[iroad].pZDK[i].cml - RoadArr[iroad].pZDK[i-1].cml);
							eyydk =  RoadArr[iroad].pZDK[i-1].RZDK + (eml - RoadArr[iroad].pZDK[i-1].cml)*
								(RoadArr[iroad].pZDK[i].RZDK - RoadArr[iroad].pZDK[i-1].RZDK)/
								(RoadArr[iroad].pZDK[i].cml - RoadArr[iroad].pZDK[i-1].cml);
						}
						break;
					}
				}

				if(sno>=0 && eno>=0)
				{	
					for(i=sno;i<eno-1;i++)
					{
						zarea +=0.5*(RoadArr[iroad].pZDK[i].LZDK+RoadArr[iroad].pZDK[i+1].LZDK)*
							(RoadArr[iroad].pZDK[i+1].cml-RoadArr[iroad].pZDK[i].cml);
						yarea +=0.5*(RoadArr[iroad].pZDK[i].RZDK+RoadArr[iroad].pZDK[i+1].RZDK)*
							(RoadArr[iroad].pZDK[i+1].cml-RoadArr[iroad].pZDK[i].cml);
					}
					if(eno>sno)
					{
						zarea += 0.5*(szydk+RoadArr[iroad].pZDK[sno].LZDK)*(RoadArr[iroad].pZDK[sno].cml-sml);
						zarea += 0.5*(ezydk+RoadArr[iroad].pZDK[eno-1].LZDK)*(eml-RoadArr[iroad].pZDK[eno-1].cml);
						yarea += 0.5*(syydk+RoadArr[iroad].pZDK[sno].RZDK)*(RoadArr[iroad].pZDK[sno].cml-sml);
						yarea += 0.5*(eyydk+RoadArr[iroad].pZDK[eno-1].RZDK)*(eml-RoadArr[iroad].pZDK[eno-1].cml);
					}
					else
					{
						zarea += 0.5*(szydk+ezydk)*(eml-sml);
						yarea += 0.5*(syydk+eyydk)*(eml-sml);
					}

				}
			}	  	 
			tab.pYdByKm[tab.NYdByKm].zarea = zarea;
			tab.pYdByKm[tab.NYdByKm].yarea = yarea;	 
			tab.NYdByKm++;
		}


		double YdAll=0;
		for(ii=0;ii<tab.NYdByKm;ii++)
		{
			YdAll+= tab.pYdByKm[ii].zarea;
			YdAll+= tab.pYdByKm[ii].yarea;
		}
		m_YdArea.Format(L"%0.3lf",YdAll);
		UpdateData(false);
		if(m_outbykm) 
			tab.OUT_YDBYKM();

	}
	//	CDialog::OnOK();
}
