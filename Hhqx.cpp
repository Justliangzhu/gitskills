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

	m_StartN=0.0;			//���N����
	m_StartE=0.0;			//���E����
	m_StartFwj=0.0;			//��㷽λ��
	m_StartL=0.0;			//��������յ㳤��
	m_StartR=1e50;			//���뾶

	m_EndN=0.0;				//�յ�N����
	m_EndE=0.0;				//�յ�E����
	m_EndFwj=pi/4;			//�յ㷽λ��
	m_EndL=200;				//�յ������յ㳤��
	m_EndR=1000;			//�յ�뾶

	m_BaseN=0.0;			//�յ�N����
	m_BaseE=0.0;			//�յ�E����
	m_BaseFwj=0.0;			//�յ㷽λ��

	m_L0=200;				//�Բ��յ��𻺺������ܳ�
	m_LOrR=1;               //����ƫ��Ϣ ��ƫΪ-1����ƫΪ+1
	m_Mod=0;				//�����������ͱ��0-6��0--���������� 1--�벨���� 2--ȫ������  3--���������  4--�ߴ������� 5--�ߴ�����ʽ 6-- S�� 

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
��������: Set  
�������ܡ����ܵȵ�����: ���뻺�����ߵ���ز����������������ߣ�����㿪ʼ��    
�����������õĺ����嵥: CHhqx:: CalLToBasePt��CalX��CalY��CalQxj��GetPoint��DisAndAngleByX
CBasicFun:: DisAndAngleByX��FormFwj
���ñ������ĺ����嵥:   CYYLLink::Set
CYYCSLink::Set
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dStartN    �����������N���ꣻ
dStartE	   �����������E���ꣻ
dStartFwj  ����������㷽λ�ǣ�
dL0        �Բ��յ�Ļ�������ȫ����
dStartR	   ����������㴦�����ʰ뾶��
dEndR	   ���������յ㴦�����ʰ뾶��
iLOrR	   ����ת��Ϣ����-1����ת+1��
iMod       ������������ ȡֵΪ0-6������
�����������˵��: ��
��������ֵ��˵��: ��
����˵��: �ú���Ϊ����㹹����������     
***************************************************************/
void CHhqx::Set(double dStartN,double dStartE,double dStartFwj,double dL0,
				double dStartR,double dEndR,int iLOrR,int iMod)
{

	/*��ʵ�δ��������*/
	m_StartN   = dStartN;
	m_StartE   = dStartE;
	m_StartFwj = dStartFwj;
	m_L0=dL0;
	m_Mod=iMod;
	m_StartR = dStartR;
	m_EndR   = dEndR;
	m_LOrR = iLOrR;

	/*���������յ㴦������յ�����߳�*/
	m_StartL = CalLToBasePt(m_StartR);
	m_EndL   = CalLToBasePt(m_EndR);

	/*�������ľֲ�������������յ����ߵĳ��Ⱥ͸��ߵ����귽λ��*/
	double x=0.0,y=0.0,sita=0.0,dist0=0.0;
	x = CalX(m_StartL);
	y = CalY(m_StartL);
	dist0 = DisAndAngleByX(sita,0,0,x,y);//�˺�����dx����0 ʱ������0.5*PI,һ��Ҫע��

	/*������յ㷽λ�Ǽ�����*/
	m_BaseFwj = m_StartFwj-m_LOrR*CalQxj(m_StartL);
	m_BaseN   = m_StartN-dist0*cos(m_BaseFwj+m_LOrR*sita);
	m_BaseE   = m_StartE-dist0*sin(m_BaseFwj+m_LOrR*sita);

	/*�����յ����꼰��λ��*/
	GetPoint(m_EndN,m_EndE,m_EndL);//�����յ�����
	m_EndFwj=m_BaseFwj+m_LOrR*CalQxj(m_EndL);//�����յ����߷�λ��

	/*��ʽ�����귽λ��(������������ĽǶȻ�Ϊ0-2pi)*/
	FormFwj(m_BaseFwj); 
	FormFwj(m_StartFwj);
	FormFwj(m_EndFwj);
}



