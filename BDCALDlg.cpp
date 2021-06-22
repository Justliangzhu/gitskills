// BDCALDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BDCALDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BDCALDlg dialog
extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;


BDCALDlg::BDCALDlg(CWnd* pParent /*=NULL*/): cdxCSizingDialog(BDCALDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BDCALDlg)
	// NOTE: the ClassWizard will add member initialization here
	NBIDUAN = 0;
	pm1=pm2=NULL;
	//}}AFX_DATA_INIT
}


void BDCALDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BDCALDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BDCALDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(BDCALDlg)
	ON_BN_CLICKED(IDC_BUTTON_SELXL, OnButtonSelxl)
	ON_BN_CLICKED(IDC_BUTTON_CALBD, OnButtonCalbd)
	ON_BN_CLICKED(IDC_BUTTON_DRAWBD, OnButtonDrawbd)
	ON_BN_CLICKED(IDC_BUTTON_ADDBD, OnButtonAddbd)
	ON_BN_CLICKED(IDC_BUTTON_DELBD, OnButtonDelbd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BDCALDlg message handlers

BOOL BDCALDlg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog();

	// TODO: Add extra initialization here	
	BAS_DRAW_FUN ob;
	int i=0;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		cdxCSizingDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir+"\\DATA\\";
	m_Dbs = m_Dbs + Pro+".mdb";
	FILE *fpr;
	CString fname;
	fname = Cworkdir+"\\DATA\\";
	fname = fname+"BDDATA";
	fpr = _wfopen(fname,L"r");
	ACHAR xl1[256],xl2[256];
	if(fpr!=NULL)
	{
		fwscanf(fpr,L"%d",&NBIDUAN);
		for(i=0;i<NBIDUAN;i++)
		{
			fwscanf(fpr,L"%s%s%d%d%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf%lf",xl1,xl2,&BDArr[i].ZDStOrEd,&BDArr[i].Lmhp,
				&BDArr[i].C1,&BDArr[i].C2,&BDArr[i].Z1,&BDArr[i].Z2,
				&BDArr[i].YljRNose,&BDArr[i].TljRNose,&BDArr[i].Yljdml1,
				&BDArr[i].Yljdml2,&BDArr[i].Tljdml1,&BDArr[i].Tljdml2,&BDArr[i].FldDml1,&BDArr[i].FldDml2);
			BDArr[i].XL1 = xl1;
			BDArr[i].XL2 = xl2;
			fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",&BDArr[i].YCSpt[0],&BDArr[i].YCSpt[1],
				&BDArr[i].YCEpt[0],&BDArr[i].YCEpt[1],&BDArr[i].YNosePt[0],&BDArr[i].YNosePt[1]); 
			fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",&BDArr[i].TCSpt[0],&BDArr[i].TCSpt[1],
				&BDArr[i].TCEpt[0],&BDArr[i].TCEpt[1],&BDArr[i].TNosePt[0],&BDArr[i].TNosePt[1]);
			fwscanf(fpr,L"%lf%lf%lf%lf",&BDArr[i].YLjPt1[0],&BDArr[i].YLjPt1[1],&BDArr[i].YLjPt2[0],&BDArr[i].YLjPt2[1]);
		}
		fclose(fpr);
	}
	else
	{
		DBS.ReadAllRoadName(m_Dbs);	
		NBIDUAN = 0;
		for(i=1;i<DBS.m_RoadSum;i++)
		{
			BDArr[NBIDUAN].XL1=m_Dbs+",主线";
			BDArr[NBIDUAN].XL2=m_Dbs+",L"+DBS.Road[i].RoadName;	
			BDArr[NBIDUAN].ZDStOrEd=0;
			BDArr[NBIDUAN].Lmhp = 1;
			BDArr[NBIDUAN].C1=BDArr[NBIDUAN].C2=0;
			BDArr[NBIDUAN].Z1=BDArr[NBIDUAN].Z2=0;
			BDArr[NBIDUAN].YljRNose=0.6;
			BDArr[NBIDUAN].TljRNose=1.5;
			BDArr[NBIDUAN].Yljdml1=BDArr[NBIDUAN].Yljdml2=0;
			BDArr[NBIDUAN].Tljdml1=BDArr[NBIDUAN].Tljdml2=0;
			NBIDUAN++;
			BDArr[NBIDUAN].XL1=m_Dbs+",主线";
			BDArr[NBIDUAN].XL2=m_Dbs+",L"+DBS.Road[i].RoadName;
			BDArr[NBIDUAN].Lmhp = 1;
			BDArr[NBIDUAN].ZDStOrEd=1;
			BDArr[NBIDUAN].C1=BDArr[NBIDUAN].C2=0;
			BDArr[NBIDUAN].Z1=BDArr[NBIDUAN].Z2=0;
			BDArr[NBIDUAN].YljRNose=0.6;
			BDArr[NBIDUAN].TljRNose=1.5;
			BDArr[NBIDUAN].Yljdml1=BDArr[NBIDUAN].Yljdml2=0;
			BDArr[NBIDUAN].Tljdml1=BDArr[NBIDUAN].Tljdml2=0;
			NBIDUAN++;
		}		
	}

	int j;
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=40;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	if(NBIDUAN<1)
		m_grid.SetRowCount(NBIDUAN+2);
	else
		m_grid.SetRowCount(NBIDUAN+1);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(17);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	//   m_grid.SetFixedBkColor(fixbkclr);

	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	Item.lfFont=font;

	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for(int i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}  
		//		   m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}
	m_grid.SetItemText(0,0,L"序 号");	   
	m_grid.SetItemText(0,1,L"主线");
	m_grid.SetItemText(0,2,L"匝道");
	m_grid.SetItemText(0,3,L"鼻端位置");
	m_grid.SetItemText(0,4,L"路面横坡");
	m_grid.SetItemText(0,5,L"偏置值1");
	m_grid.SetItemText(0,6,L"偏置值2");
	m_grid.SetItemText(0,7,L"过渡段长1");
	m_grid.SetItemText(0,8,L"过渡段长2");
	m_grid.SetItemText(0,9,L"硬路肩半径");	   
	m_grid.SetItemText(0,10,L"土路肩半径");
	m_grid.SetItemText(0,11,L"硬路肩里程1");
	m_grid.SetItemText(0,12,L"硬路肩里程2");
	m_grid.SetItemText(0,13,L"土路肩里程1");
	m_grid.SetItemText(0,14,L"土路肩里程2");
	m_grid.SetItemText(0,15,L"分流点里程1");
	m_grid.SetItemText(0,16,L"分流点里程2");

	ACHAR tmp[30];
	if(NBIDUAN > 0 )  	
	{
		for(i=0; i<NBIDUAN; i++)
		{
			m_grid.SetItemText(i+1,1,BDArr[i].XL1);
			m_grid.SetItemText(i+1,2,BDArr[i].XL2);
			if(BDArr[i].ZDStOrEd==0)
				m_grid.SetItemText(i+1,3,L"匝道起端");
			else
				m_grid.SetItemText(i+1,3,L"匝道终端");
			if(BDArr[i].Lmhp==1)
				m_grid.SetItemText(i+1,4,L"同坡");
			else
				m_grid.SetItemText(i+1,4,L"反坡");
			_stprintf(tmp,L"%0.3lf",BDArr[i].C1);
			m_grid.SetItemText(i+1,5,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].C2);
			m_grid.SetItemText(i+1,6,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Z1);
			m_grid.SetItemText(i+1,7,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Z2);
			m_grid.SetItemText(i+1,8,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].YljRNose);
			m_grid.SetItemText(i+1,9,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].TljRNose);
			m_grid.SetItemText(i+1,10,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Yljdml1);
			m_grid.SetItemText(i+1,11,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Yljdml2);
			m_grid.SetItemText(i+1,12,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Tljdml1);
			m_grid.SetItemText(i+1,13,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].Tljdml2);
			m_grid.SetItemText(i+1,14,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].FldDml1);
			m_grid.SetItemText(i+1,15,tmp);
			_stprintf(tmp,L"%0.3lf",BDArr[i].FldDml2);
			m_grid.SetItemText(i+1,16,tmp);

			_stprintf(tmp,L"%d",i+1);
			m_grid.SetItemText(i+1,0,tmp);

			for(j=11;j<m_grid.GetColumnCount();j++)
				m_grid.SetItemBkColour(i+1,j,RGB(113,189,193));	
		}
	}
	else
	{
		int nRow=1;
		m_grid.SetItemText(nRow, 4,L"同坡");
		m_grid.SetItemText(nRow, 3,L"匝道起端");
		m_grid.SetItemText(nRow, 5,L"0.0");
		m_grid.SetItemText(nRow, 6,L"0.0");
		m_grid.SetItemText(nRow, 7,L"0.0");
		m_grid.SetItemText(nRow, 8,L"0.0");
		m_grid.SetItemText(nRow, 9,L"0.6");
		m_grid.SetItemText(nRow, 10,L"1.5");
	}
	_stprintf(tmp,L"%d",i+1);
	m_grid.SetItemText(i+1,0,tmp);
	m_grid.SetColumnWidth(0,50);
	for(j=11;j<m_grid.GetColumnCount();j++)
		m_grid.SetItemBkColour(1,j,RGB(113,189,193));	

	for(i=1; i<3; i++)
		m_grid.SetColumnWidth(i,230);	
	for(i=3; i<6; i++)
		m_grid.SetColumnWidth(i,70);
	for(i=6; i<10; i++)
		m_grid.SetColumnWidth(i,80);	
	for(i=10; i<14; i++)
		m_grid.SetColumnWidth(i,100);	 	   
	for( i=0;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);

	AddSzControl(m_grid,mdResize,mdResize);	
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BDCALDlg::OnButtonSelxl() 
{
	// TODO: Add your control notification handler code here
	int nCol = m_grid.GetFocusCell().col;
	int nRow = m_grid.GetFocusCell().row;
	if(nCol>2)
		return;
	UpdateData(true);
	acedGetAcadDwgView()->SetFocus();
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;

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
	CString str;
	str = "";
	JD_CENTER *pm;
	if (pObj->isKindOf(JD_CENTER::desc())) //是GTZX实体,取数据
	{
		pObj->close();	   
		/*acdbOpenObject(pm, eId, AcDb::kForRead);*/
		if(acdbOpenObject(pObj, eId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pm=JD_CENTER::cast(pObj);
		str.Format(L"%s,%s",pm->mdbname,pm->RoadName);
		pm->close();
	}
	else
	{
		pObj->close();
		acutPrintf(L"所选实体不是线路实体!\n");
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	if(str!="")
		m_grid.SetItemText(nRow,nCol,str);
	m_grid.SetFocusCell(nRow,nCol+1);
	EnableWindow(TRUE); // Enable our dialog
	acDocManager->unlockDocument(acDocManager->curDocument());   	
}

void BDCALDlg::GetDataOnJM()
{
	UpdateData();
	int i;
	NBIDUAN = m_grid.GetRowCount()-1;
	for(i=1; i<=NBIDUAN; i++)
	{
		BDArr[i-1].XL1=m_grid.GetItemText(i, 1);
		BDArr[i-1].XL2=m_grid.GetItemText(i, 2);
		if(m_grid.GetItemText(i, 3)=="匝道起端")
			BDArr[i-1].ZDStOrEd = 0;
		else
			BDArr[i-1].ZDStOrEd = 1;
		if(m_grid.GetItemText(i, 4)=="同坡")
			BDArr[i-1].Lmhp = 1;
		else
			BDArr[i-1].Lmhp = -1;
		BDArr[i-1].C1=_wtof(m_grid.GetItemText(i, 5));
		BDArr[i-1].C2=_wtof(m_grid.GetItemText(i, 6));
		BDArr[i-1].Z1=_wtof(m_grid.GetItemText(i, 7));
		BDArr[i-1].Z2=_wtof(m_grid.GetItemText(i, 8));
		BDArr[i-1].YljRNose=_wtof(m_grid.GetItemText(i, 9));
		BDArr[i-1].TljRNose=_wtof(m_grid.GetItemText(i, 10));
		BDArr[i-1].Yljdml1=_wtof(m_grid.GetItemText(i, 11));
		BDArr[i-1].Yljdml2=_wtof(m_grid.GetItemText(i, 12));
		BDArr[i-1].Tljdml1=_wtof(m_grid.GetItemText(i, 13));
		BDArr[i-1].Tljdml2=_wtof(m_grid.GetItemText(i, 14));			
		BDArr[i-1].FldDml1=_wtof(m_grid.GetItemText(i, 15));
		BDArr[i-1].FldDml2=_wtof(m_grid.GetItemText(i, 16));			
	}
}

void BDCALDlg::ShowDataOnJM()
{
	ACHAR tmp[30];
	for(int i=0; i<NBIDUAN; i++)
	{
		m_grid.SetItemText(i+1,1,BDArr[i].XL1);
		m_grid.SetItemText(i+1,2,BDArr[i].XL2);
		if(BDArr[i].ZDStOrEd==0)
			m_grid.SetItemText(i+1,3,L"匝道起端");
		else
			m_grid.SetItemText(i+1,3,L"匝道终端");
		if(BDArr[i].Lmhp==1)
			m_grid.SetItemText(i+1,4,L"同坡");
		else
			m_grid.SetItemText(i+1,4,L"反坡");
		_stprintf(tmp,L"%0.3lf",BDArr[i].C1);
		m_grid.SetItemText(i+1,5,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].C2);
		m_grid.SetItemText(i+1,6,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Z1);
		m_grid.SetItemText(i+1,7,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Z2);
		m_grid.SetItemText(i+1,8,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].YljRNose);
		m_grid.SetItemText(i+1,9,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].TljRNose);
		m_grid.SetItemText(i+1,10,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Yljdml1);
		m_grid.SetItemText(i+1,11,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Yljdml2);
		m_grid.SetItemText(i+1,12,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Tljdml1);
		m_grid.SetItemText(i+1,13,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].Tljdml2);
		m_grid.SetItemText(i+1,14,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].FldDml1);
		m_grid.SetItemText(i+1,15,tmp);
		_stprintf(tmp,L"%0.3lf",BDArr[i].FldDml2);
		m_grid.SetItemText(i+1,16,tmp);

		_stprintf(tmp,L"%d",i+1);
		m_grid.SetItemText(i+1,0,tmp);
	}
	UpdateData(false);
}

