// DrawPmDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DrawPmDlg.h"
#include "CWorkDir.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern ACHAR SysPath[1024];
/////////////////////////////////////////////////////////////////////////////
// CDrawPmDlg dialog


CDrawPmDlg::CDrawPmDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawPmDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDrawPmDlg)
	m_ROAD = L"����";
	m_ZD = 0.0;
	m_WHT = 3;
	m_QD = 0.0;
	m_Dbs = L"";
	m_SCALE = L"2000";
	m_XSW = L"";
	m_LCBZ =L"";
	m_DrawBM = FALSE;
	m_DrawDX = FALSE;
	m_DrawTun = TRUE;
	m_DrawCulvet = TRUE;
	m_DrawDZQ = TRUE;
	m_ZcLen = 60;
	m_YcLen = 60;
	m_DelEntInf = FALSE;
	m_BZRAL = FALSE;
	m_ZxWid = L"0.0";
	m_drawfldzdk = FALSE;
	pm = NULL;
	m_YDXWid = L"1.0";
	m_DrawJD = FALSE;
	m_ZDKMode = 0;
	//}}AFX_DATA_INIT
}

void CDrawPmDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawPmDlg)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_ROAD);
	DDX_Text(pDX, IDC_EDIT_ZD, m_ZD);
	DDX_Text(pDX, IDC_EDIT_WHT, m_WHT);
	DDX_Text(pDX, IDC_EDIT_QD, m_QD);
	DDX_Text(pDX, IDC_EDIT_Dbs, m_Dbs);
	DDX_CBString(pDX, IDC_COMBO_SCALE, m_SCALE);
	DDX_CBString(pDX, IDC_COMBO_XSWS, m_XSW);
	DDX_CBString(pDX, IDC_COMBO_LCBZ, m_LCBZ);
	DDX_Check(pDX, IDC_CHECK_BM, m_DrawBM);
	DDX_Check(pDX, IDC_CHECK_DX, m_DrawDX);
	DDX_Check(pDX, IDC_CHECK_TUN, m_DrawTun);
	DDX_Check(pDX, IDC_CHECK_SMALLBRIDGE, m_DrawCulvet);
	DDX_Check(pDX, IDC_CHECK_BMBRIDGE, m_DrawDZQ);
	DDX_Text(pDX, IDC_EDIT_YCLen, m_ZcLen);
	DDX_Text(pDX, IDC_EDIT_ZCLen, m_YcLen);
	DDX_Check(pDX, IDC_CHECK_DELENT, m_DelEntInf);
	DDX_Check(pDX, IDC_CHECK_SHOWDBS, m_BZRAL);
	DDX_Text(pDX, IDC_EDIT_ZXWid, m_ZxWid);
	DDX_Check(pDX, IDC_CHECK_FLDZDK, m_drawfldzdk);
	DDX_Text(pDX, IDC_EDIT_YDXWID, m_YDXWid);
	DDX_Radio(pDX, IDC_RADIO_DrawALL, m_DrawAll);
	DDX_Radio(pDX, IDC_RADIOXWT, DrawMode);
	DDX_Radio(pDX, IDC_RADIOLCWID, m_ZDKMode);
	DDX_Check(pDX, IDC_CHECK_DRAWJD, m_DrawJD);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawPmDlg, CDialog)
	//{{AFX_MSG_MAP(CDrawPmDlg)
	ON_BN_CLICKED(IDC_BUTTON_OpenDbs, OnBUTTONOpenDbs)
	ON_BN_CLICKED(IDC_RADIO_DrawALL, OnRADIODrawALL)
	ON_BN_CLICKED(IDC_RADIO_DrawSingle, OnRADIODrawSingle)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	ON_BN_CLICKED(IDC_RADIOXWT, OnRadioxwt)
	ON_BN_CLICKED(IDC_RADIOPMZTT, OnRadiopmztt)
	ON_BN_CLICKED(IDC_RADIOZDT, OnRadiozdt)
	ON_BN_CLICKED(IDC_RADIOLCWID, OnRadiolcwid)
	ON_BN_CLICKED(IDC_RADIONECOR, OnRadionecor)
	ON_BN_CLICKED(IDC_RADIOBOTH, OnRadioboth)
	ON_BN_CLICKED(IDC_RADIONONE, OnRadionone)
	ON_BN_CLICKED(IDC_CHECK_BMBRIDGE, OnCheckBmbridge)
	ON_BN_CLICKED(IDC_CHECK_BM, OnCheckBm)
	ON_BN_CLICKED(IDC_CHECK_SMALLBRIDGE, OnCheckSmallbridge)
	ON_BN_CLICKED(IDC_CHECK_TUN, OnCheckTun)
	ON_BN_CLICKED(IDC_CHECK_DX, OnCheckDx)
	ON_BN_CLICKED(IDC_CHECK_DELENT, OnCheckDelent)
	ON_BN_CLICKED(IDC_CHECK_SHOWDBS, OnCheckShowdbs)
	ON_BN_CLICKED(IDC_CHECK_FLDZDK, OnCheckFldzdk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawPmDlg message handlers

BOOL CDrawPmDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();



	// TODO: Add extra initialization here
	if (m_Dbs.IsEmpty())
	{
		BAS_DRAW_FUN ob;
		ob.ReadWorkdir();
		if(Pro==L"")
		{
			CDialog::OnCancel();
			return 0;
		}
		m_Dbs = Cworkdir+L"\\DATA\\";
		m_Dbs = m_Dbs + Pro+L".mdb";
	}

	if (m_Dbs.Find(L"ģ��.mdb")>-1)//����ģ��
	{
		CString FName = m_Dbs;
		FName.Replace(L".mdb",L".pmcs");

		if (!ReadCSFile(FName))
		{
			AfxMessageBox(FName + L"������!");
			return FALSE;
		}

		if (m_DrawAll==0)
		{
			GetDlgItem(IDC_COMBO_RoadName)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_QD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_ZD)->EnableWindow(false);
		}
		m_RoadName.SetCurSel(0);
		m_QD = -1.0;
		m_ZD = -1.0;
	}
	else
	{
		DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",L"����");
		DBS.Read_XLDbs(m_Dbs,L"������",L"����");
		if(DBS.XYNum>0)
		{
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
		}
		else
		{
			AfxMessageBox(L"�㻹û��������·����!");
			CDialog::OnCancel();
			return 0;
		}

		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"����");
		DBS.ReadAllRoadName(m_Dbs);
		for(int i=0;i<DBS.m_RoadSum;i++)
			m_RoadName.AddString(DBS.Road[i].RoadName);

		CString FName = m_Dbs;
		FName.Replace(L".mdb",L".pmcs");

		if (!ReadCSFile(FName))
		{
			CString tempCStr = SysPath;
			FName = tempCStr + L"\\lib\\ģ��.pmcs";
			if (!ReadCSFile(FName))
			{
				AfxMessageBox(FName + L"������!");
				return FALSE;
			}
			//20190707	�޸�һЩĬ��ֵ
			m_ZxWid = L"1.4";
			m_LCBZ = L"����ע";
		}

		if (m_DrawAll==0)
		{
			GetDlgItem(IDC_COMBO_RoadName)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_QD)->EnableWindow(false);
			GetDlgItem(IDC_EDIT_ZD)->EnableWindow(false);
		}
		m_RoadName.SetCurSel(0);
		ACHAR str[180];
		ads_rtos(pm->DLArray[0].ELC,2,3,str);
		m_QD = _wtof(str);
		ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
		m_ZD = _wtof(str);
	}

	//else
	//{
	//	m_XSW = _T("��λ");
	//	m_LCBZ = _T("���ױ�");
	//	m_DrawAll = 0;//0 or 1
	//	CButton *pbtn;
	//	GetDlgItem(IDC_COMBO_RoadName)->EnableWindow(false);
	//	GetDlgItem(IDC_EDIT_QD)->EnableWindow(false);
	//	GetDlgItem(IDC_EDIT_ZD)->EnableWindow(false);
	//	GetDlgItem(IDC_CHECK_SMALLBRIDGE)->EnableWindow(true);
	//	GetDlgItem(IDC_CHECK_DELENT)->EnableWindow(true);
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawSingle);
	//	pbtn->SetCheck(0); 
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawALL);
	//	pbtn->SetCheck(1); 
	//	DrawMode = 0;
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIOXWT);
	//	pbtn->SetCheck(1); 
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIOPMZTT);
	//	pbtn->SetCheck(0);
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIOZDT);
	//	pbtn->SetCheck(0);
	//	m_ZDKMode = 0;//���+���
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIOLCWID);
	//	pbtn->SetCheck(1); 
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIONECOR);
	//	pbtn->SetCheck(0);
	//	pbtn = (CButton *)GetDlgItem(IDC_RADIOBOTH);
	//	pbtn->SetCheck(0);


	//	m_DrawDZQ = true;
	//	m_DrawTun = true;
	//	m_DrawCulvet = true;
	//	////////////////////////////////////////////////////////
	//	m_RoadName.SetCurSel(0);
	//	char str[180];
	//	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	//	m_QD = _wtof(str);
	//	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	//	m_ZD = _wtof(str);
	//}
	UpdateData(false);
	if (DrawMode==0)
	{
		OnRadioxwt();
	}
	if (DrawMode==1)
	{
		OnRadiopmztt();
	}
	if (DrawMode==2)
	{
		OnRadiozdt();
	}

	UpdateData(FALSE);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDrawPmDlg::OnBUTTONOpenDbs() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256];
	BAS_DRAW_FUN ob;	
	ob.ReadWorkdir();
	_tcscpy(DefaultPath,Cworkdir);
	_tcscat(DefaultPath,L"\\DATA\\");
	_tcscat(DefaultPath,Pro);
	_tcscat(DefaultPath,L".mdb");
	//��ȱʡͼ����
	//////////////////////////////////////////////////////////////////////////


	if ( (ads_getfiled( L"���������ݿ��� *.mdb",DefaultPath,L"mdb",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_Dbs=fname;
	if(m_Dbs==L""){AfxMessageBox(L"δָ�����ݿ�!");return;}

	//	DBS.Read_XLDbs(m_Dbs,L"���Ƶ��",L"����");
	DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",L"����");		
	DBS.Read_XLDbs(m_Dbs,L"������",L"����");
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,L"����");
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_Dbs);
	int i;
	/*
	int num=m_RoadName.GetCount();
	for(i=0;i<num;i++)
	m_RoadName.DeleteString(i);*/
	m_RoadName.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadName.AddString(DBS.Road[i].RoadName);
	m_RoadName.SetCurSel(0);
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_QD = _wtof(str);
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_ZD = _wtof(str);

	UpdateData(false);
	///////////////////////////////////////////////////////	
	////////////////////////////////////////////////////////	
}

