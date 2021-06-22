// EditAllRoads.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "EditAllRoads.h"

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
extern CString Cworkdir,Mdf;
// CEditAllRoads 对话框

IMPLEMENT_DYNAMIC(CEditAllRoads, CResizableDialog)
	CEditAllRoads::CEditAllRoads(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CEditAllRoads::IDD, pParent)
{
	m_grid.TableName = "线路相对关系表";
}

CEditAllRoads::~CEditAllRoads()
{
}

void CEditAllRoads::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_LIST_EDITALLROAD,m_grid);
}


BEGIN_MESSAGE_MAP(CEditAllRoads, CResizableDialog)
	ON_BN_CLICKED(IDC_BUTTON_INS, OnBnClickedButtonIns)
	ON_BN_CLICKED(IDC_BUTTON_del, OnBnClickedButtondel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditAllRoads 消息处理程序

void CEditAllRoads::OnBnClickedButtonIns()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{ 
		Item.row=nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for( i=0;i<m_grid.GetColumnCount();i++)
		{
			Item.col=i;
			m_grid.SetItem(&Item);
		}
		for(i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}

		if(m_grid.GetRowCount()>3)
		{
			if(nRow==m_grid.GetRowCount()-2)
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow-1,i));
			}
			else
			{
				for( i=1;i<m_grid.GetColumnCount();i++) 
					m_grid.SetItemText(nRow,i,m_grid.GetItemText(nRow+1,i));
			}
		}
		m_grid.SetFocusCell(nRow,1);

		m_grid.Invalidate();
	}
}

void CEditAllRoads::OnBnClickedButtondel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if  (nRow==m_grid.GetRowCount()-1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for(int i=nRow;i<m_grid.GetRowCount();i++)
		{
			CString str;
			str.Format(L"%d",i);
			m_grid.SetItemText(i,0,str);
		}
		m_grid.SetFocusCell(nRow,1);
		m_grid.Invalidate();
	}
}

void CEditAllRoads::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ModifyRoadArray.RemoveAll();
	UpdateData(true);
	int RowSum = m_grid.GetRowCount();
	CString Text;
	BAS_DRAW_FUN::ModifyRoadTab ModifyRoad;
	for (int i=0; i<RowSum; i++)
	{
		Text = m_grid.GetItemText(i+1,1);
		Text.Trim();
		if (!Text.IsEmpty())
		{
			ModifyRoad.RoadName = Text;
			ModifyRoad.RailName = m_grid.GetItemText(i+1, 2);
			ModifyRoad.SCkml = m_grid.GetItemText(i+1, 3);
			ModifyRoad.RailCkml = m_grid.GetItemText(i+1, 4);
			ModifyRoad.ECkml = m_grid.GetItemText(i+1, 5);
			ModifyRoad.RoadDml = _wtof(m_grid.GetItemText(i+1, 6));
			ModifyRoad.RoadLen = _wtof(m_grid.GetItemText(i+1, 7));
			ModifyRoad.InterAng = _wtof(m_grid.GetItemText(i+1, 8));
			ModifyRoad.DetaH = _wtof(m_grid.GetItemText(i+1, 9));
			ModifyRoad.UpDown = m_grid.GetItemText(i+1, 10);
			ModifyRoad.KKSY = m_grid.GetItemText(i+1, 11);
			ModifyRoad.RandType = m_grid.GetItemText(i+1, 12);
			ModifyRoad.LjWide = _wtof(m_grid.GetItemText(i+1, 13));
			ModifyRoad.LmWide = _wtof(m_grid.GetItemText(i+1, 14));
			ModifyRoad.LmThick = _wtof(m_grid.GetItemText(i+1, 15));
			ModifyRoad.LmType = m_grid.GetItemText(i+1, 16);
			ModifyRoad.LmArea = _wtof(m_grid.GetItemText(i+1, 17));
			ModifyRoad.FillVol = _wtof(m_grid.GetItemText(i+1, 18));
			ModifyRoad.CutVol = _wtof(m_grid.GetItemText(i+1, 19));
			ModifyRoad.MasonryVol = _wtof(m_grid.GetItemText(i+1, 20));
			ModifyRoad.LandAreaStr = m_grid.GetItemText(i+1, 21);
			ModifyRoad.Note = m_grid.GetItemText(i+1, 22);

			m_ModifyRoadArray.Add(ModifyRoad);
		}
	}
	if (m_ModifyRoadArray.GetSize()<1)
	{
		CResizableDialog::OnOK();
		m_xlmdb.ModifyRoadSum=0;
		if (m_xlmdb.pModifyRoad)
		{
			delete []m_xlmdb.pModifyRoad;
			m_xlmdb.pModifyRoad = NULL;
		}
		m_xlmdb.Write_XLDbs(m_MdbName,L"改移道路表",L"");
		return;
	}

	m_xlmdb.ModifyRoadSum = m_ModifyRoadArray.GetSize();
	if (m_xlmdb.pModifyRoad)
	{
		delete []m_xlmdb.pModifyRoad;
		m_xlmdb.pModifyRoad = NULL;
	}
	m_xlmdb.pModifyRoad = new BAS_DRAW_FUN::ModifyRoadTab[m_xlmdb.ModifyRoadSum];
	for (int i=0; i<m_xlmdb.ModifyRoadSum; i++)
	{
		m_xlmdb.pModifyRoad[i] = m_ModifyRoadArray[i];
	}
	m_xlmdb.Write_XLDbs(m_MdbName,L"改移道路表",L"");
	CResizableDialog::OnOK();
}

