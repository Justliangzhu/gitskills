// JSGOU.cpp: implementation of the JSGOU class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "JSGOU.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
extern double Sfactor;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ACRX_DXF_DEFINE_MEMBERS(JSGOU_ROAD, AcDbEntity,
						AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent,
						0, JSGOU_ROAD, RailHdm);
JSGOU_ROAD::JSGOU_ROAD()
{
	JSGouPtNum = 0;
	m_GouBotPoint.x = 0.0,m_GouBotPoint.y = 0.0;
	m_OriGouBotPt = m_GouBotPoint;
	m_GouBotH=m_GouBotX=m_XtoPJ=0.0;
	BJ=GW=GS=SP1=SP2=TG_JQHD=TG_GDR = 0.0;

	for(int i=0; i<4; i++)
	{
		m_JSGouPoints[i].x=0.0;
		m_JSGouPoints[i].y=0.0;
	}
}

JSGOU_ROAD::~JSGOU_ROAD()
{

}

Adesk::Boolean JSGOU_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{
	assertReadEnabled();
	if(JSGouPtNum==0)//无水沟
		return Adesk::kFalse;

	for(int i=0;i<5;i++)
		m_JqPt[i]=JqPt[i];
	m_JqPt[5]=JqPt[9];
	m_JqPt[6]=JqPt[4];
	m_JqPt[7]=JqPt[5];
	m_JqPt[8]=JqPt[6];
	m_JqPt[9]=JqPt[7];
	m_JqPt[10]=JqPt[0];
	pWd->subEntityTraits().setColor(4);
	pWd->geometry().polyline(11,m_JqPt);

	//	pWd->subEntityTraits().setColor(4);
	//  	pWd->geometry().polyline(4,m_JSGouPoints);


	pWd->subEntityTraits().setColor(6);
	AcGePoint3d origin;
	ACHAR Str[20];
	AcGePoint3d pt[4];
	pt[0] = m_GouBotPoint;
	pt[1].x = m_GouBotPoint.x - 0.2;
	pt[1].y = m_GouBotPoint.y + 0.3;
	pt[2].x = m_GouBotPoint.x + 0.2;
	pt[2].y = m_GouBotPoint.y + 0.3;
	pt[3] = pt[0];
	pWd->subEntityTraits().setFillType(kAcGiFillAlways);
	pWd->geometry().polygon(4,pt);

	origin.x = m_GouBotPoint.x;
	origin.y = m_GouBotPoint.y + 0.5;
	origin.z = m_GouBotPoint.z;
	AcGeVector3d direction(1.0, 0.0, 0.0);
	AcGeVector3d normal(0.0, 0.0, 1.0);//平行于XY平面

	_stprintf(Str,L"%.3lf",m_GouBotH);
	pWd->subEntityTraits().setColor(2);
	pWd->geometry().text(origin, normal, direction, 1,
		0.8, 0.0, Str);
	//截水沟浆切
	//pWd->subEntityTraits().setColor(4);
	//pWd->geometry().polyline(JqPtNum,JqPt);
	return Adesk::kTrue;
}
void JSGOU_ROAD::subList() const
{
	assertReadEnabled();

	AcDbEntity::subList();

	ads_printf(L"\n\t\t自定义横断面路堤水沟实体");
	//	ads_printf(L"\n\t\t里程 = %s %s", m_ckml,m_ZorY);
	//	ads_printf(L"\n\t\t地面点数 = %d", m_EarthPointNum);
	//	ads_printf(L"\n\t\t水沟结构尺寸:");
	//	ads_printf(L"\n\t\t深=%0.2lfm 底宽=%0.2lfm 内侧边坡=1:%0.2lf 外侧边坡=1:%0.2lf", m_GH,m_GW,m_SP1,m_SP2);

	for(int i=0; i<11; i++)
		ads_printf(L"\n\t\tx=%0.3lf y=%0.3lf", m_JqPt[i].x,m_JqPt[i].y);
}

