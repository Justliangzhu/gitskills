// CheckToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CheckToolBar.h"
#include "StdArx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCheckToolBar * ToolBardlg;
extern bool findtoolbar(CString tbar);
/////////////////////////////////////////////////////////////////////////////
// CCheckToolBar dialog


CCheckToolBar::CCheckToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckToolBar::IDD, pParent)
{

}


void CCheckToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//	DDX_Control(pDX,IDC_LIST1,m_list);

	//{{AFX_DATA_MAP(CCheckToolBar)
	DDX_Control(pDX, IDC_LIST1, m_checklist);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckToolBar, CDialog)
	//{{AFX_MSG_MAP(CCheckToolBar)
	ON_BN_CLICKED(IDC_OK, OnOk)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_LBN_DBLCLK(IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckToolBar message handlers

void CCheckToolBar::OnOk() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
	if (ToolBardlg) 
	{
		ToolBardlg=NULL;
	}
	EnableWindow(false);
	acedGetAcadDockCmdLine()->SetFocus(); 
	DestroyWindow();
}

void CCheckToolBar::OnCancel() 
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
	if (ToolBardlg) 
	{
		ToolBardlg=NULL;
	}
	EnableWindow(false);
	acedGetAcadDockCmdLine()->SetFocus();
	DestroyWindow();

}

