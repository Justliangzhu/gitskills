// DLInintLwDlg.cpp: 实现文件
//

#include "StdAfx.h"
#include "DLInintLwDlg.h"
#include "afxdialogex.h"
#include "DLInintLwDlg.h"

extern LOGFONT font;
extern COLORREF txtbkclr;
extern COLORREF fixbkclr;
// DLInintLwDlg 对话框

IMPLEMENT_DYNAMIC(DLInintLwDlg, CDialog)

DLInintLwDlg::DLInintLwDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(DLInintLwDlg::IDD, pParent)
	, m_Size(80)
	, m_NumSelect(1)
	, m_NumSelectThrd(1)
	, m_TZLen(320)
	, m_JM(2)
	, m_XMBLen(160)
	, m_JMXMB(2)
	, m_Volum(0)
	, m_MaxPD(10.0)
	, m_OKPD(2.0)
{
	
	m_Volum = m_TZLen * m_TZLen * 5;
	singleCost = 0.1;
	CostMainLine = 0.0;
	MaxValue = 999999999;
	CFCoofe = exp(1.0);
	
	for (int i = 0; i < NUMOFARC; i++)
	{
		S[i] = false;
		Path[i] = -1;
		D[i] = MaxValue;
		path.push_back(-1);
		slgCost.push_back(MaxValue);
	}
	
	INITyhcs();
	ptriDTM.Read_Dtm();
}

DLInintLwDlg::~DLInintLwDlg()
{
}

void DLInintLwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INFGD, m_grid);
	DDX_Text(pDX, IDC_EDIT_GRSIZE, m_Size);
	DDX_Text(pDX, IDC_EDIT_XMBNUM, m_NumSelect);
	DDX_Text(pDX, IDC_EDIT_BHZNUM, m_NumSelectThrd);
	DDX_Text(pDX, IDC_EDIT_TZLen, m_TZLen);
	DDX_Control(pDX, IDC_LIST_INFGD2, m_Grid);
	DDX_Control(pDX, IDC_LIST_INFYL, m_YLgrid);
	DDX_Text(pDX, IDC_EDIT_JMSCLA, m_JM);
	DDX_Text(pDX, IDC_EDIT_XMBLen, m_XMBLen);
	DDX_Text(pDX, IDC_EDIT_JMXMB, m_JMXMB);
	DDX_Text(pDX, IDC_EDIT_Volum, m_Volum);
	DDX_Text(pDX, IDC_EDIT_MAXPD, m_MaxPD);
	DDX_Text(pDX, IDC_EDIT_PD, m_OKPD);
}


BEGIN_MESSAGE_MAP(DLInintLwDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PICKGD, &DLInintLwDlg::OnBnClickedButtonPickgd)
	ON_BN_CLICKED(IDC_BUTTON_DLAddRow, &DLInintLwDlg::OnBnClickedButtonDladdrow)
	ON_BN_CLICKED(IDC_BUTTON_DLDet, &DLInintLwDlg::OnBnClickedButtonDldet)
	ON_BN_CLICKED(IDC_BUTTON_CRTGRD, &DLInintLwDlg::OnBnClickedButtonCrtgrd)
	ON_BN_CLICKED(IDC_BUTTON_CRTMESH, &DLInintLwDlg::OnBnClickedButtonCrtmesh)
	ON_BN_CLICKED(IDC_BUTTON_Exist, &DLInintLwDlg::OnBnClickedButtonExist)
	ON_BN_CLICKED(IDC_BUTTON_CandiPool, &DLInintLwDlg::OnBnClickedButtonCandipool)
	ON_BN_CLICKED(IDC_BUTTON_DLAddRowBX, &DLInintLwDlg::OnBnClickedButtonDladdrowbx)
	ON_BN_CLICKED(IDC_BUTTON_DLDetBX, &DLInintLwDlg::OnBnClickedButtonDldetbx)
	ON_BN_CLICKED(IDC_BUTTON_PICKYL, &DLInintLwDlg::OnBnClickedButtonPickyl)
	ON_BN_CLICKED(IDC_BUTTON_DLAddRowYL, &DLInintLwDlg::OnBnClickedButtonDladdrowyl)
	ON_BN_CLICKED(IDC_BUTTON_DLDetYL, &DLInintLwDlg::OnBnClickedButtonDldetyl)
	ON_BN_CLICKED(IDC_BUTTON_REDRAWGD, &DLInintLwDlg::OnBnClickedButtonRedrawgd)
	ON_BN_CLICKED(IDC_BUTTON_REWRAWYL, &DLInintLwDlg::OnBnClickedButtonRewrawyl)
	ON_BN_CLICKED(IDC_BUTTON_DROPEARTH, &DLInintLwDlg::OnBnClickedButtonDropearth)
	ON_BN_CLICKED(IDC_BUTTON_WorkHouse, &DLInintLwDlg::OnBnClickedButtonWorkhouse)
	ON_BN_CLICKED(IDC_BUTTON_TUBA, &DLInintLwDlg::OnBnClickedButtonTuba)
END_MESSAGE_MAP()


// DLInintLwDlg 消息处理程序


void DLInintLwDlg::OnBnClickedButtonPickgd()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pAcadWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE); // this hides your dialog
	pAcadWnd->SetFocus();
	CString ZbInf;
	AcGePoint3d PickPt;
	int rc = RTNORM;
	rc = acedGetPoint(NULL, L"\n拾取一点：", asDblArray(PickPt));
	ZbInf.Format(L"%0.4lf", PickPt.x);
	m_grid.SetItemText(m_grid.GetFocusCell().row, 2, ZbInf);
	ZbInf.Format(L"%0.4lf", PickPt.y);
	double Ptxy[2] = { 0 };
	Ptxy[0] = PickPt.y;
	Ptxy[1] = PickPt.x;
	PickPt.z = ptriDTM.interpolate(Ptxy);
	m_grid.SetItemText(m_grid.GetFocusCell().row, 3, ZbInf);
	m_grid.SetFocusCell(m_grid.GetFocusCell().row + 1, 2);
	ob.makecircle(PickPt, 10, 1, L"工点位置");
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	pAcadWnd->EnableWindow(FALSE);
	EnableWindow(TRUE); // Enable our dialog*/
}


BOOL DLInintLwDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_grid.SetRowCount(2);
	m_grid.SetColumnCount(5);
	m_grid.SetFixedRowCount(1);
	m_grid.SetFixedColumnCount(1);

	Item.mask = GVIF_TEXT | GVIF_FORMAT;
	Item.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE |
		DT_END_ELLIPSIS | DT_WORDBREAK;
	Item.lfFont = font;

	m_grid.SetTextBkColor(txtbkclr);
	m_grid.SetFixedBkColor(fixbkclr);
	CString CountNum;
	for (int k = 0; k < m_grid.GetColumnCount(); k++)
	{
		int i;
		for (i = 0; i < m_grid.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_grid.SetItem(&Item);
			CountNum.Format(L"%d", i);
			m_grid.SetItemText(i, 0, CountNum);
		}
		/*m_grid.SetItemState(i - 1, k, GVIS_READONLY);*/

	}

	m_grid.SetItemText(0, 0, L"序 号");
	m_grid.SetItemText(0, 1, L"属 性");
	m_grid.SetItemText(1, 1, L"施工面");
	m_grid.SetItemText(0, 2, L"X坐标");
	m_grid.SetItemText(0, 3, L"Y坐标");
	m_grid.SetItemText(0, 4, L"备 注");

	m_grid.SetColumnWidth(0, 50);
	m_grid.SetColumnWidth(1, 60);
	m_grid.SetColumnWidth(2, 120);
	m_grid.SetColumnWidth(3, 120);
	m_grid.SetColumnWidth(4, 60);

	for (int i = 0; i < m_grid.GetRowCount(); i++)
		m_grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_grid.SetFocusCell(1, 2);


	m_Grid.SetRowCount(2);
	m_Grid.SetColumnCount(5);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(1);


	m_Grid.SetTextBkColor(txtbkclr);
	m_Grid.SetFixedBkColor(fixbkclr);
	for (int k = 0; k < m_Grid.GetColumnCount(); k++)
	{
		int i;
		for (i = 0; i < m_Grid.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_Grid.SetItem(&Item);
			CountNum.Format(L"%d", i);
			m_Grid.SetItemText(i, 0, CountNum);
		}
		/*m_grid.SetItemState(i - 1, k, GVIS_READONLY);*/

	}

	m_Grid.SetItemText(0, 0, L"序 号");
	m_Grid.SetItemText(0, 1, L"属 性");
	m_Grid.SetItemText(1, 1, L"项目部");
	m_Grid.SetItemText(0, 2, L"X坐标");
	m_Grid.SetItemText(0, 3, L"Y坐标");
	m_Grid.SetItemText(0, 4, L"备 注");

	m_Grid.SetColumnWidth(0, 50);
	m_Grid.SetColumnWidth(1, 60);
	m_Grid.SetColumnWidth(2, 120);
	m_Grid.SetColumnWidth(3, 120);
	m_Grid.SetColumnWidth(4, 60);

	for (int i = 0; i < m_Grid.GetRowCount(); i++)
		m_Grid.SetRowHeight(i, DEFAULTCOLHEI);
	m_Grid.SetFocusCell(1, 2);


	m_YLgrid.SetRowCount(2);
	m_YLgrid.SetColumnCount(5);
	m_YLgrid.SetFixedRowCount(1);
	m_YLgrid.SetFixedColumnCount(1);


	m_YLgrid.SetTextBkColor(txtbkclr);
	m_YLgrid.SetFixedBkColor(fixbkclr);
	for (int k = 0; k < m_YLgrid.GetColumnCount(); k++)
	{
		int i;
		for (i = 0; i < m_YLgrid.GetRowCount(); i++)
		{
			Item.row = i;
			Item.col = k;
			m_YLgrid.SetItem(&Item);
			CountNum.Format(L"%d", i);
			m_YLgrid.SetItemText(i, 0, CountNum);
		}
		/*m_grid.SetItemState(i - 1, k, GVIS_READONLY);*/

	}

	m_YLgrid.SetItemText(0, 0, L"序 号");
	m_YLgrid.SetItemText(0, 1, L"属 性");
	m_YLgrid.SetItemText(1, 1, L"水源");
	m_YLgrid.SetItemText(0, 2, L"X坐标");
	m_YLgrid.SetItemText(0, 3, L"Y坐标");
	m_YLgrid.SetItemText(0, 4, L"备 注");

	m_YLgrid.SetColumnWidth(0, 50);
	m_YLgrid.SetColumnWidth(1, 60);
	m_YLgrid.SetColumnWidth(2, 120);
	m_YLgrid.SetColumnWidth(3, 120);
	m_YLgrid.SetColumnWidth(4, 60);

	for (int i = 0; i < m_YLgrid.GetRowCount(); i++)
		m_YLgrid.SetRowHeight(i, DEFAULTCOLHEI);
	m_YLgrid.SetFocusCell(1, 2);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void DLInintLwDlg::OnBnClickedButtonDladdrow()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if (nRow >= 0)
	{
		CString str;
		Item.row = nRow;
		m_grid.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_grid.GetColumnCount(); i++)
		{
			Item.col = i;
			m_grid.SetItem(&Item);
		}


		for (i = nRow; i < m_grid.GetRowCount(); i++)
		{
			str.Format(L"%d", i);
			m_grid.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_grid.GetRowCount(); i++)
			m_grid.SetRowHeight(i, DEFAULTCOLHEI);

		m_grid.SetItemText(nRow, 1, m_grid.GetItemText(nRow + 1, 1));

		m_grid.SetFocusCell(nRow, 1);

		m_grid.Invalidate();
	}
}


