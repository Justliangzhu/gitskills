﻿// CSetLevelCtrlDlg.cpp: 实现文件
//
#include "StdAfx.h"
#include "CSetLevelCtrlDlg.h"
#include "afxdialogex.h"
#include "RoadDesignWorkingDatabase.h"
#include "Ckzddlg.h"

extern CString Cworkdir, Pro;
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;
// CSetLevelCtrlDlg 对话框


IMPLEMENT_DYNAMIC(CSetLevelCtrlDlg, CDialog)

CSetLevelCtrlDlg::CSetLevelCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SetLevelCtrlData, pParent)
{
	
	ptempRoad = NULL;
}

CSetLevelCtrlDlg::~CSetLevelCtrlDlg()
{
}

void CSetLevelCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SETLEVELCTRL, m_GridCtrl);
	DDX_Control(pDX, IDC_COMBO_RoadName, m_ComboxRoadName);
}


BEGIN_MESSAGE_MAP(CSetLevelCtrlDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_SAVE, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlSave)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_ADDROW, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlAddrow)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_DELROW, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlDelrow)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_HPICK, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlHpick)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_VPICK, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlVpick)
	ON_BN_CLICKED(IDC_BUTTON_SETLEVELCTRL_DRAW, &CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlDraw)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetLevelCtrlDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO_RoadName, &CSetLevelCtrlDlg::OnCbnSelchangeComboRoadname)
END_MESSAGE_MAP()


// CSetLevelCtrlDlg 消息处理程序


BOOL CSetLevelCtrlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	BAS_DRAW_FUN Bas_Fun;
	m_MdbName = Bas_Fun.GetCurProMdbPath();
	XLDataBase DBS;
	//CString MdfRoadNmae;
	//MdfRoadNmae = Pro;
	//初始化所有线路名
	//初始化开始
	DBS.ReadAllRoadName(m_MdbName);
	if (DBS.m_RoadSum < 1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return FALSE;
	}
	m_ComboxRoadName.ResetContent();
	for (int i = 0; i < DBS.m_RoadSum; i++)
	{
		m_ComboxRoadName.AddString(DBS.Road[i].RoadName);
		ads_printf(L"%d %s\n", i, DBS.Road[i].RoadName);
	}
		
	m_ComboxRoadName.SetCurSel(0);
	m_RoadName = DBS.Road[0].RoadName;
	//初始化完毕
	g_RoadWorkingDatabase.GetLevelCtrlData(m_MdbName, m_RoadName, m_LevelCtrlPtArray);
	CString WindowText = L"设置控制标高-当前方案：" + Pro;
	SetWindowText(WindowText);
	InitGridCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CSetLevelCtrlDlg::OnCbnSelchangeComboRoadname()
{
	XLDataBase DBS;
	//CString MdfRoadNmae;
	//MdfRoadNmae = Pro;
	//初始化所有线路名
	//初始化开始
	DBS.ReadAllRoadName(m_MdbName);
	if (DBS.m_RoadSum < 1)
	{
		ads_printf(L"数据库中没有有效的道路名!\n");
		return;
	}
	int iroad = m_ComboxRoadName.GetCurSel();
	m_RoadName = DBS.Road[iroad].RoadName;
	//重新更新所有的数据
	m_LevelCtrlPtArray.clear();
	g_RoadWorkingDatabase.GetLevelCtrlData(m_MdbName, m_RoadName, m_LevelCtrlPtArray);
	WriteGridCtrl();
	UpdateData(TRUE);
	m_GridCtrl.Invalidate();
	// TODO: 在此添加控件通知处理程序代码
}

