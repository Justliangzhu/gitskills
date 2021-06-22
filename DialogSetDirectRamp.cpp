// DialogSetDirectRamp.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "DialogSetDirectRamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSetDirectRamp dialog

CDialogSetDirectRamp::CDialogSetDirectRamp(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSetDirectRamp::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CDialogSetDirectRamp)
	m_EditChangedata = _T("25");
	m_EditRampTriMiles = _T("");
	m_EditConnectLen = _T("40");
	m_EditDRampSMiles = _T("");
	m_EditDRampEMiles = _T("");
	m_EditDRampLen = _T("");
	m_EditDRampERad = _T("");
	m_AorL = _T("A");
	m_HLinkModel = 0;
	m_dA1L1 = 1.0;
	m_dA2L2 = 1.0;
	m_dA3L3 = 1.0;
	m_dA4L4 = 1.0;
	m_strW1234 = _T("6.625 10.125 9 2.25");
	m_fldR = _T("");
	m_fldA = _T("");
	m_dOffsetW = 0.0;
	//}}AFX_DATA_INIT

	for(int i=0; i<11; i++)
	{
		XYId[i] = NULL;
	}

	W1 = 6.625;
	W2 = 10.125;
	W3 = 9.0;
	W4 = 2.25;

	A1 = 1.0;
	A2 = 1.0;
	A3 = 1.0;
	A4 = 1.0;

	m_RampPosLeftOrRight = 0;
	m_RampAddOrSub = 0;

	MainVect = 0;
	RampVect = 0;

	SelEntId = NULL;
	ProTriMiles = 0;

	ValidItemType =1;

	DRampTriX =0;
	DRampTriY = 0;
	TriSOrient = 0;
	TriRad = 0;

	ProDRampSMiles = 0;
	ProDRampEMiles = 0;

	DRampSDot[Z] = 0;
	DRampEDot[Z] = 0;
	OrignDot[Z] = 0;

	DRampSOrient = 0;
	DRampEOrient = 0;
	DROriOrient = 0;

	SCirX = 0;
	SCirY = 0;

	DRampSRad = 0;
	DRampERad = 0;
	DRampDef = 0;

	TriRampId      = NULL;
	ConnectLineID  = NULL;
	ConnectCirID   = NULL;

	W1Id = NULL;
	W2Id = NULL;
	W3Id = NULL;
	W4Id = NULL;

	DRampCirX = 0;
	DRampCirY = 0;
	DRampCirRad = 0;
	SelCircleId = NULL;

	EndElementId = NULL;
	OffsetElementId = NULL;

	ChangeValue = 0;

	m_bFitConnectLen  = false;
	m_bCirSelAgain    = true;
	m_bConnectDirect  = true;
	m_bDragConnectCir = false;
	m_bJD_CENTER      = false;

	m_iConnectCirXY = 0;

	ItemNum = 0;
	SLength =0;
	SLength1 =0;

	SprialID1 = NULL;
	SprialID2 = NULL;
	m_LinkModel = 0;
	AddCirId = NULL;

	RampJD_CENTER = NULL;
	RampJD_CENTERID = NULL;
	pGottnRamp=NULL;
}

void CDialogSetDirectRamp::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CDialogSetDirectRamp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDialogSetDirectRamp)
	DDX_Text(pDX, IDC_EDITCHANGINGDATA, m_EditChangedata);
	DDX_Text(pDX, IDC_EDITTRIMILES, m_EditRampTriMiles);
	DDX_Text(pDX, IDC_EDITCHANGINGLEN, m_EditConnectLen);
	DDX_Text(pDX, IDC_EDITDRAMPSTARTMILES, m_EditDRampSMiles);
	DDX_Text(pDX, IDC_EDITDRAMPENDMILES, m_EditDRampEMiles);
	DDX_Text(pDX, IDC_EDITDRAMPLEN, m_EditDRampLen);
	DDX_Text(pDX, IDC_EDITENDRAD, m_EditDRampERad);
	DDX_CBString(pDX, IDC_AL, m_AorL);
	DDX_Radio(pDX, IDC_RADIO_TYPE, m_HLinkModel);
	DDX_Text(pDX, IDC_EDITSPRIALA1, m_dA1L1);
	DDX_Text(pDX, IDC_EDITSPRIALA2, m_dA2L2);
	DDX_Text(pDX, IDC_EDITSPRIALA3, m_dA3L3);
	DDX_Text(pDX, IDC_EDITSPRIALA4, m_dA4L4);
	DDX_CBString(pDX, IDC_COMBO_W1234, m_strW1234);
	DDX_Text(pDX, IDC_EDIT_FLD_R, m_fldR);
	DDX_Text(pDX, IDC_EDIT_FLD_A, m_fldA);
	DDX_Text(pDX, IDC_EDIT_OffsetW, m_dOffsetW);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogSetDirectRamp, CDialog)
	//{{AFX_MSG_MAP(CDialogSetDirectRamp)
	ON_BN_CLICKED(IDC_BUTTONTOKNOWMEANING, OnButtontoknowmeaning)
	ON_BN_CLICKED(IDC_BUTTONGETENDRAD, OnButtongetendrad)
	ON_BN_CLICKED(IDC_BUTTONUPDATEDRAMP, OnButtonupdatedramp)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_CBN_SELCHANGE(IDC_AL, OnSelchangeAl)
	ON_CBN_SELENDOK(IDC_AL, OnSelendokAl)
	ON_BN_CLICKED(IDC_BUTTON_IMPLY, OnButtonImply)
	ON_BN_CLICKED(IDC_CHECK_ConnectDirect, OnCHECKConnectDirect)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// CDialogSetDirectRamp message handlers

// 对话框初始化
BOOL CDialogSetDirectRamp::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	// 1. 让用户选取主线单元
	if(!SelectMainElement())
	{
		DestroyWindow();
		return FALSE;
	} 
	m_iSelElem = 1;

	// 2. 让用户选取连接单元(圆或圆弧)
	if(!SelectConnectElement())
	{
		DestroyWindow();
		return FALSE;
	}
	m_iSelElem = 2;

	// 3. 让用户选取终止单元(线路实体、直线、圆、圆弧)
	if(SelectEndElement())
	{
		m_iSelElem = 3;
	}
	else
	{
		// 隐藏与终止控制单元相关的参数设置
		GetDlgItem(IDC_STATICSPRIALA3)->EnableWindow(FALSE); 
		GetDlgItem(IDC_EDITSPRIALA3)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE); 
		GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);

		GetDlgItem(IDC_RADIO_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_C)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OffsetW)->EnableWindow(FALSE);
		//		GetDlgItem(IDC_CHECK_ConnectDirect)->EnableWindow(FALSE);
	}

	// 4. 计算设置界面默认参数	
	if(SelEntId && SelCircleId) // 正确选择了主线与连接圆
	{
		GetCenterSideAndLinkDef(); // 确定各种偏向,并创建契形端起始单元(连接段)
	}
	else
	{
		AfxMessageBox(L"主线或连接圆实体ID出错!\n");
		DestroyWindow();
		return FALSE;
	}

	// 5.按默认值初始设计方案(目前只有2单元初始化,将来考虑3单元初始化)
	if(m_iSelElem == 2)
	{
		ComputerDRampParameters();
	}

	CButton *pBtn = (CButton *)GetDlgItem(IDC_CHECK_ConnectDirect);
	pBtn->SetCheck(1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// "拖动"按钮消息
void CDialogSetDirectRamp::OnButtongetendrad()
{
	// TODO: Add your control notification handler code here
	ShowWindow(FALSE); // 隐藏对话框
	m_bDragConnectCir = true;

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		ShowWindow(TRUE);
		SetFocus();
		return ;
	}

	DragConnectCir(); // 动态拖动连接圆

	acDocManager->unlockDocument(acDocManager->curDocument());	
	UpdateData(FALSE);
	ShowWindow(TRUE);
	m_bDragConnectCir = false;
}

