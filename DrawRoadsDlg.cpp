// DrawRoadsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "DrawRoadsDlg.h"
#include "XLDataBase.h"
#include "DrawPmDlg.h"

// CDrawRoadsDlg 对话框

IMPLEMENT_DYNAMIC(CDrawRoadsDlg, CDialog)
	CDrawRoadsDlg::CDrawRoadsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawRoadsDlg::IDD, pParent)
{
	m_SelRoadArray.RemoveAll();
	m_UnSelRoadArray.RemoveAll();
}

CDrawRoadsDlg::~CDrawRoadsDlg()
{
	m_SelRoadArray.RemoveAll();
	m_UnSelRoadArray.RemoveAll();
}

void CDrawRoadsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UnSel, m_ListBox_UnSel);
	DDX_Control(pDX, IDC_LIST_Sel, m_ListBox_Sel);
}


BEGIN_MESSAGE_MAP(CDrawRoadsDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SETMOBAN, OnBnClickedButtonSetmoban)
	ON_BN_CLICKED(IDC_CHECK_ALL, OnBnClickedCheckAll)
	ON_LBN_DBLCLK(IDC_LIST_UnSel, OnLbnDblclkListUnsel)
	ON_LBN_DBLCLK(IDC_LIST_Sel, OnLbnDblclkListSel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CDrawRoadsDlg 消息处理程序

BOOL CDrawRoadsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	XLDataBase xlmdb;
	if(!xlmdb.ReadWorkdir())
	{
		AfxMessageBox(L"请指定工作目录和方案名称!");
		return false;
	}

#if ((defined GY_Road_SYS)||(defined GY_ROAD_DULI_SYS))
	CString MdfName = Cworkdir + L"\\DATA\\改移道路.mdf";
	xlmdb.Read_XLDbs(MdfName, L"改移道路表");
	if (xlmdb.ModifyRoadSum>0)
	{
		CString MdbName;
		CFileFind Findlg;
		int RoadSum = 0;
		for (int i=0; i<xlmdb.ModifyRoadSum; i++)
		{
			MdbName = Cworkdir + L"\\DATA\\" + xlmdb.pModifyRoad[i].RoadName + L".mdb";
			MdbName.Trim();
			MdbName.MakeUpper();

			if (Findlg.FindFile(MdbName))
			{
				m_ListBox_UnSel.AddString(xlmdb.pModifyRoad[i].RoadName);
				RoadSum++;
				m_AllRoadArray.Add(xlmdb.pModifyRoad[i].RoadName);
			}
		}
		if (RoadSum<1)
		{
			m_ListBox_UnSel.EnableWindow(FALSE);
			AfxMessageBox(L"目前你还没有设计线路！");
		}
	}
	else
	{
		m_ListBox_UnSel.EnableWindow(FALSE);
		AfxMessageBox(L"目前你还没有设计线路！");
	}
#endif
	

	CFileFind Findlg;
	CString MBFName = Cworkdir + "\\DATA\\模板.pmcs";
	if (Findlg.FindFile(MBFName)==0)
	{
		CString LibMBFame;
		LibMBFame.Format(L"%s\\Lib\\模板.pmcs",SysPath);
		CopyFile(LibMBFame,MBFName,false);
	}
	CString MBDrawSetFName = Cworkdir + "\\DATA\\模板.PmDrawSet";
	if (Findlg.FindFile(MBDrawSetFName)==0)
	{
		CString LibDrawSetFame;
		LibDrawSetFame.Format(L"%s\\Lib\\模板.PmDrawSet",SysPath);
		CopyFile(LibDrawSetFame,MBDrawSetFName,false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CDrawRoadsDlg::OnBnClickedButtonSet()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSelIndex;
	CString strCurSel="";
	nCurSelIndex = m_ListBox_Sel.GetCurSel();

	//if (strCurSel.IsEmpty())
	if (nCurSelIndex == -1)
	{
		AfxMessageBox(L"没有选中MDB");
		return;
	}
	else
	{
		m_ListBox_Sel.GetText(nCurSelIndex,strCurSel);
		CDrawPmDlg dlg;
		dlg.m_Dbs = Cworkdir + "\\DATA\\" + strCurSel + ".mdb";
		dlg.DoModal();
	}
}

void CDrawRoadsDlg::OnBnClickedButtonSetmoban()
{
	// TODO: 在此添加控件通知处理程序代码
	CDrawPmDlg dlg;
	dlg.m_Dbs = Cworkdir + L"\\DATA\\模板.mdb";
	dlg.DoModal();
}

void CDrawRoadsDlg::OnBnClickedCheckAll()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ListBox_Sel.ResetContent();
	m_ListBox_UnSel.ResetContent();
	for (int i=0; i<m_AllRoadArray.GetSize(); i++)
	{
		m_ListBox_Sel.AddString(m_AllRoadArray[i]);
	}
}

void CDrawRoadsDlg::OnLbnDblclkListUnsel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSelIndex;
	CString strCurSel;
	nCurSelIndex = m_ListBox_UnSel.GetCurSel();
	m_ListBox_UnSel.GetText(nCurSelIndex,strCurSel);
	m_ListBox_UnSel.DeleteString(nCurSelIndex);
	m_ListBox_Sel.AddString(strCurSel);
}

void CDrawRoadsDlg::OnLbnDblclkListSel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCurSelIndex;
	CString strCurSel;
	nCurSelIndex = m_ListBox_Sel.GetCurSel();
	m_ListBox_Sel.GetText(nCurSelIndex,strCurSel);
	m_ListBox_Sel.DeleteString(nCurSelIndex);
	m_ListBox_UnSel.AddString(strCurSel);
}

void CDrawRoadsDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	m_SelRoadArray.RemoveAll();
	int RoadSum = m_ListBox_Sel.GetCount();
	m_SelRoadArray.SetSize(RoadSum);
	for (int i=0; i<RoadSum; i++)
	{
		m_ListBox_Sel.GetText(i, m_SelRoadArray[i]);
	}
	OnOK();
}
