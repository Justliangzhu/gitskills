// TABDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TABDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTABDlg dialog


CTABDlg::CTABDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTABDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTABDlg)
	m_DuanName = _T("");
	m_Edml = 0.0;
	m_PrjMdb = _T("");
	m_RoadStr = _T("");
	m_Sdml = 0.0;
	m_Spage = 1;
	m_PageSum = 0;
	//}}AFX_DATA_INIT
}


void CTABDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTABDlg)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_DuanName, m_DuanName);
	DDX_Text(pDX, IDC_EDIT_Eml, m_Edml);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_PrjMdb);
	DDX_Text(pDX, IDC_EDIT_RoadStr, m_RoadStr);
	DDX_Text(pDX, IDC_EDIT_Sml, m_Sdml);
	DDX_Text(pDX, IDC_EDIT_startpage, m_Spage);
	DDX_Text(pDX, IDC_EDIT_PageSum, m_PageSum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTABDlg, CDialog)
	//{{AFX_MSG_MAP(CTABDlg)
	ON_BN_CLICKED(IDC_BUTTON_browser, OnBUTTONbrowser)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTABDlg message handlers

void CTABDlg::OnBUTTONbrowser() 
{
	// TODO: Add your control notification handler code here
	CString workdir,curpm;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	//	if(SYorMDB==1)
	//	{	
	workdir += "\\data";   
	CString DefaultName = m_PrjMdb;
	CFileDialog fname(true,NULL,DefaultName);
	fname.m_ofn.lpstrDefExt = L"mdb";
	fname.m_ofn.lpstrInitialDir=workdir;
	fname.m_ofn.Flags = OFN_CREATEPROMPT|OFN_LONGNAMES|OFN_EXPLORER;
	fname.m_ofn.lpstrFilter=L"线路方案数据库(*.mdb)\0*.mdb\0\0";
	if(fname.DoModal()==IDOK)
		m_PrjMdb =fname.GetPathName();
	mdb.m_RoadSum=0;
	mdb.ReadAllRoadName(m_PrjMdb);//读取线路数据库中断链表的数据
	//对所有道路循环，读取地面线，
	if(mdb.m_RoadSum<1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return ;
	}
	m_RoadName.ResetContent();
	for(int i=0; i<mdb.m_RoadSum; i++)
		m_RoadName.AddString(mdb.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	m_Sdml = mdb.Road[0].BLC;
	m_Edml = mdb.Road[0].ELC;

	UpdateData(false);
	//	}

}

void CTABDlg::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int iroad=m_RoadName.GetCurSel();
	RoadName = mdb.Road[iroad].RoadName;
	m_Sdml = mdb.Road[iroad].BLC;
	m_Edml = mdb.Road[iroad].ELC;
	UpdateData(false);

}

BOOL CTABDlg::OnInitDialog() //初始化
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	CString workdir,curpm,Path;
	char str[256];
	FILE *fpr;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	workdir += "\\data\\";   
	m_PrjMdb = workdir + Pro + ".mdb";


	mdb.ReadAllRoadName(m_PrjMdb);//读取线路数据库中断链表的数据
	//对所有道路循环，读取地面线，
	if(mdb.m_RoadSum<1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return 0;
	}

	m_RoadName.ResetContent();
	for(int i=0; i<mdb.m_RoadSum; i++)
	{
		m_RoadName.AddString(mdb.Road[i].RoadName);
		ads_printf(L"%d %s\n",i,mdb.Road[i].RoadName);
	}
	m_RoadName.SetCurSel(0);

	m_Sdml = mdb.Road[0].BLC;
	m_Edml = mdb.Road[0].ELC;
	RoadName = mdb.Road[0].RoadName;

	workdir=Cworkdir;
	//	workdir += "\\data\\";   
	//	CString tabcs;
	//    tabcs = workdir +"tabcs." + Pro;
	//	fpr = _wfopen(tabcs,L"r");
	//
	//	if(fpr)
	//	{
	//		fwscanf(fpr,L"%s",str);
	//    //    m_PrjMdb = str;
	//
	//		mdb.ReadAllRoadName(m_PrjMdb);//读取线路数据库中断链表的数据
	//		//对所有道路循环，读取地面线，
	//		m_RoadName.ResetContent();
	//		if(mdb.m_RoadSum>0)
	//		{
	//			for(int i=0; i<mdb.m_RoadSum; i++)
	//				m_RoadName.AddString(mdb.Road[i].RoadName);
	//			m_RoadName.SetCurSel(0);
	//
	//		}
	//		
	//        fwscanf(fpr,L"%s",str);
	//        RoadName = str;
	//		CString rname;
	//		for(int i=0; i<mdb.m_RoadSum; i++)
	//		{
	//			m_RoadName.GetLBText(i,rname);
	//			if(rname==RoadName)
	//			{
	//				m_RoadName.SetCurSel(i);
	//				break;
	//			}
	//		}
	//		
	//        fwscanf(fpr,L"%lf%lf",&m_Sdml,&m_Edml);
	//        fwscanf(fpr,L"%d%d",&m_Spage,&m_PageSum);
	//		if(!feof(fpr))
	//		{
	//	      fwscanf(fpr,L"%s",str);
	//       	   m_RoadStr = str;
	//		}
	//		if(!feof(fpr))
	//		{
	//          fwscanf(fpr,L"%s",str);
	//       	  m_DuanName =	str;
	//		}
	//		fclose(fpr);
	//	}

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTABDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	int iroad=m_RoadName.GetCurSel();
	if(iroad<0)
	{
		return;
	}
	RoadName = mdb.Road[iroad].RoadName;
	//写文件
	CString workdir,curpm,Path;
	char str[256];
	FILE *fpw;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();

	workdir=Cworkdir;
	workdir += "\\data\\";   
	CString tabcs;
	tabcs = workdir +"tabcs." + Pro;
	fpw = _wfopen(tabcs,L"w");
	if(fpw!=NULL)
	{
		fwprintf(fpw,L"%s\n",m_PrjMdb);
		fwprintf(fpw,L"%s\n",RoadName);
		fwprintf(fpw,L"%lf\n",m_Sdml);
		fwprintf(fpw,L"%lf\n",m_Edml);			
		fwprintf(fpw,L"%d\n",m_Spage);
		fwprintf(fpw,L"%d\n",m_PageSum);
		fwprintf(fpw,L"%s\n",m_RoadStr);
		fwprintf(fpw,L"%s\n",m_DuanName);
		fclose(fpw);
	}
	CDialog::OnOK();
}
