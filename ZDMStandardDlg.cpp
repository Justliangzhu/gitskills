// ZDMStandardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ZDMStandardDlg.h"
#include "XLdatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZDMStandardDlg dialog


ZDMStandardDlg::ZDMStandardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ZDMStandardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ZDMStandardDlg)

	m_A_JXRshuMin = _T("4000");
	m_A_YBRshu = _T("6000");
	m_MAXPD = _T("3");
	m_T_JXRShu = _T("11000");
	m_T_YBRshuMin = _T("17000");
	m_YBLShuMin = _T("250");
	m_YBPCMin = _T("400");
	m_JXPCMin = _T("300");
	m_JXLRShuMin = _T("100");
	//}}AFX_DATA_INIT
}


void ZDMStandardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ZDMStandardDlg)
	DDX_Text(pDX, IDC_EDIT_A_JXRshu, m_A_JXRshuMin);
	DDX_Text(pDX, IDC_EDIT_A_YBRshu, m_A_YBRshu);
	DDX_Text(pDX, IDC_EDIT_MAXPD, m_MAXPD);
	DDX_Text(pDX, IDC_EDIT_T_JXRshu, m_T_JXRShu);
	DDX_Text(pDX, IDC_EDIT_T_YBRshu, m_T_YBRshuMin);
	DDX_Text(pDX, IDC_EDIT_YBPCMin, m_YBPCMin);
	DDX_Text(pDX, IDC_EDIT_JXPCMin, m_JXPCMin);
	DDX_Text(pDX, IDC_EDIT_JXLRshuMin, m_JXLRShuMin);
	DDX_Text(pDX, IDC_EDIT_YBLShuMin, m_YBLShuMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ZDMStandardDlg, CDialog)
	//{{AFX_MSG_MAP(ZDMStandardDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZDMStandardDlg message handlers

BOOL ZDMStandardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	BAS_DRAW_FUN ob;
	FILE *fpr;
	ob.ReadWorkdir();

	CString curprj = Cworkdir + "\\project";   
	if((fpr = _wfopen(curprj,L"r"))==NULL)   
		curprj="";
	else
	{
		ACHAR pro[256];
		fwscanf(fpr,L"%s",pro);
		fclose(fpr);
		curprj = Cworkdir + "\\data\\" + pro + ".mdb";//当前方案


	}

	mdbname = curprj;
	if(mdbname=="")
		return true;

	ReadRoadProperty(mdbname);
	if(m_DV>0)
	{
		BAS_DRAW_FUN ob;
		ob.SetZdmMinPC(m_DV,m_MAXPD,m_YBPCMin,m_JXPCMin,
			m_T_YBRshuMin,m_T_JXRShu,
			m_A_YBRshu,m_A_JXRshuMin,
			m_YBLShuMin,m_JXLRShuMin);
	}
	CButton *pBtn;
	pBtn = (CButton *)GetDlgItem(IDC_RADIO2);
	pBtn->SetCheck(1);

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL ZDMStandardDlg::ReadRoadProperty(CString mdbname)
{
	XLDataBase xlmdb;
	xlmdb.Read_XLDbs(mdbname,L"线路属性");//读取线路数据库中的数据
	m_DV = xlmdb.RoadXZ.DV;

	return true;
}

BOOL ZDMStandardDlg::ReadZmbzFromMdb(CString mdbname)
{
	XLDataBase xlmdb;

	xlmdb.Read_XLDbs(mdbname,L"纵面设计标准");//读取线路数据库中的数据
	if(xlmdb.ZDMBZ.MAXPD!="")
		m_MAXPD=xlmdb.ZDMBZ.MAXPD;
	if(xlmdb.ZDMBZ.TRshu1!="")
		m_T_YBRshuMin=xlmdb.ZDMBZ.TRshu1;
	if(xlmdb.ZDMBZ.TRshu2!="")
		m_T_JXRShu=xlmdb.ZDMBZ.TRshu2;
	if(xlmdb.ZDMBZ.ARshu1!="")
		m_A_YBRshu=xlmdb.ZDMBZ.ARshu1;
	if(xlmdb.ZDMBZ.ARshu2!="")
		m_A_JXRshuMin=xlmdb.ZDMBZ.ARshu2;
	if(xlmdb.ZDMBZ.PCMin1!="")
		m_YBPCMin=xlmdb.ZDMBZ.PCMin1;
	if(xlmdb.ZDMBZ.PCMin2!="")
		m_JXPCMin=xlmdb.ZDMBZ.PCMin2;
	if(xlmdb.ZDMBZ.Lshu1!="")
		m_YBLShuMin=xlmdb.ZDMBZ.Lshu1;
	if(xlmdb.ZDMBZ.Lshu2!="")
		m_JXLRShuMin=xlmdb.ZDMBZ.Lshu2;

	return true;
}


void ZDMStandardDlg::OnOK() //写数据库
{
	// TODO: Add extra validation here
	CDialog::OnOK();
	if(mdbname=="")
	{
		ads_printf(L"未指定工程目录，无法保存!\n");
		return;
	}
	XLDataBase xlmdb;
	xlmdb.ZDMBZ.MAXPD = m_MAXPD;
	xlmdb.ZDMBZ.TRshu1 = m_T_YBRshuMin;
	xlmdb.ZDMBZ.TRshu2 = m_T_JXRShu;
	xlmdb.ZDMBZ.ARshu1 = m_A_YBRshu;
	xlmdb.ZDMBZ.ARshu2 = m_A_JXRshuMin;
	xlmdb.ZDMBZ.PCMin1 = m_YBPCMin;
	xlmdb.ZDMBZ.PCMin2 = m_JXPCMin;
	xlmdb.ZDMBZ.Lshu1 =  m_YBLShuMin;
	xlmdb.ZDMBZ.Lshu2 = m_JXLRShuMin;

	xlmdb.Write_XLDbs(mdbname,L"纵面设计标准",L"");//读取线路数据库中的数据

}

void ZDMStandardDlg::OnRadio1() //数据库
{
	// TODO: Add your control notification handler code here
	ReadZmbzFromMdb(mdbname);
	UpdateData(false);
}

void ZDMStandardDlg::OnRadio2() //规范
{
	// TODO: Add your control notification handler code here
	if(m_DV>0)
	{
		BAS_DRAW_FUN ob;
		ob.SetZdmMinPC(m_DV,m_MAXPD,m_YBPCMin,m_JXPCMin,
			m_T_YBRshuMin,m_T_JXRShu,
			m_A_YBRshu,m_A_JXRshuMin,
			m_YBLShuMin,m_JXLRShuMin);
	}

	UpdateData(false);	
}
