// ZBTOLC.cpp : implementation file
//

#include "stdafx.h"
#include "ZBTOLC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZBTOLC dialog


ZBTOLC::ZBTOLC(CWnd* pParent /*=NULL*/)
	: CDialog(ZBTOLC::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZBTOLC)
	m_infdrawlc = FALSE;
	m_RoadName = _T("");
	m_mdbname = _T("");
	m_Xcor = _T("0.0");
	m_Ycor = _T("0.0");
	m_CKML = _T("");
	//}}AFX_DATA_INIT
}


void ZBTOLC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZBTOLC)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_Check(pDX, IDC_CHECK_DRAWLC, m_infdrawlc);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_mdbname);
	DDX_Text(pDX, IDC_EDIT_XZB, m_Xcor);
	DDX_Text(pDX, IDC_EDIT_YZB, m_Ycor);
	DDX_Text(pDX, IDC_STATIC_CKML, m_CKML);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ZBTOLC, CDialog)
	//{{AFX_MSG_MAP(ZBTOLC)
	ON_BN_CLICKED(IDC_BUTTON_ZBBROWSER, OnButtonZbbrowser)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL, OnButtonPickxl)
	ON_BN_CLICKED(IDC_BUTTON_PickPT, OnBUTTONPickPT)
	ON_BN_CLICKED(IDC_CHECK_DRAWLC, OnCheckDrawlc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZBTOLC message handlers

void ZBTOLC::OnButtonZbbrowser() 
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

void ZBTOLC::OnSelchangeCOMBORoadName() 
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
		m_mode = 0;
	}
	_tcscpy(pm->mdbname,m_mdbname);
	_tcscpy(pm->RoadName,m_RoadName);
	UpdateData(false);	
}

void ZBTOLC::OnButtonPickxl() 
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
	/*acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	pObj->close();
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());
	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{   
		/*acdbOpenObject(pm, eId, AcDb::kForRead);*/
		/*if(acdbOpenObject(pm, eId, AcDb::kForRead)!=Acad::eOk)
		{
		ads_printf(L"打开实体失败！\n");
		return;
		}*/
		pm = JD_CENTER::cast(pObj);
		pm->close();
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

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog
	m_mode = 1;
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}

void ZBTOLC::OnBUTTONPickPT() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	acedGetAcadDwgView()->SetFocus();
	ShowWindow(SW_HIDE);		// Hide our dialog
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	ads_point pt;

	if(pm==NULL)
	{
		ads_alert(L"请先选定平面方案或实体!");
		goto L10;
	}
	if(RTNORM==acedGetPoint(NULL,L"\n拾取一点：",pt))
	{
		m_Xcor.Format(L"%0.3lf",pt[Y]);
		m_Ycor.Format(L"%0.3lf",pt[X]);

		char GH[20],xclc[80];
		double xlc,cml;

		cml=pm->PROJ_ML(pt[Y],pt[X]);		
		m_CKML = pm->XLC(cml);
		UpdateData(FALSE);		
	}
L10:	
	acutPrintf(L"\n命令:");
	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog	
}

void ZBTOLC::OnOK() 
{
	// TODO: Add extra validation here
	double XX,YY;
	UpdateData();
	XX = _wtof(m_Xcor);
	YY = _wtof(m_Ycor);
	char GH[20],xclc[80];
	double xlc,cml;
	if(pm)
	{
		cml=pm->PROJ_ML(XX,YY);		
		m_CKML = pm->XLC(cml);
		UpdateData(FALSE);
		if(m_infdrawlc)//在图上标注里程
		{
			ShowWindow(SW_HIDE);		// Hide our dialog
			acedGetAcadDwgView()->SetFocus();
			acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
			pm->INSertLC(cml);
			acDocManager->unlockDocument(acDocManager->curDocument());
			ShowWindow(SW_SHOW); // Display our dialog again
			SetFocus(); // Reset the focus back to ourselves
			EnableWindow(TRUE); // Enable our dialog	
		}
	}
	return;
	//	CDialog::OnOK();
}

BOOL ZBTOLC::OnInitDialog() 
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

void ZBTOLC::OnCheckDrawlc() 
{
	// TODO: Add your control notification handler code here
	m_infdrawlc = !m_infdrawlc;
	UpdateData();
	UpdateData(false);
}