// 根据对话框界面上所得信息计算渐变段长及变速车道参数
void CDialogSetDirectRamp::ComputerDRampParameters()
{
	int i, j, k;

	// 1.计算契形端
	// 判断连接圆圆心位于变速车道的侧向、圆圆的连接偏向(顺时针还是逆时针)
	// 加减速性质、连接圆圆心相对于线路前进方向的侧向
	// 计算渐变段起点坐标, 变速车道起点坐标、半径、方位角(考虑偏转角)
	if(SelEntId && SelCircleId) // 正确选择了主线与连接圆
	{
		if(!GetCenterSideAndLinkDef())
		{
			return;
		}
	}
	else
	{
		AfxMessageBox(L"主线或连接圆实体ID出错!\n");
		return;
	}

	//	// 解算直接式变速车道,并绘制连接缓和曲线段
	//	if(m_bFitConnectLen)
	//	{
	//		if(!CalDRampFitConnectLen()) // 固定连接段长及A值
	//		{
	//			return;
	//		}
	//	}
	//	else
	//	{
	//		if(!CalDRampFitCirCenter()) // 固定连接圆圆心
	//		{
	//			return;
	//		}
	//	}

	// 2.模式法解算连接单元
	double XYArrayTmp[5][10];
	int iXYNum;
	double A[4];
	A[0] = A1;
	A[1] = A2;
	A[2] = A3;
	A[3] = A4;

	if(m_iSelElem == 2) // 2单元模式法解算
	{
		if(m_bFitConnectLen) // 固定连接段长及A值
		{
			// 计算缓和曲线起点坐标及方位角
			if(fabs(ValidItemType-1) < DataPrecision) // 直线
			{	
				m_LinkModel = -1;

				OrignDot[X] = DRampSDot[X] + _wtof(m_EditConnectLen)*sin(DRampSOrient);
				OrignDot[Y] = DRampSDot[Y] + _wtof(m_EditConnectLen)*cos(DRampSOrient);
				DROriOrient = DRampSOrient;
			}
			else
			{	
				AcGePoint3d CenterTmp;
				GetOtherParameterOfArc(DRampSDot[X],DRampSDot[Y],DRampSOrient,DRampSRad,_wtof(m_EditConnectLen),
					DRampDef,CenterTmp[X],CenterTmp[Y],OrignDot[X],OrignDot[Y]);
				DROriOrient = DRampSOrient + DRampDef*_wtof(m_EditConnectLen)/DRampSRad;
			}
			if(!Msf2ElemFitA(StartElementId,SelCircleId,OrignDot,DROriOrient,m_LinkModel,DRampDef,A,XYArrayTmp,iXYNum))
			{
				return;
			}
		}
		else
		{
			if(!Msf2Elem(StartElementId,SelCircleId,m_LinkModel,DRampDef,A,XYArrayTmp,iXYNum))
			{
				return;
			}
		}
	}
	else if(m_iSelElem == 3) // 3单元模式法解算
	{
		// 如果不需偏置,则直接用终止控制单元解算,以便与终止线元直接连接
		// 而且如果终止控制单元是JD_CENTER,则必须偏置,并不参与线元连接
		AcDbObjectId IdTmp; 
		if(m_bConnectDirect)  
		{
			IdTmp = EndElementId;
		}
		else
		{
			IdTmp = OffsetElementId;
		}
		if(!Msf3Elem(StartElementId,SelCircleId,IdTmp,m_LinkModel,m_HLinkModel,DRampDef,A,XYArrayTmp,iXYNum))
		{
			return;
		}
	}

	// 4.组织线元,解算鼻端并绘制标志线W3,W4
	if(!CreateRampJD_CENTER(XYArrayTmp,iXYNum))
	{
		return;
	}
	pGottnRamp->CalNoseCen(RampJD_CENTER,ProDRampSMiles,(int)flag,W3,W4,m_dNoseCmlOnZX,m_dNoseCmlOnYX,m_NosePt);
	AcGePoint3d NosePt(m_NosePt[1],m_NosePt[0],0.0);

	xlpoint NoseOnZXxlpoint, NoseOnYXxlpoint; // 鼻端圆心在主线和右线上的投影点
	NoseOnZXxlpoint.lc = m_dNoseCmlOnZX;
	ProDRampEMiles     = m_dNoseCmlOnZX;
	pGottnRamp->xlpoint_pz(&NoseOnZXxlpoint);
	AcGePoint3d NosePtOnZX(NoseOnZXxlpoint.y, NoseOnZXxlpoint.x, 0.0);
	NoseOnYXxlpoint.lc = m_dNoseCmlOnYX;
	RampJD_CENTER->xlpoint_pz(&NoseOnYXxlpoint);
	AcGePoint3d NosePtOnYX(NoseOnYXxlpoint.y, NoseOnYXxlpoint.x, 0.0);

	makeline(NosePtOnZX,NosePt,W3Id,1);
	makeline(NosePtOnYX,NosePt,W4Id,1);

	// 更新变速车道终点里程和长度
	m_EditDRampEMiles.Format(L"%lf",ProDRampEMiles);
	m_EditDRampLen.Format(L"%lf",fabs(NoseOnYXxlpoint.lc));

	// 更新分流点R,A
	int ItemIndex = RampJD_CENTER->GetXYNoFromTYLC(NoseOnYXxlpoint.lc); // 变速车道起点对应右线的线元号
	double dType = RampJD_CENTER->XYArray[ItemIndex][0]; // 获取所在线元的类型

	if(fabs(dType-1.0) < DataPrecision)
	{
		m_fldR.Format(L"无穷大");
	}
	else
	{
		m_fldR.Format(L"%lf",NoseOnYXxlpoint.r);
	}
	if(dType > 2)
	{
		m_fldA.Format(L"%lf",RampJD_CENTER->XYArray[ItemIndex][1]);
	}
	else
	{
		m_fldA.Format(L"无");
	}

	// 获得A1,A2,A3,A4
	if(m_LinkModel==-1 || m_LinkModel==2)
	{
		A2 = RouteItemArray[2][1];
		if(m_iSelElem == 3)
		{
			m_iConnectCirXY = 3;
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				A3 = RouteItemArray[4][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);
			}
			else
			{
				A3 = RouteItemArray[4][1];
				A4 = RouteItemArray[5][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		A1 = RouteItemArray[2][1];
		A2 = RouteItemArray[3][1];
		if(m_iSelElem == 3)
		{
			m_iConnectCirXY = 4;
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				A3 = RouteItemArray[5][1];
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(FALSE);
			}
			else
			{
				A3 = RouteItemArray[5][1];
				A4 = RouteItemArray[6][1]; 
				GetDlgItem(IDC_STATICSPRIALA4)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDITSPRIALA4)->EnableWindow(TRUE);
			}
		}
	}
	PutA1A2(); // 由统一的A1A2值计算界面输出参数

	//////////////////////////////////////////////////////////////////////////
	// 绘制线元(GrDraw法)

	double len, L, fwj, INDATA[5];
	ads_point pt1, pt2;

	// 先重画(异或)RouteItemArrayOld,删除原来所画线元
	for(i=1; i<m_iXYMax; i++)
	{
		//	if(!m_bDragConnectCir && i>= m_iConnectCirXY)
		//	{
		//		break;
		//	}

		if(RouteItemArrayOld[i][0] > 0.01) // 只画有效的线元,起终点不绘
		{
			/*
			CalXYXY(RouteItemArrayOld[i],1,pt1[Y],pt1[X],fwj);
			len = 0.0;
			for(k=0; k<5; k++) 
			{
			INDATA[k] = RouteItemArrayOld[i][k];
			}
			L = Get_INDATA_L(INDATA,&j);           
			len += 5.0;
			while(len < L)
			{
			CalXYXY(RouteItemArrayOld[i],len,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,-1,0);	
			len += 5.0;
			pt1[X] = pt2[X];
			pt1[Y] = pt2[Y];
			}
			CalXYXY(RouteItemArrayOld[i],L-0.01,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,-1,0);

			AcGePoint3d pt(pt2[Y],pt2[X],0.0);
			double RePt[2][2];
			CalArrowPt(pt,fwj,RePt);
			pt1[X] = RePt[0][1];
			pt1[Y] = RePt[0][0];
			ads_grdraw(pt1,pt2,-1,0);	
			pt1[X] = RePt[1][1];
			pt1[Y] = RePt[1][0];
			ads_grdraw(pt1,pt2,-1,0);*/
			acedRedraw(NULL, 0);
		}
	}

	// 绘制新的RouteItemArray
	for(i=1; i<m_iXYMax; i++)
	{
		if(RouteItemArray[i][0] > 0.01) // 只画有效的线元,起终点不绘
		{
			CalXYXY(RouteItemArray[i],1,pt1[Y],pt1[X],fwj);
			len = 0.0;
			for(k=0; k<5; k++) 
			{
				INDATA[k] = RouteItemArray[i][k];
			}
			L = Get_INDATA_L(INDATA,&j);           
			len += 5.0;
			while(len < L)
			{
				CalXYXY(RouteItemArray[i],len,pt2[Y],pt2[X],fwj);
				acedGrDraw(pt1,pt2,7,0);	
				len += 5.0;
				pt1[X] = pt2[X];
				pt1[Y] = pt2[Y];
			}
			CalXYXY(RouteItemArray[i],L-0.01,pt2[Y],pt2[X],fwj);
			acedGrDraw(pt1,pt2,7,0);

			AcGePoint3d pt(pt2[Y],pt2[X],0.0);
			double RePt[2][2];
			CalArrowPt(pt,fwj,RePt);
			pt1[X] = RePt[0][1];
			pt1[Y] = RePt[0][0];
			ads_grdraw(pt1,pt2,7,0);	
			pt1[X] = RePt[1][1];
			pt1[Y] = RePt[1][0];
			ads_grdraw(pt1,pt2,7,0);

		}
	}

	for(i=1; i<m_iXYMax; i++)
	{
		for(j=0; j<10; j++) 
		{
			RouteItemArrayOld[i][j] = RouteItemArray[i][j];
		}
	}

	UpdateData(FALSE);
}

