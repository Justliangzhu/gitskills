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
��������: DisAndAngleByX   
�������ܡ����ܵȵ�����: ��������֮��ľ���dDis�����㹹�ɵ�ֱ����x�ļн�dAngleByX
�����������õĺ����嵥: ��
���ñ������ĺ����嵥: CHhqx::set,SetByEnd,GetPoint,GetPoint
CYYLuanLink::Set,L0toCC,
CYYCSLink::Set
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dXstart    ����x���ꣻ
dYstart	   ����y���ꣻ
dXend	   �յ��x����;	
dYend	   �յ��y����;
�����������˵��:  dAngleByX Ϊ����ֱ����X��н�
��������ֵ��˵��:  ���ظ�ֱ�ߵĳ���      
����˵��:  �Դ��N�������X�����E�������Y��dAngleByX�����귽λ�ǡ�  
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
��������: FormFwj   
�������ܡ����ܵȵ�����: �Ѽ�������ĽǶ�ת��Ϊ0-2pi��Χ�ڵķ�λ��
�����������õĺ����嵥: ��
���ñ������ĺ����嵥: CHhqx::set,SetByEnd��
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:      dFWjΪǰ��������ĽǶ�
�����������˵��:  dFwjΪת����ķ�λ��
��������ֵ��˵��:      
����˵��:     
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
��������: ZxAngle   
�������ܡ����ܵȵ�����: ���������ֱ��dLineAngle1��dLineAngle2��ת���
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:	CHhqx::GetAfa
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:      dLineAngle1  ��һ������ֱ�ߵ����귽λ�� 
dLineAngle2  �ڶ�������ֱ�ߵ����귽λ�� 
�����������˵��:  ��
��������ֵ��˵��:  ����dLineAngle1��dLineAngle2��ת���  
����˵��:  ת���˳ʱ�뷽��Ϊ��     
***************************************************************/
double CBasicFun::ZxAngle(double dLineAngle2,double dLineAngle1)
{
	double dAngle=0.0;
	dAngle = dLineAngle2 - dLineAngle1;
	if(fabs(dAngle)>pi)
	{
		if(dAngle<0.0) //ƫ��С��-PI
		{
			dAngle = 2.0*pi + dAngle;
		}
		else      //ƫ�Ǵ���PI
		{
			dAngle = dAngle - 2.0*pi;
		}
	}
	return dAngle;
}

/*************************************************************
��������: DistanceFromPttoLine   
�������ܡ����ܵȵ�����: ��㵽ֱ�߾��룻
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:	CHhqx::GetJd1ZxHhqx��GetJdZxHhqx��LtoDeta;
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:      
dXout      ֱ���������ꣻ
dYout      ֱ���������ꣻ
dXstart    ֱ������X���ꣻ
dYstart	   ֱ������Y���ꣻ
dXend      ֱ���յ��X���ꣻ
dYend      ֱ���յ��Y���ꣻ
�����������˵��:  ��
��������ֵ��˵��:  ����(dXout,dYout)��ֱ�ߵľ��룻
����˵��:		   �ر�Ҫע����Ǹú����ķ���ֵ�����ǵ㵽ֱ�ߵľ��룬
����ֵ���������Է�ӳ����ֱ�ߵĲ�ͬ�࣬����ֵ���ǵ㵽ֱ�ߵľ��룻
���(dXout,dYout)����ֱ����Ҳ���Լ��㣬����ֵΪһ����С��ֵ   
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
��������: ChuizutoZx   
�������ܡ����ܵȵ�����: ��һ����ֱ�����������㣻
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:	CHhqx::GoldenQj;
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:      
dXout      ֱ������X���ꣻ
dYout      ֱ������Y���ꣻ
dXstart    ֱ������X���ꣻ
dYstart	   ֱ������Y���ꣻ
dXend      ֱ���յ��X���ꣻ
dYend      ֱ���յ��Y���ꣻ
�����������˵��:  
dXchuizu   ������X���ꣻ
dYchuizu   ������Y���ꣻ
��������ֵ��˵��:  �޷���ֵ
����˵��:  ��
***************************************************************/

void CBasicFun::ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend)
{
	double k=0.0;//ֱ�ߵ�б��
	double b=0.0;//��ֱ�߻�Ϊy=kx+b���bֵ
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
��������: CCW2   
�������ܡ����ܵȵ�����: ���������������λ���Ƿ�˳ʱ��˳�����У�
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:	CHhqx::GoldenQj;
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:      
dpt1x      ��һ�����X���ꣻ
dpt1y      ��һ�����y���ꣻ
dpt2x	   �ڶ������X���ꣻ
dpt2y	   �ڶ������y���ꣻ
dpt3x      ���������X���ꣻ
dpt3y      ���������y���ꣻ
�����������˵��:  ��
��������ֵ��˵��:  ����ֵΪ��ʱ������Ϊ˳ʱ�룬Ϊ��ʱΪ��ʱ��
����˵��:  ����ĸ�����������ú����Ϳ��������ж�һ������һ��ֱ�ߵ���һ�࣬
GoldenQj���øú�������ʹ���������������
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
��������: CheckIfOnArc  
�������ܡ����ܵȵ�����: ����(dNout,dEout)�Ƿ��ڸ�����Բ����  
�����������õĺ����嵥: CBasicFun::DisAndAngleByX
���ñ������ĺ����嵥:	CHhqx::GetJdArcHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dNcircle		    Բ����Բ��N����;
dEcircle			Բ����Բ��E����;
dAngleStart			Բ�����ĽǶ�;
dAngleEnd			Բ���յ�ĽǶ�;
dNout				���жϵ��N����;
dEout				���жϵ��E����;
�����������˵��:	��
��������ֵ��˵��: �õ���Բ�����򷵻�ture���򷵻�false;
����˵��:  
ע�⣺�ú����жϵ��Ƿ���Բ�������Ըõ���Բ��Բ�ĵľ���Ϊ�뾶�������½��еġ�
Բ������Ϊ�Ǵ���㿪ʼ����ʱ�뷽���յ㣻
Բ�������յ�ĽǶ�����Բ��Ϊ����ԭ�㽨����XoY����ϵ����㡢�յ���Բ�ĵ����ߵĽǶ�ֵ,��ʱ�뷽��Ϊ����
������Щ��������յ�Ķ�������CAD�еĶ���һ�µ�
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
	else//���Ƕȵ����յ�Ƕ�,��Բ����һ��Բ
		return true;	
}
