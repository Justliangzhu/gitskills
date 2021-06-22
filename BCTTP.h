// BCTTR.h: interface for the BCTTR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCTTR_H__21BD0451_B39D_4DEB_A319_937A8BA3EF78__INCLUDED_)
#define AFX_BCTTR_H__21BD0451_B39D_4DEB_A319_937A8BA3EF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BCARC.h"

class BCTTP : public BCARC  
{
public:
	void EraseArc();
	BCTTP();
	virtual ~BCTTP();
	double Len;
protected:
	double DV;
	//三个交点
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	//圆弧上一点
	double NonArc,EonArc;

public:
	//设置三个交点
	void SetThreeJD(double n1,double e1,double n0,double e0,double n2,double e2);
	//设置圆弧上一点
	void SetPointOnArc(double n,double e);
	//更新
	void UpdateData();
};

#endif // !defined(AFX_BCTTR_H__21BD0451_B39D_4DEB_A319_937A8BA3EF78__INCLUDED_)
