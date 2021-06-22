// FourJDLocationC_S.cpp: implementation of the FourJDLocationC_S class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "FourJDLocationC_S.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

FourJDLocationC_S::FourJDLocationC_S()
{
	OK1=false;
	OK2=false;
	OK3=false;
	ERRFlag=false;
	T12=0.0;
}

FourJDLocationC_S::~FourJDLocationC_S()
{

}

//设置四个交点坐标
void FourJDLocationC_S::SetFourJD(double n1,double e1,double n2,double e2,
								  double n3,double e3,double n4,double e4)
{
	OK1=true;
	JdN1=n1;	JdE1=e1;
	JdN2=n2;	JdE2=e2;
	JdN3=n3;	JdE3=e3;
	JdN4=n4;	JdE4=e4;	
}

//设置前一个交点的参数
void FourJDLocationC_S::SetPreJDCS(double r,double a1,double a2)
{
	OK2=true;
	R1=r;
	A11=a1;
	A12=a2;
	Lo11=A11*A11/R1;
	Lo12=A12*A12/R1;
}

//设置后一个交点的参数
void FourJDLocationC_S::SetAftJDCS(double r,double a1,double a2)
{
	OK3=true;
	R2=r;
	A21=a1;
	A22=a2;
	Lo21=A21*A21/R2;
	Lo22=A22*A22/R2;
}

//计算A12，A21//计算前一个曲线的后缓　和　后一个曲线的前缓
void FourJDLocationC_S::UpdateData()
{
	if(OK1 && OK2 && OK3)
	{
		xyddaa(JdN1,JdE1,JdN2,JdE2,&Fwj12);
		DistJD23=xyddaa(JdN2,JdE2,JdN3,JdE3,&Fwj23);
		xyddaa(JdN3,JdE3,JdN4,JdE4,&Fwj34);

		aa1=Fwj23-Fwj12;
		if (aa1<-pi) aa1=aa1+2*pi;
		if (aa1>pi) aa1=aa1-2*pi;
		aa1=fabs(aa1);
		aa2=Fwj34-Fwj23;
		if (aa2<-pi) aa2=aa2+2*pi;
		if (aa2>pi) aa2=aa2-2*pi;
		aa2=fabs(aa2);			

		//////////////////////////////////////////////////////////////////////////		
		getBPMTL(dms_rad(aa1),R1,Lo11,0.0,&BPMTL[0]);
		T11=BPMTL[6];//前切长
		T12=BPMTL[7];//后切长

		getBPMTL(dms_rad(aa2),R2,0.0,Lo22,&BPMTL[0]);
		T21=BPMTL[6];//前切长
		T22=BPMTL[7];//后切长

		if(T12+T21>DistJD23+1.0)
		{
			ads_printf(L"错误: 前后曲线穿袖!\n");
			ERRFlag=true;	return;
		}

		if(A12>0.00001 && A21>0.00001)
			//如果A12与A21都大于0，按比例计算
		{
			count=0;
			double K=A12/A21;
			A12=DieDai1(0.1*min(R1,R2),2*min(R1,R2),K);

			//A12=DieDai1(0.0,2.0*min(R1,R2),K);
			//	acutPrintf(L"count=%d\n",count);
			if(A12<0.0)
			{
				ads_printf(L"错误: %lf %lf 无法计算A12!\n",R1,R2);
				ERRFlag=true;	return;
			}
			A21=A12/K;
		}		
		//		else if(fabs(A12)<0.00001 || A21<-0.1)
		else if((A12>-0.00001 && A21<-0.1)|| (fabs(A12)<0.00001 && A21>0.0001))
			//如果A12大于等于0，A21小于0，固定A12，计算A21
		{
			count=0;
			getBPMTL(dms_rad(aa2),R2,0.0,Lo22,&BPMTL[0]);
			T21=BPMTL[6];//前切长
			T22=BPMTL[7];//后切长

			if(T12+T21>DistJD23)
			{
				ads_printf(L"错误: 前后曲线穿袖!\n");
				ERRFlag=true;	return;
			}

			A21=DieDai2(0.1*min(R1,R2),2*min(R1,R2));
			//A21=DieDai2(0.0,2.0*min(R1,R2));
			//			acutPrintf(L"count=%d\n",count);
			if(A21<0.0)
			{
				ads_printf(L"错误: A21计算有误!\n");
				ERRFlag=true;	return;
			}
		}
		else if((A21>-0.00001 && A12<-0.1) || (fabs(A21)<0.00001 && A12>0.0001))//固定A21，计算A12
		{
			count=0;
			getBPMTL(dms_rad(aa1),R1,Lo11,0.0,&BPMTL[0]);
			T11=BPMTL[6];//前切长
			T12=BPMTL[7];//后切长

			if(T12+T21>DistJD23)
			{
				ads_printf(L"错误: 前后曲线穿袖!\n");
				ERRFlag=true;	return;
			}

			A12=DieDai3(0.1*min(R1,R2),2*min(R1,R2));
			//A12=DieDai3(0.0,1.2*R2);
			//			acutPrintf(L"count=%d\n",count);
			if(A12<0.0)
			{
				ads_printf(L"错误: A12计算有误!\n");
				ERRFlag=true;	return;
			}
		}
	}

}