void CDrawPmDlg::OnRADIODrawALL() 
{
	// TODO: Add your control notification handler code here
	m_DrawAll = 0;
	GetDlgItem(IDC_COMBO_RoadName)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_QD)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZD)->EnableWindow(false);
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawSingle);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawALL);
	pbtn->SetCheck(1); 
}

void CDrawPmDlg::OnRADIODrawSingle() 
{
	// TODO: Add your control notification handler code here
	m_DrawAll = 1;
	GetDlgItem(IDC_COMBO_RoadName)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_QD)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_ZD)->EnableWindow(true);
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawSingle);
	pbtn->SetCheck(1); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIO_DrawALL);
	pbtn->SetCheck(0); 
}

void CDrawPmDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();

	FILE *fp=NULL;
	if (m_Dbs.Find(L"ģ��")>-1)
	{
		CString PmDrawSetName = m_Dbs;
		PmDrawSetName.Replace(L".mdb",L".PmDrawSet");
		fp=_wfopen(PmDrawSetName,L"w");
	}
	else
	{
		CWorkDir WordDir;
		fp=_wfopen(WordDir.PmDrawSetFile,L"w");
	}
	if(fp)
	{	
		fwprintf(fp,L"%s\n",m_ROAD);
		fwprintf(fp,L"%0.3lf\n",m_QD);
		fwprintf(fp,L"%0.3lf\n",m_ZD);
		fwprintf(fp,L"%d\n",_wtoi(m_SCALE));
		if(m_XSW=="һλ")fwprintf(fp,L"%d\n",1);
		else if(m_XSW=="��λ")fwprintf(fp,L"%d\n",2);
		else fwprintf(fp,L"%d\n",3);	
		fwprintf(fp,L"%0.1lf\n",m_WHT);		
		fclose(fp);
	}
	CString FName = m_Dbs;
	FName.Replace(L".mdb",L".pmcs");
	WriteCSFile(FName);
}

