// SetArcOrCircle.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "SetArcOrCircle.h"
#include "BCTPP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetArcOrCircle dialog


CSetArcOrCircle::CSetArcOrCircle(CWnd* pParent/* =NULL*/)
	: CDialog(CSetArcOrCircle::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetArcOrCircle)
	m_pinajv = 0.0;
	m_biance = 0;
	m_qdpianjiao = 0.0;
	m_qxpzj = 0.0;
	m_qxbj = 0.0;
	m_qxcd = 0.0;
	m_qxzyp = 0;
	m_qdzyp = 0;
	//}}AFX_DATA_INIT
	CreatTOrModifyF = TRUE;
	m_FreeOrNot = FALSE;
	CreatTOrModifyF = TRUE;
	ArcId = NULL;
	CanCreatRampItem = FALSE;
	ComputeOrnot = TRUE;
}


CSetArcOrCircle::CSetArcOrCircle(AcDbObjectId EntityId,CWnd* pParent)
{
	//{{AFX_DATA_INIT(CSetArcOrCircle)
	m_pinajv = 0.0;
	m_biance = 0;
	m_qdpianjiao = 0.0;
	m_qxpzj = 0.0;
	m_qxbj = 0.0;
	m_qxcd = 0.0;
	m_qxzyp = 0;
	m_qdzyp = 0;
	//}}AFX_DATA_INIT
	Vbiance = -1;
	Vpianxiang = -1;
	Vpapianxiang = -1;
	pArc = NULL;
	pCircle = NULL;
	JCId = EntityId;
	GottnEntId = EntityId;
	itemtype = -1;
	CreatTOrModifyF = TRUE;
	m_FreeOrNot = FALSE;
	SelItem = 0;
	ArcId = NULL;
	CanCreatRampItem = FALSE;
	ComputeOrnot = TRUE;
}

void CSetArcOrCircle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetArcOrCircle)
	DDX_Text(pDX, IDC_EDITPIANJV, m_pinajv);
	DDX_Radio(pDX, IDC_RADIOZOUCE, m_biance);
	DDX_Text(pDX, IDC_EDITSAJIAODU, m_qdpianjiao);
	DDX_Text(pDX, IDC_EDITSAPAPINAZHUANJIAO, m_qxpzj);
	DDX_Text(pDX, IDC_EDITSAPAQXBJ, m_qxbj);
	DDX_Text(pDX, IDC_EDITSAPAQXCD, m_qxcd);
	DDX_Radio(pDX, IDC_RADIOPAZOUPIAN, m_qxzyp);
	DDX_Radio(pDX, IDC_RADIOZOUPIAN, m_qdzyp);
	DDX_Check(pDX, IDC_CHECKSAFREESET, m_FreeOrNot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetArcOrCircle, CDialog)
	//{{AFX_MSG_MAP(CSetArcOrCircle)
	ON_BN_CLICKED(IDC_RADIOZOUCE, OnRadiozouce)
	ON_BN_CLICKED(IDC_RADIOYOUCE, OnRadioyouce)
	ON_BN_CLICKED(IDC_RADIOZOUPIAN, OnRadiozoupian)
	ON_BN_CLICKED(IDC_RADIOYOUPIAN, OnRadioyoupian)
	ON_BN_CLICKED(IDC_RADIOPAZOUPIAN, OnRadiopazoupian)
	ON_BN_CLICKED(IDC_RADIOPAYOUPIAN, OnRadiopayoupian)
	ON_BN_CLICKED(IDC_BUTTONSAXIANDIAN, OnButtonsaxiandian)
	ON_BN_CLICKED(IDC_CHECKSAFREESET,OnChecksafreeset)
	ON_BN_CLICKED(IDSAOK, OnSaok)
	ON_BN_CLICKED(IDSACANCEL, OnSacancel)
	ON_BN_CLICKED(IDC_BUTTONSAUPDATE,OnButtonsaupdatedata)
	ON_EN_CHANGE(IDC_EDITSAPAPINAZHUANJIAO,OnChangeEditsapapianzhuanjiao)
	ON_EN_CHANGE(IDC_EDITSAPAQXBJ,OnChangeEditsapaqxbj)
	ON_EN_CHANGE(IDC_EDITSAPAQXCD,OnChangeEditsapaqxcd)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,&OnAcadKeepFocus)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetArcOrCircle message handlers

void CSetArcOrCircle::OnRadiozouce() 
{
	// TODO: Add your control notification handler code here
	Vbiance = -1;
}

void CSetArcOrCircle::OnRadioyouce() 
{
	// TODO: Add your control notification handler code here
	Vbiance = 1;
}

void CSetArcOrCircle::OnRadiozoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = -1;
}

void CSetArcOrCircle::OnRadioyoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = 1;
}

void CSetArcOrCircle::OnRadiopazoupian() 
{
	// TODO: Add your control notification handler code here
	Vpapianxiang = -1;
}

