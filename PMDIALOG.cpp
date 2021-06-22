// PMDIALOG.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "PMDIALOG.h"
#include "xldatabase.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern AcDbCircle* pCircle;
//extern PMDIALOG PmDlg;
/////////////////////////////////////////////////////////////////////////////
// PMDIALOG dialog



PMDIALOG::PMDIALOG(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(PMDIALOG::IDD, pParent)
{
	m_FLDName = L"主线";
	JdCenObj=NULL;
	EXObj=NULL;
	MainLineEnable=true;
	MainLineDlg = NULL;
	//打开编辑线路交点的对话框时，将定位圆赋值为NULL
	pCircle = NULL;
}

PMDIALOG::~PMDIALOG()
{
	//PmDlg = NULL;
}
void PMDIALOG::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PMDIALOG)
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Text(pDX, IDC_EDIT_FLDNAME, m_FLDName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PMDIALOG, cdxCSizingDialog)
	//{{AFX_MSG_MAP(PMDIALOG)
	ON_BN_CLICKED(IDC_BUTTON_NEWFLD, OnButtonNewfld)
	ON_BN_CLICKED(IDC_BUTTON_RENAME, OnButtonRename)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BUTTON_DELFLD, OnButtonDelfld)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_InPmFromMdb, OnBUTTONInPmFromMdb)
	ON_BN_CLICKED(IDC_BUTTON_OutputMdb, OnBUTTONOutputMdb)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB1, OnSelchangingTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// PMDIALOG message handlers
BOOL PMDIALOG::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);

	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	// TODO: Add extra initialization here
	FLD.RemoveAll();
	m_Tab.DeleteAllItems();
	if(JdCenObj==NULL)//创建新实体
	{
		JD_CENTER *pJC = new JD_CENTER;
		m_Tab.InsertItem(0,m_FLDName);
		pJC->setRoadName(m_FLDName);
		FLDNameArray.Add(m_FLDName);

		CRect lpRect;
		JdCenterInputDlg *pJCDlg=new JdCenterInputDlg;
		pJCDlg->JdCenObj=pJC;
		pJCDlg->m_rname = m_FLDName;
		pJCDlg->JdCenId=BAS_DRAW_FUN::AddEntityToDbs(pJC);

		pJCDlg->Create(IDD_DIALOG_JDCENTERINPUT,this);

		GetClientRect(lpRect);

		pJCDlg->SetWindowPos(NULL, lpRect.left, lpRect.top + 80, lpRect.Width() + 10, lpRect.Height() - 50, SWP_SHOWWINDOW);

		pJCDlg->ShowWindow(TRUE);

		AddSzControl(*(pJCDlg),mdResize,mdResize);	
		FLD.Add(pJCDlg);
		m_Tab.SetCurSel(0);
		MainLineDlg = pJCDlg;
		acDocManager->unlockDocument(acDocManager->curDocument());
		SetFocus();	
	}
	else//查询实体数据
	{
		m_FLDName = JdCenObj->RoadName;
		m_Tab.InsertItem(0,m_FLDName);
		FLDNameArray.Add(m_FLDName);

		CRect lpRect;
		JdCenterInputDlg *pJCDlg=new JdCenterInputDlg;
		pJCDlg->JdCenObj=JdCenObj;
		pJCDlg->m_rname = m_FLDName;
		pJCDlg->JdCenId= JdCenId;

		pJCDlg->Create(IDD_DIALOG_JDCENTERINPUT,this);

		GetClientRect(lpRect);

		pJCDlg->SetWindowPos(NULL, lpRect.left, lpRect.top + 60, lpRect.Width() + 10, lpRect.Height() + 60, SWP_SHOWWINDOW);

		pJCDlg->ShowWindow(TRUE);

		AddSzControl(*(pJCDlg),mdResize,mdResize);	
		FLD.Add(pJCDlg);
		m_Tab.SetCurSel(0);
		if(m_FLDName=="主线")
			MainLineDlg = pJCDlg;
		acDocManager->unlockDocument(acDocManager->curDocument());
		SetFocus();	
		UpdateData(false);
	}
	m_Tab.Invalidate();
	// EXCEPTION: OCX Property Pages should return FALSE
	return true;
}