void DLInintLwDlg::OnBnClickedButtonDldet()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_grid.GetFocusCell().row;
	if (nRow == m_grid.GetRowCount() - 1) return;
	if (nRow >= 0)
	{
		m_grid.DeleteRow(nRow);
		for (int i = nRow; i < m_grid.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_grid.SetItemText(i, 0, str);
		}
		m_grid.SetFocusCell(nRow, 1);
		m_grid.Invalidate();
	}
}
double xmult(double x1, double y1, double x2, double y2, double x0, double y0) {
	return (x1 - x0)*(y2 - y0) - (x2 - x0)*(y1 - y0);
}
double area_triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
	return fabs(xmult(x1, y1, x2, y2, x3, y3)) / 2;
}
double dis_ptoline(double x1, double y1, double x2, double y2, double ex, double ey, double& px, double& py)
{ //第一个点，第二个点，目标点 ，p为垂足点， 返回距离
	double k, b, dis, tem1, tem2, t1, t2, yd = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
	t2 = sqrt((x2 - ex)*(x2 - ex) + (y2 - ey)*(y2 - ey));
	t1 = sqrt((x1 - ex)*(x1 - ex) + (y1 - ey)*(y1 - ey));
	dis = area_triangle(x1, y1, x2, y2, ex, ey) * 2 / yd;
	tem1 = sqrt(t1*t1 - dis * dis);
	tem2 = sqrt(t2*t2 - dis * dis);

	if (tem1 > yd || tem2 > yd) {
		if (t1 > t2) { px = x2; py = y2; return t2; }
		else { px = x1; py = y1; return t1; }
	}
	px = x1 + (x2 - x1)*tem1 / yd;
	py = y1 + (y2 - y1)*tem1 / yd;
	return dis;
}
bool DLInintLwDlg::IsOnExistRoad(AcGePoint2d CanPt, vector<AcGePoint2dArray> Verts, AcGePoint2d &ResPt)
{
	UpdateData(TRUE);
	AcGePoint2d tempP2t;
	double MinDist = 0.51*m_Size;
	bool hasOn = false;
	double px = 0, py = 0;
	for (int i = 0; i < Verts.size(); i++)
	{
		for (int j = 0; j < Verts.at(i).length() - 1; j++)
		{
			double dis = dis_ptoline(Verts.at(i).at(j + 1).x, Verts.at(i).at(j + 1).y, Verts.at(i).at(j).x, Verts.at(i).at(j).y, CanPt.x, CanPt.y, px, py);
			/*tempP2t = ob.GetOrthPt(CanPt, Verts.at(i).at(j), Verts.at(i).at(j+1));*/
			tempP2t.x = px;
			tempP2t.y = py;
			if (/*CanPt.distanceTo(tempP2t)*/dis < MinDist)
			{
				hasOn = true;
				ResPt = tempP2t;
			}
		}

	}
	return hasOn;
}

void DLInintLwDlg::OnBnClickedButtonCrtgrd()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//获得列表数据
	GDPts.removeAll();
	CString strInf, strInfX, strInfY;
	AcGePoint3d tempPt, UpLeftPt, DownRightPt, forConnect;
	double Ptxy[2] = { 0 };
	strTypeGDs.clear();
	GetLocalTime(&lpSystemTime);
	CurTimeNum = lpSystemTime.wYear * 1e8 + lpSystemTime.wMonth * 1e6 + lpSystemTime.wDay * 1e4 + lpSystemTime.wHour * 100 + lpSystemTime.wMinute + lpSystemTime.wSecond / 100.0;
	for (int i = 1; i < m_grid.GetRowCount() + 1; i++)
	{
		strInf = m_grid.GetItemText(i, 1);
		if (strInf != L"")
		{

			strTypeGDs.push_back(strInf);
			strInfX = m_grid.GetItemText(i, 2);
			strInfY = m_grid.GetItemText(i, 3);
			tempPt.set(_wtof(strInfX), _wtof(strInfY), 0);
			Ptxy[0] = _wtof(strInfY);
			Ptxy[1] = _wtof(strInfX);
			tempPt.z = ptriDTM.interpolate(Ptxy);
			GDPts.append(tempPt);
		}
	}
	for (int j = 0; j < GDPts.length(); j++)
	{
		Paths.push_back(path);
		DCost.push_back(slgCost);
	}
	//框选建网范围
	CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pAcadWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE); // this hides your dialog
	pAcadWnd->SetFocus();

	acedGetPoint(NULL, L"\n框选范围：", asDblArray(UpLeftPt));
	acedGetCorner(asDblArray(UpLeftPt), L"", asDblArray(DownRightPt));
	if (UpLeftPt.x< DownRightPt.x &&UpLeftPt.y > DownRightPt.y)
		;
	else
	{
		tempPt = UpLeftPt;
		UpLeftPt = DownRightPt;
		DownRightPt = tempPt;
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	pAcadWnd->EnableWindow(FALSE);
	EnableWindow(TRUE); // Enable our dialog*/

	//计算范围是否越界
	for (int i = 0; i < GDPts.length(); i++)
	{
		if (GDPts.at(i).x<UpLeftPt.x || GDPts.at(i).y > UpLeftPt.y || GDPts.at(i).x > DownRightPt.x || GDPts.at(i).y < DownRightPt.y)
		{
			AfxMessageBox(L"范围框选区域未包含所有工点，请重新设置！！！");
			return;
		}
	}

	//计算网格点
	int ID = 0;
	NodeDatas.clear();
	int n = 30, m = 1;
	double a = 0;
	int areCol = 1 + (DownRightPt.x - UpLeftPt.x) / m_Size;
	int areRow = 1 + (UpLeftPt.y - DownRightPt.y) / m_Size;
	Row = areRow;
	Col = areCol;
	int Sum = areCol * areRow;
	ACHAR pszLabel[80];
	_tcscpy(pszLabel, L"构建网格节点");
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);
	AcGePoint2d temp2dPt, OnRoad;
	//打开既有道路多段线
	vector<AcDbObject *>pPlines;
	vector<AcDbObject *>forbidLines;
	pPlines.resize(ExistRoadIds.length());
	forbidLines.resize(ForbidIds.length());
	for (int i = 0; i < ExistRoadIds.length(); i++)
	{
		acdbOpenObject(pPlines.at(i), ExistRoadIds.at(i), AcDb::kForRead);
	}
	for (int i = 0; i < ForbidIds.length(); i++)
	{
		acdbOpenObject(forbidLines.at(i), ForbidIds.at(i), AcDb::kForRead);
	}


	AcGePoint2d tempP2t;
	Verts.clear();
	AcGePoint2dArray vects;
	AcDbPolyline *pPline;
	AcGePoint3d location;
	for (int i = 0; i < pPlines.size(); i++)
	{
		pPline = AcDbPolyline::cast(pPlines.at(i));
		int numverts;
		numverts = pPline->numVerts();
		vects.removeAll();
		for (int k = 0; k < numverts; k++)
		{  //寻找的某顶点
			pPline->getPointAt(k, location);
			tempP2t.set(location.x, location.y);
			vects.append(tempP2t);
		}
		Verts.push_back(vects);
	}
	for (int i = 0; i < pPlines.size(); i++)
	{
		pPlines.at(i)->close();
	}
	//竖向加
	RoadNodeDatas.clear();
#pragma omp parallel for num_threads(8) schedule(dynamic)
	for (int i = 1; i < areCol; i++)
	{
		int Pos = (int)(i * 100 / areCol);
		acedSetStatusBarProgressMeterPos(Pos);
		for (int j = 1; j < areRow; j++)
		{
			tempData.isOnExistRoad = false;
			//判断是否定义为路网上的点，做就近转换
			tempPt.x = UpLeftPt.x + i * m_Size;
			tempPt.y = UpLeftPt.y - j * m_Size;
			temp2dPt.set(tempPt.x, tempPt.y);
			tempData.isOnExistRoad = IsOnExistRoad(temp2dPt, Verts, OnRoad);
			//采集z坐标
			if (tempData.isOnExistRoad)
			{
				forConnect.set(OnRoad.x, OnRoad.y, 0);
				Ptxy[0] = forConnect.y;
				Ptxy[1] = forConnect.x;
				forConnect.z = ptriDTM.interpolate(Ptxy);
				tempData.PtNode = forConnect;
				tempData.Row = j;
				tempData.Col = i;
				tempData.IdNum = ID;
				RoadNodeDatas.push_back(tempData);
			}
			else
			{
				forConnect.set(temp2dPt.x, temp2dPt.y, 0);
				Ptxy[0] = forConnect.y;
				Ptxy[1] = forConnect.x;
				forConnect.z = ptriDTM.interpolate(Ptxy);
			}
			tempData.inForbid = false;
			for (int g = 0; g < forbidLines.size(); g++)
			{
				AcGePoint3dArray pts;
				pPline = AcDbPolyline::cast(forbidLines.at(g));
				AcDbPolyline *interPline = new AcDbPolyline(2);
				interPline->addVertexAt(0, temp2dPt);
				temp2dPt.x += 100000;
				interPline->addVertexAt(1, temp2dPt);
				interPline->intersectWith(pPline, AcDb::kOnBothOperands, pts);
				if (pts.length() < 1)
					continue;
				AcDbPolyline *interPlineo = new AcDbPolyline(2);
				temp2dPt.x -= 100000;
				interPlineo->addVertexAt(0, temp2dPt);
				temp2dPt.x -= 100000;
				interPlineo->addVertexAt(1, temp2dPt);
				pts.removeAll();
				interPlineo->intersectWith(pPline, AcDb::kOnBothOperands, pts);
				if (pts.length() < 1)
					continue;
				else {
					tempData.inForbid = true;
					break;
				}
			}
			tempData.PtNode = forConnect;
			tempData.Row = j;
			tempData.Col = i;
			tempData.IdNum = ID;
			ID++;
			NodeDatas.push_back(tempData);
		}
	}
	for (int i = 0; i < forbidLines.size(); i++)
	{
		forbidLines.at(i)->close();
	}


	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	//替换节点
	double MinDist = MaxValue;
	int Index = -1, TempId = 0;
	FearIds.clear();
	GDNames.clear();
	for (int i = 0; i < GDPts.length(); i++)
	{
		MinDist = MaxValue;
		Index = -1;
		for (int k = 0; k < NodeDatas.size(); k++)
		{
			if (GDPts.at(i).distanceTo(NodeDatas.at(k).PtNode) < MinDist)
			{
				MinDist = GDPts.at(i).distanceTo(NodeDatas.at(k).PtNode);
				Index = k;
				TempId = NodeDatas.at(k).IdNum;
			}
		}
		NodeDatas.at(Index).PtNode = GDPts.at(i);
		FearIds.push_back(TempId);//工点
		GDNames.push_back(strTypeGDs.at(i));
	}
	//画出节点
	/*for (int i = 0; i < RoadNodeDatas.size(); i++)
	{
		ob.makecircle(RoadNodeDatas.at(i).PtNode, 3, 1);
	}*/
	Sleep(500);
	OnBnClickedButtonCrtmesh();
}


