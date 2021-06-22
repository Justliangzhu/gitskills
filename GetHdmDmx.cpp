// GetHdmDmx.cpp : implementation file
//

#include "stdafx.h"
#include "GetHdmDmx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetHdmDmx dialog


GetHdmDmx::GetHdmDmx(CWnd* pParent /*=NULL*/)
	: CDialog(GetHdmDmx::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetHdmDmx)
	m_YHP = 0.0;
	m_ZHP = 0.0;
	mode = 0;
	//}}AFX_DATA_INIT
}


void GetHdmDmx::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetHdmDmx)
	DDX_Text(pDX, IDC_EDIT_YHP, m_YHP);
	DDX_Text(pDX, IDC_EDIT_ZHP, m_ZHP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetHdmDmx, CDialog)
	//{{AFX_MSG_MAP(GetHdmDmx)
	ON_BN_CLICKED(IDC_RADIO_SC, OnRadioSc)
	ON_BN_CLICKED(IDC_RADIO_XD, OnRadioXd)
	ON_BN_CLICKED(IDC_RADIO_XN, OnRadioXn)
	ON_BN_CLICKED(IDC_RADIO_DTM, OnRadioDtm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetHdmDmx message handlers

void GetHdmDmx::OnRadioSc() //ʵ��
{
	// TODO: Add your control notification handler code here
	mode = 1;
}

void GetHdmDmx::OnRadioXd() //��Ը߲�
{
	// TODO: Add your control notification handler code here
	mode = 3;
}

void GetHdmDmx::OnRadioXn() //��������
{
	// TODO: Add your control notification handler code here
	mode = 2;
}

void GetHdmDmx::OnRadioDtm() //��ģ
{
	// TODO: Add your control notification handler code here
	mode = 0;
}

void GetHdmDmx::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

BOOL GetHdmDmx::OnInitDialog() //��ʼ��
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pBtn;

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_DTM);
    pBtn->SetCheck(1);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
