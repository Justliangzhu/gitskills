// SAVEZDMDLG.cpp : implementation file
//

#include "stdafx.h"
#include "SAVEZDMDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SAVEZDMDLG dialog


SAVEZDMDLG::SAVEZDMDLG(CWnd* pParent /*=NULL*/)
	: CDialog(SAVEZDMDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(SAVEZDMDLG)
	m_infbri = FALSE;
	m_infdmx = FALSE;
	m_inftun = FALSE;
	m_infpd = FALSE;
	m_RoadName = _T("");
	m_ZDMTKNO = _T("");
	m_ZPXZ = _T("");
	m_Dbs = _T("");
	//}}AFX_DATA_INIT
}


void SAVEZDMDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SAVEZDMDLG)
	DDX_Control(pDX, IDC_COMBO_ZPXZ, m_ZPXZCrl);
	DDX_Control(pDX, IDC_COMBO_ZDMPRONO, m_ZDMTKCrl);
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_Check(pDX, IDC_CHECK_BRI, m_infbri);
	DDX_Check(pDX, IDC_CHECK_DMX, m_infdmx);
	DDX_Check(pDX, IDC_CHECK_TUN, m_inftun);
	DDX_Check(pDX, IDC_CHECK_ZDMPD, m_infpd);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_CBString(pDX, IDC_COMBO_ZDMPRONO, m_ZDMTKNO);
	DDX_CBString(pDX, IDC_COMBO_ZPXZ, m_ZPXZ);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_Dbs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SAVEZDMDLG, CDialog)
	//{{AFX_MSG_MAP(SAVEZDMDLG)
	ON_BN_CLICKED(IDC_BUTTON_PICKZDM, OnButtonPickzdm)
	ON_CBN_SELCHANGE(IDC_COMBO_ZDMPRONO, OnSelchangeComboZdmprono)
	ON_BN_CLICKED(IDC_BUTTON_browser, OnBUTTONbrowser)
	ON_BN_CLICKED(IDC_CHECK_ZDMPD, OnCheckZdmpd)
	ON_BN_CLICKED(IDC_CHECK_DMX, OnCheckDmx)
	ON_BN_CLICKED(IDC_CHECK_BRI, OnCheckBri)
	ON_BN_CLICKED(IDC_CHECK_TUN, OnCheckTun)
	//	ON_BN_CLICKED(IDC_CHECK_PM, OnCheckPm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SAVEZDMDLG message handlers

