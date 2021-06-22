// Hhqx.h: interface for the CHhqx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_)
#define AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicFun.h"




class CHhqx : public CBasicFun  
{
public:
	
	double m_StartN;		//起点N坐标
	double m_StartE;		//起点E坐标
	double m_StartFwj;		//起点方位角
	double m_StartL;		//起点距离参照点长度
	double m_StartR;		//起点半径

	double m_EndN;			//终点N坐标
	double m_EndE;			//终点E坐标
	double m_EndFwj;		//终点方位角
	double m_EndL;			//终点距离参照点长度
	double m_EndR;			//终点半径
	
	double m_L0;			//缓和曲线总长(自参照点起至缓和曲线终点)
	int m_LOrR;          //左右偏信息 左偏为-1，右偏为+1
	int m_Mod;			//缓和曲线类型编号0-6：0--三次抛物线 1--半波正弦 2--全波正弦  3--五次抛物线  4--七次抛物线 5--七次四项式 6-- S型 


	double m_BaseN;			//计算参照点N坐标
	double m_BaseE;			//计算参照点E坐标
	double m_BaseFwj;		//计算参照点方位角

	

								
	
/***********************构造函数***************************************************/
	CHhqx();
	CHhqx(double dStartN,double dStartE,double dStartFwj,double dL0,
		   double dStartR,double dEndR,int dLOrR,int iMod);
	
	virtual ~CHhqx();

	/* 传入缓和曲线的相关参数构建出缓和曲线（从起点开始）*/
	void Set(double dStartN,double dStartE,double dStartFwj,double dL0,
		   double dStartR,double dEndR,int iLOrR,int iMod);

	/* 传入缓和曲线的相关参数构建出缓和曲线（从终点开始）*/
	void SetByEnd(double dEndN,double dEndE,double dEndFwj,double dL0,
			double dEndR,double dStartR,int iLOrR,int iMod);



/***********************基本计算函数***************************************************/
	
    /*计算距离参照点的长度dLToBasePt点的切线角*/
	double CalQxj(double dLToBasePt)const;

    /*计算距离参照点的长度dLToBasePt点的横距,调用Chebftjf积分函数*/
	double CalX(double dLToBasePt)const;
	/*计算距离参照点的长度dLToBasePt点的纵距，调用Chebftjf积分函数*/
	double CalY(double dLToBasePt)const;
	/*用于计算函数在[a,x]区间上的积分*/
	    double Chebftjf(double a,double b,double x,int ixory)const;
	    /*用切比雪夫极数计算函数在给定点x处的值*/
	         double Chebev(double a,double b,double c[],int m,double x)const;
	        /*用于计算函数不定积分的切比雪夫逼近极数的系数，并选择积分常数使不定积分在a处的值为0*/		
	             void Chint(double cint1[],double a,double b,double c[],int n)const;
				 /*用于计算切比雪夫极数的系数*/
					 void Chebft(double c[],double a,double b,int n,int ixory)const;
	                //计算距参照点离为dLToBasePt点的切向角的余弦与正弦值,为计算横距和纵距做准备。
	                      double FunXY(double dLToBasePt,int ixory)const;


	/*计算半径为dRl处的点距离参照点的长度dLToBasePt 调用CalQulv、RtflspLToBasePt函数*/
	double CalLToBasePt(double dRl)const;
    	/*该函数为试位法解方程，要调用rtflspLToBasePtFun。可以试算出曲率半径为RLToBasePt时该点与参照点的曲线长度LToBasePt*/
    	double RtflspLToBasePt(double x1,double x2,double xacc,double dRl)const;
	   /*距离参照点的长度dLToBasePt点的曲率*/
    	double CalQulv(double dLToBasePt)const;
    
	/*传入到参照点的曲线长dLToBasePt，获取该点的大地坐标*/
	bool   GetPoint(double &Np,double &Ep,double dLToBasePt)const;
	/*传入到参照点的曲线长dLToBasePt，获取该点的坐标方位角*/
	double GetFwj(double dLToBasePt)const;
	/*试位法迭代计算缓和曲线上切线方位角为dFwj处距离参照点的长度dLToBasePt*/
	double RtflspGetFwj(double x1,double x2,double xacc,double dFwj)const;

/***********************求投影点坐标计算函数***************************************************/