// 响应确定按钮被点击
void CDialogSetDirectRamp::OnOK() 
{
	// TODO: Add extra validation here
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}

	//	RampJD_CENTERID = AddEntityToDbs(RampJD_CENTER);

	// 写入扩展数据(起终点不写入扩展数据)
	for(int i=1; i<m_iXYMax; i++) 
	{
		SetXYXdata(XYId[i], XYId[i+1], RouteItemArray[i]);
		if(i==m_iXYMax-1 && m_bConnectDirect) // 连接终止控制单元
		{
			SetXYXdata(XYId[i], XYId[0], RouteItemArray[i]);
		}
	}

	/*	AcDb2dPolyline * Poly2x = NULL;
	if (SprialID1)
	{
	if (acdbOpenObject(Poly2x,SprialID1,AcDb::kForWrite) ==  Acad::eOk) 
	{
	Poly2x->erase();		
	Poly2x->close();
	}
	}
	if (SprialID2)
	{
	if (acdbOpenObject(Poly2x,SprialID2,AcDb::kForWrite) ==  Acad::eOk) 
	{
	Poly2x->erase();		
	Poly2x->close();
	}
	}
	*/

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	CDialog::OnOK();
}

// 响应取消按钮被点击
void CDialogSetDirectRamp::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

// 响应帮助按钮被点击
void CDialogSetDirectRamp::OnButtontoknowmeaning() 
{
	// TODO: Add your control notification handler code here
}

// 动态布设直接式变速车道
void CDialogSetDirectRamp::OnButtonupdatedramp() 
{
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}

	if(!GetParameter()) // 获得界面参数并计算设置相关数据
	{
		return;
	}

	int MouseType;             // 鼠标控制类型
	struct resbuf ClickResult; // 结果缓冲区变量
	short KeyCode;             // 定义关键码变量

	for( ; ; ) // 动态设计循环
	{	
		// 设置鼠标拖动控制并进行跟踪;
		if(ads_grread(1,&MouseType,&ClickResult) != RTNORM) 			
		{
			break;
		}
		KeyCode = ClickResult.resval.rint;
		if(KeyCode==13 || KeyCode==32 || MouseType==25) 
		{
			break;
		}
		if(MouseType == 3) // fabs(MouseType-3) < DataPrecision
		{
			break; // 点击左键退出动态设计循环(确认设计成果)
		}
		else if(MouseType == 5) // 跟踪鼠标移动
		{	
			PickDotOld = PickDotNew;
			PickDotNew.x = ClickResult.resval.rpoint[X];
			PickDotNew.y = ClickResult.resval.rpoint[Y];
			PickDotNew.z = ClickResult.resval.rpoint[Z];	
			if(PickDotOld.distanceTo(PickDotNew) > 0.01)
			{	
				//				AcGePoint3d  centerPt(DRampCirX,DRampCirY,0.0);
				//				SetConnectCirCoord(centerPt); // 重新设置连接圆圆心坐标

				ProTriMiles = pGottnRamp->PROJ_ML(PickDotNew.y,PickDotNew.x); // 基点对应线路的统一里程	
				ComputerDRampParameters(); // 计算渐变段及变速车道参数,并绘制图形
			}
		}
		else
		{
			acutPrintf(L"MouseType: %d\n", MouseType);
			break; // 如果别的鼠标键被点击,退出动态设计循环
		}
	}

	Draw(); // 绘制设计线路 

	UpdateData(FALSE);
	acDocManager->unlockDocument(acDocManager->curDocument());
}	

// 计算缓曲线上到指定直线相距为D的点
void CDialogSetDirectRamp::GetEndPointOfDRamp()
{
	//	UpdateData(TRUE);
	//	if (fabs(ValidItemType-1) <  DataPrecision)
	//	{	
	//		ItemNum = 2;
	//		// 计算缓曲的偏距(变速车道终点偏距大于缓曲线起点偏距)
	//		double DistOfTriToDS = GetPreciseLength(DRampTriX,DRampTriY,DRampSDot[X],DRampSDot[Y]);
	//		double DistOfDSToSO = GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]);
	//		double DistOfToAndSo = _wtof(m_EditDRampSDotOffset)-_wtof(m_EditRampTriOffset);
	//		double OffsetOfSprS = (DistOfTriToDS+DistOfDSToSO)*DistOfToAndSo/DistOfTriToDS+_wtof(m_EditRampTriOffset);
	//		double Dist = fabs(_wtof(m_EditDRampEDotOffset)-OffsetOfSprS);
	//		// 获取偏转角的大小
	//		double DefAngle = ChangeValue;		
	//		// 利用循环求得缓曲线上的合适点
	//		double ValueA = _wtof(m_EditParaValueA1);
	//		double LenLimS = Dist;
	//		double LenLimE = 1000;
	//		SLength = (LenLimS+LenLimE)/2;
	//		double AbsX=0, AbsY=0, AbsDist=0;
	//		BOOL ContinueOrnot = TRUE;
	//		int i=0;
	//		while (ContinueOrnot)
	//		{
	//			// 计算给定长度处的缓曲点到直线的距离
	//			AbsX = GetAbsXCorOfSprialCir(ValueA,SLength);
	//			AbsY = GetAbsYCorOfSprialCir(ValueA,SLength);
	//			AbsDist = fabs(AbsY*cos(DefAngle)+AbsX*sin(DefAngle));
	//			// 判断计算距离与要求距离;
	//			if (fabs(AbsDist-Dist) > DataPrecision) 
	//			{
	//				if (AbsDist > Dist) 
	//				{
	//					LenLimE = SLength;
	//				}
	//				else
	//				{
	//					LenLimS = SLength;
	//				}
	//				SLength = (LenLimS+LenLimE)/2;
	//			}
	//			else
	//			{
	//				ContinueOrnot = FALSE;
	//			}
	//		}
	//		double OrignAngle = ConvertOrientToAngle(TriSOrient)+(CCLineDef>0?0:1)*PI;
	//		double InterX,InterY;
	//		InterX = GetAbsXCorOfSprialCir(ValueA,SLength);
	//		InterY = GetAbsYCorOfSprialCir(ValueA,SLength);
	//		DRampEDot[Y] = OrignDot[Y]+InterX*sin(OrignAngle)-CCLineDef*((m_RampPosLeftOrRight)==0?-1:1)*InterY*cos(OrignAngle);
	//		DRampEDot[X] = OrignDot[X]+InterX*cos(OrignAngle)+CCLineDef*((m_RampPosLeftOrRight)==0?-1:1)*InterY*sin(OrignAngle);		
	//		m_EditDRampLen.Format(L"%lf",
	//			SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));	
	//		UpdateData(FALSE);
	//	}
	//	else
	//	{
	//		//double SLength = 0;
	//		double CX1,CY1,X2,Y2;
	//		GetOtherParameterOfArc(TriMainX,TriMainY,TriMainOA,TriMainRad,100,DRampDef,CX1,CY1,X2,Y2);
	//		if (m_LinkModel == 2) //LUAN_MODEL
	//		{	
	//			GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//				DRampERad,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),2,DRampEDot[X],DRampEDot[Y],SLength);
	//			m_EditDRampLen.Format(L"%lf",
	//				SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//		}
	//		else if	(m_LinkModel == 0) //S_MODEL
	//		{
	//			// 判断在那条缓曲上
	//			ads_point InputePoint;
	//			InputePoint[X] = FirstSprialEnd[X];
	//			InputePoint[Y] = FirstSprialEnd[Y];
	//			double SelItemType,InterMiles;
	//			xlpoint RPoint;
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW3 = GetPreciseLength(RPoint.y,RPoint.x,FirstSprialEnd[X],FirstSprialEnd[Y]);
	//			//ads_printf(L"%lf,%lf\n",InterW3,_wtof(m_EditDRampEDotOffset));
	//			if (InterW3 < _wtof(m_EditDRampEDotOffset)) 
	//			{
	//				ItemNum = 2;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,FirstSprialEnd[X],FirstSprialEnd[Y],1E50,FirstSprialEOA,
	//					_wtof(m_EditDRampERad),_wtof(m_EditParaValueA2),-DRampDef,CCLineDef,-_wtof(m_EditDRampEDotOffset),0,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen+SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}
	//			else
	//			{
	//				ItemNum = 1;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//					1E50,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,-_wtof(m_EditDRampEDotOffset),0,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen-SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}			
	//		}
	//		else
	//		{
	//			// 判断在那条缓曲上
	//			ads_point InputePoint;
	//			InputePoint[X] = FirstSprialEnd[X];
	//			InputePoint[Y] = FirstSprialEnd[Y];
	//			double SelItemType,InterMiles;
	//			xlpoint RPoint;
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW3 = GetPreciseLength(RPoint.y,RPoint.x,FirstSprialEnd[X],FirstSprialEnd[Y]);
	//			
	//			InputePoint[X] = SecondSprialEnd[X];
	//			InputePoint[Y] = SecondSprialEnd[Y];
	//			ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	//			double InterW4 = GetPreciseLength(RPoint.y,RPoint.x,SecondSprialEnd[X],SecondSprialEnd[Y]);
	//			if (InterW3 < _wtof(m_EditDRampEDotOffset)) 
	//			{
	//				ItemNum = 2;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,FirstSprialEnd[X],FirstSprialEnd[Y],1E50,FirstSprialEOA,
	//					_wtof(m_EditDRampERad),_wtof(m_EditParaValueA2),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),1,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen+SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}
	//			else
	//			{
	//				ItemNum = 1;
	//				GetCorrectPointOfSprial(2,CX1,CY1,TriMainRad,OrignDot[X],OrignDot[Y],DRampSRad,DROriOrient,
	//					1E50,_wtof(m_EditParaValueA1),DRampDef,CCLineDef,_wtof(m_EditDRampEDotOffset),1,DRampEDot[X],DRampEDot[Y],SLength);
	//				m_EditDRampLen.Format(L"%lf",
	//					FirstSprialLen-SLength+GetPreciseLength(DRampSDot[X],DRampSDot[Y],OrignDot[X],OrignDot[Y]));
	//			}			
	//		}
	//		UpdateData(FALSE);
	//	}

}

