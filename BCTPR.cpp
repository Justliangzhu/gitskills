// BCTPR.cpp: implementation of the BCTPR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCTPR.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BCTPR::BCTPR()
{
	OK1=false;
	OK2=false;
	OK3=false;
	pArc=NULL;
	Len = 0.0;
}

BCTPR::~BCTPR()
{
}

void BCTPR::EraseArc()
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

//设置切线
void BCTPR::SetQX(double n1,double e1,double n2,double e2)
{
	N1=n1; E1=e1;
	N2=n2; E2=e2;
	OK1=true;
}

//设置弧上一点
void BCTPR::SetPointOnArc(double n,double e)
{
	NonArc=n;
	EonArc=e;
	OK2=true;
}

//设置半径
void BCTPR::SetRadius(double r)
{
	R=r;
	OK3=true;
}

//更新
void BCTPR::UpdateData()
{
	if(OK1 && OK2 && OK3)
	{
		double tmpN,tmpE,tmpdist;
		tmpN=NonArc;
		tmpE=EonArc;
		double dist;//弧上一点到切线的距离
		dist=DistToLine(N1,E1,N2,E2,tmpN,tmpE,tmpdist);
		double QXFwj;//切线方位角
		xyddaa(N1,E1,N2,E2,&QXFwj);
		double side;//点在线的哪侧
		side=GetSide(N1,E1,N2,E2,NonArc,EonArc);
		double ProjDist;//弧上点和圆心在切线上的投影距离
		ProjDist=sqrt( R*R - (R-dist)*(R-dist) );
		//弧的转角
		double afa;
		afa=2.0*atan(dist/ProjDist);
		//求圆心
		tmpN-=ProjDist*cos(QXFwj);
		tmpE-=ProjDist*sin(QXFwj);

		if(side>0.0)//左侧
		{			
			startAngle=FwjtoAngX(QXFwj)-0.5*PI;
			StdJiao(startAngle);
			endAngle=startAngle+afa;
			StdJiao(endAngle);
			afa*=-1.0;
			CenN=tmpN+R*cos(QXFwj-0.5*PI);
			CenE=tmpE+R*sin(QXFwj-0.5*PI);
		}
		else
		{
			endAngle=FwjtoAngX(QXFwj)+0.5*PI;
			StdJiao(endAngle);
			startAngle=endAngle-afa;
			CenN=tmpN+R*cos(QXFwj+0.5*PI);
			CenE=tmpE+R*sin(QXFwj+0.5*PI);
		}

		//修改图形
		if(pArc==NULL)
		{
			pArc=new AcDbArc(AcGePoint3d(CenE,CenN,0.0),R,startAngle,endAngle);
			eId=BAS_DRAW_FUN::AddEntityToDbs(pArc);
			Len = R*fabs(endAngle-startAngle);
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
			Len = R*fabs(endAngle-startAngle);
		}
	}
}
