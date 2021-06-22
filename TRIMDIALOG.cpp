// TRIMDIALOG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TRIMDIALOG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "BAS_DRAW_FUN.h"
/////////////////////////////////////////////////////////////////////////////
// TRIMDIALOG dialog


TRIMDIALOG::TRIMDIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(TRIMDIALOG::IDD, pParent)
{
	//{{AFX_DATA_INIT(TRIMDIALOG)
	m_DTK = _T("40");
	m_SPACEW = _T("");
	//}}AFX_DATA_INIT
	BAS_DRAW_FUN ob;
	CString SCPRO="";
	if(ob.ReadWorkdir()>0)
	{
		SCPRO=Cworkdir;
		SCPRO+="\\DATA\\SCALE.";
		SCPRO+=Pro;
		FILE *fp;
		if((fp = _wfopen(SCPRO,L"r"))==NULL)
		{
			m_Scale = 2000.0;
		}
		else
		{
			fwscanf(fp,L"%lf\n",&m_Scale);			
			fclose(fp);
		}  
		ACHAR ch[20];
		_stprintf(ch,L"%.0lf\n",m_Scale);
		m_SCALE = ch;
	}
	else m_SCALE=_T("2000");
	//////////////////////////////////////////////////////////////////////////
	m_HEIGHT = _T("297");
	m_DH = _T("10");
	m_SPACEW = _T("50");
}


void TRIMDIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TRIMDIALOG)
	DDX_CBString(pDX, IDC_COMBO_SCALE, m_SCALE);
	DDX_CBString(pDX, IDC_COMBO_Height, m_HEIGHT);
	DDX_CBString(pDX, IDC_COMBO_DH, m_DH);
	DDX_CBString(pDX, IDC_COMBO_DTK, m_DTK);
	DDX_CBString(pDX, IDC_COMBO_SPACEW, m_SPACEW);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TRIMDIALOG, CDialog)
	//{{AFX_MSG_MAP(TRIMDIALOG)
	ON_CBN_EDITCHANGE(IDC_COMBO_SCALE, OnEditchangeComboScale)
	ON_CBN_EDITCHANGE(IDC_COMBO_Height, OnEditchangeCOMBOHeight)
	ON_CBN_EDITCHANGE(IDC_COMBO_DH, OnEditchangeComboDh)
	ON_CBN_EDITCHANGE(IDC_COMBO_SPACEW, OnEditchangeComboSpacew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TRIMDIALOG message handlers

void TRIMDIALOG::OnEditchangeComboScale() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void TRIMDIALOG::OnEditchangeCOMBOHeight() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void TRIMDIALOG::OnEditchangeComboDh() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void TRIMDIALOG::OnOK() 
{
	// TODO: Add extra validation here
	IsOk=true;
	UpdateData(true);
	m_Scale=_wtof(m_SCALE)/1000;
	m_H=_wtof(m_HEIGHT)*m_Scale;
	m_dH=_wtof(m_DH)*m_Scale;
	m_dTK=_wtof(m_DTK)*m_Scale;
	m_dSpace=_wtof(m_SPACEW)*m_Scale;
	CDialog::OnOK();
}

void TRIMDIALOG::OnCancel() 
{
	// TODO: Add extra cleanup here
	IsOk=false;
	CDialog::OnCancel();
}

void TRIMDIALOG::OnEditchangeComboSpacew() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);	
}