void CDrawPmDlg::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadName.GetCurSel();
	m_RoadName.GetLBText(i,m_ROAD);
	//	DBS.Read_XLDbs(m_Dbs,L"���Ƶ��",m_ROAD);
	DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",m_ROAD);		
	DBS.Read_XLDbs(m_Dbs,L"������",m_ROAD);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,m_ROAD);
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_QD = _wtof(str);
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_ZD = _wtof(str);;
	UpdateData(false);
}

void CDrawPmDlg::OnRadioxwt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DrawMode = 0;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOXWT);
	pbtn->SetCheck(1); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIOPMZTT);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOZDT);
	pbtn->SetCheck(0);
	GetDlgItem(IDC_CHECK_SMALLBRIDGE)->EnableWindow(true);	
	GetDlgItem(IDC_CHECK_DELENT)->EnableWindow(true);	
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_YDXWID)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIOLCWID)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIOBOTH)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIONONE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIONECOR)->EnableWindow(FALSE);
}

void CDrawPmDlg::OnRadiopmztt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DrawMode = 1;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOXWT);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIOPMZTT);
	pbtn->SetCheck(1);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOZDT);
	pbtn->SetCheck(0);
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YDXWID)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIOLCWID)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIOBOTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIONONE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIONECOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SMALLBRIDGE)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_DELENT)->EnableWindow(true);
}

