// PmCutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PmCutDlg.h"
#ifdef VERSION_CAD2016
#include "acedCmdNF.h"
#endif // VERSION_CAD2016

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PmCutDlg dialog
extern ads_point ZOOM_P1,ZOOM_P2,ZOOM_P3,ZOOM_P4;

PmCutDlg::PmCutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PmCutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(PmCutDlg)
	m_Xori = _T("");
	m_Yori = _T("");
	pm = NULL;
	//}}AFX_DATA_INIT

}


void PmCutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PmCutDlg)
	DDX_Text(pDX, IDC_EDIT_X, m_Xori);
	DDX_Text(pDX, IDC_EDIT_Y, m_Yori);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PmCutDlg, CDialog)
	//{{AFX_MSG_MAP(PmCutDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_pick, OnBUTTONpick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PmCutDlg message handlers

void PmCutDlg::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	if(pm)
	{
		GetParent()->EnableWindow(TRUE);
		ShowWindow(SW_HIDE);		// Hide our dialog
		GetParent()->SetFocus();	// Give AutoCAD the focus.
		ZOOM_P1[Y]=pm->XYArray[1][8];ZOOM_P1[X]=pm->XYArray[1][9];
		ZOOM_P2[Y]=pm->XYArray[pm->XYNum-1][8];ZOOM_P2[X]=pm->XYArray[pm->XYNum-1][9];
		double xmin,xmax,ymin,ymax;
		xmin = ZOOM_P1[X] < ZOOM_P2[X] ? ZOOM_P1[X]:ZOOM_P2[X];
		xmax = ZOOM_P1[X] > ZOOM_P2[X] ? ZOOM_P1[X]:ZOOM_P2[X];
		ymin = ZOOM_P1[Y] < ZOOM_P2[Y] ? ZOOM_P1[Y]:ZOOM_P2[Y];
		ymax = ZOOM_P1[Y] > ZOOM_P2[Y] ? ZOOM_P1[Y]:ZOOM_P2[Y];

		ZOOM_P1[X] = xmin-500,ZOOM_P1[Y] = ymin-500;
		ZOOM_P2[X] = xmax+500,ZOOM_P2[Y] = ymax+500;
#ifdef VERSION_CAD2016
		acedCommandS(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2016
#ifdef VERSION_CAD2010
		ads_command(RTSTR, L"ZOOM", RTSTR, L"WINDOW", RTPOINT, ZOOM_P1, RTPOINT, ZOOM_P2, 0);
#endif // VERSION_CAD2010	

		ShowWindow(SW_SHOW); // Display our dialog again
		SetFocus(); // Reset the focus back to ourselves
		GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
		EnableWindow(TRUE); // Enable our dialog*/
	}
}

void PmCutDlg::OnBUTTONpick() 
{
	// TODO: Add your control notification handler code here
	AcGePoint3d PT;
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	if(acedGetPoint(NULL,L"请在图上空白处(务必远离地形图)采集一点:_\n",resPt)==RTNORM)
	{
		m_Xori.Format(L"%0.2lf", resPt[X]);
		m_Yori.Format(L"%0.2lf", resPt[Y]);
	}   

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	UpdateData(false);		
}

void PmCutDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(m_Xori==""||m_Yori=="")
	{
		ads_alert(L"请设置绘图原点!");
		return;
	}
	CDialog::OnOK();
}