void PMDIALOG::OnButtonNewfld() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_FLDName=="主线")
	{
		AfxMessageBox(L"已有主线命名的方案，请输入其他方案名!");
		return;
	}
	if(m_FLDName!="")
	{
		//判断是否重名
		for(int i=0;i<FLDNameArray.GetSize();i++)
		{
			if(m_FLDName==FLDNameArray[i])
			{
				AfxMessageBox(L"与既有道路名重名!");
				return;
			}
		}
		int iFLD=m_Tab.GetCurFocus();
		//		if(iFLD==0)
		//			MainLine.ShowWindow(FALSE);
		//		else 
		FLD[iFLD]->ShowWindow(FALSE);
		FLDNameArray.Add(m_FLDName);
		int numFld=m_Tab.GetItemCount();
		m_Tab.InsertItem(numFld,m_FLDName);
		JdCenterInputDlg *newFLD=new JdCenterInputDlg;
		newFLD->m_rname = m_FLDName;
		newFLD->Create(IDD_DIALOG_JDCENTERINPUT,this);
		FLD.Add(newFLD);
		CRect lpRect;
		GetClientRect(lpRect);
		FLD[numFld]->SetWindowPos(NULL,lpRect.left,lpRect.top+50,lpRect.Width()+10,lpRect.Height()+35,SWP_SHOWWINDOW);
		FLD[numFld]->ShowWindow(TRUE);
		FLD[numFld]->Invalidate(false);
		AddSzControl(*(FLD[numFld]),mdResize,mdResize);
		m_Tab.SetCurSel(numFld);
	}
	else
	{
		AfxMessageBox(L"道路名称不能为空!");
	}
}

void PMDIALOG::OnButtonRename()//对分离段重命名
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iFLD=m_Tab.GetCurFocus();
	//	if(iFLD==0 || m_FLDName=="主线")return;
	if(m_FLDName!="")
	{
		//判断是否重名
		for(int i=0;i<FLDNameArray.GetSize();i++)
		{
			if(i==iFLD-1)continue;
			if(m_FLDName==FLDNameArray[i])
			{
				AfxMessageBox(L"与其它道路名重名!");
				return;
			}
		}
		FLDNameArray[iFLD]=m_FLDName;
		FLD[iFLD]->m_rname=m_FLDName;
		TCITEM *curItem = new TCITEM();

		curItem->mask= TCIF_TEXT;
		m_Tab.GetItem(iFLD, curItem);
		ACHAR ch[256];
		_tcscpy(ch,m_FLDName);
		curItem->pszText=ch;
		m_Tab.SetItem(iFLD, curItem);

		FLD[iFLD]->OnBnClickedOk();

	}
	else
	{
		AfxMessageBox(L"分离段名称不能为空!");
	}
}

void PMDIALOG::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) //选择一个分离段
{
	// TODO: Add your control notification handler code here

	int iFLD=m_Tab.GetCurFocus();

	for(int i=0;i<FLD.GetSize();i++)
	{
		if(i!=iFLD)
			FLD[iFLD]->ShowWindow(false);
	}

	FLD[iFLD]->ShowWindow(TRUE);
	m_FLDName=FLDNameArray[iFLD];
	FLD[iFLD]->UpdateData(false);
	FLD[iFLD]->Invalidate(false);

	//	curItem.mask= TCIF_TEXT;
	//	m_Tab.GetItem(iFLD, &curItem);	
	//	m_FLDName=curItem.pszText;
	UpdateData(FALSE);
	*pResult = 0;

}