void BDCALDlg::OnButtonCalbd() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_SELXL)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DRAWBD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_ADDBD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_DELBD)->EnableWindow(false);
	GetDlgItem(IDOK)->EnableWindow(false);
	GetDlgItem(IDCANCEL)->EnableWindow(false);
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	GetDataOnJM();
	double NosePt[2],cml1,cml2,cml;
	int i,pos,len,LorR;
	CString DBS1,DBS2,Name1,Name2;
	xlpoint PZ;
	ads_point ptt;
	for(i=0;i<NBIDUAN;i++)
	{
		if(BDArr[i].XL1==""||BDArr[i].XL2=="")
			continue;
		pos=BDArr[i].XL1.FindOneOf(L",");
		DBS1 = BDArr[i].XL1.Left(pos);
		len = BDArr[i].XL1.GetLength();
		Name1 =  BDArr[i].XL1.Right(len-pos-1);
		if(DBS1==""||Name1=="")
			continue;
		else 
		{
			//		  DBS.Read_XLDbs(DBS1,L"控制点表",Name1);
			DBS.Read_XLDbs(DBS1,L"线元表",Name1);
			DBS.Read_XLDbs(DBS1,L"断链表",Name1);
			if(DBS.XYNum>0)
			{
				if(pm1)
					delete pm1;
				pm1=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//			  pm1->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					pm1->setDLB(DBS.DLArray,DBS.NDL);
			}
			_tcscpy(pm1->mdbname,DBS1);
			_tcscpy(pm1->RoadName,Name1);
		}
		pos=BDArr[i].XL2.FindOneOf(L",");
		DBS2 = BDArr[i].XL2.Left(pos);
		len = BDArr[i].XL2.GetLength();
		Name2 =  BDArr[i].XL2.Right(len-pos-1);

		if(DBS2==""||Name2=="")
			continue;
		else 
		{
			//		  DBS.Read_XLDbs(DBS2,L"控制点表",Name2);
			DBS.Read_XLDbs(DBS2,L"线元表",Name2);
			DBS.Read_XLDbs(DBS2,L"断链表",Name2);
			if(DBS.XYNum>0)
			{
				if(pm2)
					delete pm2;
				pm2=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
				//			  pm2->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				if(DBS.NDL>0)
					pm2->setDLB(DBS.DLArray,DBS.NDL);
			}
			_tcscpy(pm2->mdbname,DBS2);
			_tcscpy(pm2->RoadName,Name2);
		}

		if(pm1==NULL||pm2==NULL)
			continue;


		double cml2,fwj;
		if(BDArr[i].ZDStOrEd==0)
		{
			ads_printf(L"正在计算%s与匝道%s起始鼻端位置,请稍候...\n",BDArr[i].XL1,BDArr[i].XL2);
			cml =  pm1->PROJ_ML(pm2->XYArray[1][8],pm2->XYArray[1][9]);
			BDArr[i].InOrOut=pm1->GetInOrOut(BDArr[i].ZDStOrEd,pm2);
			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,0.0,0.0,1,0.001,cml1,cml2,NosePt);

			BDArr[i].FldDml1 = pm1->XLC1(cml1);
			BDArr[i].FldDml2 = pm2->XLC1(cml2);

			//          ads_printf(L"cml=%lf dml=%lf %lf %lf\n",cml,pm1->XLC1(cml),pm2->JdCenArray[0].N,pm2->JdCenArray[0].E);
			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,BDArr[i].C1,BDArr[i].C2,0,BDArr[i].YljRNose,cml1,cml2,NosePt);

			BDArr[i].YNosePt[0]=NosePt[0];
			BDArr[i].YNosePt[1]=NosePt[1];
			BDArr[i].Yljdml1 = pm1->XLC1(cml1);
			BDArr[i].Yljdml2 = pm2->XLC1(cml2);
			PZ.lc = cml1;
			pm1->xlpoint_pz(&PZ);		  

			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);

			BDArr[i].YCSpt[0]=NosePt[0]+BDArr[i].YljRNose*cos(fwj);
			BDArr[i].YCSpt[1]=NosePt[1]+BDArr[i].YljRNose*sin(fwj);

			BDArr[i].YLjPt1[0]=NosePt[0]+(BDArr[i].YljRNose+BDArr[i].C1)*cos(fwj);
			BDArr[i].YLjPt1[1]=NosePt[1]+(BDArr[i].YljRNose+BDArr[i].C1)*sin(fwj);
			BDArr[i].YLjPt1[0]=BDArr[i].YLjPt1[0]+BDArr[i].Z1*cos(PZ.a);
			BDArr[i].YLjPt1[1]=BDArr[i].YLjPt1[1]+BDArr[i].Z1*sin(PZ.a);
			PZ.lc = cml2;
			pm2->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].YCEpt[0]=NosePt[0]+BDArr[i].YljRNose*cos(fwj);
			BDArr[i].YCEpt[1]=NosePt[1]+BDArr[i].YljRNose*sin(fwj);

			BDArr[i].YLjPt2[0]=NosePt[0]+(BDArr[i].YljRNose+BDArr[i].C2)*cos(fwj);
			BDArr[i].YLjPt2[1]=NosePt[1]+(BDArr[i].YljRNose+BDArr[i].C2)*sin(fwj);
			BDArr[i].YLjPt2[0]=BDArr[i].YLjPt2[0]+BDArr[i].Z2*cos(PZ.a);
			BDArr[i].YLjPt2[1]=BDArr[i].YLjPt2[1]+BDArr[i].Z2*sin(PZ.a);		  

			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,BDArr[i].C1,BDArr[i].C2,1,BDArr[i].TljRNose,cml1,cml2,NosePt);
			BDArr[i].TNosePt[0]=NosePt[0];
			BDArr[i].TNosePt[1]=NosePt[1];
			BDArr[i].Tljdml1 = pm1->XLC1(cml1);
			BDArr[i].Tljdml2 = pm2->XLC1(cml2);
			PZ.lc = cml1;
			pm1->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].TCSpt[0]=NosePt[0]+BDArr[i].TljRNose*cos(fwj);
			BDArr[i].TCSpt[1]=NosePt[1]+BDArr[i].TljRNose*sin(fwj);
			PZ.lc = cml2;
			pm2->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].TCEpt[0]=NosePt[0]+BDArr[i].TljRNose*cos(fwj);
			BDArr[i].TCEpt[1]=NosePt[1]+BDArr[i].TljRNose*sin(fwj);		
		}
		else
		{
			ads_printf(L"正在计算%s与匝道%s终止鼻端位置,请稍候...\n",BDArr[i].XL1,BDArr[i].XL2);
			cml =  pm1->PROJ_ML(pm2->XYArray[pm2->XYNum][8],pm2->XYArray[pm2->XYNum][9]);
			BDArr[i].InOrOut=pm1->GetInOrOut(BDArr[i].ZDStOrEd,pm2);
			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,0.0,0.0,1,0.001,cml1,cml2,NosePt);
			BDArr[i].FldDml1 = pm1->XLC1(cml1);
			BDArr[i].FldDml2 = pm2->XLC1(cml2);
			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,BDArr[i].C1,BDArr[i].C2,0,BDArr[i].YljRNose,cml1,cml2,NosePt);
			BDArr[i].YNosePt[0]=NosePt[0];
			BDArr[i].YNosePt[1]=NosePt[1];
			BDArr[i].Yljdml1 = pm1->XLC1(cml1);
			BDArr[i].Yljdml2 = pm2->XLC1(cml2);
			PZ.lc = cml1;
			pm1->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].YCSpt[0]=NosePt[0]+BDArr[i].YljRNose*cos(fwj);
			BDArr[i].YCSpt[1]=NosePt[1]+BDArr[i].YljRNose*sin(fwj);
			BDArr[i].YLjPt1[0]=NosePt[0]+(BDArr[i].YljRNose+BDArr[i].C1)*cos(fwj);
			BDArr[i].YLjPt1[1]=NosePt[1]+(BDArr[i].YljRNose+BDArr[i].C1)*sin(fwj);
			BDArr[i].YLjPt1[0]=BDArr[i].YLjPt1[0]+BDArr[i].Z1*cos(PZ.a+PI);
			BDArr[i].YLjPt1[1]=BDArr[i].YLjPt1[1]+BDArr[i].Z1*sin(PZ.a+PI);		  
			PZ.lc = cml2;
			pm2->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].YCEpt[0]=NosePt[0]+BDArr[i].YljRNose*cos(fwj);
			BDArr[i].YCEpt[1]=NosePt[1]+BDArr[i].YljRNose*sin(fwj);
			BDArr[i].YLjPt2[0]=NosePt[0]+(BDArr[i].YljRNose+BDArr[i].C2)*cos(fwj);
			BDArr[i].YLjPt2[1]=NosePt[1]+(BDArr[i].YljRNose+BDArr[i].C2)*sin(fwj);
			BDArr[i].YLjPt2[0]=BDArr[i].YLjPt2[0]+BDArr[i].Z2*cos(PZ.a+PI);
			BDArr[i].YLjPt2[1]=BDArr[i].YLjPt2[1]+BDArr[i].Z2*sin(PZ.a+PI);

			pm1->CalNoseCen(pm2,cml,BDArr[i].InOrOut,BDArr[i].C1,BDArr[i].C2,1,BDArr[i].TljRNose,cml1,cml2,NosePt);         
			BDArr[i].TNosePt[0]=NosePt[0];
			BDArr[i].TNosePt[1]=NosePt[1];
			BDArr[i].Tljdml1 = pm1->XLC1(cml1);
			BDArr[i].Tljdml2 = pm2->XLC1(cml2);
			PZ.lc = cml1;
			pm1->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].TCSpt[0]=NosePt[0]+BDArr[i].TljRNose*cos(fwj);
			BDArr[i].TCSpt[1]=NosePt[1]+BDArr[i].TljRNose*sin(fwj);
			PZ.lc = cml2;
			pm2->xlpoint_pz(&PZ);
			xyddaa(NosePt[0],NosePt[1],PZ.x,PZ.y,&fwj);
			BDArr[i].TCEpt[0]=NosePt[0]+BDArr[i].TljRNose*cos(fwj);
			BDArr[i].TCEpt[1]=NosePt[1]+BDArr[i].TljRNose*sin(fwj);		 
		}
		ads_printf(L"OK\n");
	}

	ShowDataOnJM();
	Invalidate();
	acDocManager->unlockDocument(acDocManager->curDocument());
	GetDlgItem(IDC_BUTTON_SELXL)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_DRAWBD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_ADDBD)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_DELBD)->EnableWindow(true);
	GetDlgItem(IDOK)->EnableWindow(true);
	GetDlgItem(IDCANCEL)->EnableWindow(true);
}

