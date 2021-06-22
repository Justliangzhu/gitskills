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
	m_ROAD = L"主线";
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

	if (m_Dbs.Find(L"模板.mdb")>-1)//设置模板
	{
		CString FName = m_Dbs;
		FName.Replace(L".mdb",L".pmcs");

		if (!ReadCSFile(FName))
		{
			AfxMessageBox(FName + L"不存在!");
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
		DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");
		DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
		if(DBS.XYNum>0)
		{
			pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
			if(DBS.NDL>0)
				pm->setDLB(DBS.DLArray,DBS.NDL);
		}
		else
		{
			AfxMessageBox(L"你还没有设置线路方案!");
			CDialog::OnCancel();
			return 0;
		}

		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"主线");
		DBS.ReadAllRoadName(m_Dbs);
		for(int i=0;i<DBS.m_RoadSum;i++)
			m_RoadName.AddString(DBS.Road[i].RoadName);

		CString FName = m_Dbs;
		FName.Replace(L".mdb",L".pmcs");

		if (!ReadCSFile(FName))
		{
			CString tempCStr = SysPath;
			FName = tempCStr + L"\\lib\\模板.pmcs";
			if (!ReadCSFile(FName))
			{
				AfxMessageBox(FName + L"不存在!");
				return FALSE;
			}
			//20190707	修改一些默认值
			m_ZxWid = L"1.4";
			m_LCBZ = L"不标注";
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
	//	m_XSW = _T("两位");
	//	m_LCBZ = _T("百米标");
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
	//	m_ZDKMode = 0;//里程+宽度
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
	//获到缺省图框名
	//////////////////////////////////////////////////////////////////////////


	if ( (ads_getfiled( L"请输入数据库名 *.mdb",DefaultPath,L"mdb",2+4,&result))!=RTNORM) return ;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	_tcscpy(fname,result.resval.rstring); 
	m_Dbs=fname;
	if(m_Dbs==L""){AfxMessageBox(L"未指定数据库!");return;}

	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",L"主线");
	DBS.Read_XLDbs(m_Dbs,L"线元表",L"主线");		
	DBS.Read_XLDbs(m_Dbs,L"断链表",L"主线");
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	_tcscpy(pm->mdbname,m_Dbs);
	_tcscpy(pm->RoadName,L"主线");
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
	if (m_Dbs.Find(L"模板")>-1)
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
		if(m_XSW=="一位")fwprintf(fp,L"%d\n",1);
		else if(m_XSW=="两位")fwprintf(fp,L"%d\n",2);
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
	//	DBS.Read_XLDbs(m_Dbs,L"控制点表",m_ROAD);
	DBS.Read_XLDbs(m_Dbs,L"线元表",m_ROAD);		
	DBS.Read_XLDbs(m_Dbs,L"断链表",m_ROAD);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
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
//写平面出图参数文件
bool CDrawPmDlg::WriteCSFile(CString FName)
{
	FILE* fp = NULL;
	fp=_wfopen(FName,L"w");
	if(fp)
	{	
		fwprintf(fp,L"%d\n", DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwprintf(fp,L"%d\n", m_DrawAll);//是否绘所有线路0-是 1-非
		fwprintf(fp,L"%d\n", m_ZDKMode);//绘制占地图方式
		fwprintf(fp,L"%s\n",m_ROAD);//道路名
		fwprintf(fp,L"%lf\n",m_QD);//起点里程
		fwprintf(fp,L"%lf\n",m_ZD);//终点里程
		fwprintf(fp,L"%s\n",m_SCALE);//比例
		fwprintf(fp,L"%s\n",m_XSW);//小数位数
		fwprintf(fp,L"%lf\n",m_WHT);//字高
		fwprintf(fp,L"%s\n",m_LCBZ);//里程标注样式
		fwprintf(fp,L"%lf\n",m_ZcLen);//左侧用地标注线长
		fwprintf(fp,L"%lf\n",m_YcLen);//右侧用地标注线长
		fwprintf(fp,L"%s\n", m_ZxWid);//线路中线宽度
		fwprintf(fp,L"%s\n", m_YDXWid);//用地桩线宽度
		fwprintf(fp,L"%d\n", m_DrawDZQ);//绘大中桥
		fwprintf(fp,L"%d\n", m_DrawTun);//绘隧道
		fwprintf(fp,L"%d\n", m_DrawCulvet);//绘小桥涵
		fwprintf(fp,L"%d\n", m_DrawBM);//绘水准点
		fwprintf(fp,L"%d\n", m_DrawDX);//绘导线
		fwprintf(fp,L"%d\n", m_DelEntInf);//删除界内实体
		fwprintf(fp,L"%d\n", m_BZRAL);//标注半径缓长
		fwprintf(fp,L"%d\n", m_DrawJD);//绘交点
		fwprintf(fp,L"%d\n", m_drawfldzdk);//绘分离段占地宽
		fclose(fp);
		return true;
	}
	else
		return false;
}

//////////////////////////////////////////////////////////////////////////
//读取平面出图参数文件
bool CDrawPmDlg::ReadCSFile(CString FName)
{
	FILE* fp = NULL;
	fp=_wfopen(FName,L"r");
	ACHAR TempChr[20];
	if(fp)
	{	
		fwscanf(fp,L"%d\n", &DrawMode);//绘图方式 0  线位图 1 平面总体图 2 用地图
		fwscanf(fp,L"%d\n", &m_DrawAll);//是否绘所有线路0-是 1-非
		fwscanf(fp,L"%d\n", &m_ZDKMode);//绘制占地图方式
		fwscanf(fp,L"%s\n", TempChr);//道路名
		m_ROAD = TempChr;
		fwscanf(fp,L"%lf\n", &m_QD);//起点里程
		fwscanf(fp,L"%lf\n", &m_ZD);//终点里程
		fwscanf(fp,L"%s\n", TempChr);//比例
		m_SCALE = TempChr;
		fwscanf(fp,L"%s\n", TempChr);//小数位数
		m_XSW = TempChr;
		fwscanf(fp,L"%lf\n", &m_WHT);//字高
		fwscanf(fp,L"%s\n", TempChr);//里程标注样式
		m_LCBZ = TempChr;
		fwscanf(fp,L"%lf\n", &m_ZcLen);//左侧用地标注线长
		fwscanf(fp,L"%lf\n", &m_YcLen);//右侧用地标注线长
		fwscanf(fp,L"%s\n", TempChr);//线路中线宽度
		m_ZxWid = TempChr;
		fwscanf(fp,L"%s\n", TempChr);//用地桩线宽度
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
//读取平面出图参数文件
bool CDrawPmDlg::ReadCSFileForDraw()
{
	CString FName = m_Dbs;
	FName.Replace(L".mdb",L".pmcs");
	CString MbFName = Cworkdir + L"\\DATA\\模板.pmcs";
	if (!ReadCSFile(FName))
	{
		if (!ReadCSFile(MbFName))
		{
			AfxMessageBox(MbFName+L"不存在!");
			return false;
		}
	}

	if (m_ROAD==L"所有道路")
	{
		m_ROAD = L"主线";
	}
	DBS.Read_XLDbs(m_Dbs,L"线元表",m_ROAD);
	DBS.Read_XLDbs(m_Dbs,L"断链表",m_ROAD);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//主线对象
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
	}
	else
	{
		AfxMessageBox(m_Dbs + L"生成平面失败!");
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