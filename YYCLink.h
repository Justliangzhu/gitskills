// YYCLink.h: interface for the YYCLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_)
#define AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "spiral.h"
//C型计算模式
class YYCLink : public BAS_DRAW_FUN  
{
public:
	//两条缓和曲线
	SPIRAL pSPIRAL1,pSPIRAL2;
	//缓和曲线终点坐标
	double PtOnCir1N,PtOnCir1E,PtOnCir2N,PtOnCir2E;
	//传入圆弧实体的指针、两个初始的A值，左右转
	virtual void Set(AcDbArc *pArc1,AcDbArc *pArc2,double a1,double a2,int lr);

	//传入两圆的圆心坐标和半径、两个初始的A值，左右转
	virtual void Set(double n1,double e1,double r1,
			double n2,double e2,double r2,
			double a1,double a2,int lr);
//private:
	//循环次数
	int count;
	//////////////////////////////////////////////////////////////////////////
	//传入数据
	double CenN1,CenE1,R1;//前一个圆
	double CenN2,CenE2,R2;//后一个圆
	double A1,A2;//前缓，后缓
	int LeftOrRight;//左右转，左为-右为+
//private:
	//////////////////////////////////////////////////////////////////////////
	//内部数据
	int IsSModel;//是否为S型 是S为-1;;C为+1;;
	//圆心距
	double DistO1O2;
	//是否出错
	bool ERRFlag;
	//计算模式 #define KNOW_A1 1 #define KNOW_A2 2 #define KNOW_K_A1_A2 12	
	int K_A_mode;
	double K12;// A1,A2比值
	double Lo1,Lo2;//前缓，后缓
	double GQ_N,GQ_E;//两缓和曲线公切点

private:
	double func(double x);
	virtual double GetDistO1ToO2(double a1,double a2);
	double diedai(double A21, double A22);//递增A2-CC
	double AtoCC(double a);
public:
	void SetSLink(bool IsS=true);
	YYCLink();
	YYCLink(double n1,double e1,double r1,
			double n2,double e2,double r2,
			double a1,double a2,int lr);
	~YYCLink();	
	
//	YYCLink(double CenX1,double CenY1,double RR1,
//				 double CenX2,double CenY2,double RR2,
//				 double AA1,double AA2,double lr);
//
//	SPIRAL pSPIRAL1,pSPIRAL2;
//
//	bool ERRFlag;
//	void GetSpirItem(int i,double &AA, double &RR, double &x,double &y, double &fwj, double &slc, double &lr);
//	YYCLink(double x11,double y11,double x10,double y10,double x12,double y12,double x21,double y21,double x20,double y20,double x22,double y22,double lr,double Ak);
//	void KeyBoardInput();	
//	void CALCULATION(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2,double lr=0,double Ak=0);
//	YYCLink (AcDbEntity *pEnt1,AcDbEntity *pEnt2);
//	YYCLink(AcDbArc *pArc1,AcDbArc *pArc2);
//	YYCLink(AcDbCircle *pCir1,AcDbCircle *pCir2);
//	YYCLink(AcDbArc *pArc1,AcDbCircle *pCir2);
//	YYCLink(AcDbCircle *pCir1,AcDbArc *pArc2);
//	YYCLink();
//	virtual ~YYCLink();
//	YYCLink(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2);
//	double diedai(double A1, double A2);//S型，@A2－CC递增,减，增
//	double AtoCC(double AA);
//	double func(double x);
////private:
//	double PtOnCir1X,PtOnCir1Y,PtOnCir2X,PtOnCir2Y;
//	double fwj_O1P1,fwj_O2P2;
//	AcDbCircle *Cir1,*Cir2;
//	AcDbArc *Arc1,*Arc2;
//	AcDbObjectId spirID1,spirID2;
//	AcGePoint3d GQ;
//	void getlrRot();
//	int K_A_mode;
//	void Create();
//	AcGePoint3dArray points;//辅助点；
//	int LRflag;
//	int count;
//	double Xm1,Xm2;//切垂距
//	AcGePoint3d pCen1,pCen2,Pt1,Pt2;//圆心1，圆心2，与C1的切点，与C2的切点；
//	double K12,A1,A2,R1,R2,R,dC1C2,D1,D2,L1,L2,Bata1,Bata2,Fai,Afa1,Afa2;
//	//L1，L2为虚公共直线与两圆之间缓长,D1,D2为虚公共直线与两圆之距。

	
};

#endif // !defined(AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_)
