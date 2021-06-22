// HDMTdlg.cpp : implementation file
//

#include "stdafx.h"
#include "HDMTdlg.h"
#include "HDMDES.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern HdmDes HDM;
/////////////////////////////////////////////////////////////////////////////
// HDMTdlg dialog


HDMTdlg::HDMTdlg(CWnd* pParent /*=NULL*/)
	: CDialog(HDMTdlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(HDMTdlg)
	m_TKNAME = _T("");
	m_YK = 10.0;
	m_ZK = 10.0;
	m_Lnum = _T("1");
	m_SX = 0.0;
	m_SY = 0.0;
	m_IfOut = FALSE;
	m_startpage = 1;
	m_IfBZ = FALSE;

	//}}AFX_DATA_INIT
}


void HDMTdlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HDMTdlg)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Control(pDX, IDC_COMBO_SKML, m_SKML);
	DDX_Control(pDX, IDC_COMBO_EKML, m_EKML);
	DDX_Text(pDX, IDC_EDIT_TKNAME, m_TKNAME);
	DDX_Text(pDX, IDC_EDIT_YK, m_YK);
	DDX_Text(pDX, IDC_EDIT_ZK, m_ZK);
	DDX_CBString(pDX, IDC_COMBO_LNUM, m_Lnum);
	DDX_Text(pDX, IDC_EDIT_SX, m_SX);
	DDX_Text(pDX, IDC_EDIT_SY, m_SY);
	DDX_Text(pDX, IDC_EDIT_startpage, m_startpage);
	DDX_Check(pDX, IDC_CHECK_IfBZ, m_IfBZ);
	DDX_Text(pDX, IDC_EDIT_HDMSCALE, m_scale);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HDMTdlg, CDialog)
	//{{AFX_MSG_MAP(HDMTdlg)
	ON_BN_CLICKED(IDC_BUTTON_browser, OnBUTTONbrowser)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_BUTTON_pick, OnBUTTONpick)
	ON_BN_CLICKED(IDC_CHECK_IfBZ, OnCHECKIfBZ)
	ON_EN_CHANGE(IDC_EDIT_HDMSCALE, OnChangeEditHdmscale)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HDMTdlg message handlers

void HDMTdlg::OnBUTTONbrowser() 
{
	// TODO: Add your control notification handler code here
	CString workdir,curpm,Path;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{
	workdir += "\\data\\";   
	CString DefaultName = workdir + "hdmtk.dwg";
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"dwg";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"(*.dwg)\0*.dwg\0\0";
	if(fname.DoModal()==IDOK)
		m_TKNAME=fname.GetPathName();//横断面图框文件
	UpdateData(false);	
}

BOOL HDMTdlg::OnInitDialog() //初始化
{
	CDialog::OnInitDialog();

	m_scale = _T("200");
	// TODO: Add extra initialization here
	int i;
	for(i=0; i<HDM.RoadSum; i++)
		m_RoadName.AddString(HDM.road[i].roadname);
	m_RoadName.SetCurSel(0);

	i=0;
	int j;
	for( j=0;j<HDM.road[0].NCRS;j++)
	{
		if(HDM.road[i].hdmdes[j].Lnpt>0 || HDM.road[i].hdmdes[j].Rnpt>0 ||
			HDM.road[i].hdmdes[j].Y_Lnpt>0 || HDM.road[i].hdmdes[j].Y_Rnpt>0)
		{
			m_SKML.AddString(HDM.road[i].hdmdes[j].ckml);
			m_EKML.AddString(HDM.road[i].hdmdes[j].ckml);
		}
	}


	if(HDM.road[0].NCRS>0)
	{
		m_EKML.SetCurSel(m_EKML.GetCount()-1);
		m_SKML.SetCurSel(0);
	}

	CString workdir,curpm,Path;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	workdir += "\\data\\";   
	m_TKNAME = workdir + "hdmtk.dwg";
	GetDlgItem(IDC_EDIT_startpage)->EnableWindow(false);		
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void HDMTdlg::OnSelchangeCOMBORoadName() //选择道路
{
	SetFocus();
	CString text,rxd,ckml;
	int i,index,irxd;
	index=m_RoadName.GetCurSel();
	m_RoadName.GetLBText(index,text);
	m_SKML.ResetContent();//清空
	m_EKML.ResetContent();//清空

	//找到道路
	for(i=0; i<HDM.RoadSum; i++)
		if(HDM.road[i].roadname==text)
			break;

	if(i<HDM.RoadSum)
		for(int j=0; j<HDM.road[i].NCRS; j++)
		{
			if(HDM.road[i].hdmdes[j].Lnpt>0 || HDM.road[i].hdmdes[j].Rnpt>0 ||
				HDM.road[i].hdmdes[j].Y_Lnpt>0 || HDM.road[i].hdmdes[j].Y_Rnpt>0)
			{
				m_SKML.AddString(HDM.road[i].hdmdes[j].ckml);
				m_EKML.AddString(HDM.road[i].hdmdes[j].ckml);
			}
		}				
		if(HDM.road[i].NCRS>0)
		{
			m_SKML.SetCurSel(0);
			m_EKML.SetCurSel(m_EKML.GetCount()-1);
		}

}

void HDMTdlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
	int index;
	index=m_RoadName.GetCurSel();
	m_RoadName.GetLBText(index,roadname);
	lnum = _wtoi(m_Lnum);


	index=m_SKML.GetCurSel();
	m_SKML.GetLBText(index,CKSml);
	index=m_EKML.GetCurSel();
	m_EKML.GetLBText(index,CKEml);
}

void HDMTdlg::OnBUTTONpick() //拾取
{
	// TODO: Add your control notification handler code here

	UpdateData(true);
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	ads_point PT;
	ads_getpoint(NULL,L"\n请在屏幕上采集第一幅图绘图原点", PT);
	m_SX=int(PT[X]);
	m_SY=int(PT[Y]);// 保留三为有效数字
	UpdateData(false);

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/		

}

void HDMTdlg::OnCHECKIfBZ() 
{
	// TODO: Add your control notification handler code here
	m_IfBZ = !m_IfBZ;
	if(m_IfBZ)
	{
		GetDlgItem(IDC_EDIT_startpage)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_EDIT_startpage)->EnableWindow(false);
	}
}

void HDMTdlg::OnChangeEditHdmscale() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
