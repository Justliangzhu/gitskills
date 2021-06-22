// BCCreateXLEntity.h: interface for the BCCreateXLEntity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_)
#define AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"

class BCCreateXLEntity : public BAS_DRAW_FUN  
{
public:
	//更新
	void UpdateData(int mode=1);
	//设置圆心坐标，半径，圆心与圆弧起终点连线与X轴的夹角
	void SetARC(double cn,double ce,double r,double startangle,double endangle);
	void SetARC_A(double cn, double ce, double r,double startangle, double endangle,double a1,double a2);
		
	BCCreateXLEntity();
	virtual ~BCCreateXLEntity();
protected:
	double DV;
	//三个交点
	double JdN1,JdE1,JdN0,JdE0,JdN2,JdE2;
	//圆弧起终点
	double N1,E1,N2,E2;
	//半径及圆心坐标
	double CenN,CenE,R;
	bool OK1;
	//圆心与起终点连线与X轴的夹角
	double startAngle,endAngle;
	//前后夹直线长
	double JL1,JL2;
	//前后缓和曲线长
	//double Lo1,Lo2;
	double A1,A2;
public:
	JD_CENTER *pGtzx;
	AcDbObjectId eId;
};

#endif // !defined(AFX_BCCREATEXLENTITY_H__388117D9_F66C_4000_9F92_B1D1E0EAE173__INCLUDED_)
