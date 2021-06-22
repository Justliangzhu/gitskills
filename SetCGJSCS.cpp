// SetCGJSCS.cpp : implementation file
//

#include "stdafx.h"
#include "SetCGJSCS.h"
#include "XLDataBase.h"
#include "hdmdes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetCGJSCS dialog
SetCGJSCS::SetCGJSCS(JD_CENTER *pm,CWnd* pParent /*=NULL*/)
	: CDialog(SetCGJSCS::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetCGJSCS)
	m_HxCgJbl = _T("330");
	m_ZxCgJbl = _T("225");
	m_JKMode = _T("�����߼ӿ�");
	mdbname = pm->mdbname;
	roadname = pm->RoadName;
	Vj = pm->DV;
	m_pm = pm;
	m_flblen = _T("");
	m_hlblen = _T("");
	//}}AFX_DATA_INIT
}


void SetCGJSCS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetCGJSCS)
	DDX_Control(pDX, IDC_COMBOCGMODE, m_CgMode);
	DDX_Control(pDX, IDC_COMBOCGLEN, m_CgLenMode);
	DDX_Text(pDX, IDC_EDITHXCGJBL, m_HxCgJbl);
	DDX_Text(pDX, IDC_EDITZXCGJBL, m_ZxCgJbl);
	DDX_CBString(pDX, IDC_COMBOJKMODE, m_JKMode);
	DDX_Text(pDX, IDC_EDIT_FLBLEN, m_flblen);
	DDX_Text(pDX, IDC_EDIT_HLBLEN, m_hlblen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SetCGJSCS, CDialog)
	//{{AFX_MSG_MAP(SetCGJSCS)
	ON_CBN_SELCHANGE(IDC_COMBOCGLEN, OnSelchangeCombocglen)
	ON_CBN_SELCHANGE(IDC_COMBOCGMODE, OnSelchangeCombocgmode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetCGJSCS message handlers

BOOL SetCGJSCS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

	XLDataBase DBS;
	DBS.Read_XLDbs(mdbname,L"���߼������");
	DBS.Read_XLDbs(mdbname,L"��·����");
	GetDlgItem(IDC_EDIT_FLBLEN)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_HLBLEN)->EnableWindow(false);

	if(DBS.RoadXZ.Grade=="�ѵ�")
	{	
		HdmDes hdm;
		CString ZXmdbname,ZXRoadname;
		hdm.ReadBDFile();
		if(hdm.NBIDUAN>0)
		{
			CString xl2;
			xl2.Format(L"%s,%s",mdbname,roadname);
			int TorF;
			double ZXdml,ZDdml,flddml1,flddml2;

			if(hdm.FindWichLinkRoad(xl2,0,TorF,ZXmdbname,ZXRoadname,ZXdml,ZDdml,flddml1,flddml2))					  
			{
				if(TorF<0)
				{
					GetDlgItem(IDC_EDIT_FLBLEN)->EnableWindow(true);
					double cml1,cml2;

					cml1 = m_pm->TYLC(ZDdml);
					cml2 = m_pm->TYLC(flddml2);
					ACHAR str[80];
					if(cml1-cml2>0.001)
					{
						ads_rtos(cml1-cml2,2,3,str);
						m_flblen=str;
					}
				}
			}
			if(hdm.FindWichLinkRoad(xl2,1,TorF,ZXmdbname,ZXRoadname,ZXdml,ZDdml,flddml1,flddml2))					  
			{
				if(TorF<0)
				{
					GetDlgItem(IDC_EDIT_HLBLEN)->EnableWindow(true);
					double cml1,cml2;
					cml1 = m_pm->TYLC(ZDdml);
					cml2 = m_pm->TYLC(flddml2);
					ACHAR str[80];
					if(cml2-cml1>0.001)
					{
						ads_rtos(cml2-cml1,2,3,str);
						m_hlblen=str;
					}
				}
			}

		}
		else
			ads_printf(L"���ȼ���Ƕ�����!\n");
	}

	if(DBS.CalCGCs.m_HxCgJbl!="")
		m_HxCgJbl = DBS.CalCGCs.m_HxCgJbl;
	if(DBS.CalCGCs.m_ZxCgJbl!="")
		m_ZxCgJbl = DBS.CalCGCs.m_ZxCgJbl;
	if(DBS.CalCGCs.JkMode==0)
		m_JKMode = "���Լӿ�";
	else
		m_JKMode = "�����߼ӿ�";

	m_CgMode.AddString(L"0-������ָ�����Ե��ת");
	m_CgMode.AddString(L"1-���м��������ת");
	m_CgMode.AddString(L"2-�Ƹ����г���������ת");
	m_CgMode.AddString(L"3-��������ת");
	m_CgMode.AddString(L"4-���ڱ�Ե��ת");
	m_CgMode.AddString(L"5-�����Ե��ת");
	if(DBS.CalCGCs.CgMode>=0 && DBS.CalCGCs.CgMode<=5)
		CgMode = DBS.CalCGCs.CgMode;
	else 
		CgMode = 0;	
	m_CgMode.SetCurSel(CgMode);

	m_CgLenMode.AddString(L"0-��ȡ��");
	m_CgLenMode.AddString(L"1-1��������");
	m_CgLenMode.AddString(L"2-5��������");
	if(DBS.CalCGCs.CgLenMode>=0 && DBS.CalCGCs.CgLenMode<3)
		CgLenMode = DBS.CalCGCs.CgLenMode;
	else
		CgLenMode = 0;
	m_CgLenMode.SetCurSel(CgLenMode);

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void SetCGJSCS::OnSelchangeCombocglen() 
{
	// TODO: Add your control notification handler code here
	CgLenMode = m_CgLenMode.GetCurSel() ;
}

void SetCGJSCS::OnSelchangeCombocgmode() 
{
	// TODO: Add your control notification handler code here
	double CGJBL;
	CgMode =  m_CgMode.GetCurSel() ;
	if(CgMode<3.5)//������
		CGJBL = FindZXCGJBL(Vj,0);
	else
		CGJBL = FindZXCGJBL(Vj,1);
	m_ZxCgJbl.Format(L"%0.0lf",CGJBL);
	UpdateData(false);
}

void SetCGJSCS::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	XLDataBase DBS;

	DBS.CalCGCs.CgLenMode = CgLenMode;
	DBS.CalCGCs.CgMode = CgMode;
	DBS.CalCGCs.m_HxCgJbl = m_HxCgJbl;
	DBS.CalCGCs.m_ZxCgJbl = m_ZxCgJbl;
	if(m_JKMode=="���Լӿ�")
		DBS.CalCGCs.JkMode = 0;
	else
		DBS.CalCGCs.JkMode = 1;
	DBS.Write_XLDbs(mdbname,L"���߼������",L"");
	CDialog::OnOK();
}
