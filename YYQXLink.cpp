// YYQXLink.cpp: implementation of the YYQXLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "YYQXLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "ZYLink.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

YYQXLink::YYQXLink()
{
	ERRFlag=false;
	A1=0.0;
	A2=0.0;
	OK1=false;
	OK2=false;
	QXClass=1;
}

YYQXLink::~YYQXLink()
{

}

void YYQXLink::SetCir1(double n,double e,double r)
{
	OK1=true;
	CenN1=n;
	CenE1=e;
	R1=r;
	/*
	AcDbCircle *Cir=new AcDbCircle(AcGePoint3d(CenE1,CenN1,0.0),
	AcGeVector3d(0.0,0.0,1.0),R1);
	BAS_DRAW_FUN::AddEntityToDbs(Cir);
	*/
}

void YYQXLink::SetCir2(double n,double e,double r)
{
	OK2=true;
	CenN2=n;
	CenE2=e;
	R2=r;
	/*
	AcDbCircle *Cir=new AcDbCircle(AcGePoint3d(CenE2,CenN2,0.0),
	AcGeVector3d(0.0,0.0,1.0),R2);
	BAS_DRAW_FUN::AddEntityToDbs(Cir);
	*/
}

void YYQXLink::SetA(double a1,double a2)
{
	A1=a1;	A2=a2;
}

void YYQXLink::SetQXClass(int qx)
{
	if(qx<=4 && qx>=0)QXClass=qx;
}