void CSetArcOrCircle::OnRadiopayoupian() 
{
	// TODO: Add your control notification handler code here
	Vpapianxiang = 1;
}

void CSetArcOrCircle::OnButtonsaxiandian() 
{	
	struct resbuf resOSMODE;//设置OSNAP
	resOSMODE.restype=RTSHORT;
	// 激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	if (acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite) != Acad::eOk) 
	{
		return;
	}
	// 获取新数据;
	UpdateData(TRUE);
	 //打开所获取的实体;
	//AcDbEntity * pEntity = NULL;
	//if (acdbOpenObject(pEntity,GottnEntId,AcDb::kForRead)!=Acad::eOk) 
	//{
	//	return;
	//}
	//if (pEntity == NULL)
	//{
	//	return;
	//}

	//// 判断选中实体是否为匝道实体;
	BOOL IsRampRoad = FALSE;
	//BAS_DRAW_FUN BAS1;
	//AcDbObjectId lineId;
	//if (_tcscmp(pEntity->isA()->name(), L"JD_CENTER") == 0)
	//{
	//	BAS1.resetjd_centerid(PT, pEntity, lineId);
	//	IsRampRoad = TRUE;
	//	pEntity->close();
	//	GottnEntId = lineId;
	//}
	////若打开的是一般的缓和曲线，则将其变成直线，进行之后的求解2019.11.4zl
	//if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0) //缓和曲线
	//{
	//	XYarray xray;
	//	int res = GetXdataFromEntity(GottnEntId, xray);
	//	if (res == 0)
	//	{
	//		GottnEntId = NULL;
	//		AcDbPolyline* m_polyline = AcDbPolyline::cast(pEntity);
	//		int numverts = 0;
	//		int i = 0;
	//		int trow = 0;
	//		numverts = m_polyline->numVerts();
	//		AcGePoint3d  location, location1;
	//		for (i = 0; i < numverts; i++)
	//		{  //寻找的某顶点
	//			trow = i;
	//			m_polyline->getPointAt(i, location);
	//			m_polyline->getPointAt(i + 1, location1);
	//			BAS_DRAW_FUN_RAIL bas;
	//			double dXchuizu, dYchuizu;
	//			bas.ChuizuToZx(dXchuizu, dYchuizu, PT.x, PT.y, location.x, location.y, location1.x, location1.y);
	//			if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y))
	//			{
	//				AcDbLine* pl1 = new AcDbLine(location, location1);
	//				GottnEntId = AddEntityToDbs(pl1);
	//				break;
	//			}
	//		}
	//		/*ads_printf(L"选中实体无效!\n");
	//		pEntity->close();*/
	//	}
	//}
	
	/*pEntity->close();*/

	// 将传入实体转化成计算用线路单元;
	if (ComputeOrnot) 
	{				
		if (!GetComItemFromSelEntity(GottnEntId,CompItem))
		{
			return ;
		}	
		if (!GetComItemFromSelEntity(GottnEntId,InterItem,CompItem.ItemDef,FALSE))
		{
			return ;
		}
		ComputeOrnot = FALSE;
	}
	// 获取选中实体类型;
	SelItem = (int)(CompItem.ItemType + DataPrecision);
	// 直线引圆时直线上点的坐标及方位角;
	// CompItem(InterItem)中记录的为直线的终点数据(X,Y,OA);
	double CirSX, CirSY, CirSOrient;
	double ArcCenX, ArcCenY, InRadius, PreDeflect, CirDeflect;


	// 圆引圆时被接圆的圆心坐标,半径及偏向;
	// CompItem中记录的为圆心,半径,圆偏向;
	// InterItem中记录的为圆弧终点数据(X,Y,OA);
		ArcCenX = CompItem.FDotX;
		ArcCenY = CompItem.FDotY;
		InRadius = CompItem.ItemPro;
		PreDeflect = CompItem.ItemDef;
		Vpapianxiang = PreDeflect;
		CirDeflect = Vpapianxiang;

	CirSX = CompItem.FDotX;
	CirSY = CompItem.FDotY;
	CirSOrient = CompItem.FDotOrient;
	

	// 定义缓引圆传入的参数;
	// CompItem记录的为缓曲起点数据,A值,曲偏,起终半径;
	// InterItem中记录的为缓曲终点数据(X,Y,OA); 
	double SprStartEast,SprStartNorth,SprSOrient,SprValueA,SprDeflect,SprStartR;	
	SprStartEast = CompItem.FDotY;
	SprStartNorth = CompItem.FDotX; 
	SprSOrient = CompItem.FDotOrient;
	SprValueA = CompItem.ItemPro;
	SprDeflect = CompItem.ItemDef;
	SprStartR = CompItem.ItemStartR;
	SprEndR = CompItem.ItemEndR;

	// 定义缓曲的源点
	double OriX,OriY,OriOA,OriDef;
	int RadForWard = GetRadiusForward(SprStartR,SprEndR);
	//计算新的起点(偏角与偏距,直线的长度重设后),新的直线方位角;
	/*	double SPOffset,SPOffDist;
	if (Vbiance == -1) SPOffset = -1;
	else SPOffset = 1;
	SPOffDist = m_pinajv;

	double SPDeflect,ValueSPDeflect;
	if (Vpianxiang == -1) SPDeflect = -1;
	else SPDeflect = 1;
	ValueSPDeflect = m_qdpianjiao;	
	ModifyStartDot(CirSX,CirSY,SPOffset,SPOffDist,
	CirSOrient,SPDeflect,ValueSPDeflect);*/
	//获取圆上的第一点;
	AcGePoint3d PassDot;
	if (m_FreeOrNot) 
	{
		acedGetPoint(NULL,L"\n请拾取圆曲线一点:\n",asDblArray(PassDot));
	}
	else
	{	
		PassDot[X] = InterItem.FDotX;
		PassDot[Y] = InterItem.FDotY;
		PassDot[Z] = 0.0;
	}
	//定义圆曲单元的另一点;
	AcGePoint3d PickDot;	
	//定义圆心坐标,半径,长度变量;
	double CenX,CenY,CurRadius,CirLen =0;
	//定义鼠标控制类型及对应的获取结果缓冲区变量;
	int MouseType;
	struct resbuf ClickResult;
	//定义关键码变量;
	short KeyCode;
	//进入动态设计循环;
	for( ; ; )
	{	
		acedGetVar(L"autosnap",&resOSMODE);
		short OsnapValue=resOSMODE.resval.rint;

		//设置鼠标拖动控制并进行跟踪;
		if(ads_grread(1,&MouseType,&ClickResult)!=RTNORM) 			
		{
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		KeyCode = ClickResult.resval.rint;
		if(KeyCode == 13||KeyCode == 32||MouseType == 25) 
		{
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		//如果左键被击中;
		if(fabs(MouseType - 3.0) < DataPrecision) 
		{
			break;
		}
		//跟踪鼠标移动;
		else if(MouseType == 5)
		{
			if(OsnapValue>0)//捕捉方式打开
			{
				acedOsnap(		
					ClickResult.resval.rpoint,

					L"endp,int",

					ClickResult.resval.rpoint);

			}

			PickDot.x=ClickResult.resval.rpoint[X];
			PickDot.y=ClickResult.resval.rpoint[Y];
			PickDot.z=ClickResult.resval.rpoint[Z];	

			CanCreatRampItem = TRUE;
			if (SelItem == 1||SelItem == 31) // 前选为Line;
			{
				CirDeflect = 1.0*GetDeflectOfDotToLine(CirSX,CirSY,CirSOrient,PickDot.x,PickDot.y);
				CurRadius = GetRadiusOfLineAndTwoDot(CirSX,CirSY,CirSOrient,PassDot[X],PassDot[Y],
					PickDot.x,PickDot.y,CenX,CenY,CirSX,CirSY,FALSE);
				if (fabs(CurRadius - 1E50) < DataPrecision)
				{	
					ads_printf(L"\n1: 当前圆曲线布设不成功!\n");
					CanCreatRampItem = FALSE;
					continue;
				}
			}
			else if (SelItem == 2||SelItem == 32) // 前选为Arc;
			{
				if (m_FreeOrNot)
				{
					if (!GetCenterCorAndRadius(ArcCenX, ArcCenY, InRadius, PreDeflect, CirDeflect,
						PassDot[X], PassDot[Y], PickDot.x, PickDot.y, CenX, CenY, CurRadius))
					{
						ads_printf(L"\n当前圆曲线布设不成功!\n");
						CanCreatRampItem = FALSE;
						continue;
					}
					GetTangentCorOfTwoCircle(ArcCenX, ArcCenY, InRadius, CenX, CenY, CurRadius, CirSX, CirSY);

				}
				else
				{
					CirDeflect = 1.0 * GetDeflectOfDotToLine(CirSX, CirSY, CirSOrient, PickDot.x, PickDot.y);
					CurRadius = GetRadiusOfLineAndTwoDot(CirSX, CirSY, CirSOrient, PassDot[X], PassDot[Y],
						PickDot.x, PickDot.y, CenX, CenY, CirSX, CirSY, FALSE);
					if (fabs(CurRadius - 1E50) < DataPrecision)
					{
						ads_printf(L"\n1: 当前圆曲线布设不成功!\n");
						CanCreatRampItem = FALSE;
						continue;
					}
				}
			}
			else // 前选为Sprial;
			{	

				if (m_FreeOrNot) // 起点滑动;
				{	
					CurRadius = GetCenCorAndSprialDotCor(SprStartEast,SprStartNorth,SprSOrient,
						SprStartR,SprEndR,SprValueA,SprDeflect,CirDeflect,PickDot.x,PickDot.y,CenX,CenY,CirSX,CirSY);
				}
				else // 起点不滑动;
				{
					if (SprEndR > 8000)
					{
						ads_printf(L"缓曲线终点半径大于8000!\n");
						CirDeflect = 1.0*GetDeflectOfDotToLine(InterItem.FDotX,InterItem.FDotY,InterItem.FDotOrient,PickDot.x,PickDot.y);
						CurRadius = GetRadiusOfLineAndTwoDot(InterItem.FDotX,InterItem.FDotY,InterItem.FDotOrient,PassDot[X],PassDot[Y],
							PickDot.x,PickDot.y,CenX,CenY,CirSX,CirSY,FALSE);
						if (fabs(CurRadius - 1E50) < DataPrecision)
						{	
							ads_printf(L"\n2:当前圆曲线布设不成功!\n");
							CanCreatRampItem = FALSE;
							continue;
						}
					}
					else
					{		
						CurRadius = GetDistOfCenterAndSelDot(SprStartEast,SprStartNorth,SprSOrient,
							SprStartR,SprEndR,SprValueA,SprDeflect,CirDeflect,SprEndR,
							PassDot[X],PassDot[Y],CenX,CenY,CirSX,CirSY);
						//	CurRadius = SprEndR;

						//		CurRadius = GetCenCorAndSprialDotCor(SprStartEast,SprStartNorth,
						//			SprSOrient,SprStartR,SprEndR,SprValueA,SprDeflect,CirDeflect,
						//			PassDot[X],PassDot[Y],CenX,CenY,CirSX,CirSY);

					}
				}				
				// 判断缓曲上引出的圆是否存在;
				if (fabs(CurRadius-1E50) < DataPrecision) 
				{
					ads_printf(L"\n3:当前圆曲线布设不成功!\n");
					CanCreatRampItem = FALSE;
					continue;
				}					
			}
			if (CanCreatRampItem) 
			{	
				ads_printf(L"\n当前圆曲线布设成功!\n");			
				//动态显示当前结果;
				ShowArcOnScreen(CenX,CenY,CurRadius,CirDeflect,CirSX,CirSY,PickDot.x,PickDot.y,ArcId,pArc);							
				//获取圆弧起点变量;
				ArcSX = CirSX;
				ArcSY = CirSY;
				ArcSOA = ConvertAngleToOrient(GetAngleOfLineAndXaxes(CenX,CenY,CirSX,CirSY))
					+ CirDeflect*PI/2;
				ArcRad = CurRadius;
				//显示当前参数;
				/*acdbOpenObject(pArc,ArcId,AcDb::kForRead);*/
				if(acdbOpenObject(pArc,ArcId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pArc->close();
				m_qxbj =  pArc->radius();
				if (pArc->endAngle()>pArc->startAngle()) 
				{
					m_qxpzj = (pArc->endAngle()-pArc->startAngle())*180/PI;

					m_qxcd = CurRadius*(pArc->endAngle()-pArc->startAngle());			
				}
				else
				{
					m_qxpzj = (pArc->endAngle()-pArc->startAngle()+2*PI)*180/PI;
					m_qxcd = CurRadius*(pArc->endAngle()-pArc->startAngle()+2*PI);	
				}
				m_qxbj = ChangeDataPrecision(m_qxbj);
				m_qxpzj = ChangeDataPrecision(m_qxpzj);
				m_qxcd = ChangeDataPrecision(m_qxcd);
				//在对话框上显示圆弧的实际偏向;
				if (fabs(CirDeflect-1) < DataPrecision)
				{
					m_qxzyp = 1;
					Vpapianxiang = 1;
				}
				else 
				{
					m_qxzyp = 0;
					Vpapianxiang = -1;
				}
				UpdateData(FALSE);
			}
		}
		else
		{
			// 如果别的建被点击;
			break;
		}
	}
	if (CanCreatRampItem) 
	{	
		JD_CENTER * pJdCenter = NULL;
		if (IsRampRoad) 
		{		
			/*acdbOpenObject(pJdCenter,GottnEntId,AcDb::kForRead);*/
			if(acdbOpenObject(pJdCenter,GottnEntId, AcDb::kForRead)!=Acad::eOk)
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pJdCenter->close();
		}
		//被加入线元;
		/*acdbOpenObject(pArc,ArcId,AcDb::kForRead);	*/
		if(acdbOpenObject(pArc,ArcId, AcDb::kForRead)!=Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return;
		}
		pArc->close();
		arrxy[1][0] = 2;
		arrxy[1][1] = CurRadius;
		///
		if (pArc->endAngle()>pArc->startAngle()) 
			arrxy[1][2] = CurRadius*(pArc->endAngle()-pArc->startAngle());			
		else
			arrxy[1][2] = CurRadius*(pArc->endAngle()-pArc->startAngle()+2*PI);	
		ArcLen = arrxy[1][2];
		arrxy[1][3] = 0;
		arrxy[1][4] = CirDeflect;	
		///
		if (CirDeflect == -1) 
			arrxy[1][5] = ConvertAngleToOrient(pArc->startAngle()+PI/2);
		else
			arrxy[1][5] = ConvertAngleToOrient(pArc->endAngle()-PI/2);	
		/////////
		arrxy[1][7] = CurRadius;	
		if (CirDeflect == -1) 
		{
			arrxy[1][8] =(pArc->center())[Y]+CurRadius*cos(ConvertAngleToOrient(pArc->startAngle()));
			arrxy[1][9] =(pArc->center())[X]+CurRadius*sin(ConvertAngleToOrient(pArc->startAngle()));
		}
		else
		{
			arrxy[1][8] =(pArc->center())[Y]+CurRadius*cos(ConvertAngleToOrient(pArc->endAngle()));
			arrxy[1][9] =(pArc->center())[X]+CurRadius*sin(ConvertAngleToOrient(pArc->endAngle()));
		}
		// 结束线元;
		arrxy[2][0] = 0;
		arrxy[2][1] = 0;
		arrxy[2][2] = 0;
		arrxy[2][3] = 0;
		arrxy[2][4] = 0;
		if (CirDeflect == -1) 
			arrxy[2][5] = ConvertAngleToOrient(pArc->endAngle()+pi/2);
		else
			arrxy[2][5] = ConvertAngleToOrient(pArc->startAngle()-pi/2);

		arrxy[2][7] = CurRadius;
		if (CirDeflect == -1) 
		{
			arrxy[2][8] =(pArc->center())[Y]+CurRadius*cos(ConvertAngleToOrient(pArc->endAngle()));
			arrxy[2][9] =(pArc->center())[X]+CurRadius*sin(ConvertAngleToOrient(pArc->endAngle()));
		}
		else
		{
			arrxy[2][8] =(pArc->center())[Y]+CurRadius*cos(ConvertAngleToOrient(pArc->startAngle()));
			arrxy[2][9] =(pArc->center())[X]+CurRadius*sin(ConvertAngleToOrient(pArc->startAngle()));
		}
		// 被代替线元;
		if (SelItem == 1||SelItem == 2)
		{
			arrxy[0][0] = 0;
			arrxy[0][1] = arrxy[1][8];
			arrxy[0][2] = arrxy[1][9];
			arrxy[0][3] = arrxy[1][5];
			arrxy[0][4] = arrxy[1][6];
			arrxy[0][5] = 0;
			arrxy[0][6] = 0;
			arrxy[0][8] = 0;
			arrxy[0][7] = 0;
			arrxy[0][9] = 0;
		}
		else
		{
			XYarray xray;
			int res=GetXdataFromEntity(GottnEntId,xray);			
			if(res)//InputEntId有扩展数据
			{
				for(int i=0;i<10;i++)
				{
					arrxy[0][i] = xray.xyarr[i];
				}
			}
			/*
			if(pJdCenter->XYNum>1)
			{
			for(int i=0;i<10;i++)
			{
			arrxy[0][i] = pJdCenter->XYArray[pJdCenter->XYNum-1][i];
			}
			}*/

			if (SelItem == 31) 
			{
				arrxy[0][1] = sqrt((arrxy[0][8]-arrxy[1][8])*(arrxy[0][8]-arrxy[1][8])+
					(arrxy[0][9]-arrxy[1][9])*(arrxy[0][9]-arrxy[1][9]));
				CirLen = arrxy[0][1];
			}
			else if (SelItem == 32)
			{
				double Sfwj = arrxy[0][5],Efwj = arrxy[1][5];
				double zj = ZXJ(Efwj,Sfwj);
				if (zj*arrxy[0][4] < 0) 
					zj = 2*PI-fabs(zj);
				arrxy[0][2] = fabs(arrxy[0][1]*zj);					
				CirLen = arrxy[0][2];
			}
			else if (SelItem == 33)
			{
				// 2005.06.12 hhp
				if (SprEndR > 8000 && !m_FreeOrNot)
				{
					// 终点半径不变					
				}
				else
				{
					arrxy[0][2] = CurRadius;				
					arrxy[0][7] = CurRadius;
				}
				CirLen = arrxy[0][1]*arrxy[0][1]/arrxy[0][2];
			}
			else if (SelItem == 34) 
			{
				// 2005.06.12 hhp
				if (SprEndR > 8000 && !m_FreeOrNot)
				{
					// 终点半径不变	
					CirLen = arrxy[0][1]*arrxy[0][1]*1/arrxy[0][2];
				}
				else
				{
					arrxy[0][0] = 6;
					arrxy[0][3] = CurRadius;				
					CirLen = arrxy[0][1]*arrxy[0][1]*fabs(1/arrxy[0][2]-1/arrxy[0][3]);					
				}
			}
			else 
			{
				// 2005.06.12 hhp
				if (SprEndR > 8000 && !m_FreeOrNot)
				{
					// 终点半径不变						
				}
				else
				{
					arrxy[0][3] = CurRadius;
				}
				CirLen = arrxy[0][1]*arrxy[0][1]*fabs(1/arrxy[0][2]-1/arrxy[0][3]);
			}
		}
		arrxy[1][6] = arrxy[0][6]+CirLen;
		arrxy[2][6] = arrxy[1][6]+arrxy[1][2];
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	ads_printf(L"Command:");
	return ;
}

void CSetArcOrCircle::OnSaok() 
{

	OnButtonsaupdatedata();
	//如果当前不能创建匝道单元;
	if (!CanCreatRampItem)
	{
		AfxMessageBox(L"圆曲线没有布设成功!");
		return ;
	}
	//激活CAD文档;

	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//删除显示的样式圆弧;
	if (acdbOpenObject(pArc,ArcId,AcDb::kForWrite) == Acad::eOk)
	{		
		pArc->erase();
		pArc->close();
	}
	//	if (SelItem == 1||SelItem == 2)
	//	{			
	JD_CENTER *JdCenObj= new JD_CENTER;
	JdCenObj->SetXYArray(arrxy);

	m_XYNum = JdCenObj->XYNum;
	XYarray xray;
	xray.eID=0;
	int i,j;
	for(i=0;i<=m_XYNum;i++)
	{
		for(j=0;j<10;j++)
			xray.xyarr[j]=JdCenObj->XYArray[i][j];
		pXYArr.Add(xray);
	}

	for(i=1;i<=m_XYNum;i++)
	{
		if(pXYArr[i].xyarr[0]<1.1)
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,7);
		else if(pXYArr[i].xyarr[0]>2.1)
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,6);
		else
			DrawXY(pXYArr[i].xyarr,pXYArr[i].eID,2);
	}

	int res=GetXdataFromEntity(GottnEntId,xray);
	if(res)//InputEntId有扩展数据
		SetXYXdata(GottnEntId, pXYArr[1].eID,xray.xyarr);

	SetXYXdata(pXYArr[1].eID,0,pXYArr[1].xyarr);
	//		AddEntityToDbs(JdCenObj);		
	/*
	}
	else
	{
	JD_CENTER *pJC = NULL;	
	acdbOpenObject(pJC,JCId,AcDb::kForWrite);	
	pJC->assertWriteEnabled();	
	pJC->AddXYArray(arrxy);
	pJC->close();
	for(int i=0;i<10;i++)
	{
	ads_printf(L"%lf ",arrxy[1][i]);
	}
	}*/

	acDocManager->unlockDocument(acDocManager->curDocument());		
	CanCreatRampItem = FALSE;
	CDialog::OnOK();
}

void CSetArcOrCircle::OnSacancel() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//删除旧圆弧;
	if (acdbOpenObject(pArc,ArcId,AcDb::kForWrite) == Acad::eOk)
	{		
		pArc->erase();
		pArc->close();
		pArc = NULL;
	}
	acDocManager->unlockDocument(acDocManager->curDocument());	
	SetFocus();	
	CDialog::OnCancel();
}

BOOL CSetArcOrCircle::OnInitDialog() 
{
	CDialog::OnInitDialog();
	///////////////////////////////////
	// 获取新数据;
	// 打开所获取的实体;
	AcDbObject* pEntity = NULL;
	if (acdbOpenObject(pEntity,GottnEntId,AcDb::kForRead)!=Acad::eOk) 
	{
		return true;
	}
	if (pEntity == NULL)
	{
		return true;
	}
	// 判断选中实体是否为匝道实体;
	BOOL IsRampRoad = FALSE;
	BAS_DRAW_FUN BAS1;
	AcDbObjectId lineId;
	if (_tcscmp(pEntity->isA()->name(),L"JD_CENTER")==0)
	{
		BAS1.resetjd_centerid(PT, pEntity, lineId);
		IsRampRoad = TRUE;
		pEntity->close();
		GottnEntId = lineId;
	}
	//若打开的是一般的缓和曲线，则将其变成直线，进行之后的求解2019.11.4zl
	if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0) //缓和曲线
	{
		XYarray xray;
		int res = GetXdataFromEntity(GottnEntId, xray);
		if (res == 0)
		{
			GottnEntId = NULL;
			AcDbPolyline* m_polyline = AcDbPolyline::cast(pEntity);
			int numverts = 0;
			int i = 0;
			int trow = 0;
			numverts = m_polyline->numVerts();
			AcGePoint3d  location, location1;
			for (i = 0; i < numverts; i++)
			{  //寻找的某顶点
				trow = i;
				m_polyline->getPointAt(i, location);
				m_polyline->getPointAt(i + 1, location1);
				BAS_DRAW_FUN_RAIL bas;
				double dXchuizu, dYchuizu;
				bas.ChuizuToZx(dXchuizu, dYchuizu, PT.x, PT.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y) < 0)
				{
					AcDbLine* pl1 = new AcDbLine(location, location1);
					GottnEntId = AddEntityToDbs(pl1);
					break;
				}
			}
			/*ads_printf(L"选中实体无效!\n");
			pEntity->close();*/
		}
	}
	else
	{
		pEntity->close();
	}
	// 将传入实体转化成计算用线路单元;
			
	if (ComputeOrnot)
	{
		if (!GetComItemFromSelEntity(GottnEntId, CompItem))
		{
			return FALSE;
		}
		if (!GetComItemFromSelEntity(GottnEntId, InterItem, CompItem.ItemDef, FALSE))
		{
			return false;
		}
		ComputeOrnot = FALSE;
	}
	// 获取选中实体类型;
	//SelItem = (int)(CompItem.ItemType + DataPrecision);
	//// 直线引圆时直线上点的坐标及方位角;
	//// CompItem(InterItem)中记录的为直线的终点数据(X,Y,OA);

	//// 定义缓引圆传入的参数;
	//// CompItem记录的为缓曲起点数据,A值,曲偏,起终半径;
	//// InterItem中记录的为缓曲终点数据(X,Y,OA); 
	//double SprStartEast,SprStartNorth,SprSOrient,SprValueA,SprDeflect,SprStartR;	
	//SprStartEast = CompItem.FDotY;
	//SprStartNorth = CompItem.FDotX; 
	//SprSOrient = CompItem.FDotOrient;
	//SprValueA = CompItem.ItemPro;
	//SprDeflect = CompItem.ItemDef;
	//SprStartR = CompItem.ItemStartR;
	//SprEndR = CompItem.ItemEndR;
	//// 定义缓曲的源点
	//double OriX,OriY,OriOA,OriDef;
	//int RadForWard = GetRadiusForward(SprStartR,SprEndR);
	//计算新的起点(偏角与偏距,直线的长度重设后),新的直线方位角;

	//if (SelItem == 1||SelItem == 31) // 前选为Line;
	//{


	//}
	//else if (SelItem == 2||SelItem == 32) // 前选为Arc;
	//{
	//}
	//else // 前选为Sprial;
	//{	
	//	m_qxbj = SprEndR;
	//}
	UpdateData(FALSE);
	return TRUE;  
}