void CDrawPmDlg::OnRadiozdt() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	DrawMode = 2;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOXWT);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIOPMZTT);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOZDT);
	pbtn->SetCheck(1);
	//	GetDlgItem(IDC_CHECK_SMALLBRIDGE)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_YDXWID)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIOLCWID)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIOBOTH)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIONONE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIONECOR)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHECK_SMALLBRIDGE)->EnableWindow(true);
	GetDlgItem(IDC_CHECK_DELENT)->EnableWindow(true);
}

void CDrawPmDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CDrawPmDlg::OnRadiolcwid() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ZDKMode = 0 ;
	m_ZcLen = 60;
	m_YcLen = 60;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOLCWID);
	pbtn->SetCheck(1); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIONECOR);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOBOTH);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIONONE);
	pbtn->SetCheck(0);
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(true);		
	UpdateData(false);
}

void CDrawPmDlg::OnRadionecor() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ZDKMode = 1 ;
	m_ZcLen = 60;
	m_YcLen = 60;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOLCWID);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIONECOR);
	pbtn->SetCheck(1);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOBOTH);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIONONE);
	pbtn->SetCheck(0);	
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(true);	
	UpdateData(false);	
}

void CDrawPmDlg::OnRadioboth() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ZDKMode = 2;
	m_ZcLen = 80;
	m_YcLen = 80;
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOLCWID);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIONECOR);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOBOTH);
	pbtn->SetCheck(1);
	pbtn = (CButton *)GetDlgItem(IDC_RADIONONE);
	pbtn->SetCheck(0);
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(true);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(true);	
	UpdateData(false);		
}