// 依据变速车道起点和连接圆半径关系，自动确定连接方式
void CDialogSetDirectRamp::AutoModel()
{	
	double DistO1O2, fwj;
	DistO1O2 = BAS_DRAW_FUN::xyddaa(SCirY, SCirX, DRampCirY, DRampCirX, &fwj);
	double R1 = DRampSRad;
	double R2 = DRampERad;

	if(DistO1O2 < 0.0001)
	{
		AfxMessageBox(L"两圆同心!");
	}
	else if(DistO1O2 < fabs(R1-R2)) 
	{
		m_LinkModel = 2; // 卵
	}
	else if(DistO1O2 < (R1+R2))
	{
		m_LinkModel = 1; // C
	}
	else if(DistO1O2 > (R1+R2))
	{
		m_LinkModel = 0; // S
	}
}

//////////////////////////////////////////////////////////////////////////
// 判断连接圆圆心位于变速车道的侧向、圆圆的连接偏向(顺时针还是逆时针)
// 加减速性质、连接圆圆心相对于线路前进方向的侧向
// 计算渐变段起点坐标, 变速车道起点坐标、半径、方位角(考虑偏转角)
// 2005.07.01 ldf
//////////////////////////////////////////////////////////////////////////
bool CDialogSetDirectRamp::GetCenterSideAndLinkDef()
{		
	// 1.计算渐变段长度及偏转角
	double VerDist = 0;
	VerDist = fabs(W2 - W1);
	m_dTriLen = VerDist * _wtof(m_EditChangedata);

	if(_wtof(m_EditChangedata) < DataPrecision) // 判断数据是否有意义
	{
		acutPrintf(L"警告:渐变率为零!\n");
		return false;		
	}
	ChangeValue = atan(1/_wtof(m_EditChangedata)); // 计算偏转角

	// 2.确定各种偏向、对应半径、方位角
	// 计算基点(渐变段起点)在主线上投影点
	xlpoint RampTrixlpoint; 
	RampTrixlpoint.lc = ProTriMiles;
	m_EditRampTriMiles.Format(L"%lf", ProTriMiles);
	pGottnRamp->xlpoint_pz(&RampTrixlpoint);
	TriMainX   = RampTrixlpoint.y;
	TriMainY   = RampTrixlpoint.x;
	TriMainRad = RampTrixlpoint.r;
	TriMainOA  = RampTrixlpoint.a;
	AcGePoint3d RampTriProjPt(RampTrixlpoint.y, RampTrixlpoint.x, 0.0);

	//	if(!GetConnectCirCoordRad()) // 实时获得连接圆圆心坐标和半径
	//	{
	//		return false;
	//	}


	if(!m_bFitConnectLen || m_bCirSelAgain) // 如果连接段长固定,则相应的偏向亦固定,不需再计算
	{
		m_bCirSelAgain = false;

		// 计算连接圆圆心在主线上的投影点
		xlpoint CirCenxlpoint; 
		CirCenxlpoint.lc = pGottnRamp->PROJ_ML(DRampCirY, DRampCirX); 
		ProDRampCirMiles = CirCenxlpoint.lc;
		pGottnRamp->xlpoint_pz(&CirCenxlpoint);
		AcGePoint3d CirCenProjPt(CirCenxlpoint.y, CirCenxlpoint.x, 0.0); 

		// 连接圆圆心点
		AcGePoint3d CirCenPt(DRampCirX, DRampCirY, 0.0);   

		// 判断连接圆圆心位于变速车道的侧向、圆圆的连接偏向(顺时针还是逆时针)、加减速性质
		CCLineDef = getLRRot(RampTriProjPt, CirCenProjPt, CirCenPt);

		m_RampAddOrSub = -1.0 * CCLineDef; // 右侧减速,左侧加速

		// 判断连接圆圆心相对于线路前进方向的侧向
		if(CirCenxlpoint.lc > RampTrixlpoint.lc)
		{
			flag = 1.0; // 变速车道与主线同向
			m_RampPosLeftOrRight = CCLineDef;	
		}
		else
		{
			flag = -1.0; // 变速车道与主线反向
			m_RampPosLeftOrRight = -1.0 * CCLineDef;
		}
	}

	// 计算变速车道起点在主线上投影点信息
	xlpoint DRampSxlpoint; // 变速车道起点在主线上投影点
	if(fabs(flag-1.0) < DataPrecision) // 变速车道与主线同向
	{	
		DRampSxlpoint.lc = ProTriMiles + m_dTriLen;
	}
	else
	{
		DRampSxlpoint.lc = ProTriMiles - m_dTriLen;
		if(DRampSxlpoint.lc < 0.0)
		{
			ads_printf(L"###: 变速车道起点无法设置,请调整相关参数!\n");
			return false;
		}
	}
	ProDRampSMiles = DRampSxlpoint.lc;
	m_EditDRampSMiles.Format(L"%lf", DRampSxlpoint.lc); // 显示变速车道起始桩号
	pGottnRamp->xlpoint_pz(&DRampSxlpoint);
	DRampSMainX   = DRampSxlpoint.y;
	DRampSMainY   = DRampSxlpoint.x;
	DRampSMainRad = DRampSxlpoint.r;
	DRampSMainOA  = DRampSxlpoint.a;
	int ItemIndex = pGottnRamp->GetXYNoFromTYLC(DRampSxlpoint.lc); // 变速车道起点对应线路的线元号
	ValidItemType = pGottnRamp->XYArray[ItemIndex][0]; // 获取所选线元的类型
	MainValueA    = pGottnRamp->XYArray[ItemIndex][1];

	// 计算变速车道准确的偏向(左/右), 及起点的方位角(考虑偏转角)
	DRampDef = pGottnRamp->XYArray[ItemIndex][4]; // 对应主线单元的偏向
	if(fabs(ValidItemType-1) < DataPrecision) // 主线单元为直线时,原来偏向值为0,所以要特殊处理 
	{
		DRampDef = m_RampPosLeftOrRight;
	}

	DRampSOrient = DRampSMainOA;
	if(flag < 0.0) // 变速车道与主线反向时,其偏向和方位角亦需反向
	{
		DRampSOrient = fmod(DRampSOrient+PI, 2*PI); 
		DRampDef     = -1.0 * DRampDef; 
	}
	DRampSOrient = fmod(DRampSOrient+CCLineDef*ChangeValue, 2*PI);

	// 3.计算控制点并绘制标志线
	// 计算渐变段起点坐标(根据对应主线上投影点相关信息及W1),
	DRampTriX = TriMainX + m_RampPosLeftOrRight * W1 * sin(TriMainOA+PI/2);
	DRampTriY = TriMainY + m_RampPosLeftOrRight * W1 * cos(TriMainOA+PI/2);	

	// 计算变速车道起点坐标(根据对应主线上投影点相关信息及W2)
	DRampSDot[X] = DRampSMainX + m_RampPosLeftOrRight * W2 * sin(DRampSMainOA+PI/2);
	DRampSDot[Y] = DRampSMainY + m_RampPosLeftOrRight * W2 * cos(DRampSMainOA+PI/2);	
	DRampSDot[Z] = 0;

	// 绘制渐变段起点、变速车道起点标志直线(|)
	makeline(RampTriProjPt,AcGePoint3d(DRampTriX,DRampTriY,0),W1Id,1);
	makeline(AcGePoint3d(DRampSMainX,DRampSMainY,0),DRampSDot,W2Id,1);

	// 绘制渐变段连线(拟合线性渐变)
	AcGePoint2dArray vertices;
	AcGePoint2d Dot;
	int ArraySize = vertices.length();
	if(ArraySize > 0) // 清空顶点数组
	{
		vertices.removeSubArray(0, ArraySize-1);
	}
	xlpoint xlpointTmp;
	for(xlpointTmp.lc = RampTrixlpoint.lc; fabs(xlpointTmp.lc-DRampSxlpoint.lc)>2.0; xlpointTmp.lc+=flag*2.0)
	{
		double len = fabs(xlpointTmp.lc-RampTrixlpoint.lc);
		double offset = W1 + VerDist*len/m_dTriLen;
		pGottnRamp->xlpoint_pz(&xlpointTmp); // 主线投影点
		Dot[X] = xlpointTmp.y + m_RampPosLeftOrRight*offset*sin(xlpointTmp.a+PI/2);
		Dot[Y] = xlpointTmp.x + m_RampPosLeftOrRight*offset*cos(xlpointTmp.a+PI/2);	
		vertices.append(Dot);
	} // 利用循环结构初始化顶点数组	
	Dot.set(DRampSDot[X],DRampSDot[Y]); // 将最后一点加入数组中
	vertices.append(Dot);
	makepolyline(vertices, TriRampId, 2); // 黄色

	// 4.创建起始控制单元、确定连接模式、更新对话框选项
	if(fabs(ValidItemType-1) < DataPrecision) // 直线 
	{
		DRampSRad = -1; 

		makeline(DRampSDot,DRampSOrient,0.1,ConnectLineID,0); 
		StartElementId = ConnectLineID;
		m_LinkModel = -1;

		GetDlgItem(IDC_STATICSPRIALA1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDITSPRIALA1)->EnableWindow(FALSE);
	}
	else // 圆或缓和曲线
	{
		DRampSRad = DRampSMainRad; // 与主线保持曲率一致
		// 变速车道起点处的圆心坐标
		SCirX = DRampSDot[X] + DRampSRad*sin(DRampSOrient + DRampDef*PI/2);
		SCirY = DRampSDot[Y] + DRampSRad*cos(DRampSOrient + DRampDef*PI/2);

		makearc(DRampSDot,DRampSOrient,DRampSRad,0.1,DRampDef,ConnectCirID,0);
		StartElementId = ConnectCirID;
		AutoModel(); // 依据变速车道起点圆和连接圆半径关系，自动确定连接方式

		GetDlgItem(IDC_STATICSPRIALA1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDITSPRIALA1)->EnableWindow(TRUE);

		if(!m_bFitConnectLen) // 如果不固定连接段长,则自动初始A值
		{
			// 依据半径的比值，初始A1、A2
			//			double ScaleValue = DRampSRad / DRampCirRad;
			//			if(ScaleValue < 2.0) 
			//			{
			A1 = 1.0;
			A2 = 1.0;
			PutA1A2(); // 由统一的A1A2值计算界面输出参数
			//			}
			//			else
			//			{
			//				m_EditParaValueA1.Format(L"%lf", ScaleValue);
			//				m_EditParaValueA2 = "1";
			//			}
		}
	}	

	UpdateData(FALSE);

	return true;
}

