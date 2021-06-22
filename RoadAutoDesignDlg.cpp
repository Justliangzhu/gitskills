// RoadAutoDesignDlg.cpp: 实现文件
//
#include "StdAfx.h"
#include "RoadAutoDesignDlg.h"
#include "BAS_DRAW_FUN.h"
#include "RoadAutoDesign.h"
#include "RoadDesignWorkingDatabase.h"
#include "CSetOutPileAttri.h"
//#include "afxdialogex.h"

extern CString Cworkdir, Pro;
extern int isCT;//1表示只自动出图，不设计 0表示先自动设计，后出图
extern RoadDesignWorkingDatabase g_RoadWorkingDatabase;
extern void GetCurrentView(AcDbViewTableRecord &view);
extern void ZoomWindow(AcGePoint3d PT1, AcGePoint3d PT2);
// RoadAutoDesignDlg 对话框

IMPLEMENT_DYNAMIC(RoadAutoDesignDlg, CDialog)

RoadAutoDesignDlg::RoadAutoDesignDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_AUTODESIGN, pParent)
	, m_ProPath(_T(""))
	, m_Coor_X(0)
	, m_Coor_Y(0)
	, m_FramePath(_T(""))
	, m_IsFromMdb(FALSE)
	, m_IF_PLCT(FALSE)
	, m_Skml()
	, m_Ekml()
	, m_IsAllInView(FALSE)
{
}

RoadAutoDesignDlg::~RoadAutoDesignDlg()
{
}

void RoadAutoDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AUTODES_PATH, m_ProPath);
	DDX_Text(pDX, IDC_EDIT_AUTODES_XCOOR, m_Coor_X);
	DDX_Text(pDX, IDC_EDIT_AUTODES_YCOOR, m_Coor_Y);
	DDX_Text(pDX, IDC_EDIT4, m_FramePath);
	DDX_Check(pDX, IDC_CHECK_AUTODES_ISFROMMDB, m_IsFromMdb);
	DDX_Check(pDX, IDC_PLCT, m_IF_PLCT);
	DDX_Text(pDX, IDC_EDIT_Sml, m_Skml);
	DDX_Text(pDX, IDC_EDIT_Eml, m_Ekml);
	DDX_Check(pDX, IDC_CHECK_isInWindow, m_IsAllInView);
}


BEGIN_MESSAGE_MAP(RoadAutoDesignDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_BRO, &RoadAutoDesignDlg::OnBnClickedButtonAutodesBro)
	ON_BN_CLICKED(IDOK, &RoadAutoDesignDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_PICK, &RoadAutoDesignDlg::OnBnClickedButtonAutodesPick)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_BROFRAME, &RoadAutoDesignDlg::OnBnClickedButtonAutodesBroframe)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_HDRAWPARA, &RoadAutoDesignDlg::OnBnClickedButtonAutodesHdrawpara)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_VDRAWPARA, &RoadAutoDesignDlg::OnBnClickedButtonAutodesVdrawpara)
	ON_BN_CLICKED(IDC_BUTTON_SETOUTPILEATTRI, &RoadAutoDesignDlg::OnBnClickedButtonSetoutpileattri)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_SETCROSSPARA, &RoadAutoDesignDlg::OnBnClickedButtonAutodesSetcrosspara)
	ON_BN_CLICKED(IDC_BUTTON_AUTODES_OVALPARA, &RoadAutoDesignDlg::OnBnClickedButtonAutodesOvalpara)
	ON_BN_CLICKED(IDC_PLCT, &RoadAutoDesignDlg::OnBnClickedPlct)
END_MESSAGE_MAP()


// RoadAutoDesignDlg 消息处理程序