void CDrawPmDlg::OnRadionone() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_ZDKMode = 3;	
	CButton *pbtn;
	pbtn = (CButton *)GetDlgItem(IDC_RADIOLCWID);
	pbtn->SetCheck(0); 
	pbtn = (CButton *)GetDlgItem(IDC_RADIONECOR);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIOBOTH);
	pbtn->SetCheck(0);
	pbtn = (CButton *)GetDlgItem(IDC_RADIONONE);
	pbtn->SetCheck(1);
	GetDlgItem(IDC_EDIT_ZCLen)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_YCLen)->EnableWindow(false);
	UpdateData(false);
}

void CDrawPmDlg::OnCheckBmbridge() 
{
	// TODO: Add your control notification handler code here
	m_DrawDZQ = !m_DrawDZQ;
}

void CDrawPmDlg::OnCheckBm() 
{
	// TODO: Add your control notification handler code here
	m_DrawBM = !m_DrawBM;
}

void CDrawPmDlg::OnCheckSmallbridge() 
{
	// TODO: Add your control notification handler code here
	m_DrawCulvet = !m_DrawCulvet;
}

void CDrawPmDlg::OnCheckTun() 
{
	// TODO: Add your control notification handler code here
	m_DrawTun = !m_DrawTun;
}

void CDrawPmDlg::OnCheckDx() 
{
	// TODO: Add your control notification handler code here
	m_DrawDX = !m_DrawDX;
}

void CDrawPmDlg::OnCheckDelent() 
{
	// TODO: Add your control notification handler code here
	m_DelEntInf = !m_DelEntInf;
}

void CDrawPmDlg::OnCheckShowdbs() 
{
	// TODO: Add your control notification handler code here
	m_BZRAL=!m_BZRAL;
}

void CDrawPmDlg::OnCheckFldzdk() 
{
	// TODO: Add your control notification handler code here

}

