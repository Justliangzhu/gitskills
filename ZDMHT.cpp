// ZDMHT.cpp : implementation file
//

#include "stdafx.h"
#include "ZDMHT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ZDMHT

IMPLEMENT_DYNAMIC(ZDMHT, CPropertySheet)

	ZDMHT::ZDMHT(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

ZDMHT::ZDMHT(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&zinidlg);
	AddPage(&zinidlg.wxplm);
	AddPage(&zinidlg.zdmbx);
	SetActivePage(&zinidlg);
}

ZDMHT::~ZDMHT()
{
}

BEGIN_MESSAGE_MAP(ZDMHT, CPropertySheet)
	//{{AFX_MSG_MAP(ZDMHT)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ZDMHT message handlers
