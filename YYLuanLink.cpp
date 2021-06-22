// YYLuanLink.cpp: implementation of the YYLuanLink class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "YYLuanLink.h"
#include "math.h"
#define Pai 3.14159265358979323846264338327
#include "acedads.h"
#include "JD_CENTER.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
YYLuanLink::YYLuanLink()
{
	ERRFlag=false;

}

YYLuanLink::~YYLuanLink()
{

}

YYLuanLink::YYLuanLink(AcDbArc *pArc1,AcDbArc *pArc2,int lr)
{
	double n1,e1,r1,n2,e2,r2;

	n1=pArc1->center().y;
	e1=pArc1->center().x;
	r1=pArc1->radius();

	n2=pArc2->center().y;
	e2=pArc2->center().x;
	r2=pArc2->radius();

	//	Set(n1,e1,r1,n2,e2,r2,lr);
	//////////////////////////////////
	//	if(ERRFlag)return;

	JdCenter * jdcenay=new JdCenter[4];
	int i=0;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].LeftOrRight=lr;
	jdcenay[i].IsOrNotHuiTou=0;


	i=1;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=n1;
	jdcenay[i].E=e1;
	jdcenay[i].R=r1;
	jdcenay[i].A1=0.0;
	jdcenay[i].LinkAfter=Luan_Link;
	jdcenay[i].LeftOrRight=lr;
	i=2;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=n2;
	jdcenay[i].E=e2;
	jdcenay[i].R=r2;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].A2=0.0;
	jdcenay[i].LeftOrRight=lr;
	i=3;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].IsOrNotHuiTou=0;
	/////////////////////////////////////
	double EndJiao1,EndJiao2;
	if(lr<0.0)
	{
		EndJiao1=pArc1->startAngle();
		EndJiao2=pArc2->endAngle();
	}
	else
	{
		EndJiao1=pArc1->endAngle();
		EndJiao2=pArc2->startAngle();
	}
	EndJiao1=2.5*PI-EndJiao1;
	EndJiao2=2.5*PI-EndJiao2;

	jdcenay[0].N=n1+r1*cos(EndJiao1);
	jdcenay[0].E=e1+r1*sin(EndJiao1);

	jdcenay[3].N=n2+r2*cos(EndJiao2);
	jdcenay[3].E=e2+r2*sin(EndJiao2);
	/////////////////////////////////////
	JD_CENTER *ob=new JD_CENTER;
	ob->SetJdCenterArray(jdcenay,4);
	delete []jdcenay;	
	BAS_DRAW_FUN::AddEntityToDbs(ob);

}

YYLuanLink::YYLuanLink(double n1,double e1,double r1,
					   double n2,double e2,double r2,
					   int lr)
{
	Set(n1,e1,r1,n2,e2,r2,lr);
}
//int YYLuanLink:: Exflag=1;

