// Hhqx.cpp: implementation of the CHhqx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Hhqx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHhqx::CHhqx()
{

	m_StartN=0.0;			//起点N坐标
	m_StartE=0.0;			//起点E坐标
	m_StartFwj=0.0;			//起点方位角
	m_StartL=0.0;			//起点距离参照点长度
	m_StartR=1e50;			//起点半径

	m_EndN=0.0;				//终点N坐标
	m_EndE=0.0;				//终点E坐标
	m_EndFwj=pi/4;			//终点方位角
	m_EndL=200;				//终点距离参照点长度
	m_EndR=1000;			//终点半径

	m_BaseN=0.0;			//终点N坐标
	m_BaseE=0.0;			//终点E坐标
	m_BaseFwj=0.0;			//终点方位角

	m_L0=200;				//自参照点起缓和曲线总长
	m_LOrR=1;               //左右偏信息 左偏为-1，右偏为+1
	m_Mod=0;				//缓和曲线类型编号0-6：0--三次抛物线 1--半波正弦 2--全波正弦  3--五次抛物线  4--七次抛物线 5--七次四项式 6-- S型 

}

CHhqx::~CHhqx()
{

}

CHhqx::CHhqx(double dStartN,double dStartE,double dStartFwj,double dL0,
			 double dStartR,double dEndR,int iLOrR,int iMod)
{
	Set(dStartN,dStartE,dStartFwj,dL0,dStartR,dEndR,iLOrR,iMod);
}

/*************************************************************
函数名称: Set  
函数功能、性能等的描述: 传入缓和曲线的相关参数构建出缓和曲线（从起点开始）    
被本函数调用的函数清单: CHhqx:: CalLToBasePt，CalX，CalY，CalQxj，GetPoint，DisAndAngleByX
CBasicFun:: DisAndAngleByX，FormFwj
调用本函数的函数清单:   CYYLLink::Set
CYYCSLink::Set
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dStartN    缓和曲线起点N坐标；
dStartE	   缓和曲线起点E坐标；
dStartFwj  缓和曲线起点方位角；
dL0        自参照点的缓和曲线全长；
dStartR	   缓和曲线起点处的曲率半径；
dEndR	   缓和曲线终点处的曲率半径；
iLOrR	   左右转信息；左传-1，右转+1；
iMod       缓和曲线类型 取值为0-6的整数
对输出参数的说明: 无
函数返回值的说明: 无
其它说明: 该函数为从起点构建缓和曲线     
***************************************************************/
void CHhqx::Set(double dStartN,double dStartE,double dStartFwj,double dL0,
				double dStartR,double dEndR,int iLOrR,int iMod)
{

	/*将实参传入类变量*/
	m_StartN   = dStartN;
	m_StartE   = dStartE;
	m_StartFwj = dStartFwj;
	m_L0=dL0;
	m_Mod=iMod;
	m_StartR = dStartR;
	m_EndR   = dEndR;
	m_LOrR = iLOrR;

	/*计算起点和终点处距离参照点的曲线长*/
	m_StartL = CalLToBasePt(m_StartR);
	m_EndL   = CalLToBasePt(m_EndR);

	/*计算起点的局部坐标和起点与参照点连线的长度和该线的坐标方位角*/
	double x=0.0,y=0.0,sita=0.0,dist0=0.0;
	x = CalX(m_StartL);
	y = CalY(m_StartL);
	dist0 = DisAndAngleByX(sita,0,0,x,y);//此函数在dx近于0 时，返回0.5*PI,一定要注意

	/*计算参照点方位角及坐标*/
	m_BaseFwj = m_StartFwj-m_LOrR*CalQxj(m_StartL);
	m_BaseN   = m_StartN-dist0*cos(m_BaseFwj+m_LOrR*sita);
	m_BaseE   = m_StartE-dist0*sin(m_BaseFwj+m_LOrR*sita);

	/*计算终点坐标及方位角*/
	GetPoint(m_EndN,m_EndE,m_EndL);//计算终点坐标
	m_EndFwj=m_BaseFwj+m_LOrR*CalQxj(m_EndL);//计算终点切线方位角

	/*格式化坐标方位角(将上面算出来的角度化为0-2pi)*/
	FormFwj(m_BaseFwj); 
	FormFwj(m_StartFwj);
	FormFwj(m_EndFwj);
}



/*************************************************************
函数名称: SetByEnd  
函数功能、性能等的描述: 传入缓和曲线的相关参数构建出缓和曲线（从终点开始）    
被本函数调用的函数清单: CHhqx:: CalLToBasePt，CalX，CalY，CalQxj，GetPoint，DisAndAngleByX
CBasicFun:: DisAndAngleByX，FormFwj
调用本函数的函数清单:   CYYLLInk::Set 
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dEndN    缓和曲线起点N坐标；
dEndE	 缓和曲线起点E坐标；
dEndFwj  缓和曲线起点方位角；
dL0      从参照点到圆的缓和曲线全长；
dEndR	 缓和曲线终点处的曲率半径；
dStartR	 缓和曲线起点处的曲率半径；
iLOrR	 左右转信息；左传-1，右转+1；
iMod     缓和曲线类型 取值为0-6的整数
对输出参数的说明: 无
函数返回值的说明: 无
其它说明:   该函数为从终点构建缓和曲线    
***************************************************************/
void CHhqx::SetByEnd(double dEndN,double dEndE,double dEndFwj,double dL0,
					 double dEndR,double dStartR,int iLOrR,int iMod)
{
	/*将实参传入类变量*/
	m_EndN  = dEndN;
	m_EndE  = dEndE;
	m_EndFwj = dEndFwj;
	m_L0= dL0;
	m_EndR = dEndR;
	m_StartR  = dStartR;
	m_Mod = iMod;

	/*计算起点和终点处距离参照点的曲线长*/
	m_StartL = CalLToBasePt(m_StartR);
	m_EndL   = CalLToBasePt(m_EndR);	
	m_LOrR = iLOrR;

	/*计算终点的局部坐标和终点与参照点连线的长度和该线坐标方位角*/
	double x,y,sita,dist0;
	x=y=sita=dist0=0.0;
	x = CalX(m_EndL);
	y = CalY(m_EndL);
	dist0 = DisAndAngleByX(sita,0,0,x,y);//此函数在dx近于0 时，返回0.5*PI,一定要注意

	/*计算参照点方位角及坐标*/
	m_BaseFwj = m_EndFwj-m_LOrR*CalQxj(m_EndL);
	m_BaseN   = m_EndN-dist0*cos(m_BaseFwj+m_LOrR*sita);
	m_BaseE   = m_EndE-dist0*sin(m_BaseFwj+m_LOrR*sita);

	/*计算起点坐标及方位角*/
	GetPoint(m_StartN,m_StartE,m_StartL);
	m_StartFwj=m_BaseFwj+m_LOrR*CalQxj(m_StartL);

	/*格式化坐标方位角(将上面算出来的角度化为0-2pi)*/
	FormFwj(m_BaseFwj); 
	FormFwj(m_StartFwj);
	FormFwj(m_EndFwj);
}

/*************************************************************
函数名称: CalQxj   
函数功能、性能等的描述: 计算距离参照点的长度为dLToBasePt处的切线角
被本函数调用的函数清单: 无
调用本函数的函数清单:  CHhqx::set,SetByEnd,
CYYLLink::Set,L0toCC,
CYYCSLink::Set，L0toCC
CZYLink::Set
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:  
dLToBasePt 任意点离参照点的曲线长度    
其它说明:      
***************************************************************/
double CHhqx::CalQxj(double dLToBasePt)const
{
	double sita = 0.0;
	if (fabs(m_EndR*m_L0)<1e-5)
	{
		return sita;
	} 
	else
	{
		if (m_Mod==0)
		{
			sita=dLToBasePt*dLToBasePt/m_EndR/m_L0/2.0;
		} 
		else if(m_Mod==1)
		{
			sita=m_L0*(dLToBasePt/m_L0-sin(pi*dLToBasePt/m_L0)/pi)/m_EndR/2;
		}
		else if (m_Mod==2)
		{
			sita=m_L0/m_EndR/2.0*(pow(dLToBasePt/m_L0,2)+(cos(2.0*pi*dLToBasePt/m_L0)-1)/pi/pi/2);
		}
		else if (m_Mod==3)
		{
			sita=m_L0/m_EndR/2.0*(2.0*pow(dLToBasePt/m_L0,3)-pow(dLToBasePt/m_L0,4));
		}
		else if (m_Mod==4)
		{
			sita=m_L0/m_EndR/2.0*(2.0*pow(dLToBasePt/m_L0,6)-6.0*pow(dLToBasePt/m_L0,5)+5.0*pow(dLToBasePt/m_L0,4));
		}
		else if (m_Mod==5)
		{
			sita=m_L0/m_EndR/2.0*(10.0/3*pow(dLToBasePt/m_L0,3)-5.0*pow(dLToBasePt/m_L0,4)+4.0*pow(dLToBasePt/m_L0,5)-4.0/3.0*pow(dLToBasePt/m_L0,6));
		}
		else if (m_Mod==6)
		{
			if (dLToBasePt<m_L0/2+0.001)
			{
				sita=m_L0/m_EndR/2*4.0/3.0*pow(dLToBasePt/m_L0,3);
			}
			else 
			{
				sita=m_L0/m_EndR/2-2.0/m_EndR*(0.5*(m_L0-dLToBasePt)-1.0/3*m_L0*pow(1-dLToBasePt/m_L0,3))
					-(m_L0/m_EndR/2-2.0/m_EndR*(0.5*(m_L0/2)-1.0/3*m_L0*pow(0.5,3)))+m_L0/m_EndR/2.0*4.0/3*pow(0.5,3);
			}
		}

		else			//如果需要增加其他缓和曲线类型只需在else后添加即可
			sita=0.0;     
	}		
	return sita;
}

/*************************************************************
函数名称: GetFwj   
函数功能、性能等的描述: 计算距离参照点的长度为dLToBasePt处的大地方位角
被本函数调用的函数清单: CHhqx::CalQxj
调用本函数的函数清单:	CHhqx::RtflspGetFwj
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:  
dLToBasePt 任意点离参照点的曲线长度    
其它说明:      
***************************************************************/
double CHhqx::GetFwj(double dLToBasePt)const
{
	double Fwj;
	Fwj=0.0;
	Fwj=m_BaseFwj+m_LOrR*CalQxj(dLToBasePt);
	FormFwj(Fwj);
	return Fwj;
}

