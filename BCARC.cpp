// BCARC.cpp: implementation of the BCARC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCARC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BCARC::BCARC()
{
	OK1=false;
	OK2=false;
	OK3=false;
	pArc=NULL;
	Len = 0.0;
}

BCARC::~BCARC()
{
}
void BCARC::EraseArc()
{
	if(pArc && !pArc->isErased())
	{
		if(eId && acdbOpenObject(pArc,eId,AcDb::kForWrite)==Acad::eOk)
		{
			pArc->erase();
			pArc->close();
		}

	}
}
//设置弧中心点坐标
void BCARC::SetMidPoint(double n0,double e0)
{
	N0=n0;	E0=e0;
	OK1=true;
}

//设置半径
void BCARC::SetRadius(double r)
{
	R=r;
	OK2=true;
}

//设置弧中点右切线上一点坐标
void BCARC::SetOutPoint(double outn,double oute)
{
	OutN=outn;	OutE=oute;
	OK3=true;
}

//更新
void BCARC::UpdateData()
{
	if(OK1&&OK2&&OK3)//数据已有
	{
		double QXFwj;//切线方位角
		double CenToMidFwj;//圆心到弧中心的方位角
		if(xyddaa(N0,E0,OutN,OutE,&QXFwj)<0.001)return;
		CenToMidFwj=QXFwj+0.5*PI;
		CenN=N0-R*cos(CenToMidFwj);
		CenE=E0-R*sin(CenToMidFwj);
		startAngle=CenToMidFwj+0.25*PI;
		endAngle=CenToMidFwj-0.25*PI;
		startAngle=FwjtoAngX(startAngle);
		endAngle=FwjtoAngX(endAngle);
		//修改图形
		if(pArc==NULL)
		{
			pArc=new AcDbArc(AcGePoint3d(CenE,CenN,0.0),R,startAngle,endAngle);
			eId=BAS_DRAW_FUN::AddEntityToDbs(pArc);
			Len = R*fabs(endAngle - startAngle);
		}
		else
		{
			/*acdbOpenObject(pArc,eId,AcDb::kForWrite);*/
			if(acdbOpenObject(pArc,eId,AcDb::kForWrite)!=Acad::eOk)	
			{
				ads_printf(L"打开实体失败！\n");
				return;
			}
			pArc->setCenter(AcGePoint3d(CenE,CenN,0.0));
			pArc->setRadius(R);
			pArc->setStartAngle(startAngle);
			pArc->setEndAngle(endAngle);
			pArc->close();
			Len = R*fabs(endAngle - startAngle);
		}
	}
}

