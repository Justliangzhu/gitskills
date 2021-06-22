// SetLineDlg1.cpp : implementation file
//

#include "StdAfx.h"
#include "resource.h"
#include "SetLineDlg1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetLineDlg dialog


CSetLineDlg::CSetLineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetLineDlg)
	m_pianjiao = 0.0;
	m_pianjv = 0.0;
	m_slcd = 0.0;
	m_slex = 0.0;
	m_sley = 0.0;
	m_slfwj = 0.0;
	m_slsx = 0.0;
	m_slsy = 0.0;
	m_cixiang = 0;
	m_pianxiang = 0;
	m_FreeOrNot = FALSE;
	//}}AFX_DATA_INIT
	Vcexiang = -1;
	Vpianxiang = -1;
	CreatTOrModifyF = TRUE;
	ClickCout = 0;
	CanCreatRampItem = FALSE;
}

CSetLineDlg::CSetLineDlg(AcDbObjectId EnId,CWnd* pParent)
	: CDialog(CSetLineDlg::IDD, pParent)
{
	m_pianjiao = 0.0;
	m_pianjv = 0.0;
	m_slcd = 0.0;
	m_slex = 0.0;
	m_sley = 0.0;
	m_slfwj = 0.0;
	m_slsx = 0.0;
	m_slsy = 0.0;
	m_cixiang = 0;
	m_pianxiang = 0;
	//
	Vcexiang = -1;
	Vpianxiang = -1;
	JCId = EnId;
	InputEntId= EnId;
	m_FreeOrNot = FALSE;
	CreatTOrModifyF = TRUE;
	ClickCout = 0;
	pLine = NULL;
	LineId = NULL;
	CanCreatRampItem = FALSE;

	

}
void CSetLineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetLineDlg)
	DDX_Control(pDX, IDC_CHECKFREESET, m_CheckFreeset);
	DDX_Text(pDX, IDC_EDITPINAJIAO, m_pianjiao);
	DDX_Text(pDX, IDC_EDITPINAJV, m_pianjv);//偏距
	DDX_Text(pDX, IDC_EDITSLCD, m_slcd);
	DDX_Text(pDX, IDC_EDITSLEX, m_slex);
	DDX_Text(pDX, IDC_EDITSLEY, m_sley);
	DDX_Text(pDX, IDC_EDITSLFWJ, m_slfwj);
	DDX_Text(pDX, IDC_EDITSLSX, m_slsx);
	DDX_Text(pDX, IDC_EDITSLSY, m_slsy);
	DDX_Radio(pDX, IDC_RADIOZOUCE, m_cixiang);
	DDX_Radio(pDX, IDC_RADIOZOUPIAN, m_pianxiang);
	DDX_Check(pDX, IDC_CHECKFREESET, m_FreeOrNot);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetLineDlg, CDialog)
	//{{AFX_MSG_MAP(CSetLineDlg)
	ON_BN_CLICKED(IDSLOK, OnSlok)
	ON_BN_CLICKED(IDSLCANCEL, OnSlcancel)
	ON_BN_CLICKED(IDC_BUTTONSME, OnButtonsme)
	//	ON_BN_CLICKED(IDC_BUTTONSMS, OnButtonsms)
	ON_BN_CLICKED(IDC_RADIOYOUCE, OnRadioyouce)
	ON_BN_CLICKED(IDC_RADIOYOUPIAN, OnRadioyoupian)
	ON_BN_CLICKED(IDC_RADIOZOUCE, OnRadiozouce)
	ON_BN_CLICKED(IDC_RADIOZOUPIAN, OnRadiozoupian)
	//	ON_BN_CLICKED(IDC_BUTTONSLSD, OnButtonslsd)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS,OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTONUPDATE, OnButtonupdate)
	ON_BN_CLICKED(IDC_CHECKFREESET, OnCheckfreeset)
	//	ON_BN_CLICKED(IDC_BUTSETSDOT, OnButsetsdot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetLineDlg message handlers

