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
//爬坡车道属性页对话框
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

	Zppcd.m_psp.pszTitle=L"左幅";
	Zppcd.ZorY=-1;
	Yppcd.m_psp.pszTitle=L"右幅";
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
	//设置缺省路面宽

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
		//1.读取数据库已有的爬坡车道数据
		bool IfDefaultSet;
		IfDefaultSet = true;
		CString mdbname,roadname;
		mdbname = m_pm->mdbname;
		roadname = m_pm->RoadName;
		if(mdbname!="未命名" && roadname!="未命名道路")//读取数据库
		{
			XLDataBase xlmdb;
			xlmdb.Read_XLDbs(mdbname,L"左爬坡车道分段表",roadname);
			if(xlmdb.NPPCD>0)//有数据
			{
				Zppcd.SetPPCD(xlmdb.NPPCD,xlmdb.ppcd);
				//	   Zppcd.UpdateData(false);
				if(xlmdb.ppcd) delete []xlmdb.ppcd;
				xlmdb.ppcd = NULL;
				IfDefaultSet = false;
			}
			xlmdb.Read_XLDbs(mdbname,L"右爬坡车道分段表",roadname);

			if(xlmdb.NPPCD>0)//有数据
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


