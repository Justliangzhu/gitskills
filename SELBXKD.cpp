// SELBXKD.cpp : implementation file
//

#include "stdafx.h"
#include "SELBXKD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSELBXKD dialog


CSELBXKD::CSELBXKD(CWnd* pParent /*=NULL*/)
	: CDialog(CSELBXKD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSELBXKD)
	m_inf = _T("");
	m_newinf = _T("");
	m_yyinf = _T("");
	//}}AFX_DATA_INIT
}


void CSELBXKD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSELBXKD)
	DDX_Text(pDX, IDC_STATICMES, m_inf);
	DDX_Text(pDX, IDC_STATIC_NEWMES, m_newinf);
	DDX_Text(pDX, IDC_STATIC_YYMES, m_yyinf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSELBXKD, CDialog)
	//{{AFX_MSG_MAP(CSELBXKD)
	ON_BN_CLICKED(IDC_RADIO_NEW, OnRadioNew)
	ON_BN_CLICKED(IDC_RADIO_OLD, OnRadioOld)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSELBXKD message handlers

void CSELBXKD::OnRadioNew() 
{
	// TODO: Add your control notification handler code here
	selinf=0; 
}

BOOL CSELBXKD::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	selinf=0;
	CButton *pbutton=(CButton *)GetDlgItem(IDC_RADIO_NEW);
	pbutton->SetCheck(1);    
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSELBXKD::OnRadioOld() 
{
	// TODO: Add your control notification handler code here
	selinf=1;
}
