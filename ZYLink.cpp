// ZYLink.cpp: implementation of the ZYLink class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ZYLink.h"
#include "math.h"
#include "acdb.h"
#include "dbents.h"
#include "migrtion.h"
#include "acedads.h"
#define PI 3.14159265358979323846264338327
extern int N;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ZYLink::ZYLink()
{
	ERRFlag=false;
	dR=0.0;
}

ZYLink::~ZYLink()
{

}

ZYLink::ZYLink(AcDbLine *pLine, AcDbArc *pArc,int lr)
{
	double N1,E1,N2,E2,cenN,cenE,r;
	N1=pLine->startPoint().y;
	E1=pLine->startPoint().x;
	N2=pLine->endPoint().y;
	E2=pLine->endPoint().x;
	cenN=pArc->center().y;
	cenE=pArc->center().x;
	r=pArc->radius();
	Set(N1,E1,N2,E2,cenN,cenE,r,lr);
	//////////////////////////////////
	if(ERRFlag)return;

	JdCenter * jdcenay=new JdCenter[3];
	int i=0;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].LeftOrRight=lr;
	jdcenay[i].IsOrNotHuiTou=0;
	i=1;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=cenN;
	jdcenay[i].E=cenE;
	jdcenay[i].R=R;
	jdcenay[i].A1=A;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].A2=0;
	jdcenay[i].LeftOrRight=LeftOrRight;
	i=2;
	jdcenay[i].JDXZ=IsJiaoDian;
	jdcenay[i].IsOrNotHuiTou=0;
	double b=pSPIRAL.EndFwj-LeftOrRight*0.5*PI+LeftOrRight*0.5*PI;
	jdcenay[i].N=cenN+R*cos(b);
	jdcenay[i].E=cenE+R*sin(b);
	////////////////////////////////////////////////
	double ProjN,ProjE;
	double distToNearPt,ProjDist;
	ProjN=cenN;
	ProjE=cenE;
	ProjDist=DistToLine(N1,E1,N2,E2,ProjN,ProjE,distToNearPt);
	double side1=GetSide(cenN,cenE,ProjN,ProjE,N1,E1);
	double side2=GetSide(cenN,cenE,ProjN,ProjE,N2,E2);
	double tmpfwj;
	double dist1=xyddaa(ProjN,ProjE,N1,E1,&tmpfwj);
	double dist2=xyddaa(ProjN,ProjE,N2,E2,&tmpfwj);
	double Xm=spiral_x(A,A*A/R);
	if(lr<0.0)
	{
		if(side1<0.0)
		{
			if(dist1>Xm)
			{
				jdcenay[0].N=N1;
				jdcenay[0].E=E1;
			}
			else
			{
				jdcenay[0].N=BaseN;
				jdcenay[0].E=BaseE;
			}
		}
		else if(side2<0.0)
		{
			if(dist2>Xm)
			{
				jdcenay[0].N=N2;
				jdcenay[0].E=E2;
			}
			else
			{
				jdcenay[0].N=BaseN;
				jdcenay[0].E=BaseE;
			}
		}
		else
		{
			jdcenay[0].N=BaseN;
			jdcenay[0].E=BaseE;
		}
	}
	else
	{
		if(side1>0.0)
		{
			if(dist1>Xm)
			{
				jdcenay[0].N=N1;
				jdcenay[0].E=E1;
			}
			else
			{
				jdcenay[0].N=BaseN;
				jdcenay[0].E=BaseE;
			}
		}
		else if(side2>0.0)
		{
			if(dist2>Xm)
			{
				jdcenay[0].N=N2;
				jdcenay[0].E=E2;
			}
			else
			{
				jdcenay[0].N=BaseN;
				jdcenay[0].E=BaseE;
			}
		}
		else
		{
			jdcenay[0].N=BaseN;
			jdcenay[0].E=BaseE;
		}
	}
	/////////////////////////////////////
	double EndJiao;
	if(lr<0.0)EndJiao=pArc->endAngle();
	else EndJiao=pArc->startAngle();
	EndJiao=2.5*PI-EndJiao;
	jdcenay[2].N=cenN+R*cos(EndJiao);
	jdcenay[2].E=cenE+R*sin(EndJiao);

	/////////////////////////////////////
	JD_CENTER *ob=new JD_CENTER;
	ob->SetJdCenterArray(jdcenay,3);

	delete []jdcenay;	
	BAS_DRAW_FUN::AddEntityToDbs(ob);
}