	/*计算缓和曲线外一点（Nout,Eout）在缓和曲线上的投影点坐标和投影点到参照点的曲线长*/
	bool GetPointTy(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNout,double dEout);
     	    /*黄金分割搜索寻找-Getafa函数的最小值点距离缓和曲线长xmin;*/
	         double GoldenGetAfa(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout);
		    /*试位法求解Getafa-pi/2=0时的dLtoBasePt*/
	         double RtflspGetafa(double &dNp,double &dEp,double x1,double x2,double xacc,double dNout,double dEout);
		     /*计算缓和(dNp,dEp)与缓和曲线外一点(dNout,dEout)构成的直线与(dNp,dEp)点的切线所形成的夹角(0-pi之间)*/
	         double Getafa(double &dNp,double &dEp,double dLtoBasePt,double dNout,double dEout);
			 /*计算缓和曲线曲线上距离参照点曲线长为dLToBasePt的点距离点(dNout,dEout)距离*/
			 double DisPtToHhqx(double dLToBasePt,double dNout,double dEout);
			 /*黄金分割搜索CHhqx::GoldenDisPtToHhqx函数的极小值 */
			 double GoldenDisPtToHhqx(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout);
			 /*计算点(dNout,dEout)到缓和曲线的最短距离*/
			 double MinDisPtToHhqx(double &dLToBasePt,double dNout,double dEout);


/***********************求交计算函数***************************************************/
	/*求直线段与缓和曲线的交点*/    
	bool GetJdZxHhqx(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNstar,double dEstar,double dNend,double dEend)const;
	/*求直线段与缓和曲线的一个交点*/
	bool GetJd1ZxHhqx(double &dNp1,double &dEp1,double &dLtoBasePt1,double dNstart,double dEstart,double dNend,double dEend,int iFromwhere)const;

	/*求缓和曲线与圆弧的交点坐标*/
	bool GetJdArcHhqx(double &dNp1,double &dEp1,double &dNp2,double &dEp2,int &iNjd,double dNcircle,double dEcircle,double dRcircle,double dAngleStart,double dAngleEnd)const;
	
	/*求两缓和曲线的交点坐标*/
	bool GetJdHhqxHhqxHuitou(CArray<Jd,Jd> &HHJd,CHhqx Hhqx1,CHhqx Hhqx2);
	bool GetJdHhqxHhqx(double &dN1,double &dE1,double &dLtoBasePt1,double &dN2,double &dE2,double &dLtoBasePt2,int &iNjd,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL)const;
    	/*以第一条缓和曲线起点为原点，第一条缓和曲线的起点切线方向为X轴建立XOY坐标系，求出第二条缓和曲线和第一条缓和曲线在相同的X坐标下的Y坐标差值*/
    	double LtoDeta(const CHhqx Hhqx1,const CHhqx Hhqx2,double dLtoBasePt,int iFromwhere1)const;
    	/*该函数为试位法求解LtoDeta函数值为0时的LtoBasePt*/
	    double RtflspLtoDeta(double x1,double x2,double xacc,const CHhqx Sp1,const CHhqx Sp2,int iFromwhere1)const;
    	/*以第一条缓和曲线起点为原点，第一条缓和曲线的起点切线方向为X轴建立XOY坐标系，求出在该坐标系下两缓和曲线X可取值范围的交集*/
	    bool GoldenQj(double &al,double &cl,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL,int iFromwhere2)const;
    	/*黄金分割搜索求出LtoDeta函数的极小值和极小值处对应的dLtoBasePt*/
	    double GoldenLtoDeta(double& xminormin,double ax, double bx, double cx, double tol, int maxormin, const CHhqx Hhqx1,const CHhqx Hhqx2,int iFromwhere1)const;
		double RtflspCalQxj(double x1,double x2,double xacc,double dQxj)const;


};

#endif // !defined(AFX_HHQX_H__322E7E84_39AA_4F7C_A28B_318935D84970__INCLUDED_)