void DLInintLwDlg::ShortestPath_DIJ(AMGraph &G, int v0, int& vend, bool isOnEndRoad)
{//使用迪杰斯特拉算法求有向网G中的V0 定点到其余顶点的最短路径
	for (int i = 0; i < NUMOFARC; i++)
	{
		S[i] = false;
		Path[i] = -1;
		D[i] = MaxValue;
	}
	int n = G.vexnum;//顶点数
	double MAX = MaxValue;
	for (int v = 0; v < n; v++)//n个顶点依次初始化
	{
		S[v] = false;//S初始化为空集
		D[v] = G.arcs[v0][v];//将v0到各个终点的最短路径长度初始化为边上的权值
		if (D[v] < MAX)
			Path[v] = v0;//如果v0和v之间有边，则将v的前驱初始化为v0
		else
			Path[v] = -1;//如果v0和v之间无边，则将v的前驱初始化为-1
	}
	S[v0] = true; //将v0加入s
	D[v0] = 0;//源点到源点的权值为0
	//---------初始化结束，开始主循环，每次求得v0到某个顶点的最短路径，将v加到S数组
	ACHAR pszLabel[80];
	_tcscpy(pszLabel, L"路径搜索...");
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);
	int Pos;
	for (int i = 1; i < n; i++)//依次对其余n-1个顶点进行计算
	{
		double min = MAX;
		int v = v0;
		Pos = i * 100 / n;
		acedSetStatusBarProgressMeterPos(Pos);
		for (int w = 0; w < n; w++)
		{
			if (!S[w] && D[w] < min)
			{//选择一条当前最短路径，终点为v
				v = w;
				min = D[w];
			}
		}
		S[v] = true;//将v加到s集合中
		for (int w = 0; w < n; w++)
		{//更新从v0出发到集合V-S上所有顶点的最短路径长度
			if ((!S[w] && G.arcs[v][w]<MaxValue&&((D[v] + G.arcs[v][w] < D[w]))))
			{
				D[w] = D[v] + G.arcs[v][w];//更新D[w]
				Path[w] = v;//更改w的前驱为v
			}
		}

		//for (int w = 0; w < n; w++)
		//{
		//	if (!S[w] && D[w] < min)
		//	{//选择一条当前最短路径，终点为v
		//		v = w;
		//		min = D[w];
		//	}
		//	S[v] = true;//将v加到s集合中
		//	for (int w = 0; w < n; w++)
		//	{//更新从v0出发到集合V-S上所有顶点的最短路径长度
		//		if ((!S[w] && ((D[v] + G.arcs[v][w] < D[w]))))
		//		{
		//			D[w] = D[v] + G.arcs[v][w];//更新D[w]
		//			Path[w] = v;//更改w的前驱为v
		//		}
		//	}
		//}
	}
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
}
void DLInintLwDlg::INITyhcs()
{
	DMXCS.Cml = 0.00;//里程
	DMXCS.EarthH = 0.00;//地面标高
	DMXCS.DetaH = 0.00;//填挖高
	DMXCS.DesH = 0.00;//设计标高
	DMXCS.Area = 0.00;//填挖面积
	DMXCS.row = 0,
		DMXCS.col = 0;//行列号
	DMXCS.MaxbridgeH = 50.0;//最大桥高
	DMXCS.maxTunlen = 2 * m_Size;//最大隧道长度
	DMXCS.MinBriH = 10.0;//设桥最小填高
	DMXCS.DividBriH = 15.0;//线桥分界填高
	DMXCS.MinBriL = 15.0;//最小桥长
	DMXCS.MinTunH = 15.0;//设隧最小挖深
	DMXCS.DividTunH = 20.0;//线隧分界挖深
	DMXCS.MinTunL = m_Size;//最小隧长
	DMXCS.CutUnitCost = 0.008;//挖方每方单价
	DMXCS.CutBPXS = 1.5;//挖方边坡系数
	DMXCS.CutBPHeight = 8.0;
	DMXCS.CutPTWide = 1.0;
	DMXCS.FillUnitCost = 0.007;//填方每方单价
	DMXCS.FillBPXS = 1.5;//填方边坡系数
	DMXCS.FillBPHeight = 8.0;
	DMXCS.FillPTWide = 1.0;
	DMXCS.DQUnitCost = 7.0;//挡墙每方单价
	DMXCS.DQBPXS = 7.0;//填方边坡系数
	DMXCS.BriUnitCost = 3.0;//桥梁每方单价
	DMXCS.CZBriHeight = 20.0;//桥梁参照桥高
	DMXCS.QTCost = 5.0;//桥头费用
	DMXCS.TunlUnitCost = 8.0;//隧道每延米单价
	DMXCS.CZTunlLen = m_Size;//参照隧长
	DMXCS.DMCost = 12.0;//洞门费用
	DMXCS.ZZCost = 5.0;//征地费用
	DMXCS.YDJKLen = 1.0;//用地加宽长度

	DMXCS.ZYLDCost = 1.0;//征林地费用
	DMXCS.LDJKLen = 1.0;//用地加宽长度
	DMXCS.ZYCDCost = 1.0;//征草地费用
	DMXCS.CDJKLen = 1.0;//用地加宽长度
	DMXCS.ZYGDCost = 6.0;//征耕地费用
	DMXCS.GDJKLen = 1.0;//用地加宽长度
	DMXCS.ZYZJDCost = 20.0;//征宅地费用
	DMXCS.ZJDJKLen = 20 == 1.0;//用地加宽长度
	DMXCS.XXCost = 0.5; //线形费用
	DMXCS.g_MaxLen_HighBri = 400.0;
}

AcGePoint2dArray DLInintLwDlg::AdjustPoints(AcGePoint2dArray ForPlinePts)
{
	AcGePoint2d prePt,curPt, nexPt;
	AcGePoint2dArray pts;
	pts.append(ForPlinePts.at(0));
	pts.append(ForPlinePts.at(1));
	AcGeVector2d v1, v2;
	for (int i = 2; i < ForPlinePts.length() - 1; i++)
	{
		prePt = ForPlinePts.at(i - 1);
		curPt = ForPlinePts.at(i);
		nexPt = ForPlinePts.at(i + 1);
		v1 = curPt - prePt;
		v1.normalize();
		v2 = nexPt - curPt;
		v2.normalize();

		double res =v1.dotProduct(v2);
		if(res <0.99)
			pts.append(ForPlinePts.at(i));
	} 
	pts.append(ForPlinePts.at(ForPlinePts.length() - 1));
	return pts;
}