void RoadAutoDesignDlg::OnBnClickedButtonAutodesBro()
{
	UpdateData(true);
	// TODO: 在此添加控件通知处理程序代码
	//2020-2-28梁柱
	if (m_IF_PLCT==0)
	{
		
		CString curpm = m_ProPath;
		CFileDialog fname(true, NULL, curpm);
		fname.m_ofn.lpstrDefExt = L"mdb";
		fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
		fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER ;
		fname.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0";

		if (fname.DoModal() == IDOK)
		{

			m_ProNo = fname.GetFileName();
			m_ProNo.MakeUpper();
			m_ProNo.Replace(L".MDB", NULL);

			m_ProPath = fname.GetPathName();
			CString str = fname.GetPathName();
			str.MakeUpper();

			CString str1 = L"\\data\\" + m_ProNo + L".mdb";
			str1.MakeUpper();
			str.Replace(str1, NULL);
			m_dirstr = str;

			//向workdir写项目路径
			ACHAR wfname[1024];
			FILE* fp = NULL;
			CString  tmpname;

			_stprintf(wfname, L"%s\\workdir", SysPath);

			if ((fp = _wfopen(wfname, L"w")) != NULL)
			{
				tmpname = m_dirstr;
				NullInStrto$(tmpname);
				fwprintf(fp, L"%s", tmpname);
				fclose(fp);
			}

			str = L"当前工程项目:" + m_dirstr + L"  [方案号: " + m_ProNo + L"]";
			acedGetAcadFrame()->SetWindowText(str);

			UpdateData(false);
		}
	}
	else
	{
		
		CString curpm = m_ProPath;
		CFileDialog fname(true, NULL, curpm);
		fname.m_ofn.lpstrDefExt = L"mdb";
		fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
		fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER | OFN_ALLOWMULTISELECT;
		fname.m_ofn.lpstrFilter = L"线路方案数据库(*.mdb)\0*.mdb\0";

		if (fname.DoModal() == IDOK)
		{
			PathArray.clear();
			CString Path;
			POSITION pos = fname.GetStartPosition();
			while (pos)
			{
				Path = fname.GetNextPathName(pos);
				PathArray.push_back(Path);
				
			}
			m_ProPath = L"";
			for (int i = 0; i < PathArray.size(); i++)
			{
				
				m_ProPath +=PathArray.at(i);
				m_ProPath += L";\n";
			}
			
			acedGetAcadFrame()->SetWindowText(L"批量出图");

			UpdateData(false);
		}
	}
	
}

BOOL RoadAutoDesignDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	//20200322梁柱，隐藏浏览按钮
	GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(false);
	BAS_DRAW_FUN Bas_Fun;
	Bas_Fun.ReadWorkdir();
	m_ReadTK = FALSE;
	m_ProPath = Cworkdir + L"\\data\\" + Pro + L".mdb";
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_ProPath, L"主线");
	m_Skml=0.0;
	m_Ekml=pRoad->XLLength;
	//if (isCT == 1)//只出图，不设计
	//{
	//	GetDlgItem(IDC_CHECK_AUTODES_ISFROMMDB)->ShowWindow(FALSE);
	//	GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(FALSE);
	//}
	//2020-2-28梁柱
	GetDlgItem(IDC_CHECK_AUTODES_ISFROMMDB)->ShowWindow(FALSE);
	//GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(FALSE);

	//读取平面绘图参数
	if (!g_RoadWorkingDatabase.GetHorizontalDarwPara(m_ProPath, L"主线", m_HDrawPara))
	{
		AfxMessageBox(L"读取平面绘图参数失败，请检查arx放置位置是否正确！");
	}

	//设置纵面绘图参数
	SetProfilePara();

	UpdateData(FALSE);

	return TRUE;
}