void PMDIALOG::OnButtonDelfld()//删除一个分离段
{
	// TODO: Add your control notification handler code here
	int iFLD=m_Tab.GetCurFocus();

	if(iFLD>0)
	{
		JdCenterInputDlg *pJCDlg=FLD[iFLD];

		/*
		if(Acad::eOk==acdbOpenObject(pJCDlg->JdCenObj,pJCDlg->JdCenId,AcDb::kForWrite))
		{
		pJCDlg->JdCenObj->erase();
		pJCDlg->JdCenObj->close();
		}
		*/

		FLD.RemoveAt(iFLD);
		FLDNameArray.RemoveAt(iFLD);

		if(pJCDlg)
			delete pJCDlg;

		m_Tab.DeleteItem(iFLD);

		m_Tab.SetCurFocus(iFLD-1);
		m_FLDName=FLDNameArray[iFLD-1];
	}
	else
	{
		ads_alert(L"主线不能删除!");
	}

	m_Tab.Invalidate(false);
}

void PMDIALOG::OnOK()//确定生成基线和分离段
{
	//TODO: Add extra validation here
	UpdateData();
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	int i;
	for(i=0;i<FLD.GetSize();i++)
	{
		JdCenterInputDlg *pJCDlg=FLD[i];
		pJCDlg->OnBnClickedOk();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	

	for(i=0;i<FLD.GetSize();i++)
	{
		JdCenterInputDlg *pJCDlg=FLD[i];
		delete pJCDlg; pJCDlg=NULL;
	}
	FLD.RemoveAll();
	FLDNameArray.RemoveAll();
	//删除定位的圆
	if (pCircle != NULL)
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		AcDbEntity * pEnt;
		if(acdbOpenObject(pEnt,pCircle->id(),AcDb::kForWrite)!=Acad::eOk)
			return;
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle = AcDbCircle::cast(pEnt);
			pCircle->assertWriteEnabled();
			pCircle->erase();
			pCircle->close();
		}
		pEnt->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		pCircle = NULL;
	}
	cdxCSizingDialog::OnOK();
}

