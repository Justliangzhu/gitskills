// ZDKSHEET.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "ZDKSHEET.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZDKSHEET
extern AcDbCircle* pCircle1;
extern AcDbCircle* pCircle2;

IMPLEMENT_DYNAMIC(CZDKSHEET, CPropertySheet)

	CZDKSHEET::CZDKSHEET(GTZDK_ROAD *gtzx2,AcDbObjectId ZID,UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CZDKSHEET::CZDKSHEET(GTZDK_ROAD *pzdk,AcDbObjectId ZID,LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	Left=new CZDKINPUT(pzdk,ZID);
	//   Left->m_psp.pszTitle="��ռ�ؿ�";
	AddPage(Left);

	/*
	Right=new CZDKINPUT(pzdk,ZID,1);	
	Right->m_psp.pszTitle="��ռ�ؿ�";
	AddPage(Right);*/

}

CZDKSHEET::~CZDKSHEET()
{
	//CZDKINPUT ob;
	//ob.ShanChuDingWeiYuan();
	delete Left;	Left=NULL;
	//	delete Right;	Right=NULL;
}


BEGIN_MESSAGE_MAP(CZDKSHEET, CPropertySheet)
	//{{AFX_MSG_MAP(CZDKSHEET)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CZDKSHEET message handlers