// PPCDSheet.cpp: implementation of the CPPCDSheet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PPCDSheet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//���³�������ҳ�Ի���
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CPPCDSheet, CPropertySheet)

	CPPCDSheet::CPPCDSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CPPCDSheet::CPPCDSheet(JD_CENTER *pm,LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_pm = pm;
	SetZYPPCDInitData();

	Zppcd.m_psp.pszTitle=L"���";
	Zppcd.ZorY=-1;
	Yppcd.m_psp.pszTitle=L"�ҷ�";
	Yppcd.ZorY=1;
	AddPage(&Zppcd);		
	AddPage(&Yppcd);

}

CPPCDSheet::~CPPCDSheet()
{
}


BEGIN_MESSAGE_MAP(CPPCDSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CPPCDSheet)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPPCDSheet message handlers

BOOL CPPCDSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO: Add your specialized code here
	//����ȱʡ·���

	//

	//   UpdateData(false);
	return bResult;
}

INT_PTR CPPCDSheet::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	return 	CPropertySheet::DoModal();
}

void CPPCDSheet::SetZYPPCDInitData()
{

	if(m_pm)
	{
		//1.��ȡ���ݿ����е����³�������
		bool IfDefaultSet;
		IfDefaultSet = true;
		CString mdbname,roadname;
		mdbname = m_pm->mdbname;
		roadname = m_pm->RoadName;
		if(mdbname!="δ����" && roadname!="δ������·")//��ȡ���ݿ�
		{
			XLDataBase xlmdb;
			xlmdb.Read_XLDbs(mdbname,L"�����³����ֶα�",roadname);
			if(xlmdb.NPPCD>0)//������
			{
				Zppcd.SetPPCD(xlmdb.NPPCD,xlmdb.ppcd);
				//	   Zppcd.UpdateData(false);
				if(xlmdb.ppcd) delete []xlmdb.ppcd;
				xlmdb.ppcd = NULL;
				IfDefaultSet = false;
			}
			xlmdb.Read_XLDbs(mdbname,L"�����³����ֶα�",roadname);

			if(xlmdb.NPPCD>0)//������
			{
				Yppcd.SetPPCD(xlmdb.NPPCD,xlmdb.ppcd);
				//	   Yppcd.UpdateData(false);
				if(xlmdb.ppcd) delete []xlmdb.ppcd;
				xlmdb.ppcd = NULL;
				IfDefaultSet = false;
			}		   		   
		}

	}
}


