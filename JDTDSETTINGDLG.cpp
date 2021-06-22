// JDTDSETTINGDLG.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "JDTDSETTINGDLG.h"
#include "BaseSettting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JDTDSETTINGDLG dialog
JDTDSETTINGDLG::JDTDSETTINGDLG(CWnd* pParent /*=NULL*/)
	: CDialog(JDTDSETTINGDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(JDTDSETTINGDLG)
	m_TDFS = BaseSettting::JDTDFS;
	m_PRE = BaseSettting::JiaDianLMD*100;
	m_R = _T("任意");
	//}}AFX_DATA_INIT
}


void JDTDSETTINGDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JDTDSETTINGDLG)
	DDX_Radio(pDX, IDC_RADIO1, m_TDFS);
	DDX_Slider(pDX, IDC_SLIDER1, m_PRE);
	DDX_CBString(pDX, IDC_COMBO_R, m_R);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(JDTDSETTINGDLG, CDialog)
	//{{AFX_MSG_MAP(JDTDSETTINGDLG)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, OnCustomdrawSlider1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JDTDSETTINGDLG message handlers
void JDTDSETTINGDLG::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	BaseSettting::JDTDFS=m_TDFS;
	BaseSettting::JiaDianLMD=m_PRE/100.0;
	if(m_R=="任意")
		BaseSettting::Rstep=0;
	else if(m_R=="1米")
		BaseSettting::Rstep=1;
	else if(m_R=="10米")
		BaseSettting::Rstep=10;
	else if(m_R=="100米")
		BaseSettting::Rstep=100;
	else if(m_R=="1000米")
		BaseSettting::Rstep=1000;


	if(BaseSettting::JiaDianLMD<0.01)BaseSettting::JiaDianLMD=0.01;
	CDialog::OnOK();
}

BOOL JDTDSETTINGDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CString LMDStr;
	LMDStr.Format(L"%d %s",m_PRE,"%");
	GetDlgItem(IDC_STATIC_PRE)->SetWindowText(LMDStr);

	if(BaseSettting::Rstep==0)
		m_R="任意";
	else
		m_R.Format(L"%d米",BaseSettting::Rstep);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void JDTDSETTINGDLG::OnCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString LMDStr;
	LMDStr.Format(L"%d %s",m_PRE,"%");
	GetDlgItem(IDC_STATIC_PRE)->SetWindowText(LMDStr);
	*pResult = 0;
}