void YYLuanLink::Set(double n1,double e1,double r1,
					 double n2,double e2,double r2,
					 int lr)
{
	ERRFlag=false;
	//前一个圆
	CenN1=n1;
	CenE1=e1;
	R1=r1;

	//后一个圆
	CenN2=n2;
	CenE2=e2;
	R2=r2;
	//左右转，左为-右为+
	LeftOrRight=lr;	

	DistO1O2=xyddaa(CenN1,CenE1,CenN2,CenE2,&fwjO1O2);

	fwjO2O1=fwjO1O2+PI;

	if(DistO1O2>fabs(R1-R2) || DistO1O2<PRECISION)//交叉或同心
	{
		ads_printf(L"错误: %lf %lf 两圆交叉或同心,无法设计卵形!\n",DistO1O2,R1-R2);
		ERRFlag=true;
		return;
	}

	Rmax=R1>R2?R1:R2;
	Rmin=R1<R2?R1:R2;

	R=Rmax*Rmin/(Rmax-Rmin);

	double ax,bx,cx,tol,xmin;//AX：实型变量，IN OUT，开始存放初始区间端点，调用后存入极小值点所在区间的一个端点；//BX：实型变量，IN　OUT，IN初始区间端点，OUT极小值点的初始近似//CX：实型变量，OUT极小值点所在区间的另一端点//FA　FB　FC　实型变量，OUT，函数在AX　BX　CX处的值//TOL：精度	//XMIN：实型变量，OUT存放极小值点的横坐标
	tol=1.0e-5;
	ax=0.002*Rmin;
	bx=0.75*Rmin;
	cx=3*Rmax;
	golden(ax,bx,cx,tol,xmin);
	//	acutPrintf(L"xmin=%lf\n",xmin);

	if(xmin<=0)
	{
		ads_printf(L"错误: 计算卵形曲线连接缓和曲线 A有误!\n");
		ERRFlag=true;return;
	}
	//判别迭代区间;
	double cc0,cc1,cc2;
	cc0 = AtoCC(ax);
	cc1 = AtoCC(xmin);
	cc2 = AtoCC(cx);

	count=0;
	if(DistO1O2<cc0 && DistO1O2>cc1)
		A=diedai(ax,xmin,-1);
	else if(DistO1O2>cc1 && DistO1O2<cc2)
		A=diedai(xmin,cx,1);
	else
	{
		ads_printf(L"错误: 计算卵形曲线连接缓和曲线 A有误!\n");
		ERRFlag=true;return;
	}

	//ads_printf(L"%lf cc0=%lf %lf %lf %lf\n",DistO1O2,cc0,cc1,cc2,A);



	//A=diedai(ax,xmin,-1);
	//	acutPrintf(L"count=%d\n",count);
	//	A=diedai(0.0,Rmin,-1);

	//acutPrintf(L"卵形曲线连接缓和曲线 A=%lf\n",A);//

	if(A<0)
	{
		ads_printf(L"错误: 计算卵形曲线连接缓和曲线 A有误!\n");
		ERRFlag=true;return;
	}
	double BigCenN,BigCenE,SmallCenN,SmallCenE;
	//大小圆的圆心
	BigCenN = R1>R2 ? CenN1:CenN2;
	BigCenE = R1>R2 ? CenE1:CenE2;
	SmallCenN = R1<R2 ? CenN1:CenN2;
	SmallCenE = R1<R2 ? CenE1:CenE2;

	LeftOrRight = R1>R2 ? LeftOrRight:-1*LeftOrRight;

	//大圆圆心到小圆圆心的方位角
	double fwjBigtoSmall = R1>R2 ? fwjO1O2:fwjO2O1;
	double PtOnBigN,PtOnBigE,PtOnSmallN,PtOnSmallE;

	Ls=A*A/R;//
	startL=A*A/Rmax;
	endL=A*A/Rmin;
	double Bigaj=startL*0.5/Rmax;
	double Smallaj=endL*0.5/Rmin;
	int BigSmallFlag=R1>R2?1:-1;
	double startFwj,endfwj;//缓和曲线起点方位角

	double Dmax,Dmin;//放大后半径(公切线与圆心的距离)
	Dmax=spiral_y(A,startL)+Rmax*cos(Bigaj);//圆心到直线之距
	Dmin=spiral_y(A,endL)+Rmin*cos(Smallaj);//圆心到直线之距
	double afa;
	if(Dmax>=Dmin)
		afa=acos(fabs(Dmax-Dmin)/DistO1O2);
	else
		afa=PI-acos(fabs(Dmax-Dmin)/DistO1O2);
	startFwj=fwjBigtoSmall-LeftOrRight*(afa-Bigaj)+LeftOrRight*0.5*PI;
	endfwj = fwjBigtoSmall+LeftOrRight*(Smallaj-afa)+LeftOrRight*0.5*PI;
	//缓和曲线与大小圆的切点
	PtOnBigN=BigCenN+Rmax*cos(fwjBigtoSmall-LeftOrRight*(afa-Bigaj));
	PtOnBigE=BigCenE+Rmax*sin(fwjBigtoSmall-LeftOrRight*(afa-Bigaj));
	PtOnSmallN=SmallCenN+Rmin*cos(fwjBigtoSmall+LeftOrRight*(Smallaj-afa));
	PtOnSmallE=SmallCenE+Rmin*sin(fwjBigtoSmall+LeftOrRight*(Smallaj-afa));

	PtOnCir1N = R1>R2 ? PtOnBigN:PtOnSmallN;
	PtOnCir1E = R1>R2 ? PtOnBigE:PtOnSmallE;
	PtOnCir2N = R1<R2 ? PtOnBigN:PtOnSmallN;
	PtOnCir2E = R1<R2 ? PtOnBigE:PtOnSmallE;

	if(R1>R2)
		pSPIRAL.Set(PtOnBigN,PtOnBigE,startFwj,A,Rmax,Rmin,LeftOrRight);
	else
		pSPIRAL.SetByEnd(PtOnSmallN,PtOnSmallE,endfwj,A,Rmin,Rmax,LeftOrRight);
}

double YYLuanLink::diedai(double A1, double A2,int flag)//flag+1,递增-1递减
{
	double A0,CC,CC0;
	int num;
	num=0;

	for(;;)
	{
		num++;
		A0=(A1+A2)/2.0;
		CC=DistO1O2;//圆心距
		//count++;	
		CC0=AtoCC(A0);
		CC0=fabs(CC0);
		if(CC0<=0)return -1;

		if(num>10000)return -1;
		if(fabs(CC0-CC)<1.0E-12)return A0;
		else if(fabs(A1-A2)<1.0E-12)return -1;	

		else if(CC0>CC)
		{
			if(flag>0)
			{
				A2 = A0;
				flag = 1;
			}
			//				return diedai(A1,A0,1);
			else 
			{
				A1 = A0;
				flag = -1;
			}
			//				return diedai(A0,A2,-1);
		}
		else if(CC0<CC)
		{
			if(flag>0)
			{
				A1 = A0;
				flag = 1;
			}
			//				return diedai(A0,A2,1);
			else  
			{
				A2 = A0;
				flag = -1;
			}
			//			return	diedai(A1,A0,-1);		
		}
	}
	return -1;
}

