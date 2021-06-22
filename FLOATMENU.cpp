// FLOATMENU.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "FLOATMENU.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FLOATMENU dialog


FLOATMENU::FLOATMENU(CWnd* pParent /*=NULL*/)
	: CDialog(FLOATMENU::IDD, pParent)
{
	//{{AFX_DATA_INIT(FLOATMENU)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void FLOATMENU::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FLOATMENU)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FLOATMENU, CDialog)
	//{{AFX_MSG_MAP(FLOATMENU)
	ON_COMMAND(ID_MENUITEM_LOCATIONTOOL, OnMenuitemLocationtool)
	ON_COMMAND(ID_MENUITEM_NEWXM, OnMenuitemNewxm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FLOATMENU message handlers

void FLOATMENU::OnMenuitemLocationtool() 
{
	// TODO: Add your command handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->sendStringToExecute(acDocManager->curDocument(),L"LOCATIONTOOL\n",0);		
}

void FLOATMENU::OnMenuitemNewxm() //新建项目 
{
	// TODO: Add your command handler code here
	ACHAR fname[256];
	_stprintf(fname , L"%s\\dirdlg.exe",SysPath);
	WinExec((LPCSTR)fname,1);	
}
