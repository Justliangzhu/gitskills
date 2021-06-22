// FourJDLocationC_S.h: interface for the FourJDLocationC_S class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURJDLOCATIONC_S_H__2C4EFC32_CBA9_4E36_92DF_85755B7D026D__INCLUDED_)
#define AFX_FOURJDLOCATIONC_S_H__2C4EFC32_CBA9_4E36_92DF_85755B7D026D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"
//由四个交点计算C型或S型连接
class FourJDLocationC_S : public BAS_DRAW_FUN 
{
public:
	FourJDLocationC_S();
	virtual ~FourJDLocationC_S();
	//////////////////////////////////////////////////////////////////////////
	//设置四个交点坐标
	void SetFourJD(double n1,double e1,double n2,double e2,double n3,double e3,double n4,double e4);
	//设置前一个交点的参数
	void SetPreJDCS(double r,double a1,double a2);
	//设置后一个交点的参数
	void SetAftJDCS(double r,double a1,double a2);
	//计算A12，A21//计算前一个曲线的后缓　和　后一个曲线的前缓
	void SetT11(double t11);
/*
	void SetT12(double t12);
*/
	void UpdateData();
	//错误标志
	bool ERRFlag;
	double A12,A21;
	bool T12OK,T11OK;
//protected:
	//第二、三个交点构成S或C型
	//四个交点坐标
	double JdN1,JdE1,JdN2,JdE2,JdN3,JdE3,JdN4,JdE4;
	//前一个曲线参数
	double R1,A11,Lo11,Lo12;
	//后一个曲线参数
	double R2,A22,Lo21,Lo22;
	bool OK1,OK2,OK3;
private:
	int count;
	//两个曲线的前后切长
	double T11,T12,T21,T22;
	//前后曲线的转角
	double aa1,aa2;
	//第二、三个交点之距
	double DistJD23;
	double Fwj12,Fwj23,Fwj34;
	double BPMTL[10];
	
	double DieDai1(double a121,double a122,double k);
	double DieDai2(double a211,double a212);
	double DieDai3(double a121,double a122);
		
/*
	double DieDai4(double r21,double r22);
*/
};

#endif // !defined(AFX_FOURJDLOCATIONC_S_H__2C4EFC32_CBA9_4E36_92DF_85755B7D026D__INCLUDED_)