void DLInintLwDlg::OnBnClickedButtonCrtmesh()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (NodeDatas.size() < 1)
	{
		acutPrintf(L"请先生成网格节点");
		return;
	}
	//初始化搜索区域
	ACHAR pszLabel[80];
	
	AcGePoint2d P2t1, P2t2;
	AcGePoint3d P3t1, P3t2;
	double Cost = MaxValue;
	CString strNodeName;
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		strNodeName.Format(L"%d", i);
		amg.vexs.push_back(strNodeName);
	}
	vector<double>tempValues;
	vector<bool>tempOnRoad;
	//初始化边
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		tempValues.push_back(MaxValue);
		tempOnRoad.push_back(false);
	}
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		amg.arcs.push_back(tempValues);
		//amg.isOnRoad.push_back(tempOnRoad);
	}
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		amg.LocationPts.append(NodeDatas.at(i).PtNode);
	}
	int iCount = 0;
	_tcscpy(pszLabel, L"路网投资目标计算...");
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);
	#pragma omp parallel for num_threads(8)		
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		iCount = 0;
		for (int k = i; k < NodeDatas.size(); k++)
		{
			//目标函数的构建

			amg.arcs[i][k] = amg.arcs[k][i] = MaxValue;
			if (NodeDatas.at(i).inForbid || NodeDatas.at(k).inForbid) {
				continue;
			}
			int num = k - i;
			if (num != 1 && num != Row && num != Row -1 && num != Row - 2)
				continue;
			/*if (num != 1 && num != Row - 1)
				continue;*/
			acedSetStatusBarProgressMeterPos((int)((i*NodeDatas.size() + k) * 100) / (NodeDatas.size()*NodeDatas.size()));
			P2t1.set(NodeDatas.at(i).PtNode.x, NodeDatas.at(i).PtNode.y);
			P2t2.set(NodeDatas.at(k).PtNode.x, NodeDatas.at(k).PtNode.y);
			double dis = P2t2.distanceTo(P2t1);

			P3t1 = NodeDatas.at(i).PtNode;
			P3t2 = NodeDatas.at(k).PtNode;
			if (NodeDatas.at(i).isOnExistRoad&&NodeDatas.at(k).isOnExistRoad&&dis < 2 * m_Size)
			{
				amg.arcs[i][k] = 0.0;
				amg.arcs[k][i] = 0.0;
				continue;
			}
			double Dis = P3t2.distanceTo(P3t1);
			if (dis > 2 * m_Size)
				continue;
			Cal_Cost_a2b(NodeDatas.at(i).PtNode, NodeDatas.at(k).PtNode, DMXCS, Cost);
			amg.arcs[i][k] = Cost;
			amg.arcs[k][i] = amg.arcs[i][k];
		}
	}
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();
	Recamg = amg;

	amg.vexnum = NodeDatas.size();
	amg.arcnum = 0;
	for (int i = 0; i < amg.vexnum; i++)
	{
		for (int k = 0; k < amg.vexnum; k++)
		{
			if (amg.arcs[i][k] < MaxValue)
			{
				amg.arcnum++;
			}
		}
	}
	amg.arcnum /= 2;
	//组建施工干线
	ComPIds.clear();
	double minDis = MaxValue, tempdis = 0;
	int tempId = 0;

	for (int i = 0; i < FearIds.size(); i++)
	{
		minDis = MaxValue;
		tempId = RoadNodeDatas.at(0).IdNum;
		for (int j = 0; j < RoadNodeDatas.size(); j++)
		{
			tempdis = NodeDatas.at(FearIds.at(i)).PtNode.distanceTo(RoadNodeDatas.at(j).PtNode);
			if (tempdis < minDis)
			{
				minDis = tempdis;
				tempId = RoadNodeDatas.at(j).IdNum;
			}
		}
		vector<int>tempIds;
		ob.makecircle(NodeDatas.at(tempId).PtNode, 50, 1);
		tempIds.push_back(FearIds.at(i));
		tempIds.push_back(tempId);
		ComPIds.push_back(tempIds);
	}
	//路径搜索
	int RoadNum = 0;
	int SetOffX = 50000, SetOffY = 0;
	for (int pp = 0; pp < ComPIds.size()/*, OnRoadB*/; pp++)
	{
		RoadNum++;
		if (ComPIds.size() < 1 || pp > ComPIds.size() - 1)
			break;
		CString strCount;
		strCount.Format(L"\n正在搜索第%d条道路...", RoadNum);
		acutPrintf(strCount);

		ShortestPath_DIJ(amg, ComPIds[pp][0], ComPIds[pp][1]);
		//Paths.push_back(Path);

		int temp = ComPIds[pp][1];
		int temp1, temp2;
		int flag[10000], m = 0;
		CString str;
		str.Format(L"\n从%d到%d最短路径投资为：", ComPIds[pp][0], ComPIds[pp][1]);
		acutPrintf(str);

		while (temp != -1)
		{
			flag[m++] = temp;
			if (temp == ComPIds[pp][0])
				break;
			temp1 = temp;
			temp2 = Path[temp1];
			Path[temp1] = -1;
			temp = temp2;
		}
		/*for (decltype(RoadNodeDatas.size()) h = 0; h < RoadNodeDatas.size(); h++)
		{
			Path[RoadNodeDatas.at(h).IdNum] = -1;
		}*/
		for (auto item: RoadNodeDatas)
		{
			Path[item.IdNum] = -1;
		}
		for (int jj = 0; jj < pp; jj++)
		{
			for (int gg = 0; gg < NUMOFARC; gg++)
			{
				if(Paths[jj][gg]==-1)
					Path[gg]= -1;
			}
		}
		for (int gg = 0; gg < NUMOFARC; gg++)
		{
			Paths[pp][gg] = Path[gg];
		}
		PassRoad.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			PassRoad.push_back(_wtoi(amg.vexs[flag[i]]));
		}


		Indexs.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			Indexs.push_back(_wtoi(amg.vexs[flag[i]]));
		}
		//将路径节点连接代价更新为拓展代价
		if (Indexs.size() > 1)
		{
			for (decltype(Indexs.size()) i = 0; i < Indexs.size() - 1; i++)
			{
				NodeDatas.at(Indexs.at(i)).isOnExistRoad = true;
				amg.arcs[Indexs.at(i)][Indexs.at(i + 1)] = 0.0;
			}
		}
		CostMainLine += D[ComPIds[pp][1]];
		str.Format(L"%0.3lf万元", D[ComPIds[pp][1]]);
		acutPrintf(str);
		for (int t = 0; t < NUMOFARC; t++)
		{
			DCost.at(pp).at(t) = D[t];
		}

		ForPlinePts.removeAll();
		temppts.removeAll();
		for (int i = 0; i < Indexs.size(); i++)
		{
			temppts.append(AcGePoint2d(amg.LocationPts.at(Indexs.at(i)).x, amg.LocationPts.at(Indexs.at(i)).y));
		}
		ForPlinePts=AdjustPoints(temppts);
		AcGePoint3dArray forhatchCir;

		for (int j = 0; j < m_NumSelect; j++)
		{
			for (int i = 0; i < m_NumSelectThrd; i++)
			{
				/*forhatchCir.removeAll();
				ob.Create3dPolyLine(ForPlinePts, 1, SetOffX + i * 50000, SetOffY + j * 50000, 1);
				forhatchCir.append(AcGePoint3d(ForPlinePts.at(0).x + SetOffX + i * 50000, ForPlinePts.at(0).y + SetOffY + j * 50000, ForPlinePts.at(0).z));
				ob.CreateHatch(forhatchCir, L"SOLID", true, 1, true);
				forhatchCir.removeAll();
				forhatchCir.append(AcGePoint3d(ForPlinePts.at(ForPlinePts.length() - 1).x + SetOffX + i * 50000, ForPlinePts.at(ForPlinePts.length() - 1).y + SetOffY + j * 50000, ForPlinePts.at(ForPlinePts.length() - 1).z));
				ob.CreateHatch(forhatchCir, L"SOLID", true, 0, true);*/
				forhatchCir.removeAll();
				//ob.Create3dPolyLine(ForPlinePts, 1, 0, 0, 1);
				//ob.make2dline(ForPlinePts,1,2.0,2.0);
				ob.makepolyline(ForPlinePts, 1, 2.0);
				//ob.makepolyline(ForPlinePts,false,1,2.0,L"贯通便道",);
				forhatchCir.append(AcGePoint3d(ForPlinePts.at(0).x, ForPlinePts.at(0).y,0));
				ob.CreateHatch(forhatchCir, L"SOLID", true, 1, true);
				forhatchCir.removeAll();
				forhatchCir.append(AcGePoint3d(ForPlinePts.at(ForPlinePts.length() - 1).x, ForPlinePts.at(ForPlinePts.length() - 1).y,0));
				ob.CreateHatch(forhatchCir, L"SOLID", true, 0, true);
			}
		}

	}
	Sleep(500);
	OnBnClickedButtonCandipool();
	return;

}
bool DLInintLwDlg::Cal_Cost_a2b(AcGePoint3d PT1, AcGePoint3d PT2, DEMData DMXCS, double & Cost)
{
	
	/*double dist2d = sqrt(pow(PT1.x - PT2.x, 2) + pow(PT1.y - PT2.y, 2));

	double deitaH = fabs(PT1.z - PT2.z);
	double pd = deitaH / dist2d;
	Cost = singleCost * (1 + pd);
	if (pd > (m_MaxPD / 100.0))
		Cost = MaxValue;*/
	
	/*double dist3d = PT1.distanceTo(PT2);
	if (dist3d > dist2d*(1 + (m_MaxPD/100.0)))
		Cost = MaxValue;
	else if (dist3d > dist2d*(1 + (m_OKPD/100.0)))
		Cost = pow(CFCoofe, ((dist3d / dist2d) - 1) / (m_OKPD / 100.0))*dist2d*singleCost;
	else
		Cost = dist3d *singleCost;*/
	//return true;


	//double dist2d = sqrt(pow(PT1.x - PT2.x, 2) + pow(PT1.y - PT2.y, 2));

	//double deitaH = fabs(PT1.z - PT2.z);
	//double pd = deitaH / dist2d;
	///*if (pd>(m_MaxPD / 100.0))
	//	Cost = MaxValue;
	//else */if (pd>(m_OKPD / 100.0))
	//	Cost = pow(CFCoofe, pd / (m_OKPD / 100.0))*dist2d*singleCost;
	//else
	//	Cost = dist2d * singleCost*(1+pd);
	///*double dist3d = PT1.distanceTo(PT2);
	//if (dist3d > dist2d*(1 + (m_MaxPD/100.0)))
	//	Cost = MaxValue;
	//else if (dist3d > dist2d*(1 + (m_OKPD/100.0)))
	//	Cost = pow(CFCoofe, ((dist3d / dist2d) - 1) / (m_OKPD / 100.0))*dist2d*singleCost;
	//else
	//	Cost = dist3d *singleCost;*/
	//return true;
	/*double dist2d = sqrt(pow(PT1.x - PT2.x, 2) + pow(PT1.y - PT2.y, 2));
	double X_dir = (PT2.x - PT1.x) / dist2d;
	double Y_dir = (PT2.y - PT1.y) / dist2d;
	double signlen = m_Size / 10;
	double tempLC = 0.0;
	AcGePoint3d Pt1,Pt2,PT;
	PT = PT1;
	double Ptxy[2] = { 0 }, PtxyLast[2] = { 0 };
	Ptxy[0] = PT.y;
	Ptxy[1] = PT.x;
	Pt1.z = ptriDTM.interpolate(Ptxy);
	double tempHight = 0.0;
	Cost = 0.0;
	double SingleCost = 0.5;
	while (tempLC < dist2d)
	{
		PT.x = PT.x + X_dir;
		PT.y = PT.y + Y_dir;
		PtxyLast[0] = PT.y;
		PtxyLast[1] = PT.x;
		Pt2.z = ptriDTM.interpolate(PtxyLast);
		tempHight = fabs(Pt2.z - Pt1.z);
		Cost += (1+ tempHight/(signlen*0.3))*signlen*SingleCost;
		Pt1.z = Pt2.z;
		tempLC += signlen;
	}
	return true;*/
	/*double dist3d = PT1.distanceTo(PT2);
	if (dist3d > dist2d*1.3)
		Cost = MaxValue;
	else
		Cost = dist3d;
	return true;*/
	vector<DEMData> DMXCSArray;
	DMXCSArray.clear();
	int NCRS = 0, NBRI = 0, NTUN = 0;
	double N_Start, E_Start, N_End, E_End;
	double Fwj;
	vector<BRITAB_DN> BridgeArray;
	vector<TUNTAB_DN> TunArray;
	BridgeArray.clear();
	TunArray.clear();
	//double Dist = Cal_FWJ_Dist(N_Start, E_Start, N_End, E_End, Fwj);
	//double Slope = (EndH - StartH) / Dist;
	/*Cost = 0.0;*/

	//CostData cd;
	if (FormDMXTW(PT1, PT2, NCRS, DMXCS, DMXCSArray) == false)
		;
	//return false;
////自动设桥
//	if (false == AutoSetBRI(DMXCS, DMXCSArray, BridgeArray))
//		;
//	//return false;
////自动设隧
//	if (false == AutoSetTUN(DMXCS, DMXCSArray, TunArray))
//		;
	//return false;
//统计造价
	Cost = CalCost(DMXCSArray, BridgeArray, TunArray, DMXCS, Cost);
	return true;
}
bool DLInintLwDlg::AutoSetTUN(DEMData DMXCS, vector<DEMData>& DMXCSArray, vector<TUNTAB_DN>& TunArray)
{
	TUNTAB_DN Tunstruct;
	//计算填挖高
	double sml, eml, len, Btwh;
	double MinTunLen = 0.0;//最小隧道长度
	double MaxTunLen = 1000.0;
	char GH[8];
	int i;
	for (i = 0; i < DMXCSArray.size(); i++)
	{
		double cml = DMXCSArray[i].Cml;
		double earH = DMXCSArray[i].EarthH;
		double twh = DMXCSArray[i].DetaH;
		if (twh <= -DMXCS.MinTunH)//需要设置隧道
		{
			MinTunLen = DMXCS.MinTunL;
			MaxTunLen = DMXCS.maxTunlen;
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh < -DMXCS.DividTunH)//找终点
				{
					Btwh = Stwh;
					n--;
					cml = DMXCSArray[n].Cml;
					earH = DMXCSArray[n].EarthH;
					Stwh = DMXCSArray[n].DetaH;
				}
				if (Stwh > -DMXCS.DividTunH)
					sml = cml + (DMXCSArray[n + 1].Cml - cml)*(-DMXCS.DividTunH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				if (fabs(sml - cml) < fabs(DMXCSArray[n + 1].Cml - sml))
				{
					sml = cml;
					/********************检测隧道洞门位置********************/
				}
				else
				{
					sml = DMXCSArray[n + 1].Cml;
					/********************检测隧道洞门位置********************/
				}
				//sml = fabs(sml-cml)<fabs(DMXTWArray[n+1].x-sml) ? cml:DMXTWArray[n+1].x;

			}

			while (i < DMXCSArray.size() - 1 && twh <= -DMXCS.DividTunH)//找终点
			{
				Btwh = twh;
				i++;
				cml = DMXCSArray[i].Cml;
				earH = DMXCSArray[i].EarthH;
				twh = DMXCSArray[i].DetaH;
			}
			if (twh > -DMXCS.MinTunH && i < DMXCSArray.size() - 1)
				eml = DMXCSArray[i - 1].Cml + (cml - DMXCSArray[i - 1].Cml)*(-DMXCS.DividTunH - Btwh) / (twh - Btwh);
			else
				eml = DMXCSArray[i].Cml;
			if (fabs(eml - cml) < fabs(DMXCSArray[i - 1].Cml - eml))
			{
				eml = cml;
				/********************检测隧道洞门位置********************/
				/********************检测隧道洞门位置********************/
			}
			else
			{
				eml = DMXCSArray[i - 1].Cml;
				/********************检测隧道洞门位置********************/
			}
			//eml = fabs(eml-cml)<fabs(DMXTWArray[i-1].x-eml) ? cml:DMXTWArray[i-1].x;

			len = eml - sml;
			//if (len > MaxTunLen)//xjx
			//	return false;
			if (len > MinTunLen)//隧道长至少20m
			{
				///*****************************/
				Tunstruct.BCml = sml;
				Tunstruct.ECml = eml;
				Tunstruct.z = len;
				TunArray.push_back(Tunstruct);
			}
			else
				return false;//隧道太短
		}
		Btwh = twh;

	}

	return true;
}
double DLInintLwDlg::CalCost(vector<DEMData>& DMXCSArray, vector<BRITAB_DN>& BridgeArray, vector<TUNTAB_DN>& TunArray, DEMData DMXCS, double & Cost)
{
	Cost = 0.0;
	double Len = 0.0;
	DEMData Data;
	BridgeArray.clear();
	TunArray.clear();
	for (int i = 0; i < BridgeArray.size(); i++)   //计算桥梁造价  NBRI桥梁数量
	{
		Len = BridgeArray[i].ECml - BridgeArray[i].BCml;
		double cml = 0.5*(BridgeArray[i].BCml + BridgeArray[i].ECml);
		double Ptxy[2] = { 0.0 };
		Len = BridgeArray[i].ECml - BridgeArray[i].BCml;
		if (BridgeArray[i].TWg > DMXCS.MaxbridgeH)
		{
			if (Len < DMXCS.g_MaxLen_HighBri)
				Cost += 600.0 * Len * DMXCS.BriUnitCost;
			else
				Cost += 60000.0 * Len * DMXCS.BriUnitCost;
		}
		else
		{
			if (BridgeArray[i].TWg < DMXCS.CZBriHeight)
				Cost += Len * DMXCS.BriUnitCost;
			else
				Cost += Len * DMXCS.BriUnitCost * (BridgeArray[i].TWg / DMXCS.CZBriHeight);
		}
	}

	for (int i = 0; i < TunArray.size(); i++)   //计算隧道造价
	{

		Len = TunArray[i].ECml - TunArray[i].BCml;
		if (Len < DMXCS.CZTunlLen)
			Cost += DMXCS.TunlUnitCost * Len;
		else
			Cost += (Len / DMXCS.CZTunlLen) * DMXCS.TunlUnitCost * Len;
	}
	for (int i = 0; i < DMXCSArray.size(); i++)	//计算土方造价,每个桩号的填挖面积记录在y中
		DMXCSArray[i].Area = 0.0;

	bool InBT;//是否在桥隧内部
	for (int i = 1; i < DMXCSArray.size(); i++)
	{
		InBT = false;//是否在桥隧内部
		for (int iBri = 0; iBri < BridgeArray.size(); iBri++)
		{
			if (DMXCSArray[i].Cml > BridgeArray[iBri].BCml && DMXCSArray[i].Cml < BridgeArray[iBri].ECml)
			{
				InBT = true;
				DMXCSArray[i].DetaH = 0.0;
				break;
			}
		}
		if (InBT)
			continue;

		for (int iTun = 0; iTun < TunArray.size(); iTun++)
		{
			if (DMXCSArray[i].Cml > TunArray[iTun].BCml && DMXCSArray[i].Cml < TunArray[iTun].ECml)
			{
				InBT = true;
				DMXCSArray[i].DetaH = 0.0;
				break;
			}
		}
		if (InBT)
			continue;

		//计算填挖面积
		if (DMXCSArray[i].DetaH > MaxValue - 1.0)
		{
			Cost = MaxValue;
			return Cost;
		}
		else if (DMXCSArray[i].DetaH > 0.0)//填方
		{
			//临时改变路基面
			DMXCSArray[i].Area = (3.8+3.8 + 2 * DMXCSArray[i].DetaH * DMXCS.FillBPXS) * DMXCSArray[i].DetaH / 2.0;//梯形面积
			Cost += (DMXCSArray[i].Area + DMXCSArray[i - 1].Area) * (DMXCSArray[i].Cml - DMXCSArray[i - 1].Cml) * 0.5 * DMXCS.FillUnitCost;//土方造价
		}
		else//挖方
		{
			DMXCSArray[i].Area = (3.8+3.8 - 2 * DMXCSArray[i].DetaH * DMXCS.CutBPXS) * (-DMXCSArray[i].DetaH)/2.0;//梯形面积
			Cost += (DMXCSArray[i].Area + DMXCSArray[i - 1].Area) * (DMXCSArray[i].Cml - DMXCSArray[i - 1].Cml) * 0.5 * DMXCS.CutUnitCost;//土方造价
		}

	}
	return Cost;
}
bool DLInintLwDlg::AutoSetBRI(DEMData DMXCS, vector<DEMData>& DMXCSArray, vector<BRITAB_DN>& BridgeArray)
{
	// TODO: Implement the command
	BRITAB_DN Bridgetemp;
	double sml, eml, len, Btwh;
	int i;
	double cml = 0.0;
	double earH = 0.0;
	double twh = 0.0;

	double MinH, MinHCml;//最小地面线标高，最小地面线标高处的里程
	MinH = 1e99;
	double AveH = 0.0;
	int PtNum = 0;
	double MaxTWh = 0.0;
	double MinBriLen = 0.0;//最小桥长
	double MaxBriH = 100.0;

	for (i = 0; i < DMXCSArray.size(); i++)
	{
		cml = DMXCSArray[i].Cml;
		earH = DMXCSArray[i].EarthH;
		twh = DMXCSArray[i].DetaH;
		if (twh >= DMXCS.MinBriH)//可能设桥
		{
			MinBriLen = DMXCS.MinBriL;
			MaxBriH = DMXCS.MaxbridgeH;
			double Temptwh = twh;
			//找起点
			if (i < 1)
				sml = cml;
			else
			{
				int n = i;
				double Stwh = twh;
				while (n > 0 && Stwh > DMXCS.DividBriH)//找起点
				{
					Btwh = Stwh;
					n--;
					cml = DMXCSArray[n].Cml;
					earH = DMXCSArray[n].EarthH;
					Stwh = DMXCSArray[n].DetaH;
					Temptwh = max(Temptwh, Stwh);
				}
				if (Stwh < DMXCS.DividBriH)
					sml = cml + (DMXCSArray[n + 1].Cml - cml)*(DMXCS.DividBriH - Stwh) / (Btwh - Stwh);
				else
					sml = cml;
				if (fabs(sml - cml) < fabs(DMXCSArray[n + 1].Cml - sml))
				{
					sml = cml;
				}
				else
				{
					sml = DMXCSArray[n + 1].Cml;
				}

			}

			MinHCml = sml;

			while (i<DMXCSArray.size() - 1 && twh>DMXCS.DividBriH)//找终点
			{
				Btwh = twh;
				i++;
				cml = DMXCSArray[i].Cml;
				earH = DMXCSArray[i].EarthH;
				twh = DMXCSArray[i].DetaH;
				Temptwh = max(Temptwh, twh);

				if (earH < MinH)
				{
					MinH = earH;
					MinHCml = cml;
				}
				if (twh > MaxTWh)
				{
					MaxTWh = twh;
				}

				AveH += earH;
				PtNum++;
			}
			if (twh < DMXCS.DividBriH)
				eml = cml - (cml - DMXCSArray[i - 1].Cml)*(DMXCSArray[i - 1].DividBriH - twh) / (Btwh - twh);
			else
				eml = DMXCSArray[i].Cml;

			if (fabs(eml - cml) < fabs(DMXCSArray[i - 1].Cml - eml))
			{
				eml = cml;
			}
			else
			{
				eml = DMXCSArray[i - 1].Cml;
			}
			//eml = fabs(eml-cml)<fabs(DMXTWArray[i-1].x-eml) ? cml:DMXTWArray[i-1].x;


			len = eml - sml;
			if (len > MinBriLen)//桥长至少10m
			{
				Bridgetemp.BCml = sml;
				Bridgetemp.ECml = eml;
				Bridgetemp.TWg = Temptwh;
				if (Bridgetemp.TWg > MaxBriH && len > DMXCS.g_MaxLen_HighBri)
					return false;

				BridgeArray.push_back(Bridgetemp);

				MinH = 1e99;
				PtNum = 0;
				AveH = 0.0;
				MaxTWh = 0.0;
			}

		}
		Btwh = twh;
	}


	return true;
}
bool DLInintLwDlg::FormDMXTW(AcGePoint3d PT1, AcGePoint3d PT2, int & NCRS, DEMData DMXCS, vector<DEMData>& DMXCSArray)
{

	DEMData DMXCStemp;
	double NStart, EStart, NEnd, EEnd, NPre, EPre;
	int PreRow, PreCol;

	NCRS = 0;
	double slop;
	bool IsHighBri = false;
	int BriStaNum = 0;      //填方超过最大桥高的桩号数
	int TunStaNum = 0;      //隧道点数

	AcDbPolyline *pLine = NULL;
	double Bulge = 1.0;
	AcGePoint2d VerPt;
	double SumLength = 0.0;
	double NOUSE;
	//bool HasCheckXXW = FALSE;

	pLine = new AcDbPolyline(2);
	VerPt.x = PT1.x;
	VerPt.y = PT1.y;
	pLine->addVertexAt(0, VerPt);
	VerPt.x = PT2.x;
	VerPt.y = PT2.y;
	pLine->addVertexAt(1, VerPt);
	AcGePoint3d apt;

	pLine->getDistAtPoint(PT2, SumLength);
	double pdc = 0.00;
	//pdc = fabs(PT2.z - PT1.z);
	pdc = PT2.z - PT1.z;
	double slope = 0;
	if (pdc > 0)
		slope = m_OKPD / 100.0;
	else
		slope = -m_OKPD / 100.0;
	//double slope = pdc / SumLength;
	double TempLen = m_Size / 3;

	double Ptxy[2] = { 0.0 };
	DMXCStemp.Cml = 0;
	DMXCStemp.EarthH = PT1.z;     //地面高程
	DMXCStemp.DesH = PT1.z;
	DMXCStemp.DetaH = 0.00;
	DMXCSArray.push_back(DMXCStemp);
	while (TempLen < SumLength + 1.0)
	{
		bool IsInKY = false;//是否跨越
		AcGePoint3d Pt;
		pLine->getPointAtDist(TempLen, Pt);
		DMXCStemp.Cml = TempLen;
		Ptxy[0] = Pt.y;
		Ptxy[1] = Pt.x;
		Pt.z = ptriDTM.interpolate(Ptxy);
		DMXCStemp.EarthH = Pt.z;     //地面高程

		DMXCStemp.DesH = PT1.z + slope * TempLen;
		DMXCStemp.DetaH = DMXCStemp.DesH - DMXCStemp.EarthH;//高差
		///*if (slope*100.0 > 0.2)
		//	DMXCStemp.DetaH = MaxValue;
		//else */if (slope*100.0> m_MaxPD)
		//	DMXCStemp.DetaH *= pow(10, slope*100.0 / m_MaxPD);
		//else if (slope*100.0 > m_OKPD)
		//	DMXCStemp.DetaH *= pow(CFCoofe, slope*100.0 / m_MaxPD);
		TempLen += m_Size / 3;
		DMXCSArray.push_back(DMXCStemp);
	}
	//DMXCStemp.Cml = TempLen;
	//DMXCStemp.EarthH = PT2.z;     //地面高程
	//DMXCStemp.DesH = PT2.z;
	//DMXCStemp.DetaH = 0.00;
	//DMXCSArray.push_back(DMXCStemp);
	NCRS = DMXCSArray.size();
	delete pLine;
	pLine = NULL;//xjx 2017年5月26日9:53:51
	return true;
}
//bool DLInintLwDlg::FormDMXTW(AcGePoint3d PT1, AcGePoint3d PT2, int & NCRS, DEMData DMXCS, vector<DEMData>& DMXCSArray)
//{
//
//	DEMData DMXCStemp;
//	double NStart, EStart, NEnd, EEnd, NPre, EPre;
//	int PreRow, PreCol;
//	
//	NCRS = 0;
//	double slop;
//	bool IsHighBri = false;
//	int BriStaNum = 0;      //填方超过最大桥高的桩号数
//	int TunStaNum = 0;      //隧道点数
//
//	AcDbPolyline *pLine = NULL;
//	double Bulge = 1.0;
//	AcGePoint2d VerPt;
//	double SumLength = 0.0;
//	double NOUSE;
//	//bool HasCheckXXW = FALSE;
//
//	pLine = new AcDbPolyline(2);
//	VerPt.x = PT1.x;
//	VerPt.y = PT1.y;
//	pLine->addVertexAt(0, VerPt);
//	VerPt.x = PT2.x;
//	VerPt.y = PT2.y;
//	pLine->addVertexAt(1, VerPt);
//	AcGePoint3d apt;
//	
//	pLine->getDistAtPoint(PT2, SumLength);
//	double pdc = 0.00;
//	pdc = fabs(PT2.z - PT1.z);
//	double slope = pdc / SumLength;
//	double TempLen = m_Size/2;
//
//	double Ptxy[2] = { 0.0 };
//	DMXCStemp.Cml = 0;
//	DMXCStemp.EarthH = PT1.z;     //地面高程
//	DMXCStemp.DesH = PT1.z;
//	DMXCStemp.DetaH = 0.00;
//	DMXCSArray.push_back(DMXCStemp);
//	while (TempLen < SumLength - 1.0)
//	{
//		bool IsInKY = false;//是否跨越
//		AcGePoint3d Pt;
//		pLine->getPointAtDist(TempLen, Pt);
//		DMXCStemp.Cml = TempLen;
//		Ptxy[0] = Pt.y;
//		Ptxy[1] = Pt.x;
//		Pt.z = ptriDTM.interpolate(Ptxy);
//		DMXCStemp.EarthH = Pt.z;     //地面高程
//		
//		DMXCStemp.DesH = PT1.z + slope * TempLen;
//		DMXCStemp.DetaH = DMXCStemp.DesH - DMXCStemp.EarthH;//高差
//		/*if (slope*100.0 > 0.2)
//			DMXCStemp.DetaH = MaxValue;
//		else */if (slope*100.0> m_MaxPD)
//			DMXCStemp.DetaH *= pow(10, slope*100.0 / m_MaxPD);
//		else if (slope*100.0 > m_OKPD)
//			DMXCStemp.DetaH *= pow(CFCoofe, slope*100.0 / m_MaxPD);
//		TempLen += m_Size/2;
//		DMXCSArray.push_back(DMXCStemp);
//	}
//	DMXCStemp.Cml = TempLen;
//	DMXCStemp.EarthH = PT2.z;     //地面高程
//	DMXCStemp.DesH = PT2.z;
//	DMXCStemp.DetaH = 0.00;
//	DMXCSArray.push_back(DMXCStemp);
//	NCRS = DMXCSArray.size();
//	delete pLine;
//	pLine = NULL;//xjx 2017年5月26日9:53:51
//	return true;
//}