void PMDIALOG::OnCancel() 
{
	//TODO: Add extra cleanup here
	acedGetAcadDwgView()->SetFocus();	
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	for(int i=0;i<FLD.GetSize();i++)
	{

		JdCenterInputDlg *pJCDlg=FLD[i];
		/*		if(Acad::eOk==acdbOpenObject(FLD[i]->JdCenObj,FLD[i]->JdCenId,AcDb::kForWrite))
		{
		FLD[i]->JdCenObj->erase();
		FLD[i]->JdCenObj->close();
		}*/

		delete pJCDlg; pJCDlg=NULL;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();	

	FLD.RemoveAll();
	FLDNameArray.RemoveAll();
	//删除定位的圆
	if (pCircle != NULL)
	{
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		AcDbEntity * pEnt;
		if (acdbOpenObject(pEnt, pCircle->id(), AcDb::kForWrite) != Acad::eOk)
		{
			pCircle = NULL;
			cdxCSizingDialog::OnCancel();
			return;
		}
		if (pEnt->isKindOf(AcDbCircle::desc()))
		{
			pCircle = AcDbCircle::cast(pEnt);
			pCircle->assertWriteEnabled();
			pCircle->erase();
			pCircle->close();
		}
		pEnt->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		pCircle = NULL;
	}
	cdxCSizingDialog::OnCancel();
}

void PMDIALOG::OnPaint() //绘图
{
	CPaintDC dc(this); // device context for painting

	// Do not call cdxCSizingDialog::OnPaint() for painting messages
}

void PMDIALOG::OnBUTTONInPmFromMdb() //导入数据
{
	// TODO: Add your control notification handler code here
	CString workdir,curpm,Path;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{
	workdir += "\\data\\";   
	CString DefaultName = 	workdir + Pro + ".mdb";
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
	{
		Path =fname.GetPathName();//数据库名
		//删除现有的对话框

		acedGetAcadDwgView()->SetFocus();	
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		int i;

		while(m_Tab.GetItemCount()>1)
		{
			JdCenterInputDlg *pJCDlg=FLD[1];
			FLD.RemoveAt(1);
			FLDNameArray.RemoveAt(1);
			if(pJCDlg)
				delete pJCDlg;
			m_Tab.DeleteItem(1);		
		}

		//读入数据
		XLDataBase DBS;
		DBS.m_RoadSum=0;
		DBS.ReadAllRoadName(Path);//先读出path数据库中所有的路名
		for(i=0; i<DBS.m_RoadSum; i++)
		{
			JD_CENTER *pJC=NULL;
			DBS.Read_XLDbs(Path,L"控制点表",DBS.Road[i].RoadName);		
			DBS.Read_XLDbs(Path,L"断链表",DBS.Road[i].RoadName);
			if(DBS.NJdCen>0)
			{
				pJC=new JD_CENTER;			
				pJC->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
				pJC->setRoadName(DBS.Road[i].RoadName);
				pJC->setMdbName(Path);
				if(DBS.NDL>0)
				{
					pJC->setDLB(DBS.DLArray,DBS.NDL);
				}
				pJC->setLayer(L"公路平面设计专用层");
				//	BAS_DRAW_FUN::AddEntityToDbs(pJC);
			}

			if(DBS.Road[i].RoadName != "主线")//新建
			{			   
				m_Tab.InsertItem(m_Tab.GetItemCount(),DBS.Road[i].RoadName);			   
				FLDNameArray.Add(DBS.Road[i].RoadName);

				JdCenterInputDlg *pJCDlg=new JdCenterInputDlg;
				pJCDlg->JdCenObj=pJC;
				pJCDlg->m_rname = DBS.Road[i].RoadName;
				CRect lpRect;
				pJCDlg->JdCenId=BAS_DRAW_FUN::AddEntityToDbs(pJC);
				pJCDlg->Create(IDD_DIALOG_JDCENTERINPUT,this);
				GetClientRect(lpRect);
				pJCDlg->SetWindowPos(NULL,lpRect.left,lpRect.top+50,lpRect.Width()+10,lpRect.Height()+15,SWP_SHOWWINDOW);
				pJCDlg->ShowWindow(false);
				AddSzControl(*(pJCDlg),mdResize,mdResize);	
				FLD.Add(pJCDlg);
				m_FLDName = DBS.Road[i].RoadName;
				pJCDlg->UpdateData(false);
				pJCDlg->Invalidate(false);
			}
			else//主线
			{
				MainLineDlg->JdCenObj=pJC;
				MainLineDlg->JdCenId=BAS_DRAW_FUN::AddEntityToDbs(pJC);
				FLD[0] = MainLineDlg;
				MainLineDlg->INIT();
				MainLineDlg->UpdateData(false);
				MainLineDlg->Invalidate(false);
				MainLineDlg->Invalidate();
			}		   
		}
		//		m_Tab.SetCurSel(0);
		DBS.Release();
	}
	//	Invalidate(false);
	//	UpdateData(false);
}

void PMDIALOG::OnBUTTONOutputMdb() //导出数据
{
	CString workdir,curpm,Path;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{
	workdir += "\\data\\";   
	CString DefaultName = workdir + Pro + ".mdb";
	CFileDialog fname(false,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
	{
		Path =fname.GetPathName();//数据库名
		UpdateData();
		acedGetAcadDwgView()->SetFocus();	
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);


		XLDataBase DBS;
		DBS.DelAllRecords(Path,L"控制点表");//删除表中所有记录
		DBS.DelAllRecords(Path,L"断链表");//删除表中所有记录

		int i;
		for(i=0;i<FLD.GetSize();i++)
		{
			JdCenterInputDlg *pJCDlg=FLD[i];
			pJCDlg->OnBnClickedOk();

			//写入数据库

			if(pJCDlg->JdCenObj == NULL)
				continue;

			//   ads_printf(L"#### %s %d\n",pJCDlg->JdCenObj->RoadName,pJCDlg->JdCenObj->JdCenNum);
			DBS.NJdCen=pJCDlg->JdCenObj->JdCenNum;
			if(DBS.NJdCen>0)
				DBS.JdCenArray=new BAS_DRAW_FUN::JdCenter[DBS.NJdCen];
			for(int i=0;i<DBS.NJdCen;i++)
				DBS.JdCenArray[i]=pJCDlg->JdCenObj->JdCenArray[i];

			DBS.NDL=pJCDlg->JdCenObj->DLNum;
			DBS.DLArray=new BAS_DRAW_FUN::DLBZ[DBS.NDL];
			for(i=0;i<DBS.NDL;i++)
				DBS.DLArray[i]=pJCDlg->JdCenObj->DLArray[i];

			if(Path != "")
			{
				/*
				if(_tcscmp(pJCDlg->JdCenObj->RoadName , "主线") !=0 )
				{
				//修正断链值
				if(MainLineDlg->JdCenObj)//主线非空
				{
				if(MainLineDlg->JdCenObj->JdCenNum>0 && MainLineDlg->JdCenObj->DLNum>0 &&
				pJCDlg->JdCenObj->JdCenNum>0 && pJCDlg->JdCenObj->DLNum>0)
				{

				double tylc1=MainLineDlg->JdCenObj->PROJ_ML(pJCDlg->JdCenObj->JdCenArray[0].JDN,pJCDlg->JdCenObj->JdCenArray[0].JDE);

				CString BGH,EGH;
				double BLC = MainLineDlg->JdCenObj->XLC(tylc1,BGH);

				acdbOpenObject(pJCDlg->JdCenObj,pJCDlg->JdCenId,AcDb::kForWrite);
				pJCDlg->JdCenObj->assertWriteEnabled();
				pJCDlg->JdCenObj->setDLM(0,BLC, pJCDlg->JdCenObj->DLArray[0].ELC,BGH, pJCDlg->JdCenObj->DLArray[0].EGH);					   
				pJCDlg->JdCenObj->JdCenArray[0].A1 = pJCDlg->JdCenObj->DLArray[0].BLC;

				CString str;
				str.Format(L"右线%0.1lf", pJCDlg->JdCenObj->DLArray[0].BLC/1000.0);
				pJCDlg->JdCenObj->setRoadName(str);

				tylc1=MainLineDlg->JdCenObj->PROJ_ML(pJCDlg->JdCenObj->JdCenArray[pJCDlg->JdCenObj->JdCenNum-1].JDN,pJCDlg->JdCenObj->JdCenArray[pJCDlg->JdCenObj->JdCenNum-1].JDE);//终点
				double ELC = MainLineDlg->JdCenObj->XLC(tylc1,EGH);
				pJCDlg->JdCenObj->setDLM(0,pJCDlg->JdCenObj->DLArray[pJCDlg->JdCenObj->DLNum-1].BLC,ELC,pJCDlg->JdCenObj->DLArray[pJCDlg->JdCenObj->DLNum-1].BGH,EGH);						   						   					   
				pJCDlg->JdCenObj->close();

				m_FLDName = pJCDlg->JdCenObj->RoadName;

				}    					   
				}				   
				}
				*/
				CString rname;
				rname = pJCDlg->JdCenObj->RoadName;

				DBS.Write_XLDbs(Path,L"道路名",rname);
				DBS.Write_XLDbs(Path,L"控制点表",rname);		
				DBS.Write_XLDbs(Path,L"断链表",rname);		
				ACHAR mes[256];
				_stprintf(mes,L"已将数据存入%s数据库,道路名: %s\n",Path,pJCDlg->JdCenObj->RoadName);
				ads_printf(mes);
				acDocManager->unlockDocument(acDocManager->curDocument());
				SetFocus();		
			}

		}
	}

	Invalidate(false);
}

void PMDIALOG::OnSelchangingTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int iFLD=m_Tab.GetCurFocus();
	if(iFLD>=0)
		FLD[iFLD]->ShowWindow(false);
	*pResult = 0;
}