void CSetLineDlg::OnSlok() 
{

	OnButtonupdate();
	//如果当前不能创建匝道单元;
	if (!CanCreatRampItem)
	{
		return ;
	}
	//激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	if (acdbOpenObject(pLine,LineId,AcDb::kForWrite) == Acad::eOk)
	{
		pLine->erase();
		pLine->close();
	}
	else 
		return ;

	//	if (SelItem == 1||SelItem == 2)
	//	{
	JD_CENTER *JdCenObj=new JD_CENTER;
	//		JdCenObj->assertWriteEnabled();
	JdCenObj->SetXYArray(RouteArray);
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

	int res=GetXdataFromEntity(InputEntId,xray);

	if(res)//InputEntId有扩展数据
		SetXYXdata(InputEntId, pXYArr[1].eID,xray.xyarr);	
	SetXYXdata(pXYArr[1].eID,0,pXYArr[1].xyarr);

	//		AddEntityToDbs(JdCenObj);
	/*
	}
	else 
	{
	JD_CENTER *pJC= NULL;
	acdbOpenObject(pJC,InputEntId,AcDb::kForWrite);
	pJC->assertWriteEnabled();
	if (SelItem == 31)
	{
	pJC->ReplaceItem(RouteArray);
	}
	else
	{
	pJC->AddXYArray(RouteArray);
	}		
	pJC->close();
	}*/


	acDocManager->unlockDocument(acDocManager->curDocument());
	CanCreatRampItem = FALSE;
	//SetFocus();
	CDialog::OnOK();	
}

void CSetLineDlg::OnSlcancel() 
{
	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);

	if (acdbOpenObject(pLine,LineId,AcDb::kForWrite) == Acad::eOk)
	{
		pLine->erase();
		pLine->close();
	}

	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();
	CDialog::OnCancel();
}

