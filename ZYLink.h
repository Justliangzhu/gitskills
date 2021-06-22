// ZYLink.h: interface for the ZYLink class.
//
//////////////////////////////////////////////////////////////////////
#define Pai 3.14159265358979323846264338327
#if !defined(AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_)
#define AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SPIRAL.h"
#include "JD_CENTER.h"
//直线和圆曲线通过缓和曲线连接计算类
class ZYLink:public BAS_DRAW_FUN 
{
public:
	//传入直线的两个端点坐标经纬距，圆曲线的圆心坐标、半径，缓和曲线的转向，左转为负。
	void Set(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr);
public:
	//缓和曲线
	SPIRAL pSPIRAL;
	//是否出错（无合适的缓和曲线）
	bool ERRFlag;
private:
	//左右转，左－1，右＋1
	int LeftOrRight;
	double dR,R,A;
	//缓和曲线起终点坐标
	double BaseN,BaseE,EndN,EndE;
	//缓和曲线起终点方位角
	double EndFwj,BaseFwj;
	//迭代次数
	int count;
private:
	double func(double x);
	double diedai(double AA1, double AA2,int flag);
	double AtodR(double AA);			
public:
	//由半径和A值计算缓和曲线的转角和圆心到直线的距离
	static double DistCenToQX(double r,double a,double &afa);
	ZYLink(AcDbLine *pLine,AcDbArc *pArc,int lr=0);
	ZYLink(double N1,double E1,double fwj,double cenN,double cenE,double r,int lr);
	ZYLink(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr);
	ZYLink();
	~ZYLink();
};

#endif // !defined(AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_)
/*
bool drawmode;
bool ERRFlag;
void GetSpirItem(int i, double &AA, double &RR, AcGePoint3d &sPt, double &fwj, double &slc, int &lr,AcGePoint3d &ePt);
void getSpiralId(AcDbObjectId& Id1);
int count;
double jifen_sin(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s);
double jifen_cos(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s);
double diedai(double AA1,double AA2,int flag);
double afa_s(double afa1,int pianxiang,double s,double Ls,double Rt);
ZYLink(AcDbLine *pL1,AcDbArc *pArc,int lr=0);
ZYLink(double X0,double Y0,double fwj,double CenX,double CenY,double RR,int lr=0);

ZYLink(AcDbLine *pLine,AcDbCircle *pCircle);
ZYLink();
virtual ~ZYLink();
SPIRAL *pSpiral;
//private:
AcDbObjectId pSpirId;
void CALCULATION(int lr=0);
void getlrRot(int lr=0);
double startFWJ;
double func(double x);
double dR;
double AtodR(double AA);
AcDbArc* Arc;
AcDbCircle* Cir;
AcDbLine* Line;
AcGePoint3d startPtL,endPtL,center,startPtCur,endPtCur;
double R,A;
	double lrRot;//右偏为正1，左偏为负－1*/
