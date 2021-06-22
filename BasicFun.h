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

	/*��������֮��ľ���Dis�����㹹�ɵ�ֱ����x�ļн�dAngleByX */
	static double DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend);
	
	/*�Ѽ�������ĽǶ�ת��Ϊ0-2pi��Χ�ڵķ�λ��*/
	static void FormFwj(double& dFwj);

	/*��������������λ�ǵó�ת���*/
	static double ZxAngle(double dLineAngle2,double dLineAngle1);	

	/*��㵽ֱ�߾���*/
	static double DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);
	
	/*��һ����ֱ������������*/
	static void ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*���������������λ���Ƿ�˳ʱ��˳������,˳ʱ�뷵��һ����ֵ����Ϊ��ֵ*/
	static double CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y);
	
	/*����(dNout,dEout)�Ƿ��ڸ�����Բ����*/
    static bool CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout);
};

#endif // !defined(AFX_BASICFUN_H__596081B3_5044_4DA3_9256_DA0124EF0085__INCLUDED_)
