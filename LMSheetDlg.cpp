// LMSheetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LMSheetDlg.h"
#include "xldatabase.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LMSheetDlg

IMPLEMENT_DYNAMIC(LMSheetDlg, CPropertySheet)

	LMSheetDlg::LMSheetDlg(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

LMSheetDlg::LMSheetDlg(JD_CENTER *pm,LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	m_pm = pm;
	SetZYLmkInitData();
	zlmk.m_psp.pszTitle=L"���";
	zlmk.ZorY=-1;
	ylmk.m_psp.pszTitle=L"�ҷ�";
	ylmk.ZorY=1;
	AddPage(&zlmk);		
	AddPage(&ylmk);

}

LMSheetDlg::~LMSheetDlg()
{
}


BEGIN_MESSAGE_MAP(LMSheetDlg, CPropertySheet)
	//{{AFX_MSG_MAP(LMSheetDlg)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LMSheetDlg message handlers

BOOL LMSheetDlg::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO: Add your specialized code here
	//����ȱʡ·���

	//

	//   UpdateData(false);
	return bResult;
}

INT_PTR LMSheetDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	return 	CPropertySheet::DoModal();
}

void LMSheetDlg::SetZYLmkInitData()
{

	if(m_pm)
	{
		//1.��ȡ���ݿ����е�·������
		bool IfDefaultSet;
		IfDefaultSet = true;
		CString mdbname,roadname;
		mdbname = m_pm->mdbname;
		roadname = m_pm->RoadName;
		if(mdbname!="δ����" && roadname!="δ������·")//��ȡ���ݿ�
		{
			XLDataBase xlmdb;
			xlmdb.Read_XLDbs(mdbname,"��·���ֶα�",roadname);
			if(xlmdb.NLMK>0)//������
			{
				xlmdb.lmk[xlmdb.NLMK-1].xlc = m_pm->DLArray[m_pm->DLNum-1].BLC;
				zlmk.SetLmk(xlmdb.NLMK,xlmdb.lmk);
				//	   zlmk.UpdateData(false);
				if(xlmdb.lmk) delete []xlmdb.lmk;
				xlmdb.lmk = NULL;
				IfDefaultSet = false;
			}
			xlmdb.Read_XLDbs(mdbname,"��·���ֶα�",roadname);

			if(xlmdb.NLMK>0)//������
			{
				xlmdb.lmk[xlmdb.NLMK-1].xlc = m_pm->DLArray[m_pm->DLNum-1].BLC;
				ylmk.SetLmk(xlmdb.NLMK,xlmdb.lmk);
				//	   ylmk.UpdateData(false);
				if(xlmdb.lmk) delete []xlmdb.lmk;
				xlmdb.lmk = NULL;
				IfDefaultSet = false;
			}		   		   
		}

		if(IfDefaultSet)//��Ҫȱʡ����·���
		{
			//��������:Ӧ��������ʽ�����ʽ�Զ�����·���(�Ժ���05.4.10)

			int nlmk=1;
			LMKdata *lmk=NULL;
			lmk = new LMKdata[1];

			SetDefaultLmk(nlmk,lmk[0]);

			if(roadname!="����")
			{
				//���÷����·���
				if(roadname.Find(L"��",0) >= 0 )//���߷���
				{
					nlmk = 1;
					lmk[0].fgdk=lmk[0].lydk=lmk[0].cxdk=lmk[0].lmhp=lmk[0].yljk=lmk[0].yljhp=0.0;
					lmk[0].tljk=lmk[0].tljhp=0.0;
					lmk[0].bkmode=lmk[0].sjbgmode=0;
					lmk[0].fgdhp = 0.0;
					lmk[0].OffsetByXLZX = 0.0;
				}
			}
			zlmk.SetLmk(nlmk,lmk);

			if(roadname!="����")
			{
				//���÷����·���
				if(roadname.Find(L"��",0) >= 0 )//���߷���
				{
					nlmk = 1;
					lmk[0].fgdk=lmk[0].lydk=lmk[0].cxdk=lmk[0].lmhp=lmk[0].yljk=lmk[0].yljhp=0.0;
					lmk[0].tljk=lmk[0].tljhp=0.0;
					lmk[0].bkmode=lmk[0].sjbgmode=0;
					lmk[0].fgdhp = 0.0;
					lmk[0].OffsetByXLZX = 0.0;
				}
			}
			ylmk.SetLmk(nlmk,lmk);

			//   ylmk.UpdateData(false);
			delete []lmk;		   
		}

	}
	else
	{
		ads_alert(L"������¼��·��ֶ�����ǰ,δָ����Ч��ƽ�淽��!");
	}
}

void LMSheetDlg::SetDefaultLmk(int &nlmk,LMKdata &lmk)
{
	int DV;
	CString Grade;

	DV = m_pm->DV;
	Grade = m_pm->Grade;
	lmk.xlc = m_pm->DLArray[m_pm->DLNum-1].BLC;
	lmk.fgdhp = 0.0;
	lmk.OffsetByXLZX = 0.0;
	SetLMKByDV(Grade,DV,lmk.fgdk,lmk.lydk,lmk.cxdk,
		lmk.lmhp,lmk.yljk,lmk.yljhp,lmk.tljk,
		lmk.tljhp,lmk.bkmode,lmk.sjbgmode);

}