void CSetLineDlg::OnButtonsme() //拖动终点
{
	struct resbuf resOSMODE;//设置OSNAP
	resOSMODE.restype=RTSHORT;

	// TODO: Add your control notification handler code here
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
	UpdateData(TRUE);
	if (CreatTOrModifyF) 
	{		
		//获取计算单元;
		//GetComputerItemOfRamps(ComItem,NULL,1);
		/*GetComItemFromSelEntity(InputEntId,ComItem);  */                      // :E,C,S;
		GetComItemFromSelEntity(InputEntId,InterItem,ComItem.ItemDef,FALSE);// :E(O);
		CreatTOrModifyF = FALSE;
	}
	SelItem = int(ComItem.ItemType);
	//如果被选线元为31,则返回;
	/*if (SelItem == 31) 
	{	
	ads_printf(L"\n当前引出直线单元不合理!\n");
	return ;
	}*/
	//如果被选线元是33,35则返回;
	if (SelItem == 33||SelItem == 35) 
	{	
		ads_printf(L"\n当前不能引出直线单元!\n");
		return ;
	}
	//;
	double LineLen = 0,CirDef = 0;
	// 定义直线的起点变量;
	double LineSX,LineSY,LineOA,LineR;	
	LineSX = InterItem.FDotX;
	LineSY = InterItem.FDotY;
	LineOA = InterItem.FDotOrient;
	//如果选中线元是36则要获取其源点数据;
	if (SelItem == 36) 
	{
		//缓曲线的源点;
		GetOrignCorOfSprial(ComItem.FDotY,ComItem.FDotX,ComItem.ItemStartR,
			ComItem.FDotOrient,ComItem.ItemPro,ComItem.ItemDef,ComItem.RadForWard,
			LineSY,LineSX,LineOA);
	}
	if (m_FreeOrNot&&(SelItem == 2||SelItem == 32)) 
	{
		//此时为圆心坐标及半径R;
		LineSX = ComItem.FDotX;
		LineSY = ComItem.FDotY;
		LineR = ComItem.ItemPro;
		CirDef = ComItem.ItemDef;
	}
	//计算新的起点(偏角与偏距,直线的长度重设后),新的直线方位角;
	double SPOffset,SPDeflect;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;
	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;
	ModifyStartDot(LineSX,LineSY,SPOffset,m_pianjv,LineOA,SPDeflect,m_pianjiao);
	//进入拖动循环;	
	AcGePoint3d StaDot,EndDot,PickDot;
	StaDot[X] = LineSX;
	StaDot[Y] = LineSY;
	StaDot[Z] = 0;
	EndDot[Z] = 0;

	int type;
	struct resbuf result;
	for( ; ; )
	{
		acedGetVar(L"autosnap",&resOSMODE);
		short OsnapValue=resOSMODE.resval.rint;

		if (ads_grread(1 , &type , &result) != RTNORM)
		{ 
			break;
		}
		short KEYCODE=result.resval.rint;
		if(KEYCODE==13 || KEYCODE==32 || type==25)
		{
			break;
		}
		if(type == 3)
		{
			break;
		}
		else if(type == 5)
		{
/*			if(OsnapValue>0)//捕捉方式打开
			{
				ads_point PtTemp;

				acedOsnap(		
					result.resval.rpoint,L"endp,int",PtTemp);
				double dist = sqrt((PtTemp[X]- result.resval.rpoint[0])*(PtTemp[X]- result.resval.rpoint[0])+(PtTemp[1]- result.resval.rpoint[1])*(PtTemp[1]- result.resval.rpoint[1]));
				if(dist > 0.01)
				{
					result.resval.rpoint[0]=PtTemp[0];
					result.resval.rpoint[1]=PtTemp[1];
				}

			}*/

			PickDot.x=result.resval.rpoint[X];
			PickDot.y=result.resval.rpoint[Y];
			PickDot.z=result.resval.rpoint[Z];			
			if (m_FreeOrNot&&(SelItem == 2||SelItem == 32)) //前接圆曲线或者圆弧
			{				
				EndDot[X] = PickDot.x;
				EndDot[Y] = PickDot.y;

				LineLen = GetPreciseLength(LineSX,LineSY,EndDot[X],EndDot[Y]);
				LineOA = GetAngleOfLineAndXaxes(LineSX,LineSY,EndDot[X],EndDot[Y]);
				//如果圆心与直线终点间距离不合理;
				if (LineLen < LineR) continue;
				if (LineLen < DataPrecision) continue;
				//求得切线辐角;		
				LineOA = LineOA+CirDef*acos(LineR/LineLen);
				//将辐角转化成方位角;
				LineOA = ConvertAngleToOrient(LineOA);
				//获取切点坐标即直线起点坐标;
				StaDot[X] = LineSX+LineR*sin(LineOA);
				StaDot[Y] = LineSY+LineR*cos(LineOA);
			}	
			else  
			{
				GetProjectDotOfLine(StaDot[X],StaDot[Y],GetValueKByOrient(LineOA*180/PI),
					PickDot.x,PickDot.y,EndDot[X],EndDot[Y]);
			}
			if(pLine == NULL)
			{
				LineId = makeline1(StaDot,EndDot,3);
				/*	acdbOpenObject(pLine,LineId,AcDb::kForWrite);*/
				if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pLine->close();
			}
			else
			{
				/*acdbOpenObject(pLine,LineId,AcDb::kForWrite);*/
				if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				pLine->setStartPoint(StaDot);
				pLine->setEndPoint(EndDot);
				pLine->close();
			}
			m_slsx = ChangeDataPrecision(StaDot[X]);
			m_slsy = ChangeDataPrecision(StaDot[Y]);
			m_slex = ChangeDataPrecision(EndDot[X]);
			m_sley = ChangeDataPrecision(EndDot[Y]);
			m_slcd = ChangeDataPrecision(distfwj(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y],&LineOA));
			m_slfwj = ChangeDataPrecision(LineOA*180/PI);			
			UpdateData(FALSE);
		}
	}
	LineLen	=GetPreciseLength(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y]);
	LineOA = ConvertAngleToOrient(
		GetAngleOfLineAndXaxes(StaDot[X],StaDot[Y],EndDot[X],EndDot[Y]));
	//添入线元;
	RouteArray[1][0] = 1;
	RouteArray[1][1] = LineLen;
	RouteArray[1][2] = 0;
	RouteArray[1][4] = 0;
	RouteArray[1][3] = 0;
	RouteArray[1][5] = LineOA;
	RouteArray[1][6] = 0;
	RouteArray[1][7] = 1E50;
	RouteArray[1][8] = StaDot[Y];
	RouteArray[1][9] = StaDot[X];
	//结束线元;
	RouteArray[2][0] = 0;
	RouteArray[2][1] = 0;
	RouteArray[2][2] = 0;
	RouteArray[2][3] = 0;
	RouteArray[2][4] = 0;
	RouteArray[2][5] = LineOA;
	RouteArray[2][6] = 0;
	RouteArray[2][7] = 1E50;
	RouteArray[2][8] = EndDot[Y];
	RouteArray[2][9] = EndDot[X];
	//代替线元;
	CanCreatRampItem = TRUE;
	if (SelItem ==1 ||SelItem == 2)
	{		
		RouteArray[0][0] = 0;
		RouteArray[0][1] = RouteArray[1][8];
		RouteArray[0][2] = RouteArray[1][9];
		RouteArray[0][3] = RouteArray[1][5];
		RouteArray[0][4] = RouteArray[1][6];
		RouteArray[0][5] = 0;
		RouteArray[0][6] = 0;
		RouteArray[0][7] = 0;
		RouteArray[0][8] = 0;
		RouteArray[0][9] = 0;
	}
	else
	{
		AcDbObject* pObj;
		if(acdbOpenObject(pObj,InputEntId,AcDb::kForRead)!=Acad::eOk)
			return;
		if(!pObj->isKindOf(JD_CENTER::desc()))
		{
			pObj->close();	
			RouteArray[0][0] = 0;
			RouteArray[0][1] = RouteArray[1][8];
			RouteArray[0][2] = RouteArray[1][9];
			RouteArray[0][3] = RouteArray[1][5];
			RouteArray[0][4] = RouteArray[1][6];
			RouteArray[0][5] = 0;
			RouteArray[0][6] = 0; 
			RouteArray[0][7] = 0;
			RouteArray[0][8] = 0;
			RouteArray[0][9] = 0;
			return ;		
		}
		JD_CENTER * pJdcenter = JD_CENTER::cast(pObj);


		/*JD_CENTER * pJdcenter = NULL;
		if (acdbOpenObject(pObj,InputEntId,AcDb::kForRead) != Acad::eOk)
		{
		pJdcenter=JD_CENTER::cast(pObj);
		RouteArray[0][0] = 0;
		RouteArray[0][1] = RouteArray[1][8];
		RouteArray[0][2] = RouteArray[1][9];
		RouteArray[0][3] = RouteArray[1][5];
		RouteArray[0][4] = RouteArray[1][6];
		RouteArray[0][5] = 0;
		RouteArray[0][6] = 0; 
		RouteArray[0][7] = 0;
		RouteArray[0][8] = 0;
		RouteArray[0][9] = 0;
		return ;
		}
		pJdcenter->close();*/
		//如果前面为JD直线实体;
		if (SelItem == 31)
		{
			for(int i=0; i<10; i++)
			{
				RouteArray[0][i] = pJdcenter->XYArray[pJdcenter->XYNum-2][i];
				RouteArray[1][i] = pJdcenter->XYArray[pJdcenter->XYNum-1][i];
			}
			RouteArray[1][1] = pJdcenter->XYArray[pJdcenter->XYNum-1][1] + LineLen;
		}
		else 
		{
			for(int i=0; i<10; i++)
			{
				RouteArray[0][i] = pJdcenter->XYArray[pJdcenter->XYNum-1][i];
			}
			//设置直线起点里程;
			RouteArray[1][6] = pJdcenter->XYArray[pJdcenter->XYNum][6];
			//如果是JD圆实体则要修改圆弧长度并重并报设置直线起点里程;
			if (SelItem == 32 && m_FreeOrNot) 
			{
				double Sfwj = RouteArray[0][5],Efwj = RouteArray[1][5];
				double zj = ZXJ(Efwj,Sfwj);
				if (zj*RouteArray[0][4]<0) 
					zj = 2*PI-fabs(zj);
				RouteArray[0][2] = fabs(RouteArray[0][1]*zj);	
				RouteArray[1][6] = RouteArray[0][6] + RouteArray[0][2] ;
			}
			//如果不完后缓则要调成完整后缓才能引出直线;
			if (SelItem == 36) 
			{
				RouteArray[0][0] = 4.0;
				RouteArray[1][6] = RouteArray[0][6] + 
					RouteArray[0][1]*RouteArray[0][1]/RouteArray[0][2];
			}

		}	
		pJdcenter->close();
	}
	RouteArray[2][6] = RouteArray[1][6] +RouteArray[1][1];

	CanCreatRampItem = TRUE;

	acDocManager->unlockDocument(acDocManager->curDocument());
	//SetFocus();
	ads_printf(L"\nCommand:");
}