ZYLink::ZYLink(double N1,double E1,double fwj,double cenN,double cenE,double r,int lr)
{
	Set(N1,E1,N1+1000*cos(fwj),E1+1000*sin(fwj),cenN,cenE,r,lr);
}

ZYLink::ZYLink(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr)
{
	Set(N1,E1,N2,E2,cenN,cenE,r,lr);
}

void ZYLink::Set(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr)
{
	R=r;
	LeftOrRight=lr;

	double tmpcenN,tmpcenE,ProDist;
	tmpcenN=cenN;
	tmpcenE=cenE;
	dR=DistToLine(N1,E1,N2,E2,tmpcenN,tmpcenE,ProDist);

	ERRFlag=false;

	if(dR<R-0.00001)
	{
		A = 0;
		ERRFlag=true;
		return;
	}

	if(fabs(dR-R)<=0.00001)
	{
		A = 0;
		//	acutPrintf(L"直圆相切!\n");
		return;
	}
	else
	{
		double ax=0.002*R;
		double bx=0.75*R;
		double cx=20*R;
		double tol=0.0000001;
		double xmin;

		double ymin=golden(ax,bx,cx,tol,xmin);

		//	ymin=-ymin;
		if(xmin<=0)
		{
			ads_printf(L"错误: 直圆计算有误!\n");
			ERRFlag=true;return;
		}
		count=0;

		A=diedai(ax,xmin,1);
		if(A<1e-6)
			//		if(ymin>=dR) A=diedai(ax,xmin,1);
				//		else
		{
			ads_printf(L"错误: A计算有误!\n");
			ERRFlag=true;return;
		}	
	}

	double fwjCenToProj;//圆心到投影点的方位角
	xyddaa(cenN,cenE,tmpcenN,tmpcenE,&fwjCenToProj);
	double EndL=A*A/R;

	double sita=acos( (dR-spiral_y(A,EndL) )/R);//圆心与HY点连线与圆心与投影点连线的交角
	double EndFwj=fwjCenToProj+LeftOrRight*( sita +0.5*PI );
	double BaseFwj=EndFwj-LeftOrRight*(0.5*EndL/R);
	double distProjToBase=spiral_x(A,EndL)-R*sin(sita);//投影点到ZH点的距离

	BaseN=tmpcenN-distProjToBase*cos(BaseFwj);
	BaseE=tmpcenE-distProjToBase*sin(BaseFwj);
	pSPIRAL.Set(BaseN,BaseE,BaseFwj,A,VERY_MAX,R,LeftOrRight);

	/*
	AcDbLine *L=new AcDbLine(AcGePoint3d(BaseE,BaseN,0.0),AcGePoint3d(cenE,cenN,0.0));
	AddEntityToDbs(L);

	AcDbLine *L2=new AcDbLine(AcGePoint3d(BaseE,BaseN,0.0),AcGePoint3d(tmpcenE,tmpcenN,0.0));
	AddEntityToDbs(L2);*/	
}

double ZYLink::diedai(double AA1, double AA2,int flag)
{
	int num;
	num=0;

	for(;;)
	{
		num++;
		double dr0,AA0=(AA1+AA2)/2.0;

		if(num >5000)
			//	return AA0;
				return 0;
		if(fabs(AA1-AA2)<1.0e-8)
			return 0;

		dr0=AtodR(AA0);
		if(flag<0)dr0=-dr0;
		if(fabs(dr0-dR)<1.0e-8 )return AA0;
		else if(dr0>dR)
		{ 
			AA2 = AA0;
			//			return diedai(AA1,AA0,flag);
		}
		else if(dr0<dR)
		{ 
			AA1 = AA0;
			//			return diedai(AA0,AA2,flag);
		}
	}
	return 0;
}

double ZYLink::func(double x)
{
	if(x<0){return -1;}
	return -1.0*AtodR(x);
}

