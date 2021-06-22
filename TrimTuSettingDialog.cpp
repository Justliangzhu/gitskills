// TrimTuSettingDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "TrimTuSettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrimTuSettingDialog dialog

#include "CWorkDir.h"
CTrimTuSettingDialog::CTrimTuSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CTrimTuSettingDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTrimTuSettingDialog)
	m_Length = 350.0f;
	m_Frame_Height = 554.0;
	m_Frame_Width = 780.0;
	m_TrimFlag = 0;
	m_TuKuangFile = _T("");
	m_OutName = _T("ƽ��ͼ");
	m_StartNo = 1;
	m_SumT = _T("");
	m_RoadName = _T("");
	m_Edml = _T("");
	m_Dbs = _T("");
	m_Sdml = _T("");
	m_IsNowShow = true;
	//}}AFX_DATA_INIT
	CWorkDir WorkDir;
	m_TuKuangFile=WorkDir.TKDwgFile;
}


void CTrimTuSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrimTuSettingDialog)
	DDX_Control(pDX, IDC_COMBO_RoadName, m_RoadNameCrl);
	DDX_Text(pDX, IDC_EDIT_LEN, m_Length);
	DDX_Text(pDX, IDC_EDIT_Frame_Height, m_Frame_Height);
	DDX_Text(pDX, IDC_EDIT_Frame_Width, m_Frame_Width);
	DDX_Radio(pDX, IDC_RADIO_BREAK, m_TrimFlag);
	DDX_Text(pDX, IDC_EDIT_TKFILE, m_TuKuangFile);
	DDX_Text(pDX, IDC_EDIT_OUTNAME, m_OutName);
	DDX_Text(pDX, IDC_EDITSNO, m_StartNo);
	DDX_Text(pDX, IDC_EDITSUMT, m_SumT);
	DDX_CBString(pDX, IDC_COMBO_RoadName, m_RoadName);
	DDX_Text(pDX, IDC_EDIT_EDML, m_Edml);
	DDX_Text(pDX, IDC_EDIT_MDBNAME, m_Dbs);
	DDX_Text(pDX, IDC_EDIT_SDML, m_Sdml);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_BUTTON_SHOW_DETAIL_INFOR, m_ShowInforCtrl);
}


BEGIN_MESSAGE_MAP(CTrimTuSettingDialog, CDialog)
	//{{AFX_MSG_MAP(CTrimTuSettingDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSTK, OnButtonBrowstk)
	ON_BN_CLICKED(IDC_RADIO_TOP, OnRadioTop)
	ON_BN_CLICKED(IDC_RADIO_MIDDLE, OnRadioMiddle)
	ON_BN_CLICKED(IDC_RADIO_BOTTOM, OnRadioBottom)
	ON_BN_CLICKED(IDC_BUTTON_LIULAN, OnButtonLiulan)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, OnSelchangeCOMBORoadName)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SHOW_DETAIL_INFOR, &CTrimTuSettingDialog::OnBnClickedButtonShowDetailInfor)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrimTuSettingDialog message handlers

void CTrimTuSettingDialog::OnButtonBrowstk() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	struct resbuf result;
	int rc=RTNORM;
	CWorkDir WorkDir;

	if ( (ads_getfiled( L"�������ļ��� *.dwg",WorkDir.TKDwgFile,L"dwg",2+4,&result))!=RTNORM) return;
	if ((result.resval.rint==1)&& (result.restype==RTSHORT) ) return ;
	m_TuKuangFile=result.resval.rstring; 
	UpdateData(FALSE);	
}

void CTrimTuSettingDialog::OnRadioTop() 
{
	// TODO: Add your control notification handler code here
	m_TKPos=1;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_MIDDLE);
	pBtn->SetCheck(0);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_TOP);
	pBtn->SetCheck(1);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_BOTTOM);
	pBtn->SetCheck(0);	
}

void CTrimTuSettingDialog::OnRadioMiddle() 
{
	// TODO: Add your control notification handler code here
	m_TKPos=2;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_MIDDLE);
	pBtn->SetCheck(1);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_TOP);
	pBtn->SetCheck(0);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_BOTTOM);
	pBtn->SetCheck(0);	
}

void CTrimTuSettingDialog::OnRadioBottom() 
{
	// TODO: Add your control notification handler code here
	m_TKPos=3;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_MIDDLE);
	pBtn->SetCheck(0);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_TOP);
	pBtn->SetCheck(0);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_BOTTOM);
	pBtn->SetCheck(1);	
}