BOOL CEditAllRoads::OnInitDialog()
{
	CResizableDialog::OnInitDialog();
	AddAnchor(IDC_LIST_EDITALLROAD, TOP_LEFT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_INS,  BOTTOM_CENTER);
	AddAnchor(IDC_BUTTON_del,  BOTTOM_CENTER);
	AddAnchor(IDOK,  BOTTOM_CENTER);
	AddAnchor(IDCANCEL,  BOTTOM_CENTER);
	//AddAnchor(IDC_STATIC_LXLEFT, TOP_LEFT, BOTTOM_LEFT);
	// TODO:  在此添加额外的初始化
	Item.lfFont=font;
	Item.mask = GVIF_TEXT|GVIF_FORMAT;
	Item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE|
		DT_END_ELLIPSIS|DT_WORDBREAK;
	SetGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常：OCX 属性页应返回 FALSE
}

void CEditAllRoads::SetGrid()
{

	ACHAR  tmp[80];
	int i,j;

	txtbkclr=RGB(0XFF, 0XFF,0XE0);
	fixbkclr=RGB(155,188,0);


	m_xlmdb.ReadWorkdir();
	m_MdbName = Cworkdir+"\\DATA\\" + "改移道路.mdf";
	m_xlmdb.Read_XLDbs(m_MdbName,L"改移道路表");

	m_grid.SetRowCount(m_xlmdb.ModifyRoadSum+2);
	m_grid.SetFixedRowCount(1);
	m_grid.SetColumnCount(23);
	m_grid.SetFixedColumnCount(1);
	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);

	Item.row = 0;
	for(int k=0;k<m_grid.GetColumnCount();k++)
	{
		for( i=0;i<m_grid.GetRowCount();i++) 
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
		}		
		m_grid.SetItemState(i-1, k,GVIS_READONLY);
	}


	m_grid.SetItemText(0,0,L"序号");
	m_grid.SetItemText(0,1,L"改移道路名");
	m_grid.SetItemText(0,2,L"相交铁路名");
	m_grid.SetItemText(0,3,L"起始里程");
	m_grid.SetItemText(0,4,L"铁路道口(立交)中心里程");
	m_grid.SetItemText(0,5,L"终止里程");
	m_grid.SetItemText(0,6,L"公路道口(立交)中心里程");
	m_grid.SetItemText(0,7,L"改移道路长度(m)");
	m_grid.SetItemText(0,8,L"交叉角(°'\")");
	m_grid.SetItemText(0,9,L"高差(m)");
	m_grid.SetItemText(0,10,L"铁路在上或下");
	m_grid.SetItemText(0,11,L"孔跨结构");
	m_grid.SetItemText(0,12,L"等级及种类");
	m_grid.SetItemText(0,13,L"路基面宽(m)");
	m_grid.SetItemText(0,14,L"路面宽(m)");
	m_grid.SetItemText(0,15,L"路面厚度(cm)");
	m_grid.SetItemText(0,16,L"路面材料");
	m_grid.SetItemText(0,17,L"路面工程数量(m^2)");
	m_grid.SetItemText(0,18,L"填方(m^3)");
	m_grid.SetItemText(0,19,L"挖方(m^3)");
	m_grid.SetItemText(0,20,L"圬工(m^3)");
	m_grid.SetItemText(0,21,L"用地(亩)");
	m_grid.SetItemText(0,22,L"附注");

	int NLMK = m_xlmdb.ModifyRoadSum;

	i=0;
	if(NLMK != 0)
	{
		BAS_DRAW_FUN::ModifyRoadTab ModifyRoad;
		for(i=0; i<NLMK; i++)
		{
			ModifyRoad = m_xlmdb.pModifyRoad[i];

			_stprintf(tmp,L"%d", i+1);
			m_grid.SetItemText(i+1, 0, tmp);

			m_grid.SetItemText(i+1, 1, ModifyRoad.RoadName);

			m_grid.SetItemText(i+1, 2, ModifyRoad.RailName);

			m_grid.SetItemText(i+1, 3, ModifyRoad.SCkml);

			m_grid.SetItemText(i+1, 4, ModifyRoad.RailCkml);

			m_grid.SetItemText(i+1, 5, ModifyRoad.ECkml);

			ads_rtos(ModifyRoad.RoadDml, 2, 3, tmp);
			m_grid.SetItemText(i+1, 6, tmp);

			ads_rtos(ModifyRoad.RoadLen, 2, 3, tmp);
			m_grid.SetItemText(i+1, 7, tmp);

			_stprintf(tmp,L"%.4lf", ModifyRoad.InterAng);
			m_grid.SetItemText(i+1, 8, tmp);

			ads_rtos(ModifyRoad.DetaH, 2, 3, tmp);
			m_grid.SetItemText(i+1, 9, tmp);

			m_grid.SetItemText(i+1, 10, ModifyRoad.UpDown);

			m_grid.SetItemText(i+1, 11, ModifyRoad.KKSY);

			m_grid.SetItemText(i+1, 12, ModifyRoad.RandType);

			ads_rtos(ModifyRoad.LjWide, 2, 3, tmp);
			m_grid.SetItemText(i+1, 13, tmp);

			ads_rtos(ModifyRoad.LmWide, 2, 3, tmp);
			m_grid.SetItemText(i+1, 14, tmp);

			ads_rtos(ModifyRoad.LmThick, 2, 3, tmp);
			m_grid.SetItemText(i+1, 15, tmp);

			m_grid.SetItemText(i+1, 16, ModifyRoad.LmType);

			ads_rtos(ModifyRoad.LmArea, 2, 3, tmp);
			m_grid.SetItemText(i+1, 17, tmp);

			ads_rtos(ModifyRoad.FillVol, 2, 3, tmp);
			m_grid.SetItemText(i+1, 18, tmp);

			ads_rtos(ModifyRoad.CutVol, 2, 3, tmp);
			m_grid.SetItemText(i+1, 19, tmp);

			ads_rtos(ModifyRoad.MasonryVol, 2, 3, tmp);
			m_grid.SetItemText(i+1, 20, tmp);

			m_grid.SetItemText(i+1, 21, ModifyRoad.LandAreaStr);

			m_grid.SetItemText(i+1, 22, ModifyRoad.Note);

		}     
	}
	//设置表格的行高和列宽
	//m_grid.SetColumnWidth(0,40);
	//for( i=1;i<23;i++)
	//	m_grid.SetColumnWidth(i,85);
	for( i=1;i<m_grid.GetRowCount();i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.AutoSize();
	//m_grid.SetColumnWidth(4,150);
	//m_grid.SetColumnWidth(6,150);
	//m_grid.SetColumnWidth(7,120);
	//m_grid.SetColumnWidth(17,150);
	//m_grid.ResetScrollBars();
	m_grid.SetFocusCell(1,1);
}