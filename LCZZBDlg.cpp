// LCZZBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LCZZBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CLCZZBDlg dialog

CLCZZBDlg::CLCZZBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLCZZBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLCZZBDlg)
	m_XCLC = _T("");
	m_ENDCKML = _T("");
	m_STARTCKML = _T("");
	m_ROAD = _T("");
	mdbname = _T("");
	m_SPACER = 20.0;
	pm = NULL;
	m_mode = 0;
	m_infdrawlc = FALSE;
	//}}AFX_DATA_INIT
}


void CLCZZBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLCZZBDlg)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_ROAD);
	DDX_Text(pDX, IDC_EDIT_XCLC, m_XCLC);
	DDX_Text(pDX, IDC_EDITENDCKML, m_ENDCKML);
	DDX_Text(pDX, IDC_EDITSTARTCKML, m_STARTCKML);
	DDX_Text(pDX, IDC_EDITSPACER, m_SPACER);
	DDX_Text(pDX,IDC_EDIT_MDBNAME,mdbname);
	DDX_Check(pDX, IDC_CHECK_BZLC, m_infdrawlc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLCZZBDlg, CDialog)
	//{{AFX_MSG_MAP(CLCZZBDlg)
	ON_BN_CLICKED(IDC_BUTTON_lczzb_MDB, OnButtonMdb)
	ON_BN_CLICKED(IDC_BUTTON_lczzb_Set, OnBUTTONSet)
	ON_BN_CLICKED(IDC_BUTTON_GETPOINT, OnButtonGetpoint)
	ON_BN_CLICKED(IDC_RADIOSINGLE, OnRadiosingle)
	ON_BN_CLICKED(IDC_RADIOPLOUT, OnRadioplout)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_CHECK_BZLC, OnCheckDrawlc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCZZBDlg message handlers

void CLCZZBDlg::OnButtonMdb() 
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
	mdbname=fname;
	if(mdbname==""){AfxMessageBox(L"未指定数据库!");return;}

	//	DBS.Read_XLDbs(mdbname,L"控制点表",L"主线");
	DBS.Read_XLDbs(mdbname,L"线元表",L"主线");			
	DBS.Read_XLDbs(mdbname,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		if(pm && m_mode==0)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		m_mode = 0;
		_tcscpy(pm->mdbname,mdbname);
		_tcscpy(pm->RoadName,L"主线");
	}
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(mdbname);
	int i;

	m_RoadName.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadName.AddString(DBS.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_STARTCKML = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_ENDCKML = str;

	UpdateData(false);	
}

void CLCZZBDlg::OnBUTTONSet() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

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
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
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
		pm = JD_CENTER::cast(pObj);
		pm->close();
		GTZXId = eId;
	}
	else
	{
		pObj->close();
		DBS.GetXLXdata(eId,mdbname,m_ROAD);
		if(mdbname!=""&&m_ROAD!="")
		{
			//			DBS.Read_XLDbs(mdbname,L"控制点表",m_ROAD);	
			DBS.Read_XLDbs(mdbname,L"线元表",m_ROAD);					
			DBS.Read_XLDbs(mdbname,L"断链表",m_ROAD);
			if(DBS.XYNum>0)
			{
				pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//				pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					pm->setDLB(DBS.DLArray,DBS.NDL);
				_tcscpy(pm->mdbname,mdbname);
				_tcscpy(pm->RoadName,m_ROAD);   
			}
			GTZXId = eId;
		}		
	}	

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog
	m_mode = 1;
	acDocManager->unlockDocument(acDocManager->curDocument());
	return ;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE	
}

void CLCZZBDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);	
	BAS_DRAW_FUN::xlpoint PZ;
	PZ.x=0;PZ.y=0;
	LCZB plcb;

	ACHAR GH[20],xclc[80];
	double xlc;
	if(SingleorPL==0)//单个
	{
		if(m_XCLC == L"")
			return;
		if(pm!=NULL)
		{			
			xlc = _wtof(m_XCLC);
			PZ.lc=pm->TYLC(xlc);
			pm->xlpoint_pz(&PZ);
			if(m_infdrawlc)//在图上标注里程
			{
				ShowWindow(SW_HIDE);		// Hide our dialog
				acedGetAcadDwgView()->SetFocus();
				acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
				pm->INSertLC(PZ.lc);
				acDocManager->unlockDocument(acDocManager->curDocument());
				ShowWindow(SW_SHOW); // Display our dialog again
				SetFocus(); // Reset the focus back to ourselves
				EnableWindow(TRUE); // Enable our dialog	
			}
		}

		CString NStr,EStr;	
		NStr.Format(L"N:%.4lf",PZ.x);
		EStr.Format(L"E:%.4lf",PZ.y);
		GetDlgItem(IDC_STATIC_ZBN)->SetWindowText(NStr);
		GetDlgItem(IDC_STATIC_ZBE)->SetWindowText(EStr);
	}
	else//批量
	{
		CString fpath;
		fpath.Format(L"%s\\lib\\里程坐标模板.xlt",SysPath);
		CFileFind finder;
		if(finder.FindFile(fpath)==false)
		{
			fpath.Format(L"%s\\lib\\里程坐标模板.xlt不存在,无法输出表格!",SysPath);
			AfxMessageBox(fpath);
			return;
		}

		if(m_STARTCKML == L"" || m_ENDCKML == L"" )
			return;
		if(m_SPACER < 1.0)
			m_SPACER = 1.0;

		LCZBArr.RemoveAll();
		NLCZB = 0;
		ACHAR ckml[80];
		double sml,eml,cml,dml; 
		CString gh;

		if(pm!=NULL)
		{			
			sml=pm->TYLC(_wtof(m_STARTCKML));
			eml=pm->TYLC(_wtof(m_ENDCKML));
			if(int((eml-sml)/m_SPACER) > 9990)
				m_SPACER = floor((eml-sml)/10000);
			cml = sml;
			while(cml < eml) 
			{
				PZ.lc = cml;
				pm->xlpoint_pz(&PZ);
				plcb.x = PZ.x; 
				plcb.y = PZ.y; 
				_tcscpy(plcb.Ckml, pm->XLC(cml));	
				LCZBArr.Add(plcb);
				NLCZB++;
				cml += m_SPACER;
			}
			PZ.lc = eml;
			pm->xlpoint_pz(&PZ);
			_tcscpy(plcb.Ckml, pm->XLC(_wtof(m_ENDCKML)));		
			plcb.x = PZ.x;
			plcb.y = PZ.y;
			LCZBArr.Add(plcb);
			NLCZB++;
		}

		FillData();
	}
	//CDialog::OnOK();
}

void CLCZZBDlg::FillData()
{
	FILE *fpr;
	ACHAR filepath[180];	
	CWnd* pWnd=CWnd::FindWindow(L"XLMAIN",NULL);
	m_app.SetUserControl(TRUE);
	if(pWnd!=NULL)
	{
		TRACE(L"Excel window found\n");
		pWnd->ShowWindow(SW_SHOWMAXIMIZED);
		pWnd->UpdateWindow();
		pWnd->BringWindowToTop();
	}

	if (!m_app.CreateDispatch(L"Excel.Application",NULL)) 
	{ 
		AfxMessageBox(L"创建Excel服务失败!"); 
		return ; 
	} 
	m_workbooks.AttachDispatch(m_app.GetWorkbooks(),true);
	CString fpath;
	fpath.Format(L"%s\\lib\\里程坐标模板.xlt",SysPath);
	//	fpath="C:\\lib\\方案比较模板.xlt";
	m_app.SetVisible(true);
	m_app.SetUserControl(true);   //调用excel表
	/*
	LPDISPATCH pWorkbook = NULL;
	VERIFY(pWorkbook=m_workbooks.Open(fpath));
	m_workbook.AttachDispatch(pWorkbook);*/   
	m_workbook.AttachDispatch(m_workbooks.Add(_variant_t(fpath)));
	m_worksheets.AttachDispatch(m_workbook.GetWorksheets(),true);
	LPDISPATCH pRange=NULL;
	Range m_range;
	LPDISPATCH pWorksheet=m_worksheets.GetItem(COleVariant((short)1));
	m_worksheet.AttachDispatch(pWorksheet);
	m_worksheet.Select();
	VERIFY(pRange = m_worksheet.GetRange(COleVariant(L"A4")));	  
	m_range.AttachDispatch(pRange);
	int i;
	CString str;

	for(i=0;i<NLCZB;i++)
	{
		str.Format(L"%s",LCZBArr[i].Ckml);
		WriteInExcel(m_range,i-1,1,_variant_t(str));
		str.Format(L"%0.3lf",LCZBArr[i].x);
		WriteInExcel(m_range,i-1,2,_variant_t(str));
		str.Format(L"%0.3lf",LCZBArr[i].y);
		WriteInExcel(m_range,i-1,3,_variant_t(str));
	}

	m_worksheet.Activate();
	m_range.ReleaseDispatch();
	m_worksheet.ReleaseDispatch();
}