BOOL CTrimTuSettingDialog::OnInitDialog() 
{
	if (!CDialog::OnInitDialog())
	{
		AfxMessageBox(L"��ʼ���Ի���ʧ�ܣ�");
		return FALSE;
	};
	m_TrimFlag = 0;//���
	// TODO: Add extra initialization here;
	m_TKPos=2;
	CButton *pBtn = (CButton *)GetDlgItem(IDC_RADIO_MIDDLE);
	pBtn->SetCheck(1);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_TOP);
	pBtn->SetCheck(0);	

	pBtn = (CButton *)GetDlgItem(IDC_RADIO_BOTTOM);
	pBtn->SetCheck(0);
	pm = NULL;
	BAS_DRAW_FUN ob;
	ob.ReadWorkdir();
	if(Pro=="")
	{
		CDialog::OnCancel();
		return 0;
	}
	m_Dbs = Cworkdir+"\\DATA\\";
	m_Dbs = m_Dbs + Pro+".mdb";

	DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",L"����");		
	DBS.Read_XLDbs(m_Dbs,L"������",L"����");
	if(DBS.XYNum>0)
	{
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"����");
	}
	else
	{
		CDialog::OnCancel();
		return FALSE;
	}
	DBS.ReadAllRoadName(m_Dbs);
	for(int i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	m_RoadName = "����";
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_Sdml = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_Edml = str;

	Show_Detail_Infor(false);
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTrimTuSettingDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}

void CTrimTuSettingDialog::OnButtonLiulan() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	struct resbuf result;
	int rc=RTNORM;
	ACHAR fname[256];

	//////////////////////////////////////////////////////////////////////////
	ACHAR DefaultPath[256],wordir[256],pro[256];
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
	if(m_Dbs==""){AfxMessageBox(L"δָ�����ݿ�!");return;}

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
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,L"����");
	}
	DBS.m_RoadSum=0;
	DBS.ReadAllRoadName(m_Dbs);
	int i;

	m_RoadNameCrl.ResetContent();
	for(i=0;i<DBS.m_RoadSum;i++)
		m_RoadNameCrl.AddString(DBS.Road[i].RoadName);
	m_RoadNameCrl.SetCurSel(0);
	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_Sdml = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_Edml = str;

	UpdateData(false);	
}

void CTrimTuSettingDialog::OnSelchangeCOMBORoadName() 
{
	// TODO: Add your control notification handler code here
	int i = m_RoadNameCrl.GetCurSel();
	m_RoadNameCrl.GetLBText(i,m_RoadName);
	//	DBS.Read_XLDbs(m_Dbs,L"���Ƶ��",m_RoadName);
	DBS.Read_XLDbs(m_Dbs,L"��Ԫ��",m_RoadName);			
	DBS.Read_XLDbs(m_Dbs,L"������",m_RoadName);
	if(DBS.XYNum>0)
	{
		if(pm)
			delete pm;
		pm=new JD_CENTER(DBS.XYArray,DBS.XYNum);//���߶���
		//		pm->SetJdCenterArray(DBS.JdCenArray,DBS.NJdCen);
		if(DBS.NDL>0)
			pm->setDLB(DBS.DLArray,DBS.NDL);
		_tcscpy(pm->mdbname,m_Dbs);
		_tcscpy(pm->RoadName,m_RoadName);
	}

	ACHAR str[180];
	ads_rtos(pm->DLArray[0].ELC,2,3,str);
	m_Sdml = str;
	ads_rtos(pm->DLArray[pm->DLNum-1].BLC,2,3,str);
	m_Edml = str;
	UpdateData(false);	
}
void CTrimTuSettingDialog::Show_Detail_Infor(bool IsShow)
{
	//��������
	if (IsShow)
		m_ShowInforCtrl.SetWindowText(L"�������<<");
	else
		m_ShowInforCtrl.SetWindowText(L"��ϸ����>>");

	//���öԻ����С
	CRect CurRect;
	GetWindowRect(CurRect);
	double Extend_Width = 300;
	if (IsShow)
		CurRect.right += Extend_Width;
	else
		CurRect.right -= Extend_Width;

	MoveWindow(&CurRect);

	if (m_IsNowShow)
		m_IsNowShow = false;
	else
		m_IsNowShow = true;
}


void CTrimTuSettingDialog::OnBnClickedButtonShowDetailInfor()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Show_Detail_Infor(!m_IsNowShow);
}
