// YYSLink.cpp: implementation of the YYSLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "YYSLink.h"
#include "ZYLink.h"
#include "math.h"
#define Pai 3.14159265358979323846264338327
#include "spiral.h"
#include "acedads.h"
#include "dbents.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

YYSLink::YYSLink()
{
	SetSLink(true);
}

YYSLink::YYSLink(double n1,double e1,double r1,
				 double n2,double e2,double r2,
				 double a1,double a2,int lr)
{
	SetSLink(true);
	Set(n1,e1,r1,n2,e2,r2,a1,a2,lr);	
}

YYSLink::~YYSLink()
{
	
}


/*
YYSLink::YYSLink(double CenX1,double CenY1,double RR1,
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

YYSLink::YYSLink(double x11,double y11,double x10,double y10,double x12,double y12,double x21,double y21,double x20,double y20,double x22,double y22,double lr,double Ak)
//(x11,y11),(x10,y10),(x12,y12)圆一上三点
//(x21,y21),(x20,y20),(x22,y22)圆二上三点
//lr左右转，左偏为负，右偏为正
{
	Arc1=getArc(x11,y11,x10,y10,x12,y12);
	Arc2=getArc(x21,y21,x20,y20,x22,y22);
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Arc1->center(),Arc1->radius(),Arc2->center(),Arc2->radius(),lr,Ak);
}

YYSLink::YYSLink(AcDbEntity *pEnt1,AcDbEntity *pEnt2)
{
	
	Cir1=NULL;Cir2=NULL;
	Arc1=NULL;Arc2=NULL;
	if(pEnt1->isKindOf(AcDbCircle::desc()))Cir1=AcDbCircle::cast(pEnt1);
	if(pEnt2->isKindOf(AcDbCircle::desc()))Cir2=AcDbCircle::cast(pEnt2);
	if(pEnt1->isKindOf(AcDbArc::desc()))Arc1=AcDbArc::cast(pEnt1);
	if(pEnt2->isKindOf(AcDbArc::desc()))Arc2=AcDbArc::cast(pEnt2);

	if(Cir1!=NULL&&Cir2!=NULL)YYSLink(Cir1,Cir2);
	else if(Arc1!=NULL&&Arc2!=NULL)YYSLink(Arc1,Arc2);
	else if(Cir1!=NULL&&Arc2!=NULL)YYSLink(Cir1,Arc2);
	else if(Arc1!=NULL&&Cir2!=NULL)YYSLink(Arc1,Cir2);
	else if(Arc1=NULL&&Cir1!=NULL)acutPrintf(L"所选第一个实体不是圆或圆弧！\n");
	else if(Arc2=NULL&&Cir2!=NULL)acutPrintf(L"所选第二个实体不是圆或圆弧！\n");
	return;
}

YYSLink::YYSLink(AcDbArc *pArc1,AcDbArc *pArc2)
{
	Arc1=pArc1;Arc2=pArc2;
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Arc1->center(),Arc1->radius(),Arc2->center(),Arc2->radius());
}
YYSLink::YYSLink(AcDbCircle *pCir1,AcDbCircle *pCir2)
{
	Arc1=NULL;Arc2=NULL;
	Cir1=pCir1;Cir2=pCir2;
	CALCULATION(Cir1->center(),Cir1->radius(),Cir2->center(),Cir2->radius());
}
YYSLink::YYSLink(AcDbArc *pArc1,AcDbCircle *pCir2)
{
	Arc1=pArc1;Arc2=NULL;
	Cir1=NULL;Cir2=pCir2;
	CALCULATION(Arc1->center(),Arc1->radius(),Cir2->center(),Cir2->radius());
}
YYSLink::YYSLink(AcDbCircle *pCir1,AcDbArc *pArc2)
{
	Arc1=NULL;Arc2=pArc2;
	Cir1=pCir1;Cir2=NULL;
	CALCULATION(Cir1->center(),Cir1->radius(),Arc2->center(),Arc2->radius());
}
YYSLink::YYSLink(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2)//K12=A1/A2
{
	//pCir1 is big circle,pCir2 is small circle;
	Arc1=NULL;Arc2=NULL;
	Cir1=NULL;Cir2=NULL;
	CALCULATION(Cen1,r1,Cen2,r2);
}

YYSLink::YYSLink()
{
	
}

YYSLink::~YYSLink()
{
	
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
double YYSLink::func(double x)
{
//	if(x<0.001){acedAlert(L"A<0");return 0.1;}
	 return AtoCC(x);
}



void YYSLink::CALCULATION(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2,double lr,double Ak)//K12=A1/A2
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
	if(dC1C2<R2+R1){acutPrintf(L"Intersectwith!");ERRFlag=true;return;}

	if(fabs(lr)<0.01)
	{
		KeyBoardInput();
		getlrRot();
	}
	else
	{
		LRflag=lr;
		K_A_mode=12;//K12
		K12=Ak;//I define	
	}
	
	double Rmax=R1>R2?R1:R2;
	double Rmin=R1<R2?R1:R2;
	double iR=2;
	double Ab=0.001,Ae=Ab;	
	while(AtoCC(Ae)<dC1C2)
	{
		Ab=Ae;
		Ae+=0.2*Rmin;
	}
	//	if(Ae>3*Rmax)return;
//	acutPrintf(L"Ab=%lf,Ae=%lf,CC=%lf\n",Ab,Ae,dC1C2);
	A2=diedai(Ab,Ae);
	if(A2<0)return;
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
	double d1=D1*dC1C2/(D1+D2);//两圆（R->R＋dR）公切线与两圆连线交点到圆心之距
	double d2=dC1C2-d1;
	AcDbLine* O1O2=new AcDbLine(pCen1,pCen2);
	double fwj_O1O2=fwj(O1O2);
	delete O1O2;
	double fwj_O2O1=fwj_O1O2+Pai;
	if(fwj_O2O1>2*Pai)fwj_O2O1-=2*Pai;
	fwj_O1P1=fwj_O1O2+LRflag*acos(D1/d1);//S时针-1 C1圆1
	fwj_O2P2=fwj_O2O1+LRflag*acos(D1/d1);//S时针-1 C2圆2
	AcGePoint3d P1(pCen1.x+D1*sin(fwj_O1P1),pCen1.y+D1*cos(fwj_O1P1),0);
	AcGePoint3d P2(pCen2.x+D2*sin(fwj_O2P2),pCen2.y+D2*cos(fwj_O2P2),0);
	GQ.set(P1.x+Xm1*sin(fwj_O1P1-LRflag*0.5*Pai),P1.y+Xm1*cos(fwj_O1P1-LRflag*0.5*Pai),0);
	
	pSPIRAL1=new SPIRAL(A1,R1,GQ,fwj_O1P1+LRflag*0.5*Pai,0,LRflag);
	pSPIRAL2=new SPIRAL(A2,R2,GQ,fwj_O2P2+LRflag*0.5*Pai,0,LRflag);
	/////////////////////////////////
	AcDbCircle *Circle1=new AcDbCircle(pCen1,AcGeVector3d(0,0,1),R1);
	AcDbCircle *Circle2=new AcDbCircle(pCen2,AcGeVector3d(0,0,1),R2);
	pSPIRAL1->AddReviseItem(Circle1);pSPIRAL2->AddReviseItem(Circle2);

	PtOnCir1X=pSPIRAL1->endX0;
	PtOnCir1Y=pSPIRAL1->endY0;
	PtOnCir2X=pSPIRAL2->endX0;
	PtOnCir2Y=pSPIRAL2->endY0;	
	
	//	pSPIRAL1->worldDraw();pSPIRAL2->worldDraw();
	//	delete Circle1;delete Circle2;
	//	delete pSPIRAL1;delete pSPIRAL2;
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
}

void YYSLink::KeyBoardInput()
{
	char kw[8];
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

double YYSLink::diedai(double A21, double A22)//递增A2-CC
{
	double A20,CC0;
	A20=(A21+A22)/2.0;
	count++;	
	CC0=AtoCC(A20);
	if(count==-1)
	{
		acedCommandS(RTSTR,"pline",RTNONE);
		double kk=0;
		double buchang=(A22-A21)/100.0;
		for(double a=0.1;a<5*A22&&kk<6*R2;a+=buchang)
		{
			ads_point Pt;
			Pt[X]=a;
			Pt[Y]=AtoCC(a);
			kk=Pt[Y];
			acedCommandS(RTPOINT,Pt,RTNONE);
		}
		acedCommandS(RTSTR,"",RTNONE);
	}
		
	if(fabs(CC0-dC1C2)<1.0E-4)return A20;	
	
	else if(CC0>dC1C2)
	{
		return diedai(A21,A20);
	}
	else if(CC0<dC1C2)
	{
		return diedai(A20,A22);
	}
	return -1;
}

double YYSLink::AtoCC(double AA2)
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
	dR1=Y1+R1*sin(t1+0.5*Pai)-R1;
	dR2=Y2+R2*sin(t2+0.5*Pai)-R2;
	XXm1=X1+R1*cos(t1+0.5*Pai);
	XXm2=X2+R2*cos(t2+0.5*Pai);
	CC=powf(powf(R1+dR1+(R2+dR2),2)+powf(XXm1+XXm2,2),0.5);
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

void YYSLink::Create()
{}
/ *
{
	AcGePoint3d startPt,Pt0;//第一个切点，ZH点
	double sita,sita0,dd;//第一个切点的切线方位角,ZH点处切线方位角
	int i=1;
	if(LRflag<0)i=0;//右偏。
	if(D1>D2)
	{
		dd=powf(powf((points[0].x-pCen1.x),2)+powf((points[0].y-pCen1.y),2)+0,0.5);
		AcGeVector3d normal1((points[i].x-pCen1.x)/dd,(points[i].y-pCen1.y)/dd,0);
		AcGePoint3d P1(pCen1.x+D1*normal1.x,pCen1.y+D1*normal1.y,0);
		AcGePoint3d P2(pCen2.x+D2*normal1.x,pCen2.y+D2*normal1.y,0);
		AcDbLine *Line=new AcDbLine(P1,P2);
		Line->close();
		sita0=fwj(Line);
		Pt0.x=P1.x-Xm1*sin(sita0);
		Pt0.y=P1.y-Xm1*cos(sita0);
		SPIRAL *pSpir=new SPIRAL(A,R2,Pt0,sita0,L1,LRflag);
		AddEntityToDbs(pSpir);
	}
	else 
	{
		dd=powf(powf((points[0].x-pCen2.x),2)+powf((points[0].y-pCen2.y),2)+0,0.5);
		AcGeVector3d normal1((points[i].x-pCen2.x)/dd,(points[i].y-pCen2.y)/dd,0); 	
		AcGePoint3d P1(pCen1.x+D1*normal1.x,pCen1.y+D1*normal1.y,0);
		AcGePoint3d P2(pCen2.x+D2*normal1.x,pCen2.y+D2*normal1.y,0);
		AcDbLine *Line=new AcDbLine(P1,P2);
		Line->close();		
		sita0=fwj(Line);
		Pt0.x=P1.x-Xm1*sin(sita0);
		Pt0.y=P1.y-Xm1*cos(sita0);
		SPIRAL *pSpir=new SPIRAL(A,R2,Pt0,sita0,L1,LRflag);
		AddEntityToDbs(pSpir);
	}	
}* /



void YYSLink::getlrRot()
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

void YYSLink::getSpiralId(AcDbObjectId &Id1,AcDbObjectId &Id2)
{
	Id1=spirID1;	Id2=spirID2;////////////
}

void YYSLink::GetSpirItem(int i,double &AA, double &RR, double &x,double &y, double &fwj, double &slc, double &lr)
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
}*/