double ZYLink::AtodR(double AA)
{
	double l=AA*AA/R;
	double y=spiral_x(AA,l);
	double x=spiral_y(AA,l);
	double t=l/(2*R);
	double dr=x+R*sin(t+0.5*PI);	
	//	double dr=x+R*cos(t);	
	return dr;
}
/*
ZYLink::ZYLink(double X0,double Y0,double fwj,double CenX,double CenY,double RR,int lr)
{
	ERRFlag=false;
	Line=NULL;
	Arc=NULL;
	Cir=NULL;
	startFWJ=fwj;
	/////////////////
	startPtL.set(X0,Y0,0.0);//直线起点l
	double X1,Y1,temp;
	X1=X0+100.0*sin(fwj);
	Y1=Y0+100.0*cos(fwj);
	endPtL.set(X1,Y1,0.0);//直线终点
	center.set(CenX,CenY,0.0);//圆心
	R=RR;//半径
	dR=DistToLine(X0,Y0,X1,Y1,CenX,CenY,temp);
	///////////////////////////
	if(dR<R){acutPrintf(L"dR<R!\n");ERRFlag=true;return;}
	CALCULATION(lr);		
}

ZYLink::ZYLink(AcDbLine *pLine, AcDbArc *pArc,int lr)
{
	ERRFlag=false;
	Line=pLine;
	Arc=pArc;
	Cir=NULL;
	/////////////////
	startPtL=Line->startPoint();//直线起点l
	startPtL.z=0;
	endPtL=Line->endPoint();//直线终点
	endPtL.z=0;
	center=Arc->center();//圆心
	center.z=0;
	R=Arc->radius();//半径
	dR=distptol(center,Line);//干线到圆心的距离	
	///////////////////////////
	if(dR<R){acutPrintf(L"dR<R!\n");ERRFlag=true;return;}
	CALCULATION(lr);		
}

ZYLink::ZYLink(AcDbLine *pLine,AcDbCircle *pCir)
{
	Line=pLine;
	Cir=pCir;
	Arc=NULL;
	/////////////////
	startPtL=Line->startPoint();//直线起点l
	startPtL.z=0;
	endPtL=Line->endPoint();//直线终点
	endPtL.z=0;
	center=Cir->center();//圆心
	center.z=0;
	R=Cir->radius();//半径
	dR=distptol(center,Line);//干线到圆心的距离	
///////////////////////////
	CALCULATION();
}

ZYLink::ZYLink()
{
}

ZYLink::~ZYLink()
{

}

double ZYLink::jifen_sin(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s){
	double buchang=0.001,chuzhi,sum=0;
	for(chuzhi=start_s;chuzhi<=end_s;chuzhi+=buchang){
		sum+=sin(afa_s(afa1,pianxiang,chuzhi,Ls,Rt))*buchang;
	}
	return (sum);
}

double ZYLink::jifen_cos(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s){
	double buchang=0.001,chuzhi,sum=0;
	for(chuzhi=start_s;chuzhi<=end_s;chuzhi+=buchang)sum+=cos(afa_s(afa1,pianxiang,chuzhi,Ls,Rt))*buchang;
	return (sum);
}

double ZYLink::afa_s(double afa1, int pianxiang, double s, double Ls, double Rt)
{
	return (afa1-pianxiang*s*s/(2*Rt*Ls));
}

double ZYLink::diedai(double AA1, double AA2,int flag)
{
	double dr0,AA0=(AA1+AA2)/2.0;
	count++;
	if(count==-1)
	{
		acedCommandS(RTSTR,"pline",RTNONE);
		double kk=0;
		double buchang=fabs(AA2-AA1)/100.0;
		for(double a=0.1;a<5*AA2;a+=buchang)
		{
			ads_point Pt;
			Pt[X]=a;
			Pt[Y]=AtodR(a);
			kk=Pt[Y];
			acedCommandS(RTPOINT,Pt,RTNONE);
		}
		acedCommandS(RTSTR,"",RTNONE);
	}
	dr0=AtodR(AA0);
//	acutPrintf(L"A'=%lf,dr=%lf,dR=%lf\n",AA0,dr0,dR);
	if(flag<0)dr0=-dr0;
	
//	if(count>10000||dr0<0)return -1;
	if(fabs(dr0-dR)<1.0e-4)return AA0;
	else if(dr0>dR)
	{ 
		return diedai(AA1,AA0,flag);
//		if(flag>0)if(flag<0)return diedai(AA0,AA2,flag);
	}
	else if(dr0<dR)
	{ 
		return diedai(AA0,AA2,flag);
//		if(flag<0)if(flag>0)return diedai(AA0,AA2,flag);
	}
	return -1;
}

void ZYLink::getlrRot(int lr)
{	
	//	AcDbLine *L1=new AcDbLine(center,startPtL);
	//	AcDbLine *L2=new AcDbLine(center,endPtL);
	//	double a1=fwj(L1),a2=fwj(L2);
	//	delete L1;
	//	delete L2;
	//	double a3=a2-a1;
	//	if(a3<0)a3+=2*PI;
	//	if(a3<=PI)lrRot=1;
	//	else lrRot=-1;
	if(lr==0)
	{
		char kw[8];
		acedInitGet(RSG_NONULL,"F S f s");
		acedGetKword(L"\n请选择连接方向--(S)顺时针 / (F)返时针:",kw);
		if(acedGetInput(kw)==RTNORM)
		{
			if(_tcscmp(kw,"F")==0||_tcscmp(kw,"f")==0)lrRot=-1;
			else if(_tcscmp(kw,"S")==0||_tcscmp(kw,"s")==0)lrRot=1;
		}
	}
	else if(lr<0)lrRot=-1;//left
	else lrRot=1;
	AcDbLine *L1=new AcDbLine(center,startPtL);
	AcDbLine *L2=new AcDbLine(center,endPtL);
	double a1=fwj(L1),a2=fwj(L2);
	delete L1;
	delete L2;
	double a3=a2-a1,lrRot1;
	if(a3<0)a3+=2*PI;
	if(a3<=PI)lrRot1=1;
	else lrRot1=-1;
	if(Line)startFWJ=fwj(Line);
	if(lrRot1*lrRot<0)startFWJ+=Pai;	
}



void ZYLink::CALCULATION(int lr)
{
	ERRFlag=false;
	if(dR<R){ERRFlag=true;acutPrintf(L"直圆相交!");return;}
	if(dR==R){ERRFlag=true;acutPrintf(L"直圆相切!");return;}
	
	getlrRot(lr);//得到startFwj,lrRot
	double ax=0.002*R;
	double bx=0.75*R;
	double cx=20*R;
	double tol=0.00001;
	double xmin;
	double ymin=golden(ax,bx,cx,tol,xmin);
	ymin=-ymin;
	acutPrintf(L"xmin=%lf\n",xmin);
//	acedAlert(L"xmin");
	if(xmin<=0){ERRFlag=true;return;}
	count=0;
	acutPrintf(L"ymin=%lf,dR=%lf\n",ymin,dR);
//	acedAlert(L"ymin");
	if(ymin>=dR)A=diedai(ax,xmin,1);
	else {ERRFlag=true;return;}
//	else A=diedai(xmin,cx,-1);
//	acutPrintf(L"Axmin=%lf,A_left=%lf,A_right=%lf\n",xmin,A_left,A_right);//
//	if(A_left<0)return;
//	A=A_left;
//	A=A_right;
	
//	count=0;
//	A=diedai(Ab,Ae);
//	if(A<0){acutPrintf(L"A=%lf\n");return;}
//	count=0;
//	A=diedai(0.5*R,1.8*R,1);
	double Ls=A*A/R;
	double x,y;
/ *
	double b=1.0/24/Rt;
	double a=-1.0/2688/(Rt*Rt*Rt);
	double c=-dR;
	if(b*b-4*a*c<0){return -1;}
	double u=(-1*b+sqrt(b*b-4*a*c))/(2*a);
	Ls=sqrt(u);
* /
	//	Ls1=0.9*Ls;
	//	Ls2=1.1*Ls;
//	acutPrintf(L"zydiedai=%lf",Ls);
	double afa2=startFWJ+0.5*lrRot*Ls/R;
	endPtCur.x=center.x-R*sin(afa2+lrRot*Pai/2.0);
	endPtCur.y=center.y-R*cos(afa2+lrRot*Pai/2.0);
	endPtCur.z=0;
//	AcDbLine *ll=new AcDbLine(center,endPtCur);
//	AddEntityToDbs(ll);
	//////////////////////////////////////////////////////////////////////////
	y=spiral_x(A,Ls);x=lrRot*spiral_y(A,Ls);
	double sita;
	double dist0=xyddaa(0,0,y,x,&sita);	//缓和曲线起点到R无穷大处（虚）之距
	startPtCur.x=endPtCur.x-dist0*sin(startFWJ+sita);//缓和曲线半径无穷大处真实坐标
	startPtCur.y=endPtCur.y-dist0*cos(startFWJ+sita);
	startPtCur.z=0;
	AcGePoint3d Pt(startPtCur.x,startPtCur.y,0);
	pSpiral=new SPIRAL(A,R,startPtCur,startFWJ,0,lrRot);
	pSpiral->AddReviseItem(Line);
	if(Cir!=NULL)pSpiral->AddReviseItem(Cir);
	if(Arc!=NULL)pSpiral->AddReviseItem(Arc);
//	pSpiral->worldDraw();
//	delete pSpiral;
	if(lr==0)
	{
		pSpirId=AddEntityToDbs(pSpiral,1);//not close
		pSpiral->TrimLine(Line);
		if(Arc!=NULL)pSpiral->TrimARC(Arc);
		pSpiral->close();
	}
	else
	{
/ *
		pSpirId=AddEntityToDbs(pSpiral);
		acDocManager->lockDocument(acDocManager->curDocument(),AcAp::kWrite);
		
		acdbOpenObject(pSpiral,pSpirId,AcDb::kForWrite,Adesk::kFalse);
		pSpiral->erase();
		pSpiral->close();

		acDocManager->unlockDocument(acDocManager->curDocument());* /

	}
}

void ZYLink::getSpiralId(AcDbObjectId& Id1)
{
	Id1=pSpirId;
}

void ZYLink::GetSpirItem(int i, double &AA, double &RR, AcGePoint3d &sPt, double &fwj, double &slc, int &lr,AcGePoint3d &ePt)
{
	if(i==1)
	{
		AA=A;RR=R;sPt=startPtCur;fwj=startFWJ;slc=0;
		ePt=pSpiral->getPoint(pSpiral->logicalLength()-1);
		if(lrRot>0)lr=1;
		else lr=-1;
	}
	else return;
}
*/

