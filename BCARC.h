// BCARC.h: interface for the BCARC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_)
#define AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.H"
class BCARC : public BAS_DRAW_FUN 
{
public:
	void EraseArc();
	BCARC();
	virtual ~BCARC();
//protected:
	double R;//半径
	double N0,E0;//弧中点坐标
	double OutN,OutE;//弧中点右切线上一点坐标
	double CenN,CenE;//圆心坐标
	double startAngle,endAngle;//起终角度
	bool OK1,OK2,OK3;
	AcDbArc *pArc;
	AcDbObjectId eId;
	double Len;
public:
	//设置弧中心点坐标
	void SetMidPoint(double n0,double e0);
	//设置半径
	virtual void SetRadius(double r);
	//设置弧中点右切线上一点坐标
	void SetOutPoint(double outn,double oute);
	virtual void UpdateData();
};

#endif // !defined(AFX_BCARC_H__209CA20D_3A99_4F8E_A7BC_9FFBB122D209__INCLUDED_)