/*************************************************************
函数名称: RtflspGetFwj   
函数功能、性能等的描述: 试位法迭代计算缓和曲线上切线方位角为dFwj处距离参照点的长度dLToBasePt
被本函数调用的函数清单: CHhqx::GetFwj
调用本函数的函数清单:   CHhqx::GetJdZxHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
x1		试算区间的下限值；
x2		试算区间的下限值；
xacc    试算的精度；
dFwj    方位角；
对输出参数的说明: 
函数返回值的说明:  返回缓和曲线上切线方位角为dFwj处距离参照点的长度dLToBasePt
其它说明:      
***************************************************************/
double CHhqx::RtflspGetFwj(double x1,double x2,double xacc,double dFwj)const
{
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	j=0;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=GetFwj(x1)-dFwj;//迭代计算的方程式
	fh=GetFwj(x2)-dFwj;//迭代计算的方程式
	if (fl*fh>0)
	{
		AfxMessageBox(L"root must be bracketed for false position");
		return m_StartL;
	}
	if (fl<0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=maxit;j++)
	{
		temp=xl+dx*fl/(fl-fh);
		f=GetFwj(temp)-dFwj;//迭代计算的方程式
		if (f<0)
		{
			del=xl-temp;
			xl=temp;
			fl=f;
		}
		else
		{
			del=xh-temp;
			xh=temp;
			fh=f;
		}
		dx=xh-xl;
		if ((fabs(del)<xacc)||(f==0)) break;

	}
	return temp;
	AfxMessageBox(L"rtflsp exceed maximum iterations");

}

/*************************************************************
函数名称: RtflspGetFwj   
函数功能、性能等的描述: 试位法迭代计算缓和曲线上切线方位角为dFwj处距离参照点的长度dLToBasePt
被本函数调用的函数清单: CHhqx::GetFwj
调用本函数的函数清单:   CHhqx::GetJdZxHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
x1		试算区间的下限值；
x2		试算区间的下限值；
xacc    试算的精度；
dFwj    方位角；
对输出参数的说明: 
函数返回值的说明:  返回缓和曲线上切线方位角为dFwj处距离参照点的长度dLToBasePt
其它说明:      
***************************************************************/
double CHhqx::RtflspCalQxj(double x1,double x2,double xacc,double dQxj)const
{
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	j=0;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=CalQxj(x1)-dQxj;//迭代计算的方程式
	fh=CalQxj(x2)-dQxj;//迭代计算的方程式
	if (fl*fh>0)
	{
		AfxMessageBox(L"root must be bracketed for false position");
		return m_StartL;
	}
	if (fl<0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=maxit;j++)
	{
		temp=xl+dx*fl/(fl-fh);
		f=CalQxj(temp)-dQxj;//迭代计算的方程式
		if (f<0)
		{
			del=xl-temp;
			xl=temp;
			fl=f;
		}
		else
		{
			del=xh-temp;
			xh=temp;
			fh=f;
		}
		dx=xh-xl;
		if ((fabs(del)<xacc)||(f==0)) break;

	}
	return temp;
	AfxMessageBox(L"rtflsp exceed maximum iterations");

}

/*************************************************************
函数名称: FunXY   
函数功能、性能等的描述: 计算距参照点离为dLToBasePt点的切向角的余弦与正弦值,为计算横距和纵距做准备。
被本函数调用的函数清单: 无
调用本函数的函数清单: Chebft 
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明: dLToBasePt 任意点距离参照点的曲线长；
ixory 为1时表示算x坐标，为其它数时表示算y坐标;
对输出参数的说明: 
函数返回值的说明: 返回局部坐标系下的x,y坐标      
其它说明: 局部坐标参照点为坐标原点     
***************************************************************/
double CHhqx::FunXY(double dLToBasePt,int ixory)const
{
	if (m_Mod==0)
	{
		if (ixory==1)
		{
			return cos(dLToBasePt*dLToBasePt/2/m_L0/m_EndR);
		} 
		else 	
		{
			return sin(dLToBasePt*dLToBasePt/2/m_L0/m_EndR);
		}
	} 
	else if(m_Mod==1)
	{
		if (ixory==1)
		{
			return cos(m_L0/m_EndR/2.0*(dLToBasePt/m_L0-sin(pi*dLToBasePt/m_L0)/pi));
		} 
		else	
		{
			return sin(m_L0/m_EndR/2.0*(dLToBasePt/m_L0-sin(pi*dLToBasePt/m_L0)/pi));
		}
	}

	else if(m_Mod==2)
	{
		if (ixory==1)
		{
			return cos(m_L0/m_EndR/2.0*(pow(dLToBasePt/m_L0,2)+(cos(2.0*pi*dLToBasePt/m_L0)-1)/pi/pi/2));
		} 
		else	
		{
			return sin(m_L0/m_EndR/2.0*(pow(dLToBasePt/m_L0,2)+(cos(2.0*pi*dLToBasePt/m_L0)-1)/pi/pi/2));
		}
	}

	else if(m_Mod==3)
	{
		if (ixory==1)
		{
			return cos(m_L0/m_EndR/2*(2.0*pow(dLToBasePt/m_L0,3)-pow(dLToBasePt/m_L0,4)));
		} 
		else	
		{
			return sin(m_L0/m_EndR/2*(2.0*pow(dLToBasePt/m_L0,3)-pow(dLToBasePt/m_L0,4)));
		}
	}
	else if(m_Mod==4)
	{
		if (ixory==1)
		{
			return cos(m_L0/m_EndR/2*(2.0*pow(dLToBasePt/m_L0,6)-6.0*pow(dLToBasePt/m_L0,5)+5.0*pow(dLToBasePt/m_L0,4)));
		} 
		else	
		{
			return sin(m_L0/m_EndR/2*(2.0*pow(dLToBasePt/m_L0,6)-6.0*pow(dLToBasePt/m_L0,5)+5.0*pow(dLToBasePt/m_L0,4)));
		}
	}

	else if(m_Mod==5)
	{
		if (ixory==1)
		{
			return cos(m_L0/m_EndR/2.0*(10.0/3.0*pow(dLToBasePt/m_L0,3)-5.0*pow(dLToBasePt/m_L0,4)+4.0*pow(dLToBasePt/m_L0,5)-4.0/3.0*pow(dLToBasePt/m_L0,6)));
		} 
		else	
		{
			return sin(m_L0/m_EndR/2.0*(10.0/3.0*pow(dLToBasePt/m_L0,3)-5.0*pow(dLToBasePt/m_L0,4)+4.0*pow(dLToBasePt/m_L0,5)-4.0/3.0*pow(dLToBasePt/m_L0,6)));
		}
	}
	else if (m_Mod==6)
	{
		if (ixory==1)
		{
			if (dLToBasePt<m_L0/2+1e-5)
			{
				return cos(m_L0/m_EndR/2.0*4.0/3.0*pow(dLToBasePt/m_L0,3));
			} 
			else
			{
				return cos(m_L0/m_EndR/2.0-2.0/m_EndR*(0.5*(m_L0-dLToBasePt)-1.0/3.0*m_L0*pow(1-dLToBasePt/m_L0,3)));
			}
		} 
		else 
		{
			if (dLToBasePt<m_L0/2+1e-5)
			{
				return sin(m_L0/m_EndR/2.0*4.0/3.0*pow(dLToBasePt/m_L0,3));
			} 
			else
			{
				return sin(m_L0/m_EndR/2.0-2.0/m_EndR*(0.5*(m_L0-dLToBasePt)-1.0/3.0*m_L0*pow(1-dLToBasePt/m_L0,3)));
			}

		}
	}
	else		 //如果需要增加其他缓和曲线类型只需在else中添加即可
		return 0.0;
}

/*************************************************************
函数名称: Chebft   
函数功能、性能等的描述: 用于计算切比雪夫极数的系数												
被本函数调用的函数清单: FunXY
调用本函数的函数清单:  Chebftjf
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   a   积分区间的下限；
b   积分区间的上限不能超过的限定值；
xacc   积分值的精度；
n    展开的项数
xory  为1表示对x积分，为其它值表示对y积分;
对输出参数的说明: c[]  n个元素的数组，存放切比雪夫级数的系数，该数组以后要传入chint函数；
函数返回值的说明:        
其它说明:		[a,b]为该函数的区间。但是该函数并不是计算a到b的积分，b只是积分上限的最大值      
***************************************************************/
void CHhqx::Chebft(double c[],double a,double b,int n,int ixory)const
{
	int k,nmax=1000;
	double f[51];
	for (int i=0;i<51;i++)
	{
		f[i]=0.0;
	}
	double bma,bpa,y,sum,fac;
	bma=bpa=y=sum=fac=0.0;
	bma=0.5*(b-a);
	bpa=0.5*(b+a);
	for (k=1;k<=n;k++)
	{
		y=cos(pi*(k-0.5)/n);
		f[k]=FunXY(y*bma+bpa,ixory);//待积分函数

	}
	fac=2.0/n;
	for (int j=1;j<=n;j++)
	{
		sum=0.0;
		for (k=1;k<=n;k++)
		{
			sum=sum+f[k]*cos((pi*(j-1))*((k-0.5)/n));
		}
		c[j]=fac*sum;
	}
}

/*************************************************************
函数名称: Chint   
函数功能、性能等的描述: 用于计算函数不定积分的切比雪夫逼近极数的系数，
并选择积分常数使不定积分在a处的值为0												
被本函数调用的函数清单: 无
调用本函数的函数清单:  Chebftjf
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
a			积分区间的下限；
b			积分区间的上限不能超过的限定值；
c[]			n个元素的数组，存放切比雪夫级数的系数，该数组由chebft函数计算得出；
n			展开的项数；
对输出参数的说明: cint1[]  n个元素的数组存放函数不定积分的切比雪夫逼近级数的系数；
函数返回值的说明: 无      
其它说明:  [a,b]为该函数的区间。但是该函数并不是计算a到b的积分，b只是积分上限的最大值      
***************************************************************/
void CHhqx::Chint(double cint1[],double a,double b,double c[],int n)const
{
	double con,sum,fac;
	con=sum=fac=0.0;
	con=0.25*(b-a);
	sum=0.0;
	fac=1.0;
	for (int j=2;j<=n-1;j++)
	{
		cint1[j]=con*(c[j-1]-c[j+1])/(j-1);
		sum=sum+fac*cint1[j];
		fac=-fac;
	}
	cint1[n]=con*c[n-1]/(n-1);
	sum=sum+fac*cint1[n];
	cint1[1]=2.0*sum;
}

