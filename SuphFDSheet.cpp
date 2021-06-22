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


	ZCG.m_psp.pszTitle=L"�������";
	ZCG.CgJkInf=0;

	YCG.m_psp.pszTitle=L"�ҷ�����";
	YCG.CgJkInf=0;

	ZJK.m_psp.pszTitle=L"����ӿ�";
	ZJK.CgJkInf=1;

	YJK.m_psp.pszTitle=L"�ҷ��ӿ�";
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
