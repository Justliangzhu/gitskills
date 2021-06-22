// ReWriteFILE.cpp : implementation file
//

#include "stdafx.h"
#include "ReWriteFILE.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReWriteFILE dialog


CReWriteFILE::CReWriteFILE(CWnd* pParent /*=NULL*/)
	: CDialog(CReWriteFILE::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReWriteFILE)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CReWriteFILE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReWriteFILE)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReWriteFILE, CDialog)
	//{{AFX_MSG_MAP(CReWriteFILE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReWriteFILE message handlers

BOOL CReWriteFILE::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	GetDlgItem(IDC_STATIC_MES)->SetWindowText(MES);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
	