/*************************************************************
函数名称: Chebev   
函数功能、性能等的描述: 用切比雪夫极数计算函数在给定点x处的值												
被本函数调用的函数清单: 无
调用本函数的函数清单:  Chebftjf
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   a   积分区间的下限；
b   积分区间的上限不能超过的限定值；
c[]  m个元素的数组，存放切比雪夫级数的系数，该数组值由chebft中的c[]传入；
m    所用切比雪夫级数的项数；
x    所求积分的积分上限；
对输出参数的说明: 
函数返回值的说明: 返回函数在[a,x]上的积分      
其它说明:[a,b]为该函数的区间。但是该函数并不是计算a到b的积分，b只是积分上限的最大值      
***************************************************************/
double CHhqx::Chebev(double a,double b,double c[],int m,double x)const
{
	if (fabs(a-b)<=1e-5)
	{
		return 0.0;
	}

	if ((x-a)*(x-b)>0.0)
	{
		AfxMessageBox(L"x not in range");
	}
	double d=0.0;
	double sv,y,y2,dd=0.0;
	y=(2.0*x-a-b)/(b-a);
	y2=2.0*y;
	for (int j=m;j>=2;j--)
	{
		sv=d;
		d=y2*d-dd+c[j];
		dd=sv;
	}
	return y*d-dd+0.5*c[1];
}

/*************************************************************
函数名称: Chebftjf   
函数功能、性能等的描述: 用于计算函数在[a,x]区间上的积分												
被本函数调用的函数清单: CHhqx::Chebft，Chint,Chebev;
调用本函数的函数清单:   CHhqx::CalX,CalY;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
a		积分区间的下限；
b		积分区间的上限不能超过的限定值；
x		所求积分的积分上限；
ixory	为1表示对x积分，为2表示对y积分;
对输出参数的说明:  
函数返回值的说明:  函数在[a,x]区间上的积分值      
其它说明:[a,b]为该函数的区间。但是该函数并不是计算a到b的积分，而是[a,x]积分，b只是积分上限x的最大值      
***************************************************************/
double CHhqx::Chebftjf(double a,double b,double x,int ixory)const
{	
	double c[51],cint1[51];
	for (int i=0;i<51;i++)
	{
		c[i]=0.0;
		cint1[i]=0.0;
	}

	Chebft(c,a,b,50,ixory);

	Chint(cint1,a,b,c,45);

	return Chebev(a,b,cint1,45,x);
}

/*************************************************************
函数名称: CalQulv   
函数功能、性能等的描述: 计算距参照点离为dLToBasePt点的曲率
被本函数调用的函数清单: 无
调用本函数的函数清单:   RtflspLToBasePt
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明: 
dLToBasePt    任意点距离参照点的曲线长
对输出参数的说明: 
函数返回值的说明: 返回距参照点离为dLToBasePt点的曲率
其它说明:      
***************************************************************/
double CHhqx::CalQulv(double dLToBasePt)const
{
	if (m_Mod==0)
	{
		return 1/m_EndR*(dLToBasePt/m_L0);
	}
	else if (m_Mod==1)
	{
		return 0.5/m_EndR*(1.0-cos(pi*dLToBasePt/m_L0));
	}
	else if (m_Mod==2)
	{
		return 1/m_EndR*(dLToBasePt/m_L0-sin(2*pi*dLToBasePt/m_L0)/pi/2);
	} 
	else if (m_Mod==3)
	{
		return 1/m_EndR*(3*pow(dLToBasePt/m_L0,2)-2*pow(dLToBasePt/m_L0,3));
	} 
	else if (m_Mod==4)
	{
		return 1/m_EndR*(6*pow(dLToBasePt/m_L0,5)-15*pow(dLToBasePt/m_L0,4)+10*pow(dLToBasePt/m_L0,3));
	}
	else if (m_Mod==5)
	{
		return 1/m_EndR*(5*pow(dLToBasePt/m_L0,2)-10*pow(dLToBasePt/m_L0,3)+10*pow(dLToBasePt/m_L0,4)-4*pow(dLToBasePt/m_L0,5));
	}

	else			//如果需要增加其他缓和曲线类型只需在else中添加即可
		return 0.0;
}


/*************************************************************
函数名称: RtflspLToBasePt   
函数功能、性能等的描述: 试位法迭代计算缓和曲线上曲率半径为dR处距离参照点的长度dLToBasePt						
被本函数调用的函数清单: CHhqx::CalQulv
调用本函数的函数清单:   CHhqx::CalLToBasePt 
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
x1		试算区间的下限值；
x2		试算区间的下限值；
xacc    试算的精度；
dRl      曲率半径；
对输出参数的说明: 
函数返回值的说明:  返回缓和曲线上曲率半径为dR处距离参照点的长度dLToBasePt      
其它说明:      
***************************************************************/
double CHhqx::RtflspLToBasePt(double x1,double x2,double xacc,double dRl)const
{
	if(fabs(dRl)<1e-5||dRl>1e13)
		return 0.0;
	if(fabs(dRl-m_EndR)<1e-5)
		return m_L0;
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	maxit=100;
	fl=CalQulv(x1)-1/dRl;//迭代计算的方程式
	fh=CalQulv(x2)-1/dRl;//迭代计算的方程式
	if (fl*fh>0)
	{
		AfxMessageBox(L"root must be bracketed for false position");
		return 0.0;
	}
	if (fl<0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=maxit;j++)
	{
		temp=xl+dx*fl/(fl-fh);
		f=CalQulv(temp)-1/dRl;//迭代计算的方程式
		if (f<0)
		{
			del=xl-temp;
			xl=temp;
			fl=f;
		}
		else
		{
			del=xh-temp;
			xh=temp;
			fh=f;
		}
		dx=xh-xl;
		if ((fabs(del)<xacc)||(f==0)) break;

	}
	return temp;
	AfxMessageBox(L"rtflsp exceed maximum iterations");

}

/*************************************************************
函数名称: CalLToBasePt   
函数功能、性能等的描述: 计算曲率半径为dRl处的点距离参照点的曲线长度dLToBasePt
被本函数调用的函数清单: CHhqx::RtflspLToBasePt
调用本函数的函数清单:   CHhqx::set,SetByEnd
CYYLLink::Set,L0toCC 
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明: 
dRl  曲率半径;
对输出参数的说明: 
函数返回值的说明: 曲率半径为dRl处的点距离参照点的曲线长度dLToBasePt       
其它说明: 
***************************************************************/
double CHhqx::CalLToBasePt(double dRl)const
{	
	double l = 0.0;	
	if (fabs(m_EndR*m_L0)<1e-5)
	{
		return 0.0;
	} 
	else
	{
		if (m_Mod==0)
		{
			l=m_EndR*m_L0/dRl;
		} 
		else if(m_Mod==1)
		{
			l=(m_L0/pi)*acos(1-2*m_EndR/dRl); 
		}
		else if (m_Mod==2)
		{
			l=RtflspLToBasePt(1e-5,m_L0+1e-5,1e-5,dRl);
		}
		else if (m_Mod==3)
		{
			l=RtflspLToBasePt(1e-5,m_L0+1e-5,1e-5,dRl);
		}
		else if (m_Mod==4)
		{
			l=RtflspLToBasePt(1e-5,m_L0+1e-5,1e-5,dRl);
		}
		else if (m_Mod==5)
		{
			l=RtflspLToBasePt(1e-5,m_L0+1e-5,1e-5,dRl);
		}
		else if (m_Mod==6)
		{
			if (dRl>2*m_EndR+1e-5)
			{
				l=sqrt(m_EndR*m_L0*m_L0/dRl/2);
			} 
			else 
			{
				l=m_L0*(1-sqrt((dRl-m_EndR)/dRl/2));
			}
		}
		else        //如果需要增加其他缓和曲线类型只需在else中添加即可
			l=0.0;
		return l;
	}

}

/*************************************************************
函数名称: CalX   
函数功能、性能等的描述: 计算距参照点的长度dLToBasePt点的横距
被本函数调用的函数清单: CHhqx::Chebftjf
调用本函数的函数清单:   CHhqx::set,SetByEnd, GetPoint
CYYLLink::Set,L0toCC
CYYCSLink::Set,L0toCC
CZYLink::Set,L0ToDisOfCenAndLine
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明: 
dLToBasePt 距参照点的长度
对输出参数的说明:  
函数返回值的说明:  距参照点的长度dLToBasePt点的横距    
其它说明:    
***************************************************************/
double CHhqx::CalX(double dLToBasePt)const
{	
	double j_x=0.0;
	if(fabs(m_EndR*m_L0)<=1e-5)
	{	
		return 0.0;
	}
	if (m_Mod==6 && dLToBasePt>m_L0/2)
	{
		j_x=Chebftjf(0.0,m_L0/2,m_L0/2,1) + Chebftjf(m_L0/2,dLToBasePt,dLToBasePt,1);
	} 
	else
		j_x=Chebftjf(0.0,dLToBasePt,dLToBasePt,1);

	return j_x;

}

/*************************************************************
函数名称: CalY   
函数功能、性能等的描述: 计算距参照点的长度dLToBasePt点的纵距
被本函数调用的函数清单: CHhqx::Chebftjf
调用本函数的函数清单:   CHhqx::set,SetByEnd, GetPoint
CYYLLink::Set,L0toCC
CYYCSLink::Set,L0toCC
CZYLink::Set,L0ToDisOfCenAndLine
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:
dLToBasePt 距参照点的长度
对输出参数的说明:  
函数返回值的说明:  距参照点的长度dLToBasePt点的纵距    
其它说明: 局部坐标参照点为坐标原点     
***************************************************************/
double CHhqx::CalY(double dLToBasePt)const
{	
	double j_y=0.0;
	if(fabs(m_EndR*m_L0)<=1e-5)
	{	
		return 0.0;
	}
	if (m_Mod==6 && dLToBasePt>m_L0/2)
	{
		j_y=Chebftjf(0.0,m_L0/2,m_L0/2,2) + Chebftjf(m_L0/2,dLToBasePt,dLToBasePt,2);
	} 
	else
		j_y=Chebftjf(0.0,dLToBasePt,dLToBasePt,2);	

	return j_y;
}

