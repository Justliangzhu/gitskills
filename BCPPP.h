// BCPPP.h: interface for the BCPPP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_)
#define AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class BCPPP : public BAS_DRAW_FUN  
{
public:
	void EraseArc();
	BCPPP();
	virtual ~BCPPP();
//protected:
	//弧上三点(起，中，终)坐标
	double NonArc1,EonArc1,NonArc0,EonArc0,NonArc2,EonArc2;
	//半径及圆心坐标
	double CenN,CenE,R;
	//圆心与起终点连线与X轴的夹角
	double startAngle,endAngle;
	//圆心与弧中点边线与X轴夹角
	double MidAngle;
	//给定弧附近所有点//大地坐标
	AcGePoint3dArray PtArray;
	AcDbArc *pArc;
	AcDbObjectId eId;
	bool GetCirByThreePointOnArc(double &r,double &CN,double &CE,double N1,double E1,
		double N2,double E2,double N3,double E3);
	double DV;//设计车速
	double Len;
public:
	//增加弧附近点
	void AppendPointNearArc(double N,double E);
	//更新
	void UpdateData();
};

#endif // !defined(AFX_BCPPP_H__749F300B_1FBE_4C37_A428_AF350893FCE4__INCLUDED_)
