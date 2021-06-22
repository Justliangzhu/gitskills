// Norm3JD.h: interface for the Norm3JD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_)
#define AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
//普通的三交点法定线计算模型：“缓圆缓型”，由连续三个交点坐标，
//中间交点对应的曲线半径和前后缓和曲线A值
class Norm3JD : public BAS_DRAW_FUN  
{
public:
//	Norm3JD();
	Norm3JD(double n1,double e1,double n0,double e0,double n2,double e2,double r,double a1,double a2);
	virtual ~Norm3JD();
private:
	//三交点坐标
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	//中间交点对应的半径、缓和曲线A值及长度
	double R,A1,A2,Lo1,Lo2;
public:
	//前后边长
	double DistJD10,DistJD02;
	//转角
	double aa;
	//左右转
	int LeftOrRight;
	//前后边的方位角
	double fb,fe;
	//前后切长
	double T1,T2;
};

#endif // !defined(AFX_NORM3JD_H__3CA589EA_569F_4179_AF9A_7DB1CCC50307__INCLUDED_)
