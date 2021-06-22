// ThreeJDCalThisAFromThatA_PreT_R.h: interface for the ThreeJDCalThisAFromThatA_PreT_R class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_)
#define AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

//给定三个交点坐标、中间交点对应的曲线半径和前切长，
//在已知一条缓和曲线的情况下计算另一条缓和曲线
class ThreeJDCalThisAFromThatA_PreT_R : public BAS_DRAW_FUN  
{
public:
	//设置转角
	void SetAfa(double aa);
	//设置前切长和曲线半径
	void SetPreT_R(double t1,double r);
	//设置前缓和曲线的A值
	void SetA1(double a);
	//设置后缓和曲线的A值
	void SetA2(double a);
	//查询前缓和曲线的A值
	bool GetA1(double &a);
	//查询后缓和曲线的A值
	bool GetA2(double &a);
public:
	ThreeJDCalThisAFromThatA_PreT_R();
	virtual ~ThreeJDCalThisAFromThatA_PreT_R();
private:
	int count;
	double DieDaiA1(double a11,double a12);
	double DieDaiA2(double a21,double a22);
	double T1,T2,afa,R,A1,A2;
	bool OKAfa,OKA1,OKA2,OKPreT;
};

#endif // !defined(AFX_THREEJDCALTHISAFROMTHATA_PRET_R_H__788D7386_E570_4A84_B3D7_3BDBFDD46E08__INCLUDED_)