Acad::ErrorStatus JSGOU_ROAD::subTransformBy(const AcGeMatrix3d& xform)
{
	int i;

	if(JSGouPtNum==0)
		return AcDbEntity::subTransformBy(xform);
	//	m_FillArea=0.0;
	m_OriGouBotPt = m_GouBotPoint; 

	assertWriteEnabled();	
	m_GouBotPoint.transformBy(xform);

	for(i=0; i<JSGouPtNum; i++)
		m_JSGouPoints[i].transformBy(xform);

	m_GouBotH = m_GouBotH + (m_GouBotPoint.y - m_OriGouBotPt.y)/Sfactor;
	m_GouBotX = m_GouBotX + (m_GouBotPoint.x - m_OriGouBotPt.x)/Sfactor;

	return AcDbEntity::subTransformBy(xform);
}

int JSGOU_ROAD::Cal_Yd(double x,double &Yd,int &ptnum)
{
	int i;

	i = 0;
	Yd = m_JSGouPoints[0].y;
	ptnum = 0;
	while(m_ZorY*x < m_ZorY*(m_JSGouPoints[i].x+0.001) && i<JSGouPtNum)
		i++;
	if(i==JSGouPtNum)
	{
		Yd = m_JSGouPoints[i-1].y;
		ptnum = i-1;
		return 0;//找不到
	}
	if(i==0)
		return -1;		
	ptnum = i;
	Yd = m_JSGouPoints[i-1].y + (x-m_JSGouPoints[i-1].x)*(m_JSGouPoints[i].y-m_JSGouPoints[i-1].y)/(m_JSGouPoints[i].x-m_JSGouPoints[i-1].x);
	return 1;//找到
}
Adesk::Boolean JSGOU_ROAD::Draw(int iColor)
{
	assertReadEnabled();

	AcGePoint3dArray ptarray,ptarray1;
	int i;

	if(JSGouPtNum<4)//无水沟
		return Adesk::kFalse;
	ACHAR LayerName[256];
	_tcscpy(LayerName, L"横断面图-天沟");
	AcDbObjectId layerId = CreateLayer(LayerName);

	for(i=0; i<JqPtNum-1; i++)
		ptarray.append(JqPt[i]);
	make2dline(ptarray,iColor,0.5,0.5,layerId);

	ptarray1.append(JqPt[4]);
	ptarray1.append(JqPt[JqPtNum-1]);
	make2dline(ptarray1,iColor,0.5,0.5, layerId);


	ACHAR Str[20];
	_stprintf(Str,L"%.3lf",m_GouBotH);	
	NoteBG(m_GouBotPoint,1.0,Str,6,2,Sfactor,2, LayerName);


	return Adesk::kTrue;
}

void JSGOU_ROAD::SetGutterPara(double ParaBJ, double ParaGW, double ParaGS, double ParaSP1, double ParaSP2, double ParaJQHD, double ParaGDR)
{
	BJ = ParaBJ;
	GW = ParaGW;
	GS = ParaGS;
	SP1 = ParaSP1;
	SP2 = ParaSP2;
	TG_JQHD = ParaJQHD;
	ParaGDR = ParaGDR;
}

//20190928新增	删除天沟
void JSGOU_ROAD::DeleteGutter()
{
	JSGouPtNum = 0;
	m_GouBotPoint.x = 0.0, m_GouBotPoint.y = 0.0;
	m_OriGouBotPt = m_GouBotPoint;
	m_GouBotH = m_GouBotX = m_XtoPJ = 0.0;
	BJ = GW = GS = SP1 = SP2 = TG_JQHD = TG_GDR = 0.0;

	for (int i = 0; i < 4; i++)
	{
		m_JSGouPoints[i].x = 0.0;
		m_JSGouPoints[i].y = 0.0;
	}
}

void JSGOU_ROAD::CheckNSetGutterStatus()
{
	if (!JSGouPtNum)
		DeleteGutter();
}

