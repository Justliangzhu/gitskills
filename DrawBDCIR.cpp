// DrawBDCIR.cpp : implementation file
//

#include "stdafx.h"
#include "DrawBDCIR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DrawBDCIR dialog


DrawBDCIR::DrawBDCIR(CWnd* pParent /*=NULL*/)
	: CDialog(DrawBDCIR::IDD, pParent)
{
	//{{AFX_DATA_INIT(DrawBDCIR)
	m_XL1 = _T("");
	m_XL2 = _T("");
	m_pm=NULL;
	//}}AFX_DATA_INIT
}


void DrawBDCIR::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DrawBDCIR)
	DDX_Text(pDX, IDC_EDIT_XL1, m_XL1);
	DDX_Text(pDX, IDC_EDIT_XL2, m_XL2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DrawBDCIR, CDialog)
	//{{AFX_MSG_MAP(DrawBDCIR)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL1, OnButtonPickxl1)
	ON_BN_CLICKED(IDC_BUTTON_PICKXL2, OnButtonPickxl2)
	ON_BN_CLICKED(IDC_RADIO_DRAWFLB, OnRadioDrawflb)
	ON_BN_CLICKED(IDC_RADIO_DRAWHLB, OnRadioDrawhlb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DrawBDCIR message handlers

void DrawBDCIR::OnButtonPickxl1() 
{
	// TODO: Add your control notification handler code here
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml;
	CString mdbname="",roadname="";
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	rc = ads_entsel(L"\n选择线路中线 : ", en, asDblArray(PT));
	if (rc == RTNORM) 
	{    
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
			m_pm = JD_CENTER::cast(pObj);
			m_XL1.Format(L"%s,%s",m_pm->mdbname,m_pm->RoadName);
			UpdateData(FALSE);
		}
		else
		{
			GetXLXdata(eId,mdbname,roadname);
			m_XL1.Format(L"%s,%s",mdbname,roadname);
			UpdateData(FALSE);				
		}
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/	
}

void DrawBDCIR::OnButtonPickxl2() 
{
	// TODO: Add your control notification handler code here
	int rc;
	AcGePoint3d PT;
	ads_name en;
	AcDbObjectId eId;
	double cml;
	CString mdbname="",roadname="";
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	//	GetParent()->SetFocus();	// Give AutoCAD the focus.

	rc = ads_entsel(L"\n选择线路中线 : ", en, asDblArray(PT));
	if (rc == RTNORM) 
	{    
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
			m_pm = JD_CENTER::cast(pObj);
			m_XL2.Format(L"%s,%s",m_pm->mdbname,m_pm->RoadName);
			UpdateData(FALSE);
		}
		else
		{
			GetXLXdata(eId,mdbname,roadname);
			m_XL2.Format(L"%s,%s",mdbname,roadname);
			UpdateData(FALSE);				
		}
	}
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/		
}

void DrawBDCIR::OnRadioDrawflb() 
{
	// TODO: Add your control notification handler code here
	InorOut = 0;
}

void DrawBDCIR::OnRadioDrawhlb() 
{
	// TODO: Add your control notification handler code here
	InorOut = 1;
}

void DrawBDCIR::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_XL1.MakeUpper();
	m_XL2.MakeUpper();
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	int i;
	AcGePoint3d spt,ept;
	double R1,R2;
	i = FindBDXH();
	if(i>=0)
	{
		if(BDArr[i].ZDStOrEd==0)
		{
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YCEpt[1],ept.y = BDArr[i].YCEpt[0];
			R1 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1]));
			R2 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1]));
			//			makearc(spt,ept,BDArr[i].YljRNose,-1,1);
			makearc(spt,ept,0.5*(R1+R2),-1,1);
			//			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			//			ept.x = BDArr[i].TCEpt[1],ept.y = BDArr[i].TCEpt[0];
			//			makearc(spt,ept,BDArr[i].TljRNose,-1,1);
			spt.x = BDArr[i].YNosePt[1];
			spt.y = BDArr[i].YNosePt[0];
			makeline(spt,spt,0,0.2);           
		}
		else
		{
			spt.x = BDArr[i].YCSpt[1],spt.y = BDArr[i].YCSpt[0];
			ept.x = BDArr[i].YCEpt[1],ept.y = BDArr[i].YCEpt[0];
			ept.x = BDArr[i].YCEpt[1],ept.y = BDArr[i].YCEpt[0];
			R1 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCSpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCSpt[1]));
			R2 = sqrt((BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])*(BDArr[i].YNosePt[0]-BDArr[i].YCEpt[0])+
				(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1])*(BDArr[i].YNosePt[1]-BDArr[i].YCEpt[1]));
			makearc(spt,ept,0.5*(R1+R2),1,1);
			//			spt.x = BDArr[i].TCSpt[1],spt.y = BDArr[i].TCSpt[0];
			//			ept.x = BDArr[i].TCEpt[1],ept.y = BDArr[i].TCEpt[0];
			//			makearc(spt,ept,BDArr[i].TljRNose,1,1);
			spt.x = BDArr[i].YNosePt[1];
			spt.y = BDArr[i].YNosePt[0];
			makeline(spt,spt,0,0.2);
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
	//		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/	
	//	CDialog::OnOK();
}

BOOL DrawBDCIR::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_DRAWFLB);
	pBtn->SetCheck(1);		
	InorOut = 0;
	ReadBDFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

int DrawBDCIR::FindBDXH()
{
	for(int i=0;i<NBIDUAN;i++)
	{
		if(((BDArr[i].XL1==m_XL1&&BDArr[i].XL2==m_XL2)||
			(BDArr[i].XL1==m_XL2&&BDArr[i].XL2==m_XL1))&&BDArr[i].ZDStOrEd==InorOut)
			return i;
	}
	return -1;
}
bool DrawBDCIR::ReadBDFile()
{
	FILE *fpr;
	CString fname;
	ReadWorkdir();

	fname = Cworkdir+"\\DATA\\";
	fname = fname+"BDDATA";
	fpr = _wfopen(fname,L"r");
	char xl1[256],xl2[256];
	int i;
	NBIDUAN = 0;
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
			BDArr[i].XL1.MakeUpper();

			BDArr[i].XL2 = xl2;
			BDArr[i].XL2.MakeUpper();

			fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",&BDArr[i].YCSpt[0],&BDArr[i].YCSpt[1],
				&BDArr[i].YCEpt[0],&BDArr[i].YCEpt[1],&BDArr[i].YNosePt[0],&BDArr[i].YNosePt[1]); 
			fwscanf(fpr,L"%lf%lf%lf%lf%lf%lf",&BDArr[i].TCSpt[0],&BDArr[i].TCSpt[1],
				&BDArr[i].TCEpt[0],&BDArr[i].TCEpt[1],&BDArr[i].TNosePt[0],&BDArr[i].TNosePt[1]);
			fwscanf(fpr,L"%lf%lf%lf%lf",&BDArr[i].YLjPt1[0],&BDArr[i].YLjPt1[1],&BDArr[i].YLjPt2[0],&BDArr[i].YLjPt2[1]);
		}
		fclose(fpr);	
	}
	else
		ads_alert(L"请先进行鼻端计算!");
	return true;
}
