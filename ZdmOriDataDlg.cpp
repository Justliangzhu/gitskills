// ZdmOriDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "stdarx.h"
#include "ZdmOriDataDlg.h"
#include "ori_scale.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern AcDbObjectId ZpId;//记录当前纵坡
/////////////////////////////////////////////////////////////////////////////
// ZdmOriDataDlg dialog


ZdmOriDataDlg::ZdmOriDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ZdmOriDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZdmOriDataDlg)
	m_LCBoxH = _T("20");
	m_LCTextH = _T("3");
	m_PQXBoxH = _T("40");
	m_PQXTextH = _T("5");
	m_VScale = _T("200");
	m_Xori = _T("0.0");
	m_Yori = _T("0.0");
	m_Hscale = _T("200");
	m_bd = _T("保留三位小数");
	VerRadius = _T("1米整倍数");
	m_TDFS = _T("任意拖动");
	m_desh = _T("保留三位小数");
	m_PDTextH = _T("4");
	//}}AFX_DATA_INIT
}


void ZdmOriDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZdmOriDataDlg)
	DDX_Text(pDX, IDC_EDIT_LCBoxH, m_LCBoxH);
	DDX_Text(pDX, IDC_EDIT_LCTextH, m_LCTextH);
	DDX_Text(pDX, IDC_EDIT_PQXBoxH, m_PQXBoxH);
	DDX_Text(pDX, IDC_EDIT_PQXTextH, m_PQXTextH);
	DDX_Text(pDX, IDC_EDIT_Vscale, m_VScale);
	//DDX_Text(pDX, IDC_EDIT_xori, m_Xori);
	//DDX_Text(pDX, IDC_EDIT_Yori, m_Yori);
	DDX_Text(pDX, IDC_EDIT_Hscale, m_Hscale);
	DDX_CBString(pDX, IDC_COMBO_bd, m_bd);
	DDX_Text(pDX, IDC_COMBO_VerRadius, VerRadius);
	DDX_Control(pDX, IDC_COMBO_bpdlc, m_bpdlc);
	DDX_CBString(pDX, IDC_COMBO_TDFS, m_TDFS);
	DDX_CBString(pDX, IDC_COMBOdesh, m_desh);
	DDX_Text(pDX, IDC_EDIT_PDTextH, m_PDTextH);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ZdmOriDataDlg, CDialog)
	//{{AFX_MSG_MAP(ZdmOriDataDlg)
	//ON_BN_CLICKED(IDC_BUTTON_pick, OnBUTTONpick)
	ON_CBN_SELCHANGE(IDC_COMBO_bpdlc, OnSelchangeComboxBpdlc)
	//	ON_BN_CLICKED(IDC_RADIOLJLP, OnRadioljlp)
	//	ON_BN_CLICKED(IDC_RADIOGMLP, OnRadiogmlp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZdmOriDataDlg message handlers

void ZdmOriDataDlg::OnOK() 
{
	CDialog::OnOK();
	// TODO: Add extra validation here
	pORI_SCALE.HSCALE = _wtof(m_Hscale)/1000.0;
	pORI_SCALE.VSCALE = _wtof(m_VScale)/1000.0;
	pORI_SCALE.X_ORI = _wtof(m_Xori);
	pORI_SCALE.Y_ORI = _wtof(m_Yori);
	pORI_SCALE.LCBTEXTH = _wtof(m_LCTextH);
	pORI_SCALE.PQXTEXTH = _wtof(m_PQXTextH);
	pORI_SCALE.HBVSCALE = pORI_SCALE.HSCALE/pORI_SCALE.VSCALE;
	pORI_SCALE.PQXHIGH = _wtof(m_PQXBoxH);
	pORI_SCALE.LCBHigh = _wtof(m_LCBoxH);
	pORI_SCALE.PDTextH = _wtof(m_PDTextH);


	//	pORI_SCALE.m_DXorRXD = m_DXorRXD;
	//	pORI_SCALE.LJorGM=m_LJorGM;
	if(m_bd=="保留一位小数")
		pORI_SCALE.XSW = 1;
	else if(m_bd=="保留两位小数")
		pORI_SCALE.XSW = 2;
	else if(m_bd=="保留三位小数")
		pORI_SCALE.XSW = 3;
	else 
		pORI_SCALE.XSW = 0;

	if(VerRadius=="1米整倍数")
		pORI_SCALE.VerRadius =1;
	else if(VerRadius=="10米整倍数")
		pORI_SCALE.VerRadius = 2;
	else if(VerRadius=="100米整倍数")
		pORI_SCALE.VerRadius = 3;
	else 
		pORI_SCALE.VerRadius = 0;

	if(ModeBpdlc==0)
		pORI_SCALE.QZZ = 0;
	else if(ModeBpdlc==1)
		pORI_SCALE.QZZ = 10;
	else if(ModeBpdlc==2)
		pORI_SCALE.QZZ = 50;
	else
		pORI_SCALE.QZZ = 500;

	if(m_TDFS=="任意拖动")
		pORI_SCALE.TDFS=1;
	else if(m_TDFS=="前坡度不变拖动")
		pORI_SCALE.TDFS=2;
	else
		pORI_SCALE.TDFS=3;


	if(m_desh=="保留一位小数")
		pORI_SCALE.DesHXSW = 1;
	else if(m_desh=="保留两位小数")
		pORI_SCALE.DesHXSW = 2;
	else if(m_desh=="保留三位小数")
		pORI_SCALE.DesHXSW = 3;
	else 
		pORI_SCALE.DesHXSW = 0;

	//修改当前纵坡
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	GTZDM_ROAD *zdm;
	if(acdbOpenObject(zdm,ZpId, AcDb::kForWrite)==Acad::eOk)
	{
		zdm->assertWriteEnabled();
		zdm->Xsw = pORI_SCALE.XSW;
		zdm->QZZ = pORI_SCALE.QZZ;
		zdm->TDFS = pORI_SCALE.TDFS;
		zdm->DesHXSW = pORI_SCALE.DesHXSW;
		zdm->VerRadius = pORI_SCALE.VerRadius;
		zdm->texth = pORI_SCALE.PDTextH;
		//		 zdm->LJorGM = pORI_SCALE.LJorGM;			 
		zdm->close();
	}
	acDocManager->unlockDocument(acDocManager->curDocument());
}