//初始化表格控件
void CSetLevelCtrlDlg::InitGridCtrl()
{
	ACHAR tmp[50];

	//设置行和列
	m_GridCtrl.SetRowCount(3);
	m_GridCtrl.SetFixedRowCount(1);
	m_GridCtrl.SetColumnCount(6);
	m_GridCtrl.SetFixedColumnCount(1);

	//设置单元格属性

	LOGFONT font = { 12, 0, 0, 0, 400, FALSE, FALSE, 0,
	ANSI_CHARSET, OUT_DEFAULT_PRECIS,
	CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
	DEFAULT_PITCH | FF_SWISS,L"Arial" };
	m_Item.mask = GVIF_TEXT | GVIF_FORMAT;
	m_Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	m_Item.lfFont = font;

	//传入单元格属性
	for (int k = 0; k < m_GridCtrl.GetColumnCount(); k++)
	{
		for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			m_GridCtrl.SetItem(&m_Item);
		}
	}

	//设置标题
	m_GridCtrl.SetItemText(0, 0, L"序号");
	m_GridCtrl.SetItemText(0, 1, L"冠号");
	m_GridCtrl.SetItemText(0, 2, L"里程");
	m_GridCtrl.SetItemText(0, 3, L"高程上限(m)");
	m_GridCtrl.SetItemText(0, 4, L"高程下限(m)");
	m_GridCtrl.SetItemText(0, 5, L"备注");

	//设置宽度与长度
	CRect Rect;
	m_GridCtrl.GetClientRect(&Rect);
	int Onewidth = int(Rect.Width() / 10.0);

	m_GridCtrl.SetColumnWidth(0, Onewidth * 1);
	m_GridCtrl.SetColumnWidth(1, Onewidth * 1);
	m_GridCtrl.SetColumnWidth(2, Onewidth * 2);
	m_GridCtrl.SetColumnWidth(3, Onewidth * 2);
	m_GridCtrl.SetColumnWidth(4, Onewidth * 2);
	m_GridCtrl.SetColumnWidth(5, Rect.Width() - Onewidth * 8);
	for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		m_GridCtrl.SetRowHeight(i, 25);

	//设置序列号
	for (int i = 1; i < 3; i++)
	{
		_stprintf(tmp, L"%d", i);
		m_GridCtrl.SetItemText(i, 0, tmp);
	}

	//额外设置
	m_GridCtrl.ResetScrollBars();
	m_GridCtrl.SetFocusCell(1, 1);

	if (m_LevelCtrlPtArray.size())
		WriteGridCtrl();

	UpdateData(FALSE);
	m_GridCtrl.Invalidate();
}

//读表格
void CSetLevelCtrlDlg::ReadGridCtrl()
{
	std::vector<GCKZPt> temp_m_LevelCtrlPtArray;
	temp_m_LevelCtrlPtArray.clear();
	for (int i=0;i< m_LevelCtrlPtArray.size();i++)
	{
		if (m_LevelCtrlPtArray[i].Note.Find(L"交叉") >= 0)
		{
			temp_m_LevelCtrlPtArray.push_back(m_LevelCtrlPtArray[i]);

		}
	}
	m_LevelCtrlPtArray.clear();
	GCKZPt tempGCKZPt;
	tempGCKZPt.RoadName = m_RoadName;
	CString tempText, tempCeil, tempFloor;

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		ads_printf(L"无平面方案，无法设置高程控制点！");
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		return;
	}

	for (int i = 1; i < m_GridCtrl.GetRowCount(); i++)
	{
		tempText = m_GridCtrl.GetItemText(i, 1);
		if (tempText.IsEmpty())
			continue;

		_tcscpy(tempGCKZPt.GH, tempText);

		tempText = m_GridCtrl.GetItemText(i, 2);
		if (tempText.IsEmpty())
			continue;
		tempGCKZPt.XCLc = _wtof(tempText);

		tempCeil = m_GridCtrl.GetItemText(i, 3);
		tempFloor = m_GridCtrl.GetItemText(i, 4);

		if(tempCeil.IsEmpty() && tempFloor.IsEmpty())
			continue;

		if (tempCeil.IsEmpty())
		{
		/*	tempGCKZPt.DesHSX = L"";*/
			tempGCKZPt.DesHXX = _wtof(tempFloor);
			tempGCKZPt.Style = 2;	//只有下限点
		}
		else if (tempFloor.IsEmpty())
		{
			tempGCKZPt.DesHSX = _wtof(tempCeil);
			/*tempGCKZPt.DesHXX = _T("");*/
			tempGCKZPt.Style = 1;	//只有下限点
		}
		else
		{
			tempGCKZPt.DesHSX = _wtof(tempCeil);
			tempGCKZPt.DesHXX = _wtof(tempFloor);
			tempGCKZPt.Style = 3;
		}
		tempGCKZPt.Note = m_GridCtrl.GetItemText(i, 5);
		if (tempGCKZPt.Note.Find(L"交叉")>=0)
		{
			continue;
		}
		m_LevelCtrlPtArray.push_back(tempGCKZPt);
	}

	ACHAR CKML[64];
	for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
	{
		_tcscpy(CKML, pRoad->LCchr(m_LevelCtrlPtArray[i].GH, m_LevelCtrlPtArray[i].XCLc, 3));
		m_LevelCtrlPtArray[i].Cml = pRoad->Trs_Ckml_To_ProjMl(CKML);
	}
	for (int i=0;i< temp_m_LevelCtrlPtArray.size();i++)
	{
		m_LevelCtrlPtArray.push_back(temp_m_LevelCtrlPtArray[i]);
	}
}

