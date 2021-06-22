// SpiralY.cpp: implementation of the SpiralY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SpiralY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpiralY::SpiralY()
{

}

SpiralY::~SpiralY()
{

}

double SpiralY::func(double a)
{
	if(a<=0.0)return 0.0;
	else return sin(a)/sqrt(a);
}	

double SpiralY::SpiralGetY(double A,double l)
{
	return SpiralX::SpiralGetX(A,l);
}