BOOL SAVEZDMDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	if(GlobalZDMTK.NZDM==0)
	{
		ads_alert(L"无纵断面图幅，不能保存纵断面方案!");
		CDialog::OnCancel();
		return 0;
	}
	CString str;
	int i;
	for(i=0;i<GlobalZDMTK.NZDM;i++)
	{
		str.Format(L"%d",i);
		m_ZDMTKCrl.AddString(str);
	}
	m_ZDMTKCrl.SetCurSel(0);
	m_ZDMTKNO = "0";

	m_ZPXZCrl.AddString(L"全幅");
	m_ZPXZCrl.AddString(L"左幅");
	m_ZPXZCrl.AddString(L"右幅");
	m_ZPXZCrl.SetCurSel(0);
	m_ZPXZ = "全幅";

	m_Dbs = GlobalZDMTK.ZDMTK[0].mdbname;
	m_RoadName = GlobalZDMTK.ZDMTK[0].RXDname;
	DBS.ReadAllRoadName(m_Dbs);
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);

	if(GlobalZDMTK.ZDMTK[0].zdm)
	{
		m_infpd = true;
		if(GlobalZDMTK.ZDMTK[0].zdm->Property!="")
			m_ZPXZ = GlobalZDMTK.ZDMTK[0].zdm->Property;
	}
	else
	{
		GetDlgItem(IDC_COMBO_ZPXZ)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_ZDMPD)->EnableWindow(false);
	}
	if(GlobalZDMTK.ZDMTK[0].pbri)
		m_infbri = true;
	else
		GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
	if(GlobalZDMTK.ZDMTK[0].ptun)
		m_inftun = true;
	else
		GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
	if(GlobalZDMTK.ZDMTK[0].dmx)
		m_infdmx = true;
	else
		GetDlgItem(IDC_CHECK_DMX)->EnableWindow(false);	

	UpdateData(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SAVEZDMDLG::OnButtonPickzdm() 
{
	// TODO: Add your control notification handler code here
	AcGePoint3d PT;
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	int izdm;
	CString mes;
	if(acedGetPoint(NULL,L"请在纵面图内拾取一点!\n",resPt)==RTNORM)
	{
		PT.x = resPt[X];
		PT.y = resPt[Y];
		izdm = GlobalZDMTK.FindWichZdm(PT);
		if(izdm>=0)
		{
			m_ZDMTKCrl.SetCurSel(izdm);
			m_ZDMTKNO.Format(L"%d",izdm);

			m_Dbs = GlobalZDMTK.ZDMTK[izdm].mdbname;
			m_RoadName = GlobalZDMTK.ZDMTK[izdm].RXDname;
			DBS.m_RoadSum=0;
			DBS.ReadAllRoadName(m_Dbs);
			m_RoadNameCrl.ResetContent();
			for(int i=0;i<DBS.m_RoadSum;i++)
				m_RoadNameCrl.AddString(DBS.Road[i].RoadName);

			if(GlobalZDMTK.ZDMTK[izdm].zdm)
			{
				m_infpd = true;
				if(GlobalZDMTK.ZDMTK[izdm].zdm->Property!="")
					m_ZPXZ = GlobalZDMTK.ZDMTK[izdm].zdm->Property;
				GetDlgItem(IDC_COMBO_ZPXZ)->EnableWindow(true);
				GetDlgItem(IDC_CHECK_ZDMPD)->EnableWindow(true);
			}
			else
			{
				GetDlgItem(IDC_COMBO_ZPXZ)->EnableWindow(false);
				GetDlgItem(IDC_CHECK_ZDMPD)->EnableWindow(false);
			}
			if(GlobalZDMTK.ZDMTK[izdm].pbri)
			{
				m_infbri = true;
				GetDlgItem(IDC_CHECK_BRI)->EnableWindow(true);
			}
			else
				GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
			if(GlobalZDMTK.ZDMTK[izdm].ptun)
			{
				m_inftun = true;
				GetDlgItem(IDC_CHECK_TUN)->EnableWindow(true);
			}
			else
				GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
			if(GlobalZDMTK.ZDMTK[izdm].dmx)
			{
				m_infdmx = true;
				GetDlgItem(IDC_CHECK_DMX)->EnableWindow(true);
			}
			else
				GetDlgItem(IDC_CHECK_DMX)->EnableWindow(false);	

			UpdateData(false);	
		}
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

}

void SAVEZDMDLG::OnSelchangeComboZdmprono() 
{
	// TODO: Add your control notification handler code here
	int i = m_ZDMTKCrl.GetCurSel();
	m_ZDMTKCrl.GetLBText(i,m_ZDMTKNO);
	int izdm = _wtoi(m_ZDMTKNO);
	m_Dbs = GlobalZDMTK.ZDMTK[izdm].mdbname;
	m_RoadName = GlobalZDMTK.ZDMTK[izdm].RXDname;
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_Dbs);
	m_RoadNameCrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);

	if(GlobalZDMTK.ZDMTK[izdm].zdm)
	{
		m_infpd = true;
		if(GlobalZDMTK.ZDMTK[izdm].zdm->Property!="")
			m_ZPXZ = GlobalZDMTK.ZDMTK[izdm].zdm->Property;
		GetDlgItem(IDC_COMBO_ZPXZ)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_ZDMPD)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_COMBO_ZPXZ)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_ZDMPD)->EnableWindow(false);
	}
	if(GlobalZDMTK.ZDMTK[izdm].pbri)
	{
		m_infbri = true;
		GetDlgItem(IDC_CHECK_BRI)->EnableWindow(true);
	}
	else
		GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
	if(GlobalZDMTK.ZDMTK[izdm].ptun)
	{
		m_inftun = true;
		GetDlgItem(IDC_CHECK_TUN)->EnableWindow(true);
	}
	else
		GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
	if(GlobalZDMTK.ZDMTK[izdm].dmx)
	{
		m_infdmx = true;
		GetDlgItem(IDC_CHECK_DMX)->EnableWindow(true);
	}
	else
		GetDlgItem(IDC_CHECK_DMX)->EnableWindow(false);	

	UpdateData(false);		
}

void SAVEZDMDLG::OnBUTTONbrowser() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fname(true,NULL,m_Dbs);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=Cworkdir+"\\data";
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	int i;
	if(fname.DoModal()==IDOK)
	{
		m_RoadNameCrl.ResetContent();
		m_Dbs =fname.GetPathName();
		DBS.m_RoadSum = 0;
		DBS.ReadAllRoadName(m_Dbs);
		for(i=0;i<DBS.m_RoadSum;i++)
			m_RoadNameCrl.AddString(DBS.Road[i].RoadName);			 
		UpdateData(false);
	}	
}

void SAVEZDMDLG::OnCheckZdmpd() 
{
	// TODO: Add your control notification handler code here
	m_infpd = !m_infpd;
}

void SAVEZDMDLG::OnCheckDmx() 
{
	// TODO: Add your control notification handler code here
	m_infdmx = !m_infdmx;
}

void SAVEZDMDLG::OnCheckBri() 
{
	// TODO: Add your control notification handler code here
	m_infbri = !m_infbri;
}

void SAVEZDMDLG::OnCheckTun() 
{
	// TODO: Add your control notification handler code here
	m_inftun = !m_inftun;
}

/*
void SAVEZDMDLG::OnCheckPm() 
{
// TODO: Add your control notification handler code here
m_infpm = !m_infpm;
}*/


void SAVEZDMDLG::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void SAVEZDMDLG::OnOK() 
{
	// TODO: Add extra validation here
	if(m_Dbs=="")
	{
		ads_alert(L"数据库名为空!");
		return;
	}
	if(m_RoadName=="")
	{
		ads_alert(L"道路名为空!");
		return;
	}
	CDialog::OnOK();
}