// 圆弧上两点及圆心坐标求此两点间的圆弧长度
double CDialogSetDirectRamp::GetArcLength(double CenX,double CenY,double ArcX1,double ArcY1,
										  double ArcX2,double ArcY2,double ArcRad)
{
	double Angle1 = GetAngleOfLineAndXaxes(CenX,CenY,ArcX1,ArcY1); //S
	double Angle2 = GetAngleOfLineAndXaxes(CenX,CenY,ArcX2,ArcY2); //E
	double Angle = 0;
	if (Angle1 > Angle2) 
	{
		Angle = 2*PI-Angle1+Angle2;
	}
	else
	{
		Angle = Angle2-Angle1;
	}

	return  Angle*ArcRad;
}

// 选取主线单元
bool CDialogSetDirectRamp::SelectMainElement()
{
	ads_name   EntityName; // 所选实体名称
	ads_point  BasePoint;  // 所选实体基点

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	// 提示用户选择主线
	if(ads_entsel(L"\n请选取主线:\n",EntityName,BasePoint) != RTNORM) 
	{
		SelEntId = NULL;
		acutPrintf(L"主线选取失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		DestroyWindow();
		return false;
	}
	if(acdbGetObjectId(SelEntId,EntityName) != Acad::eOk)
	{
		SelEntId = NULL;
		acutPrintf(L"获取主线实体ID失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	GetMainElement(); // 获得主线单元指针

	ProTriMiles = pGottnRamp->PROJ_ML(BasePoint[Y], BasePoint[X]); // 获取基点对应的统一里程

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// 选取连接单元(圆或圆弧)
bool CDialogSetDirectRamp::SelectConnectElement()
{	
	ads_name   EntityName; // 所选实体名称
	ads_point  BasePoint;  // 所选实体基点

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	// 选择变速车道连接圆
	if(ads_entsel(L"\n请选取变速车道连接单元(圆或圆弧):\n",EntityName,BasePoint) != RTNORM) 
	{
		SelCircleId = NULL;
		acutPrintf(L"邻近圆或圆弧选择失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	if(acdbGetObjectId(SelCircleId,EntityName) != Acad::eOk)
	{
		SelCircleId = NULL;
		acutPrintf(L"获取实体对象ID失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(!GetConnectCirCoordRad()) // 获得连接圆圆心坐标和半径
	{
		return false;
	}

	m_bCirSelAgain = true;

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// 选取终止控制单元(线路实体、直线、圆或圆弧)(不选) 2005.07.19 ldf
bool CDialogSetDirectRamp::SelectEndElement()
{
	ads_name   EntityName; // 所选实体名称
	int        resultCode; // 结果码

	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	resultCode = ads_entsel(L"\n选取终止控制单元(线路实体、直线、圆或圆弧)(不选) : ",EntityName, PickDotEnd);
	if(resultCode == RTCAN || resultCode != RTNORM)
	{
		EndElementId = NULL;
		acutPrintf(L"终止控制单元没有选择或选择失败!\n");
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	if(acdbGetObjectId(EndElementId,EntityName) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"获取终止控制单元ID失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	GetOffsetElement(); // 根据偏置值,实时获得终止偏置单元	

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());

	return true;
}

// 根据偏置值,实时获得终止偏置单元
bool CDialogSetDirectRamp::GetOffsetElement()
{
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return false;
	}

	// 1.根据选取的终止单元类型,创建偏置的终止单元
	AcDbEntity * pEntity = NULL;
	if(acdbOpenObject(pEntity,EndElementId,AcDb::kForRead) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"终止单元打开失败,释放实体ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(_tcscmp(pEntity->isA()->name(),L"JD_CENTER") == 0) 
	{
		// JD_CENTER不参与线元连接
		m_bJD_CENTER = true;
		m_bConnectDirect = false;
		GetDlgItem(IDC_CHECK_ConnectDirect)->EnableWindow(FALSE); 

		JD_CENTER* pEndJD_CENTER;
		pEndJD_CENTER = JD_CENTER::cast(pEntity); 
		double EndProMiles = pEndJD_CENTER->PROJ_ML(PickDotEnd[Y], PickDotEnd[X]);	
		xlpoint xlpoint_End; 
		xlpoint_End.lc = EndProMiles;
		pEndJD_CENTER->xlpoint_pz(&xlpoint_End);
		DRampSMainRad = xlpoint_End.r;
		double dFwj  = xlpoint_End.a;
		int ItemIndex = pEndJD_CENTER->GetXYNoFromTYLC(xlpoint_End.lc); // 线元号
		double ItemType = pEndJD_CENTER->XYArray[ItemIndex][0]; // 线元的类型
		double LeftOrRight = pEndJD_CENTER->XYArray[ItemIndex][4]; // 线元的偏向

		if(fabs(ItemType-1.0) < DataPrecision)
		{
			AcGePoint3d startPt(xlpoint_End.y,xlpoint_End.x,0.0);
			makeline(startPt,dFwj,0.1,OffsetElementId,0);
		}
		else if(ItemType > 1.0)
		{
			AcGePoint3d startPt(xlpoint_End.y,xlpoint_End.x,0.0);
			makearc(startPt,dFwj,xlpoint_End.r,0.1,LeftOrRight,OffsetElementId,0);
		}
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEntity);
		AcGePoint3d centerPt;
		centerPt[X] = (pCircle->center())[X];
		centerPt[Y] = (pCircle->center())[Y];
		double R = pCircle->radius();
		makecircle(centerPt,R,OffsetElementId,0);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{	
		AcDbArc *pArc = AcDbArc::cast(pEntity);
		AcGePoint3d centerPt;
		centerPt[X] = (pArc->center())[X];
		centerPt[Y] = (pArc->center())[Y];
		double R = pArc->radius();
		makecircle(centerPt,R,OffsetElementId,0);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbLine") == 0) 
	{
		AcDbLine *pLine = AcDbLine::cast(pEntity);
		AcGePoint3d sPt,ePt;
		sPt = pLine->startPoint();
		ePt = pLine->endPoint();

		makeline(sPt,ePt,OffsetElementId,0);
	}
	else
	{
		pEntity->close(); 	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	pEntity->close(); 	

	// 2.按偏置值,修改终止偏置单元
	if(acdbOpenObject(pEntity,OffsetElementId,AcDb::kForWrite) != Acad::eOk)
	{
		EndElementId = NULL;
		acutPrintf(L"终止单元打开失败,释放实体ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *pCircle = AcDbCircle::cast(pEntity);
		double R = pCircle->radius();
		R = R + m_dOffsetW;
		pCircle->setRadius(R);
		m_dEndElemR = R;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{	
		AcDbArc *pArc = AcDbArc::cast(pEntity);
		double R = pArc->radius();
		R = R + m_dOffsetW;
		pArc->setRadius(R);
		m_dEndElemR = R;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbLine") == 0) 
	{
		AcDbLine *pLine = AcDbLine::cast(pEntity);
		AcGePoint3d sPt, ePt;
		sPt = pLine->startPoint();
		ePt = pLine->endPoint();

		double dFwj = fwj(sPt, ePt);
		dFwj = dFwj + SIGN(m_dOffsetW)*0.5*PI;

		AcGePoint3d sPtNew, ePtNew;
		getLineEndPt(sPt,fabs(m_dOffsetW),dFwj,sPtNew);
		getLineEndPt(ePt,fabs(m_dOffsetW),dFwj,ePtNew);
		pLine->setStartPoint(sPtNew);
		pLine->setEndPoint(ePtNew);

		m_dEndElemR = 1e40;
	}
	else
	{
		pEntity->close(); 	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}
	pEntity->close(); 	

	// 释放CAD文档
	acDocManager->unlockDocument(acDocManager->curDocument());
	return true;
}

// 创建变速车道线路实体 2005.07.04 ldf
bool CDialogSetDirectRamp::CreateRampJD_CENTER(double XYArrayTmp[][10],int iXYNum)
{
	int i, j;

	// 1.组织线元数据
	// 起点线元
	m_iXYMax = 0; 
	RouteItemArray[0][0] = 0; 
	RouteItemArray[0][1] = DRampSDot[Y];
	RouteItemArray[0][2] = DRampSDot[X];
	RouteItemArray[0][3] = DRampSOrient;
	RouteItemArray[0][4] = 0;

	// 连接段线元(不包括渐变段),先计算其长度
	double dConnectLen;
	OrignDot[X] = XYArrayTmp[0][9];
	OrignDot[Y] = XYArrayTmp[0][8];
	DROriOrient = XYArrayTmp[0][5];

	//  
	if(!CheckSprial())
	{
		return false;
	}	

	if(fabs(ValidItemType-1) < DataPrecision)
	{
		dConnectLen = DRampSDot.distanceTo(OrignDot);
	}
	else
	{
		dConnectLen = fabs(DROriOrient-DRampSOrient) * DRampSRad;
	}
	if(!m_bFitConnectLen) // 在不固定的情况下,连接段长由计算复出
	{
		m_EditConnectLen.Format(L"%lf", dConnectLen); // 更新连接段长
	}

	m_iXYMax++;
	if(fabs(ValidItemType-1) < DataPrecision)
	{
		RouteItemArray[1][0] = 1; // 直线段
		RouteItemArray[1][1] = dConnectLen;
		RouteItemArray[1][2] = 0;
		RouteItemArray[1][3] = 0;
		RouteItemArray[1][4] = 0;
		RouteItemArray[1][5] = DRampSOrient;
		RouteItemArray[1][6] = 0;
		RouteItemArray[1][7] = 1e40;
		RouteItemArray[1][8] = DRampSDot[Y];
		RouteItemArray[1][9] = DRampSDot[X];
	}
	else if(ValidItemType > 1)
	{
		RouteItemArray[1][0] = 2; // 圆曲线段
		RouteItemArray[1][1] = DRampSRad;
		RouteItemArray[1][2] = dConnectLen;
		RouteItemArray[1][3] = 0;
		RouteItemArray[1][4] = DRampDef;
		RouteItemArray[1][5] = DRampSOrient;
		RouteItemArray[1][6] = 0;
		RouteItemArray[1][7] = DRampSRad;
		RouteItemArray[1][8] = DRampSDot[Y];
		RouteItemArray[1][9] = DRampSDot[X];
	}

	// 中间解算线元
	for(i=0; i<iXYNum; i++)
	{
		m_iXYMax++;
		for(j=0; j<10; j++)
		{
			RouteItemArray[m_iXYMax][j] = XYArrayTmp[i][j];
			if(j == 6)
			{
				RouteItemArray[m_iXYMax][j] += dConnectLen;
			}
		}
	}

	// 终点线元
	double INDATA[5], len;

	m_iXYMax++;
	for(i=0; i<5; i++)
	{
		INDATA[i] = RouteItemArray[m_iXYMax-1][i];
	}
	RouteItemArray[m_iXYMax][0] = 0;
	RouteItemArray[m_iXYMax][1] = 0;
	RouteItemArray[m_iXYMax][2] = 0; 
	RouteItemArray[m_iXYMax][3] = 0; 
	RouteItemArray[m_iXYMax][4] = RouteItemArray[m_iXYMax-1][4]; 

	len = Get_INDATA_L(INDATA,&j);
	RouteItemArray[m_iXYMax][6] = RouteItemArray[m_iXYMax-1][6] + len;
	RouteItemArray[m_iXYMax][7] = RouteItemArray[m_iXYMax-1][7];
	CalXYXY(RouteItemArray[m_iXYMax-1],len,RouteItemArray[m_iXYMax][8],RouteItemArray[m_iXYMax][9],RouteItemArray[m_iXYMax][5]);

	// 2.组织匝道JD_CENTER数据
	if(RampJD_CENTER == NULL)
	{
		RampJD_CENTER = new JD_CENTER;
	}
	RampJD_CENTER->XYArray = RouteItemArray;
	RampJD_CENTER->XYNum   = m_iXYMax; // 最大线元号(线元数-1)	

	double endml  = RampJD_CENTER->XYArray[m_iXYMax][6];
	XYId[m_iXYMax]  = SelCircleId; // 连接线元 

	RampJD_CENTER->IfCalXY = false;
	RampJD_CENTER->SHOWJD  = false;
	RampJD_CENTER->setDLM(0,0.0,0.0,L"K",L"K");
	RampJD_CENTER->setDLM(1,endml,endml,L"K",L"K");
	//	RampJD_CENTER->XYArrayToJD(); // 如果要绘制交点连线,则必须计算

	return true;
}

void CDialogSetDirectRamp::OnCheck1() 
{
	// TODO: Add your control notification handler code here
	m_bFitConnectLen = !m_bFitConnectLen;
	if(m_bFitConnectLen) // 如果固定连接段长,则禁止拖动连接圆
	{
		GetDlgItem(IDC_BUTTONGETENDRAD)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_BUTTONGETENDRAD)->EnableWindow(TRUE);
	}
}

// 检查缓和曲线的合理性
bool CDialogSetDirectRamp::CheckSprial()
{
	// 判断缓曲线起点是否合理(!!!编写一个函数专门进行规范判断)
	ads_point InputePoint;
	xlpoint RPoint;
	double SelItemType,InterMiles;
	InputePoint[X] = OrignDot[X];
	InputePoint[Y] = OrignDot[Y];
	ComputerMilesAndCorsOfRoute(SelEntId,InputePoint,SelItemType,InterMiles,RPoint);
	double deltaMiles1 = flag * (InterMiles - _wtof(m_EditDRampSMiles));
	double deltaMiles2 = flag * (ProDRampCirMiles - InterMiles);
	if(deltaMiles1 < 0.0)
	{
		acutPrintf(L"\n###: 缓曲线起点越过变速车道起点,请调整相关参数!\n");
		return false;
	}
	if(deltaMiles2 < 0.0)
	{
		acutPrintf(L"\n###: 缓曲线起点越过连接圆切点,请调整相关参数!\n");
		return false;
	}

	return true;
}

// 实时获得连接圆圆心坐标和半径
bool CDialogSetDirectRamp::GetConnectCirCoordRad()
{
	// 打开所获取的实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForRead) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// 获取其半径及圆心坐标
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		DRampCirX = (m_pCircle->center())[X];
		DRampCirY = (m_pCircle->center())[Y];
		DRampCirRad = m_pCircle->radius();
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		DRampCirX = (m_pArc->center())[X];
		DRampCirY = (m_pArc->center())[Y];
		DRampCirRad = m_pArc->radius();
	}
	else
	{
		acutPrintf(L"所选实体不是圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	DRampERad = DRampCirRad;
	m_EditDRampERad.Format(L"%lf", DRampCirRad);

	return true;
}

// 设置连接圆半径
bool CDialogSetDirectRamp::SetConnectCirRad(double dRadius)
{
	// 打开所获取的实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// 获取其半径及圆心坐标
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		m_pCircle->setRadius(dRadius);
		DRampCirRad = dRadius;
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		m_pArc->setRadius(dRadius);
		DRampCirRad = dRadius;
	}
	else
	{
		acutPrintf(L"所选实体不是圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	DRampERad = DRampCirRad;

	return true;
}

// 设置连接圆圆心坐标
bool CDialogSetDirectRamp::SetConnectCirCoord(AcGePoint3d centerPt)
{
	// 打开所获取的实体
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelCircleId,AcDb::kForWrite) != Acad::eOk)
	{
		acutPrintf(L"实体打开失败!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// 设置其圆心坐标
	if(_tcscmp(pEntity->isA()->name(),L"AcDbCircle") == 0) 
	{
		AcDbCircle *m_pCircle = AcDbCircle::cast(pEntity);
		m_pCircle->setCenter(centerPt);
	}
	else if(_tcscmp(pEntity->isA()->name(),L"AcDbArc") == 0) 
	{
		AcDbArc *m_pArc = AcDbArc::cast(pEntity);
		m_pArc->setCenter(centerPt);
	}
	else
	{
		acutPrintf(L"所选实体不是圆或圆弧!\n");
		return false;
	}
	pEntity->close(); // 关闭所选实体

	return true;
}

// 实时获得主线单元
bool CDialogSetDirectRamp::GetMainElement()
{
	// 打开所获取的实体, 根据所得基点获取桩号值
	AcDbEntity * pEntity = NULL;
	if (acdbOpenObject(pEntity,SelEntId,AcDb::kForRead) != Acad::eOk)
	{
		SelEntId = NULL;
		acutPrintf(L"实体打开失败,释放实体ID!\n");	
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	// 判断所选实体是否为线路实体
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER") != 0) 
	{
		SelEntId = NULL;
		acutPrintf(L"所选实体不是线路实体,释放实体ID!\n");
		pEntity->close();
		acDocManager->unlockDocument(acDocManager->curDocument());
		return false;
	}

	pGottnRamp  = JD_CENTER::cast(pEntity); // 获取主线实体指针
	pEntity->close(); // 关闭所选实体

	return true;
}

// 更改缓和曲线参数类型(效果同下)
void CDialogSetDirectRamp::OnSelchangeAl() 
{
	//	// TODO: Add your control notification handler code here
	//	GetDlgItemText(IDC_AL,m_AorL);
	//	
	//	if(m_AorL == "A")
	//	{
	//		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"A2");
	//		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"A2");
	//	}
	//	else
	//	{
	//		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"L1");
	//		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"L2");
	//	}
	//
	//	UpdateData(FALSE);

}

// 更改缓和曲线参数类型(效果同上)
void CDialogSetDirectRamp::OnSelendokAl() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_AorL == "A")
	{
		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"A2");
		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"A2");
		GetDlgItem(IDC_STATICSPRIALA3)->SetWindowText(L"A3");
		GetDlgItem(IDC_STATICSPRIALA4)->SetWindowText(L"A4");
	}
	else
	{
		GetDlgItem(IDC_STATICSPRIALA1)->SetWindowText(L"L1");
		GetDlgItem(IDC_STATICSPRIALA2)->SetWindowText(L"L2");
		GetDlgItem(IDC_STATICSPRIALA3)->SetWindowText(L"L3");
		GetDlgItem(IDC_STATICSPRIALA4)->SetWindowText(L"L4");
	}

	PutA1A2(); // 由统一的A1A2值计算界面输出参数

	UpdateData(FALSE);
}

// 由界面输入参数获得统一的A1A2值
void CDialogSetDirectRamp::GetA1A2()
{
	if(m_AorL == "A") // 如果输入的A值,则直接使用
	{
		A1 = m_dA1L1;
		A2 = m_dA2L2;
	}
	else // 如果输入的L值,则需转换成A值,以便统一运算: A = sqrt(L * R) 
	{
		A1 = sqrt(m_dA1L1 * DRampSRad); 
		if(m_LinkModel == 2) // 卵型A值需特别计算
		{
			A2 = sqrt(m_dA2L2*DRampSRad*DRampCirRad / fabs(DRampSRad-DRampCirRad));
		}
		else
		{
			A2 = sqrt(m_dA2L2 * DRampCirRad);
		}
	}
}

// 由统一的A1A2值计算界面输出参数
void CDialogSetDirectRamp::PutA1A2()
{
	if(m_AorL == "A")
	{
		m_dA1L1 = A1;
		m_dA2L2 = A2;
		m_dA3L3 = A3;
		m_dA4L4 = A4;
	}
	else // 显示L值: L = A*A / R
	{
		m_dA1L1 = A1*A1 / DRampSRad; 
		if(m_LinkModel == 2) // 卵型L值需特别计算
		{
			m_dA2L2 = A2*A2*fabs(DRampSRad-DRampCirRad) / (DRampSRad*DRampCirRad);
		}
		else
		{
			m_dA2L2 = A2*A2 / DRampCirRad;
		}
		if(m_HLinkModel == 2) // 卵型L值需特别计算
		{
			m_dA3L3 = A3*A3*fabs(m_dEndElemR-DRampCirRad) / (m_dEndElemR*DRampCirRad);
		}
		else
		{
			m_dA3L3 = A3*A3 / DRampCirRad; 
		}
		m_dA4L4 = A4*A4 / m_dEndElemR; 
	}
}

// 获得控制点参数W1,W2,W3,W4
bool CDialogSetDirectRamp::GetW1234()
{
	int nStart = 0, nEnd, nCount;
	CString strTmp[4];

	for(int i=0; i<3; i++) // W1,W2,W3
	{
		nEnd   = m_strW1234.Find(L" ", nStart);
		nCount = nEnd - nStart;
		if(nCount)
		{
			strTmp[i] = m_strW1234.Mid(nStart, nCount);
		}
		else
		{
			return false;
		}
		nStart = nEnd + 1;
	}

	nCount = m_strW1234.GetLength() - nStart; // W4
	if(nCount)
	{
		strTmp[3] = m_strW1234.Mid(nStart, nCount);
	}
	else
	{
		return false;
	}

	W1 = _wtof(strTmp[0]);
	W2 = _wtof(strTmp[1]);
	W3 = _wtof(strTmp[2]);
	W4 = _wtof(strTmp[3]);

	return true;
}

// 应用(按界面参数刷新图形)
void CDialogSetDirectRamp::OnButtonImply() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus(); // 激活锁定CAD文档
	if(acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		ads_printf(L"CAD文档打开错误!\n");
		return ;
	}

	if(!GetParameter()) // 获得界面参数并计算设置相关数据
	{
		return;
	}

	ComputerDRampParameters();

	Draw();

	UpdateData(FALSE);
	acDocManager->unlockDocument(acDocManager->curDocument());
}

// 指定是否直接连接终止单元
void CDialogSetDirectRamp::OnCHECKConnectDirect() 
{
	// TODO: Add your control notification handler code here
	m_bConnectDirect = !m_bConnectDirect;
}

// 获得界面参数并计算设置相关数据 2005.07.21 ldf
bool CDialogSetDirectRamp::GetParameter()
{
	UpdateData(TRUE);

	GetMainElement(); // 实时获得主线单元指针
	ProTriMiles = _wtof(m_EditRampTriMiles);
	GetA1A2();        // 由界面输入参数获得统一的A1A2值
	if(!GetW1234())   // 获得控制点参数W1,W2,W3,W4
	{
		AfxMessageBox(L"控制点参数设置有误!");
		return false;
	}

	if(fabs(_wtof(m_EditDRampERad)-DRampCirRad) > DataPrecision) 
	{
		if(!SetConnectCirRad(_wtof(m_EditDRampERad))) // 重新设置连接圆半径
		{
			acutPrintf(L"设置连接圆半径出错!\n");
			return false;
		}
	}

	if(!GetConnectCirCoordRad()) // 实时获得连接圆圆心坐标和半径
	{
		return false;
	}

	if(m_iSelElem == 3)
	{
		if(!GetOffsetElement()) // 根据偏置值,获得终止偏置单元
		{
			acutPrintf(L"获得终止偏置单元出错!\n");
			return false;
		}
	}

	UpdateData(FALSE);
	return true;
}

// 动态拖动连接圆
bool CDialogSetDirectRamp::DragConnectCir()
{
	if(!GetParameter()) // 获得界面参数并计算设置相关数据
	{
		return false;
	}

	int resultCode; 
	ads_name     EntityName;
	AcDbObjectId SelId;
	AcGePoint3d  PickDotCir;
	AcGePoint3d  mousePtOld, mousePtNew;
	double       deltaX, deltaY;

	int    MouseType;         // 鼠标控制类型
	short  KeyCode;           // 定义关键码变量
	struct resbuf resbufTmp;  // 结果缓冲区变量

	resultCode = ads_entsel(L"\n请选择连接单元: ", EntityName, asDblArray(PickDotCir));
	if(resultCode == RTCAN)
	{
		return false;
	}
	if(resultCode != RTNORM) 
	{
		ads_printf(L"连接单元选择有误!\n");
		return false;
	}       

	if(acdbGetObjectId(SelId,EntityName) != Acad::eOk)
	{
		acutPrintf(L"获取连接单元ID失败!\n");	
		return false;
	}

	if(SelId != SelCircleId)
	{
		ads_printf(L"连接单元选择有误!\n");
		return false;
	}

	UpdateData(FALSE); // 重新弹出对话框,实时查看数据
	ShowWindow(TRUE);

	mousePtOld = PickDotCir;
	for(;;)
	{	
		if(ads_grread(1,&MouseType,&resbufTmp) != RTNORM) 			
		{
			break;
		}	
		KeyCode = resbufTmp.resval.rint;
		if(KeyCode==13 || KeyCode==32 || MouseType==25) 
		{
			break;
		}
		if(MouseType == 3) 
		{
			break; // 点击左键退出动态设计循环(确认设计成果)
		}
		if((MouseType!=5) && (MouseType!=12))
		{
			ads_printf(L"必须用鼠标!\n");
			break;
		}
		if(MouseType == 5)
		{
			mousePtNew[X] = resbufTmp.resval.rpoint[X];
			mousePtNew[Y] = resbufTmp.resval.rpoint[Y];
			mousePtNew[Z] = 0.0;

			deltaX = mousePtNew[X] - mousePtOld[X];
			deltaY = mousePtNew[Y] - mousePtOld[Y];

			DRampCirX += deltaX;
			DRampCirY += deltaY;
			mousePtOld = mousePtNew;

			AcGePoint3d  centerPt(DRampCirX,DRampCirY,0.0);
			SetConnectCirCoord(centerPt); // 重新设置连接圆圆心坐标
			ComputerDRampParameters();    // 重新解算连接
		}
	}

	Draw(); // 绘制设计线路 

	return true;
}

// 绘制设计线路 2005.07.22 ldf
// XYId[0]记录终止控制单元ID,当要连接终止单元时可作为最后一个线元的后接ID传入
// XYId[1]-->XYId[6]存储最后整理的ID号,用于写入扩展数据,下标对应线元号,
// 由于每次写入数据时要用传入后接线元ID,所以会用到XYId[7],通常为Null
// XYId[8]-->XYId[10]是临时线元ID,
// XYId[8]是圆弧(连接圆段)ID,其它是Polyline(缓曲线)ID
void CDialogSetDirectRamp::Draw()                  
{
	// 记录终止控制单元ID	
	if(m_iSelElem == 3)
	{	
		if(m_bConnectDirect)
		{
			XYId[0] = EndElementId; 
		}
		else
		{
			XYId[0] = OffsetElementId; 
		}
	}
	else
	{
		XYId[0] = SelCircleId; 
	}

	// 绘制连接段
	if(fabs(ValidItemType-1) < DataPrecision)
	{
		DrawXY(RouteItemArray[1],ConnectLineID,0);
		XYId[1] = ConnectLineID;
	}
	else if(ValidItemType > 1)
	{
		DrawXY(RouteItemArray[1],ConnectCirID,0);
		XYId[1] = ConnectCirID;
	}

	// 绘制缓和曲线
	if(m_LinkModel==-1 || m_LinkModel==2)
	{
		DrawXY(RouteItemArray[2],SprialID1,1);
		XYId[2] = SprialID1;
		A2 = RouteItemArray[2][1];

		if(m_iSelElem == 3)
		{
			DrawXY(RouteItemArray[3],XYId[8],2);
			m_iConnectCirXY = 3;
			XYId[3] = XYId[8];
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				DrawXY(RouteItemArray[4],XYId[9],1);
				XYId[4] = XYId[9];
				XYId[5] = NULL;
				A3 = RouteItemArray[4][1];
			}
			else
			{
				DrawXY(RouteItemArray[4],XYId[9],1);
				DrawXY(RouteItemArray[5],XYId[10],1);
				XYId[4] = XYId[9];
				XYId[5] = XYId[10];
				XYId[6] = NULL;
				A3 = RouteItemArray[4][1];
				A4 = RouteItemArray[5][1];
			}
		}
		else
		{
			XYId[3] = NULL;
		}
	}
	else
	{
		DrawXY(RouteItemArray[2],SprialID1,1);
		DrawXY(RouteItemArray[3],SprialID2,1);
		XYId[2] = SprialID1;
		XYId[3] = SprialID2;
		A1 = RouteItemArray[2][1];
		A2 = RouteItemArray[3][1];
		if(m_iSelElem == 3)
		{
			DrawXY(RouteItemArray[4],XYId[8],2);
			m_iConnectCirXY = 4;
			XYId[4] = XYId[8];
			if(m_HLinkModel==-1 || m_HLinkModel==2)
			{
				DrawXY(RouteItemArray[5],XYId[9],1);
				XYId[5] = XYId[9];
				XYId[6] = NULL;
				A3 = RouteItemArray[5][1];
			}
			else
			{
				DrawXY(RouteItemArray[5],XYId[9],1);
				DrawXY(RouteItemArray[6],XYId[10],1);
				XYId[5] = XYId[9];
				XYId[6] = XYId[10];
				XYId[7] = NULL;
				A3 = RouteItemArray[5][1];
				A4 = RouteItemArray[6][1]; 
			}
		}
		else
		{
			XYId[4] = NULL;
		}
	}
}