void RoadAutoDesignDlg::NullInStrto$(CString& text)
{
	int len, i;

	len = text.GetLength();
	i = text.Find(L" ");

	while (i < len && i >= 0)
	{
		text.SetAt(i, '$');
		i = text.Find(L" ");
	}
	if (len == 0)
		text = "$";
}
//void GetCurrentView(AcDbViewTableRecord &view)
//{
//	//AcDbViewTableRecord view;
//	struct resbuf rb;
//	struct resbuf wcs, ucs, dcs; // 转换坐标时使用的坐标系统标记
//	wcs.restype = RTSHORT;
//	wcs.resval.rint = 0;
//	ucs.restype = RTSHORT;
//	ucs.resval.rint = 1;
//	dcs.restype = RTSHORT;
//	dcs.resval.rint = 2;
//
//	// 获得当前视口的“查看”模式
//	acedGetVar(L"VIEWMODE", &rb);
//	view.setPerspectiveEnabled(rb.resval.rint & 1);//透视
//	view.setFrontClipEnabled(rb.resval.rint & 2);//剪切
//	view.setBackClipEnabled(rb.resval.rint & 4);//回剪切
//	view.setFrontClipAtEye(!(rb.resval.rint & 16));//前剪切
//
//												   // 当前视口中视图的中心点（UCS坐标）
//	acedGetVar(L"VIEWCTR", &rb);
//	acedTrans(rb.resval.rpoint, &ucs, &dcs, 0, rb.resval.rpoint);
//	view.setCenterPoint(AcGePoint2d(rb.resval.rpoint[X], rb.resval.rpoint[Y]));
//
//	// 当前视口透视图中的镜头焦距长度（单位为毫米）
//	acedGetVar(L"LENSLENGTH", &rb);
//	view.setLensLength(rb.resval.rreal);
//
//	// 当前视口中目标点的位置（以 UCS 坐标表示）
//	acedGetVar(L"TARGET", &rb);
//	acedTrans(rb.resval.rpoint, &ucs, &wcs, 0, rb.resval.rpoint);
//	view.setTarget(AcGePoint3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));
//
//	// 当前视口的观察方向（UCS）
//	acedGetVar(L"VIEWDIR", &rb);
//	acedTrans(rb.resval.rpoint, &ucs, &wcs, 1, rb.resval.rpoint);
//	view.setViewDirection(AcGeVector3d(rb.resval.rpoint[X], rb.resval.rpoint[Y], rb.resval.rpoint[Z]));
//
//	// 当前视口的视图高度（图形单位）
//	acedGetVar(L"VIEWSIZE", &rb);
//	view.setHeight(rb.resval.rreal);
//	double height = rb.resval.rreal;
//
//	// 以像素为单位的当前视口的大小（X 和 Y 值）
//	acedGetVar(L"SCREENSIZE", &rb);
//	view.setWidth(rb.resval.rpoint[X] / rb.resval.rpoint[Y] * height);
//
//	// 当前视口的视图扭转角
//	acedGetVar(L"VIEWTWIST", &rb);
//	view.setViewTwist(rb.resval.rreal);
//
//	// 将模型选项卡或最后一个布局选项卡置为当前
//	acedGetVar(L"TILEMODE", &rb);
//	int tileMode = rb.resval.rint;
//
//	// 设置当前视口的标识码（modelSpace视图还是paperSpace视图）
//	acedGetVar(L"CVPORT", &rb);
//	int cvport = rb.resval.rint;
//
//	// 是否是模型空间的视图
//	bool paperspace = ((tileMode == 0) && (cvport == 1)) ? true : false;
//	view.setIsPaperspaceView(paperspace);
//	if (!paperspace)
//	{
//		// 当前视口中前向剪裁平面到目标平面的偏移量
//		acedGetVar(L"FRONTZ", &rb);
//		view.setFrontClipDistance(rb.resval.rreal);
//
//		// 获得当前视口后向剪裁平面到目标平面的偏移值
//		acedGetVar(L"BACKZ", &rb);
//		view.setBackClipDistance(rb.resval.rreal);
//	}
//	else
//	{
//		view.setFrontClipDistance(0.0);
//		view.setBackClipDistance(0.0);
//	}
//	return;
//}
//void ZoomWindow(AcGePoint3d PT1, AcGePoint3d PT2)
//{
//	// 提示用户选择定义缩放窗口的两个角点
//	ads_point pt1, pt2;
//
//	pt1[X] = PT1.x;
//	pt1[Y] = PT1.y;
//	pt1[Z] = PT1.z;
//	pt2[X] = PT2.x;
//	pt2[Y] = PT2.y;
//	pt2[Z] = PT2.z;
//	//pt1 = asDblArray(PT1);
//	// 转换坐标时使用的坐标系统标记 
//	struct resbuf wcs, ucs, dcs;
//	wcs.restype = RTSHORT;
//	wcs.resval.rint = 0;
//	ucs.restype = RTSHORT;
//	ucs.resval.rint = 1;
//	dcs.restype = RTSHORT;
//	dcs.resval.rint = 2;
//
//	acedTrans(pt1, &ucs, &dcs, 0, pt1);
//	acedTrans(pt2, &ucs, &dcs, 0, pt2);
//	AcDbViewTableRecord view;
//	GetCurrentView(view);
//
//	// 设置视图的中心点
//	view.setCenterPoint(AcGePoint2d((pt1[X] + pt2[X]) / 2, (pt1[Y] + pt2[Y]) / 2));
//
//	// 设置视图的高度和宽度
//	view.setHeight(fabs(pt1[Y] - pt2[Y]));
//	view.setWidth(fabs(pt1[X] - pt2[X]));
//
//	// 将视图对象设置为当前视图
//	acedSetCurrentView(&view, NULL);
//}
void RoadAutoDesignDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CDialog::OnOK();
	UpdateData(true);
	//2020-2-28梁柱
	if (m_IF_PLCT==0)
	{
		RoadAutoDesign AuDes;
		AuDes.SetMdbName(m_ProPath);
		AuDes.SetRoadName(L"主线");
		AuDes.SetFramePath(m_FramePath);
		AuDes.SetDrawOriCoor(m_Coor_X, m_Coor_Y);
		AuDes.SetDataSource(m_IsFromMdb);
		AuDes.SetTK(m_ReadTK);//图框
		AuDes.SetOverallDrawPara(m_OvalDrawPara);
		AuDes.SetHorizontalDrawPara(m_HDrawPara);
		AuDes.SetProfileDrawPara(m_VDrawPara);
		AuDes.SetCrossDrawPara(m_CrossDrawPara);
		AuDes.SetCrossAssignedPileAttri(m_CrossTypeArray);
		AuDes.CTsml = m_Skml;
		AuDes.CTeml = m_Ekml;
		AuDes.if_PL_CT=false;
		AuDes.isAllinView = false/*m_IsAllInView*/;
		AuDes.AutoDesign();
	}
	else
	{
		//20200323梁柱，出图完成回到初始窗口
		AcGePoint3d L, R;
		L.set(m_Coor_X, m_Coor_Y, 0);
		for (int i = 0; i < PathArray.size(); i++)
		{
			RoadAutoDesign AuDes;
			m_ProPath = PathArray.at(i);
			AuDes.SetMdbName(m_ProPath);
			AuDes.SetRoadName(L"主线");
			AuDes.SetFramePath(m_FramePath);
			m_Coor_Y = m_Coor_Y+ 800;
			AuDes.SetDrawOriCoor(m_Coor_X, m_Coor_Y);
			//AuDes.SetDataSource(m_IsFromMdb);
			AuDes.SetTK(m_ReadTK);//图框
			AuDes.SetOverallDrawPara(m_OvalDrawPara);
			AuDes.SetHorizontalDrawPara(m_HDrawPara);
			AuDes.SetProfileDrawPara(m_VDrawPara);
			AuDes.SetCrossDrawPara(m_CrossDrawPara);
			AuDes.SetCrossAssignedPileAttri(m_CrossTypeArray);
			AuDes.AutoDesign();
		}
		R.set(m_Coor_X + 800 * PathArray.size(), m_Coor_Y + 800, 0);
		//ZoomWindow(L,R);
		BAS_DRAW_FUN::Zoom(L.x, R.y, R.x, L.y);
		ads_printf(L"X:%lf, Y:%lf, LX:%lf, RY:%lf, RX:%lf, LY:%lf\n", m_Coor_X, m_Coor_Y, L.x, R.y, R.x, L.y);
	}

}


