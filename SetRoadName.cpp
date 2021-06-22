// SetRoadName.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "SetRoadName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetRoadName dialog


CSetRoadName::CSetRoadName(CString name,CWnd* pParent /*=NULL*/)
	: CDialog(CSetRoadName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetRoadName)
	mdbname = name;
	m_ZPXZ = _T("È«·ù");
	//}}AFX_DATA_INIT
}


void CSetRoadName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetRoadName)
	DDX_Control(pDX, IDC_COMBOSetZPXZ, m_ZPXZCrl);
	DDX_Control(pDX, IDC_COMBOSetRoadName, m_RoadName);
	DDX_CBString(pDX, IDC_COMBOSetZPXZ, m_ZPXZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetRoadName, CDialog)
	//{{AFX_MSG_MAP(CSetRoadName)
	ON_CBN_SELCHANGE(IDC_COMBOSetRoadName, OnSelchangeCOMBOSetRoadName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetRoadName message handlers

BOOL CSetRoadName::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	int i;

	DBS.ReadAllRoadName(mdbname);
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadName.AddString(DBS.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	RoadName = DBS.Road[0].RoadName;
	DBS.ReadAllXZ(mdbname,RoadName);
	m_ZPXZCrl.AddString(L"È«·ù");
	if(DBS.RoadZ=="×ó·ù")
		m_ZPXZCrl.AddString(L"×ó·ù");
	if(DBS.RoadY=="ÓÒ·ù")
		m_ZPXZCrl.AddString(L"ÓÒ·ù");
	m_ZPXZCrl.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetRoadName::OnSelchangeCOMBOSetRoadName() 
{
	// TODO: Add your control notification handler code here
	int roadno=0;
	roadno=m_RoadName.GetCurSel();
	RoadName = DBS.Road[roadno].RoadName;
	DBS.ReadAllXZ(mdbname,RoadName);
	m_ZPXZCrl.AddString(L"È«·ù");
	if(DBS.RoadZ=="×ó·ù")
		m_ZPXZCrl.AddString(L"×ó·ù");
	if(DBS.RoadY=="ÓÒ·ù")
		m_ZPXZCrl.AddString(L"ÓÒ·ù");
	m_ZPXZCrl.SetCurSel(0);	
}

void CSetRoadName::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
