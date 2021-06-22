// JMBXDlg.cpp : implementation file
//

#include "stdafx.h"
#include "JMBXDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// JMBXDlg dialog


JMBXDlg::JMBXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(JMBXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(JMBXDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void JMBXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(JMBXDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(JMBXDlg, CDialog)
	//{{AFX_MSG_MAP(JMBXDlg)
	ON_BN_CLICKED(IDC_BUTTON_XYtoJD, OnBUTTONXYtoJD)
	ON_BN_CLICKED(IDC_BUTTON_line, OnBUTTONline)
	ON_BN_CLICKED(IDC_BUTTON_HHQX, OnButtonHhqx)
	ON_BN_CLICKED(IDC_BUTTON_arc, OnBUTTONarc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// JMBXDlg message handlers

void JMBXDlg::OnBUTTONXYtoJD() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"CXJ\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void JMBXDlg::OnBUTTONline() 
{
	acedGetAcadDwgView()->SetFocus();
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"BL\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void JMBXDlg::OnButtonHhqx() 
{
	acedGetAcadDwgView()->SetFocus();
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"BS\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}

void JMBXDlg::OnBUTTONarc() 
{
	acedGetAcadDwgView()->SetFocus();
	ACHAR cmdstr[80];
	_tcscpy(cmdstr, L"(command \"BA\")\n");
	acDocManager->sendStringToExecute(acDocManager->curDocument(),cmdstr,0);
}
