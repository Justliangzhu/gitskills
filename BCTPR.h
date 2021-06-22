// BCTPR.h: interface for the BCTPR class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_)
#define AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCTPR : public BAS_DRAW_FUN  
{
public:
	void EraseArc();		
	BCTPR();
	virtual ~BCTPR();
//protected:
	//半径
	double R;
	//切线端点
	double N1,E1,N2,E2;
	//弧上一点
	double NonArc,EonArc;
	//圆心
	double CenN,CenE;
	double startAngle,endAngle;
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//设置半径
	void SetRadius(double r);
	//设置切线
	void SetQX(double n1,double e1,double n2,double e2);
	//设置弧上一点
	void SetPointOnArc(double n,double e);
	//更新
	void UpdateData();
};

#endif // !defined(AFX_BCTPR_H__47530C59_B321_4A26_A850_3FE755668067__INCLUDED_)
