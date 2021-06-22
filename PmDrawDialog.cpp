// PmDrawDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "PmDrawDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPmDrawDialog dialog
#include "XLDataBase.h"
#include "CWorkDir.h"
CPmDrawDialog::CPmDrawDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPmDrawDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPmDrawDialog)
	m_Nlc = _T("两位");
	m_QxysLR = _T("曲线内侧");
	m_Scale = 2000;
	m_Texth = 8;
	m_JDGH = _T("JD");
	m_JD0 = 1;
	m_XLName = _T("0-主线");
	//}}AFX_DATA_INIT
}

void CPmDrawDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPmDrawDialog)
	DDX_Control(pDX, IDC_COMBO_XLSEL, m_XLCtrl);
	DDX_Text(pDX, IDC_EDIT_STARTLC, m_StartLc);
	DDX_CBString(pDX, IDC_COMBO_NLC, m_Nlc);
	DDX_CBString(pDX, IDC_COMBO_QXYSLR, m_QxysLR);
	DDX_Text(pDX, IDC_EDIT_ENDLC, m_EndLc);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_Scale);
	DDX_Text(pDX, IDC_EDIT_TEXTH, m_Texth);
	DDX_Text(pDX, IDC_EDIT_JDGH1, m_JDGH);
	DDX_Text(pDX, IDC_EDIT_JD0, m_JD0);
	DDX_CBString(pDX, IDC_COMBO_XLSEL, m_XLName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPmDrawDialog, CDialog)
	//{{AFX_MSG_MAP(CPmDrawDialog)
	ON_CBN_CLOSEUP(IDC_COMBO_XLSEL, OnCloseupComboXlsel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPmDrawDialog message handlers

void CPmDrawDialog::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CWorkDir WordDir;
	FILE *fp=NULL;
	fp=_wfopen(WordDir.PmDrawSetFile,L"w");
	if(fp)
	{	
		fwprintf(fp,L"%s\n",m_XLName);
		fwprintf(fp,L"%s\n",m_StartLc);
		fwprintf(fp,L"%s\n",m_EndLc);
		fwprintf(fp,L"%s\n",m_JDGH);
		fwprintf(fp,L"%d\n",m_JD0);
		if(m_Nlc=="一位")fwprintf(fp,L"%d\n",1);
		else if(m_Nlc=="两位")fwprintf(fp,L"%d\n",2);
		else fwprintf(fp,L"%d\n",3);
		if(m_QxysLR=="曲线内侧")fwprintf(fp,L"%d\n",0);
		else if(m_QxysLR=="曲线左侧")fwprintf(fp,L"%d\n",-1);
		else fwprintf(fp,L"%d\n",1);
		fwprintf(fp,L"%d\n",m_Scale);
		fwprintf(fp,L"%d\n",m_Texth);

		fclose(fp);
	}
	CDialog::OnOK();
}

BOOL CPmDrawDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CStringArray XLNameArray;
	XLNameArray.Add(m_XLName);

	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{		
		Path=WorkDir.DbsPathName;
		DBS.Read_XLDbs(Path,L"断链表");
		m_StartLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[0].EGH,DBS.DLArray[0].ELC,3);
		m_EndLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[DBS.NDL-1].BGH,DBS.DLArray[DBS.NDL-1].BLC,3);
		DBS.Read_XLDbs(Path,L"分离段断链表");
		DBS.GetAllXLName(&XLNameArray);
	}
	int NXL=XLNameArray.GetSize();
	for(int iFLD=0;iFLD<NXL;iFLD++)
	{
		CString str=XLNameArray.GetAt(iFLD);
		m_XLCtrl.AddString(str);
	}
	XLNameArray.RemoveAll();

	CWorkDir WordDir;
	FILE *fp=NULL;
	fp=_wfopen(WordDir.PmDrawSetFile,L"r");
	if(fp)
	{	
		char ch[256];
		int m_NLC,m_QxysLtRt;
		fwscanf(fp,L"%s",ch);m_XLName=ch;
		fwscanf(fp,L"%s",ch);m_StartLc=ch;
		fwscanf(fp,L"%s",ch);m_EndLc=ch;
		fwscanf(fp,L"%s",ch);m_JDGH=ch;
		fwscanf(fp,L"%d",&m_JD0);
		fwscanf(fp,L"%d",&m_NLC);
		fwscanf(fp,L"%d",&m_QxysLtRt);
		fwscanf(fp,L"%d",&m_Scale);
		fwscanf(fp,L"%d",&m_Texth);		
		fclose(fp);

		if(m_NLC==1)m_Nlc=="一位";
		else if(m_NLC==2)m_Nlc=="两位";
		else m_Nlc=="三位";
		if(m_QxysLtRt==0)m_QxysLR=="曲线内侧";
		else if(m_QxysLtRt<0)m_QxysLR=="曲线左侧";
		else m_QxysLR=="曲线右侧";
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPmDrawDialog::OnCloseupComboXlsel() 
{
	// TODO: Add your control notification handler code here
	int CurSel=m_XLCtrl.GetCurSel();
	m_XLCtrl.GetLBText(CurSel,m_XLName);

	int iFLD;
	int n=m_XLName.FindOneOf(L"-");
	iFLD=(int)(_wtof(m_XLName.Left(n))+0.1);
	XLDataBase DBS;
	CWorkDir WorkDir;
	CString Path;
	if(WorkDir.ReadOk)
	{	
		if(iFLD==0)
		{
			Path=WorkDir.DbsPathName;
			DBS.Read_XLDbs(Path,L"断链表");
			m_StartLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[0].EGH,DBS.DLArray[0].ELC,3);
			m_EndLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[DBS.NDL-1].BGH,DBS.DLArray[DBS.NDL-1].BLC,3);
		}
		else
		{
			Path=WorkDir.DbsPathName;
			DBS.Read_XLDbs(Path,L"分离段断链表");

			int sDLNo,eDLNo;
			if(DBS.GetiFLDDLInfo(iFLD,sDLNo,eDLNo))
			{
				m_StartLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[sDLNo].EGH,DBS.DLArray[sDLNo].ELC,3);
				m_EndLc=BAS_DRAW_FUN::LCchr(DBS.DLArray[eDLNo].BGH,DBS.DLArray[eDLNo].BLC,3);
			}
		}
	}
	UpdateData(FALSE);
}