BOOL CCheckToolBar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_checklist.SubclassDlgItem(IDD_CheckToolBar,this);
	m_checklist.AddString(_T("��Ŀ����"));
	m_checklist.AddString(_T("������ģ"));

	m_checklist.AddString(_T("�������"));
	m_checklist.AddString(_T("��������"));
	m_checklist.AddString(_T("�������"));
	m_checklist.AddString(_T("ƽ��һ�廯"));
	m_checklist.AddString(_T("��������"));


	m_checklist.AddString(_T("�¶����"));
	m_checklist.AddString(_T("��������"));
	m_checklist.AddString(_T("�������"));
	m_checklist.AddString(_T("�ݶ���һ�廯"));
	m_checklist.AddString(_T("��������"));
	m_checklist.AddString(_T("�õ����"));


	m_checklist.AddString(_T("ƽ��ͼ"));
	m_checklist.AddString(_T("�ݶ���ͼ"));
	//m_checklist.AddString(_T("������"));

	m_checklist.SetCheckStyle(BS_AUTOCHECKBOX);//check��ĸ�ʽ

	if (findtoolbar(L"��Ŀ����"))  //�������Ѿ��ڴ��ڿɼ�����ѡ��ѡ��
		m_checklist.SetCheck(0,1);
	else 
		m_checklist.SetCheck(0,0);

	if (findtoolbar(L"������ģ"))  //�������Ѿ��ڴ��ڿɼ�����ѡ��ѡ��
		m_checklist.SetCheck(1,1);
	else 
		m_checklist.SetCheck(1,0);

	if (findtoolbar(L"�������")) 
		m_checklist.SetCheck(2,1);
	else 
		m_checklist.SetCheck(2,0);
	if (findtoolbar(L"��������")) 
		m_checklist.SetCheck(3,1);
	else 
		m_checklist.SetCheck(3,0);

	if (findtoolbar(L"�������"))
		m_checklist.SetCheck(4,1);
	else 
		m_checklist.SetCheck(4,0);


	if (findtoolbar(L"ƽ��һ�廯")) 
		m_checklist.SetCheck(5,1);
	else 
		m_checklist.SetCheck(5,0);

	if (findtoolbar(L"��������")) 
		m_checklist.SetCheck(6,1);
	else 
		m_checklist.SetCheck(6,0);

	if (findtoolbar(L"�¶����"))
		m_checklist.SetCheck(7,1);
	else 
		m_checklist.SetCheck(7,0);


	if (findtoolbar(L"��������")) 
		m_checklist.SetCheck(8,1);
	else 
		m_checklist.SetCheck(8,0);

	if (findtoolbar(L"�������")) 
		m_checklist.SetCheck(9,1);
	else 
		m_checklist.SetCheck(9,0);

	if (findtoolbar(L"�ݶ���һ�廯")) 
		m_checklist.SetCheck(10,1);
	else 
		m_checklist.SetCheck(10,0);

	if (findtoolbar(L"��������")) 
		m_checklist.SetCheck(11,1);
	else 
		m_checklist.SetCheck(11,0);

	if (findtoolbar(L"�õ����")) 
		m_checklist.SetCheck(12,1);
	else 
		m_checklist.SetCheck(12,0);

	if (findtoolbar(L"ƽ��ͼ")) 
		m_checklist.SetCheck(13,1);
	else 
		m_checklist.SetCheck(13,0);

	if (findtoolbar(L"�ݶ���ͼ")) 
		m_checklist.SetCheck(14,1);
	else 
		m_checklist.SetCheck(14,0);





	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCheckToolBar::OnSelchangeList1() 
{
	// TODO: Add your control notification handler code here

	CString rString;
	int iIndex=m_checklist.GetCurSel();
	int iState=m_checklist.GetCheck(iIndex);
	m_checklist.GetText(iIndex,rString);

	if(iState==0&&rString==L"��Ŀ����")
	{
		darcydarcyxmglout(); //������������
	}
	if(iState==1&&rString==L"��Ŀ����")
	{
		darcydarcyxmgl();  //����������

	}

	if(iState==0&&rString==L"������ģ")
	{
		darcydarcyjlsmout(); //������������
	}
	if(iState==1&&rString==L"������ģ")
	{
		darcydarcyjlsm();  //����������

	}
	if (iState==0&&rString==L"�������") 
	{
		DarcySLDESIGNDXSJOUT();
	}
	if(iState==1&&rString==L"�������")
	{
		darcysldesignsldesign();  //����������
	}
	if (iState==0&&rString==L"��������") 
	{
		DarcyFAGLOUT();
	}
	if(iState==1&&rString==L"��������")
	{
		darcysldesignfagl();  //����������
	}

	if (iState==0&&rString==L"�������") 
	{
		DarcyDLDESIGNDLSJOUT();
	}
	if(iState==1&&rString==L"�������")
	{
		darcydldesigndldesign(); //����������
	}
	if (iState==0&&rString==L"��������") 
	{
		DarcyYTHDLSZOUT(); 
	}
	if(iState==1&&rString==L"��������")
	{
		darcydldes(); //����������
	}
	if (iState==0&&rString==L"ƽ��һ�廯") 
	{
		DarcyDLDESIGNPMYTHOUT(); 

	}
	if(iState==1&&rString==L"ƽ��һ�廯")
	{
		darcyythyth(); //����������
	}
	if (iState==0&&rString==L"�¶����") 
	{
		DarcyPDDESIGNPDSJOUT();
	}
	if(iState==1&&rString==L"�¶����")
	{
		darcypddesignpddesign(); //����������
	}
	if (iState==0&&rString==L"��������") 
	{
		DarcyBRIDGEDESIGNQLSJOUT();
	}
	if(iState==1&&rString==L"��������")
	{

		darcybridgedesignbridgedes(); //����������
	}
	if (iState==0&&rString==L"�������") 
	{
		DarcySDDESIGNSDDESIGNOUT();
	}
	if(iState==1&&rString=="�������")
	{
		darcysddesignsddesign(); //����������
	}
	if (iState==0&&rString=="�ݶ���һ�廯") 
	{
		DarcyDLDESIGNZMYTHSJOUT();
	}
	if(iState==1&&rString=="�ݶ���һ�廯")
	{
		darcyzdmdeszdmdes(); //����������
	}
	if (iState==0&&rString=="��������") 
	{
		DarcyHDMDESOUT();
	}
	if(iState==1&&rString=="��������")
	{
		darcyhdmdes(); //����������
	}
	if (iState==0&&rString=="�õ����") 
	{
		DarcyYDOUT();
	}
	if(iState==1&&rString=="�õ����")
	{
		darcyyddes(); //����������
	}

	if (iState==0&&rString=="ƽ��ͼ") 
	{
		DarcyPLANPMTOUT();
	}
	if(iState==1&&rString=="ƽ��ͼ")
	{
		darcyplanplan(); //����������
	}
	if (iState==0&&rString=="�ݶ���ͼ") 
	{
		DarcyZDMDESZDMTOUT();
	}
	if(iState==1&&rString=="�ݶ���ͼ")
	{
		darcyverticalsectionver_section(); //����������
	}
	/*
	if (iState==0&&rString=="������") 
	{

	}
	if(iState==1&&rString=="������")
	{

	}*/




}


