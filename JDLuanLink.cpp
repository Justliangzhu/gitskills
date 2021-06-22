// JDLuanLink.cpp: implementation of the JDLuanLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "JDLuanLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JDLuanLink::JDLuanLink()
{
	OKPreJD=false;
	OKCurJD=false;
	OKAftJD=false;
	OKPreT=false;
	ERRFlag=true;
}

JDLuanLink::~JDLuanLink()
{

}

//����ǰһ����������꼰�뾶
void JDLuanLink::SetPreJD(double n,double e,double r)
{
	OKPreJD=true;

	PreR=r;
	PreN=n;
	PreE=e;
}

//���õ�ǰ��������ꡢ�뾶��ǰ�󻺺����ߵ�Aֵ
void JDLuanLink::SetCurJD(double n,double e,double r,double a1,double a2)
{
	OKCurJD=true;

	CurR=r;
	CurN=n;
	CurE=e;

	PreA=a1;
	AftA=a2;
}

//���ú�һ�����������
void JDLuanLink::SetAftJD(double n,double e)
{
	OKAftJD=true;

	AftN=n;
	AftE=e;
}

//����ǰһ������ĺ��г�
void JDLuanLink::SetPreT(double t)
{
	OKPreT=true;
	PreT=t;
}

//��ʼ���㻺������
void JDLuanLink::UpdateData()
{
	if(OKPreJD && OKCurJD && OKAftJD && OKPreT)
	{
		//	xyddaa(CurN,CurE,PreN,PreE,&PreFwj);
		//	xyddaa(CurN,CurE,AftN,AftE,&AftFwj);
		HYN=CurN+PreT*cos(PreFwj);
		HYE=CurE+PreT*sin(PreFwj);
		double aa=AftFwj-PreFwj+PI;
		if (aa<-pi) aa=aa+2*pi;
		if  (aa>pi) aa=aa-2*pi;
		if (aa>0)LeftOrRight=1;//��ƫ
		else LeftOrRight=-1;//��ƫ

		ads_printf(L"prea=%lf afta=%lf\n",PreA,AftA);		
		if(AftA<0.1)
		{
			AftA=0.0;
			count=0;

			double a;

			PreA=DieDai(min(PreR,CurR)/3.0,2*min(PreR,CurR));

			//PreA=DieDai(0.0,2.0*min(PreR,CurR));
			//			acutPrintf(L"count=%d\n",count);
			if(PreA<0.0)
			{
				ads_printf(L"��������ǰA��������!");
				ERRFlag=true; return;
			}
		}
		else
		{

			AftA=func(PreA);


			if(AftA<0.0){ads_printf(L"�������ߺ�A��������!");ERRFlag=true;return;}
		}
		double tN,tE;
		GetCenter(PreN,PreE,CurN,CurE,AftN,AftE,PreR,CurR,PreA,AftA,tN,tE,ArcLength,AftT);
		//ads_printf(L"PreN=%lf,PreE=%lf,CurN=%lf,CurE=%lf,AftN=%lf,AftE=%lf,PreR=%lf,CurR=%lf,PreA=%lf,AftA=%lf,tN=%lf,tE=%lf,ArcLength=%lf,AftT=%lf\n",
		//		   PreN,PreE,CurN,CurE,AftN,AftE,PreR,CurR,PreA,AftA,tN,tE,ArcLength,AftT);
		ERRFlag=false;
	}
	else ERRFlag=true;
}

double JDLuanLink::func(double a10)
{

	if(PreR>CurR)
	{
		SPIRAL pSpir;

		pSpir.Set(HYN,HYE,PreFwj+PI,a10,PreR,CurR,LeftOrRight);

		CurCenN=pSpir.EndN+CurR*cos(pSpir.EndFwj+LeftOrRight*0.5*PI);
		CurCenE=pSpir.EndE+CurR*sin(pSpir.EndFwj+LeftOrRight*0.5*PI);

	}
	else
	{
		SPIRAL pSpir;
		pSpir.Set2(HYN,HYE,PreFwj,a10,CurR,PreR,-1*LeftOrRight);
		//		pSpir.DRAW();
		CurCenN=pSpir.StartN+CurR*cos(pSpir.StartFwj-LeftOrRight*0.5*PI);
		CurCenE=pSpir.StartE+CurR*sin(pSpir.StartFwj-LeftOrRight*0.5*PI);
		//		AcDbCircle *cir=new AcDbCircle(AcGePoint3d(CurCenE,CurCenN,0.0),AcGeVector3d(0,0,1.0),CurR);
		//		AddEntityToDbs(cir);
		//		
		//		AcDbLine *Line=new AcDbLine(AcGePoint3d(pSpir.StartE,pSpir.StartN,0.0),AcGePoint3d(pSpir.StartE+10,pSpir.StartN,0.0));
		//		AddEntityToDbs(Line);
		//		AcDbCircle *cir1=new AcDbCircle(AcGePoint3d(pSpir.StartE,pSpir.StartN,0.0),AcGeVector3d(0,0,1.0),5);
		//		AddEntityToDbs(cir1);
	}
	//���A

	ZYLink ZYObj;
	//	ZYObj.Set(CurN,CurE,AftN,AftE,CurCenN,CurCenE,CurR,-1);
	if(AftA<400)
		ads_printf(L"#### %lf %lf %lf %lf %lf %lf\n",HYN,HYE,CurCenN,CurCenE,CurN,CurE);
	ZYObj.Set(CurN,CurE,AftN,AftE,CurCenN,CurCenE,CurR,LeftOrRight);
	if(!ZYObj.ERRFlag)
	{
		double tmpA=ZYObj.pSPIRAL.A;
		return tmpA;
	}
	else return -1.0;
}

