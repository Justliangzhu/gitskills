// BasicFun.h: interface for the CBasicFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICFUN_H__596081B3_5044_4DA3_9256_DA0124EF0085__INCLUDED_)
#define AFX_BASICFUN_H__596081B3_5044_4DA3_9256_DA0124EF0085__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define pi 3.1415926535897932384626433832795
#include <math.h>


class CBasicFun  
{
public:
	CBasicFun();
	virtual ~CBasicFun();

	struct Jd
	{
		double dNJd;
		double dEJd;
		double dLToBasePt;
	}m_Jd;

	/*计算两点之间的距离Dis和两点构成的直线与x的夹角dAngleByX */
	static double DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend);
	
	/*把计算出来的角度转换为0-2pi范围内的方位角*/
	static void FormFwj(double& dFwj);

	/*给定两个两个方位角得出转向角*/
	static double ZxAngle(double dLineAngle2,double dLineAngle1);	

	/*求点到直线距离*/
	static double DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);
	
	/*从一点向直线作垂线求垂足*/
	static void ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*检测给定的三个点的位置是否按顺时针顺序排列,顺时针返回一个正值否则为负值*/
	static double CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y);
	
	/*检查点(dNout,dEout)是否在给定的圆弧上*/
    static bool CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout);
};

#endif // !defined(AFX_BASICFUN_H__596081B3_5044_4DA3_9256_DA0124EF0085__INCLUDED_)