afx_msg LRESULT CSetArcOrCircle::OnAcadKeepFocus(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}
void CSetArcOrCircle::OnChecksafreeset() 
{
	// TODO: Add your control notification handler code here
	ClickCout+=1;	
	if (fmod(ClickCout,2)>DataPrecision)
	{
		m_FreeOrNot = TRUE;
	}
	else 
	{
		m_FreeOrNot = FALSE;
	}
}

void CSetArcOrCircle::OnButtonsaupdatedata()
{
	if (!CanCreatRampItem) 
	{
		return ;
	}
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	//定义圆弧起点变量;
	double StartX,StartY,StartOA;
	StartX = ArcSX;
	StartY = ArcSY;
	StartOA = ArcSOA;
	//获取新的数据;
	UpdateData(TRUE);
	double CurCirR,CurCirLen,CurCirDef;
	CurCirR = m_qxbj;
	CurCirLen = m_qxcd;
	//判断圆弧长度的合理性;
	if (CurCirLen > 2*PI* CurCirR - DataPrecision) 
	{
		AfxMessageBox(L"圆弧长度不合理!");
		return ;
	}
	CurCirDef = Vpapianxiang;
	//如果当前圆弧线元的前一线元是缓曲线元;
	if (SelItem==33 || SelItem==34 ||
		SelItem==35 || SelItem==36) 
	{	
		if (SprEndR > 8000 && !m_FreeOrNot) 
		{
			// 缓曲线终点数据不变	
		}
		else
		{			
			RouteDot SPoint,EPoint;
			double SprialLength;
			SPoint.DotX = CompItem.FDotY;
			SPoint.DotY = CompItem.FDotX;
			SPoint.CurRadius = CompItem.ItemStartR;
			SPoint.OrientAngle = CompItem.FDotOrient;
			SPoint.CurMiles = 0;
			if (SelItem == 33)
			{
				SprialLength = CompItem.ItemPro*CompItem.ItemPro/CurCirR;
				arrxy[0][2] = CurCirR;
				arrxy[1][6] = arrxy[0][6] + SprialLength;
			}
			else 
			{				
				SprialLength = CompItem.ItemPro*CompItem.ItemPro*
					fabs(1/CurCirR - 1/CompItem.ItemStartR);
				arrxy[0][3] = CurCirR;
				arrxy[1][6] = arrxy[0][6] + SprialLength;
			}
			ComputerCorOfAnyDotOnItem(SPoint,EPoint,SprialLength,
				CompItem.ItemPro,CurCirR,SprialItem,CompItem.ItemDef);
			StartX = EPoint.DotY;
			StartY = EPoint.DotX;
			StartOA = EPoint.OrientAngle;
		}
	}
	//修改起点信息;
	ModifyStartDot(StartX,StartY,Vbiance,m_pinajv,StartOA,Vpianxiang,m_qdpianjiao);		
	//计算圆心坐标及圆弧的另一点坐标;
	double CurCenX,CurCenY,CurEndX,CurEndY;
	GetOtherParameterOfArc(StartX,StartY,StartOA,CurCirR,CurCirLen,CurCirDef,CurCenX,CurCenY,CurEndX,CurEndY);
	//显示圆弧;
	//动态显示当前结果;
	if(pArc==NULL)
	{
		pArc = new AcDbArc;
		ArcId = BAS_DRAW_FUN::AddEntityToDbs(pArc);
	}
	ShowArcOnScreen(CurCenX,CurCenY,CurCirR,CurCirDef,StartX,StartY,CurEndX,CurEndY,ArcId,pArc);
	//被加入线元;
	if (acdbOpenObject(pArc,ArcId,AcDb::kForRead) != Acad::eOk)
	{
		return  ;
	}
	pArc->close();

	arrxy[1][1] = CurCirR;
	///
	if (pArc->endAngle()>pArc->startAngle()) 
		arrxy[1][2] = arrxy[1][1]*(pArc->endAngle()-pArc->startAngle());			
	else
		arrxy[1][2] = arrxy[1][1]*(pArc->endAngle()-pArc->startAngle()+2*PI);	
	ArcLen = arrxy[1][2];
	arrxy[1][4] = CurCirDef;	
	///
	if (CurCirDef == -1) 
		arrxy[1][5] = ConvertAngleToOrient(pArc->startAngle()+PI/2);
	else
		arrxy[1][5] = ConvertAngleToOrient(pArc->endAngle()-PI/2);	
	/////////
	arrxy[1][7] = CurCirR;	
	if (CurCirDef == -1) 
	{
		arrxy[1][8] =(pArc->center())[Y]+CurCirR*cos(ConvertAngleToOrient(pArc->startAngle()));
		arrxy[1][9] =(pArc->center())[X]+CurCirR*sin(ConvertAngleToOrient(pArc->startAngle()));
	}
	else
	{
		arrxy[1][8] =(pArc->center())[Y]+CurCirR*cos(ConvertAngleToOrient(pArc->endAngle()));
		arrxy[1][9] =(pArc->center())[X]+CurCirR*sin(ConvertAngleToOrient(pArc->endAngle()));
	}
	// 结束线元;
	if (CurCirDef == -1) 
		arrxy[2][5] = ConvertAngleToOrient(pArc->endAngle()+pi/2);
	else
		arrxy[2][5] = ConvertAngleToOrient(pArc->startAngle()-pi/2);
	arrxy[2][6] = arrxy[1][6]+arrxy[1][2];
	arrxy[2][7] = CurCirR;
	if (CurCirDef == -1) 
	{
		arrxy[2][8] =(pArc->center())[Y]+CurCirR*cos(ConvertAngleToOrient(pArc->endAngle()));
		arrxy[2][9] =(pArc->center())[X]+CurCirR*sin(ConvertAngleToOrient(pArc->endAngle()));
	}
	else
	{
		arrxy[2][8] =(pArc->center())[Y]+CurCirR*cos(ConvertAngleToOrient(pArc->startAngle()));
		arrxy[2][9] =(pArc->center())[X]+CurCirR*sin(ConvertAngleToOrient(pArc->startAngle()));
	}

	acDocManager->unlockDocument(acDocManager->curDocument());	
	SetFocus();	

	return ;
}