//写表格
void CSetLevelCtrlDlg::WriteGridCtrl()
{
	//设置行和列
	m_GridCtrl.SetRowCount(1 + m_LevelCtrlPtArray.size());

	//传入单元格属性
	for (int k = 0; k < m_GridCtrl.GetColumnCount(); k++)
	{
		for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		{
			m_Item.row = i;
			m_Item.col = k;
			m_GridCtrl.SetItem(&m_Item);
		}
	}

	for (int i = 0; i < m_GridCtrl.GetRowCount(); i++)
		m_GridCtrl.SetRowHeight(i, 25);

	//设置标题
	m_GridCtrl.SetItemText(0, 0, L"序号");
	m_GridCtrl.SetItemText(0, 1, L"冠号");
	m_GridCtrl.SetItemText(0, 2, L"里程");
	m_GridCtrl.SetItemText(0, 3, L"高程上限(m)");
	m_GridCtrl.SetItemText(0, 4, L"高程下限(m)");
	m_GridCtrl.SetItemText(0, 5, L"备注");

	//设置序列号
	ACHAR tmp[50];
	for (int i = 1; i < m_GridCtrl.GetRowCount(); i++)
	{
		_stprintf(tmp, L"%d", i);
		m_GridCtrl.SetItemText(i, 0, tmp);
	}

	//写入数据
	for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
	{
		m_GridCtrl.SetItemText(i + 1, 1, m_LevelCtrlPtArray[i].GH);

		_stprintf(tmp, L"%.3lf", m_LevelCtrlPtArray[i].XCLc);
		m_GridCtrl.SetItemText(i + 1, 2, tmp);

		if (m_LevelCtrlPtArray[i].Style == 1 || m_LevelCtrlPtArray[i].Style == 3)
		{
			_stprintf(tmp, L"%.3lf", m_LevelCtrlPtArray[i].DesHSX);
			m_GridCtrl.SetItemText(i + 1, 3, tmp);
		}

		if (m_LevelCtrlPtArray[i].Style == 2 || m_LevelCtrlPtArray[i].Style == 3)
		{
			_stprintf(tmp, L"%.3lf", m_LevelCtrlPtArray[i].DesHXX);
			m_GridCtrl.SetItemText(i + 1, 4, tmp);
		}

		m_GridCtrl.SetItemText(i + 1, 5, m_LevelCtrlPtArray[i].Note);


		if (m_LevelCtrlPtArray[i].Note.Find(L"交叉点") >= 0)
		{
			for (int j =0;j<6;j++)
			{
				m_GridCtrl.SetItemState(i + 1, j, m_GridCtrl.GetItemState(i + 1, j) | GVIS_READONLY); //只读
				m_GridCtrl.SetItemBkColour(i + 1 , j, RGB(255, 192, 203)); //颜色区别
			}

		}

	}
}


