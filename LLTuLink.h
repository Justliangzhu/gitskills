// LLTuLink.h: interface for the LLTuLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_)
#define AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JD_CENTER.h"
//////////////////
//  凸型曲线    //
//////////////////
class LLTuLink : public BAS_DRAW_FUN  
{
public:
	LLTuLink();
	virtual ~LLTuLink();
	LLTuLink(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2);
public:
	void CreateSpiral();		
	void CreateXL();
//	void AppendXL(JD_CENTER *JdCenObj,int jdno);
	void Set(double n1,double e1,double n0,double e0,double n2,double e2,double a1,double a2);
	bool ERRFlag;
	SPIRAL pSPIRAL1,pSPIRAL2;//前后缓和曲线
	double R,A1,A2,T1,T2,Lo1,Lo2;//圆曲线半径，前缓，后缓，前切，后切，前缓长，后缓长
private:
	double afa,afa1,afa2;//中间交点的转角，前缓转角，后缓转角
	int LRFlag;//左右转，左-1,右+1;
	double fwj1,fwj2;//前后边的方位角，1-》0，0-》2
	double N1,E1,N0,E0,N2,E2;//三个交点
	double Nzh,Ezh,Nhz,Ehz;//ZH点坐标，HZ点坐标
};

#endif // !defined(AFX_LLTULINK_H__2BF8467A_1D03_4A33_B11E_821E6E3E8EC2__INCLUDED_)
