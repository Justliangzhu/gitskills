// YYCLink.cpp: implementation of the YYCLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "YYCLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "JD_CENTER.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
YYCLink::YYCLink()
{
	SetSLink(false);	
}

void YYCLink::Set(AcDbArc *pArc1,AcDbArc *pArc2,double a1,double a2,int lr)
{
	double n1,e1,r1,n2,e2,r2;

	n1=pArc1->center().y;
	e1=pArc1->center().x;
	r1=pArc1->radius();

	n2=pArc2->center().y;
	e2=pArc2->center().x;
	r2=pArc2->radius();

	//	Set(n1,e1,r1,n2,e2,r2,a1,a2,lr);
	//////////////////////////////////
	//	if(ERRFlag)return;

	JdCenter * jdcenay=new JdCenter[4];
	int i=0;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].LeftOrRight=lr;
	jdcenay[i].IsOrNotHuiTou=0;
	i=1;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=n1;
	jdcenay[i].E=e1;
	jdcenay[i].R=r1;
	jdcenay[i].A1=0.0;
	jdcenay[i].A2=a1;
	if(IsSModel>0.0)jdcenay[i].LinkAfter=C_Link;
	else jdcenay[i].LinkAfter=S_Link;
	jdcenay[i].LeftOrRight=lr;
	i=2;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=n2;
	jdcenay[i].E=e2;
	jdcenay[i].R=r2;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].A1=a2;
	jdcenay[i].A2=0.0;
	jdcenay[i].LeftOrRight=IsSModel*lr;
	i=3;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].IsOrNotHuiTou=0;
	/////////////////////////////////////
	double EndJiao1,EndJiao2;
	if(lr<0.0)
	{
		EndJiao1=pArc1->startAngle();
		if(IsSModel>0.0)//C
			EndJiao2=pArc2->endAngle();	
		else
			EndJiao2=pArc2->startAngle();
	}
	else
	{
		EndJiao1=pArc1->endAngle();
		if(IsSModel>0.0)
			EndJiao2=pArc2->startAngle();
		else
			EndJiao2=pArc2->endAngle();
	}
	EndJiao1=2.5*PI-EndJiao1;
	EndJiao2=2.5*PI-EndJiao2;

	jdcenay[0].N=n1+r1*cos(EndJiao1);
	jdcenay[0].E=e1+r1*sin(EndJiao1);

	jdcenay[3].N=n2+r2*cos(EndJiao2);
	jdcenay[3].E=e2+r2*sin(EndJiao2);
	/////////////////////////////////////
	JD_CENTER *ob=new JD_CENTER;
	ob->SetJdCenterArray(jdcenay,4);
	delete []jdcenay;	
	BAS_DRAW_FUN::AddEntityToDbs(ob);

}
YYCLink::YYCLink(double n1,double e1,double r1,
				 double n2,double e2,double r2,
				 double a1,double a2,int lr)
{
	SetSLink(false);
	Set(n1,e1,r1,n2,e2,r2,a1,a2,lr);	
}

YYCLink::~YYCLink()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void YYCLink::SetSLink(bool IsS)
{
	if(IsS)IsSModel=-1;
	else IsSModel=1;
}