void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlSave()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadGridCtrl();
	/*for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
	{
		if (m_LevelCtrlPtArray[i].Note.CompareNoCase(L"交叉点") == 0)
			m_LevelCtrlPtArray[i].clearrence = m_LevelCtrlPtArray[i].clearrence;
	}*/
	g_RoadWorkingDatabase.SetLevelCtrlData(m_MdbName, m_RoadName, m_LevelCtrlPtArray, true);
	ads_printf(L"成功保存控制标高数据至数据库！");
}


void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlAddrow()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_GridCtrl.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_Item.row = nRow;
		m_GridCtrl.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_GridCtrl.GetColumnCount(); i++)
		{
			m_Item.col = i;
			m_GridCtrl.SetItem(&m_Item);
		}
		for (i = nRow; i < m_GridCtrl.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_GridCtrl.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_GridCtrl.GetRowCount(); i++)
			m_GridCtrl.SetRowHeight(i, 25);

		m_GridCtrl.SetFocusCell(nRow, 1);
	}

	UpdateData(FALSE);
	m_GridCtrl.Invalidate();
}


void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlDelrow()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	int nRow = m_GridCtrl.GetFocusCell().row;
	if (nRow >= 0)
	{
		m_GridCtrl.DeleteRow(nRow);
		int i;
		for (i = nRow; i < m_GridCtrl.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_GridCtrl.SetItemText(i, 0, str);
		}
		m_GridCtrl.SetFocusCell(nRow, 1);
		m_GridCtrl.Invalidate();
	}
}


void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlHpick()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int Row = m_GridCtrl.GetFocusCell().row;
	if (Row < 0)
	{
		AfxMessageBox(L"请先在表格中选中需要拾取里程的对应行，再执行该命令");
		return;
	}

	ads_point resPt;
	ShowWindow(SW_HIDE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		ads_printf(L"无平面方案，无法拾取里程！");
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		return;
	}

	if (acedGetPoint(NULL, L"请在屏幕上拾取一点!\n", resPt) == RTNORM)
	{
		ACHAR AcharCkml[256], GH[64], temp[256];
		_tcscpy(AcharCkml, pRoad->Trs_NE_To_Ckml(resPt[Y], resPt[X]));
		double Dml = 0.0;
		pRoad->split_ckml(AcharCkml, &Dml, GH);
		m_GridCtrl.SetItemText(Row, 1, GH);
		_stprintf(temp, L"%.3lf", Dml);
		m_GridCtrl.SetItemText(Row, 2, temp);
		UpdateData(FALSE);
		m_GridCtrl.Invalidate();
	}

	ShowWindow(SW_SHOW);
	SetFocus(); // Reset the focus back to ourselves
}

