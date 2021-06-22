// BCPPP.cpp: implementation of the BCPPP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BCPPP.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#include "XLDataBase.h"
BCPPP::BCPPP()
{
	pArc=NULL;
	Len = 0.0;
	//读取设计车速
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

BCPPP::~BCPPP()
{
}

void BCPPP::EraseArc()
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

void BCPPP::AppendPointNearArc(double N,double E)
{
	PtArray.append(AcGePoint3d(N,E,0.0));
}

bool BCPPP::GetCirByThreePointOnArc(double &r,double &CN,double &CE,double N1,double E1,
									double N2,double E2,double N3,double E3)
{
	//弦中点
	double MidN1,MidE1,MidN2,MidE2;
	MidN1=0.5*(N1+N2);
	MidE1=0.5*(E1+E2);
	MidN2=0.5*(N2+N3);
	MidE2=0.5*(E2+E3);
	//弦方位角
	double j1,j2;
	double d1,d2;
	d1=xyddaa(N1,E1,N2,E2,&j1);
	d2=xyddaa(N2,E2,N3,E3,&j2);
	if( fabs(d1-d2)<PRECISION
		|| fabs(j1-j2)<PRECISION
		|| fabs(fabs(j1-j2)-PI)<PRECISION )return false;
	//弦中垂线上一点
	double PerN1,PerE1,PerN2,PerE2;
	PerN1=MidN1+10.0*cos(j1+0.5*PI);
	PerE1=MidE1+10.0*sin(j1+0.5*PI);

	PerN2=MidN2+10.0*cos(j2+0.5*PI);
	PerE2=MidE2+10.0*sin(j2+0.5*PI);

	//求圆心//plh07.25
	if(INTERS(&CN,&CE,MidN1,MidE1,PerN1,PerE1,MidN2,MidE2,PerN2,PerE2)>=0)
	{
		double tmpj;
		r=xyddaa(CN,CE,N1,E1,&tmpj);
		return true;
	}
	else return false;
}

void BCPPP::UpdateData()
{
	double SumN=0.0,SumE=0.0,SumR=0.0;
	int i=0,j=0,k=0,counter=0;
	int PtNum=PtArray.length();
	double N1,E1,N2,E2,N3,E3;
	double CN,CE,r;
	if(PtNum>=3)
	{
		for(i=0;i<PtNum-2;i++)
		{
			for(j=i+1;j<PtNum-1;j++)
			{
				for(k=j+1;k<PtNum;k++)
				{
					N1=PtArray[i].x;
					E1=PtArray[i].y;

					N2=PtArray[j].x;
					E2=PtArray[j].y;

					N3=PtArray[k].x;
					E3=PtArray[k].y;

					if(GetCirByThreePointOnArc(r,CN,CE,N1,E1,N2,E2,N3,E3))
					{
						SumN+=CN;
						SumE+=CE;
						counter++;
					}
				}
			}
		}
		CenN=SumN/counter;
		CenE=SumE/counter;
		//求半径////////////////////////////////////////////////////////////////
		SumR=0.0;
		double tmp;
		for(i=0;i<PtNum;i++)
		{
			SumR+=xyddaa(CenN,CenE,PtArray[i].x,PtArray[i].y,&tmp);
		}
		R=SumR/PtNum;
		//半径从半径系列中取接近值，弧起终点不变，重新计算圆心
		double tmpR;
		tmpR=R;
		//		R=get_R(R,DV);
		//求弦中点的坐标
		i=PtArray.length()-1;
		j=i/2;
		double N0,E0;
		N0=0.5*(PtArray[0].x+PtArray[i].x);
		E0=0.5*(PtArray[0].y+PtArray[i].y);			

		double jiao;//弧起点与弦中点连线的方位角
		double ll=xyddaa(PtArray[0].x,PtArray[0].y,N0,E0,&jiao);//弦长一半
		double TT=sqrt(fabs(R*R-ll*ll));//弦中点到圆心的距离
		double tN,tE,disttmp;
		tN=PtArray[j].x;
		tE=PtArray[j].y;
		//中间点到起终点连线的距离
		double ditoline=DistToLine(PtArray[0].x,PtArray[0].y,PtArray[i].x,PtArray[i].y,tN,tE,disttmp);
		TT-=0.5*fabs(ditoline-(R-TT));
		//定弧的范围,假定点是沿弧依次点取		
		//取出第一个点、中间点、最后一个点
		double side=GetSide(PtArray[0].x,PtArray[0].y,PtArray[i].x,PtArray[i].y,
			PtArray[j].x,PtArray[j].y);
		//判断转向
		if(side<0.0)//返时针点取
		{
			CenN=N0+TT*cos(jiao-0.5*PI);
			CenE=E0+TT*sin(jiao-0.5*PI);
			xyddaa(CenE,CenN,PtArray[0].y,PtArray[0].x,&startAngle);
			xyddaa(CenE,CenN,PtArray[i].y,PtArray[i].x,&endAngle);
		}
		else
		{
			CenN=N0+TT*cos(jiao+0.5*PI);
			CenE=E0+TT*sin(jiao+0.5*PI);
			xyddaa(CenE,CenN,PtArray[0].y,PtArray[0].x,&endAngle);
			xyddaa(CenE,CenN,PtArray[i].y,PtArray[i].x,&startAngle);
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