void CCheckToolBar::OnDblclkList1() 
{
	// TODO: Add your control notification handler code here
	CString rString;
	int iIndex=m_checklist.GetCurSel();
	int iState=m_checklist.GetCheck(iIndex);
	m_checklist.GetText(iIndex,rString);

	if(iState==0&&rString=="��Ŀ����")
	{
		darcydarcyxmglout(); //������������
	}
	if(iState==1&&rString=="��Ŀ����")
	{
		darcydarcyxmgl();  //����������

	}
	if(iState==0&&rString=="������ģ")
	{
		darcydarcyjlsmout(); //������������
	}
	if(iState==1&&rString=="������ģ")
	{
		darcydarcyjlsm();  //����������
	}
	if (iState==0&&rString=="�������") 
	{
		DarcySLDESIGNDXSJOUT();
	}
	if(iState==1&&rString=="�������")
	{
		darcysldesignsldesign();  //����������
	}
	if (iState==0&&rString=="��������") 
	{
		DarcyFAGLOUT();
	}
	if(iState==1&&rString=="��������")
	{
		darcysldesignfagl();  //����������
	}
	if (iState==0&&rString=="�������") 
	{
		DarcyDLDESIGNDLSJOUT();
	}
	if(iState==1&&rString=="�������")
	{
		darcydldesigndldesign(); //����������
	}
	if (iState==0&&rString=="��������") 
	{
		DarcyYTHDLSZOUT(); 
	}
	if(iState==1&&rString=="��������")
	{
		darcydldes(); //����������
	}
	if (iState==0&&rString=="ƽ��һ�廯") 
	{
		DarcyDLDESIGNPMYTHOUT(); 

	}
	if(iState==1&&rString=="ƽ��һ�廯")
	{
		darcyythyth(); //����������
	}
	if (iState==0&&rString=="�¶����") 
	{
		DarcyPDDESIGNPDSJOUT();
	}
	if(iState==1&&rString=="�¶����")
	{
		darcypddesignpddesign(); //����������
	}
	if (iState==0&&rString=="��������") 
	{
		DarcyBRIDGEDESIGNQLSJOUT();
	}
	if(iState==1&&rString=="��������")
	{
		darcybridgedesignbridgedes(); //����������
	}
	if (iState==0&&rString=="�������") 
	{
		DarcySDDESIGNSDDESIGNOUT();
	}
	if(iState==1&&rString=="�������")
	{
		darcysddesignsddesign(); //����������
	}
	if (iState==0&&rString=="�ݶ���һ�廯") 
	{
		DarcyDLDESIGNZMYTHSJOUT();
	}
	if(iState==1&&rString=="�ݶ���һ�廯")
	{
		darcyzdmdeszdmdes(); //����������
	}	if (iState==0&&rString=="��������") 
	{
		DarcyHDMDESOUT();
	}
	if(iState==1&&rString=="��������")
	{
		darcyhdmdes(); //����������
	}
	if (iState==0&&rString=="�õ����") 
	{
		DarcyYDOUT();
	}
	if(iState==1&&rString=="�õ����")
	{
		darcyyddes(); //����������
	}


	if (iState==0&&rString=="ƽ��ͼ") 
	{
		DarcyPLANPMTOUT();
	}
	if(iState==1&&rString=="ƽ��ͼ")
	{
		darcyplanplan(); //����������
	}
	if (iState==0&&rString=="�ݶ���ͼ") 
	{
		DarcyZDMDESZDMTOUT();
	}
	if(iState==1&&rString=="�ݶ���ͼ")
	{
		darcyverticalsectionver_section(); //����������
	}
	/*
	if (iState==0&&rString=="������") 
	{

	}*/

	/*
	if(iState==1&&rString=="������")
	{

	}*/




}
