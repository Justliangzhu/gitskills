// BasicFun.cpp: implementation of the CBasicFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "BasicFun.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#define PRECISION 1e-12

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicFun::CBasicFun()
{

}

CBasicFun::~CBasicFun()
{

}

/*************************************************************
函数名称: DisAndAngleByX   
函数功能、性能等的描述: 计算两点之间的距离dDis和两点构成的直线与x的夹角dAngleByX
被本函数调用的函数清单: 无
调用本函数的函数清单: CHhqx::set,SetByEnd,GetPoint,GetPoint
CYYLuanLink::Set,L0toCC,
CYYCSLink::Set
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dXstart    起点的x坐标；
dYstart	   起点的y坐标；
dXend	   终点的x坐标;	
dYend	   终点的y坐标;
对输出参数的说明:  dAngleByX 为该条直线与X轴夹角
函数返回值的说明:  返回该直线的长度      
其它说明:  以大地N坐标代替X，大地E坐标代替Y，dAngleByX是坐标方位角。  
***************************************************************/
double CBasicFun::DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend)
{	
	double dx=0.0,dy=0.0,dis=0.0,aa=0.0;
	dx = dXend - dXstart;
	dy = dYend - dYstart;
	dis = sqrt(dx*dx+dy*dy);
	if (fabs(dx)<PRECISION) 
	{
		if (dy>=0.0) 
			aa=pi*0.5;
		else
			aa=pi*1.5;
	}
	else 
		aa=atan(dy/dx);

	if (dy>=0 && dx<-PRECISION) 
		aa=aa+pi;
	else if (dy<0 && dx<-PRECISION) 
		aa=aa+pi;
	else if (dy<0 && dx>PRECISION) 
		aa=aa+2*pi;
	if (aa<-PRECISION) 
		aa=aa+2*pi;
	else if (aa>2*pi) 
		aa=aa-2*pi;
	dAngleByX=aa;
	return(dis);
}
/*************************************************************
函数名称: FormFwj   
函数功能、性能等的描述: 把计算出来的角度转换为0-2pi范围内的方位角
被本函数调用的函数清单: 无
调用本函数的函数清单: CHhqx::set,SetByEnd；
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      dFWj为前面算出来的角度
对输出参数的说明:  dFwj为转化后的方位角
函数返回值的说明:      
其它说明:     
***************************************************************/
void CBasicFun::FormFwj(double& dFwj) 
{
	while(dFwj < 0.0)
	{
		dFwj += 2*pi;
	}
	if(dFwj>2*pi)
	{
		dFwj = fmod(dFwj, 2*pi);
	}
}

/*************************************************************
函数名称: ZxAngle   
函数功能、性能等的描述: 求出两有向直线dLineAngle1到dLineAngle2的转向角
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GetAfa
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      dLineAngle1  第一条有向直线的坐标方位角 
dLineAngle2  第二条有向直线的坐标方位角 
对输出参数的说明:  无
函数返回值的说明:  返回dLineAngle1到dLineAngle2的转向角  
其它说明:  转向角顺时针方向为正     
***************************************************************/
double CBasicFun::ZxAngle(double dLineAngle2,double dLineAngle1)
{
	double dAngle=0.0;
	dAngle = dLineAngle2 - dLineAngle1;
	if(fabs(dAngle)>pi)
	{
		if(dAngle<0.0) //偏角小于-PI
		{
			dAngle = 2.0*pi + dAngle;
		}
		else      //偏角大于PI
		{
			dAngle = dAngle - 2.0*pi;
		}
	}
	return dAngle;
}

/*************************************************************
函数名称: DistanceFromPttoLine   
函数功能、性能等的描述: 求点到直线距离；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GetJd1ZxHhqx，GetJdZxHhqx，LtoDeta;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dXout      直线外点的坐标；
dYout      直线外点的坐标；
dXstart    直线起点的X坐标；
dYstart	   直线起点的Y坐标；
dXend      直线终点的X坐标；
dYend      直线终点的Y坐标；
对输出参数的说明:  无
函数返回值的说明:  返回(dXout,dYout)到直线的距离；
其它说明:		   特别要注意的是该函数的返回值并不是点到直线的距离，
返回值有正负号以反映点在直线的不同侧，绝对值才是点到直线的距离；
如果(dXout,dYout)就在直线上也可以计算，返回值为一个很小的值   
***************************************************************/

double CBasicFun::DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
{
	double a=0.0,b=0.0,c=0.0,d=0.0,absq=0.0,sr=0.0;
	if (fabs(dXend-dXstart)<PRECISION)
		d=dXout-(dXstart+dXend)/2;

	else if(fabs(dYend-dYstart)<PRECISION)
		d=dYout-(dYstart+dYend)/2;

	else
	{
		a=(dYend-dYstart)/(dXend-dXstart);
		b=-1;
		c=dYstart-dXstart*(dYend-dYstart)/(dXend-dXstart);
		absq=sqrt(a*a+b*b);
		sr=a*dXout+b*dYout+c;
		d=sr/absq;
	}	
	return d;
}

