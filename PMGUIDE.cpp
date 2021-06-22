// PMGUIDE.cpp : implementation file
//

#include "stdafx.h"
#include "PMGUIDE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PMGUIDE dialog
extern ads_point ZOOM_P1,ZOOM_P2;

PMGUIDE::PMGUIDE(CWnd* pParent /*=NULL*/)
	: CDialog(PMGUIDE::IDD, pParent)
{
	//{{AFX_DATA_INIT(PMGUIDE)
	m_RoadName = _T("");
	m_mdbname = _T("");
	m_startdml = _T("");
	m_enddml = _T("");
	m_ZdDml = _T("");
	//}}AFX_DATA_INIT
}


void PMGUIDE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PMGUIDE)
	DDX_Control(pDX, IDC_SLIDER_DML, m_sld);
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_mdbname);
	DDX_Text(pDX, IDC_STATIC_ENDDML, m_enddml);
	DDX_Text(pDX, IDC_STATIC_STARTDML, m_startdml);
	DDX_Text(pDX, IDC_EDIT_ZDDML, m_ZdDml);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PMGUIDE, CDialog)
	//{{AFX_MSG_MAP(PMGUIDE)
	ON_BN_CLICKED(IDC_BUTTON_ZDMDB, OnButtonZdmdb)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_BUTTON_PICKPMXL, OnButtonPickpmxl)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PMGUIDE message handlers

void PMGUIDE::OnButtonZdmdb() 
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

	//	DBS.Read_XLDbs(m_mdbname,L"控制点表",L"主线");
	DBS.Read_XLDbs(m_mdbname,L"线元表",L"主线");		
	DBS.Read_XLDbs(m_mdbname,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		if(pm && m_mode==0)
			delete pm;
		//		pm=new JD_CENTER;//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		m_ZdDml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_startdml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_enddml.Format(L"%0.3lf",pm->DLArray[pm->DLNum-1].BLC);
		m_mode = 0;
	}
	_tcscpy(pm->mdbname,m_mdbname);
	_tcscpy(pm->RoadName,L"主线");
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_mdbname);
	int i;

	m_RoadNameCrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	UpdateData(false);			
}

void PMGUIDE::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
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
		m_ZdDml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_startdml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_enddml.Format(L"%0.3lf",pm->DLArray[pm->DLNum-1].BLC);
		m_mode = 0;
	}
	_tcscpy(pm->mdbname,m_mdbname);
	_tcscpy(pm->RoadName,m_RoadName);
	UpdateData(false);		
}

BOOL PMGUIDE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	pm = NULL;
	m_mode = 0;

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CDialog::OnCancel();
		return 0;
	}

	m_mdbname = Cworkdir+"\\DATA\\";
	m_mdbname = m_mdbname + Pro+".mdb";

	//	DBS.Read_XLDbs(m_mdbname,L"控制点表",L"主线");
	DBS.Read_XLDbs(m_mdbname,L"线元表",L"主线");		
	DBS.Read_XLDbs(m_mdbname,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		//		pm=new JD_CENTER;//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		m_ZdDml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_startdml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_enddml.Format(L"%0.3lf",pm->DLArray[pm->DLNum-1].BLC);
	}
	else
	{
		CDialog::OnCancel();
		return 0;
	}
	m_RoadName = "主线";
	_tcscpy(pm->mdbname,m_mdbname);
	_tcscpy(pm->RoadName,L"主线");
	DBS.ReadAllRoadName(m_mdbname);
	for(int i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);

	UpdateData(FALSE);		
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PMGUIDE::OnButtonPickpmxl() 
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
	rc = ads_entsel(L"\n选择线路 : ", en, asDblArray(PT));
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
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{
		pObj->close();	   
		/*acdbOpenObject(pm, eId, AcDb::kForRead);*/
		pm = JD_CENTER::cast(pObj);
		//if(acdbOpenObject(pm, eId, AcDb::kForRead)!=Acad::eOk)
		//{
		//	ads_printf(L"打开实体失败！\n");
		//	return;
		//}
		//pm->close();
	}
	else
	{
		pObj->close();
		DBS.GetXLXdata(eId,m_mdbname,m_RoadName);
		if(m_mdbname!=""&&m_RoadName!="")
		{
			//			DBS.Read_XLDbs(m_mdbname,L"控制点表",m_RoadName);
			DBS.Read_XLDbs(m_mdbname,L"线元表",m_RoadName);					
			DBS.Read_XLDbs(m_mdbname,L"断链表",m_RoadName);
			if(DBS.XYNum>0)
			{
				//				pm=new JD_CENTER;//主线对象
				//				pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				if(DBS.NDL>0)
					pm->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(pm->mdbname,m_mdbname);
				_tcscpy(pm->RoadName,m_RoadName);   
			}
		}		
	}	
	if(pm)
	{
		m_ZdDml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_startdml.Format(L"%0.3lf",pm->DLArray[0].ELC);
		m_enddml.Format(L"%0.3lf",pm->DLArray[pm->DLNum-1].BLC);
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog
	m_mode = 1;
	acDocManager->unlockDocument(acDocManager->curDocument());	
}

void PMGUIDE::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	CSliderCtrl* pSlider = (CSliderCtrl*)pScrollBar;
	int flag;
	flag = pSlider->GetPos();
	double cml;
	cml = pm->DLArray[0].ELC+flag*pm->LenOfLine/100.0;
	m_ZdDml.Format(L"%0.3lf",pm->XLC1(cml));

	UpdateData(false);

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void PMGUIDE::OnOK() 
{
	// TODO: Add extra validation here
	double XX,YY,flag;
	UpdateData();
	BAS_DRAW_FUN::xlpoint PZ;
	if(pm)
	{
		PZ.lc = pm->TYLC(_wtof(m_ZdDml));
		pm->xlpoint_pz(&PZ);	
		ShowWindow(SW_HIDE);		// Hide our dialog
		acedGetAcadDwgView()->SetFocus();

		ZOOM_P1[Y]=PZ.x-100,ZOOM_P1[X]=PZ.y-100;
		ZOOM_P2[Y]=PZ.x+100,ZOOM_P2[X]=PZ.y+100;
		acedGetAcadDockCmdLine()->SetFocus();
		acDocManager->sendStringToExecute(acDocManager->curDocument(),L"PZOOM\n",0);	
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		EnableWindow(TRUE); // Enable our dialog
		flag = (PZ.lc-pm->DLArray[0].ELC)/pm->LenOfLine;

		m_sld.SetPos(flag*100.0);
		UpdateData(false);
	}	
	//	CDialog::OnOK();
}