/*************************************************************
函数名称: GetPoint  
函数功能、性能等的描述: 传入距参照点的长度dLToBasePt，返回该点的大地坐标(N,E)。    
被本函数调用的函数清单: CHhqx::CalX,CalY,
CBasicFun::DisAndAngleByX
调用本函数的函数清单:	CHhqx::Set,SetByEnd,GetJd1ZxHhqx,GetJdZxHhqx,GetJdArcHhqx,GetJdHhqxHhqx
CYYCSLInk::Set
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dLToBasePt     距参照点的长度dLToBasePt；
对输出参数的说明: 
Np  距参照点的长度dLToBasePt点大地N坐标，
Ep  大地E坐标；
函数返回值的说明:dLToBasePt小于0表明参数给错返回false,否则返回ture；
其它说明:      
***************************************************************/
bool CHhqx::GetPoint(double &Np,double &Ep,double dLToBasePt)const
{	
	double sita = 0,dist0 = 0;
	double x=0.0,y=0.0;
	if(fabs(dLToBasePt)<1e-5)
	{
		Np=m_BaseN;
		Ep=m_BaseE;
	}
	else
	{
		x=CalX(dLToBasePt);
		y=CalY(dLToBasePt);
		dist0=DisAndAngleByX(sita,0,0,x,y);
		Np=m_BaseN+dist0*cos(m_BaseFwj+m_LOrR*sita);
		Ep=m_BaseE+dist0*sin(m_BaseFwj+m_LOrR*sita);
	}
	return true;
}

/*************************************************************
函数名称: Getafa  
函数功能、性能等的描述: 计算缓和曲线上距离参照点曲线长为dLtoBasePt的点(dNp,dEp)与缓和曲线外一点(dNout,dEout)构成的直线与
(dNp,dEp)点的切线所形成的夹角(0-pi之间)
被本函数调用的函数清单: CBasicFun::ZXAngle
CHhqx::GetPoint,CalQxj;
CBasicFun::DisAndAngleByX,ZxAngle
调用本函数的函数清单:	CHhqx::GetPointTy,GoldenGetAfa,RtflspLToBasePtGetAfa
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNout	   曲线外点N坐标;
dEout	   曲线外点E坐标;
dLtoBasePt 缓和曲线上的点距离参照点的曲线长；
对输出参数的说明:	
dNp	   缓和曲线上点的N坐标；
dEp	   缓和曲线上点的E坐标；
函数返回值的说明: 返回两直线的夹角（0-2pi之间）
其它说明:  
***************************************************************/
double CHhqx::Getafa(double &dNp,double &dEp,double dLtoBasePt,double dNout,double dEout)
{
	double sita1 = 0.0;
	double sita2 = 0.0;
	double afa = 0.0;
	GetPoint(dNp,dEp,dLtoBasePt);	
	DisAndAngleByX(sita1,dNp,dEp,dNout,dEout);
	sita2=m_BaseFwj+m_LOrR*CalQxj(dLtoBasePt);
	afa=fabs(ZxAngle(sita2,sita1));
	return afa;
}

/*************************************************************
函数名称: RtflspGetafa   
函数功能、性能等的描述: 该函数为试位法求解Getafa-pi/2=0时的dLtoBasePt				
被本函数调用的函数清单: Getafa
调用本函数的函数清单:   PointTy
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
x1      试算区间的下限值；
x2      试算区间的下限值；
xacc    试算的精度；
dNout   曲线外点的N坐标;
dEout   曲线外点的E坐标;
对输出参数的说明:
dNp	   Getafa-pi/2=0时的dLtoBasePt处点的N坐标；
dEp	   Getafa-pi/2=0时的dLtoBasePt处点的E坐标；	 
函数返回值的说明:   Getafa-pi/2=0时的dLtoBasePt值；     
其它说明:      
***************************************************************/
double CHhqx::RtflspGetafa(double &dNp,double &dEp,double x1,double x2,double xacc,double dNout,double dEout)
{
	int maxit,j;
	maxit=j=0;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=Getafa(dNp,dEp,x1,dNout,dEout)-pi/2;//迭代计算的方程式
	fh=Getafa(dNp,dEp,x2,dNout,dEout)-pi/2;//迭代计算的方程式
	if (fl*fh>0)
	{
		AfxMessageBox(L"root must be bracketed for false position");
	}
	if (fl<0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=maxit;j++)
	{
		temp=xl+dx*fl/(fl-fh);
		f=Getafa(dNp,dEp,temp,dNout,dEout)-pi/2;//迭代计算的方程式
		if (f<0)
		{
			del=xl-temp;
			xl=temp;
			fl=f;
		}
		else
		{
			del=xh-temp;
			xh=temp;
			fh=f;
		}
		dx=xh-xl;
		if ((fabs(del)<xacc)||(f==0)) 
			break;

	}
	return temp;
	AfxMessageBox(L"rtflsp exceed maximum iterations");	
}


/*************************************************************
函数名称: GoldenGetAfa  
函数功能、性能等的描述: 黄金分割搜索寻找-Getafa函数的最小值点(即Getafa的最大值)距离参照点的长度xmin;
被本函数调用的函数清单: CHhqx::Getafa:	
调用本函数的函数清单:	CHhqx::PointTy；  
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
ax         搜索区间的下限值；
bx		   搜索区间内的一个值，越接近xmin越好，如果开始无法估计xmin则可给区间的中间值(ax+bx)/2;		   
cx		   搜索区间的上限值；
dNout	   曲线外点N坐标;
dEout	   曲线外点E坐标;
tol		   搜索精度；
对输出参数的说明:	xmin   -Getafa在[ax,bx]区间上极小值对应的x坐标；   
函数返回值的说明:	返回-Getafa的极大值；
其它说明:  该函数在GetPrjPt中当有两个投影点时将起到判断迭代区间的作用。
***************************************************************/
double CHhqx::GoldenGetAfa(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout)
{
	double Np,Ep;
	Np=Ep=0.0;
	double x0,x1,x2,x3,f0,f1,f2,f3,r= 0.61803399;
	x0=x1=x2=x3=f0=f1=f2=f3=0.0;
	double c = 0.38196601;
	x0 = ax;
	x3 = cx;
	if (fabs(cx - bx) > fabs(bx - ax))
	{
		x1 = bx;
		x2 = bx + c * (cx - bx);
	}
	else
	{
		x2 = bx;
		x1 = bx - c * (bx - ax);
	}
	f1 = -Getafa(Np,Ep,x1,dNout,dEout);//求极小值的目标函数
	f2 = -Getafa(Np,Ep,x2,dNout,dEout);//求极小值的目标函数
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = -Getafa(Np,Ep,x2,dNout,dEout);//求极小值的目标函数
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = -Getafa(Np,Ep,x1,dNout,dEout);//求极小值的目标函数
		}
	}
	if (f1 < f2)
	{
		xmin = x1;
		return f1;
	}
	else
	{
		xmin = x2;
		return f2;
	}
}


/*************************************************************
函数名称: DisPtToHhqx
函数功能、性能等的描述: 计算缓和曲线曲线上距离参照点曲线长为dLtoBasePt的点距离点(dNout,dEout)距离  
被本函数调用的函数清单: CHhqx::GetPoint
调用本函数的函数清单:	CHhqx::GoldenDisPtToHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:
dLtoBasePt			缓和曲线上点距离计算参照点的曲线长
dNout				曲线外点的N坐标;
dEout				曲线外点的E坐标;
对输出参数的说明:	无
函数返回值的说明:   缓和曲线曲线上距离参照点曲线长为dLtoBasePt的点距离点(dNout,dEout)距离;
其它说明:  无
***************************************************************/
double CHhqx::DisPtToHhqx(double dLToBasePt,double dNout,double dEout)
{
	double Np=0.0,Ep=0.0;
	double Dis=0.0;
	GetPoint(Np,Ep,dLToBasePt);
	Dis=sqrt((dNout-Np)*(dNout-Np)+(dEout-Ep)*(dEout-Ep));
	return Dis;
}

/*************************************************************
函数名称: GoldenDisPtToHhqx
函数功能、性能等的描述: 黄金分割搜索CHhqx::GoldenDisPtToHhqx函数的极小值 
被本函数调用的函数清单: CHhqx::DisPtToHhqx
调用本函数的函数清单:	CHhqx::MinDisPtToHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:
ax         搜索区间的下限值；
bx		   搜索区间内的一个值，越接近xmin越好，如果开始无法估计xmin则可给区间的中间值(ax+bx)/2;		   
cx		   搜索区间的上限值；
dNout	   曲线外点N坐标;
dEout	   曲线外点E坐标;
tol		   搜索精度；
对输出参数的说明:	xmin   -Getafa在[ax,bx]区间上极小值对应的x坐标；
函数返回值的说明:   CHhqx::GoldenDisPtToHhqx函数的极小值
其它说明:  无
***************************************************************/
double CHhqx::GoldenDisPtToHhqx(double& xmin, double ax, double bx, double cx, double tol, double dNout,double dEout)
{
	double x0,x1,x2,x3,f0,f1,f2,f3,r= 0.61803399;
	x0=x1=x2=f0=f1=f2=f3=0.0;
	double c = 0.38196601;
	x0 = ax;
	x3 = cx;
	if (fabs(cx - bx) > fabs(bx - ax))
	{
		x1 = bx;
		x2 = bx + c * (cx - bx);
	}
	else
	{
		x2 = bx;
		x1 = bx - c * (bx - ax);
	}
	f1 = DisPtToHhqx(x1,dNout,dEout);//求极小值的目标函数
	f2 = DisPtToHhqx(x2,dNout,dEout);//求极小值的目标函数
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = DisPtToHhqx(x2,dNout,dEout);//求极小值的目标函数
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = DisPtToHhqx(x1,dNout,dEout);//求极小值的目标函数
		}
	}
	if (f1 < f2)
	{
		xmin = x1;
		return f1;
	}
	else
	{
		xmin = x2;
		return f2;
	}
}

/*************************************************************
函数名称: MinDisPtToHhqx
函数功能、性能等的描述: 计算点(dNout,dEout)到缓和曲线的最短距离  
被本函数调用的函数清单: CBasicFun::DisAndAngleByX
调用本函数的函数清单:	CHhqx::GetJdArcHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNout				缓和曲线外点的N坐标;
dEout				缓和曲线外点的E坐标;
对输出参数的说明:	无
函数返回值的说明:   计算点(dNout,dEout)到缓和曲线的最短距离
其它说明:  无
***************************************************************/
double CHhqx::MinDisPtToHhqx(double &dLToBasePt,double dNout,double dEout)
{
	double MinDis=0.0,StartDis=0.0,EndDis=0.0,MidDis=0.0;
	StartDis=DisPtToHhqx(m_StartL,dNout,dEout);
	EndDis=DisPtToHhqx(m_EndL,dNout,dEout);
	MidDis=DisPtToHhqx((m_StartL+m_EndL)/2,dNout,dEout);
	if ((StartDis-MidDis)*(MidDis-EndDis)<0)
	{
		MinDis=GoldenDisPtToHhqx(dLToBasePt,m_StartL,(m_StartL+m_EndL)/2,m_EndL,1e-5,dNout,dEout);
	}
	else
		MinDis=StartDis<EndDis?StartDis:EndDis;
	return MinDis;

}

