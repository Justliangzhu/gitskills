// JDLuanLink.h: interface for the JDLuanLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_)
#define AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "YYLuanLink.h"
#include "ZYLink.h"
#include "SPIRAL.h"
//由连接三个交点及第一个交点的后切长计算卵型
class JDLuanLink : public BAS_DRAW_FUN  
{
public:
	JDLuanLink();
	virtual ~JDLuanLink();
	double GetCenter_dR2(double a);
	double func(double x);
private:
	double PreR,PreN,PreE;//前一个交点的半径，坐标
	double CurR,CurN,CurE;//当前交点的半径，坐标
	double AftN,AftE;//后一个交点的坐标
	double PreT;//前一个交点的后切长
	//////////////////////////////////////////////////////////////////////////
	double PreFwj,AftFwj;//前一条切线的方位角
	int LeftOrRight;//左右转
	bool OKPreJD,OKCurJD,OKAftJD,OKPreT;//是否给定这些参数
	int count;//迭代次数
	double DieDai(double a11,double a12);
public:
	bool ERRFlag;
	//当前交点对应圆曲线的圆心坐标
	double CurCenN,CurCenE;
	//缓圆点坐标
	double HYN,HYE;
	//弧长、后切长、前A、后A
	double ArcLength,AftT,PreA,AftA;
	//设置前一个交点的坐标及半径
	void SetPreJD(double n,double e,double r);
	//设置当前交点的坐标、半径和前后缓和曲线的A值
	void SetCurJD(double n,double e,double r,double a1,double a2);
	//设置后一个交点的坐标
	void SetAftJD(double n,double e);
	//设置前一个交点的后切长
	void SetPreT(double t);
	//开始计算缓和缓和曲线
	void UpdateData();

	//////////////////////////////////////////////////////////////////////////
	//由三个交点，第一二个交点构成卵型，前一个半径，后一个半径，前A，后A，返回后一个圆的圆心,圆曲线长	
	static void GetCenter(double JdN1,double JdE1,double JdN0,double JdE0,
		double JdN2,double JdE2,double r1,double r2,double a1,double a2,
		double &cenN,double &cenE,double &l,double &T2);
	void SetPreFwj(double pfwj);
	void SetAftFwj(double pfwj);
};

#endif // !defined(AFX_JDLUANLINK_H__047ABBC6_50AC_40F4_9037_E43D10A3BC9C__INCLUDED_)
