// CXLMBG.cpp : implementation file
//

#include "stdafx.h"
#include "CXLMBG.h"
#include "DrawPM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCXLMBG dialog


CCXLMBG::CCXLMBG(CWnd* pParent /*=NULL*/)
	: CDialog(CCXLMBG::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCXLMBG)
	m_lmbgxzb = _T("");
	m_lmbgyzb = _T("");
	m_desh1 = _T("");
	m_desh2 = _T("");
	m_dist1 = _T("");
	m_dist2 = _T("");
	m_lmbg1 = _T("");
	m_lmbg2 = _T("");
	m_lmbgc = _T("");
	m_lmhp1 = _T("");
	m_lmhp2 = _T("");
	m_prjckml1 = _T("");
	m_prjckml2 = _T("");
	m_xl1 = _T("");
	m_xl2 = _T("");
	//}}AFX_DATA_INIT
}


void CCXLMBG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCXLMBG)


	DDX_Text(pDX, IDC_EDIT_LMBGXZB, m_lmbgxzb);
	DDX_Text(pDX, IDC_EDIT_LMBGYZB, m_lmbgyzb);
	DDX_Text(pDX, IDC_STATIC_DESH1, m_desh1);
	DDX_Text(pDX, IDC_STATIC_DESH2, m_desh2);
	DDX_Text(pDX, IDC_STATIC_DIST1, m_dist1);
	DDX_Text(pDX, IDC_STATIC_DIST2, m_dist2);
	DDX_Text(pDX, IDC_STATIC_LMBG1, m_lmbg1);
	DDX_Text(pDX, IDC_STATIC_LMBG2, m_lmbg2);
	DDX_Text(pDX, IDC_STATIC_LMBGC, m_lmbgc);
	DDX_Text(pDX, IDC_STATIC_LMHP1, m_lmhp1);
	DDX_Text(pDX, IDC_STATIC_LMHP2, m_lmhp2);
	DDX_Text(pDX, IDC_STATIC_PRJCKML1, m_prjckml1);
	DDX_Text(pDX, IDC_STATIC_PRJCKML2, m_prjckml2);
	DDX_Text(pDX, IDC_STATIC_XL1, m_xl1);
	DDX_Text(pDX, IDC_STATIC_XL2, m_xl2);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCXLMBG, CDialog)
	//{{AFX_MSG_MAP(CCXLMBG)
	ON_BN_CLICKED(IDC_BUTTON_LMBGPickPT, OnBUTTONLMBGPickPT)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL1, OnButtonPickxl1)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL2, OnButtonPickxl2)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCXLMBG::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCXLMBG message handlers

void CCXLMBG::OnBUTTONLMBGPickPT() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	acedGetAcadDwgView()->SetFocus();
	ShowWindow(SW_HIDE);		// Hide our dialog
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	ads_point pt;


	if(RTNORM==acedGetPoint(NULL,L"\n拾取一点：",pt))
	{
		m_lmbgxzb.Format(L"%0.3lf",pt[Y]);
		m_lmbgyzb.Format(L"%0.3lf",pt[X]);

		UpdateData(FALSE);		
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog		
}

void CCXLMBG::OnButtonPickxl1() 
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
	/* acdbOpenObject(pObj, eId, AcDb::kForRead);*/
	if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		return;
	}
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{
		pObj->close();	   
		/*acdbOpenObject(pm1, eId, AcDb::kForRead);	*/
		if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pm1=JD_CENTER::cast(pObj);
		mdbname = pm1->mdbname;
		RoadName = pm1->RoadName;
		pm1->close();
	}
	else
	{
		pObj->close();
		DBS.GetXLXdata(eId,mdbname,RoadName);
		if(mdbname!=""&&RoadName!="")
		{
			//			DBS.Read_XLDbs(m_mdbname,"控制点表",m_RoadName);
			DBS.Read_XLDbs(mdbname,L"线元表",RoadName);					
			DBS.Read_XLDbs(mdbname,L"断链表",RoadName);
			if(DBS.XYNum>0)
			{
				//				pm=new JD_CENTER;//主线对象
				//				pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				pm1=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				if(DBS.NDL>0)
					pm1->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(pm1->mdbname,mdbname);
				_tcscpy(pm1->RoadName,RoadName);   
			}
		}		
	}	

	mdbname.MakeUpper();
	int pos,len;
	pos = mdbname.Find(L"DATA");
	len = mdbname.GetLength();
	mdbname=mdbname.Right(len-pos-5);
	m_xl1 = mdbname+L","+RoadName;
	UpdateData(FALSE);		
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;  // return TRUE unless you set the focus to a	
}