/*************************************************************
函数名称: GetPointTy  
函数功能、性能等的描述: 计算缓和曲线外一点（Nout,Eout）在缓和曲线上的投影点坐标和投影点到参照点的曲线长。    
被本函数调用的函数清单: CHhqx::Getafa,RtflspGetafa,GoldenGetAfa
调用本函数的函数清单:	  
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNout	   曲线外点N坐标;
dEout	   曲线外点E坐标;
对输出参数的说明:	
dNp1	   缓和曲线上第一个投影点的N坐标；
dEp1	   缓和曲线上第一个投影点的E坐标；
dLtoBasePt1  第一个投影点距离参照点的长度；
dNp2		缓和曲线上第二个投影点的N坐标；
dEp2		缓和曲线上第二个投影点的E坐标；
dLtoBasePt2  第二个投影点距离参照点的曲线长；
iNjd	    缓和曲线上投影点的个数；
函数返回值的说明: 能找到该垂足返回ture否则返回false;
其它说明:       
***************************************************************/
bool CHhqx::GetPointTy(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNout,double dEout)
{
	double afa1=0.0,afa2=0.0;
	double dLToBasePt;
	dLToBasePt=0.0;
	iNjd=0;
	if (MinDisPtToHhqx(dLToBasePt,dNout,dEout)<1e-5)
	{
		AfxMessageBox(L"指定点在缓和曲线上");
		return false;
	}
	afa1=Getafa(dNp1,dEp1,m_StartL,dNout,dEout);
	afa2=Getafa(dNp1,dEp1,m_EndL,dNout,dEout);

	if (max(afa1,afa2)>pi/2-1e-5 && min(afa1,afa2)<pi/2+1e-5)
	{	
		dLtoBasePt1=RtflspGetafa(dNp1,dEp1,m_StartL,m_EndL,1e-5,dNout,dEout);
		iNjd++;
		return true;
	}
	else if (max(afa1,afa2)<pi/2-1e-5)
	{
		double g,ax,bx,cx,tol=1e-5;
		double xmin;
		ax=m_StartL;
		bx=(m_StartL+m_EndL)/2;
		cx=m_EndL;
		g=GoldenGetAfa(xmin,ax,bx,cx,tol,dNout,dEout);

		if (Getafa(dNp1,dEp1,xmin,dNout,dEout)<pi/2-1e-5)	
			return false;

		dLtoBasePt1=RtflspGetafa(dNp1,dEp1,m_StartL,xmin,1e-5,dNout,dEout);
		iNjd++;
		dLtoBasePt2=RtflspGetafa(dNp2,dEp2,xmin,m_EndL,1e-5,dNout,dEout);
		iNjd++;
		return true;
	}
	else 
		return false;
}
/*************************************************************
函数名称: GetJdZxHhqx  
函数功能、性能等的描述: 求直线段与缓和曲线的交点坐标和交点距离参照点的长度。    
被本函数调用的函数清单: CBasicFun::DistanceFromPttoLine
CHhqx::GetPoint;
调用本函数的函数清单:	
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
dNstar	   直线段起点N坐标;
dEstar	   直线段起点E坐标;
dNend	   直线段终点N坐标;
dEend	   直线段终点E坐标;
对输出参数的说明:	   
dNp1	   缓和曲线与直线段第一个交点的N坐标；
dEp1 	   缓和曲线与直线段第一个交点的E坐标；
dNp2	   缓和曲线与直线段第二个交点的N坐标；
dEp2 	   缓和曲线与直线段第二个交点的E坐标；
iNjd	   缓和曲线与直线段交点的个数；
函数返回值的说明: 能找到交点返回ture否则返回false;
其它说明:   并不是所有的直线段与缓和曲线都有两个交点。当只有一个交点时Np2,Ep2都将不赋值
即等于系统随机给定的初始值；  
***************************************************************/
bool CHhqx::GetJdZxHhqx(double &dN1,double &dE1,double &dLtoBasePt1,double &dN2,double &dE2,double &dLtoBasePt2,int &iNjd,double dNstar,double dEstar,double dNend,double dEend)const
{
	double Nmax=0.0, Nmin=0.0, Emax=0.0, Emin=0.0;
	double LtoBasePt=0.0;
	double N=0.0,E=0.0;
	double dis=100;
	double Zxj;
	Zxj=0;
	double QxjStart,QxjEnd,QxjToStartFwj,Qxj;
	QxjStart=QxjEnd=QxjToStartFwj=Qxj=0.0;
	double Fwj=0.0,Fwj1=0.0,Fwj2=0.0;
	int cout=0;
	QxjStart=CalQxj(m_StartL);
	QxjEnd=CalQxj(m_EndL);
	DisAndAngleByX(Fwj,dNstar,dEstar,dNend,dEend);//计算直线的方位角
	Fwj1=Fwj;
	Fwj2=Fwj+pi;
	FormFwj(Fwj2);
	Zxj=ZxAngle(Fwj1,m_StartFwj);
	QxjToStartFwj=m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if(Qxj<QxjEnd)
		LtoBasePt=RtflspCalQxj(m_StartL,m_EndL,1e-5,Qxj);
	Zxj=ZxAngle(Fwj2,m_StartFwj);
	QxjToStartFwj=m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if (Qxj<QxjEnd)
		LtoBasePt=RtflspCalQxj(m_StartL,m_EndL,1e-5,Qxj);
	GetPoint(N,E,LtoBasePt);
	/*判断距离直线的距离*/
	dis=DistanceFromPttoLine(N,E,dNstar,dEstar,dNend,dEend);
	if(fabs(dis)<1e-5)//距离小于1e-5认为缓和曲线与直线相切
	{
		AfxMessageBox(L"直线与缓和曲线相切");
		dLtoBasePt1=LtoBasePt;
		dN1=N;
		dE1=E;
		return true;
	}


	Nmax=dNstar>dNend?dNstar:dNend;
	Nmin=dNstar<dNend?dNstar:dNend;
	Emax=dEstar>dEend?dEstar:dEend;
	Emin=dEstar<dEend?dEstar:dEend;
	iNjd=0;
	LtoBasePt=m_StartL;
	dis=100;
	while(LtoBasePt<m_EndL+1e-5 && dis>1e-5)
	{
		GetPoint(N,E,LtoBasePt);
		dis=DistanceFromPttoLine(N,E,dNstar,dEstar,dNend,dEend);
		dis=fabs(dis);
		LtoBasePt+=dis;
	}
	if (dis<1e-5 && N>Nmin-1e-5 && N<Nmax+1e-5 && E>Emin-1e-5 && E<Emax+1e-5)
	{
		dLtoBasePt1=LtoBasePt;
		dN1=N;
		dE1=E;
		iNjd++;
	}

	LtoBasePt=m_EndL;
	dis=100;	
	while (LtoBasePt>m_StartL-1e-5 && dis>1e-5)
	{
		GetPoint(N,E,LtoBasePt);
		dis=DistanceFromPttoLine(N,E,dNstar,dEstar,dNend,dEend);
		dis=fabs(dis);
		LtoBasePt-=dis;
		cout++;
	}
	if(dis<1e-3 && N>Nmin-1e-3 && N<Nmax+1e-3 && E>Emin-1e-3 && E<Emax+1e-3)
	{
		if(iNjd>0 && sqrt((dN1-N)*(dN1-N)+(dE1-E)*(dE1-E))<1e-3)
		{
			dLtoBasePt1=LtoBasePt;
			dN1=N;
			dE1=E;
		}

		if(iNjd>0 && sqrt((dN1-N)*(dN1-N)+(dE1-E)*(dE1-E))>1e-3)
		{
			dLtoBasePt2=LtoBasePt;
			dN2=N;
			dE2=E;
			iNjd++;
		}
		if (iNjd==0)
		{
			dLtoBasePt1=LtoBasePt;
			dN1=N;
			dE1=E;
			iNjd++;
		}
	}
	if (iNjd==0)
		return false;
	else
		return true;	
}

/*************************************************************
函数名称: GetJdArcHhqx  
函数功能、性能等的描述: 求缓和曲线与圆弧的交点;  
被本函数调用的函数清单: CHhqx::GetPoint,CheckIfOnArc;
调用本函数的函数清单:	
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
No		    圆弧的圆心N坐标;
Eo			圆弧的圆心E坐标;
Anglestart	圆弧起点的角度;
Angleend	圆弧终点的角度;
r			圆弧半径
mod        缓和曲线类型 取值为0-6的整数;
对输出参数的说明:	
dN1	   缓和曲线与直线段第一个交点的N坐标；
E1     缓和曲线与直线段第一个交点的E坐标；
N2	   缓和曲线与直线段第二个交点的N坐标；
E2     缓和曲线与直线段第二个交点的E坐标；
Njd	   缓和曲线与直线段交点的个数；
函数返回值的说明: 能找到交点返回ture否则返回false;
其它说明:   并不是所有的直线段与缓和曲线都有两个交点。当只有一个交点时N2,E2都将不赋值
即等于系统随机给定的初始值；
圆弧被认为是从起点开始按逆时针方向到终点；
圆弧起点和终点的角度是以圆心为坐标原点建立的XoY坐标系下圆心与起点、终点的连线的角度值
角度以逆时针方向为正。
对于这些方向和起终点的定义是与CAD中的定义一致的
***************************************************************/
bool CHhqx::GetJdArcHhqx(double &dNp1,double &dEp1,double &dNp2,double &dEp2,int &iNjd,double dNcircle,double dEcircle,double dRcircle,double dAngleStart,double dAngleEnd)const
{
	double LtoBasePt=0.0;
	double N=0.0,E=0.0;
	double dis=100;
	iNjd=0;
	LtoBasePt=m_StartL;
	while(LtoBasePt<m_EndL+1e-5 && dis>1e-5)
	{
		GetPoint(N,E,LtoBasePt);
		dis=sqrt((N-dNcircle)*(N-dNcircle)+(E-dEcircle)*(E-dEcircle))-dRcircle;
		dis=fabs(dis);
		LtoBasePt+=dis;
	}
	if (dis<1e-5 && CheckIfOnArc(dNcircle,dEcircle,dAngleStart,dAngleEnd,N,E))
	{
		dNp1=N;
		dEp1=E;
		iNjd++;
	}

	LtoBasePt=m_EndL;
	dis=100;
	while (LtoBasePt>m_StartL-1e-5 && dis>1e-5)
	{
		GetPoint(N,E,LtoBasePt);
		dis=sqrt((N-dNcircle)*(N-dNcircle)+(E-dEcircle)*(E-dEcircle))-dRcircle;
		dis=fabs(dis);
		LtoBasePt-=dis;
	}
	if(dis<1e-5 && CheckIfOnArc(dNcircle,dEcircle,dAngleStart,dAngleEnd,N,E))
	{
		if(iNjd>0 && sqrt((dNp1-N)*(dNp1-N)+(dEp1-E)*(dEp1-E))<1e-5)
		{
			dNp1=N;
			dEp1=E;
		}

		if(iNjd>0 && sqrt((dNp1-N)*(dNp1-N)+(dEp1-E)*(dEp1-E))>1e-5)
		{
			dNp2=N;
			dEp2=E;
			iNjd++;
		}
		if (iNjd==0)
		{
			dNp1=N;
			dEp1=E;
			iNjd++;
		}
	}
	if (iNjd==0)
		return false;
	else
		return true;	
}