/*************************************************************
��������: SetByEnd  
�������ܡ����ܵȵ�����: ���뻺�����ߵ���ز����������������ߣ����յ㿪ʼ��    
�����������õĺ����嵥: CHhqx:: CalLToBasePt��CalX��CalY��CalQxj��GetPoint��DisAndAngleByX
CBasicFun:: DisAndAngleByX��FormFwj
���ñ������ĺ����嵥:   CYYLLInk::Set 
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dEndN    �����������N���ꣻ
dEndE	 �����������E���ꣻ
dEndFwj  ����������㷽λ�ǣ�
dL0      �Ӳ��յ㵽Բ�Ļ�������ȫ����
dEndR	 ���������յ㴦�����ʰ뾶��
dStartR	 ����������㴦�����ʰ뾶��
iLOrR	 ����ת��Ϣ����-1����ת+1��
iMod     ������������ ȡֵΪ0-6������
�����������˵��: ��
��������ֵ��˵��: ��
����˵��:   �ú���Ϊ���յ㹹����������    
***************************************************************/
void CHhqx::SetByEnd(double dEndN,double dEndE,double dEndFwj,double dL0,
					 double dEndR,double dStartR,int iLOrR,int iMod)
{
	/*��ʵ�δ��������*/
	m_EndN  = dEndN;
	m_EndE  = dEndE;
	m_EndFwj = dEndFwj;
	m_L0= dL0;
	m_EndR = dEndR;
	m_StartR  = dStartR;
	m_Mod = iMod;

	/*���������յ㴦������յ�����߳�*/
	m_StartL = CalLToBasePt(m_StartR);
	m_EndL   = CalLToBasePt(m_EndR);	
	m_LOrR = iLOrR;

	/*�����յ�ľֲ�������յ�����յ����ߵĳ��Ⱥ͸������귽λ��*/
	double x,y,sita,dist0;
	x=y=sita=dist0=0.0;
	x = CalX(m_EndL);
	y = CalY(m_EndL);
	dist0 = DisAndAngleByX(sita,0,0,x,y);//�˺�����dx����0 ʱ������0.5*PI,һ��Ҫע��

	/*������յ㷽λ�Ǽ�����*/
	m_BaseFwj = m_EndFwj-m_LOrR*CalQxj(m_EndL);
	m_BaseN   = m_EndN-dist0*cos(m_BaseFwj+m_LOrR*sita);
	m_BaseE   = m_EndE-dist0*sin(m_BaseFwj+m_LOrR*sita);

	/*����������꼰��λ��*/
	GetPoint(m_StartN,m_StartE,m_StartL);
	m_StartFwj=m_BaseFwj+m_LOrR*CalQxj(m_StartL);

	/*��ʽ�����귽λ��(������������ĽǶȻ�Ϊ0-2pi)*/
	FormFwj(m_BaseFwj); 
	FormFwj(m_StartFwj);
	FormFwj(m_EndFwj);
}

