// BCTPP.h: interface for the BCTPP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_)
#define AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCTPP : public BAS_DRAW_FUN  
{
public:
	void EraseArc();
	double DV;
	BCTPP();
	virtual ~BCTPP();
//protected:
	//半径
	double R;
	//切线端点
	double N1,E1,N2,E2;
	//弧上一点
	double NonArc1,EonArc1;
	double NonArc2,EonArc2;
	//圆心
	double CenN,CenE;
	double startAngle,endAngle;
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//设置切线
	void SetQX(double n1,double e1,double n2,double e2);
	//设置弧上一点
	void SetFirstPointOnArc(double n,double e);
	//设置弧上一点
	void SetSecondPointOnArc(double n,double e);
	//更新
	void UpdateData();
};

#endif // !defined(AFX_BCTPP_H__AE8FAD10_4E73_4C4C_BAAD_8F1BC9A4532F__INCLUDED_)