void RoadAutoDesignDlg::OnBnClickedButtonAutodesPick()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: Add your control notification handler code here
	AcGePoint3d PT;
	ads_point resPt;
	GetParent()->EnableWindow(TRUE);
	ShowWindow(SW_HIDE);		// Hide our dialog
	GetParent()->SetFocus();	// Give AutoCAD the focus.

	if (acedGetPoint(NULL, L"请在图上空白处(务必远离地形图)采集一点:_\n", resPt) == RTNORM)
	{
		m_Coor_X = resPt[X];
		m_Coor_Y = resPt[Y];
	}

	ShowWindow(SW_SHOW); // Display our dialog again
	SetFocus(); // Reset the focus back to ourselves
	GetParent()->EnableWindow(FALSE); // Disable AutoCAD window
	EnableWindow(TRUE); // Enable our dialog*/

	UpdateData(false);
}


void RoadAutoDesignDlg::OnBnClickedButtonAutodesBroframe()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString curpm = m_ProPath;
	CFileDialog fname(true, NULL, curpm);
	fname.m_ofn.lpstrDefExt = L"dwg";
	fname.m_ofn.lpstrInitialDir = Cworkdir + "\\data\\";
	fname.m_ofn.Flags = /*OFN_CREATEPROMPT|*/OFN_LONGNAMES | OFN_EXPLORER;
	fname.m_ofn.lpstrFilter = L"CAD图形文件(*.dwg)\0*.dwg\0";

	if (fname.DoModal() == IDOK)
	{
		m_FramePath = fname.GetPathName();
		m_ReadTK = TRUE;
	}

	UpdateData(FALSE);
}


void RoadAutoDesignDlg::OnBnClickedButtonAutodesHdrawpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_HDrawParaDlg.m_DrawPara = m_HDrawPara;

	if (m_HDrawParaDlg.DoModal() == IDOK)
		m_HDrawPara = m_HDrawParaDlg.m_DrawPara;
	SetFocus();
}

