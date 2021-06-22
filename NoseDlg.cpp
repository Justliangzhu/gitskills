// NoseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NoseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;

/////////////////////////////////////////////////////////////////////////////
// NoseDlg dialog


NoseDlg::NoseDlg(CWnd* pParent /*=NULL*/)
	: cdxCSizingDialog(NoseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NoseDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void NoseDlg::DoDataExchange(CDataExchange* pDX)
{
	cdxCSizingDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NoseDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NoseDlg, cdxCSizingDialog)
	//{{AFX_MSG_MAP(NoseDlg)
	ON_BN_CLICKED(IDC_BUTTON_CAL, OnButtonCal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NoseDlg message handlers

void NoseDlg::OnButtonCal() 
{
	// TODO: Add your control notification handler code here

}

BOOL NoseDlg::OnInitDialog() 
{
	cdxCSizingDialog::OnInitDialog(0);

	// TODO: Add extra initialization here
	CRect rect;
	GetClientRect(rect);
	rect.left+=10;
	rect.top+=50;
	rect.right-=10;
	rect.bottom-=10;
	m_grid.Create(rect, this, 900);

	AddSzControl(m_grid,mdResize,mdResize);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE

}