double YYLuanLink::AtoCC(double AA)
{
	//	return AA*iDistO1O2;
	double LL,LL1,LL2,CC,XXm1,XXm2,X1,X2,Y1,Y2,t1,t2,dR1,dR2;
	LL=AA*AA/R;
	LL1=AA*AA/Rmax;
	LL2=LL+LL1;

	X1=spiral_x(AA,LL1);
	X2=spiral_x(AA,LL2);
	Y1=spiral_y(AA,LL1);
	Y2=spiral_y(AA,LL2);

	t1=0.5*LL1/Rmax;
	t2=0.5*LL2/Rmin;

	dR1=Y1+Rmax*cos(t1);
	dR2=Y2+Rmin*cos(t2);

	XXm1=X1-Rmax*sin(t1);
	XXm2=X2-Rmin*sin(t2);

	double dX,dY;
	dX=fabs(XXm1-XXm2);
	dY=fabs(dR1-dR2);

	if(XXm1<0||XXm2<0)return -1;

	CC=sqrt(dX*dX+dY*dY);

	if(AA<0||CC<=0)
	{
		return 1;
	}
	/*
	//////////////////////////////////////////////////////////////////////////	
	SPIRAL pSpir(0.0,0.0,0.0,AA,Rmax,Rmin,1);
	double CenNmax,CenEmax,CenNmin,CenEmin;

	//	acutPrintf(L"fwj=%lf\n",pSpir.StartFwj);
	CenNmax=Rmax*cos(0.5*PI);
	CenEmax=Rmax*sin(0.5*PI);
	CenNmin=pSpir.EndN+Rmin*cos(pSpir.EndFwj+0.5*PI);
	CenEmin=pSpir.EndE+Rmin*sin(pSpir.EndFwj+0.5*PI);
	double f;
	iDistO1O2=xyddaa(CenNmax,CenEmax,CenNmin,CenEmin,&f);

	acutPrintf(L"CC: %lf,%lf,%lf\n",CC,iDistO1O2,DistO1O2); 
	//////////////////////////////////////////////////////////////////////////	
	//	return iDistO1O2;
	*/
	return CC;	
}

double YYLuanLink::func(double x)
{
	if(x<0){return -1;}
	return AtoCC(x);
}

//求一个圆心(r2)到另一个圆(r1)与缓和曲线公切线的距离
//不完整缓和曲线的转角
double YYLuanLink::DistThisCenToOtherGQX(double r1, double r2, double a,double &afa)
{
	double tmpx,tmpy,tmpdist;
	double x1,y1,x2,y2,cx,cy;

	if(r1>r2)
	{
		SPIRAL pSpir(0.0,0.0,0.0,a,r1,r2,-1);
		x1=pSpir.StartN;
		y1=pSpir.StartE;

		x2=x1+1000.0*cos(pSpir.StartFwj);
		y2=y1+1000.0*sin(pSpir.StartFwj);

		cx=pSpir.EndN+r2*cos(pSpir.EndFwj-0.5*PI);
		cy=pSpir.EndE+r2*sin(pSpir.EndFwj-0.5*PI);

		//afa=fabs(pSpir.StartFwj-pSpir.EndFwj);
		afa = ZXJ(pSpir.StartFwj,pSpir.EndFwj);
		tmpx=cx;
		tmpy=cy;
		return DistToLine(x1,y1,x2,y2,tmpx,tmpy,tmpdist);
	}
	else
	{
		SPIRAL pSpir(0.0,0.0,0.0,a,r2,r1,-1);
		x1=pSpir.EndN;
		y1=pSpir.EndE;

		x2=x1+1000.0*cos(pSpir.EndFwj);
		y2=y1+1000.0*sin(pSpir.EndFwj);

		cx=pSpir.StartN+r2*cos(pSpir.StartFwj-0.5*PI);
		cy=pSpir.StartE+r2*sin(pSpir.StartFwj-0.5*PI);

		//	afa=fabs(pSpir.StartFwj-pSpir.EndFwj);
		afa = ZXJ(pSpir.StartFwj,pSpir.EndFwj);
		tmpx=cx;
		tmpy=cy;
		return DistToLine(x1,y1,x2,y2,tmpx,tmpy,tmpdist);
	}	
}