//����ǰ������֤�󻺺�ǰ��
double JDLuanLink::DieDai(double a11,double a12)
{
	int num=0;

	for(;;)
	{
		num++;
		if(fabs(a11-a12)<PRECISION)return a11;
		double a10=0.5*(a11+a12);
		////��Բ��//////////////////////////////////////////////////////////////////////

		double tmpA=func(a10);
		//   ads_printf(L"tpa=%lf afta=%lf a=%lf a1=%lf a2=%lf \n",tmpA,AftA,a10,a11,a12);

		if(fabs(tmpA-AftA)<PRECISION)return a10;
		else
		{
			double tmpA1=func(a11);
			double tmpA2=func(a12);
			//   if(tmpA1<tmpA2)//��������
			//  {
			if(tmpA<AftA)
				a11 = a10;
			else 
				a12 = a10;
			//   }
			//   else
			//		   {
			//			   if(tmpA>AftA)
			//				   a11=a10;
			//			   else 
			//				   a12 = a10;
			//		   }
		}
		if(num >1000)
			return 0;
	}
	return 0;
}
//���������㣬��һ�������㹹�����ͣ�ǰһ���뾶����һ���뾶��ǰA����A�����غ�һ��Բ��Բ��,Բ���߳�
void JDLuanLink::GetCenter(double JdN1, double JdE1, double JdN0, double JdE0,
						   double JdN2, double JdE2, double r1, double r2, double a1,
						   double a2, double &cenN, double &cenE,double &l,double &T2)
{
	double d1,d2;//ǰ��ƫ��
	double afa1,afa2;//ǰ�󻺺�����ƫ��
	d1=YYLuanLink::DistThisCenToOtherGQX(r1,r2,a1,afa1);
	if(a2<=0.00001)
	{
		d2=r2;
		afa2=0.0;
	}
	else d2=ZYLink::DistCenToQX(r2,a2,afa2);

	//����ת
	int zy;
	double aa;//ת��
	double fb,fe;//ǰ�������ߵķ�λ��
	xyddaa(JdN1,JdE1,JdN0,JdE0,&fb);
	xyddaa(JdN0,JdE0,JdN2,JdE2,&fe);
	aa=fe-fb;
	//	acutPrintf(L"d1=%lf,d2=%lf\n",d1,d2);
	if (aa<-pi) aa=aa+2*pi;
	if  (aa>pi) aa=aa-2*pi;
	if (aa>0)zy=1;//��ƫ
	else zy=-1;//��ƫ

	double x1,y1,x2,y2,x3,y3,x4,y4;

	x1=JdN1+d1*cos(fb+zy*0.5*PI);
	y1=JdE1+d1*sin(fb+zy*0.5*PI);

	x2=JdN0+d1*cos(fb+zy*0.5*PI);
	y2=JdE0+d1*sin(fb+zy*0.5*PI);

	x3=JdN0+d2*cos(fe+zy*0.5*PI);
	y3=JdE0+d2*sin(fe+zy*0.5*PI);

	x4=JdN2+d2*cos(fe+zy*0.5*PI);
	y4=JdE2+d2*sin(fe+zy*0.5*PI);

	double afa;//Բ����ת��
	afa=fabs(aa)-fabs(afa1)-fabs(afa2);
	//	ads_printf(L"afa1=%lf afa2=%lf\n",afa1,afa2);
	l=r2*afa;
	BAS_DRAW_FUN::INTERS(&cenN,&cenE,x1,y1,x2,y2,x3,y3,x4,y4);

	//����г�
	double GQN2,GQE2;//���е�����
	GQN2=cenN+d2*cos(fe-zy*0.5*PI);
	GQE2=cenE+d2*sin(fe-zy*0.5*PI);
	double f,Xm,x,dX;
	x=spiral_x(a2,a2*a2/r2);
	dX=r2*sin(afa2);
	Xm=x-dX;
	T2=xyddaa(JdN0,JdE0,GQN2,GQE2,&f)+Xm;
}

void JDLuanLink::SetPreFwj(double pfwj)
{
	PreFwj = pfwj;
}

void JDLuanLink::SetAftFwj(double pfwj)
{
	AftFwj = pfwj;
}

