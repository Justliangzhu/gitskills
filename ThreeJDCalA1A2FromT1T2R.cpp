// ThreeJDCalA1A2FromT1T2R.cpp: implementation of the ThreeJDCalA1A2FromT1T2R class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ThreeJDCalA1A2FromT1T2R.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern int N;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreeJDCalA1A2FromT1T2R::ThreeJDCalA1A2FromT1T2R()
{
	OK1=false;
	OK2=false;
	OK3=false;
	OK4=false;
	OK5=false;
	ERRFlag=false;
}

ThreeJDCalA1A2FromT1T2R::~ThreeJDCalA1A2FromT1T2R()
{

}

void ThreeJDCalA1A2FromT1T2R::SetPreJd(double n,double e)
{
	JdN1=n;
	JdE1=e;
	OK1=true;
}

void ThreeJDCalA1A2FromT1T2R::SetMidJd(double n,double e)
{
	JdN0=n;
	JdE0=e;
	OK2=true;
}

void ThreeJDCalA1A2FromT1T2R::SetAftJd(double n,double e)
{
	JdN2=n;
	JdE2=e;
	OK3=true;
}

void ThreeJDCalA1A2FromT1T2R::SetRadius(double r)
{
	R=r;
	OK4=true;
}

void ThreeJDCalA1A2FromT1T2R::SetT1T2(double t1,double t2)//设置前后切长
{
	T1=t1;
	T2=t2;
	OK5=true;
}

void ThreeJDCalA1A2FromT1T2R::UpdateData()//计算
{
	if(OK1 && OK2 && OK3 && OK4 && OK5)
	{
		double D1,D2;//前一条边长/后一条边长
		D1=xyddaa(JdN1,JdE1,JdN0,JdE0,&fb);
		D2=xyddaa(JdN0,JdE0,JdN2,JdE2,&fe);
		double aa=fe-fb;

		if (aa<-pi) aa=aa+2*pi;
		if (aa>pi) aa=aa-2*pi;
		if(aa<0.0)LeftOrRight=-1;
		else LeftOrRight=1;
		aa=fabs(aa);

		double BPMTL[10];
		getBPMTL(dms_rad(aa),R,0.0,0.0,&BPMTL[0]);
		double t1,t2;
		t1=BPMTL[6];//前切长
		t2=BPMTL[7];//后切长

		if(min(t1,t2)>max(T1,T2))
		{
			ERRFlag=true;
			return;
		}
		zhN=JdN0-T1*cos(fb);
		zhE=JdE0-T1*sin(fb);

		count=0;
		//if(!DieDai(0.0,R))

		//	if(!DieDai(R/3.0,R))
		double Acz = sqrt(2*aa)*R;

		if(!DieDai(0.0,Acz))
		{

			ERRFlag=true;
			return;
		}

		//		acutPrintf(L"count=%d\n",count);
	}
}

double ThreeJDCalA1A2FromT1T2R::func(double AA1)
{
	if(AA1>0.0000001)
	{
		SPIRAL pSpir(zhN,zhE,fb,AA1,VERY_MAX,R,LeftOrRight);
		CenN=pSpir.EndN+R*cos(pSpir.EndFwj+LeftOrRight*0.5*PI);
		CenE=pSpir.EndE+R*sin(pSpir.EndFwj+LeftOrRight*0.5*PI);
	}
	else
	{
		CenN=zhN+R*cos(fb+LeftOrRight*0.5*PI);
		CenE=zhE+R*sin(fb+LeftOrRight*0.5*PI);
	}	

	ZYLink ZYObj;
	ZYObj.Set(JdN2,JdE2,JdN0,JdE0,CenN,CenE,R,-1*LeftOrRight);
	if(ZYObj.ERRFlag)return -1.0;// DieDai(a11,A1);

	A2=ZYObj.pSPIRAL.A;
	hzN=ZYObj.pSPIRAL.BaseN;
	hzE=ZYObj.pSPIRAL.BaseE;

	double f,t2;
	t2=xyddaa(JdN0,JdE0,hzN,hzE,&f);

	return t2;
}

bool ThreeJDCalA1A2FromT1T2R::DieDai(double a11,double a12)
{
	int num;
	num=0;

	for(;;)
	{
		A1=0.5*(a11 + a12);
		double t2=func(A1);

		if(fabs(a11-a12)<PRECISION || fabs(t2-T2)<PRECISION)
			return true;
		else
		{
			if(t2>T2)  
				a11 = A1;
			else
				a12 = A1;
		}

		num++;
		if(num>1000)
			return false;
	}
	return true;
}