/*************************************************************
函数名称: ChuizutoZx   
函数功能、性能等的描述: 从一点向直线作垂线求垂足；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GoldenQj;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dXout      直线外点的X坐标；
dYout      直线外点的Y坐标；
dXstart    直线起点的X坐标；
dYstart	   直线起点的Y坐标；
dXend      直线终点的X坐标；
dYend      直线终点的Y坐标；
对输出参数的说明:  
dXchuizu   垂足点的X坐标；
dYchuizu   垂足点的Y坐标；
函数返回值的说明:  无返回值
其它说明:  无
***************************************************************/

void CBasicFun::ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
{
	double k=0.0;//直线的斜率
	double b=0.0;//将直线化为y=kx+b后的b值
	if (fabs(dYstart-dYend)<PRECISION)
	{
		dXchuizu = dXout;
		dYchuizu = (dYstart+dYend)/2;
	}
	else if (fabs(dXstart-dXend)<PRECISION)
	{
		dYchuizu = dYout;
		dXchuizu = (dXstart+dXend)/2;
	}
	else 
	{	
		k = (dYend-dYstart)/(dXend-dXstart);
		b = dYstart-dXstart*k;
		dXchuizu = (dYout+dXout/k-b)/(k+1/k);
		dYchuizu = k*(dYout+dXout/k-b)/(k+1/k)+b;
	}
}

/*************************************************************
函数名称: CCW2   
函数功能、性能等的描述: 检测给定的三个点的位置是否按顺时针顺序排列；
被本函数调用的函数清单: 无
调用本函数的函数清单:	CHhqx::GoldenQj;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:      
dpt1x      第一个点的X坐标；
dpt1y      第一个点的y坐标；
dpt2x	   第二个点的X坐标；
dpt2y	   第二个点的y坐标；
dpt3x      第三个点的X坐标；
dpt3y      第三个点的y坐标；
对输出参数的说明:  无
函数返回值的说明:  返回值为正时这三点为顺时针，为负时为逆时针
其它说明:  合理的给定这三个点该函数就可以用于判定一个点在一条直线的哪一侧，
GoldenQj调用该函数就是使用了它的这个功能
***************************************************************/

double CBasicFun::CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y) 
{
	double x1,x2,x3,y1,y2,y3,bx,by;

	bx = (dPt1X + dPt2X + dPt3X) /3.0;
	by = (dPt1Y + dPt2Y + dPt3Y) /3.0;
	x1 = dPt1X - bx;
	x2 = dPt2X - bx;
	x3 = dPt3X - bx;
	y1 = dPt1Y - by;
	y2 = dPt2Y - by;
	y3 = dPt3Y - by;
	return (x1*(y2-y3)-x2*(y1-y3)+x3*(y1-y2));
}


/*************************************************************
函数名称: CheckIfOnArc  
函数功能、性能等的描述: 检查点(dNout,dEout)是否在给定的圆弧上  
被本函数调用的函数清单: CBasicFun::DisAndAngleByX
调用本函数的函数清单:	CHhqx::GetJdArcHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNcircle		    圆弧的圆心N坐标;
dEcircle			圆弧的圆心E坐标;
dAngleStart			圆弧起点的角度;
dAngleEnd			圆弧终点的角度;
dNout				待判断点的N坐标;
dEout				待判断点的E坐标;
对输出参数的说明:	无
函数返回值的说明: 该点在圆弧上则返回ture否则返回false;
其它说明:  
注意：该函数判断点是否在圆弧上是以该点与圆弧圆心的距离为半径的条件下进行的。
圆弧被认为是从起点开始按逆时针方向到终点；
圆弧起点和终点的角度是以圆心为坐标原点建立的XoY坐标系下起点、终点与圆心的连线的角度值,逆时针方向为正。
对于这些方向和起终点的定义是与CAD中的定义一致的
***************************************************************/
bool CBasicFun::CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout)
{
	double Angle=0.0;
	DisAndAngleByX(Angle,dEcircle,dNcircle,dEout,dNout);
	if (dAngleStart<dAngleEnd)
	{
		if (Angle<dAngleEnd+1e-11 && Angle>dAngleStart-1e-11)
			return true;
		else
			return false;
	}
	else if (dAngleStart>dAngleEnd)
	{
		if (Angle>dAngleEnd-1e-11 && Angle<dAngleStart+1e-11)
			return false;
		else
			return true;
	}
	else//起点角度等于终点角度,该圆弧是一个圆
		return true;	
}