void DLInintLwDlg::OnBnClickedButtonExist()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	AcGePoint3d tempPt, UpLeftPt, DownRightPt;
	//框选建网范围
	CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pAcadWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE); // this hides your dialog
	pAcadWnd->SetFocus();

	acedGetPoint(NULL, L"\n框选范围：", asDblArray(UpLeftPt));
	acedGetCorner(asDblArray(UpLeftPt), L"", asDblArray(DownRightPt));
	if (UpLeftPt.x< DownRightPt.x &&UpLeftPt.y > DownRightPt.y)
		;
	else
	{
		tempPt = UpLeftPt;
		UpLeftPt = DownRightPt;
		DownRightPt = tempPt;
	}


	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kRead);
	ads_name ssname;
	acedSSGet(L"C", asDblArray(UpLeftPt), asDblArray(DownRightPt), NULL, ssname);
	long len = 0;
	ads_name entName;

	AcDbObjectId id;
	acedSSLength(ssname, &len);

	ExistRoadIds.removeAll();
	for (int i = 0; i < len; i++)
	{
		if (acedSSName(ssname, i, entName) == RTNORM)
		{
			acdbGetObjectId(id, entName);
			ExistRoadIds.append(id);
		}
	}

	acedSSFree(ssname);
	acDocManager->unlockDocument(acDocManager->curDocument());

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	pAcadWnd->EnableWindow(FALSE);
	EnableWindow(TRUE); // Enable our dialog*/
	//OnBnClickedButtonCrtgrd();
}
AcGePoint3dArray DLInintLwDlg::FormAbdMask(double length, int precision, AcGePoint3d Pt) {
	UpdateData(TRUE);
	AcGePoint3d leftUp, tempPt;
	AcGePoint3dArray pts;
	leftUp.set(Pt.x - length / 2.0, Pt.y + length / 2.0, 0);
	double singlen = length / precision;
	double Ptxy[2] = { 0 };
	for (int i = 0; i < precision; i++)
	{
		for (int j = 0; j < precision; j++)
		{
			Ptxy[0] = leftUp.y - j * singlen;
			Ptxy[1] = leftUp.x + i * singlen;
			tempPt.set(leftUp.x + i * singlen, leftUp.y - j * singlen, ptriDTM.interpolate(Ptxy));
			pts.append(tempPt);
		}
	}
	return pts;
}

