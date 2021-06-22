// YYLuanLink.h: interface for the YYLuanLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_)
#define AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SPIRAL.h"
//卵型计算模型
class YYLuanLink : public BAS_DRAW_FUN 
{
public:
	//缓和曲线对象
	SPIRAL pSPIRAL;
	//两缓和曲线终点的坐标
	double PtOnCir1N,PtOnCir1E,PtOnCir2N,PtOnCir2E;
	
	//传入两圆心坐标和半径、左右转
	virtual void Set(double n1,double e1,double r1,
			double n2,double e2,double r2,
			int lr);
private:
	double func(double x);
	double diedai(double A1, double A2,int flag);//flag+1,递增-1递减
	double AtoCC(double AA);
	int count;
	double iDistO1O2;
public:
//private:
	//////////////////////////////////////////////////////////////////////////
	//传入数据
	double CenN1,CenE1,R1;//前一个圆
	double CenN2,CenE2,R2;//后一个圆
	int LeftOrRight;//左右转，左为-右为+
//private:
	//////////////////////////////////////////////////////////////////////////
	//内部数据
	double A,R;//缓
	//圆心距
	double DistO1O2;
	//是否出错
	bool ERRFlag;
//	//或R1<R2，两圆要交换，Exflag=-1;
//	static int Exflag;
	//计算模式 #define KNOW_A1 1 #define KNOW_A2 2 #define KNOW_K_A1_A2 12	
	double Ls;//缓
	double startL,endL;
	double Rmax,Rmin;
	double fwjO1O2,fwjO2O1;
	
public:
	static double DistThisCenToOtherGQX(double r1,double r2,double a,double &afa);
	YYLuanLink();
	~YYLuanLink();
	YYLuanLink(AcDbArc *pArc1,AcDbArc *pArc2,int lr);
	YYLuanLink(double n1,double e1,double r1,
			double n2,double e2,double r2,
			int lr);

};

#endif // !defined(AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_)
