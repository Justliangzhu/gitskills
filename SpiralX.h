// SpiralX.h: interface for the SpiralX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIRALX_H__823E37E4_B7F3_46E5_BC33_43BBC2A4FFF1__INCLUDED_)
#define AFX_SPIRALX_H__823E37E4_B7F3_46E5_BC33_43BBC2A4FFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "math.h"
class SpiralX  
{
public:
	void polint(double xa[], double ya[], int n, double x, double& y, double& dy);		
	double SpiralGetX(double A,double l);
	SpiralX();
	virtual ~SpiralX();
	virtual double func(double a);
	void trapzd(double a, double b, double& s, int n);
	void qromb(double a, double b, double& ss);
	
};

#endif // !defined(AFX_SPIRALX_H__823E37E4_B7F3_46E5_BC33_43BBC2A4FFF1__INCLUDED_)
