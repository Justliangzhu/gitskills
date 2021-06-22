// InputZDM.cpp : implementation file
//

#include "stdafx.h"
#include "InputZDM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputZDM dialog


CInputZDM::CInputZDM(CWnd* pParent /*=NULL*/)
	: CDialog(CInputZDM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputZDM)
	m_BriInf = FALSE;
	m_PdxInf = FALSE;
	m_TunInf = FALSE;
	m_DmxInf = FALSE;
	m_RoadName = _T("主线");
	m_ZPXZ = _T("全幅");
	m_ZtfNo = _T("");
	m_MdbName = _T("");
	m_OriX = 0.0;
	m_OriY = 0.0;
	m_DmxInf = FALSE;
	//}}AFX_DATA_INIT
}


void CInputZDM::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputZDM)
	DDX_Control(pDX, IDC_COMBO_ZPPROPERTY, m_ZPXZCrl);
	DDX_Control(pDX, IDC_COMBO_ZTFNo, m_ZtfNoCrl);
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_Check(pDX, IDC_CHECK_BRI, m_BriInf);
	DDX_Check(pDX, IDC_CHECK_PDX, m_PdxInf);
	DDX_Check(pDX, IDC_CHECK_TUN, m_TunInf);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_CBString(pDX, IDC_COMBO_ZPPROPERTY, m_ZPXZ);
	DDX_CBString(pDX, IDC_COMBO_ZTFNo, m_ZtfNo);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_MdbName);
	DDX_Text(pDX, IDC_EDITORIX, m_OriX);
	DDX_Text(pDX, IDC_EDITORIY, m_OriY);
	DDX_Check(pDX, IDC_CHECK_DMX, m_DmxInf);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputZDM, CDialog)
	//{{AFX_MSG_MAP(CInputZDM)
	ON_BN_CLICKED(IDC_BUTTON_SCANZDM, OnButtonScanzdm)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_RADIOALLZM, OnRadioallzm)
	ON_BN_CLICKED(IDC_RADIO_PART, OnRadioPart)
	ON_CBN_SELCHANGE(IDC_COMBO_ZTFNo, OnSelchangeCOMBOZTFNo)
	ON_BN_CLICKED(IDC_BUTTON_pick, OnBUTTONpick)
	ON_BN_CLICKED(IDC_CHECK_PDX, OnCheckPdx)
	ON_BN_CLICKED(IDC_CHECK_TUN, OnCheckTun)
	ON_BN_CLICKED(IDC_CHECK_BRI, OnCheckBri)
	ON_BN_CLICKED(IDC_CHECK_DMX, OnCheckDmx)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CInputZDM::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputZDM message handlers

void CInputZDM::OnButtonScanzdm() 
{
	// TODO: Add your control notification handler code here
	CFileDialog fname(true,NULL,m_MdbName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=Cworkdir+"\\data";
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	int i;
	if(fname.DoModal()==IDOK)
	{
		/*
		num=m_RoadNameCrl.GetCount();
		for(i=0;i<num;i++)
		m_RoadNameCrl.DeleteString(i);*/

		m_RoadNameCrl.ResetContent();
		m_MdbName =fname.GetPathName();
		DBS.m_RoadSum=0;
		DBS.ReadAllRoadName(m_MdbName);
		for(i=0;i<DBS.m_RoadSum;i++)
			m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
		m_RoadNameCrl.SetCurSel(0);

		/*
		num=m_ZPXZCrl.GetCount();
		for(i=0;i<num;i++)
		m_ZPXZCrl.DeleteString(i);*/
		m_ZPXZCrl.ResetContent();
		DBS.ReadAllXZ(m_MdbName,L"主线");
		m_ZPXZCrl.AddString(L"全幅");
		if(DBS.RoadZ=="左幅")
			m_ZPXZCrl.AddString(L"左幅");
		if(DBS.RoadY=="右幅")
			m_ZPXZCrl.AddString(L"右幅");
		m_ZPXZCrl.SetCurSel(0);
		UpdateData(false);
	}
}

void CInputZDM::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i,m_RoadName);
	m_ZPXZCrl.ResetContent();
	DBS.ReadAllXZ(m_MdbName,m_RoadName);
	m_ZPXZCrl.AddString(L"全幅");
	if(DBS.RoadZ=="左幅")
		m_ZPXZCrl.AddString(L"左幅");
	if(DBS.RoadY=="右幅")
		m_ZPXZCrl.AddString(L"右幅");
	m_ZPXZCrl.SetCurSel(0);
	UpdateData(false);	
}

void CInputZDM::OnRadioallzm() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_SETPT)->SetWindowText(L"指定绘图原点");
	CButton *pbtn;
	DrawMode = 0;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOALLZM);
	pbtn->SetCheck(1); 	
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_PART);
	pbtn->SetCheck(0);
	GetDlgItem(IDC_COMBO_ZTFNo)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_PDX)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_DMX)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
}

