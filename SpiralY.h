// SpiralY.h: interface for the SpiralY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIRALY_H__6D5BD92A_5AA6_4949_8BAA_8006958A3042__INCLUDED_)
#define AFX_SPIRALY_H__6D5BD92A_5AA6_4949_8BAA_8006958A3042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SpiralX.h"

class SpiralY : public SpiralX  
{
public:
	SpiralY();
	virtual ~SpiralY();
	virtual double func(double a);	
	double SpiralGetY(double A,double l);
};

#endif // !defined(AFX_SPIRALY_H__6D5BD92A_5AA6_4949_8BAA_8006958A3042__INCLUDED_)