void ZdmOriDataDlg::OnBUTTONpick() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	AcGePoint3d pkPt;	
	int retCode;

	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	acedInitGet(NULL, NULL);
	retCode = acedGetPoint(NULL,L"请在屏幕上指定绘图原点,",
		asDblArray(pkPt));
	if(retCode==RTCAN)
	{
		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog*/
		return;
	}
	switch(retCode)
	{
	case RTCAN  :
	case RTNONE :

		pkPt.set(0.0, 0.0, 0.0);

		break;

	case RTNORM :
		break;

	}

	ACHAR str[80];
	_stprintf(str,L"%0.3lf",pkPt.x);
	m_Xori = str ;
	_stprintf(str,L"%0.3lf",pkPt.y);
	m_Yori = str;

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	// Transfer the data values from the member variables
	// to the dialog.
	UpdateData(FALSE);		
}

void ZdmOriDataDlg::OnSelchangeComboxBpdlc() 
{
	// TODO: Add your control notification handler code here

	ModeBpdlc =  m_bpdlc.GetCurSel() ;	
}

BOOL ZdmOriDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	GTZDM_ROAD *zdm;
	CButton *pbutton;
	/*
	m_LJorGM =0;
	pbutton=(CButton *)GetDlgItem(IDC_RADIOLJLP);
	pbutton->SetCheck(1);*/

	if(acdbOpenObject(zdm,ZpId, AcDb::kForRead)==Acad::eOk)
	{
		zdm->close();
		if(zdm->Xsw==1)
			m_bd = _T("保留一位小数");
		else if(zdm->Xsw==2)
			m_bd = _T("保留两位小数");
		else if(zdm->Xsw==3)
			m_bd = _T("保留三位小数");
		else
			m_bd = _T("不保留小数");

		if(zdm->VerRadius==0)
			VerRadius ="不取整";
		else if(zdm->VerRadius==1)
			VerRadius="1米整倍数";
		else if(zdm->VerRadius==2)
			VerRadius="10米整倍数";
		else
			VerRadius="100米整倍数";

		m_bpdlc.AddString(L"不取整") ;
		m_bpdlc.AddString(L"10米的整倍数");
		m_bpdlc.AddString(L"50米的整倍数");
		m_bpdlc.AddString(L"500米的整倍数");

		if(fabs(zdm->QZZ)<0.1)
		{
			m_bpdlc.SetCurSel(0) ;
			ModeBpdlc=0;
		}
		else if(fabs(zdm->QZZ-10.0)<0.1)
		{
			m_bpdlc.SetCurSel(1) ;
			ModeBpdlc=1;
		}
		else if(fabs(zdm->QZZ-50.0)<0.1)
		{
			m_bpdlc.SetCurSel(2) ;
			ModeBpdlc=2;
		}
		else
		{
			m_bpdlc.SetCurSel(3) ;
			ModeBpdlc=3;
		}

		if(zdm->TDFS==1)
			m_TDFS=_T("任意拖动");
		else if(zdm->TDFS==2)
			m_TDFS=_T("前坡度不变拖动");
		else
			m_TDFS=_T("后坡度不变拖动");
	}
	else
	{
		m_bpdlc.AddString(L"不取整") ;
		m_bpdlc.AddString(L"10米的整倍数");
		m_bpdlc.AddString(L"50米的整倍数");
		m_bpdlc.AddString(L"500米的整倍数");

		m_bpdlc.SetCurSel(0) ;
		ModeBpdlc=0;
	}

	m_LCBoxH.Format(L"%0.1lf",pORI_SCALE.LCBHigh);
	m_LCTextH.Format(L"%0.1lf",pORI_SCALE.LCBTEXTH);
	m_PQXBoxH.Format(L"%0.1lf",pORI_SCALE.PQXHIGH);
	m_PQXTextH.Format(L"%0.1lf",pORI_SCALE.PQXTEXTH);
	m_VScale.Format(L"%0.0lf",pORI_SCALE.VSCALE*1000);
	m_Hscale.Format(L"%0.0lf",pORI_SCALE.HSCALE*1000);
	m_Xori.Format(L"%0.3lf",pORI_SCALE.X_ORI);
	m_Yori.Format(L"%0.3lf",pORI_SCALE.Y_ORI);
	m_PDTextH.Format(L"%0.1lf",pORI_SCALE.PDTextH);

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