AcGePoint3dArray DLInintLwDlg::FormAbdMask(double length, double step, int index) 
{
	UpdateData(TRUE);
	AcGePoint3dArray pts;
	int RCnum = length / step;
	int half = RCnum / 2;
	int baseIndex = index - half * Row;
	int largIndex= index + half * Row;
	if (baseIndex<0 || largIndex>NodeDatas.size() - 1)
		return pts;
	AcGePoint3d leftUp, tempPt;
	for (int i = 0; i < RCnum; i++)//列
	{
		for (int j = 0; j < RCnum; j++)//行
		{
			tempPt = NodeDatas.at(baseIndex+i*RCnum+j).PtNode;
			pts.append(tempPt);
		}
	}
	return pts;
}

void DLInintLwDlg::Sort(int flag)
{
	double tempDist;
	int Index;
	if (flag == 1)
	{
		for (size_t i = 0; i < BXDis2Road.size(); i++)
		{
			// 当前轮是否发生过交换事件标志位，若未发生交换，则表明列表已有序。
			bool isExchanged = false;
			for (size_t j = 0; j < BXDis2Road.size() - i - 1; j++)
			{
				if (BXDis2Road[j] > BXDis2Road[j + 1])
				{
					tempDist = BXDis2Road[j];
					BXDis2Road[j] = BXDis2Road[j + 1];
					BXDis2Road[j + 1] = tempDist;

					Index = IndexDropEarth[j];
					IndexDropEarth[j] = IndexDropEarth[j + 1];
					IndexDropEarth[j + 1] = Index;
					isExchanged = true;
				}
			}
			if (!isExchanged) {
				break;
			}
		}
	}
	else if (flag == 2)
	{
		for (size_t i = 0; i < BXDis2YL.size(); i++)
		{
			// 当前轮是否发生过交换事件标志位，若未发生交换，则表明列表已有序。
			bool isExchanged = false;
			for (size_t j = 0; j < BXDis2YL.size() - i - 1; j++)
			{
				if (BXDis2YL[j] > BXDis2YL[j + 1])
				{
					tempDist = BXDis2YL[j];
					BXDis2YL[j] = BXDis2YL[j + 1];
					BXDis2YL[j + 1] = tempDist;

					Index = IndexWorkhouse[j];
					IndexWorkhouse[j] = IndexWorkhouse[j + 1];
					IndexWorkhouse[j + 1] = Index;
					isExchanged = true;
				}
			}
			if (!isExchanged) {
				break;
			}
		}
	}

}




void DLInintLwDlg::OnBnClickedButtonCandipool()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//全网扫描选址
	ACHAR pszLabel[80];
	_tcscpy(pszLabel, L"DT扫描地形...");
	acedSetStatusBarProgressMeter(pszLabel, 0, 100);
	double Sum = 0.0, Ave = 0.0;
	double MaxH = -10000, MinH = 9999999;
	AcGePoint3dArray SetOfPts, XMBPts;
