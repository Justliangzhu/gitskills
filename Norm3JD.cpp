// Norm3JD.cpp: implementation of the Norm3JD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Norm3JD.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
Norm3JD::Norm3JD()
{
}
*/

Norm3JD::Norm3JD(double n1,double e1,double n0,double e0,double n2,double e2,double r,double a1,double a2)
{
	JdN1=n1;	JdE1=e1;
	JdN0=n0;	JdE0=e0;
	JdN2=n2;	JdE2=e2;
	R=r;
	A1=a1;	A2=a2;

	Lo1=A1*A1/R;
	Lo2=A2*A2/R;

	DistJD10=xyddaa(JdN1,JdE1,JdN0,JdE0,&fb);
	DistJD02=xyddaa(JdN0,JdE0,JdN2,JdE2,&fe);
	aa=fe-fb;
	if (aa<-pi) aa=aa+2*pi;
	if (aa>pi) aa=aa-2*pi;
	if(aa<0.0)LeftOrRight=LEFT;
	else LeftOrRight=RIGHT;
	double BPMTL[10];
	getBPMTL(dms_rad(fabs(aa)),R,Lo1,Lo2,&BPMTL[0]);
	T1=BPMTL[6];
	T2=BPMTL[7];				
}

Norm3JD::~Norm3JD()
{

}
