// ThreeJDCalA1A2FromT1T2R.h: interface for the ThreeJDCalA1A2FromT1T2R class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_)
#define AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
#include "SPIRAL.h"
#include "ZYLink.h"
//由三个交点坐标及中间交点对应的曲线半径和切长，计算缓和曲线的A值
class ThreeJDCalA1A2FromT1T2R : public BAS_DRAW_FUN  
{
public:
	ThreeJDCalA1A2FromT1T2R();
	virtual ~ThreeJDCalA1A2FromT1T2R();
public:
	//设置前一个交点的坐标
	void SetPreJd(double n,double e);
	//设置中间个交点的坐标
	void SetMidJd(double n,double e);
	//设置后一个交点的坐标
	void SetAftJd(double n,double e);
	//设置中间交点对应的曲线半径　
	void SetRadius(double r);
	//设置前后切长
	void SetT1T2(double t1,double t2);

	void UpdateData();//计算
	
	//中间交点对应的缓和曲线A值
	double A1,A2;
	//是否计算出错
	bool ERRFlag;
private:
	//标记是否给定所有参数
	bool OK1,OK2,OK3,OK4,OK5;
	//交点坐标
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	
	double T1,T2,R;	

	int LeftOrRight;
	//直缓点坐标
	double zhN,zhE;
	//缓直点坐标
	double hzN,hzE;
	double fb,fe;//前一条边的方位角/后一条边的文位角
	//圆心坐标
	double CenN,CenE;
	bool DieDai(double a11,double a12);
	int count;
	double func(double A1);
};

#endif // !defined(AFX_THREEJDCALA1A2FROMT1T2R_H__07DAA8D3_6582_421D_98A8_40D3FD0DC851__INCLUDED_)