void BDCALDlg::OnButtonDrawbd() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);		// Hide our dialog
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	AcGePoint3d spt,ept;
	double R1,R2;
	int InOrOut;
	for(int i=0;i<NBIDUAN;i++)
	{
		//		InOrOut = pm1->GetInOrOut(BDArr[i].ZDStOrEd,pm2);
		if(BDArr[i].ZDStOrEd==0)
		{
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YCEpt[1],ept.y = BDArr[i].YCEpt[0];
			R1 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1]));
			R2 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1]));			
			makearc(spt,ept,0.5*(R1+R2),-1,1);
			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			ept.x = BDArr[i].TCEpt[1],ept.y = BDArr[i].TCEpt[0];
			R1 = sqrt((BDArr[i].TNosePt[0]-BDArr[i].TCSpt[0])*(BDArr[i].TNosePt[0]-BDArr[i].TCSpt[0])+
				(BDArr[i].TNosePt[1]-BDArr[i].TCSpt[1])*(BDArr[i].TNosePt[1]-BDArr[i].TCSpt[1]));
			R2 = sqrt((BDArr[i].TNosePt[0]-BDArr[i].TCEpt[0])*(BDArr[i].TNosePt[0]-BDArr[i].TCEpt[0])+
				(BDArr[i].TNosePt[1]-BDArr[i].TCEpt[1])*(BDArr[i].TNosePt[1]-BDArr[i].TCEpt[1]));		
			makearc(spt,ept,0.5*(R1+R2),-1,1);
			spt.x = BDArr[i].YNosePt[1];
			spt.y = BDArr[i].YNosePt[0];
			makeline(spt,spt,0,0.2);           
			/*
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YNosePt[1],ept.y = BDArr[i].YNosePt[0];
			makeline(spt,ept,0,0);
			spt.x = BDArr[i].YCEpt[1],spt.y = BDArr[i].YCEpt[0];
			makeline(spt,ept,1,0);
			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			ept.x = BDArr[i].TNosePt[1],ept.y = BDArr[i].TNosePt[0];
			makeline(spt,ept,0,0);*/
		}
		else
		{
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YCEpt[1],ept.y = BDArr[i].YCEpt[0];
			R1 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1]));
			R2 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1]));		
			makearc(spt,ept,0.5*(R1+R2),1,1);
			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			ept.x = BDArr[i].TCEpt[1],ept.y = BDArr[i].TCEpt[0];
			R1 = sqrt((BDArr[i].TNosePt[0]-BDArr[i].TCSpt[0])*(BDArr[i].TNosePt[0]-BDArr[i].TCSpt[0])+
				(BDArr[i].TNosePt[1]-BDArr[i].TCSpt[1])*(BDArr[i].TNosePt[1]-BDArr[i].TCSpt[1]));
			R2 = sqrt((BDArr[i].TNosePt[0]-BDArr[i].TCEpt[0])*(BDArr[i].TNosePt[0]-BDArr[i].TCEpt[0])+
				(BDArr[i].TNosePt[1]-BDArr[i].TCEpt[1])*(BDArr[i].TNosePt[1]-BDArr[i].TCEpt[1]));	
			makearc(spt,ept,0.5*(R1+R2),1,1);
			spt.x = BDArr[i].YNosePt[1];
			spt.y = BDArr[i].YNosePt[0];
			makeline(spt,spt,0,0.2);
			/*
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YNosePt[1],ept.y = BDArr[i].YNosePt[0];
			makeline(spt,ept,0,0);
			spt.x = BDArr[i].YCEpt[1],spt.y = BDArr[i].YCEpt[0];
			makeline(spt,ept,1,0);
			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			ept.x = BDArr[i].TNosePt[1],ept.y = BDArr[i].TNosePt[0];
			makeline(spt,ept,0,0);*/
		}
		spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
		ept.x = BDArr[i].YLjPt1[1],ept.y = BDArr[i].YLjPt1[0];
		makeline(spt,ept,0,0);
		spt.x = BDArr[i].YCEpt[1],spt.y = BDArr[i].YCEpt[0];
		ept.x = BDArr[i].YLjPt2[1],ept.y = BDArr[i].YLjPt2[0];
		makeline(spt,ept,0,0);
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog		
}