//afa:缓和曲转角
double ZYLink::DistCenToQX(double r, double a,double &afa)
{
	double tmpx,tmpy,tmpdist;
	double x1,y1,x2,y2,cx,cy;

	SPIRAL pSpir(0.0,0.0,0.0,a,VERY_MAX,r,-1);
	x1=pSpir.StartN;
	y1=pSpir.StartE;

	x2=x1+1000.0*cos(pSpir.StartFwj);
	y2=y1+1000.0*sin(pSpir.StartFwj);

	cx=pSpir.EndN+r*cos(pSpir.EndFwj-0.5*PI);
	cy=pSpir.EndE+r*sin(pSpir.EndFwj-0.5*PI);
	//afa=pSpir.StartFwj-pSpir.EndFwj;
	//	StdJiao(afa);
	afa = ZXJ(pSpir.StartFwj,pSpir.EndFwj);

	tmpx=cx;
	tmpy=cy;
	return DistToLine(x1,y1,x2,y2,tmpx,tmpy,tmpdist);
}
/*

void ZYLink::AppendXL(JD_CENTER *XLObj1,int JDNo1)
{
    JdCenter * jdcenay=new JdCenter[2];
	int i=0;
	jdcenay[i].JDXZ=IsCenter;
	jdcenay[i].N=cenN;
	jdcenay[i].E=cenE;
	jdcenay[i].R=R;
	jdcenay[i].A1=A;
	jdcenay[i].LinkAfter=NotRelation;
	jdcenay[i].A2=0;
	jdcenay[i].LeftOrRight=LeftOrRight;
	i=1;
	jdcenay[i].JDXZ=IsJiaoDian;
	double b=pSPIRAL.EndFwj-LeftOrRight*0.5*PI+LeftOrRight*0.5*PI;
	jdcenay[i].N=cenN+R*cos(b);
	jdcenay[i].E=cenE+R*sin(b);

	//移除本线的后面交点/圆
	XLObj1->JdCenArray.RemoveAt(JDNo1+1,XLObj1->JdCenNum-(JDNo1+1));
    XLObj1->JdCenArray.Add(jdcenay[0]);
	XLObj1->JdCenArray.Add(jdcenay[1]);
	XLObj1->JdCenNum=XLObj1->JdCenArray.GetSize();		
	delete []jdcenay;
}
*/
