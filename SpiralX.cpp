// SpiralX.cpp: implementation of the SpiralX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "SpiralX.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SpiralX::SpiralX()
{

}

SpiralX::~SpiralX()
{

}

//Áú±´¸ñ»ý·Ö
void SpiralX::qromb(double a, double b, double& ss)
{
	double dss,eps = 0.000001;
	const int jmax = 20;
	int jmaxp = jmax + 1;
	int j,k = 5;
	int km = k - 1;
	double s[21], h[21];
	h[1] = 1.0;
	for (j = 1; j<=jmax; j++)
	{
		trapzd(a, b, s[j], j);
		if (j >= k)
		{
			polint(h, s, k, 0.0, ss, dss);
			if (fabs(dss) < eps * fabs(ss))
			{
				return;
			}
		}
		s[j + 1] = s[j];
		h[j + 1] = 0.25 * h[j];
	}
	//cout<<"too many steps."<<endl;
} 	

void SpiralX::trapzd(double a, double b, double& s, int n)
{
	double del,x,sum;
	int j,it, tnm;
	if (n ==1)
	{
		s = 0.5 * (b - a) * (func(a) + func(b));
		it = 1;
	}
	else
	{
		it = (int)pow(2.0 , n - 2);
		tnm = it;
		del = (b - a) / tnm;
		x = a + 0.5 * del;
		sum = 0.0;
		for (j = 1; j<=it; j++)
		{
			sum = sum + func(x);
			x = x + del;
		}
		s = 0.5 * (s + (b - a) * sum / tnm);
	}
} 
void SpiralX::polint(double xa[], double ya[], int n, double x, double& y, double& dy)
{
	double c[11], d[11],dift,dif,ho,hp,w,den;
	int i,m,ns = 1;
	dif = fabs(x - xa[1]);
	for ( i = 1; i<=n; i++)
	{
		dift = fabs(x - xa[i]);
		if (dift < dif)
		{
			ns = i;
			dif = dift;
		}
		c[i] = ya[i];
		d[i] = ya[i];
	}
	y = ya[ns];
	ns = ns - 1;
	for(m = 1; m<=n-1; m++)
	{
		for (i = 1; i<=n-m; i++)
		{
			ho = xa[i] - x;
			hp = xa[i + m] - x;
			w = c[i + 1] - d[i];
			den = ho - hp;
			if (den == 0.0) 
			{
				//              cout<<"pause"<<endl;
				return;
			}
			den = w / den;
			d[i] = hp * den;
			c[i] = ho * den;
		}
		if (2 * ns < n - m )
			dy = c[ns + 1];
		else
		{
			dy = d[ns];
			ns = ns - 1;
		}

		y = y + dy;
	}
}

double SpiralX::func(double a)
{
	if(a<=0.0)return 0.0;
	else return cos(a)/sqrt(a);
}	

double SpiralX::SpiralGetX(double A,double l)
{
	if(l<0.000000000001)return 0.0;
	double R=A*A/l;
	double t=0.5*l/R;
	double K=A/sqrt(2.0);
	double res;
	qromb(0.0,t,res);
	return K*res;
}