void CInputZDM::OnRadioPart() 
{
	// TODO: Add your control notification handler code here
	DrawMode = 1;
	GetDlgItem(IDC_STATIC_SETPT)->SetWindowText(L"在纵面图幅内拾取一点");

	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOALLZM);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_PART);
	pbtn->SetCheck(1);

	GetDlgItem(IDC_COMBO_ZTFNo)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_PDX)->EnableWindow(true);
	int zno = _wtoi(m_ZtfNo);
	m_OriX = GlobalZDMTK.ZDMTK[zno].X0-30;
	m_OriY = GlobalZDMTK.ZDMTK[zno].Y0-(pORI_SCALE.PQXHIGH+pORI_SCALE.LCBHigh+20);
	m_PdxInf = true;
	pbtn = (CButton *)GetDlgItem(IDC_CHECK_PDX);
	pbtn->SetCheck(1);
	GetDlgItem(IDC_CHECK_DMX)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_BRI)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_TUN)->EnableWindow(true);
	UpdateData(false);	
}

void CInputZDM::OnSelchangeCOMBOZTFNo() 
{
	// TODO: Add your control notification handler code here
	int i = m_ZtfNoCrl.GetCurSel();
	m_ZtfNoCrl.GetLBText(i,m_ZtfNo);
	int zno = _wtoi(m_ZtfNo);
	m_OriX = GlobalZDMTK.ZDMTK[zno].X0-30;
	m_OriY = GlobalZDMTK.ZDMTK[zno].X0-(pORI_SCALE.PQXHIGH+pORI_SCALE.LCBHigh+20);
	for(i=0;i<DBS.m_RoadSum;i++)
	{		
		if(DBS.Road[i].RoadName==GlobalZDMTK.ZDMTK[zno].RXDname)
			break;
	}
	m_RoadName = GlobalZDMTK.ZDMTK[zno].RXDname;
	m_RoadNameCrl.SetCurSel(i);
	UpdateData(false);	
}

void CInputZDM::OnBUTTONpick() 
{
	// TODO: Add your control notification handler code here
	AcGePoint3d PT;
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.
	int izdm,i;
	CString mes;
	UpdateData();	
	if(acedGetPoint(NULL,L"请在纵面图内拾取一点!\n",resPt)==RTNORM)
	{
		if(DrawMode==0)
		{
			m_OriX = resPt[X];
			m_OriY = resPt[Y];
		}
		else
		{
			PT.x = resPt[X];
			PT.y = resPt[Y];
			izdm = GlobalZDMTK.FindWichZdm(PT);
			if(izdm>=0)
			{
				m_ZtfNoCrl.SetCurSel(izdm);
				m_ZtfNo.Format(L"%d",izdm);
				m_OriX = GlobalZDMTK.ZDMTK[izdm].X0-30;
				m_OriY = GlobalZDMTK.ZDMTK[izdm].Y0-(pORI_SCALE.PQXHIGH+pORI_SCALE.LCBHigh+20);
				for(i=0;i<DBS.m_RoadSum;i++)
					if(DBS.Road[i].RoadName==GlobalZDMTK.ZDMTK[izdm].RXDname)
						break;
				m_RoadName = GlobalZDMTK.ZDMTK[izdm].RXDname;
				m_RoadNameCrl.SetCurSel(i);
			}
		}
	}

	m_OriX=double(m_OriX*1000)/1000.0;
	m_OriY=double(m_OriY*1000)/1000.0;

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	UpdateData(false);	
}

void CInputZDM::OnCheckPdx() 
{
	// TODO: Add your control notification handler code here
	m_PdxInf = !m_PdxInf;
	if(m_PdxInf)
	{
		GetDlgItem(IDC_CHECK_BRI)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_TUN)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
	}
}

void CInputZDM::OnCheckTun() 
{
	// TODO: Add your control notification handler code here
	m_TunInf = ! m_TunInf;
}

BOOL CInputZDM::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CButton *pbtn;
	DrawMode = 0;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOALLZM);
	pbtn->SetCheck(1); 	
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_PART);
	pbtn->SetCheck(0);
	if(GlobalZDMTK.NZDM==0)
		GetDlgItem(IDC_RADIO_PART)->EnableWindow(false);
	CString str;
	int i;
	for( i=0;i<GlobalZDMTK.NZDM;i++)
	{
		str.Format(L"%d",i);
		m_ZtfNoCrl.AddString(str);
	}
	if(GlobalZDMTK.NZDM>0)
	{
		m_ZtfNoCrl.SetCurSel(0);
		m_ZtfNo = "0";
	}
	GetDlgItem(IDC_COMBO_ZTFNo)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_DMX)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_PDX)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_BRI)->EnableWindow(false);
	GetDlgItem(IDC_CHECK_TUN)->EnableWindow(false);
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	m_MdbName = Cworkdir+"\\DATA\\";
	m_MdbName = m_MdbName + Pro+".mdb";


	DBS.ReadAllRoadName(m_MdbName);
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);

	DBS.ReadAllXZ(m_MdbName,L"主线");
	m_ZPXZCrl.AddString(L"全幅");
	if(DBS.RoadZ=="左幅")
		m_ZPXZCrl.AddString(L"左幅");
	if(DBS.RoadY=="右幅")
		m_ZPXZCrl.AddString(L"右幅");
	m_ZPXZCrl.SetCurSel(0);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInputZDM::OnCheckBri() 
{
	// TODO: Add your control notification handler code here
	m_BriInf = !m_BriInf;
}

void CInputZDM::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	CDialog::OnOK();
}

void CInputZDM::OnCheckDmx() 
{
	// TODO: Add your control notification handler code here
	m_DmxInf = !m_DmxInf;
}


void CInputZDM::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnOK();
}