/*===================================================================
函数名:OnBnClickedButtonSetlevelctrlVpick
------------------------------------------------------------
函数功能:从纵面拾取里程
------------------------------------------------------------
输入:
------------------------------------------------------------
输出:
------------------------------------------------------------
算法描述:	1.检测用户是否选中表格中行，未选中提示用户先选择，
			再执行该命令，退出
				1.1生成平面指针，生成失败退出该命令
			2.隐藏对话框
			3.提示用户选择坡度线实体，用户未选择则恢复对话框，
			退出
			4.锁文档，检测拾取实体是否是坡度线实体，不是或没
			打开则恢复对话框，解锁文档，退出
			5.提示用户拾取里程点，若用户未选择，跳转7
			6.根据坡度实体计算出连续里程，再利用平面实体计算出
			现场里程
			7.恢复对话框，解锁文档，命令执行完毕，退出
------------------------------------------------------------
编写日期:20190919
------------------------------------------------------------
备注：
=====================================================================*/
void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlVpick()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int Row = m_GridCtrl.GetFocusCell().row;
	if (Row < 0)
	{
		AfxMessageBox(L"请先在表格中选中需要拾取里程的对应行，再执行该命令");
		return;
	}

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		ads_printf(L"无平面方案，无法拾取里程！");
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		//acedGetAcadFrame()->EnableWindow(FALSE); // Enable our dialog*/
		return;
	}

	ads_point resPt;
	ShowWindow(SW_HIDE);
	//acedGetAcadFrame()->EnableWindow(TRUE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity* pEnt;

	//如果用户空选或者按esc，那么返回
	if (RTNORM != acedEntSel(L"\n请选择坡度线实体:", en, pt))
	{
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		return;
	}

	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//正常选择了线路实体
	acdbGetObjectId(eId, en);
	if (acdbOpenObject(pEnt, eId, AcDb::kForWrite) != Acad::eOk)
	{
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		acDocManager->unlockDocument(acDocManager->curDocument());
		return;
	}
		
	pEnt->close();
	GTZDM_ROAD* pProfile = NULL;
	if (!pEnt->isKindOf(GTZDM_ROAD::desc()))
	{
		AfxMessageBox(L"所选并非坡度线实体！");
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		acDocManager->unlockDocument(acDocManager->curDocument());
		return;
	}
	else
		pProfile = GTZDM_ROAD::cast(pEnt);

	//提示用户拾取里程
	if (acedGetPoint(NULL, L"\n请在屏幕上拾取一点\n", resPt) == RTNORM)
	{
		ACHAR AcharCkml[256], GH[64], temp[256];

		double Cml = pProfile->Trs_Coor_System(resPt[X], MILE, TO_SLOPE);
		_tcscpy(AcharCkml, pRoad->Trs_ProjMl_To_Ckml(Cml));
		double Dml = 0.0;
		pRoad->split_ckml(AcharCkml, &Dml, GH);
		m_GridCtrl.SetItemText(Row, 1, GH);
		_stprintf(temp, L"%.3lf", Dml);
		m_GridCtrl.SetItemText(Row, 2, temp);
		UpdateData(FALSE);
		m_GridCtrl.Invalidate();
	}

	ShowWindow(SW_SHOW);
	SetFocus(); // Reset the focus back to ourselves
	acDocManager->unlockDocument(acDocManager->curDocument());
}