#pragma omp parallel for num_threads(8)
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		acedSetStatusBarProgressMeterPos((int)(i * 100 / NodeDatas.size()));
		MaxH = -10000, MinH = 9999999;
		Sum = 0.0, Ave = 0.0;
		SetOfPts.removeAll();
		XMBPts.removeAll();
		SetOfPts=FormAbdMask(m_TZLen, m_Size, NodeDatas.at(i).IdNum);
		if (SetOfPts.length()<1)
		{
			SetOfPts = FormAbdMask(m_TZLen, m_JM, NodeDatas.at(i).PtNode);
		}
		XMBPts = FormAbdMask(m_XMBLen, m_Size, NodeDatas.at(i).IdNum);
		if (XMBPts.length() < 1)
		{
			XMBPts = FormAbdMask(m_XMBLen, m_JM, NodeDatas.at(i).PtNode);
		}
		
		for (int k = 0; k < SetOfPts.length(); k++)
			Sum += SetOfPts.at(k).z;
		Ave = Sum / SetOfPts.length();

		double area = m_TZLen * m_TZLen / (m_JM*m_JM);
		//计算起伏度
		double res = 0;
		Sum = 0;
		for (int k = 0; k < SetOfPts.length(); k++)
		{
			res = Ave - SetOfPts.at(k).z;
			Sum += (res > 0 ? res : 0)*area;
		}
		NodeDatas.at(i).V_colum = Sum;
		//////////////
		for (int k = 0; k < XMBPts.length(); k++)
		{
			if (XMBPts.at(k).z > MaxH)
			{
				MaxH = XMBPts.at(k).z;
			}
			if (XMBPts.at(k).z < MinH)
			{
				MinH = XMBPts.at(k).z;
			}
		}

		NodeDatas.at(i).relief = MaxH - MinH;
		//计算粗糙度
		//平均值
		Sum = 0.0;
		for (int k = 0; k < XMBPts.length(); k++)
		{
			Sum += XMBPts.at(k).z;
		}
		Ave = Sum / XMBPts.length();
		Sum = 0.0;
		for (int k = 0; k < XMBPts.length(); k++)
		{
			Sum += pow(XMBPts.at(k).z - Ave, 2);
		}
		NodeDatas.at(i).roughness = sqrt(Sum / (XMBPts.length() - 1));

		/////////



	}
	acedSetStatusBarProgressMeterPos(100);
	acedRestoreStatusBar();

	double minDis = MaxValue;
	int recIndex = 0;
	double m_ToTalVcolum = m_Volum;
	for (int j = 0; j < GDPts.length(); j++)
	{
		AcGePoint2d pt2(GDPts.at(j).x, GDPts.at(j).y);
		minDis = MaxValue;
		for (int i = 0; i < NodeDatas.size(); i++)
		{
			if (NodeDatas.at(i).V_colum > m_ToTalVcolum)
			{
				double dis = 0;
				AcGePoint2d pt1(NodeDatas.at(i).PtNode.x, NodeDatas.at(i).PtNode.y);
				dis = pt1.distanceTo(pt2);
				if (dis < minDis&&dis>6*m_TZLen)
				{
					minDis = dis;
					recIndex = i;
				}
			}
		}
		IndexDropEarth.push_back(recIndex);
		NodeDatas.at(recIndex).V_colum -= m_ToTalVcolum;
	}

	CString strX, strY;
	m_Grid.SetRowCount(IndexDropEarth.size() + 2);
	m_Grid.Invalidate();
	for (int i = 0; i < IndexDropEarth.size(); i++)
	{
		m_Grid.SetFocusCell(i + 1, 2);
		strX.Format(L"%0.3lf", NodeDatas.at(IndexDropEarth.at(i)).PtNode.x);
		strY.Format(L"%0.3lf", NodeDatas.at(IndexDropEarth.at(i)).PtNode.y);
		m_Grid.SetItemText(i + 1, 1, L"弃渣场");
		m_Grid.SetItemText(i + 1, 2, strX);
		m_Grid.SetItemText(i + 1, 3, strY);
	}


	//////


	//归一化计算

	double Maxrelief = -10000, Minrelief = 999999, Maxroughness = -10000, Minroughness = 999999;
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		if (NodeDatas.at(i).relief < Minrelief)
			Minrelief = NodeDatas.at(i).relief;
		if (NodeDatas.at(i).roughness < Minroughness)
			Minroughness = NodeDatas.at(i).roughness;
		if (NodeDatas.at(i).relief > Maxrelief)
			Maxrelief = NodeDatas.at(i).relief;
		if (NodeDatas.at(i).roughness > Maxroughness)
			Maxroughness = NodeDatas.at(i).roughness;
	}

	for (int i = 0; i < NodeDatas.size(); i++)
	{
		NodeDatas.at(i).relief = (NodeDatas.at(i).relief - Minrelief) / (Maxrelief - Minrelief);
		NodeDatas.at(i).roughness = (NodeDatas.at(i).roughness - Minroughness) / (Maxroughness - Minroughness);
	}

	//筛选备选池中项目部
	//获得有利条件
	CString strInf;
	AcGePoint3d tempPt;
	double Ptxy[2] = { 0.0 };
	for (int i = 1; i < m_YLgrid.GetRowCount() + 1; i++)
	{
		strInf = m_YLgrid.GetItemText(i, 1);
		if (strInf != L"")
		{
			tempPt.set(_wtof(m_YLgrid.GetItemText(i, 2)), _wtof(m_YLgrid.GetItemText(i, 3)), 0);
			Ptxy[0] = _wtof(m_YLgrid.GetItemText(i, 3));
			Ptxy[1] = _wtof(m_YLgrid.GetItemText(i, 2));
			tempPt.z = ptriDTM.interpolate(Ptxy);
			YLPts.append(tempPt);
		}
	}
	//依据距离既有线路和地形和有利条件筛选
	AcGePoint3d PtTan, PtRoad;
	AcGePoint3d PtYl;
	BXDis2Road.clear();
	int Indexrcd = -1;
	bool isOk = true;
	for (int i = 0; i < NodeDatas.size(); i++)
	{
		isOk = false;
		for (int k = 0; k < YLPts.length(); k++)
		{
			if (NodeDatas.at(i).PtNode.distanceTo(YLPts.at(k)) < 2000)
			{
				isOk = true;
				break;
			}
		}
		if (!isOk)
			continue;
		for (int j = 0; j < FearIds.size(); j++)
		{
			if (NodeDatas.at(i).IdNum == FearIds.at(j))
			{
				isOk = false;
				break;
			}
		}
		if (!isOk)
			continue;
		else if (NodeDatas.at(i).relief < 0.2 && NodeDatas.at(i).roughness < 0.2)
		{
			IndexWorkhouse.push_back(i);
		}
	}
	if (IndexWorkhouse.size() < 1)
	{
		AfxMessageBox(L"无可用项目部，请检查设置");
		return;
	}
	/////
	double minCost = MaxValue;
	int recdIndex = 0;
	for (int t = 0; t < IndexWorkhouse.size(); t++)
	{
		int index = IndexWorkhouse.at(t);
		double tempCost = 0;
		for (int s = 0; s < DCost.size(); s++)
		{
			tempCost += DCost.at(s).at(index);
		}
		if (tempCost < minCost)
		{
			minCost = tempCost;
			recdIndex = index;
		}
	}
	IndexWorkhouse.clear();
	IndexWorkhouse.push_back(recdIndex);
	////
	//Sort(2);
	if (IndexWorkhouse.size() > 0)
	{
		m_Grid.SetFocusCell(IndexDropEarth.size() + 1, 2);
		strX.Format(L"%0.3lf", NodeDatas.at(IndexWorkhouse.at(0)).PtNode.x);
		strY.Format(L"%0.3lf", NodeDatas.at(IndexWorkhouse.at(0)).PtNode.y);
		m_Grid.SetItemText(IndexDropEarth.size() + 1, 1, L"项目部");
		m_Grid.SetItemText(IndexDropEarth.size() + 1, 2, strX);
		m_Grid.SetItemText(IndexDropEarth.size() + 1, 3, strY);
		strX.Format(L"%d", IndexDropEarth.size() + 1);
		m_Grid.SetItemText(IndexDropEarth.size() + 1, 0, strX);
		OnBnClickedButtonDropearth();
	}
	
}


void DLInintLwDlg::OnBnClickedButtonDladdrowbx()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_Grid.GetFocusCell().row;
	if (nRow >= 0)
	{
		CString str;
		Item.row = nRow;
		m_Grid.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_Grid.GetColumnCount(); i++)
		{
			Item.col = i;
			m_Grid.SetItem(&Item);
		}


		for (i = nRow; i < m_Grid.GetRowCount(); i++)
		{
			str.Format(L"%d", i);
			m_Grid.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_Grid.GetRowCount(); i++)
			m_Grid.SetRowHeight(i, DEFAULTCOLHEI);

		m_Grid.SetItemText(nRow, 1, m_Grid.GetItemText(nRow + 1, 1));

		m_Grid.SetFocusCell(nRow, 1);

		m_Grid.Invalidate();
	}
}


void DLInintLwDlg::OnBnClickedButtonDldetbx()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_Grid.GetFocusCell().row;
	if (nRow == m_Grid.GetRowCount() - 1) return;
	if (nRow >= 0)
	{
		m_Grid.DeleteRow(nRow);
		for (int i = nRow; i < m_Grid.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_Grid.SetItemText(i, 0, str);
		}
		m_Grid.SetFocusCell(nRow, 1);
		m_Grid.Invalidate();
	}
}


void DLInintLwDlg::OnBnClickedButtonPickyl()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pAcadWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE); // this hides your dialog
	pAcadWnd->SetFocus();
	CString ZbInf;
	AcGePoint3d PickPt;
	int rc = RTNORM;
	rc = acedGetPoint(NULL, L"\n拾取一点：", asDblArray(PickPt));
	ZbInf.Format(L"%0.4lf", PickPt.x);
	m_YLgrid.SetItemText(m_YLgrid.GetFocusCell().row, 2, ZbInf);
	ZbInf.Format(L"%0.4lf", PickPt.y);
	double Ptxy[2] = { 0 };
	Ptxy[0] = PickPt.y;
	Ptxy[1] = PickPt.x;
	PickPt.z = ptriDTM.interpolate(Ptxy);
	m_YLgrid.SetItemText(m_YLgrid.GetFocusCell().row, 3, ZbInf);
	m_YLgrid.SetFocusCell(m_YLgrid.GetFocusCell().row + 1, 2);
	ob.makecircle(PickPt, 10, 3, L"工点位置");
	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	pAcadWnd->EnableWindow(FALSE);
	EnableWindow(TRUE); // Enable our dialog*/
}


