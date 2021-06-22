// SuphFDSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SuphFDSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SuphFDSheet

IMPLEMENT_DYNAMIC(SuphFDSheet, CPropertySheet)

	SuphFDSheet::SuphFDSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{


}

SuphFDSheet::SuphFDSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//	SetZYLmkInitData();


	ZCG.m_psp.pszTitle=L"×ó·ù³¬¸ß";
	ZCG.CgJkInf=0;

	YCG.m_psp.pszTitle=L"ÓÒ·ù³¬¸ß";
	YCG.CgJkInf=0;

	ZJK.m_psp.pszTitle=L"×ó·ù¼Ó¿í";
	ZJK.CgJkInf=1;

	YJK.m_psp.pszTitle=L"ÓÒ·ù¼Ó¿í";
	YJK.CgJkInf=1;

	AddPage(&ZCG);		
	AddPage(&YCG);
	AddPage(&ZJK);		
	AddPage(&YJK);
}

SuphFDSheet::~SuphFDSheet()
{
}


BEGIN_MESSAGE_MAP(SuphFDSheet, CPropertySheet)
	//{{AFX_MSG_MAP(SuphFDSheet)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SuphFDSheet message handlers
