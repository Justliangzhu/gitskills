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
	m_FLDName = L"����";
	JdCenObj=NULL;
	EXObj=NULL;
	MainLineEnable=true;
	MainLineDlg = NULL;
	//�򿪱༭��·����ĶԻ���ʱ������λԲ��ֵΪNULL
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
	if(JdCenObj==NULL)//������ʵ��
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
	else//��ѯʵ������
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
		if(m_FLDName=="����")
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
	if(m_FLDName=="����")
	{
		AfxMessageBox(L"�������������ķ���������������������!");
		return;
	}
	if(m_FLDName!="")
	{
		//�ж��Ƿ�����
		for(int i=0;i<FLDNameArray.GetSize();i++)
		{
			if(m_FLDName==FLDNameArray[i])
			{
				AfxMessageBox(L"����е�·������!");
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
		AfxMessageBox(L"��·���Ʋ���Ϊ��!");
	}
}

void PMDIALOG::OnButtonRename()//�Է����������
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int iFLD=m_Tab.GetCurFocus();
	//	if(iFLD==0 || m_FLDName=="����")return;
	if(m_FLDName!="")
	{
		//�ж��Ƿ�����
		for(int i=0;i<FLDNameArray.GetSize();i++)
		{
			if(i==iFLD-1)continue;
			if(m_FLDName==FLDNameArray[i])
			{
				AfxMessageBox(L"��������·������!");
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
		AfxMessageBox(L"��������Ʋ���Ϊ��!");
	}
}

void PMDIALOG::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) //ѡ��һ�������
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

void PMDIALOG::OnButtonDelfld()//ɾ��һ�������
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
		ads_alert(L"���߲���ɾ��!");
	}

	m_Tab.Invalidate(false);
}

void PMDIALOG::OnOK()//ȷ�����ɻ��ߺͷ����
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
	//ɾ����λ��Բ
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
	//ɾ����λ��Բ
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

void PMDIALOG::OnPaint() //��ͼ
{
	CPaintDC dc(this); // device context for painting

	// Do not call cdxCSizingDialog::OnPaint() for painting messages
}

void PMDIALOG::OnBUTTONInPmFromMdb() //��������
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
	fname.m_ofn.lpstrFilter=L"��·�������ݿ�(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
	{
		Path =fname.GetPathName();//���ݿ���
		//ɾ�����еĶԻ���

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

		//��������
		XLDataBase DBS;
		DBS.m_RoadSum=0;
		DBS.ReadAllRoadName(Path);//�ȶ���path���ݿ������е�·��
		for(i=0; i<DBS.m_RoadSum; i++)
		{
			JD_CENTER *pJC=NULL;
			DBS.Read_XLDbs(Path,L"���Ƶ��",DBS.Road[i].RoadName);		
			DBS.Read_XLDbs(Path,L"������",DBS.Road[i].RoadName);
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
				pJC->setLayer(L"��·ƽ�����ר�ò�");
				//	BAS_DRAW_FUN::AddEntityToDbs(pJC);
			}

			if(DBS.Road[i].RoadName != "����")//�½�
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
			else//����
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

void PMDIALOG::OnBUTTONOutputMdb() //��������
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
	fname.m_ofn.lpstrFilter=L"��·�������ݿ�(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
	{
		Path =fname.GetPathName();//���ݿ���
		UpdateData();
		acedGetAcadDwgView()->SetFocus();	
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);


		XLDataBase DBS;
		DBS.DelAllRecords(Path,L"���Ƶ��");//ɾ���������м�¼
		DBS.DelAllRecords(Path,L"������");//ɾ���������м�¼

		int i;
		for(i=0;i<FLD.GetSize();i++)
		{
			JdCenterInputDlg *pJCDlg=FLD[i];
			pJCDlg->OnBnClickedOk();

			//д�����ݿ�

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
				if(_tcscmp(pJCDlg->JdCenObj->RoadName , "����") !=0 )
				{
				//��������ֵ
				if(MainLineDlg->JdCenObj)//���߷ǿ�
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
				str.Format(L"����%0.1lf", pJCDlg->JdCenObj->DLArray[0].BLC/1000.0);
				pJCDlg->JdCenObj->setRoadName(str);

				tylc1=MainLineDlg->JdCenObj->PROJ_ML(pJCDlg->JdCenObj->JdCenArray[pJCDlg->JdCenObj->JdCenNum-1].JDN,pJCDlg->JdCenObj->JdCenArray[pJCDlg->JdCenObj->JdCenNum-1].JDE);//�յ�
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

				DBS.Write_XLDbs(Path,L"��·��",rname);
				DBS.Write_XLDbs(Path,L"���Ƶ��",rname);		
				DBS.Write_XLDbs(Path,L"������",rname);		
				ACHAR mes[256];
				_stprintf(mes,L"�ѽ����ݴ���%s���ݿ�,��·��: %s\n",Path,pJCDlg->JdCenObj->RoadName);
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























