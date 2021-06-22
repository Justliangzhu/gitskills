// YYSLink.h: interface for the YYSLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYSLINK_H__71B626D8_5739_4C63_AE6A_0007DE96A9BD__INCLUDED_)
#define AFX_YYSLINK_H__71B626D8_5739_4C63_AE6A_0007DE96A9BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "YYCLink.h"

//S型计算模型，与C型似，故从C型计算模型继承
class YYSLink: public YYCLink 
{
public:
	YYSLink();
	YYSLink(double n1,double e1,double r1,
		double n2,double e2,double r2,
		double a1,double a2,int lr);
	~YYSLink();	
};

#endif // !defined(AFX_YYSLINK_H__71B626D8_5739_4C63_AE6A_0007DE96A9BD__INCLUDED_)
/*
public:
	YYSLink(double CenX1,double CenY1,double r1,
		double CenX2,double CenY2,double r2,
		double AA1,double AA2,double lr);
	
	void GetSpirItem(int i,double &AA, double &RR, double &x,double &y, double &fwj, double &slc, double &lr);
	bool ERRFlag;
	void KeyBoardInput();
	void getSpiralId(AcDbObjectId&,AcDbObjectId&);
	double diedai(double A1, double A2);//S型，@A2－CC递增,减，增
	YYSLink(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2);
	void CALCULATION(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2,double lr=0,double Ak=0);
	YYSLink(double x11,double y11,double x10,double y10,double x12,double y12,double x21,double y21,double x20,double y20,double x22,double y22,double lr,double Ak);
	YYSLink (AcDbEntity *pEnt1,AcDbEntity *pEnt2);
	YYSLink(AcDbArc *pArc1,AcDbArc *pArc2);
	YYSLink(AcDbCircle *pCir1,AcDbCircle *pCir2);
	YYSLink(AcDbArc *pArc1,AcDbCircle *pCir2);
	YYSLink(AcDbCircle *pCir1,AcDbArc *pArc2);
	YYSLink();
	virtual ~YYSLink();
	double AtoCC(double AA);
	double func(double x);
	SPIRAL *pSPIRAL1,*pSPIRAL2;
	double PtOnCir1X,PtOnCir1Y,PtOnCir2X,PtOnCir2Y;	
	//private:
	double fwj_O1P1,fwj_O2P2;
	AcGePoint3d GQ;
	AcDbCircle *Cir1,*Cir2;
	AcDbArc *Arc1,*Arc2;
	AcDbObjectId spirID1,spirID2;
	int K_A_mode;
	void Create();
	void getlrRot();
	AcGePoint3dArray points;//辅助点；
	double LRflag;//顺时针为-1；
	int count;
	double Xm1,Xm2;//切垂距
	AcGePoint3d pCen1,pCen2,Pt1,Pt2;//圆心1，圆心2，与C1的切点，与C2的切点；
	double K12,A1,A2,R1,R2,R,dC1C2,D1,D2,L1,L2,Bata1,Bata2,Fai,Afa1,Afa2;
	//L1，L2为虚公共直线与两圆之间缓长,D1,D2为虚公共直线与两圆之距。*/