//设置初始纵断面参数
void RoadAutoDesignDlg::SetProfilePara()
{
	JD_CENTER* pRoad = g_RoadWorkingDatabase.CreateHorizontalRoad(m_ProPath, L"主线");
	if (!pRoad)
	{
		AfxMessageBox(L"设置纵断面绘图参数时无法获取平面信息，请确认是否有平面方案！");
		return;
	}

	//设置绘图参数
	m_VDrawPara.DrawMode = 1;
	m_VDrawPara.FForZF = 0;
	m_VDrawPara.ZPInf = 0;
	m_VDrawPara.Hscale = 2000.0;
	m_VDrawPara.Zscale = 200.0;
	m_VDrawPara.StDml = pRoad->DLArray[0].ELC;
	m_VDrawPara.EdDml = pRoad->DLArray[pRoad->DLNum - 1].BLC;
	m_VDrawPara.SPage = 0;
	m_VDrawPara.bzym = false;
	m_VDrawPara.infwgx = true;
	m_VDrawPara.ifDrawCtrH = true;
	m_VDrawPara.dzgkname = L"";	//暂时先不考虑地质描述文件

	//设置五线谱参数
	m_VDrawPara.WXPArray.resize(5);
	for (int i = 0; i < m_VDrawPara.WXPArray.size(); i++)
		m_VDrawPara.WXPArray[i].texth = 5.0;

	m_VDrawPara.WXPArray[0].wxpinf = 1, m_VDrawPara.WXPArray[1].wxpinf = 2, m_VDrawPara.WXPArray[2].wxpinf = 3;
	m_VDrawPara.WXPArray[3].wxpinf = 4; m_VDrawPara.WXPArray[4].wxpinf = 5;
	m_VDrawPara.WXPArray[0].Lht = 20.0, m_VDrawPara.WXPArray[1].Lht = 20.0, m_VDrawPara.WXPArray[2].Lht = 20.0;
	m_VDrawPara.WXPArray[3].Lht = 30.0, m_VDrawPara.WXPArray[4].Lht = 30.0;
}

void RoadAutoDesignDlg::OnBnClickedButtonAutodesVdrawpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_VDrawParaDlg.m_VDrawPara = m_VDrawPara;

	if (m_VDrawParaDlg.DoModal() == IDOK)
		m_VDrawPara = m_VDrawParaDlg.m_VDrawPara;
	SetFocus();
}


void RoadAutoDesignDlg::OnBnClickedButtonSetoutpileattri()
{
	// TODO: 在此添加控件通知处理程序代码
	CSetOutPileAttri PileAttriDlg;
	PileAttriDlg.m_MdbName = m_ProPath;
	PileAttriDlg.m_RoadName = L"主线";
	PileAttriDlg.m_CrossTypeArray = m_CrossTypeArray;
	if (PileAttriDlg.DoModal() == IDOK)
		m_CrossTypeArray = PileAttriDlg.m_CrossTypeArray;
}


void RoadAutoDesignDlg::SetCrossPara()
{
	m_CrossDrawPara.EarthWidL = 5.0;
	m_CrossDrawPara.EarthWidR = 5.0;
	m_CrossDrawPara.IsDrawEarthPile = false;
	m_CrossDrawPara.IsDrawGrid = true;
	m_CrossDrawPara.Scale = 200.0;
}

void RoadAutoDesignDlg::OnBnClickedButtonAutodesSetcrosspara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CrossDrawParaDlg.m_DrawPara = m_CrossDrawPara;

	if (m_CrossDrawParaDlg.DoModal() == IDOK)
		m_CrossDrawPara = m_CrossDrawParaDlg.m_DrawPara;
	SetFocus();
}


void RoadAutoDesignDlg::OnBnClickedButtonAutodesOvalpara()
{
	// TODO: 在此添加控件通知处理程序代码
	m_OvalDrawParaDlg.m_DrawOvalPara = m_OvalDrawPara;

	if (m_OvalDrawParaDlg.DoModal() == IDOK)
		m_OvalDrawPara = m_OvalDrawParaDlg.m_DrawOvalPara;
	SetFocus();
}


void RoadAutoDesignDlg::OnBnClickedPlct()
{
	// TODO: 在此添加控件通知处理程序代码
	//20200322梁柱，隐藏浏览按钮
	if (m_IF_PLCT==0)
	{
		m_IF_PLCT = 1;
		GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_Sml)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_Eml)->ShowWindow(false);
	}
	else
	{
		m_IF_PLCT = 0;
		GetDlgItem(IDC_BUTTON_AUTODES_BRO)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_Sml)->ShowWindow(true);
		GetDlgItem(IDC_EDIT_Eml)->ShowWindow(true);
	}
	
}