void YYCLink::Set(double n1,double e1,double r1,
				  double n2,double e2,double r2,
				  double a1,double a2,int lr)
{
	ERRFlag=false;
	//前一个圆
	CenN1=n1;
	CenE1=e1;
	R1=r1;

	//后一个圆
	CenN2=n2;
	CenE2=e2;
	R2=r2;

	//前缓，后缓
	A1=a1;
	A2=a2;

	K_A_mode=12;
	K12=1.0;//缺省

	/*
	if(A1<=0.00001&&A2<=0.00001)
	{
	K_A_mode=KNOW_K_A1_A2;
	K12=1.0;//缺省
	}
	if(A1>0.00001&&A2<=0.00001)K_A_mode=KNOW_A1;
	if(A2>0.00001&&A1<=0.00001)K_A_mode=KNOW_A2;
	if(A1>0.00001&&A2>0.00001)
	{
	K_A_mode=KNOW_K_A1_A2;
	K12=A1/A2;
	}
	*/
	//ads_printf(L"A1=%lf A2=%lf\n",A1,A2);
	if(A1<0.0 && A2<0.0)
	{
		K_A_mode=KNOW_K_A1_A2;
		K12=1;
	}
	else if(A2<0.0)
	{
		K_A_mode=KNOW_A1;
	}
	else if(A1<0.0)
	{
		K_A_mode=KNOW_A2;
	}
	else
	{
		if(A1<0.000001)
		{
			A1=0.0;
			K_A_mode=KNOW_A1;
		}
		else if(A2<0.000001)
		{
			A2=0.0;
			K_A_mode=KNOW_A2;
		}
		else
		{
			K_A_mode=KNOW_K_A1_A2;
			K12=A1/A2;
		}
	}
	//左右转，左为-右为+
	LeftOrRight=lr;

	//////////////////////////////////////////////////////////////////////////
	//   CALCULATION
	//////////////////////////////////////////////////////////////////////////
	//求圆心距
	//DistO1O2=sqrt( (CenN1-CenN2)*(CenN1-CenN2) +  (CenE1-CenE2)*(CenE1-CenE2) );
	double fwj_O1O2;
	DistO1O2=xyddaa(CenN1,CenE1,CenN2,CenE2,&fwj_O1O2);

	if(DistO1O2<fabs(R1-IsSModel*R2))
	{
		acutPrintf(L"圆心距小于两圆半径差,无法设计公切曲线!\n");
		ERRFlag=true;return;
	}

	if(fabs(DistO1O2-fabs(R1-IsSModel*R2))<PRECISION)
	{
		//两圆公切
		GQ_N=CenN1+R1*cos(fwj_O1O2);
		GQ_E=CenE1+R1*sin(fwj_O1O2);
		pSPIRAL1.Set(GQ_N,GQ_E,fwj_O1O2-LeftOrRight*0.5*Pai,A1,VERY_MAX,VERY_MAX,-1*LeftOrRight);

		pSPIRAL2.Set(GQ_N,GQ_E,fwj_O1O2+LeftOrRight*0.5*Pai,A2,VERY_MAX,VERY_MAX,IsSModel*LeftOrRight);

		pSPIRAL1.GetPoint(pSPIRAL1.EndL,PtOnCir1N,PtOnCir1E);
		pSPIRAL2.GetPoint(pSPIRAL2.EndL,PtOnCir2N,PtOnCir2E);
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	//叠代A1、A2
	//////////////////////////////////////////////////////////////////////////
	double Rmax=R1>R2?R1:R2;
	double Rmin=R1<R2?R1:R2;
	double Ab,Ae;
	Ab=Ae=0.001;

	while(AtoCC(Ae)<DistO1O2)
	{
		Ab=Ae;
		Ae+=0.2*Rmin;	
	}
	//以上为求叠代范围
	//////////////////////////////////////////////////////////////////////////
	count =0;
	if(K_A_mode==KNOW_A1)
	{
		A2=diedai(Ab,Ae);
		if(A2<0)
		{
			ads_printf(L"A2计算有误!");
			ERRFlag=true;return;
		}
	}
	else if(K_A_mode==KNOW_A2)
	{
		A1=diedai(Ab,Ae);

		if(A1<0)
		{
			ads_printf(L"A1计算有误!");
			ERRFlag=true;return;
		}
	}
	else
	{
		A2=diedai(Ab,Ae);
		if(A2<0)
		{
			ads_printf(L"A2计算有误!");
			ERRFlag=true;return;
		}
		A1=K12*A2;
	}

	Lo1=A1*A1/R1;
	Lo2=A2*A2/R2;

	double D1,D2;//放大后半径(公切线与圆心的距离)
	double Xm1/*,Xm2*/;//圆心在公切线上的投影点与缓和曲线起点的距离
	double aj1=Lo1*0.5/R1;
	double y1=spiral_y(A1,Lo1);
	D1=y1+R1*cos(aj1);//圆心到直线之距
	Xm1=spiral_x(A1,Lo1)+R1*cos(0.5*Lo1/R1+0.5*Pai);

	double aj2=Lo2*0.5/R2;
	double y2=spiral_y(A2,Lo2);
	D2=y2+R2*cos(aj2);
	//	acutPrintf(L"fwj_O1O2=%lf\n",fwj_O1O2);
	double jiao=acos((D1-IsSModel*D2)/DistO1O2);
	double fwj_O1P1=fwj_O1O2-LeftOrRight*jiao;//S时针-1 C1圆1
	double fwj_O2P2;

	if(IsSModel)fwj_O2P2=fwj_O1P1+PI;
	else fwj_O2P2=fwj_O1P1;

	//	acutPrintf(L"fwj_O1P1=%lf,fwj_O2P2=%lf\n",fwj_O1P1,fwj_O2P2);


	double N1,E1;//公切线起终点，此点与圆心连线与公切线垂直
	N1=CenN1+D1*cos(fwj_O1P1);
	E1=CenE1+D1*sin(fwj_O1P1);

	//求公切点、两缓和曲线起点
	GQ_N=N1+Xm1*cos(fwj_O1P1+LeftOrRight*0.5*Pai);
	GQ_E=E1+Xm1*sin(fwj_O1P1+LeftOrRight*0.5*Pai);

	pSPIRAL1.Set(GQ_N,GQ_E,fwj_O1P1-LeftOrRight*0.5*Pai,A1,VERY_MAX,R1,-1*LeftOrRight);

	pSPIRAL2.Set(GQ_N,GQ_E,fwj_O2P2-LeftOrRight*0.5*Pai,A2,VERY_MAX,R2,IsSModel*LeftOrRight);

	pSPIRAL1.GetPoint(pSPIRAL1.EndL,PtOnCir1N,PtOnCir1E);
	pSPIRAL2.GetPoint(pSPIRAL2.EndL,PtOnCir2N,PtOnCir2E);
}

//由前后缓和曲线 的A求圆心距
double YYCLink::GetDistO1ToO2(double AA1, double AA2)
{
	double LL1,LL2,CC,XXm1,XXm2,X1,X2,Y1,Y2,t1,t2,dR1,dR2;

	LL1=AA1*AA1/R1;
	LL2=AA2*AA2/R2;
	X1=spiral_x(AA1,LL1);
	Y1=spiral_y(AA1,LL1);
	X2=spiral_x(AA2,LL2);
	Y2=spiral_y(AA2,LL2);
	t1=0.5*LL1/R1;
	t2=0.5*LL2/R2;
	dR1=Y1+R1*cos(t1)-R1;
	dR2=Y2+R2*cos(t2)-R2;
	XXm1=X1-R1*sin(t1);
	XXm2=X2-R2*sin(t2);
	if(IsSModel<0)//如果是S型连接
	{
		CC=sqrt((R1+dR1+(R2+dR2))*(R1+dR1+(R2+dR2))+(XXm1+XXm2)*(XXm1+XXm2));
	}
	else//C型
	{
		CC=sqrt((R1+dR1-(R2+dR2))*(R1+dR1-(R2+dR2))+(XXm1+XXm2)*(XXm1+XXm2));
	}
	return CC;
}

double YYCLink::func(double x)
{
	//	if(x<0.001){acedAlert(L"A<0");return 0.1;}
	return AtoCC(x);
}

double YYCLink::AtoCC(double a)
{
	//
	double AA1,AA2;
	if(K_A_mode==1)
	{
		AA1=A1;
		AA2=a;
	}
	else if(K_A_mode==2)
	{
		AA1=a;
		AA2=A2;
	}
	else //if(K_A_mode==12)
	{
		AA2=a;
		AA1=K12*AA2;
	}	

	return GetDistO1ToO2(AA1,AA2);
}

double YYCLink::diedai(double A21, double A22)//递增A-CC
{
	double A20,CC0;
	int num;
	num=0;

	for(;;)
	{
		num++;
		A20=(A21+A22)/2.0;
		if(A21==A22)return -1;
		CC0=AtoCC(A20);
		if(AtoCC(A21)>AtoCC(A22))return -1;

		if(AtoCC(A22)<AtoCC(A21))
		{
			double temp=A22;
			A22=A21;
			A21=temp;
		}
		if(fabs(CC0-DistO1O2)<1.0E-4)return A20;	

		else if(CC0>DistO1O2)
		{
			A22 = A20;
			//			return diedai(A21,A20);
		}
		else if(CC0<DistO1O2)
		{
			A21 =  A20;
			//			return diedai(A20,A22);
		}
		if(num > 1000)
			return -1;
	}
	return -1;
}


/*
YYCLink::YYCLink(double CenX1,double CenY1,double RR1,
				 double CenX2,double CenY2,double RR2,
				 double AA1,double AA2,double lr)
//lr左右转，左偏为负，右偏为正
{
	Arc1=NULL;Arc2=NULL;
	Cir1=NULL;Cir2=NULL;
	double Ak;
	if(fabs(AA1)<0.0001||fabs(AA1)<0.0001)Ak=1.0;
	else Ak=fabs(AA1/AA2);
	CALCULATION(AcGePoint3d(CenX1,CenY1,0.0),RR1,
		AcGePoint3d(CenX2,CenY2,0.0),RR2,lr,Ak);
}

YYCLink::YYCLink(double x11,double y11,double x10,double y10,double x12,double y12,double x21,double y21,double x20,double y20,double x22,double y22,double lr,double Ak)
//(x11,y11),(x10,y10),(x12,y12)圆一上三点
//(x21,y21),(x20,y20),(x22,y22)圆二上三点
//lr左右转，左偏为负，右偏为正
{
	Arc1=getArc(x11,y11,x10,y10,x12,y12);
	Arc2=getArc(x21,y21,x20,y20,x22,y22);
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Arc1->center(),Arc1->radius(),Arc2->center(),Arc2->radius(),lr,Ak);
}

YYCLink::YYCLink(AcDbEntity *pEnt1,AcDbEntity *pEnt2)
{
	
	Cir1=NULL;Cir2=NULL;
	Arc1=NULL;Arc2=NULL;
	if(pEnt1->isKindOf(AcDbCircle::desc()))Cir1=AcDbCircle::cast(pEnt1);
	if(pEnt2->isKindOf(AcDbCircle::desc()))Cir2=AcDbCircle::cast(pEnt2);
	if(pEnt1->isKindOf(AcDbArc::desc()))Arc1=AcDbArc::cast(pEnt1);
	if(pEnt2->isKindOf(AcDbArc::desc()))Arc2=AcDbArc::cast(pEnt2);
	
	if(Cir1!=NULL&&Cir2!=NULL)YYCLink(Cir1,Cir2);
	else if(Arc1!=NULL&&Arc2!=NULL)YYCLink(Arc1,Arc2);
	else if(Cir1!=NULL&&Arc2!=NULL)YYCLink(Cir1,Arc2);
	else if(Arc1!=NULL&&Cir2!=NULL)YYCLink(Arc1,Cir2);
	else if(Arc1=NULL&&Cir1!=NULL)acutPrintf(L"所选第一个实体不是圆或圆弧！\n");
	else if(Arc2=NULL&&Cir2!=NULL)acutPrintf(L"所选第二个实体不是圆或圆弧！\n");
	return;
}

YYCLink::YYCLink(AcDbArc *pArc1,AcDbArc *pArc2)
{
	Arc1=pArc1;Arc2=pArc2;
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Arc1->center(),Arc1->radius(),Arc2->center(),Arc2->radius());
}
YYCLink::YYCLink(AcDbCircle *pCir1,AcDbCircle *pCir2)
{
	Arc1=NULL;Arc2=NULL;
	Cir1=pCir1;Cir2=pCir2;
	CALCULATION(Cir1->center(),Cir1->radius(),Cir2->center(),Cir2->radius());
}
YYCLink::YYCLink(AcDbArc *pArc1,AcDbCircle *pCir2)
{
	Arc1=pArc1;Arc2=NULL;
	Cir1=NULL;Cir2=pCir2;
	CALCULATION(Arc1->center(),Arc1->radius(),Cir2->center(),Cir2->radius());
}
YYCLink::YYCLink(AcDbCircle *pCir1,AcDbArc *pArc2)
{
	Arc1=NULL;Arc2=pArc2;
	Cir1=pCir1;Cir2=NULL;
	CALCULATION(Cir1->center(),Cir1->radius(),Arc2->center(),Arc2->radius());
}
YYCLink::YYCLink(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2)//K12=A1/A2
{
	//pCir1 is big circle,pCir2 is small circle;
	Arc1=NULL;Arc2=NULL;
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Cen1,r1,Cen2,r2);
}

YYCLink::YYCLink()
{

}

YYCLink::~YYCLink()
{

}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double YYCLink::func(double x)
{
//	if(x<0.001){acedAlert(L"A<0");return 0.1;}
	 return AtoCC(x);
}

void YYCLink::KeyBoardInput()
{
	char kw[20];
	//不允许输入空格
	acedInitGet(RSG_NONULL,"A K a k");
	acedGetKword(L"\n请选择输入方式--(A)输入A1的值 / (K)输入A1与A2的比值:",kw);
	if(acedGetInput(kw)==RTNORM)
	{
		if(_tcscmp(kw,"A")==0||_tcscmp(kw,"a")==0)
		{
			acedGetReal(L"\n请输入A1的值: ",&A1);
			acutPrintf(L"A1=%lf\n",A1);
			K_A_mode=1;//A1
		}
		else if(_tcscmp(kw,"K")==0||_tcscmp(kw,"k")==0)
		{
			acedGetReal(L"\n请输入A1与A2的比值: ",&K12);
			acutPrintf(L"K12=%lf\n",K12);
			K_A_mode=12;//K12
		}
	}
}

void YYCLink::CALCULATION(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2,double lr,double Ak)//K12=A1/A2
{
	//pCir1 is big circle,pCir2 is small circle;
	ERRFlag=false;
	R1=r1;//半径
	R2=r2;//半径
	pCen1=Cen1;//圆心
	pCen2=Cen2;//圆心
	pCen1.z=0;
	pCen2.z=0;
	dC1C2=pCen1.distanceTo(pCen2);
	if(dC1C2+R2<=R1||dC1C2+R1<=R2){acutPrintf(L"dC1C2+R2<=R1||dC1C2+R1<=R2!");ERRFlag=true;return;}
	if(fabs(lr)<0.01)
	{
		KeyBoardInput();
		getlrRot();
	}
	else
	{
		K_A_mode=12;
		if(lr<0)LRflag=-1;
		else LRflag=1;
		K12=Ak;
	}
	count=0;
	double Rmax=R1>R2?R1:R2;
	double Rmin=R1<R2?R1:R2;
	double iR=2;
	double Ab=0.001,Ae=Ab;	
	
	while(AtoCC(Ae)<dC1C2)
	{
		Ab=Ae;
		Ae+=0.2*Rmin;
	}
	
	acutPrintf(L"Ab=%lf,Ae=%lf,CC=%lf\n",Ab,Ae,dC1C2);
	A2=diedai(Ab,Ae);
	if(A2<0){ERRFlag=true;return;}
	acutPrintf(L"diedaifinish");
	if(K_A_mode==12)A1=K12*A2;
	acutPrintf(L"A2=%lf...CC=%lf\n",A2,AtoCC(A2));


	L1=A1*A1/R1;
	L2=A2*A2/R2;

	double aj1=L1*0.5/R1;
	double y1=spiral_y(A1,L1);
	D1=y1+R1*cos(aj1);//圆心到直线之距
	Xm1=spiral_x(A1,L1)+R1*cos(0.5*L1/R1+0.5*Pai);
	acutPrintf(L"Xm1=%lf\n",Xm1);
	double aj2=L2*0.5/R2;
	double y2=spiral_y(A2,L2);
	D2=y2+R2*cos(aj2);
	Xm2=spiral_x(A2,L2)+R2*cos(0.5*L2/R2+0.5*Pai);
	acutPrintf(L"Xm2=%lf\n",Xm2);
	if(Xm1<0||Xm2<0){acutPrintf(L"Xm<0!");ERRFlag=true;return;}
	double d1=Xm1*dC1C2/(Xm1+Xm2);//
	double d2=dC1C2-d1;//
	acutPrintf(L"D1=%lf,D2=%lf\n",D1,D2);
	
	AcDbLine* O1O2=new AcDbLine(pCen1,pCen2);
	double fwj_O1O2=fwj(O1O2);
	acutPrintf(L"fwj_O1O2=%lf\n",fwj_O1O2);
	delete O1O2;
	fwj_O1P1=fwj_O1O2+LRflag*acos((D1-D2)/dC1C2);//S时针-1 C1圆1
	fwj_O2P2=fwj_O1P1;//S时针-1 C2圆2
	acutPrintf(L"fwj_O1P1=%lf,fwj_O2P2=%lf\n",fwj_O1P1,fwj_O2P2);
	
	AcGePoint3d P1(pCen1.x+D1*sin(fwj_O1P1),pCen1.y+D1*cos(fwj_O1P1),0);
	AcGePoint3d P2(pCen2.x+D2*sin(fwj_O2P2),pCen2.y+D2*cos(fwj_O2P2),0);
//	AcDbLine *l1=new AcDbLine(pCen1,P1);
//	AcDbLine *l2=new AcDbLine(pCen2,P2);
//	AddEntityToDbs(l1);
//	AddEntityToDbs(l2);
	
	GQ.set(P1.x+Xm1*sin(fwj_O1P1-LRflag*0.5*Pai),P1.y+Xm1*cos(fwj_O1P1-LRflag*0.5*Pai),0);

	pSPIRAL1.Set(GQ.y,GQ.x,);
	pSPIRAL2=new SPIRAL(A2,R2,GQ,fwj_O2P2+LRflag*0.5*Pai+Pai,0,-1.0*LRflag);
	
//	pSPIRAL1=new SPIRAL(A1,R1,GQ,fwj_O1P1+LRflag*0.5*Pai,0,LRflag);
//	pSPIRAL2=new SPIRAL(A2,R2,GQ,fwj_O2P2+LRflag*0.5*Pai+Pai,0,-1.0*LRflag);
	////////////////
//	AcDbCircle *Circle1=new AcDbCircle(pCen1,AcGeVector3d(0,0,1),R1);
//	AcDbCircle *Circle2=new AcDbCircle(pCen2,AcGeVector3d(0,0,1),R2);
//	pSPIRAL1->AddReviseItem(Circle1);pSPIRAL2->AddReviseItem(Circle2);
	PtOnCir1X=pSPIRAL1->endX0;
	PtOnCir1Y=pSPIRAL1->endY0;
	PtOnCir2X=pSPIRAL2->endX0;
	PtOnCir2Y=pSPIRAL2->endY0;	
	if(fabs(lr)<0.01)
	{
		spirID1=AddEntityToDbs(pSPIRAL1,1);spirID2=AddEntityToDbs(pSPIRAL2,1);
		if(Arc1!=NULL)pSPIRAL1->TrimARC(Arc1);
		if(Arc2!=NULL)pSPIRAL2->TrimARC(Arc2);
		pSPIRAL1->close();	pSPIRAL2->close();
	}
	else
	{
		delete pSPIRAL1;delete pSPIRAL2;
	}
	//	pSPIRAL1->worldDraw();pSPIRAL2->worldDraw();
//	delete pSPIRAL1;delete pSPIRAL2;
//	delete Circle1;delete Circle2;		
}




double YYCLink::AtoCC(double AA2)
{
	double AA1,LL1,LL2,CC,XXm1,XXm2,X1,X2,Y1,Y2,t1,t2,dR1,dR2;
	if(K_A_mode==12)AA1=K12*AA2;
	else if(K_A_mode==1)AA1=A1;
	LL1=AA1*AA1/R1;
	LL2=AA2*AA2/R2;
	X1=spiral_x(AA1,LL1);
	Y1=spiral_y(AA1,LL1);
	X2=spiral_x(AA2,LL2);
	Y2=spiral_y(AA2,LL2);
	t1=0.5*LL1/R1;
	t2=0.5*LL2/R2;
	dR1=Y1+R1*cos(t1)-R1;
	dR2=Y2+R2*cos(t2)-R2;
	XXm1=X1-R1*sin(t1);
	XXm2=X2-R2*sin(t2);
	CC=powf(powf(R1+dR1-(R2+dR2),2)+powf(XXm1+XXm2,2),0.5);
//	if(AA<0||CC<=0)
//	{
////		char ch[80];
////		_stprintf(ch,"CC=%lf\n",CC);
////		acedAlert(ch);
//	acutPrintf(L"AA=%lf,CC=%lf\n",AA,CC);	
//		return 1;
//	}
	return CC;
}

void YYCLink::Create()
{}


void YYCLink::getlrRot()
{	
	char kw[8];
	acedInitGet(RSG_NONULL,"F S f s");
	acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
	if(acedGetInput(kw)==RTNORM)
	{
		if(_tcscmp(kw,"F")==0||_tcscmp(kw,"f")==0)LRflag=1;
		else if(_tcscmp(kw,"S")==0||_tcscmp(kw,"s")==0)LRflag=-1;
	}
}

void YYCLink::GetSpirItem(int i,double &AA, double &RR, double &x,double &y, double &fwj, double &slc, double &lr)
{
	if(i==1)
	{
		AA=A1;RR=R1;x=GQ.x;y=GQ.y;fwj=fwj_O1P1+LRflag*0.5*Pai;slc=0;
		lr=LRflag;
	}
	else
	{
		AA=A2;RR=R2;x=GQ.x;y=GQ.y;fwj=fwj_O2P2+LRflag*0.5*Pai;slc=0;
		lr=LRflag;
	}	
}
*/
