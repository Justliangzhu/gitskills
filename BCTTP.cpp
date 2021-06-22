// BCTTR.cpp: implementation of the BCTTR class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCTTP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "XLDataBase.h"
BCTTP::BCTTP()
{
	OK1=false;
	OK2=false;
	pArc=NULL;
	Len=0.0;

	/*
	if(!ReadWorkdir())
	return ;	
	XLDataBase xlmdb;
	CString prj_fname;
	prj_fname.Format(L"%s\\data\\%s.mdb",Cworkdir,Pro);
	xlmdb.Read_XLDbs(prj_fname,"主要技术标准");
	DV = xlmdb.JSBZ.SDMB;*/
	//    ReadGuifanFile();
	//	DV = mGuiFanCS.SuDuMuBiao;	
}

BCTTP::~BCTTP()
{
}

void BCTTP::EraseArc()
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

//设置三个交点
void BCTTP::SetThreeJD(double n1,double e1,double n0,double e0,double n2,double e2)
{
	OK1=true;
	double j1,j2;
	xyddaa(n1,e1,n0,e0,&j1);
	xyddaa(n2,e2,n0,e0,&j2);
	if(fabs(j1-j2)<0.0001 || fabs(fabs(j1-j2)-PI)<0.0001 )
	{
		OK1=false;
		return;
	}

	JdN1=n1;	JdE1=e1;
	JdN0=n0;	JdE0=e0;
	JdN2=n2;	JdE2=e2;

}

//设置圆弧上一点
void BCTTP::SetPointOnArc(double n,double e)
{
	OK2=true;
	NonArc=n;
	EonArc=e;
}

void BCTTP::UpdateData()
{
	if(OK1 && OK2)
	{

		AcDbLine L1(AcGePoint3d(JdE1,JdN1,0.0),AcGePoint3d(JdE0,JdN0,0.0));
		AcDbLine L2(AcGePoint3d(JdE0,JdN0,0.0),AcGePoint3d(JdE2,JdN2,0.0));
		R=radiusfree(&L1,&L2,AcGePoint3d(EonArc,NonArc,0.0));

		if(R<0.0)return;
		double JdToCenterFwj;//中间交点到圆心的方位角
		double j1,j2;
		xyddaa(JdN1,JdE1,JdN0,JdE0,&j1);
		xyddaa(JdN0,JdE0,JdN2,JdE2,&j2);
		//左右转
		double aa=j2-j1;
		if(aa<-PI)aa+=2.0*PI;
		if(aa>PI)aa-=2.0*PI;

		if(aa<0.0)//左转
		{
			startAngle=j1+0.5*PI;
			endAngle=j2+0.5*PI;
			JdToCenterFwj=j1+0.5*aa-0.5*PI;
		}
		else
		{
			startAngle=j2-0.5*PI;
			endAngle=j1-0.5*PI;
			JdToCenterFwj=j1+0.5*aa+0.5*PI;
		}		
		startAngle=FwjtoAngX(startAngle);
		endAngle=FwjtoAngX(endAngle);

		StdJiao(JdToCenterFwj);
		/*
		if(JdToCenterFwj>PI)JdToCenterFwj=2.0*PI-JdToCenterFwj;
		*/

		double afa;//圆心角的一半
		afa=0.5*(PI-fabs(aa));

		CenN=JdN0+R/sin(afa)*cos(JdToCenterFwj);
		CenE=JdE0+R/sin(afa)*sin(JdToCenterFwj);

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