void CSetLevelCtrlDlg::OnBnClickedButtonSetlevelctrlDraw()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	ReadGridCtrl();

	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	if (!pRoad)
	{
		ads_printf(L"无平面方案，无法绘图！");
		return;
	}

	ads_point resPt;
	ShowWindow(SW_HIDE);
	//acedGetAcadFrame()->EnableWindow(TRUE);
	acedGetAcadFrame()->SetFocus();	// Give AutoCAD the focus.

	ads_name en;
	ads_point pt;
	AcDbObjectId eId;
	AcDbEntity* pEnt;

	//如果用户空选或者按esc，那么返回
	if (RTNORM != acedEntSel(L"\n请选择坡度线实体:", en, pt))
	{
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		return;
	}

	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	//正常选择了线路实体
	acdbGetObjectId(eId, en);
	if (acdbOpenObject(pEnt, eId, AcDb::kForWrite) != Acad::eOk)
	{
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		acDocManager->unlockDocument(acDocManager->curDocument());
		return;
	}

	GTZDM_ROAD* pProfile = NULL;
	if (!pEnt->isKindOf(GTZDM_ROAD::desc()))
	{
		AfxMessageBox(L"所选并非坡度线实体！");
		ShowWindow(SW_SHOW);
		SetFocus(); // Reset the focus back to ourselves
		acDocManager->unlockDocument(acDocManager->curDocument());
		return;
	}
	else
		pProfile = GTZDM_ROAD::cast(pEnt);

	pProfile->assertWriteEnabled();

	ACHAR CKML[64];
	for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
	{
		_tcscpy(CKML, pRoad->LCchr(m_LevelCtrlPtArray[i].GH, m_LevelCtrlPtArray[i].XCLc, 3));
		m_LevelCtrlPtArray[i].Cml = pRoad->Trs_Ckml_To_ProjMl(CKML);
	}

	g_RoadWorkingDatabase.SetLevelCtrlData(m_MdbName, m_RoadName, m_LevelCtrlPtArray);
	pProfile->m_LevelCtrlPtArray = m_LevelCtrlPtArray;
	pProfile->close();

	ShowWindow(SW_SHOW);
	SetFocus(); // Reset the focus back to ourselves
	acDocManager->unlockDocument(acDocManager->curDocument());
}
int CSetLevelCtrlDlg::GetNumOfTK()
{
	int rc;
	//int NumOfTK = -1;
	ads_name en;
	ads_point pt;
	AcDbObject* pObj;
	AcDbObjectId eID;
	//选取纵面实体 
	rc = ads_entsel(L"\n请拾取纵面地面线", en, pt);
	if (rc != RTNORM) //用户输入一个有效值,表示函数调用成功
		return -1;

	//获取实体ID
	acdbGetObjectId(eID, en);

	if (acdbOpenObject(pObj, eID, AcDb::kForWrite) != Acad::eOk)
	{
		ads_printf(L"打开实体失败！\n");
		/*return -1;*/
	}
	pObj->close();

	//根据实体获取实体
	if (pObj->isKindOf(DMX_ROAD::desc()))			 //拾取地面
	{
		DMX_ROAD* pDMX = DMX_ROAD::cast(pObj);
		NumOfTK = pDMX->Zdmno;
	}
	else
		ads_printf(L"未正确拾取实体!\n");

	return NumOfTK;

}

void CSetLevelCtrlDlg::OnBnClickedButton1()
{
	//NumOfTK = 0;
	//NumOfTK = GetNumOfTK();
	//ptempRoad = GlobalZDMTK.ZDMTK[NumOfTK].pm;

	JD_CENTER* ptempRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_MdbName, m_RoadName);
	XLDataBase DBS;

	DBS.Read_XLDbs(m_MdbName, L"道铁交叉信息表", m_RoadName);
	if (DBS.m_RoadNRailInterInforArray.size())
		ptempRoad->m_InterSectData = DBS.m_RoadNRailInterInforArray[0];
	
	if (!ptempRoad)
	{
		ads_printf(L"无平面方案！");
		return;
	}
	Ckzddlg SetcontrolDlg;
	SetcontrolDlg.pline = ptempRoad;
	SetcontrolDlg.m_MdbName = m_MdbName;

	ReadGridCtrl();
	if (SetcontrolDlg.DoModal()==IDOK)
	{
		if (m_LevelCtrlPtArray.size() >= 1)
		{
			for (int i = 0; i < m_LevelCtrlPtArray.size(); i++)
			{
				if (m_LevelCtrlPtArray[i].Note.Find(L"交叉点")>=0)
				{
					m_LevelCtrlPtArray.erase(m_LevelCtrlPtArray.begin()+i);
				}
			}
		}
		for (int i=0;i< SetcontrolDlg.m_LevelCtrlPtArray.size();i++)
		{
			m_LevelCtrlPtArray.push_back(SetcontrolDlg.m_LevelCtrlPtArray[i]);
			break;
		}
		
		
		WriteGridCtrl();
		UpdateData(FALSE);
		m_GridCtrl.Invalidate();
	}

		return;
	// TODO: 在此添加控件通知处理程序代码
}