double FourJDLocationC_S::DieDai1(double a121,double a122,double k)
{
	int num;
	num=0;
	for(;;)
	{
		double a12=0.5*(a121+a122);

		double a21=a12/k;
		Lo12=a12*a12/R1;
		Lo21=a21*a21/R2;
		//////////////////////////////////////////////////////////////////////////		
		getBPMTL(dms_rad(aa1),R1,Lo11,Lo12,&BPMTL[0]);

		T11=BPMTL[6];//前切长
		T12=BPMTL[7];//后切长

		getBPMTL(dms_rad(aa2),R2,Lo21,Lo22,&BPMTL[0]);
		T21=BPMTL[6];//前切长
		T22=BPMTL[7];//后切长

		double t=T12+T21;
		if(fabs(t-DistJD23)<PRECISION)
			return a12;
		else
		{
			if(t>DistJD23)
				a122  = a12;   
			//	return DieDai1(a121,a12,k);
			else
				a121  = a12;   
			//		return DieDai1(a12,a122,k);
		}
		num++;
		if(num>1000)
			return false;
	}
	return true;


	//////////////////////////////////////////////////////////////////////////////////////////////


}

double FourJDLocationC_S::DieDai2(double a211,double a212)
{
	int num;
	num=0;
	for(;;)
	{
		double a21=0.5*(a211+a212);
		Lo21=a21*a21/R2;

		getBPMTL(dms_rad(aa2),R2,Lo21,Lo22,&BPMTL[0]);
		T21=BPMTL[6];//前切长

		double t=T12+T21;
		if(fabs(t-DistJD23)<PRECISION)
			return a21;
		else
		{
			if(t>DistJD23)
				a212 = a21;
			//return DieDai2(a211,a21);
			else
				a211 = a21;
			//return DieDai2(a21,a212);
		}
		num++;
		if(num>1000)
			return false;
	}
	return true;
}

double FourJDLocationC_S::DieDai3(double a121,double a122)
{
	int num;
	num=0;
	for(;;)
	{
		double a12=0.5*(a121+a122);
		Lo12=a12*a12/R1;

		getBPMTL(dms_rad(aa1),R1,Lo11,Lo12,&BPMTL[0]);
		T12=BPMTL[7];//后切长

		double t=T12+T21;
		if(fabs(t-DistJD23)<PRECISION)
			return a12;
		else
		{
			if(t>DistJD23)
				a122 = a12;
			//return DieDai3(a121,a12);
			else
				a121 = a12;
			//return DieDai3(a12,a122);
		}
		num++;
		if(num>1000)
			return false;
	}
	return true;
}

/*

void FourJDLocationC_S::SetT12(double t12)
{
	T12=t12;
}*/
