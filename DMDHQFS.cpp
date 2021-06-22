// DMDHQFS.cpp : implementation file
//

#include "stdafx.h"
#include "DMDHQFS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMDHQFS dialog


DMDHQFS::DMDHQFS(CWnd* pParent /*=NULL*/)
	: CDialog(DMDHQFS::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMDHQFS)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DMDHQFS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMDHQFS)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMDHQFS, CDialog)
	//{{AFX_MSG_MAP(DMDHQFS)
	ON_BN_CLICKED(IDC_RADIOFILE, OnRadiofile)
	ON_BN_CLICKED(IDC_RADIODTM, OnRadiodtm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMDHQFS message handlers

void DMDHQFS::OnRadiofile() 
{
	// TODO: Add your control notification handler code here
	m_buttonFile->SetCheck(1);
	m_buttonDmx->SetCheck(0);
	NCinf=0;
}

void DMDHQFS::OnRadiodtm() 
{
	// TODO: Add your control notification handler code here
	m_buttonFile->SetCheck(0);
	m_buttonDmx->SetCheck(1);
	NCinf=1;
	/*
	DMXFname="";
	GetDlgItem(IDC_STATICFiledmd)->SetWindowText(DMXFname);	*/

}

BOOL DMDHQFS::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_buttonFile =	(CButton *) GetDlgItem(IDC_RADIOFILE); 
	m_buttonDmx =  (CButton *) GetDlgItem(IDC_RADIODTM); 
	m_buttonFile->SetCheck(0);
	m_buttonDmx->SetCheck(1);	
	NCinf=1;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
