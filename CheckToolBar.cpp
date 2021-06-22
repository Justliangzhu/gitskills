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
	m_checklist.AddString(_T("项目管理"));
	m_checklist.AddString(_T("建立数模"));

	m_checklist.AddString(_T("基线设计"));
	m_checklist.AddString(_T("方案管理"));
	m_checklist.AddString(_T("二线设计"));
	m_checklist.AddString(_T("平面一体化"));
	m_checklist.AddString(_T("断链设置"));


	m_checklist.AddString(_T("坡度设计"));
	m_checklist.AddString(_T("桥梁设置"));
	m_checklist.AddString(_T("隧道设置"));
	m_checklist.AddString(_T("纵断面一体化"));
	m_checklist.AddString(_T("横断面设计"));
	m_checklist.AddString(_T("用地设计"));


	m_checklist.AddString(_T("平面图"));
	m_checklist.AddString(_T("纵断面图"));
	//m_checklist.AddString(_T("表格输出"));

	m_checklist.SetCheckStyle(BS_AUTOCHECKBOX);//check框的格式

	if (findtoolbar(L"项目管理"))  //工具条已经在窗口可见，则复选框选上
		m_checklist.SetCheck(0,1);
	else 
		m_checklist.SetCheck(0,0);

	if (findtoolbar(L"建立数模"))  //工具条已经在窗口可见，则复选框选上
		m_checklist.SetCheck(1,1);
	else 
		m_checklist.SetCheck(1,0);

	if (findtoolbar(L"基线设计")) 
		m_checklist.SetCheck(2,1);
	else 
		m_checklist.SetCheck(2,0);
	if (findtoolbar(L"方案管理")) 
		m_checklist.SetCheck(3,1);
	else 
		m_checklist.SetCheck(3,0);

	if (findtoolbar(L"二线设计"))
		m_checklist.SetCheck(4,1);
	else 
		m_checklist.SetCheck(4,0);


	if (findtoolbar(L"平面一体化")) 
		m_checklist.SetCheck(5,1);
	else 
		m_checklist.SetCheck(5,0);

	if (findtoolbar(L"断链设置")) 
		m_checklist.SetCheck(6,1);
	else 
		m_checklist.SetCheck(6,0);

	if (findtoolbar(L"坡度设计"))
		m_checklist.SetCheck(7,1);
	else 
		m_checklist.SetCheck(7,0);


	if (findtoolbar(L"桥梁设置")) 
		m_checklist.SetCheck(8,1);
	else 
		m_checklist.SetCheck(8,0);

	if (findtoolbar(L"隧道设置")) 
		m_checklist.SetCheck(9,1);
	else 
		m_checklist.SetCheck(9,0);

	if (findtoolbar(L"纵断面一体化")) 
		m_checklist.SetCheck(10,1);
	else 
		m_checklist.SetCheck(10,0);

	if (findtoolbar(L"横断面设计")) 
		m_checklist.SetCheck(11,1);
	else 
		m_checklist.SetCheck(11,0);

	if (findtoolbar(L"用地设计")) 
		m_checklist.SetCheck(12,1);
	else 
		m_checklist.SetCheck(12,0);

	if (findtoolbar(L"平面图")) 
		m_checklist.SetCheck(13,1);
	else 
		m_checklist.SetCheck(13,0);

	if (findtoolbar(L"纵断面图")) 
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

	if(iState==0&&rString==L"项目管理")
	{
		darcydarcyxmglout(); //不弹出工具条
	}
	if(iState==1&&rString==L"项目管理")
	{
		darcydarcyxmgl();  //弹出工具条

	}

	if(iState==0&&rString==L"建立数模")
	{
		darcydarcyjlsmout(); //不弹出工具条
	}
	if(iState==1&&rString==L"建立数模")
	{
		darcydarcyjlsm();  //弹出工具条

	}
	if (iState==0&&rString==L"基线设计") 
	{
		DarcySLDESIGNDXSJOUT();
	}
	if(iState==1&&rString==L"基线设计")
	{
		darcysldesignsldesign();  //弹出工具条
	}
	if (iState==0&&rString==L"方案管理") 
	{
		DarcyFAGLOUT();
	}
	if(iState==1&&rString==L"方案管理")
	{
		darcysldesignfagl();  //弹出工具条
	}

	if (iState==0&&rString==L"二线设计") 
	{
		DarcyDLDESIGNDLSJOUT();
	}
	if(iState==1&&rString==L"二线设计")
	{
		darcydldesigndldesign(); //弹出工具条
	}
	if (iState==0&&rString==L"断链设置") 
	{
		DarcyYTHDLSZOUT(); 
	}
	if(iState==1&&rString==L"断链设置")
	{
		darcydldes(); //弹出工具条
	}
	if (iState==0&&rString==L"平面一体化") 
	{
		DarcyDLDESIGNPMYTHOUT(); 

	}
	if(iState==1&&rString==L"平面一体化")
	{
		darcyythyth(); //弹出工具条
	}
	if (iState==0&&rString==L"坡度设计") 
	{
		DarcyPDDESIGNPDSJOUT();
	}
	if(iState==1&&rString==L"坡度设计")
	{
		darcypddesignpddesign(); //弹出工具条
	}
	if (iState==0&&rString==L"桥梁设置") 
	{
		DarcyBRIDGEDESIGNQLSJOUT();
	}
	if(iState==1&&rString==L"桥梁设置")
	{

		darcybridgedesignbridgedes(); //弹出工具条
	}
	if (iState==0&&rString==L"隧道设置") 
	{
		DarcySDDESIGNSDDESIGNOUT();
	}
	if(iState==1&&rString=="隧道设置")
	{
		darcysddesignsddesign(); //弹出工具条
	}
	if (iState==0&&rString=="纵断面一体化") 
	{
		DarcyDLDESIGNZMYTHSJOUT();
	}
	if(iState==1&&rString=="纵断面一体化")
	{
		darcyzdmdeszdmdes(); //弹出工具条
	}
	if (iState==0&&rString=="横断面设计") 
	{
		DarcyHDMDESOUT();
	}
	if(iState==1&&rString=="横断面设计")
	{
		darcyhdmdes(); //弹出工具条
	}
	if (iState==0&&rString=="用地设计") 
	{
		DarcyYDOUT();
	}
	if(iState==1&&rString=="用地设计")
	{
		darcyyddes(); //弹出工具条
	}

	if (iState==0&&rString=="平面图") 
	{
		DarcyPLANPMTOUT();
	}
	if(iState==1&&rString=="平面图")
	{
		darcyplanplan(); //弹出工具条
	}
	if (iState==0&&rString=="纵断面图") 
	{
		DarcyZDMDESZDMTOUT();
	}
	if(iState==1&&rString=="纵断面图")
	{
		darcyverticalsectionver_section(); //弹出工具条
	}
	/*
	if (iState==0&&rString=="表格输出") 
	{

	}
	if(iState==1&&rString=="表格输出")
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

	if(iState==0&&rString=="项目管理")
	{
		darcydarcyxmglout(); //不弹出工具条
	}
	if(iState==1&&rString=="项目管理")
	{
		darcydarcyxmgl();  //弹出工具条

	}
	if(iState==0&&rString=="建立数模")
	{
		darcydarcyjlsmout(); //不弹出工具条
	}
	if(iState==1&&rString=="建立数模")
	{
		darcydarcyjlsm();  //弹出工具条
	}
	if (iState==0&&rString=="基线设计") 
	{
		DarcySLDESIGNDXSJOUT();
	}
	if(iState==1&&rString=="基线设计")
	{
		darcysldesignsldesign();  //弹出工具条
	}
	if (iState==0&&rString=="方案管理") 
	{
		DarcyFAGLOUT();
	}
	if(iState==1&&rString=="方案管理")
	{
		darcysldesignfagl();  //弹出工具条
	}
	if (iState==0&&rString=="二线设计") 
	{
		DarcyDLDESIGNDLSJOUT();
	}
	if(iState==1&&rString=="二线设计")
	{
		darcydldesigndldesign(); //弹出工具条
	}
	if (iState==0&&rString=="断链设置") 
	{
		DarcyYTHDLSZOUT(); 
	}
	if(iState==1&&rString=="断链设置")
	{
		darcydldes(); //弹出工具条
	}
	if (iState==0&&rString=="平面一体化") 
	{
		DarcyDLDESIGNPMYTHOUT(); 

	}
	if(iState==1&&rString=="平面一体化")
	{
		darcyythyth(); //弹出工具条
	}
	if (iState==0&&rString=="坡度设计") 
	{
		DarcyPDDESIGNPDSJOUT();
	}
	if(iState==1&&rString=="坡度设计")
	{
		darcypddesignpddesign(); //弹出工具条
	}
	if (iState==0&&rString=="桥梁设置") 
	{
		DarcyBRIDGEDESIGNQLSJOUT();
	}
	if(iState==1&&rString=="桥梁设置")
	{
		darcybridgedesignbridgedes(); //弹出工具条
	}
	if (iState==0&&rString=="隧道设置") 
	{
		DarcySDDESIGNSDDESIGNOUT();
	}
	if(iState==1&&rString=="隧道设置")
	{
		darcysddesignsddesign(); //弹出工具条
	}
	if (iState==0&&rString=="纵断面一体化") 
	{
		DarcyDLDESIGNZMYTHSJOUT();
	}
	if(iState==1&&rString=="纵断面一体化")
	{
		darcyzdmdeszdmdes(); //弹出工具条
	}	if (iState==0&&rString=="横断面设计") 
	{
		DarcyHDMDESOUT();
	}
	if(iState==1&&rString=="横断面设计")
	{
		darcyhdmdes(); //弹出工具条
	}
	if (iState==0&&rString=="用地设计") 
	{
		DarcyYDOUT();
	}
	if(iState==1&&rString=="用地设计")
	{
		darcyyddes(); //弹出工具条
	}


	if (iState==0&&rString=="平面图") 
	{
		DarcyPLANPMTOUT();
	}
	if(iState==1&&rString=="平面图")
	{
		darcyplanplan(); //弹出工具条
	}
	if (iState==0&&rString=="纵断面图") 
	{
		DarcyZDMDESZDMTOUT();
	}
	if(iState==1&&rString=="纵断面图")
	{
		darcyverticalsectionver_section(); //弹出工具条
	}
	/*
	if (iState==0&&rString=="表格输出") 
	{

	}*/

	/*
	if(iState==1&&rString=="表格输出")
	{

	}*/




}