void BDCALDlg::OnButtonAddbd() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for(i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		for( i=1;i<m_grid.GetRowCount();i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.SetItemText(nRow, 4,L"同坡");
		m_grid.SetItemText(nRow, 3,L"匝道起端");
		m_grid.SetItemText(nRow, 5,L"0.0");
		m_grid.SetItemText(nRow, 6,L"0.0");
		m_grid.SetItemText(nRow, 7,L"0.0");
		m_grid.SetItemText(nRow, 8,L"0.0");
		m_grid.SetItemText(nRow, 9,L"0.6");
		m_grid.SetItemText(nRow, 10,L"1.5");
		for(int j=11;j<m_grid.GetColumnCount();j++)
			m_grid.SetItemBkColour(nRow,j,RGB(113,189,193));	
		m_grid.Invalidate();
	}		
}

void BDCALDlg::OnButtonDelbd() 
{
	// TODO: Add your control notification handler code here
	int nRow = m_grid.GetFocusCell().row;
	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}		
}

void BDCALDlg::OnOK() 
{
	// TODO: Add extra validation here	
	BAS_DRAW_FUN ob;
	int i;
	ob.ReadWorkdir();
	FILE *fpw;

	GetDataOnJM();
	CString fname;
	fname = Cworkdir+"\\DATA\\";
	fname = fname+"BDDATA";
	fpw = _wfopen(fname,L"w");
	fwprintf(fpw,L"%d\n",NBIDUAN);
	for(i=0;i<NBIDUAN;i++)
	{
		fwprintf(fpw,L"%s %s %d %d %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf\n",
			BDArr[i].XL1,BDArr[i].XL2,BDArr[i].ZDStOrEd,BDArr[i].Lmhp,BDArr[i].C1,BDArr[i].C2,BDArr[i].Z1,
			BDArr[i].Z2, BDArr[i].YljRNose,BDArr[i].TljRNose,BDArr[i].Yljdml1,
			BDArr[i].Yljdml2,BDArr[i].Tljdml1,BDArr[i].Tljdml2,BDArr[i].FldDml1,BDArr[i].FldDml2);
		fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf\n",BDArr[i].YCSpt[0],BDArr[i].YCSpt[1],
			BDArr[i].YCEpt[0],BDArr[i].YCEpt[1],BDArr[i].YNosePt[0],BDArr[i].YNosePt[1]); 
		fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf %0.3lf %0.3lf %0.3lf\n",BDArr[i].TCSpt[0],BDArr[i].TCSpt[1],
			BDArr[i].TCEpt[0],BDArr[i].TCEpt[1],BDArr[i].TNosePt[0],BDArr[i].TNosePt[1]);
		fwprintf(fpw,L"%0.3lf %0.3lf %0.3lf %0.3lf\n",BDArr[i].YLjPt1[0],BDArr[i].YLjPt1[1],BDArr[i].YLjPt2[0],BDArr[i].YLjPt2[1]);
	}
	fclose(fpw);
	ads_alert(L"数据已经入库");
	//	cdxCSizingDialog::OnOK();
}