/*************************************************************
函数名称: LtoDeta  
函数功能、性能等的描述: 以第一条缓和曲线起点为原点，第一条缓和曲线的起点切线方向为X轴建立XOY坐标系
求出第二条缓和曲线和第一条缓和曲线在相同的X坐标下的Y坐标差值
被本函数调用的函数清单: CHhqx::GetPoint，GetJd1ZxHhqx
CBasicFun::DistanceFromPttoLine
调用本函数的函数清单:	CBasicFun::GoldenLtoDeta,RtflspLtoDeta,GetJdHhqxHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
Hhqx1		    第一条缓和曲线对象;
Hhqx2			第二条缓和曲线对象;
dLtoBasePt      第二条缓和曲线上的任意点到参照点的长度;
函数返回值的说明: 求出第二条缓和曲线和第一条缓和曲线在相同的X坐标下的Y坐标差值
其它说明:   无
***************************************************************/

double CHhqx::LtoDeta(const CHhqx Hhqx1,const CHhqx Hhqx2,double dLtoBasePt,int iFromwhere1)const
{
	double N1=0.0 ,E1=0.0, N2=0.0, E2=0.0;
	double deta1=0.0 ,deta2=0.0;
	double LtoBasePt1=0.0;
	Hhqx2.GetPoint(N2,E2,dLtoBasePt);
	Hhqx1.GetJd1ZxHhqx(N1,E1,LtoBasePt1,N2+1e10*cos(Hhqx1.m_StartFwj-0.5*pi),E2+1e10*sin(Hhqx1.m_StartFwj-0.5*pi),N2+1e10*cos(Hhqx1.m_StartFwj+0.5*pi),E2+1e10*sin(Hhqx1.m_StartFwj+0.5*pi),iFromwhere1);
	deta1=DistanceFromPttoLine(N1,E1,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	deta2=DistanceFromPttoLine(N2,E2,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	return deta1-deta2;	
}
/*************************************************************
函数名称: GoldenLtoDeta  
函数功能、性能等的描述: 黄金分割搜索求出LtoDeta函数的极小值（极大值）和极小值（极大值）点处在第二条缓和曲线上距离参照点的长度;
被本函数调用的函数清单: CHhqx::LtoDeta;
调用本函数的函数清单:	CHhqx::GetJdHhqxHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
ax		    求极小值的区间下限;
bx			求极小值的区间中点处的值;（该值只要给在区间内就可以了，一般可以给在区间中点这样比较快）
cx			求极小值的区间上限;
tol			精度;
Hhqx1		第一条缓和曲线;
Hhqx2		第二条缓和曲线;
imaxormin	为1时计算极小值，为-1时计算极大值
输出参数说明:
xminormax        函数达到极小值或极大值时对应的第二条缓和曲线上的任意点到参照点的长度;
函数返回值的说明:  返回LtoDeta函数的极小值
其它说明:   
***************************************************************/

double CHhqx::GoldenLtoDeta(double& xminormin,double ax, double bx, double cx, double tol, int imaxormin, const CHhqx Hhqx1,const CHhqx Hhqx2,int iFromwhere1)const
{
	double x0,x1,x2,x3,f0,f1,f2,f3,r= 0.61803399;
	double c = 0.38196601;
	x0=x1=x2=x3=f0=f1=f2=f3=0.0;
	x0 = ax;
	x3 = cx;
	if (fabs(cx - bx) > fabs(bx - ax))
	{
		x1 = bx;
		x2 = bx + c * (cx - bx);
	}
	else
	{
		x2 = bx;
		x1 = bx - c * (bx - ax);
	}
	f1 = imaxormin*LtoDeta(Hhqx1,Hhqx2,x1,iFromwhere1);
	f2 = imaxormin*LtoDeta(Hhqx1,Hhqx2,x2,iFromwhere1);
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = imaxormin*LtoDeta(Hhqx1,Hhqx2,x2,iFromwhere1);
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = imaxormin*LtoDeta(Hhqx1,Hhqx2,x1,iFromwhere1);
		}
	}
	if (f1 < f2)
	{
		xminormin = x1;
		return f1;
	}
	else
	{
		xminormin = x2;
		return f2;
	}
}

/*************************************************************
函数名称: GoldenQj  
函数功能、性能等的描述: 以第一条缓和曲线起点为原点，第一条缓和曲线的起点切线方向为X轴建立XOY坐标系;
求出在该坐标系下两缓和曲线X可取值范围的交集
被本函数调用的函数清单: CHhqx::GetJd1ZxHhqx;
CBasicFun::ChuizuToZx,CCW2;
调用本函数的函数清单:	CHhqx::GetJdHhqxHhqx
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
Hhqx1		    第一条缓和曲线对象;
Hhqx2			第二条缓和曲线对象;
输出参数说明:
al				交集的区间下限;
cl				交集的区间上限；
函数返回值的说明:  交集为空时返回false 否则返回true;
其它说明:   
***************************************************************/

