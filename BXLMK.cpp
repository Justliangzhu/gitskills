// BXLMK.cpp : implementation file
//

#include "stdafx.h"
#include "BXLMK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBXLMK dialog


CBXLMK::CBXLMK(CWnd* pParent /*=NULL*/)
	: CDialog(CBXLMK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBXLMK)
	m_bxedml = _T("");
	m_bxsdml = _T("");
	//}}AFX_DATA_INIT
}


void CBXLMK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBXLMK)
	DDX_Text(pDX, IDC_EDIT_EDDMLBX, m_bxedml);
	DDX_Text(pDX, IDC_EDIT_STDMLBX, m_bxsdml);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBXLMK, CDialog)
	//{{AFX_MSG_MAP(CBXLMK)
	ON_BN_CLICKED(IDC_RADIO_ZFD, OnRadioZfd)
	ON_BN_CLICKED(IDC_RADIO_LYD, OnRadioLyd)
	ON_BN_CLICKED(IDC_RADIO_CXD, OnRadioCxd)
	ON_BN_CLICKED(IDC_RADIO_YLJ, OnRadioYlj)
	ON_BN_CLICKED(IDC_RADIO_TLJ, OnRadioTlj)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBXLMK message handlers

void CBXLMK::OnRadioZfd() 
{
	// TODO: Add your control notification handler code here
	bxinf = 0;
}

void CBXLMK::OnRadioLyd() 
{
	// TODO: Add your control notification handler code here
	bxinf =1;
}

void CBXLMK::OnRadioCxd() 
{
	// TODO: Add your control notification handler code here
	bxinf =2;
}

void CBXLMK::OnRadioYlj() 
{
	// TODO: Add your control notification handler code here
	bxinf =3;
}

void CBXLMK::OnRadioTlj() 
{
	// TODO: Add your control notification handler code here
	bxinf =4;
}

BOOL CBXLMK::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pbtn;
	bxinf =3;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_YLJ);
	pbtn->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CBXLMK::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
