// DELDBBZ.cpp : implementation file
//

#include "stdafx.h"
//#include "roadandrail.h"
#include "DELDBBZ.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDELDBBZ dialog


CDELDBBZ::CDELDBBZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDELDBBZ::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDELDBBZ)
	m_HxZh = _T("");
	m_KdBz = FALSE;
	m_LjDesh = FALSE;
	m_LmDesh = FALSE;
	m_LmHp = FALSE;
	m_TljHp = FALSE;
	m_ZhBz = FALSE;
	//}}AFX_DATA_INIT
}


void CDELDBBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDELDBBZ)
	DDX_Text(pDX, IDC_STATIC_HXZH, m_HxZh);
	DDX_Check(pDX, IDC_CHECK_KDBZ, m_KdBz);
	DDX_Check(pDX, IDC_CHECK_LJDESH, m_LjDesh);
	DDX_Check(pDX, IDC_CHECK_LMDESH, m_LmDesh);
	DDX_Check(pDX, IDC_CHECK_LMHP, m_LmHp);
	DDX_Check(pDX, IDC_CHECK_TLJHP, m_TljHp);
	DDX_Check(pDX, IDC_CHECK_ZHBZ, m_ZhBz);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDELDBBZ, CDialog)
	//{{AFX_MSG_MAP(CDELDBBZ)
	ON_BN_CLICKED(IDC_BUTTON_PICKZH, OnButtonPickzh)
	ON_BN_CLICKED(IDC_RADIO_LEFT, OnRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_Right, OnRADIORight)
	ON_BN_CLICKED(IDC_RADIO_BOTH, OnRadioBoth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDELDBBZ message handlers

void CDELDBBZ::OnButtonPickzh() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	int rc,i,xsw;
	ads_name en;
	AcGePoint3d PT;
	double cml,tmp;

	rc = ads_entsel(L"\n选择需删除的文字 : ", en, asDblArray(PT));
	if (rc == RTNORM) 	   
	{   	
		AcDbObjectId eId;
		acdbGetObjectId(eId, en);	
		BAS_DRAW_FUN::GetDBXdata(eId,mdbname,roadname,cml,hxbh,tmp,tmp,tmp,tmp,xsw);
		if(mdbname!=""&&roadname!="")
		{
			m_HxZh = mdbname+","+roadname+"-"+hxbh;
			GetDlgItem(IDC_CHECK_LMDESH)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_ZHBZ)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_KDBZ)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_LJDESH)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_LMHP)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_TLJHP)->EnableWindow(TRUE); 
			GetDlgItem(IDOK)->EnableWindow(true);
		}
		UpdateData(false);
	}		

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/			
}

void CDELDBBZ::OnRadioLeft() 
{
	// TODO: Add your control notification handler code here
	m_bc = -1;
}

void CDELDBBZ::OnRADIORight() 
{
	// TODO: Add your control notification handler code here
	m_bc = 1;
}

void CDELDBBZ::OnRadioBoth() 
{
	// TODO: Add your control notification handler code here
	m_bc = 0;
}

BOOL CDELDBBZ::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	mdbname = roadname = hxbh ="";
	m_bc = 0;

	CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_BOTH);
	pbutton->SetCheck(1);

	GetDlgItem(IDC_CHECK_LMDESH)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_ZHBZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_KDBZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_LJDESH)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_LMHP)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_TLJHP)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDELDBBZ::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	acedGetAcadDwgView()->SetFocus();
	//	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);
	JD_CENTER *m_pm;
	if(mdbname!=""&&roadname!="")
	{
		DBS.Read_XLDbs(mdbname,"线元表",roadname);
		DBS.Read_XLDbs(mdbname,"断链表",roadname);
		if(DBS.XYNum>0)
		{
			m_pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				m_pm->setDLB(DBS.DLArray,DBS.NDL);
			_tcscpy(m_pm->mdbname,mdbname);
			_tcscpy(m_pm->RoadName,roadname);     
		}
	}
	AcArray<CString> layerArray;
	if(m_KdBz)//删除宽度标注
		layerArray.append("宽度");	
	if(m_LjDesh)//删除宽度标注
		layerArray.append("路肩高");
	if(m_LmDesh)
		layerArray.append("设计高");
	if(m_LmHp)	
		layerArray.append("路面横坡");
	if(m_TljHp)
		layerArray.append("路肩横坡");
	if(m_ZhBz)
		layerArray.append("桩号");

	IdArr.removeSubArray(0,IdArr.length()-1);
	BAS_DRAW_FUN::GetDBHXEntity(IdArr,layerArray,mdbname,roadname,hxbh);
	CString mname,rname,bh;
	double cml,curx,cury,prex,prey,dis;
	int i,LorR,xsw;
	AcDbEntity *pEnt;
	prex=prey=-9999;
	for(i=0;i<IdArr.length();i++)
	{		
		BAS_DRAW_FUN::GetDBXdata(IdArr[i],mname,rname,cml,hxbh,curx,cury,prex,prey,xsw);
		if(prex > -5000 && prey > -5000)
		{
			curx = 0.5*(curx+prex);
			cury = 0.5*(cury+prey);
		}

		if(m_bc==0)//两侧删除
		{
			if(acdbOpenObject(pEnt,IdArr[i],AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
			{
				pEnt->erase();
				pEnt->close();
			}
		}
		else if(m_bc==-1)//左侧
		{
			LorR = m_pm->JudgePTLorR(cury,curx,dis,m_pm->XYArray,m_pm->XYNum);
			if(LorR==-1)
			{
				if(acdbOpenObject(pEnt,IdArr[i],AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
				{
					pEnt->erase();
					pEnt->close();
				}
			}
		}
		else // 右侧
		{
			LorR = m_pm->JudgePTLorR(cury,curx,dis,m_pm->XYArray,m_pm->XYNum);
			if(LorR==1)
			{
				if(acdbOpenObject(pEnt,IdArr[i],AcDb::kForWrite,Adesk::kFalse)==Acad::eOk)
				{
					pEnt->erase();
					pEnt->close();
				}
			}
		}
	}

	if(m_pm)
		delete m_pm;	
	acDocManager->unlockDocument(acDocManager->curDocument());
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	EnableWindow(TRUE); // Enable our dialog*/	
	//	CDialog::OnOK();
}
