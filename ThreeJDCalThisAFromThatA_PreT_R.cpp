// ThreeJDCalThisAFromThatA_PreT_R.cpp: implementation of the ThreeJDCalThisAFromThatA_PreT_R class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ThreeJDCalThisAFromThatA_PreT_R.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ThreeJDCalThisAFromThatA_PreT_R::ThreeJDCalThisAFromThatA_PreT_R()
{
	T1=0.0;
	T2=0.0;
	afa=0.0;
	A1=0.0;
	A2=0.0;
	OKAfa=false;
	OKA1=false;
	OKA2=false;
	OKPreT=false;
}

ThreeJDCalThisAFromThatA_PreT_R::~ThreeJDCalThisAFromThatA_PreT_R()
{

}

void ThreeJDCalThisAFromThatA_PreT_R::SetAfa(double aa)
{
	OKAfa=true;
	afa=fabs(aa);
}

void ThreeJDCalThisAFromThatA_PreT_R::SetPreT_R(double t1,double r)
{
	if(t1<0.001 || r<0.001)return;
	OKPreT=true;	
	T1=t1;
	R=r;
}
void ThreeJDCalThisAFromThatA_PreT_R::SetA1(double a)
{
	if(a<0.0)return;
	OKA1=true;
	A1=a;
}

void ThreeJDCalThisAFromThatA_PreT_R::SetA2(double a)
{
	if(a<0.0)return;
	OKA2=true;
	A2=a;
}

bool ThreeJDCalThisAFromThatA_PreT_R::GetA1(double &a1)
{
	if(OKAfa && OKPreT && OKA2)
	{
		count=0;
		a1=DieDaiA1(0.1*R,2*R);
		//a1=DieDaiA1(0.0,R);
		//		acutPrintf(L"count=%d\n",count);
		if(a1<0.0)return false;
		else return true;
	}
	else return false;
}

bool ThreeJDCalThisAFromThatA_PreT_R::GetA2(double &a2)
{
	if(OKAfa && OKPreT && OKA1)
	{
		count=0;
		a2=DieDaiA2(0.1*R,2*R);
		a2=DieDaiA2(R/3,R);
		//		acutPrintf(L"count=%d\n",count);
		if(a2<0.0)return false;
		else return true;
	}
	else return false;
}

double ThreeJDCalThisAFromThatA_PreT_R::DieDaiA1(double a11,double a12)
{
	int num;
	double a1;
	num=0;

	for(;;)
	{
		num++;
		if(fabs(a11-a12)<PRECISION)return a11;
		a1=0.5*(a11+a12);
		double BPMTL[10];
		getBPMTL(dms_rad(afa),R,a1*a1/R,A2*A2/R,&BPMTL[0]);
		double t1;
		t1=BPMTL[6];//前切长

		if(fabs(t1-T1)<PRECISION)return a1;
		else
		{
			if(t1>T1)
				a12 = a1;
			//				return DieDaiA1(a11,a1);
			else 
				a11 = a1;
			//				return DieDaiA1(a1,a12);
		}
		if(num > 1000)
			return a1;
	}
	return a1;
}

double ThreeJDCalThisAFromThatA_PreT_R::DieDaiA2(double a21,double a22)
{
	int num;
	double a2;
	num=0;

	for(;;)
	{
		num++;
		if(fabs(a21-a22)<PRECISION)return a21;
		a2=0.5*(a21+a22);
		double BPMTL[10];
		getBPMTL(dms_rad(afa),R,A1*A1/R,a2*a2/R,&BPMTL[0]);
		double t1;
		t1=BPMTL[6];//前切长

		if(fabs(t1-T1)<PRECISION)return a2;
		else
		{
			if(t1>T1)
				a22 = a2;
			//				return DieDaiA2(a21,a2);
			else 
				a21 = a2;
			//				return DieDaiA2(a2,a22);
		}
		if(num > 1000)
			return a2;
	}
	return a2;
}