void DLInintLwDlg::OnBnClickedButtonDladdrowyl()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_YLgrid.GetFocusCell().row;
	if (nRow >= 0)
	{
		CString str;
		Item.row = nRow;
		m_YLgrid.InsertRow(_T("NEW"), nRow);
		int i;
		for (i = 0; i < m_YLgrid.GetColumnCount(); i++)
		{
			Item.col = i;
			m_YLgrid.SetItem(&Item);
		}


		for (i = nRow; i < m_YLgrid.GetRowCount(); i++)
		{
			str.Format(L"%d", i);
			m_YLgrid.SetItemText(i, 0, str);
		}
		for (i = 1; i < m_YLgrid.GetRowCount(); i++)
			m_YLgrid.SetRowHeight(i, DEFAULTCOLHEI);

		m_YLgrid.SetItemText(nRow, 1, m_YLgrid.GetItemText(nRow + 1, 1));

		m_YLgrid.SetFocusCell(nRow, 1);

		m_YLgrid.Invalidate();
	}
}


void DLInintLwDlg::OnBnClickedButtonDldetyl()
{
	// TODO: 在此添加控件通知处理程序代码
	int nRow = m_YLgrid.GetFocusCell().row;
	if (nRow == m_YLgrid.GetRowCount() - 1) return;
	if (nRow >= 0)
	{
		m_YLgrid.DeleteRow(nRow);
		for (int i = nRow; i < m_YLgrid.GetRowCount(); i++)
		{
			CString str;
			str.Format(L"%d", i);
			m_YLgrid.SetItemText(i, 0, str);
		}
		m_YLgrid.SetFocusCell(nRow, 1);
		m_YLgrid.Invalidate();
	}
}


void DLInintLwDlg::OnBnClickedButtonRedrawgd()
{
	// TODO: 在此添加控件通知处理程序代码
	AcGePoint3dArray Pts;
	AcDbObjectId tempId;
	for (int i = 0; i < GDPts.length(); i++)
	{
		Pts.removeAll();
		Pts.append(AcGePoint3d(GDPts.at(i).x, GDPts.at(i).y, 0));

		tempId = ob.CreateHatch(Pts, L"SOLID", true, 1, true);
		TreenIds.append(tempId);
	}
}


void DLInintLwDlg::OnBnClickedButtonRewrawyl()
{
	// TODO: 在此添加控件通知处理程序代码
	AcGePoint3dArray Pts;
	AcDbObjectId tempId;
	for (int i = 0; i < YLPts.length(); i++)
	{
		Pts.removeAll();
		Pts.append(AcGePoint3d(YLPts.at(i).x, YLPts.at(i).y, 0));
		tempId = ob.CreateHatch(Pts, L"SOLID", true, 3, true);
		TreenIds.append(tempId);
	}
}


void DLInintLwDlg::OnBnClickedButtonDropearth()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//组建弃渣场路网
	Recamg = amg;
	for (int k = 0; k < IndexDropEarth.size(); k++)
	{
		int SetOffX = 50000, SetOffY = 0;
		int temp = IndexDropEarth.at(k);
		int temp1, temp2;
		int flag[10000], m = 0;
		CString str;
		str.Format(L"\n从%d到%d最短路径投资为：", FearIds.at(k), IndexDropEarth.at(k));
		acutPrintf(str);

		while (temp != -1)
		{
			flag[m++] = temp;
			if (temp == FearIds.at(k))
				break;
			temp1 = temp;
			temp2 = Paths[k][temp1];
			Paths[k][temp1] = -1;
			temp = temp2;
		}
		
		PassRoad.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			PassRoad.push_back(_wtoi(amg.vexs[flag[i]]));
		}


		Indexs.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			Indexs.push_back(_wtoi(amg.vexs[flag[i]]));
		}
		//将路径节点连接代价更新为拓展代价
		if (Indexs.size() > 1)
		{
			for (int i = 0; i < Indexs.size() - 1; i++)
			{
				NodeDatas.at(Indexs.at(i)).isOnExistRoad = true;
				amg.arcs[Indexs.at(i)][Indexs.at(i + 1)] = 0.0;
			}
		}
		//CostsofDropEarth.at(k) += D[ComPIds[pp][1]];
		str.Format(L"%0.3lf万元", D[IndexDropEarth.at(k)]);
		acutPrintf(str);

		ForPlinePts.removeAll();
		temppts.removeAll();
		for (int i = 0; i < Indexs.size(); i++)
		{
			temppts.append(AcGePoint2d(amg.LocationPts.at(Indexs.at(i)).x, amg.LocationPts.at(Indexs.at(i)).y));
		}
		ForPlinePts = AdjustPoints(temppts);

		//ForPlinePts.removeAll();
		//for (int i = 0; i < Indexs.size()/* - 1*/; i++)
		//{
		//	ForPlinePts.append(AcGePoint2d(amg.LocationPts.at(Indexs.at(i)).x, amg.LocationPts.at(Indexs.at(i)).y));
		//}
		AcGePoint3dArray forhatchCir;
		//ob.Create3dPolyLine(ForPlinePts, 2, 0, 0, 2);
		//ob.makepolylineTD(ForPlinePts, 2, 1.0, L"弃渣场支线");
		forhatchCir.removeAll();
		
		ob.makepolyline(ForPlinePts, 2, 1.0);
		forhatchCir.append(AcGePoint3d(ForPlinePts.at(ForPlinePts.length() - 1).x, ForPlinePts.at(ForPlinePts.length() - 1).y,0));
		ob.CreateHatch(forhatchCir, L"SOLID", true, 2, true);
		//for (int j = 0; j < m_NumSelect; j++)
		//{
		//	/*forhatchCir.removeAll();
		//	ob.Create3dPolyLine(ForPlinePts, 2, SetOffX + k * 50000, SetOffY + j * 50000);
		//	forhatchCir.append(AcGePoint3d(ForPlinePts.at(0).x + SetOffX + k * 50000, ForPlinePts.at(0).y + SetOffY + j * 50000, ForPlinePts.at(0).z));
		//	ob.CreateHatch(forhatchCir, L"SOLID", true, 2,true);*/
		//	ob.Create3dPolyLine(ForPlinePts, 2, SetOffX + k * 50000, SetOffY + j * 50000, 2);
		//	forhatchCir.removeAll();
		//	forhatchCir.append(AcGePoint3d(ForPlinePts.at(ForPlinePts.length() - 1).x + SetOffX + k * 50000, ForPlinePts.at(ForPlinePts.length() - 1).y + SetOffY + j * 50000, ForPlinePts.at(ForPlinePts.length() - 1).z));
		//	ob.CreateHatch(forhatchCir, L"SOLID", true, 2, true);
		//}
	}
	Sleep(500);
	OnBnClickedButtonWorkhouse();
}


void DLInintLwDlg::OnBnClickedButtonWorkhouse()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//组建弃渣场路网
	Recamg = amg;
	for (int k = 0; k < FearIds.size(); k++)
	{
		int SetOffX = 50000, SetOffY = 0;
		int temp = IndexWorkhouse.at(0);
		int temp1, temp2;
		int flag[10000], m = 0;
		CString str;
		str.Format(L"\n从%d到%d最短路径投资为：", FearIds.at(k), IndexWorkhouse.at(0));
		acutPrintf(str);

		while (temp != -1)
		{
			flag[m++] = temp;
			if (temp == FearIds.at(k))
				break;
			temp1 = temp;
			temp2 = Paths[k][temp1];
			temp = temp2;
		}
		PassRoad.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			PassRoad.push_back(_wtoi(amg.vexs[flag[i]]));
		}


		Indexs.clear();
		for (int i = m - 1; i >= 0; i--)
		{
			Indexs.push_back(_wtoi(amg.vexs[flag[i]]));
		}
		//将路径节点连接代价更新为拓展代价
		if (Indexs.size() > 1)
		{
			for (int i = 0; i < Indexs.size() - 1; i++)
			{
				NodeDatas.at(Indexs.at(i)).isOnExistRoad = true;
				amg.arcs[Indexs.at(i)][Indexs.at(i + 1)] = 0.00;
			}
		}
		//CostsofDropEarth.at(k) += D[ComPIds[pp][1]];
		str.Format(L"%0.3lf万元", D[IndexWorkhouse.at(0)]);
		acutPrintf(str);

		ForPlinePts.removeAll();
		temppts.removeAll();
		for (int i = 0; i < Indexs.size(); i++)
		{
			temppts.append(AcGePoint2d(amg.LocationPts.at(Indexs.at(i)).x, amg.LocationPts.at(Indexs.at(i)).y));
		}
		ForPlinePts = AdjustPoints(temppts);
		AcGePoint3dArray forhatchCir;
		ob.makepolyline(ForPlinePts, 3, 0.5);	
		forhatchCir.removeAll();
		forhatchCir.append(AcGePoint3d(ForPlinePts.at(ForPlinePts.length() - 1).x, ForPlinePts.at(ForPlinePts.length() - 1).y,0));
		ob.CreateHatch(forhatchCir, L"SOLID", true, 3, true);
		
	}



	GetLocalTime(&lpSystemTime);   // address of system time structure
	double CurTimeNumLast = lpSystemTime.wYear * 1e8 + lpSystemTime.wMonth * 1e6 + lpSystemTime.wDay * 1e4 + lpSystemTime.wHour * 100 + lpSystemTime.wMinute + lpSystemTime.wSecond / 100.0;
	CString strTime;
	strTime.Format(L"用时：%0.3lf", CurTimeNumLast - CurTimeNum);
	acutPrintf(strTime);
	AfxMessageBox(L"搜索完成！！");
}


void DLInintLwDlg::OnBnClickedButtonTuba()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	AcGePoint3d tempPt, UpLeftPt, DownRightPt;
	//框选建网范围
	CWnd* pAcadWnd = CWnd::FromHandle(adsw_acadMainWnd());
	pAcadWnd->EnableWindow(TRUE);
	ShowWindow(SW_HIDE); // this hides your dialog
	pAcadWnd->SetFocus();

	acedGetPoint(NULL, L"\n框选范围：", asDblArray(UpLeftPt));
	acedGetCorner(asDblArray(UpLeftPt), L"", asDblArray(DownRightPt));
	if (UpLeftPt.x< DownRightPt.x &&UpLeftPt.y > DownRightPt.y)
		;
	else
	{
		tempPt = UpLeftPt;
		UpLeftPt = DownRightPt;
		DownRightPt = tempPt;
	}


	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kRead);
	ads_name ssname;
	acedSSGet(L"C", asDblArray(UpLeftPt), asDblArray(DownRightPt), NULL, ssname);
	long len = 0;
	ads_name entName;

	AcDbObjectId id;
	acedSSLength(ssname, &len);

	ForbidIds.removeAll();
	for (int i = 0; i < len; i++)
	{
		if (acedSSName(ssname, i, entName) == RTNORM)
		{
			acdbGetObjectId(id, entName);
			ForbidIds.append(id);
		}
	}

	acedSSFree(ssname);
	acDocManager->unlockDocument(acDocManager->curDocument());

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	pAcadWnd->EnableWindow(FALSE);
	EnableWindow(TRUE); // Enable our dialog*/
	//OnBnClickedButtonCrtgrd();
}