void CCXLMBG::OnButtonPickxl2() 
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
	//ads_printf(L"\nAcDbEntity name=%s",pObj->isA()->name());

	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{
		pObj->close();	   
		/*acdbOpenObject(pm2, eId, AcDb::kForRead);	*/
		if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pm2=JD_CENTER::cast(pObj);
		mdbname = pm2->mdbname;
		RoadName = pm2->RoadName;
		pm2->close();
	}
	else
	{
		pObj->close();
		DBS.GetXLXdata(eId,mdbname,RoadName);
		if(mdbname!=""&&RoadName!="")
		{
			//			DBS.Read_XLDbs(m_mdbname,"控制点表",m_RoadName);
			DBS.Read_XLDbs(mdbname,L"线元表",RoadName);					
			DBS.Read_XLDbs(mdbname,L"断链表",RoadName);
			if(DBS.XYNum>0)
			{
				//				pm=new JD_CENTER;//主线对象
				//				pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				pm2=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				if(DBS.NDL>0)
					pm2->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(pm2->mdbname,mdbname);
				_tcscpy(pm2->RoadName,RoadName);   
			}
		}		
	}	

	mdbname.MakeUpper();
	int pos,len;
	pos = mdbname.Find(L"DATA");
	len = mdbname.GetLength();
	mdbname=mdbname.Right(len-pos-5);
	m_xl2 = mdbname+L","+RoadName;
	UpdateData(FALSE);		
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;  // return TRUE unless you set the focus to a		
}

BOOL CCXLMBG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	pm1=NULL;
	pm2=NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCXLMBG::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	if(m_lmbgxzb==""||m_lmbgyzb=="")
	{
		ads_alert(L"请先选定坐标!");
		return;
	}
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	CDrawPM drawpm;
	double N,E,prjml,ZJ,Desh,lmbg,lmhp;
	if(pm1)
	{
		drawpm.pm = pm1;
		N = _wtof(m_lmbgxzb);
		E = _wtof(m_lmbgyzb);
		drawpm.CalLmht(N,E,prjml,ZJ,Desh,lmhp,lmbg);
		m_desh1.Format(L"%0.3lf",Desh);
		m_prjckml1=pm1->XLC(prjml);
		m_dist1.Format(L"%0.3lf",ZJ);
		m_lmbg1.Format(L"%0.3lf",lmbg);
		m_lmhp1.Format(L"%0.3lf",lmhp);
	}
	if(pm2)
	{
		drawpm.pm = pm2;
		N = _wtof(m_lmbgxzb);
		E = _wtof(m_lmbgyzb);
		drawpm.CalLmht(N,E,prjml,ZJ,Desh,lmhp,lmbg);
		m_desh1.Format(L"%0.3lf",Desh);
		m_prjckml2=pm2->XLC(prjml);
		m_dist2.Format(L"%0.3lf",ZJ);
		m_lmbg2.Format(L"%0.3lf",lmbg);
		m_lmhp2.Format(L"%0.3lf",lmhp);
	}
	if(pm1&&pm2)
		m_lmbgc.Format(L"%0.3lf",_wtof(m_lmbg1)-_wtof(m_lmbg2));

	UpdateData(false);
	acDocManager->unlockDocument(acDocManager->curDocument());
	//	CDialog::OnOK();
}


void CCXLMBG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