/*************************************************************
��������: CalQxj   
�������ܡ����ܵȵ�����: ���������յ�ĳ���ΪdLToBasePt�������߽�
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:  CHhqx::set,SetByEnd,
CYYLLink::Set,L0toCC,
CYYCSLink::Set��L0toCC
CZYLink::Set
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:  
dLToBasePt ���������յ�����߳���    
����˵��:      
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

		else			//�����Ҫ��������������������ֻ����else����Ӽ���
			sita=0.0;     
	}		
	return sita;
}

/*************************************************************
��������: GetFwj   
�������ܡ����ܵȵ�����: ���������յ�ĳ���ΪdLToBasePt���Ĵ�ط�λ��
�����������õĺ����嵥: CHhqx::CalQxj
���ñ������ĺ����嵥:	CHhqx::RtflspGetFwj
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:  
dLToBasePt ���������յ�����߳���    
����˵��:      
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
��������: RtflspGetFwj   
�������ܡ����ܵȵ�����: ��λ���������㻺�����������߷�λ��ΪdFwj��������յ�ĳ���dLToBasePt
�����������õĺ����嵥: CHhqx::GetFwj
���ñ������ĺ����嵥:   CHhqx::GetJdZxHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
x1		�������������ֵ��
x2		�������������ֵ��
xacc    ����ľ��ȣ�
dFwj    ��λ�ǣ�
�����������˵��: 
��������ֵ��˵��:  ���ػ������������߷�λ��ΪdFwj��������յ�ĳ���dLToBasePt
����˵��:      
***************************************************************/
double CHhqx::RtflspGetFwj(double x1,double x2,double xacc,double dFwj)const
{
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	j=0;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=GetFwj(x1)-dFwj;//��������ķ���ʽ
	fh=GetFwj(x2)-dFwj;//��������ķ���ʽ
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
		f=GetFwj(temp)-dFwj;//��������ķ���ʽ
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
��������: RtflspGetFwj   
�������ܡ����ܵȵ�����: ��λ���������㻺�����������߷�λ��ΪdFwj��������յ�ĳ���dLToBasePt
�����������õĺ����嵥: CHhqx::GetFwj
���ñ������ĺ����嵥:   CHhqx::GetJdZxHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
x1		�������������ֵ��
x2		�������������ֵ��
xacc    ����ľ��ȣ�
dFwj    ��λ�ǣ�
�����������˵��: 
��������ֵ��˵��:  ���ػ������������߷�λ��ΪdFwj��������յ�ĳ���dLToBasePt
����˵��:      
***************************************************************/
double CHhqx::RtflspCalQxj(double x1,double x2,double xacc,double dQxj)const
{
	int maxit,j;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	j=0;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=CalQxj(x1)-dQxj;//��������ķ���ʽ
	fh=CalQxj(x2)-dQxj;//��������ķ���ʽ
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
		f=CalQxj(temp)-dQxj;//��������ķ���ʽ
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
��������: FunXY   
�������ܡ����ܵȵ�����: �������յ���ΪdLToBasePt�������ǵ�����������ֵ,Ϊ��������ݾ���׼����
�����������õĺ����嵥: ��
���ñ������ĺ����嵥: Chebft 
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: dLToBasePt ����������յ�����߳���
ixory Ϊ1ʱ��ʾ��x���꣬Ϊ������ʱ��ʾ��y����;
�����������˵��: 
��������ֵ��˵��: ���ؾֲ�����ϵ�µ�x,y����      
����˵��: �ֲ�������յ�Ϊ����ԭ��     
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
	else		 //�����Ҫ��������������������ֻ����else����Ӽ���
		return 0.0;
}

/*************************************************************
��������: Chebft   
�������ܡ����ܵȵ�����: ���ڼ����б�ѩ������ϵ��												
�����������õĺ����嵥: FunXY
���ñ������ĺ����嵥:  Chebftjf
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   a   ������������ޣ�
b   ������������޲��ܳ������޶�ֵ��
xacc   ����ֵ�ľ��ȣ�
n    չ��������
xory  Ϊ1��ʾ��x���֣�Ϊ����ֵ��ʾ��y����;
�����������˵��: c[]  n��Ԫ�ص����飬����б�ѩ������ϵ�����������Ժ�Ҫ����chint������
��������ֵ��˵��:        
����˵��:		[a,b]Ϊ�ú��������䡣���Ǹú��������Ǽ���a��b�Ļ��֣�bֻ�ǻ������޵����ֵ      
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
		f[k]=FunXY(y*bma+bpa,ixory);//�����ֺ���

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
��������: Chint   
�������ܡ����ܵȵ�����: ���ڼ��㺯���������ֵ��б�ѩ��ƽ�������ϵ����
��ѡ����ֳ���ʹ����������a����ֵΪ0												
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:  Chebftjf
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
a			������������ޣ�
b			������������޲��ܳ������޶�ֵ��
c[]			n��Ԫ�ص����飬����б�ѩ������ϵ������������chebft��������ó���
n			չ����������
�����������˵��: cint1[]  n��Ԫ�ص������ź����������ֵ��б�ѩ��ƽ�������ϵ����
��������ֵ��˵��: ��      
����˵��:  [a,b]Ϊ�ú��������䡣���Ǹú��������Ǽ���a��b�Ļ��֣�bֻ�ǻ������޵����ֵ      
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
��������: Chebev   
�������ܡ����ܵȵ�����: ���б�ѩ�������㺯���ڸ�����x����ֵ												
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:  Chebftjf
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   a   ������������ޣ�
b   ������������޲��ܳ������޶�ֵ��
c[]  m��Ԫ�ص����飬����б�ѩ������ϵ����������ֵ��chebft�е�c[]���룻
m    �����б�ѩ������������
x    ������ֵĻ������ޣ�
�����������˵��: 
��������ֵ��˵��: ���غ�����[a,x]�ϵĻ���      
����˵��:[a,b]Ϊ�ú��������䡣���Ǹú��������Ǽ���a��b�Ļ��֣�bֻ�ǻ������޵����ֵ      
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
��������: Chebftjf   
�������ܡ����ܵȵ�����: ���ڼ��㺯����[a,x]�����ϵĻ���												
�����������õĺ����嵥: CHhqx::Chebft��Chint,Chebev;
���ñ������ĺ����嵥:   CHhqx::CalX,CalY;
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
a		������������ޣ�
b		������������޲��ܳ������޶�ֵ��
x		������ֵĻ������ޣ�
ixory	Ϊ1��ʾ��x���֣�Ϊ2��ʾ��y����;
�����������˵��:  
��������ֵ��˵��:  ������[a,x]�����ϵĻ���ֵ      
����˵��:[a,b]Ϊ�ú��������䡣���Ǹú��������Ǽ���a��b�Ļ��֣�����[a,x]���֣�bֻ�ǻ�������x�����ֵ      
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
��������: CalQulv   
�������ܡ����ܵȵ�����: �������յ���ΪdLToBasePt�������
�����������õĺ����嵥: ��
���ñ������ĺ����嵥:   RtflspLToBasePt
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: 
dLToBasePt    ����������յ�����߳�
�����������˵��: 
��������ֵ��˵��: ���ؾ���յ���ΪdLToBasePt�������
����˵��:      
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

	else			//�����Ҫ��������������������ֻ����else����Ӽ���
		return 0.0;
}


/*************************************************************
��������: RtflspLToBasePt   
�������ܡ����ܵȵ�����: ��λ���������㻺�����������ʰ뾶ΪdR��������յ�ĳ���dLToBasePt						
�����������õĺ����嵥: CHhqx::CalQulv
���ñ������ĺ����嵥:   CHhqx::CalLToBasePt 
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
x1		�������������ֵ��
x2		�������������ֵ��
xacc    ����ľ��ȣ�
dRl      ���ʰ뾶��
�����������˵��: 
��������ֵ��˵��:  ���ػ������������ʰ뾶ΪdR��������յ�ĳ���dLToBasePt      
����˵��:      
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
	fl=CalQulv(x1)-1/dRl;//��������ķ���ʽ
	fh=CalQulv(x2)-1/dRl;//��������ķ���ʽ
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
		f=CalQulv(temp)-1/dRl;//��������ķ���ʽ
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
��������: CalLToBasePt   
�������ܡ����ܵȵ�����: �������ʰ뾶ΪdRl���ĵ������յ�����߳���dLToBasePt
�����������õĺ����嵥: CHhqx::RtflspLToBasePt
���ñ������ĺ����嵥:   CHhqx::set,SetByEnd
CYYLLink::Set,L0toCC 
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: 
dRl  ���ʰ뾶;
�����������˵��: 
��������ֵ��˵��: ���ʰ뾶ΪdRl���ĵ������յ�����߳���dLToBasePt       
����˵��: 
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
		else        //�����Ҫ��������������������ֻ����else����Ӽ���
			l=0.0;
		return l;
	}

}

/*************************************************************
��������: CalX   
�������ܡ����ܵȵ�����: �������յ�ĳ���dLToBasePt��ĺ��
�����������õĺ����嵥: CHhqx::Chebftjf
���ñ������ĺ����嵥:   CHhqx::set,SetByEnd, GetPoint
CYYLLink::Set,L0toCC
CYYCSLink::Set,L0toCC
CZYLink::Set,L0ToDisOfCenAndLine
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��: 
dLToBasePt ����յ�ĳ���
�����������˵��:  
��������ֵ��˵��:  ����յ�ĳ���dLToBasePt��ĺ��    
����˵��:    
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
��������: CalY   
�������ܡ����ܵȵ�����: �������յ�ĳ���dLToBasePt����ݾ�
�����������õĺ����嵥: CHhqx::Chebftjf
���ñ������ĺ����嵥:   CHhqx::set,SetByEnd, GetPoint
CYYLLink::Set,L0toCC
CYYCSLink::Set,L0toCC
CZYLink::Set,L0ToDisOfCenAndLine
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:
dLToBasePt ����յ�ĳ���
�����������˵��:  
��������ֵ��˵��:  ����յ�ĳ���dLToBasePt����ݾ�    
����˵��: �ֲ�������յ�Ϊ����ԭ��     
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
��������: GetPoint  
�������ܡ����ܵȵ�����: �������յ�ĳ���dLToBasePt�����ظõ�Ĵ������(N,E)��    
�����������õĺ����嵥: CHhqx::CalX,CalY,
CBasicFun::DisAndAngleByX
���ñ������ĺ����嵥:	CHhqx::Set,SetByEnd,GetJd1ZxHhqx,GetJdZxHhqx,GetJdArcHhqx,GetJdHhqxHhqx
CYYCSLInk::Set
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dLToBasePt     ����յ�ĳ���dLToBasePt��
�����������˵��: 
Np  ����յ�ĳ���dLToBasePt����N���꣬
Ep  ���E���ꣻ
��������ֵ��˵��:dLToBasePtС��0��������������false,���򷵻�ture��
����˵��:      
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
��������: Getafa  
�������ܡ����ܵȵ�����: ���㻺�������Ͼ�����յ����߳�ΪdLtoBasePt�ĵ�(dNp,dEp)�뻺��������һ��(dNout,dEout)���ɵ�ֱ����
(dNp,dEp)����������γɵļн�(0-pi֮��)
�����������õĺ����嵥: CBasicFun::ZXAngle
CHhqx::GetPoint,CalQxj;
CBasicFun::DisAndAngleByX,ZxAngle
���ñ������ĺ����嵥:	CHhqx::GetPointTy,GoldenGetAfa,RtflspLToBasePtGetAfa
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dNout	   �������N����;
dEout	   �������E����;
dLtoBasePt ���������ϵĵ������յ�����߳���
�����������˵��:	
dNp	   ���������ϵ��N���ꣻ
dEp	   ���������ϵ��E���ꣻ
��������ֵ��˵��: ������ֱ�ߵļнǣ�0-2pi֮�䣩
����˵��:  
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
��������: RtflspGetafa   
�������ܡ����ܵȵ�����: �ú���Ϊ��λ�����Getafa-pi/2=0ʱ��dLtoBasePt				
�����������õĺ����嵥: Getafa
���ñ������ĺ����嵥:   PointTy
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
x1      �������������ֵ��
x2      �������������ֵ��
xacc    ����ľ��ȣ�
dNout   ��������N����;
dEout   ��������E����;
�����������˵��:
dNp	   Getafa-pi/2=0ʱ��dLtoBasePt�����N���ꣻ
dEp	   Getafa-pi/2=0ʱ��dLtoBasePt�����E���ꣻ	 
��������ֵ��˵��:   Getafa-pi/2=0ʱ��dLtoBasePtֵ��     
����˵��:      
***************************************************************/
double CHhqx::RtflspGetafa(double &dNp,double &dEp,double x1,double x2,double xacc,double dNout,double dEout)
{
	int maxit,j;
	maxit=j=0;
	double fl,fh,xl,xh,swap,dx,temp,f,del;
	fl=fh=xl=xh=swap=dx=temp=f=del=0.0;
	maxit=100;
	fl=Getafa(dNp,dEp,x1,dNout,dEout)-pi/2;//��������ķ���ʽ
	fh=Getafa(dNp,dEp,x2,dNout,dEout)-pi/2;//��������ķ���ʽ
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
		f=Getafa(dNp,dEp,temp,dNout,dEout)-pi/2;//��������ķ���ʽ
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
��������: GoldenGetAfa  
�������ܡ����ܵȵ�����: �ƽ�ָ�����Ѱ��-Getafa��������Сֵ��(��Getafa�����ֵ)������յ�ĳ���xmin;
�����������õĺ����嵥: CHhqx::Getafa:	
���ñ������ĺ����嵥:	CHhqx::PointTy��  
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
ax         �������������ֵ��
bx		   ���������ڵ�һ��ֵ��Խ�ӽ�xminԽ�ã������ʼ�޷�����xmin��ɸ�������м�ֵ(ax+bx)/2;		   
cx		   �������������ֵ��
dNout	   �������N����;
dEout	   �������E����;
tol		   �������ȣ�
�����������˵��:	xmin   -Getafa��[ax,bx]�����ϼ�Сֵ��Ӧ��x���ꣻ   
��������ֵ��˵��:	����-Getafa�ļ���ֵ��
����˵��:  �ú�����GetPrjPt�е�������ͶӰ��ʱ�����жϵ�����������á�
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
	f1 = -Getafa(Np,Ep,x1,dNout,dEout);//��Сֵ��Ŀ�꺯��
	f2 = -Getafa(Np,Ep,x2,dNout,dEout);//��Сֵ��Ŀ�꺯��
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = -Getafa(Np,Ep,x2,dNout,dEout);//��Сֵ��Ŀ�꺯��
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = -Getafa(Np,Ep,x1,dNout,dEout);//��Сֵ��Ŀ�꺯��
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
��������: DisPtToHhqx
�������ܡ����ܵȵ�����: ���㻺�����������Ͼ�����յ����߳�ΪdLtoBasePt�ĵ�����(dNout,dEout)����  
�����������õĺ����嵥: CHhqx::GetPoint
���ñ������ĺ����嵥:	CHhqx::GoldenDisPtToHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:
dLtoBasePt			���������ϵ���������յ�����߳�
dNout				��������N����;
dEout				��������E����;
�����������˵��:	��
��������ֵ��˵��:   �������������Ͼ�����յ����߳�ΪdLtoBasePt�ĵ�����(dNout,dEout)����;
����˵��:  ��
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
��������: GoldenDisPtToHhqx
�������ܡ����ܵȵ�����: �ƽ�ָ�����CHhqx::GoldenDisPtToHhqx�����ļ�Сֵ 
�����������õĺ����嵥: CHhqx::DisPtToHhqx
���ñ������ĺ����嵥:	CHhqx::MinDisPtToHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:
ax         �������������ֵ��
bx		   ���������ڵ�һ��ֵ��Խ�ӽ�xminԽ�ã������ʼ�޷�����xmin��ɸ�������м�ֵ(ax+bx)/2;		   
cx		   �������������ֵ��
dNout	   �������N����;
dEout	   �������E����;
tol		   �������ȣ�
�����������˵��:	xmin   -Getafa��[ax,bx]�����ϼ�Сֵ��Ӧ��x���ꣻ
��������ֵ��˵��:   CHhqx::GoldenDisPtToHhqx�����ļ�Сֵ
����˵��:  ��
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
	f1 = DisPtToHhqx(x1,dNout,dEout);//��Сֵ��Ŀ�꺯��
	f2 = DisPtToHhqx(x2,dNout,dEout);//��Сֵ��Ŀ�꺯��
	while (fabs(x3 - x0) > tol * (fabs(x1) + fabs(x2)))
	{
		if (f2 < f1)
		{
			x0 = x1;
			x1 = x2;
			x2 = r * x1 + c * x3;
			f0 = f1;
			f1 = f2;
			f2 = DisPtToHhqx(x2,dNout,dEout);//��Сֵ��Ŀ�꺯��
		}
		else
		{
			x3 = x2;
			x2 = x1;
			x1 = r * x2 + c * x0;
			f3 = f2;
			f2 = f1;
			f1 = DisPtToHhqx(x1,dNout,dEout);//��Сֵ��Ŀ�꺯��
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
��������: MinDisPtToHhqx
�������ܡ����ܵȵ�����: �����(dNout,dEout)���������ߵ���̾���  
�����������õĺ����嵥: CBasicFun::DisAndAngleByX
���ñ������ĺ����嵥:	CHhqx::GetJdArcHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dNout				������������N����;
dEout				������������E����;
�����������˵��:	��
��������ֵ��˵��:   �����(dNout,dEout)���������ߵ���̾���
����˵��:  ��
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
��������: GetPointTy  
�������ܡ����ܵȵ�����: ���㻺��������һ�㣨Nout,Eout���ڻ��������ϵ�ͶӰ�������ͶӰ�㵽���յ�����߳���    
�����������õĺ����嵥: CHhqx::Getafa,RtflspGetafa,GoldenGetAfa
���ñ������ĺ����嵥:	  
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dNout	   �������N����;
dEout	   �������E����;
�����������˵��:	
dNp1	   ���������ϵ�һ��ͶӰ���N���ꣻ
dEp1	   ���������ϵ�һ��ͶӰ���E���ꣻ
dLtoBasePt1  ��һ��ͶӰ�������յ�ĳ��ȣ�
dNp2		���������ϵڶ���ͶӰ���N���ꣻ
dEp2		���������ϵڶ���ͶӰ���E���ꣻ
dLtoBasePt2  �ڶ���ͶӰ�������յ�����߳���
iNjd	    ����������ͶӰ��ĸ�����
��������ֵ��˵��: ���ҵ��ô��㷵��ture���򷵻�false;
����˵��:       
***************************************************************/
bool CHhqx::GetPointTy(double &dNp1,double &dEp1,double &dLtoBasePt1,double &dNp2,double &dEp2,double &dLtoBasePt2,int &iNjd,double dNout,double dEout)
{
	double afa1=0.0,afa2=0.0;
	double dLToBasePt;
	dLToBasePt=0.0;
	iNjd=0;
	if (MinDisPtToHhqx(dLToBasePt,dNout,dEout)<1e-5)
	{
		AfxMessageBox(L"ָ�����ڻ���������");
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
��������: GetJdZxHhqx  
�������ܡ����ܵȵ�����: ��ֱ�߶��뻺�����ߵĽ�������ͽ��������յ�ĳ��ȡ�    
�����������õĺ����嵥: CBasicFun::DistanceFromPttoLine
CHhqx::GetPoint;
���ñ������ĺ����嵥:	
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
dNstar	   ֱ�߶����N����;
dEstar	   ֱ�߶����E����;
dNend	   ֱ�߶��յ�N����;
dEend	   ֱ�߶��յ�E����;
�����������˵��:	   
dNp1	   ����������ֱ�߶ε�һ�������N���ꣻ
dEp1 	   ����������ֱ�߶ε�һ�������E���ꣻ
dNp2	   ����������ֱ�߶εڶ��������N���ꣻ
dEp2 	   ����������ֱ�߶εڶ��������E���ꣻ
iNjd	   ����������ֱ�߶ν���ĸ�����
��������ֵ��˵��: ���ҵ����㷵��ture���򷵻�false;
����˵��:   ���������е�ֱ�߶��뻺�����߶����������㡣��ֻ��һ������ʱNp2,Ep2��������ֵ
������ϵͳ��������ĳ�ʼֵ��  
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
	DisAndAngleByX(Fwj,dNstar,dEstar,dNend,dEend);//����ֱ�ߵķ�λ��
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
	/*�жϾ���ֱ�ߵľ���*/
	dis=DistanceFromPttoLine(N,E,dNstar,dEstar,dNend,dEend);
	if(fabs(dis)<1e-5)//����С��1e-5��Ϊ����������ֱ������
	{
		AfxMessageBox(L"ֱ���뻺����������");
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
��������: GetJdArcHhqx  
�������ܡ����ܵȵ�����: �󻺺�������Բ���Ľ���;  
�����������õĺ����嵥: CHhqx::GetPoint,CheckIfOnArc;
���ñ������ĺ����嵥:	
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
No		    Բ����Բ��N����;
Eo			Բ����Բ��E����;
Anglestart	Բ�����ĽǶ�;
Angleend	Բ���յ�ĽǶ�;
r			Բ���뾶
mod        ������������ ȡֵΪ0-6������;
�����������˵��:	
dN1	   ����������ֱ�߶ε�һ�������N���ꣻ
E1     ����������ֱ�߶ε�һ�������E���ꣻ
N2	   ����������ֱ�߶εڶ��������N���ꣻ
E2     ����������ֱ�߶εڶ��������E���ꣻ
Njd	   ����������ֱ�߶ν���ĸ�����
��������ֵ��˵��: ���ҵ����㷵��ture���򷵻�false;
����˵��:   ���������е�ֱ�߶��뻺�����߶����������㡣��ֻ��һ������ʱN2,E2��������ֵ
������ϵͳ��������ĳ�ʼֵ��
Բ������Ϊ�Ǵ���㿪ʼ����ʱ�뷽���յ㣻
Բ�������յ�ĽǶ�����Բ��Ϊ����ԭ�㽨����XoY����ϵ��Բ������㡢�յ�����ߵĽǶ�ֵ
�Ƕ�����ʱ�뷽��Ϊ����
������Щ��������յ�Ķ�������CAD�еĶ���һ�µ�
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
��������: LtoDeta  
�������ܡ����ܵȵ�����: �Ե�һ�������������Ϊԭ�㣬��һ���������ߵ�������߷���ΪX�Ὠ��XOY����ϵ
����ڶ����������ߺ͵�һ��������������ͬ��X�����µ�Y�����ֵ
�����������õĺ����嵥: CHhqx::GetPoint��GetJd1ZxHhqx
CBasicFun::DistanceFromPttoLine
���ñ������ĺ����嵥:	CBasicFun::GoldenLtoDeta,RtflspLtoDeta,GetJdHhqxHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
Hhqx1		    ��һ���������߶���;
Hhqx2			�ڶ����������߶���;
dLtoBasePt      �ڶ������������ϵ�����㵽���յ�ĳ���;
��������ֵ��˵��: ����ڶ����������ߺ͵�һ��������������ͬ��X�����µ�Y�����ֵ
����˵��:   ��
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
��������: GoldenLtoDeta  
�������ܡ����ܵȵ�����: �ƽ�ָ��������LtoDeta�����ļ�Сֵ������ֵ���ͼ�Сֵ������ֵ���㴦�ڵڶ������������Ͼ�����յ�ĳ���;
�����������õĺ����嵥: CHhqx::LtoDeta;
���ñ������ĺ����嵥:	CHhqx::GetJdHhqxHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
ax		    ��Сֵ����������;
bx			��Сֵ�������е㴦��ֵ;����ֵֻҪ���������ھͿ����ˣ�һ����Ը��������е������ȽϿ죩
cx			��Сֵ����������;
tol			����;
Hhqx1		��һ����������;
Hhqx2		�ڶ�����������;
imaxormin	Ϊ1ʱ���㼫Сֵ��Ϊ-1ʱ���㼫��ֵ
�������˵��:
xminormax        �����ﵽ��Сֵ�򼫴�ֵʱ��Ӧ�ĵڶ������������ϵ�����㵽���յ�ĳ���;
��������ֵ��˵��:  ����LtoDeta�����ļ�Сֵ
����˵��:   
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
��������: GoldenQj  
�������ܡ����ܵȵ�����: �Ե�һ�������������Ϊԭ�㣬��һ���������ߵ�������߷���ΪX�Ὠ��XOY����ϵ;
����ڸ�����ϵ������������X��ȡֵ��Χ�Ľ���
�����������õĺ����嵥: CHhqx::GetJd1ZxHhqx;
CBasicFun::ChuizuToZx,CCW2;
���ñ������ĺ����嵥:	CHhqx::GetJdHhqxHhqx
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
Hhqx1		    ��һ���������߶���;
Hhqx2			�ڶ����������߶���;
�������˵��:
al				��������������;
cl				�������������ޣ�
��������ֵ��˵��:  ����Ϊ��ʱ����false ���򷵻�true;
����˵��:   
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
��������: GetJd1ZxHhqx  
�������ܡ����ܵȵ�����: ��ֱ�߶��뻺�����ߵ�һ���������ꡣ    
�����������õĺ����嵥: CBasic::DistanceFromPttoLine
CHhqx::GetPoint
���ñ������ĺ����嵥:	CHhqx::GoldenQj,LtoDeta;
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
Nstar	   ֱ�߶����N����;
Estar	   ֱ�߶����E����;
Nend	   ֱ�߶��յ�N����;
Eend	   ֱ�߶��յ�E����;
�����������˵��:	   
Np1		   ����������ֱ�߶�һ�������N���ꣻ
Ep1 	   ����������ֱ�߶�һ�������E���ꣻ
LtoBasePt1 ����������ֱ�߶ν��������յ�ĳ��ȣ�
��������ֵ��˵��: ���ҵ����㷵��ture���򷵻�false;
����˵��:   �ú�������������������󽻵����Ҫ��GetJdZxHhqx�ĸĽ�����
��Ϊ���������������߽����е���ֱ���뻺��������ʱֻ���ܳ���һ������
������ÿ������������GetJdZxHhqx���������ٶ��ر���������û�б�Ҫ
���������øú����ٶȿ�������������ϣ�
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
	DisAndAngleByX(Fwj,dNstart,dEstart,dNend,dEend);//����ֱ�ߵķ�λ��
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
		/*�жϾ���ֱ�ߵľ���*/
		dis=DistanceFromPttoLine(N,E,dNstart,dEstart,dNend,dEend);
		if(fabs(dis)<1e-5)//����С��1e-5��Ϊ����������ֱ������
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
��������: RtflspLtoDeta   
�������ܡ����ܵȵ�����: �ú���Ϊ��λ�����LtoDeta����ֵΪ0ʱ��dLtoBasePt					
�����������õĺ����嵥: CHhqx::LtoDeta
���ñ������ĺ����嵥:   CHhqx::GetJdHhqxHhqx 
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
x1			����������������ֵ��
x2			����������������ֵ��
xacc		��������ľ��ȣ�
Hhqx1	    ��һ���������߶���
Hhqx2	    �ڶ����������߶���
�����������˵��: 
��������ֵ��˵��:   ����LtoDeta����ֵΪ0ʱ��dLtoBasePt     
����˵��:      
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
��������: GetJdHhqxHhqx  
�������ܡ����ܵȵ�����: �����������ߵĽ��㣻
�����������õĺ����嵥: CHhqx::GoldenQj,LtoDeta,RtflspLtoDeta,GetPoint,GoldenLtoDeta;
���ñ������ĺ����嵥:	
�����ʵı����������ǣ�������ݿ�����ĳ���: ��
���޸ĵı����������ǣ�������ݿ�����ĳ���: ��
�������˵��:   
Hhqx1	   ��һ���������߶���
Hhqx2	   �ڶ����������߶���
�����������˵��:	   
dNp1	   ���������ߵ�һ�������N���ꣻ
dEp1 	   ���������ߵ�һ�������E���ꣻ
Np2		   ���������ߵڶ��������N���ꣻ
Ep2 	   ���������ߵڶ��������E���ꣻ
iNjd	   ���������
dLtoBasePt1 ���������ߵ�һ�����㣨�ڶ������������ϣ�����ڶ����������߲��յ�ĳ��ȣ�
dLtoBasePt2 ���������ߵڶ������㣨�ڶ������������ϣ�����ڶ����������߲��յ�ĳ��ȣ�
��������ֵ��˵��: ���ҵ����㷵��ture���򷵻�false;
����˵��:   
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
	//	/*�ֱ���Hhqx1��Hhqx2��������߷���ΪX���������������[ax,cx]*/
	//	bool GoldenQj1,GoldenQj2;
	//	GoldenQj1=GoldenQj(ax,cx,Hhqx1,Hhqx2);
	//	if (!GoldenQj1)//�����Hhqx1��������߷���ΪX��û�����������ĵ�������[ax,cx]����Hhqx2��������߷���ΪX��
	//	{
	//		GoldenQj2=GoldenQj(ax,cx,Hhqx2,Hhqx1);
	//		if (!GoldenQj2)//���Hhqx1��Hhqx2��������߷���ΪX�ᶼ�Ҳ�������������[ax,cx]����������ڵ�������[ax,cx]
	//			return false;
	//		else//��Hhqx1��������߷���ΪX�����ҵ�[ax,cx]����Hhqx1��Hhqx2
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