void CSetArcOrCircle::OnChangeEditsapaqxbj()
{
	/*
	// 约定1:当半径发生变化时,偏转角不变,改变长度;
	UpdateData(TRUE);
	//获取新的半径与先前的偏转角值;
	double NewRad,OldDivAngle;
	NewRad = m_qxbj;
	OldDivAngle = m_qxpzj*PI/180;
	//计算新的曲线长度;
	double NewCirLen = NewRad*OldDivAngle;
	//重置曲线长度;
	m_qxcd = NewCirLen;
	UpdateData(FALSE);*/

}

void CSetArcOrCircle::OnChangeEditsapapianzhuanjiao()
{
	/*
	// 约定2:当偏转角发生改变,规定半径不变,曲线长度发生变化;
	UpdateData(TRUE);
	//获取新的半径与先前的偏转角值;
	double OldRad,NewDivAngle;
	OldRad = m_qxbj;
	NewDivAngle = m_qxpzj*PI/180;
	//计算新的曲线长度;
	double NewCirLen = OldRad*NewDivAngle;
	//重置曲线长度;
	m_qxcd = NewCirLen;
	UpdateData(FALSE);	*/

}
void CSetArcOrCircle::OnChangeEditsapaqxcd()
{
	/*
	// 约定3:当曲长发生改变,规定半径不变,曲线偏转角发生变化;
	UpdateData(TRUE);
	//获取新的半径与先前的偏转角值;
	double OldRad,NewCirLen;
	OldRad = m_qxbj;
	NewCirLen = m_qxcd;
	//计算新的曲线长度;
	double NewDivAngle = NewCirLen/OldRad*180/PI;
	//重置曲线长度;
	m_qxpzj = NewDivAngle;
	UpdateData(FALSE);*/

}





