//往EXCLE的CELL中写数据
bool CLCZZBDlg::WriteInExcel(Range rgMyRge,long Line,long Col,_variant_t Value)
{
	rgMyRge.SetItem(_variant_t(Line),_variant_t(Col),Value);
	return true;
}

BOOL CLCZZBDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	PtN=0.0;
	PtE=0.0;
	pm = NULL;

	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CDialog::OnCancel();
		return 0;
	}
	SingleorPL =0;
	CButton *Pbutton=(CButton *) GetDlgItem(IDC_RADIOSINGLE);
	Pbutton->SetCheck(1);
	GetDlgItem(IDC_EDITSTARTCKML)->EnableWindow(false);
	GetDlgItem(IDC_EDITENDCKML)->EnableWindow(false);
	GetDlgItem(IDC_EDITSPACER)->EnableWindow(false);

	mdbname = Cworkdir+"\\DATA\\";
	mdbname = mdbname + Pro+".mdb";

	//	DBS.Read_XLDbs(mdbname,L"控制点表",L"主线");
	DBS.Read_XLDbs(mdbname,L"线元表",L"主线");			
	DBS.Read_XLDbs(mdbname,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,mdbname);
		_tcscpy(pm->RoadName,L"主线");
	}
	else
	{
		CDialog::OnCancel();
		return 0;
	}
	m_ROAD = "主线";

	DBS.ReadAllRoadName(mdbname);
	for(int i=0;i<DBS.m_RoadSum;i++)
		m_RoadName.AddString(DBS.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_STARTCKML = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_ENDCKML = str;	
	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CLCZZBDlg::Release()
{
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	acDocManager->unlockDocument(acDocManager->curDocument());
}

void CLCZZBDlg::OnButtonGetpoint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	ads_point pt;

	if(pm==NULL)
	{
		ads_alert(L"请先选定平面方案或实体!");
		goto L10;
	}
	if(RTNORM==acedGetPoint(NULL,L"\n拾取一点：",pt))
	{
		PtN=pt[Y];
		PtE=pt[X];

		ACHAR GH[20],xclc[80];
		double xlc,cml;
		if(pm!=NULL)
		{
			cml=pm->PROJ_ML(PtN,PtE);
			xlc=pm->XLC1(cml);
		}

		m_XCLC.Format(L"%0.3lf",xlc);
		UpdateData(FALSE);

	}
L10:	
	acutPrintf(L"\n命令:");
	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog
}/////////////////////////////////////////////////////////////////////////////
// CLCZZBDlg message handlers

void CLCZZBDlg::OnRadiosingle() 
{
	// TODO: Add your control notification handler code here
	SingleorPL = 0;
	GetDlgItem(IDC_EDITSTARTCKML)->EnableWindow(false);
	GetDlgItem(IDC_EDITENDCKML)->EnableWindow(false);
	GetDlgItem(IDC_EDITSPACER)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_GETPOINT)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_XCLC)->EnableWindow(true);
}

void CLCZZBDlg::OnRadioplout() 
{
	// TODO: Add your control notification handler code here
	SingleorPL = 1;
	GetDlgItem(IDC_EDITSTARTCKML)->EnableWindow(true);
	GetDlgItem(IDC_EDITENDCKML)->EnableWindow(true);
	GetDlgItem(IDC_EDITSPACER)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_GETPOINT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_XCLC)->EnableWindow(false);

}

void CLCZZBDlg::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadName.GetCurSel();
	m_RoadName.GetLBText(i,m_ROAD);
	//	DBS.Read_XLDbs(mdbname,L"控制点表",m_ROAD);	
	DBS.Read_XLDbs(mdbname,L"线元表",m_ROAD);			
	DBS.Read_XLDbs(mdbname,L"断链表",m_ROAD);
	if(DBS.XYNum>0)
	{
		if(pm && m_mode==0)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		m_mode = 0;
		_tcscpy(pm->mdbname,mdbname);
		_tcscpy(pm->RoadName,m_ROAD);
	}
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_STARTCKML = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_ENDCKML = str;
	UpdateData(false);
}

void CLCZZBDlg::OnCheckDrawlc() 
{
	// TODO: Add your control notification handler code here
	m_infdrawlc = !m_infdrawlc;
	UpdateData();
	UpdateData(false);
}