//////////////////////////////////////////////////////////////////////////
//дƽ���ͼ�����ļ�
bool CDrawPmDlg::WriteCSFile(CString FName)
{
	FILE* fp = NULL;
	fp=_wfopen(FName,L"w");
	if(fp)
	{	
		fwprintf(fp,L"%d\n", DrawMode);//��ͼ��ʽ 0  ��λͼ 1 ƽ������ͼ 2 �õ�ͼ
		fwprintf(fp,L"%d\n", m_DrawAll);//�Ƿ��������·0-�� 1-��
		fwprintf(fp,L"%d\n", m_ZDKMode);//����ռ��ͼ��ʽ
		fwprintf(fp,L"%s\n",m_ROAD);//��·��
		fwprintf(fp,L"%lf\n",m_QD);//������
		fwprintf(fp,L"%lf\n",m_ZD);//�յ����
		fwprintf(fp,L"%s\n",m_SCALE);//����
		fwprintf(fp,L"%s\n",m_XSW);//С��λ��
		fwprintf(fp,L"%lf\n",m_WHT);//�ָ�
		fwprintf(fp,L"%s\n",m_LCBZ);//��̱�ע��ʽ
		fwprintf(fp,L"%lf\n",m_ZcLen);//����õر�ע�߳�
		fwprintf(fp,L"%lf\n",m_YcLen);//�Ҳ��õر�ע�߳�
		fwprintf(fp,L"%s\n", m_ZxWid);//��·���߿��
		fwprintf(fp,L"%s\n", m_YDXWid);//�õ�׮�߿��
		fwprintf(fp,L"%d\n", m_DrawDZQ);//�������
		fwprintf(fp,L"%d\n", m_DrawTun);//�����
		fwprintf(fp,L"%d\n", m_DrawCulvet);//��С�ź�
		fwprintf(fp,L"%d\n", m_DrawBM);//��ˮ׼��
		fwprintf(fp,L"%d\n", m_DrawDX);//�浼��
		fwprintf(fp,L"%d\n", m_DelEntInf);//ɾ������ʵ��
		fwprintf(fp,L"%d\n", m_BZRAL);//��ע�뾶����
		fwprintf(fp,L"%d\n", m_DrawJD);//�潻��
		fwprintf(fp,L"%d\n", m_drawfldzdk);//������ռ�ؿ�
		fclose(fp);
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
//��ȡƽ���ͼ�����ļ�
bool CDrawPmDlg::ReadCSFile(CString FName)
{
	FILE* fp = NULL;
	fp=_wfopen(FName,L"r");
	ACHAR TempChr[20];
	if(fp)
	{	
		fwscanf(fp,L"%d\n", &DrawMode);//��ͼ��ʽ 0  ��λͼ 1 ƽ������ͼ 2 �õ�ͼ
		fwscanf(fp,L"%d\n", &m_DrawAll);//�Ƿ��������·0-�� 1-��
		fwscanf(fp,L"%d\n", &m_ZDKMode);//����ռ��ͼ��ʽ
		fwscanf(fp,L"%s\n", TempChr);//��·��
		m_ROAD = TempChr;
		fwscanf(fp,L"%lf\n", &m_QD);//������
		fwscanf(fp,L"%lf\n", &m_ZD);//�յ����
		fwscanf(fp,L"%s\n", TempChr);//����
		m_SCALE = TempChr;
		fwscanf(fp,L"%s\n", TempChr);//С��λ��
		m_XSW = TempChr;
		fwscanf(fp,L"%lf\n", &m_WHT);//�ָ�
		fwscanf(fp,L"%s\n", TempChr);//��̱�ע��ʽ
		m_LCBZ = TempChr;
		fwscanf(fp,L"%lf\n", &m_ZcLen);//����õر�ע�߳�
		fwscanf(fp,L"%lf\n", &m_YcLen);//�Ҳ��õر�ע�߳�
		fwscanf(fp,L"%s\n", TempChr);//��·���߿��
		m_ZxWid = TempChr;
		fwscanf(fp,L"%s\n", TempChr);//�õ�׮�߿��
		m_YDXWid = TempChr;
		fwscanf(fp,L"%d\n", &m_DrawDZQ);
		fwscanf(fp,L"%d\n", &m_DrawTun);
		fwscanf(fp,L"%d\n", &m_DrawCulvet);
		fwscanf(fp,L"%d\n", &m_DrawBM);
		fwscanf(fp,L"%d\n", &m_DrawDX);
		fwscanf(fp,L"%d\n", &m_DelEntInf);
		fwscanf(fp,L"%d\n", &m_BZRAL);
		fwscanf(fp,L"%d\n", &m_DrawJD);
		fwscanf(fp,L"%d\n", &m_drawfldzdk);
		fclose(fp);
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
//��ȡƽ���ͼ�����ļ�
bool CDrawPmDlg::ReadCSFileForDraw()
{
	CString FName = m_Dbs;
	FName.Replace(L".mdb",L".pmcs");
	CString MbFName = Cworkdir + L"\\DATA\\ģ��.pmcs";
	if (!ReadCSFile(FName))
	{
		if (!ReadCSFile(MbFName))
		{
			AfxMessageBox(MbFName+L"������!");
			return false;
		}
	}

	if (m_ROAD==L"���е�·")
	{
		m_ROAD = L"����";
	}
	DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",m_ROAD);
	DBS.Read_XLDbs(m_Dbs,L"������",m_ROAD);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	else
	{
		AfxMessageBox(m_Dbs + L"����ƽ��ʧ��!");
		return false;
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,m_ROAD);
	DBS.ReadAllRoadName(m_Dbs);
	for(int i=0;i<DBS.m_RoadSum;i++)
		m_RoadName.AddString(DBS.Road[i].RoadName);
	ACHAR str[180];
	if (m_QD<0.0 && m_ZD<0.0)
	{
		ads_rtos(pm->DLArray[0].ELC,2,3,str);
		m_QD = _wtof(str);
		ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
		m_ZD = _wtof(str);
	}

	return true;

}