bool CHhqx::GoldenQj(double &al,double &cl,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL,int iFromwhere2)const
{
	double ax,cx;
	ax=cx=0.0;
	double LtoBasePt;
	LtoBasePt=0;
	double TempX;
	TempX=0.0;
	//	double l1start,l1end,l2start,l2end;
	double X1Start,X1End,X2Start,X2End;
	X1Start=X1End=X2Start=X2End=0.0;
	double N1,E1;
	N1=E1=0.0;
	double NHhqx2QjStart,EHhqx2QjStart,NHhqx2QjEnd,EHhqx2QjEnd;
	NHhqx2QjStart=EHhqx2QjStart=NHhqx2QjEnd=EHhqx2QjEnd=0;
	double N,E;
	N=E=0.0;
	//	double Hhqx1EndNchuizu,Hhqx1EndEchuizu;
	double Nchuizu,Echuizu;
	Nchuizu=Echuizu=0.0;
	//	l1start = 0.0;
	Hhqx2.GetPoint(NHhqx2QjStart,EHhqx2QjStart,dHhqx2QjStartL);
	ChuizuToZx(Nchuizu,Echuizu,NHhqx2QjStart,EHhqx2QjStart,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	X2Start=sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
		X2Start *=-1 ;

	Hhqx2.GetPoint(NHhqx2QjEnd,EHhqx2QjEnd,dHhqx2QjEndL);
	ChuizuToZx(Nchuizu,Echuizu,NHhqx2QjEnd,EHhqx2QjEnd,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	X2End=sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
		X2End *=-1 ;

	Hhqx1.GetPoint(N,E,dHhqx1QjStartL);
	ChuizuToZx(Nchuizu,Echuizu,N,E,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	X1Start=sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
		X1Start *=-1 ;

	Hhqx1.GetPoint(N,E,dHhqx1QjEndL);
	ChuizuToZx(Nchuizu,Echuizu,N,E,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	X1End=sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
		X1End *=-1 ;

	//	ChuizuToZx(Hhqx1EndNchuizu,Hhqx1EndEchuizu,Hhqx1.m_EndN,Hhqx1.m_EndE,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	//	l1end = sqrt((Hhqx1EndNchuizu-Hhqx1.m_StartN)*(Hhqx1EndNchuizu-Hhqx1.m_StartN)+(Hhqx1EndEchuizu-Hhqx1.m_StartE)*(Hhqx1EndEchuizu-Hhqx1.m_StartE));
	//	ChuizuToZx(Nchuizu,Echuizu,Hhqx2.m_StartN,Hhqx2.m_StartE,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	//	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
	//	{   
	//		l2start = l1start - sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	//	}
	//	else
	//	{
	//		l2start = l1start + sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	//	}
	//	ChuizuToZx(Nchuizu,Echuizu,Hhqx2.m_EndN,Hhqx2.m_EndE,Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj));
	//
	//	if (CCW2(Hhqx1.m_StartN,Hhqx1.m_StartE,Hhqx1.m_StartN+10*cos(Hhqx1.m_StartFwj-0.5*pi),Hhqx1.m_StartE+10*sin(Hhqx1.m_StartFwj-0.5*pi),Nchuizu,Echuizu)<0)
	//	{		
	//		l2end = l1start - sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	//	}
	//	else
	//	{		
	//		l2end = l1start + sqrt((Nchuizu-Hhqx1.m_StartN)*(Nchuizu-Hhqx1.m_StartN)+(Echuizu-Hhqx1.m_StartE)*(Echuizu-Hhqx1.m_StartE));
	//	}
	//	if (l2end>l2start)
	//	{
	//		ax = l1start > l2start ? l1start : l2start; 
	//		cx = l1end < l2end ? l1end : l2end;
	//		ads_printf(L"ax=%lf,cx=%lf\n",ax,cx);
	double TempX2Start,TempX2End;
	TempX2Start=X2Start;
	TempX2End=X2End;
	if (X1Start>X1End)
	{
		TempX=X1Start;
		X1Start=X1End;
		X1End=TempX;
	}

	if (X2Start>X2End)
	{
		TempX=X2Start;
		X2Start=X2End;
		X2End=TempX;
	}

	ax = X1Start > X2Start ? X1Start : X2Start;
	cx = X1End < X2End ? X1End : X2End;
	ads_printf(L"ax=%lf,cx=%lf\n",ax,cx);
	if (ax>cx)
		return false;
	else
	{
		if (fabs(ax-TempX2Start)<1e-5)
		{
			al=dHhqx2QjStartL;
		} 
		else if(fabs(ax-TempX2End)<1e-5)
		{
			al=dHhqx2QjEndL;
		}
		else
		{
			N = Hhqx1.m_StartN + ax*cos(Hhqx1.m_StartFwj);
			E = Hhqx1.m_StartE + ax*sin(Hhqx1.m_StartFwj);
			Hhqx2.GetJd1ZxHhqx(N1,E1,LtoBasePt,N+1e10*cos(Hhqx1.m_StartFwj-pi/2),E+1e10*sin(Hhqx1.m_StartFwj-pi/2),N+1e10*cos(Hhqx1.m_StartFwj+pi/2),E+1e10*sin(Hhqx1.m_StartFwj+pi/2),iFromwhere2);
			al = LtoBasePt;
		}
		if (fabs(cx-TempX2Start)<1e-5)
		{
			cl=dHhqx2QjStartL;
		} 
		else if(fabs(cx-TempX2End)<1e-5)
		{
			cl=dHhqx2QjEndL;
		}
		else
		{
			N = Hhqx1.m_StartN + cx*cos(Hhqx1.m_StartFwj);
			E = Hhqx1.m_StartE + cx*sin(Hhqx1.m_StartFwj);
			Hhqx2.GetJd1ZxHhqx(N1,E1,LtoBasePt,N-1e10*cos(Hhqx1.m_StartFwj-pi/2),E-1e10*sin(Hhqx1.m_StartFwj-pi/2),N+1e10*cos(Hhqx1.m_StartFwj-pi/2),E+1e10*sin(Hhqx1.m_StartFwj-pi/2),iFromwhere2);
			cl = LtoBasePt;
		}
		ads_printf(L"al=%lf,cl=%lf\n",al,cl);
		return true;
	}
	//	} 
	//	else
	//	{
	//		ax = l1start > l2end ? l1start : l2end; 
	//		cx = l1end < l2start ? l1end : l2start;
	//		ads_printf(L"ax=%lf,cx=%lf\n",ax,cx);
	//		if (ax>cx)
	//			return false;
	//		else
	//		{
	//			N = Hhqx1.m_StartN + ax*cos(Hhqx1.m_StartFwj);
	//			E = Hhqx1.m_StartE + ax*sin(Hhqx1.m_StartFwj);
	//			Hhqx2.GetJd1ZxHhqx(N1,E1,LtoBasePt,N-1e10*cos(Hhqx1.m_StartFwj-pi/2),E-1e10*sin(Hhqx1.m_StartFwj-pi/2),N+1e10*cos(Hhqx1.m_StartFwj-pi/2),E+1e10*sin(Hhqx1.m_StartFwj-pi/2));
	//			cl = LtoBasePt;
	//			N = Hhqx1.m_StartN + cx*cos(Hhqx1.m_StartFwj);
	//			E = Hhqx1.m_StartE + cx*sin(Hhqx1.m_StartFwj);
	//			Hhqx2.GetJd1ZxHhqx(N1,E1,LtoBasePt,N-1e10*cos(Hhqx1.m_StartFwj-pi/2),E-1e10*sin(Hhqx1.m_StartFwj-pi/2),N+1e10*cos(Hhqx1.m_StartFwj-pi/2),E+1e10*sin(Hhqx1.m_StartFwj-pi/2));
	//			al = LtoBasePt;
	//			ads_printf(L"al=%lf,cl=%lf\n",al,cl);
	//			return true;
	//		}
	//	}	
}

/*************************************************************
函数名称: GetJd1ZxHhqx  
函数功能、性能等的描述: 求直线段与缓和曲线的一个交点坐标。    
被本函数调用的函数清单: CBasic::DistanceFromPttoLine
CHhqx::GetPoint
调用本函数的函数清单:	CHhqx::GoldenQj,LtoDeta;
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
Nstar	   直线段起点N坐标;
Estar	   直线段起点E坐标;
Nend	   直线段终点N坐标;
Eend	   直线段终点E坐标;
对输出参数的说明:	   
Np1		   缓和曲线与直线段一个交点的N坐标；
Ep1 	   缓和曲线与直线段一个交点的E坐标；
LtoBasePt1 缓和曲线与直线段交点距离参照点的长度；
函数返回值的说明: 能找到交点返回ture否则返回false;
其它说明:   该函数是针对两缓和曲线求交点的需要对GetJdZxHhqx的改进函数
因为在在求两缓和曲线交点中调用直线与缓和曲线求交时只可能出现一个交点
如果还用考虑两个交点的GetJdZxHhqx函数处理速度特别慢，而且没有必要
经过测试用该函数速度可以提高两倍以上；
***************************************************************/

bool CHhqx::GetJd1ZxHhqx(double &dNp1,double &dEp1,double &dLtoBasePt1,double dNstart,double dEstart,double dNend,double dEend,int iFromwhere)const
{
	double Nmax,Nmin,Emax,Emin;
	Nmax=Nmin=Emax=Emin=0.0;
	double LtoBasePt=-1;
	double Zxj;
	Zxj=0;
	double QxjStart,QxjEnd,QxjToStartFwj,Qxj;
	QxjStart=QxjEnd=QxjToStartFwj=Qxj=0.0;
	double N,E;
	N=E=0.0;
	double disstart,disend,dis;
	disstart=disend=dis=0;
	double Fwj=0.0,Fwj1=0.0,Fwj2=0.0;
	QxjStart=CalQxj(m_StartL);
	QxjEnd=CalQxj(m_EndL);
	DisAndAngleByX(Fwj,dNstart,dEstart,dNend,dEend);//计算直线的方位角
	Fwj1=Fwj;
	Fwj2=Fwj+pi;
	FormFwj(Fwj2);
	Zxj=ZxAngle(Fwj1,m_StartFwj);
	QxjToStartFwj=m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if(Qxj<QxjEnd)
		LtoBasePt=RtflspCalQxj(m_StartL,m_EndL,1e-5,Qxj);

	Zxj=ZxAngle(Fwj2,m_StartFwj);
	QxjToStartFwj=m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if (Qxj<QxjEnd)
		LtoBasePt=RtflspCalQxj(m_StartL,m_EndL,1e-5,Qxj);
	if (LtoBasePt>0)
	{
		GetPoint(N,E,LtoBasePt);
		/*判断距离直线的距离*/
		dis=DistanceFromPttoLine(N,E,dNstart,dEstart,dNend,dEend);
		if(fabs(dis)<1e-5)//距离小于1e-5认为缓和曲线与直线相切
		{
			dLtoBasePt1=LtoBasePt;
			dNp1=N;
			dEp1=E;
			return true;
		}
	}



	disstart=fabs(DistanceFromPttoLine(m_StartN,m_StartE,dNstart,dEstart,dNend,dEend));
	disend = fabs(DistanceFromPttoLine(m_EndN,m_EndE,dNstart,dEstart,dNend,dEend));

	if (fabs(disstart)<1e-5)
	{
		dNp1=m_StartN;
		dEp1=m_StartE;
		dLtoBasePt1=m_StartL;
		return true;
	}
	if (fabs(disend)<1e-5)
	{
		dNp1=m_EndN;
		dEp1=m_EndE;
		dLtoBasePt1=m_EndL;
		return true;
	}
	Nmax=dNstart>dNend?dNstart:dNend;
	Nmin=dNstart<dNend?dNstart:dNend;
	Emax=dEstart>dEend?dEstart:dEend;
	Emin=dEstart<dEend?dEstart:dEend;
	//	if (disstart<=disend)
	if(iFromwhere==1)
	{
		dis=100;
		LtoBasePt=m_StartL;
		while(LtoBasePt<m_EndL+1e-5 && dis>1e-5)
		{
			GetPoint(N,E,LtoBasePt);
			dis=DistanceFromPttoLine(N,E,dNstart,dEstart,dNend,dEend);
			dis=fabs(dis);
			LtoBasePt+=dis;
		}
		if (dis<1e-5 && N>Nmin-1e-5 && N<Nmax+1e-5 && E>Emin-1e-5 && E<Emax+1e-5)
		{
			dLtoBasePt1=LtoBasePt;
			dNp1=N;
			dEp1=E;
			return true;
		}
		else
			return false;
	} 
	else
	{
		dis=100;
		LtoBasePt=m_EndL;
		while(LtoBasePt>m_StartL-1e-5 && dis>1e-5)
		{
			GetPoint(N,E,LtoBasePt);
			dis=DistanceFromPttoLine(N,E,dNstart,dEstart,dNend,dEend);
			dis=fabs(dis);
			LtoBasePt -= dis;
		}
		if (dis<1e-5 && N>Nmin-1e-5 && N<Nmax+1e-5 && E>Emin-1e-5 && E<Emax+1e-5)
		{
			dLtoBasePt1=LtoBasePt;
			dNp1=N;
			dEp1=E;
			return true;
		}
		else
			return false;
	}

}
/*************************************************************
函数名称: RtflspLtoDeta   
函数功能、性能等的描述: 该函数为试位法求解LtoDeta函数值为0时的dLtoBasePt					
被本函数调用的函数清单: CHhqx::LtoDeta
调用本函数的函数清单:   CHhqx::GetJdHhqxHhqx 
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
x1			试算求根区间的下限值；
x2			试算求根区间的下限值；
xacc		试算求根的精度；
Hhqx1	    第一条缓和曲线对象；
Hhqx2	    第二条缓和曲线对象；
对输出参数的说明: 
函数返回值的说明:   返回LtoDeta函数值为0时的dLtoBasePt     
其它说明:      
***************************************************************/
double CHhqx::RtflspLtoDeta(double x1,double x2,double xacc,const CHhqx Hhqx1,const CHhqx Hhqx2,int iFromwhere1)const
{
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	j=0;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=LtoDeta(Hhqx1,Hhqx2,x1,iFromwhere1);
	fh=LtoDeta(Hhqx1,Hhqx2,x2,iFromwhere1);
	if (fl*fh>0)
	{
		AfxMessageBox(L"root must be bracketed for false position");
	}
	if (fl<0)
	{
		xl=x1;
		xh=x2;
	}
	else
	{
		xl=x2;
		xh=x1;
		swap=fl;
		fl=fh;
		fh=swap;
	}
	dx=xh-xl;
	for (j=1;j<=maxit;j++)
	{
		temp=xl+dx*fl/(fl-fh);
		f=LtoDeta(Hhqx1,Hhqx2,temp,iFromwhere1);
		if (f<0)
		{
			del=xl-temp;
			xl=temp;
			fl=f;
		}
		else
		{
			del=xh-temp;
			xh=temp;
			fh=f;
		}
		dx=xh-xl;
		if ((fabs(del)<xacc)||(f==0)) break;

	}
	return temp;
	AfxMessageBox(L"rtflsp exceed maximum iterations");	
}

/*************************************************************
函数名称: GetJdHhqxHhqx  
函数功能、性能等的描述: 求两缓和曲线的交点；
被本函数调用的函数清单: CHhqx::GoldenQj,LtoDeta,RtflspLtoDeta,GetPoint,GoldenLtoDeta;
调用本函数的函数清单:	
被访问的表（此项仅对于牵扯到数据库操作的程序）: 无
被修改的表（此项仅对于牵扯到数据库操作的程序）: 无
输入参数说明:   
Hhqx1	   第一条缓和曲线对象；
Hhqx2	   第二条缓和曲线对象；
对输出参数的说明:	   
dNp1	   两缓和曲线第一个交点的N坐标；
dEp1 	   两缓和曲线第一个交点的E坐标；
Np2		   两缓和曲线第二个交点的N坐标；
Ep2 	   两缓和曲线第二个交点的E坐标；
iNjd	   交点个数；
dLtoBasePt1 两缓和曲线第一个交点（第二条缓和曲线上）距离第二条缓和曲线参照点的长度；
dLtoBasePt2 两缓和曲线第二个交点（第二条缓和曲线上）距离第二条缓和曲线参照点的长度；
函数返回值的说明: 能找到交点返回ture否则返回false;
其它说明:   
***************************************************************/

bool CHhqx::GetJdHhqxHhqx(double &dN1,double &dE1,double &dLtoBasePt1,double &dN2,double &dE2,double &dLtoBasePt2,int &iNjd,const CHhqx Hhqx1,const CHhqx Hhqx2,double dHhqx1QjStartL,double dHhqx1QjEndL,double dHhqx2QjStartL,double dHhqx2QjEndL)const
{
	double ax,bx,cx;
	ax=bx=cx=0.0;
	double g;
	g=0.0;
	double tol=1e-5;
	double xmin,xmax;
	xmin=xmax=0.0;
	double deta1,deta2;
	deta1=deta2=0.0;
	int fromwhere1,fromwhere2;
	fromwhere1=fromwhere2=0;
	iNjd=0;
	//	/*分别以Hhqx1，Hhqx2的起点切线方向为X轴求算迭代的区间[ax,cx]*/
	//	bool GoldenQj1,GoldenQj2;
	//	GoldenQj1=GoldenQj(ax,cx,Hhqx1,Hhqx2);
	//	if (!GoldenQj1)//如果以Hhqx1的起点切线方向为X轴没有满足条件的迭代区间[ax,cx]则以Hhqx2的起点切线方向为X轴
	//	{
	//		GoldenQj2=GoldenQj(ax,cx,Hhqx2,Hhqx1);
	//		if (!GoldenQj2)//如果Hhqx1，Hhqx2的起点切线方向为X轴都找不到迭代的区间[ax,cx]则表明不存在迭代区间[ax,cx]
	//			return false;
	//		else//以Hhqx1的起点切线方向为X轴能找到[ax,cx]更换Hhqx1，Hhqx2
	//		{
	//			TempHhqx=Hhqx1;
	//			Hhqx1=Hhqx2;
	//			Hhqx2=TempHhqx;
	//		}		
	//	}
	if (fabs(dHhqx1QjStartL-Hhqx1.m_StartL)<1e-6)
	{
		fromwhere1=1;
	}
	else
		fromwhere1=2;

	if (fabs(dHhqx2QjStartL-Hhqx2.m_StartL)<1e-6)
	{
		fromwhere2=1;
	}
	else
		fromwhere2=2;

	if(GoldenQj(ax,cx,Hhqx1,Hhqx2,dHhqx1QjStartL,dHhqx1QjEndL,dHhqx2QjStartL,dHhqx2QjEndL,fromwhere2))
	{
		deta1=LtoDeta(Hhqx1,Hhqx2,ax,fromwhere1);
		deta2=LtoDeta(Hhqx1,Hhqx2,cx,fromwhere1);
		if (fabs(deta1)<1e-5)
		{
			dLtoBasePt1=ax;
			Hhqx2.GetPoint(dN1,dE1,dLtoBasePt1);
			iNjd++;
			ax+=1e-5;
			deta1=LtoDeta(Hhqx1,Hhqx2,ax,fromwhere1);
		}
		if (fabs(deta2)<1e-5)
		{
			if (iNjd==0)
			{
				dLtoBasePt1=cx;
				Hhqx2.GetPoint(dN1,dE1,dLtoBasePt1);
				iNjd++;
			}
			else
			{
				dLtoBasePt2=cx;
				Hhqx2.GetPoint(dN2,dE2,dLtoBasePt2);
				iNjd++;
				return true;
			}
			cx-=1e-5;
			deta2=LtoDeta(Hhqx1,Hhqx2,cx,fromwhere1);
		}

		if ((deta1<-1e-5 && deta2>1e-5)||(deta2<-1e-5 && deta1>1e-5))
		{
			if (iNjd==0)
			{
				dLtoBasePt1=RtflspLtoDeta(ax,cx,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN1,dE1,dLtoBasePt1);
				iNjd++;
			}
			else
			{
				dLtoBasePt2=RtflspLtoDeta(ax,cx,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN2,dE2,dLtoBasePt2);
				iNjd++;
				return true;
			}					
		}
		else if (deta1>1e-5 && deta2>1e-5)
		{
			bx=(ax+cx)/2;
			g=GoldenLtoDeta(xmin,ax,bx,cx,tol,1,Hhqx1,Hhqx2,fromwhere1);
			if (g<-1e-5)
			{
				dLtoBasePt1=RtflspLtoDeta(ax,xmin,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN1,dE1,dLtoBasePt1);
				iNjd++;

				dLtoBasePt2=RtflspLtoDeta(xmin,cx,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN2,dE2,dLtoBasePt2);
				iNjd++;
				return true;
			}

		}

		else if (deta1<-1e-5 && deta2<-1e-5)
		{
			bx=(ax+cx)/2;
			g=-GoldenLtoDeta(xmax,ax,bx,cx,tol,-1,Hhqx1,Hhqx2,fromwhere1);
			if (g>1e-5)
			{
				dLtoBasePt1=RtflspLtoDeta(ax,xmax,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN1,dE1,dLtoBasePt1);
				iNjd++;

				dLtoBasePt2=RtflspLtoDeta(xmax,cx,1e-5,Hhqx1,Hhqx2,fromwhere1);
				Hhqx2.GetPoint(dN2,dE2,dLtoBasePt2);
				iNjd++;
				return true;
			}			
		}
	}
	if (iNjd==0)
		return false;
	else
		return true;
}

bool CHhqx::GetJdHhqxHhqxHuitou(CArray<Jd,Jd> &HHJd,CHhqx Hhqx1,CHhqx Hhqx2)
{
	double Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2;
	Np1=Ep1=LToBasePt1=Np2=Ep2=LToBasePt2=0.0;
	int Njd=0;
	bool IfFenjie1=false,IfFenjie2=false;	
	double Fwj1,Fwj2,Fwj;
	Fwj=Fwj1=Fwj2=0.0;
	double QxjStart,QxjEnd,QxjToStartFwj,Zxj,Qxj;
	QxjStart=QxjEnd=QxjToStartFwj=Zxj=Qxj=0.0;
	double LtoBasePtFenjie1,LtoBasePtFenjie2;
	LtoBasePtFenjie1=LtoBasePtFenjie2=0.0;
	Fwj=Hhqx1.m_StartFwj;
	QxjStart=Hhqx1.CalQxj(Hhqx1.m_StartL);
	QxjEnd=Hhqx1.CalQxj(Hhqx1.m_EndL);
	Qxj=QxjStart+pi/2;
	if (Qxj<QxjEnd)
	{
		LtoBasePtFenjie1=Hhqx1.RtflspCalQxj(Hhqx1.m_StartL,Hhqx1.m_EndL,1e-5,Qxj);
		IfFenjie1=true;
	}
	Fwj1=Fwj-pi/2;
	FormFwj(Fwj1);
	Fwj2=Fwj+pi/2;
	FormFwj(Fwj2);
	QxjStart=Hhqx2.CalQxj(Hhqx2.m_StartL);
	QxjEnd=Hhqx2.CalQxj(Hhqx2.m_EndL);
	Zxj=ZxAngle(Fwj1,Hhqx2.m_StartFwj);
	QxjToStartFwj=Hhqx2.m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if (Qxj<QxjEnd)
	{
		LtoBasePtFenjie2=Hhqx2.RtflspCalQxj(Hhqx2.m_StartL,Hhqx2.m_EndL,1e-5,Qxj);
		IfFenjie2=true;
	}

	Zxj=ZxAngle(Fwj2,Hhqx2.m_StartFwj);
	QxjToStartFwj=Hhqx2.m_LOrR*Zxj;
	if (QxjToStartFwj<0)
		QxjToStartFwj+=2*pi;
	Qxj=QxjStart+QxjToStartFwj;
	if (Qxj<QxjEnd)
	{
		LtoBasePtFenjie2=Hhqx2.RtflspCalQxj(Hhqx2.m_StartL,Hhqx2.m_EndL,1e-5,Qxj);
		IfFenjie2=true;
	}


	if (IfFenjie1)
	{
		if (IfFenjie2)
		{
			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,LtoBasePtFenjie1,Hhqx2.m_StartL,LtoBasePtFenjie2))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,LtoBasePtFenjie1,LtoBasePtFenjie2,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,LtoBasePtFenjie1,Hhqx1.m_EndL,Hhqx2.m_StartL,LtoBasePtFenjie2))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,LtoBasePtFenjie1,Hhqx1.m_EndL,LtoBasePtFenjie2,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}
		}
		else
		{
			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,LtoBasePtFenjie1,Hhqx2.m_StartL,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,LtoBasePtFenjie1,Hhqx1.m_EndL,Hhqx2.m_StartL,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

		}

	}
	else 
	{
		if (IfFenjie2)
		{
			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,Hhqx1.m_EndL,Hhqx2.m_StartL,LtoBasePtFenjie2))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}

			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,Hhqx1.m_EndL,LtoBasePtFenjie2,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}
		}
		else
		{
			if (Hhqx1.GetJdHhqxHhqx(Np1,Ep1,LToBasePt1,Np2,Ep2,LToBasePt2,Njd,Hhqx1,Hhqx2,Hhqx1.m_StartL,Hhqx1.m_EndL,Hhqx2.m_StartL,Hhqx2.m_EndL))
			{
				if (Njd==1)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
				}
				if (Njd==2)
				{
					m_Jd.dNJd=Np1;
					m_Jd.dEJd=Ep1;
					m_Jd.dLToBasePt=LToBasePt1;
					HHJd.Add(m_Jd);
					m_Jd.dNJd=Np2;
					m_Jd.dEJd=Ep2;
					m_Jd.dLToBasePt=LToBasePt2;
					HHJd.Add(m_Jd);
				}
			}
		}

	}
	Njd=HHJd.GetSize();
	if (Njd==0)
		return false;
	else 
		return true;

}