void YYQXLink::CalQX()
{
	ERRFlag=false;
	if(OK1 && OK2)
	{
		//先求出内移量
		if(A1>0.0)
		{
			Lo1=A1*A1/R1;
			Xm1=spiral_x(A1,Lo1);
			Ym1=spiral_y(A1,Lo1);
			double t=Lo1/(2*R1);
			double dr=Ym1+R1*cos(t);//sin(t+0.5*PI);
			p1=dr-R1;
			M1=Xm1-R1*sin(t);
		}
		else p1=0.0;

		if(A2>0.0)
		{
			Lo2=A2*A2/R2;
			Xm2=spiral_x(A2,Lo2);
			Ym2=spiral_y(A2,Lo2);
			double t=Lo2/(2*R2);
			double dr=Ym2+R2*cos(t);//sin(t+0.5*PI);			
			p2=dr-R2;
			M2=Xm2-R2*sin(t);
		}
		else p2=0.0;

		//半径+内移量
		double tR1,tR2;
		tR1=R1+p1;
		tR2=R2+p2;

		double FwjO1O2;//圆心1到圆心2的方位角
		//圆心距
		double O1O2=xyddaa(CenN1,CenE1,CenN2,CenE2,&FwjO1O2);
		double t;//圆心1到圆心2的连线与加以与切点连线的夹角
		if(QXClass==S_F)//S连接返时针
		{
			if((tR1+tR2)>=O1O2)
			{
				ERRFlag=true;
				return;
			}
			t=acos( (tR1+tR2)/O1O2 );
			//求与大圆切点
			GQN1=CenN1+tR1*cos(FwjO1O2+t);
			GQE1=CenE1+tR1*sin(FwjO1O2+t);
			GQN2=CenN2+tR2*cos(FwjO1O2+PI+t);
			GQE2=CenE2+tR2*sin(FwjO1O2+PI+t);			

			JiaZXLength=xyddaa(GQN1,GQE1,GQN2,GQE2,&FwjJiaZX)-M1-M2;

			//求直缓点
			if(A1>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN2,GQE2,GQN1,GQE1,CenN1,CenE1,R1,RIGHT);
				pSPIRAL1=ZYObj.pSPIRAL;
				//				GQN1+=M1*cos(FwjJiaZX);
				//				GQE1+=M1*sin(FwjJiaZX);
			}
			if(A2>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN1,GQE1,GQN2,GQE2,CenN2,CenE2,R2,RIGHT);
				pSPIRAL2=ZYObj.pSPIRAL;
				//				GQN2+=M2*cos(FwjJiaZX+PI);
				//				GQE2+=M2*sin(FwjJiaZX+PI);
			}

			/*
			//求与圆曲线的切点
			if(A1>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm1,Ym1,&at);
			PtOnCirN1=GQN1+dM*cos(FwjJiaZX+PI+at);
			PtOnCirE1=GQE1+dM*sin(FwjJiaZX+PI+at);
			}
			else
			{
			PtOnCirN1=GQN1;
			PtOnCirE1=GQE1;
			}

			if(A2>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm2,Ym2,&at);
			PtOnCirN2=GQN2+dM*cos(FwjJiaZX+at);
			PtOnCirE2=GQE2+dM*sin(FwjJiaZX+at);
			}
			else
			{
			PtOnCirN2=GQN2;
			PtOnCirE2=GQE2;
			}*/

		}
		else if(QXClass==S_S)//S连接顺时针
		{
			if((tR1+tR2)>=O1O2)
			{
				ERRFlag=true;
				return;
			}
			t=acos( (tR1+tR2)/O1O2 );
			t*=-1.0;
			//求切点
			GQN1=CenN1+tR1*cos(FwjO1O2+t);
			GQE1=CenE1+tR1*sin(FwjO1O2+t);
			GQN2=CenN2+tR2*cos(FwjO1O2+PI+t);
			GQE2=CenE2+tR2*sin(FwjO1O2+PI+t);

			JiaZXLength=xyddaa(GQN1,GQE1,GQN2,GQE2,&FwjJiaZX);

			//求直缓点
			if(A1>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN2,GQE2,GQN1,GQE1,CenN1,CenE1,R1,LEFT);
				pSPIRAL1=ZYObj.pSPIRAL;

				//				GQN1+=M1*cos(FwjJiaZX);
				//				GQE1+=M1*sin(FwjJiaZX);
			}
			if(A2>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN1,GQE1,GQN2,GQE2,CenN2,CenE2,R2,LEFT);
				pSPIRAL2=ZYObj.pSPIRAL;
				//				GQN2+=M2*cos(FwjJiaZX+PI);
				//				GQE2+=M2*sin(FwjJiaZX+PI);
			}

			/*
			//求与圆曲线的切点
			if(A1>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm1,Ym1,&at);
			PtOnCirN1=GQN1+dM*cos(FwjJiaZX+PI-at);
			PtOnCirE1=GQE1+dM*sin(FwjJiaZX+PI-at);
			}
			else
			{
			PtOnCirN1=GQN1;
			PtOnCirE1=GQE1;
			}

			if(A2>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm2,Ym2,&at);
			PtOnCirN2=GQN2+dM*cos(FwjJiaZX-at);
			PtOnCirE2=GQE2+dM*sin(FwjJiaZX-at);
			}
			else
			{
			PtOnCirN2=GQN2;
			PtOnCirE2=GQE2;
			}*/

		}
		else if(QXClass==C_F)//C连接返时针
		{
			if(O1O2<=fabs(tR1-tR2) && O1O2>0.001)
			{
				ERRFlag=true;
				return;
			}
			t=acos( (tR1-tR2)/O1O2 );
			//求切点
			GQN1=CenN1+tR1*cos(FwjO1O2+t);
			GQE1=CenE1+tR1*sin(FwjO1O2+t);
			GQN2=CenN2+tR2*cos(FwjO1O2+t);
			GQE2=CenE2+tR2*sin(FwjO1O2+t);

			JiaZXLength=xyddaa(GQN1,GQE1,GQN2,GQE2,&FwjJiaZX);

			//求直缓点
			if(A1>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN2,GQE2,GQN1,GQE1,CenN1,CenE1,R1,RIGHT);
				pSPIRAL1=ZYObj.pSPIRAL;
				//				GQN1+=M1*cos(FwjJiaZX);
				//				GQE1+=M1*sin(FwjJiaZX);
			}
			if(A2>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN1,GQE1,GQN2,GQE2,CenN2,CenE2,R2,LEFT);
				pSPIRAL2=ZYObj.pSPIRAL;
				//				GQN2+=M2*cos(FwjJiaZX+PI);
				//				GQE2+=M2*sin(FwjJiaZX+PI);
			}

			/*
			//求与圆曲线的切点
			if(A1>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm1,Ym1,&at);
			PtOnCirN1=GQN1+dM*cos(FwjJiaZX+PI+at);
			PtOnCirE1=GQE1+dM*sin(FwjJiaZX+PI+at);
			}
			else
			{
			PtOnCirN1=GQN1;
			PtOnCirE1=GQE1;
			}

			if(A2>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm2,Ym2,&at);
			PtOnCirN2=GQN2+dM*cos(FwjJiaZX-at);
			PtOnCirE2=GQE2+dM*sin(FwjJiaZX-at);
			}
			else
			{
			PtOnCirN2=GQN2;
			PtOnCirE2=GQE2;
			}*/

		}
		else//C连接顺时针
		{
			if(O1O2<=fabs(tR1-tR2) && O1O2>0.001)
			{
				ERRFlag=true;
				return;
			}
			t=acos( (tR1-tR2)/O1O2 );
			t*=-1.0;
			//求切点
			GQN1=CenN1+tR1*cos(FwjO1O2+t);
			GQE1=CenE1+tR1*sin(FwjO1O2+t);
			GQN2=CenN2+tR2*cos(FwjO1O2+t);
			GQE2=CenE2+tR2*sin(FwjO1O2+t);

			JiaZXLength=xyddaa(GQN1,GQE1,GQN2,GQE2,&FwjJiaZX);

			//求直缓点
			if(A1>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN2,GQE2,GQN1,GQE1,CenN1,CenE1,R1,LEFT);
				pSPIRAL1=ZYObj.pSPIRAL;
				//				GQN1+=M1*cos(FwjJiaZX);
				//				GQE1+=M1*sin(FwjJiaZX);
			}
			if(A2>0.0)
			{
				ZYLink ZYObj;
				ZYObj.Set(GQN1,GQE1,GQN2,GQE2,CenN2,CenE2,R2,RIGHT);
				pSPIRAL2=ZYObj.pSPIRAL;
				//				GQN2+=M2*cos(FwjJiaZX+PI);
				//				GQE2+=M2*sin(FwjJiaZX+PI);
			}

			/*
			//求与圆曲线的切点
			if(A1>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm1,Ym1,&at);
			PtOnCirN1=GQN1+dM*cos(FwjJiaZX+PI-at);
			PtOnCirE1=GQE1+dM*sin(FwjJiaZX+PI-at);
			}
			else
			{
			PtOnCirN1=GQN1;
			PtOnCirE1=GQE1;
			}

			if(A2>0.0)
			{
			double dM,at;//缓和曲线起点与终点之距，及连线与起点切线的夹角
			dM=xyddaa(0.0,0.0,Xm2,Ym2,&at);
			PtOnCirN2=GQN2+dM*cos(FwjJiaZX+at);
			PtOnCirE2=GQE2+dM*sin(FwjJiaZX+at);
			}
			else
			{
			PtOnCirN2=GQN2;
			PtOnCirE2=GQE2;
			}*/

		}
	}
}