void CSetLineDlg::OnButtonsms() 
{
	// TODO: Add your control notification handler code here

}

void CSetLineDlg::OnRadioyouce() 
{
	// TODO: Add your control notification handler code here
	Vcexiang = 1;
}

void CSetLineDlg::OnRadioyoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = 1;
}

void CSetLineDlg::OnRadiozouce() 
{
	// TODO: Add your control notification handler code here
	Vcexiang = -1;
}

void CSetLineDlg::OnRadiozoupian() 
{
	// TODO: Add your control notification handler code here
	Vpianxiang = -1;
}

void CSetLineDlg::OnButtonslsd() 
{
}

afx_msg LRESULT CSetLineDlg::OnAcadKeepFocus(WPARAM wParam,LPARAM lParam)
{
	return TRUE;
}

BOOL CSetLineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//struct resbuf resOSMODE;//设置OSNAP
	//resOSMODE.restype = RTSHORT;
	// 激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(), AcAp::kWrite);
	// 获取新数据;
	UpdateData(TRUE);
	// 打开所获取的实体;
	AcDbObject* pEntity = NULL;
	if (acdbOpenObject(pEntity, InputEntId, AcDb::kForRead) != Acad::eOk)
	{
		return false;
	}
	if (pEntity == NULL)
	{
		return false;
	}
	pEntity->close();

	// 对自定义实体和多段线变换成直线zl11.4
	BOOL IsRampRoad = FALSE;
	BAS_DRAW_FUN BAS1;
	AcGePoint3d startPt;
	AcDbObjectId lineId=NULL;
	if (_tcscmp(pEntity->isA()->name(), L"JD_CENTER") == 0)
	{
		
		startPt=BAS1.resetjd_centerid(point, pEntity, lineId);
		IsRampRoad = TRUE;
		if (acdbOpenObject(pEntity, lineId, AcDb::kForRead) != Acad::eOk)
		{
			ads_printf(L"打开实体失败！\n");
			return false;
		}
		pEntity->close();
		InputEntId = lineId;
	}
	//若打开的是一般的缓和曲线，则将其变成直线，进行之后的求解2019.11.4zl
	if (_tcscmp(pEntity->isA()->name(), L"AcDbPolyline") == 0) //缓和曲线
	{
		XYarray xray;
		int res = GetXdataFromEntity(InputEntId, xray);
		if (res == 0)
		{
			InputEntId = NULL;
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
				bas.ChuizuToZx(dXchuizu, dYchuizu, point.x, point.y, location.x, location.y, location1.x, location1.y);
				if ((dXchuizu - location.x) * (dXchuizu - location1.x) < 0 && (dYchuizu - location.y) * (dYchuizu - location1.y))
				{
					AcDbLine* pl1 = new AcDbLine(location, location1);
					InputEntId = AddEntityToDbs(pl1);
					if (acdbOpenObject(pEntity, InputEntId, AcDb::kForRead) != Acad::eOk)
					{
						ads_printf(L"打开实体失败！\n");
						return false;
					}
					/*			pEntity->close();
								InputEntId = lineId;
								m_slsx = pl1->startPoint().x;
								m_slsy = pl1->startPoint().y;
								m_slex = pl1->endPoint().x;
								m_sley = pl1->endPoint().y;
								m_slcd = pl1->startPoint().distanceTo(pl1->endPoint());
								InputEntId = AddEntityToDbs(pl1);*/
					break;
				}
			}
			/*ads_printf(L"选中实体无效!\n");
			pEntity->close();*/
		}
	}
	//得到新的实体
	GetComItemFromSelEntity(InputEntId, ComItem);                        // :E,C,S;
	GetComItemFromSelEntity(InputEntId, InterItem, ComItem.ItemDef, FALSE);

	// TODO: Add extra initialization here
	if (pEntity->isKindOf(AcDbLine::desc()))
	{
		m_slsx = ComItem.FDotX;
		m_slsy = ComItem.FDotY;
	}
	else if (pEntity->isKindOf(AcDbArc::desc()))
	{
		//GetComItemFromSelEntity(InputEntId, ComItem);                        // :E,C,S;
		//GetComItemFromSelEntity(InputEntId, InterItem, ComItem.ItemDef, FALSE);
		m_slsx = ComItem.FDotX;
		m_slsy = ComItem.FDotY;
		/*ComItem.FDotOrient = ConvertAngleToOrient(arc->endAngle() + PI / 2);
		m_slsx = arc;
		m_slsy = startPt.y;*/
		//double angle;
		//angle = arc->startAngle();
		//angle = 2 * PI - angle;
		//if (angle > 2 * PI)angle -= 2 * PI;
		////double angletemp = FwjtoAngX(angle);
		//m_slfwj = ChangeDataPrecision(angle * 180 / PI);
	}
	pEntity->close();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetLineDlg::OnButtonupdate() 
{
	//激活CAD文档;
	acedGetAcadDwgView()->SetFocus();
	acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);	
	//打开已创建的线实体读取其起终点数据;	
	if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
	{
		return ;
	}
	//读取直线实体起终点数据;	
	double LineSX = (pLine->startPoint())[X];
	double LineSY = (pLine->startPoint())[Y];
	double LineEX = (pLine->endPoint())[X];
	double LineEY = (pLine->endPoint())[Y];
	pLine->close();
	//获取直线的方位角;
	double ValueOrient;
	distfwj(LineSX,LineSY,LineEX,LineEY,&ValueOrient);
	//如果刷新前设置起点偏角(偏向),起点偏距;
	double SPDeflect ;
	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;

	double SPOffset;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;
	BackTonotModifidOfSDot(LineSX,LineSY,SPOffset,m_pianjv,ValueOrient,SPDeflect,m_pianjiao);
	// 2005.06.12 hhp
	if (SelItem == 31) 
	{
		RouteArray[1][1] = RouteArray[1][1] - m_slcd;
	}
	//获取新的数据;
	UpdateData(TRUE);
	//计算新的起点(偏角与偏距,直线的长度重设后),新的直线方位角;
	if (m_cixiang == 0) SPOffset = -1;
	else SPOffset = 1;

	if (m_pianxiang == 0) SPDeflect = -1;
	else SPDeflect = 1;
	ModifyStartDot(LineSX,LineSY,SPOffset,m_pianjv,ValueOrient,SPDeflect,m_pianjiao);
	//获取直线的长度;
	double LineLen = m_slcd;
	//计算直线的终点;
	LineEX = LineSX + LineLen*sin(ValueOrient);
	LineEY = LineSY + LineLen*cos(ValueOrient);
	//修改直线属性;
	if(acdbOpenObject(pLine,LineId,AcDb::kForWrite)!=Acad::eOk)
	{
		return ;
	}
	//读取直线实体起终点数据;	
	pLine->setStartPoint(AcGePoint3d(LineSX,LineSY,0.0));
	pLine->setEndPoint(AcGePoint3d(LineEX,LineEY,0.0));
	pLine->close();
	//更新对话框中数据;
	m_slsx = LineSX;
	m_slex = LineSY;
	m_slsy = LineEX;
	m_sley = LineEY;
	m_slcd = distfwj(LineSX,LineSY,LineEX,LineEY,&ValueOrient);
	m_slfwj = ValueOrient*180/PI;
	UpdateData(FALSE);
	//重新设置线元数据;
	//添入线元;
	if (SelItem == 31) 
	{
		RouteArray[1][1] = RouteArray[1][1]+m_slcd;
		RouteArray[1][5] = ValueOrient;
		RouteArray[1][7] = 1E50;
	}
	else
	{
		RouteArray[1][1] = m_slcd;
		RouteArray[1][5] = ValueOrient;
		RouteArray[1][7] = 1E50;
		RouteArray[1][8] = LineSY;
		RouteArray[1][9] = LineSX;
	}
	//结束线元;
	RouteArray[2][5] = ValueOrient;
	RouteArray[2][6] = RouteArray[1][6]+RouteArray[1][1];
	RouteArray[2][7] = 1E50;
	RouteArray[2][8] = LineSY+RouteArray[1][1]*cos(ValueOrient);
	RouteArray[2][9] = LineSX+RouteArray[1][1]*sin(ValueOrient);

	acDocManager->unlockDocument(acDocManager->curDocument());
	SetFocus();				
}

void CSetLineDlg::OnCheckfreeset() 
{
	// TODO: Add your control notification handler code here
	ClickCout+=1;
	if (fmod(ClickCout,2) !=0) 
		m_FreeOrNot = TRUE;
	else m_FreeOrNot = FALSE;	
}

// 起点设置
void CSetLineDlg::OnButsetsdot() 
{

}
