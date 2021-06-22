// SPIRAL.cpp: implementation of the SPIRAL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SPIRAL.h"
#define Pai 3.14159265358979323846264338327
#include "dbspline.h"
#include "geassign.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
SPIRAL::SPIRAL()
{
	SprialID = NULL;
}

SPIRAL::SPIRAL(double startn,double starte,double startfwj,double a,
			   double startr,double endr,int lr)
{
	SprialID = NULL;
	Set(startn,starte,startfwj,a,startr,endr,lr);
}

void SPIRAL::Set(double startn,double starte,double startfwj,double a,
				 double startr,double endr,int lr,int iColor)
{
	SprialID = NULL;

	StartN   = startn;
	StartE   = starte;
	StartFwj = startfwj;

	A = a;
	StartR = startr;
	EndR   = endr;

	StartL = A*A / StartR;
	EndL   = A*A / EndR;

	LeftOrRight = lr;
	m_iColor    = iColor;

	double x,y,sita,dist0;
	x = spiral_x(A,StartL);
	y = spiral_y(A,StartL);
	dist0 = xyddaa(0,0,x,y,&sita);//此函数在dx近于0 时，返回0.5*PI,一定要注意

	// 参照点方位角及坐标
	BaseFwj = StartFwj-LeftOrRight*(0.5*StartL/StartR);
	BaseN   = StartN-dist0*cos(BaseFwj+LeftOrRight*sita);
	BaseE   = StartE-dist0*sin(BaseFwj+LeftOrRight*sita);

	// 终点坐标及方位角
	GetPoint(EndL,EndN,EndE);
	EndFwj=GetQXFwj(EndL);

	// 2005.07.08 ldf -->
	FormFwj(BaseFwj); 
	FormFwj(StartFwj);
	FormFwj(EndFwj);

	/*
	if(fabs(endr-600.457)<0.1)
	{
	GetPoint(StartL,x,y);
	DRAW();
	ads_printf(L"sl=%lf x=%lf y=%lf El=%lf %lf %lf\n",StartL,x,y,EndL,EndN,EndE);
	}*/


	// <-- 2005.07.08 ldf
}

void SPIRAL::SetByEnd(double endn,double ende,double endfwj,double a,
					  double endr,double startr,int lr,int iColor)
{
	SprialID = NULL;

	EndN   = endn;
	EndE   = ende;
	EndFwj = endfwj;

	A = a;
	EndR = endr;
	StartR   = startr;

	StartL = A*A / StartR;
	EndL   = A*A / EndR;

	LeftOrRight = lr;
	m_iColor    = iColor;

	double x,y,sita,dist0;
	x = spiral_x(A,EndL);
	y = spiral_y(A,EndL);
	dist0 = xyddaa(0,0,x,y,&sita);//此函数在dx近于0 时，返回0.5*PI,一定要注意

	// 参照点方位角及坐标
	BaseFwj = EndFwj-LeftOrRight*(0.5*EndL/EndR);
	BaseN   = EndN-dist0*cos(BaseFwj+LeftOrRight*sita);
	BaseE   = EndE-dist0*sin(BaseFwj+LeftOrRight*sita);

	// 终点坐标及方位角
	GetPoint(StartL,StartN,StartE);
	StartFwj=GetQXFwj(StartL);

	// 2005.07.08 ldf -->
	FormFwj(BaseFwj); 
	FormFwj(StartFwj);
	FormFwj(EndFwj);

	/*
	if(fabs(startr-600.457)<0.1)
	{
	GetPoint(StartL,x,y);
	DRAW();
	ads_printf(L"sl=%lf x=%lf y=%lf El=%lf %lf %lf\n",StartL,x,y,EndL,EndN,EndE);
	}*/


	// <-- 2005.07.08 ldf
}

void SPIRAL::Set2(double endn,double ende,double endfwj,double a,
				  double startr,double endr,int lr,int iColor)
{
	EndN   = endn;
	EndE   = ende;
	EndFwj = endfwj;

	A = a;
	StartR = startr;
	EndR   = endr;

	StartL = A*A / StartR;
	EndL   = A*A / EndR;

	LeftOrRight = lr;
	m_iColor    = iColor;

	double x,y,sita,dist0;
	x = spiral_x(A,EndL);
	y = spiral_y(A,EndL);
	dist0 = xyddaa(0,0,x,y,&sita);//此函数在dx近于0 时，返回0.5*PI,一定要注意

	// 参照点方位角及坐标
	BaseFwj = EndFwj-LeftOrRight*(0.5*EndL/EndR);
	BaseN   = EndN-dist0*cos(BaseFwj+LeftOrRight*sita);
	BaseE   = EndE-dist0*sin(BaseFwj+LeftOrRight*sita);

	// 起点坐标及方位角
	GetPoint(StartL,StartN,StartE);
	StartFwj = GetQXFwj(StartL);

	// 2005.07.08 ldf -->
	FormFwj(BaseFwj); 
	FormFwj(StartFwj);
	FormFwj(EndFwj);
	// <-- 2005.07.08 ldf
}

SPIRAL::~SPIRAL()
{

}

void SPIRAL::CALCULATION()
{

}

// 传入到参照点的曲线长，返回切线方位角
double  SPIRAL::GetQXFwj(double LToBasePt)
{
	double sita;
	if(A>0.0001)
		sita=LToBasePt*LToBasePt/A/A/2.0;
	else
		sita=0.0;
	return BaseFwj+LeftOrRight*sita;
}	

// 传入到参照点的曲线长，返回该点与参照点连线与X轴夹角,及与参照点的距离（局部坐标系下）
double SPIRAL::GetPtJiaoByX(double LToBasePt,double &sita)
{
	if(LToBasePt<0.0)return 0.0;
	double x,y,dist0;
	x=spiral_x(A,LToBasePt);
	y=spiral_y(A,LToBasePt);
	dist0=xyddaa(0,0,x,y,&sita);//
	return dist0;
}	

// 传入到参照点的曲线长，返回坐标
bool SPIRAL::GetPoint(double LToBasePt,double &N,double &E)
{
	if(LToBasePt<0.0)return false;
	double sita,dist0;
	dist0=GetPtJiaoByX(LToBasePt,sita);//
	N=BaseN+dist0*cos(BaseFwj+LeftOrRight*sita);
	E=BaseE+dist0*sin(BaseFwj+LeftOrRight*sita);
	return true;
}	

// 绘制缓和曲线 2005.07.15 ldf
void SPIRAL::DRAW()
{	
	// 定义一个顶点数组及临时点变量
	double N,E;
	AcGePoint2dArray VerticsArray;
	AcGePoint2d Dot;

	// 清空顶点数组
	int ArraySize = VerticsArray.length();
	if(ArraySize > 0)
	{
		VerticsArray.removeSubArray(0,ArraySize-1);
	}

	// 将缓和曲线的顶点加入到顶点数组中
	for(double l=StartL; l<EndL; l+=SPIRALSTEP)
	{
		GetPoint(l,N,E);
		Dot.set(E,N);
		VerticsArray.append(Dot);
	}
	GetPoint(EndL,N,E);
	Dot.set(E,N);
	VerticsArray.append(Dot);
	m_iColor = 4;
	makepolyline(VerticsArray, SprialID, m_iColor);
}

/*
SPIRAL::SPIRAL(double AA,double RR,AcGePoint3d ssPt0,double fwj,double sstartl,int lr)
{
	A=AA;
	R=RR;
	sPt0=ssPt0;
	startl=sstartl;
	endl=A*A/R;
	lrRot=lr;
	Buchang=5;//0.05*R;
	fx0=fwj;
	pObjAy.NUM=0;pObjAy.OBJ1=NULL;pObjAy.OBJ2=NULL;
	double tempX,tempY;
	AcGePoint3d temPt=getPoint(AA*AA/RR);
	endX0=temPt.x;
	endY0=temPt.y;
}*/


/*
SPIRAL::SPIRAL(double AA,double RR,double x,double y,double fwj,double sstartl,int lr)
{
	A=AA;
	R=RR;
	sPt0.x=x;
	sPt0.y=y;
	sPt0.z=0;
	startl=sstartl;
	endl=A*A/R;
	lrRot=lr;
	Buchang=5;//0.05*R;
	fx0=fwj;
	pObjAy.NUM=0;pObjAy.OBJ1=NULL;pObjAy.OBJ2=NULL;
}

SPIRAL::SPIRAL(AcGePoint3d sPt,AcGePoint3d ePt,double sa,double ea,double R1,double R2,double L,int ZRflag)//左偏为-1
{
	startX0=sPt.x;
	startY0=sPt.y;
	endX0=ePt.x;
	endY0=ePt.y;
	double X0,Y0,sita,dist0;
	double Rmin=R1<R2?R1:R2;
	Buchang=0.001*Rmin;
	R=R1*R2/fabs(R1-R2);
	A=sqrt(L*R);
	startl=A*A/R1;
	endl=A*A/R2;
	lrRot=ZRflag;
	
	fx0=sa-ZRflag*0.5*startl/R1;//R＝无穷大处的切线方向
	Y0=spiral_x(A,startl);//局部坐标
	X0=lrRot*spiral_y(A,startl);//缓和曲线起点，不一定是R无穷大
	dist0=xyddaa(0,0,Y0,X0,&sita);	//缓和曲线起点到R无穷大处（虚）之距
	sPt0.x=sPt.x-dist0*sin(fx0+sita);//缓和曲线半径无穷大处真实坐标
	sPt0.y=sPt.y-dist0*cos(fx0+sita);
	startl=0;
	huanhequxian();
	acutPrintf(L"fx01=%lf,sptx=%lf,spty=%lf\n",fx0,sPt0.x,sPt0.y);
	
	fx0=ea-ZRflag*0.5*endl/R2;//R＝无穷大处的切线方向
	Y0=spiral_x(A,endl);//局部坐标
	X0=lrRot*spiral_y(A,endl);//缓和曲线起点，不一定是R无穷大
	dist0=xyddaa(0,0,Y0,X0,&sita);	//缓和曲线起点到R无穷大处（虚）之距
	sPt0.x=ePt.x-dist0*sin(fx0+sita);//缓和曲线半径无穷大处真实坐标
	sPt0.y=ePt.y-dist0*cos(fx0+sita);
	
	acutPrintf(L"fx02=%lf,sptx=%lf,spty=%lf\n",fx0,sPt0.x,sPt0.y);
//	startl=0;
//	huanhequxian();
	
	
	//////////////////////////////////////////////////////////////////////////
	double l=startl;double aj=l*0.5/R;
	double x=spiral_x(A,l);double y=spiral_y(A,l);
	double a0=fx0;
	double t1=x-y/tan(aj);double t2=y/sin(aj);double az=a0-aj*lrRot;
	double xm,ym;
	xm=sPt.x-t2*sin(az);
	ym=sPt.y-t2*cos(az);
	sPt0.x=xm-t1*sin(a0);
	sPt0.y=ym-t1*cos(a0);
//	startl=0;
//	huanhequxian();	
}

SPIRAL::SPIRAL()
{
	
}
SPIRAL::~SPIRAL()
{
	if(pspir)delete []pspir;

//	if(pObjAy.OBJ1)delete pObjAy.OBJ1;
//	if(pObjAy.OBJ2)delete pObjAy.OBJ2;
//	if(pObjAy.OBJ3)delete pObjAy.OBJ3;
}

void SPIRAL::huanhequxian()
{
	double l;
	int i;
	double x,y,x1,y1,sita,dist0;

	pspir=new double[2*(endl-startl)/Buchang+10];
	
	for(i=0,l=startl;l<=endl;l+=Buchang,i++)
	{
		y=spiral_x(A,l);
		x=lrRot*spiral_y(A,l);
		dist0=xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		pspir[2*i]=sPt0.x+x1;
		pspir[2*i+1]=sPt0.y+y1;
	}
	l=endl;
	y=spiral_x(A,l);
	x=lrRot*spiral_y(A,l);
	dist0=xyddaa(0,0,y,x,&sita);//方位角
	
	x1=dist0*sin(fx0+sita);
	y1=dist0*cos(fx0+sita);
	pspir[2*i]=sPt0.x+x1;
	pspir[2*i+1]=sPt0.y+y1;
	i++;
	NUM=i;	
}*/


/*
Adesk::Boolean  SPIRAL::worldDraw(AcGiWorldDraw* pWd)
{   
	huanhequxian();
	if(pObjAy.NUM==1)Revise(pObjAy.OBJ1);
	if(pObjAy.NUM==2){Revise(pObjAy.OBJ1);Revise(pObjAy.OBJ2);}
	AcGePoint3d *verts=new AcGePoint3d[NUM];
	for (int i = 0; i < NUM; i++) 
	{
		verts[i].x=pspir[2*i]; verts[i].y=pspir[2*i+1];verts[i].z=0.0;
	}
	if (pWd!= NULL) 
	{
//		pWd->subEntityTraits().setSelectionMarker(1);
//		pWd->subEntityTraits().setLayer(DmxId);//先设层
		pWd->subEntityTraits().setLineWeight(AcDb::kLnWt000);//先设层
		pWd->subEntityTraits().setColor(6);	
		pWd->geometry().polyline(NUM,verts);
	} 
	else
	{
		assert(Adesk::kFalse);//?	
	}
	if(verts) delete [] verts;
	
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

void SPIRAL::Revise_EndPoint(AcGePoint3d ENDPOINT)
{
	pspir[2*(NUM-1)]=ENDPOINT.x;
	pspir[2*(NUM-1)+1]=ENDPOINT.y;
}

void SPIRAL::AddReviseItem(AcDbObject* obj)
{
	pObjAy.NUM+=1;
	if(pObjAy.NUM==1)pObjAy.OBJ1=obj;
	if(pObjAy.NUM==2)pObjAy.OBJ2=obj;
}

void SPIRAL::Revise(AcDbObject* pObj)
{
//	acedAlert(L"revise");
	
	AcDbObjectId objId;	
	acdbOpenObject(pObj,objId,AcDb::kForWrite);
	if(pObj->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *Line=AcDbLine::cast(pObj);
		ReviseLine(Line);
	}
	else if(pObj->isKindOf(AcDbCircle::desc()))
	{
		AcDbCircle *Circle=AcDbCircle::cast(pObj);
		ReviseCircle(Circle);
	}
	pObj->close();
}

void SPIRAL::ReviseLine(AcDbLine *Line)//修正缓和曲线起终点
{
	AcGePoint3d STARTPOINT(pspir[0],pspir[1],0);
	AcGePoint3d ENDPOINT(pspir[2*(NUM-1)],pspir[2*(NUM-1)+1],0);
	
	AcGePoint3d SNearPt,ENearPt;//起终最近点
	double SPtDist,EPtDist;//起终点到各自最近点之距
	
	SPtDist=LineNearestPoint(STARTPOINT,Line,SNearPt);
	EPtDist=LineNearestPoint(ENDPOINT,Line,ENearPt);

	if(SPtDist<=EPtDist&&SPtDist<0.1)
	{
//		acedAlert(L"reviselinestart");
		pspir[0]=SNearPt.x;pspir[1]=SNearPt.y;
	}
	if(EPtDist<=SPtDist&&EPtDist<0.1)
	{
//		acedAlert(L"reviselineend");
		pspir[2*(NUM-1)]=ENearPt.x;pspir[2*(NUM-1)+1]=ENearPt.y;
	}
}
void SPIRAL::ReviseCircle(AcDbCircle *Circle)//修正缓和曲线起终点
{

	AcGePoint3d STARTPOINT(pspir[0],pspir[1],0);
	AcGePoint3d ENDPOINT(pspir[2*(NUM-1)],pspir[2*(NUM-1)+1],0);
	AcGePoint3d SNearPt,ENearPt;//起终最近点
	double SPtDist,EPtDist;//起终点到各自最近点之距
	SPtDist=CircleNearestPoint(STARTPOINT,Circle,SNearPt);
	EPtDist=CircleNearestPoint(ENDPOINT,Circle,ENearPt);
	
	if(SPtDist<=EPtDist&&SPtDist<0.1)
	{
//		acedAlert(L"ReviseCirclest");
// 		acutPrintf(L"dist=%lf\n",SPtDist);
		pspir[0]=SNearPt.x;pspir[1]=SNearPt.y;
	}
	if(EPtDist<=SPtDist&&EPtDist<0.1)
	{
//		acedAlert(L"ReviseCirclend");
//		acutPrintf(L"dist=%lf\n",EPtDist);
		pspir[2*(NUM-1)]=ENearPt.x;pspir[2*(NUM-1)+1]=ENearPt.y;
	}
}

void SPIRAL::worldDraw(int mode)
{
	/////////////////////////////////////////////	
	huanhequxian();
	if(pObjAy.NUM==1)Revise(pObjAy.OBJ1);
	if(pObjAy.NUM==2){Revise(pObjAy.OBJ1);Revise(pObjAy.OBJ2);}
	
	AcGePoint3dArray fitPoints;
	for (int i = 0; i < NUM; i++)fitPoints.append(AcGePoint3d(pspir[2*i],pspir[2*i+1],0));
	SPLINE=new AcDbSpline(fitPoints,26,0);
	SPLINE->setColorIndex(1);
	if(mode==0)AddEntityToDbs(SPLINE);
}*/




/*
Acad::ErrorStatus
SPIRAL::intersectWith(
    const AcDbEntity* ent,
    AcDb::Intersect intType,
    AcGePoint3dArray& points,
    int / *thisGsMarker* /,
    int / *otherGsMarker* /) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    if (ent == NULL)
        return Acad::eNullEntityPointer;
///////////////////////////////////
	intType=AcDb::kExtendThis;
	double dist=0.001;
    if (ent->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *pline=new AcDbLine(AcGePoint3d(pspir[0]-dist*cos(0.5*Pai),pspir[1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[0]+dist*cos(0.5*Pai),pspir[1]+dist*sin(0.5*Pai),0));
		es=pline->intersectWith(ent,intType,points);
		delete pline;
		//		points[0].x=pspir[0];points[0].y=pspir[1];points[0].z=0;
	}
    else
	{
		AcDbLine *pline=new AcDbLine(AcGePoint3d(pspir[2*(NUM-1)]-dist*cos(0.5*Pai),pspir[2*(NUM-1)+1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[2*(NUM-1)]+dist*cos(0.5*Pai),pspir[2*(NUM-1)+1]+dist*sin(0.5*Pai),0));
		es=pline->intersectWith(ent,intType,points);
		if(points.length()<1)
		{
			pline=new AcDbLine(AcGePoint3d(pspir[0]-dist*cos(0.5*Pai),pspir[1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[0]+dist*cos(0.5*Pai),pspir[1]+dist*sin(0.5*Pai),0));
			es=pline->intersectWith(ent,intType,points);
		}		
		delete pline;
	}*/

//////////////////////////////create pline/////////////
/*
	AcGePoint3dArray vertices;
	double l;
	int i;
	double x,y,x1,y1,sita,dist0,X,Y;
	BasicCal ob;
	for(i=0,l=startl;l<=endl;l+=Buchang,i++)
	{
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		vertices.append(AcGePoint3d(X,Y,0));
	}
	l=endl;
	y=ob.spiral_x(A,l);
	x=lrRot*ob.spiral_y(A,l);
	dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
	
	x1=dist0*sin(fx0+sita);
	y1=dist0*cos(fx0+sita);
	X=sPt0.x+x1;
	Y=sPt0.y+y1;
	vertices.append(AcGePoint3d(X,Y,0));
	i++;
	
	AcDb3dPolyline pPolyLine(AcDb::k3dQuadSplinePoly/ *k3dSimplePoly* /,vertices);
	vertices.removeSubArray(0,i-1);
	es=ent->intersectWith(&pPolyLine,intType,points);
*/
//////////////////////////////////////////////////////////////////////////
/*
		double l;
		double x,y,x1,y1,sita,dist0,X,Y;
		BasicCal ob;
		
		l=startl;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt1(X,Y,0);
		
		l=startl+0.1;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt2(X,Y,0);
		
		AcDbLine sLine(Pt1,Pt2);
	//////////////////////////////////////////////////////////////////////////
		l=endl;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt3(X,Y,0);
		
		l=endl-0.1;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt4(X,Y,0);
		
		AcDbLine eLine(Pt3,Pt4);
		es=ent->intersectWith(&sLine,intType,points);
		es=ent->intersectWith(&eLine,intType,points);*/
		
/*
////////////////////
	return es;
}
*/


/*
Acad::ErrorStatus
SPIRAL::intersectWith(
    const AcDbEntity* ent,
    AcDb::Intersect intType,
    const AcGePlane& projPlane,
    AcGePoint3dArray& points,
    int / *thisGsMarker* /,
    int / *otherGsMarker* /) const
{
    assertReadEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    if (ent == NULL)
        return Acad::eNullEntityPointer;
//////////////////////////
	intType=AcDb::kExtendThis;
	double dist=0.001;
    if (ent->isKindOf(AcDbLine::desc()))
	{
		AcDbLine *pline=new AcDbLine(AcGePoint3d(pspir[0]-dist*cos(0.5*Pai),pspir[1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[0]+dist*cos(0.5*Pai),pspir[1]+dist*sin(0.5*Pai),0));
		es=pline->intersectWith(ent,intType,projPlane,points);
		delete pline;
		//		points[0].x=pspir[0];points[0].y=pspir[1];points[0].z=0;
	}
    else
	{
		AcDbLine *pline=new AcDbLine(AcGePoint3d(pspir[2*(NUM-1)]-dist*cos(0.5*Pai),pspir[2*(NUM-1)+1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[2*(NUM-1)]+dist*cos(0.5*Pai),pspir[2*(NUM-1)+1]+dist*sin(0.5*Pai),0));
		es=pline->intersectWith(ent,intType,projPlane,points);
		if(points.length()<1)
		{
			pline=new AcDbLine(AcGePoint3d(pspir[0]-dist*cos(0.5*Pai),pspir[1]-dist*sin(0.5*Pai),0),AcGePoint3d(pspir[0]+dist*cos(0.5*Pai),pspir[1]+dist*sin(0.5*Pai),0));
			es=pline->intersectWith(ent,intType,projPlane,points);
		}		
		delete pline;
	}*/


/////////////////////////////create pline////////////
/*
	AcGePoint3dArray vertices;
	double l;
	int i;
	double x,y,x1,y1,sita,dist0,X,Y;
	BasicCal ob;
	for(i=0,l=startl;l<=endl;l+=Buchang,i++)
	{
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		vertices.append(AcGePoint3d(X,Y,0));
	}
	l=endl;
	y=ob.spiral_x(A,l);
	x=lrRot*ob.spiral_y(A,l);
	dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
	
	x1=dist0*sin(fx0+sita);
	y1=dist0*cos(fx0+sita);
	X=sPt0.x+x1;
	Y=sPt0.y+y1;
	vertices.append(AcGePoint3d(X,Y,0));
	i++;
	AcDb3dPolyline pPolyLine(AcDb::k3dQuadSplinePoly/ *k3dSimplePoly* /,vertices);
	vertices.removeSubArray(0,i-1);
	es=ent->intersectWith(&pPolyLine,intType,projPlane,points);
*/
//////////////////////////////////////////////////////////////////////////
/*
		double l;
		double x,y,x1,y1,sita,dist0,X,Y;
		BasicCal ob;
		
		l=startl;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt1(X,Y,0);
		
		l=startl+0.1;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt2(X,Y,0);
		
		AcDbLine sLine(Pt1,Pt2);
		//////////////////////////////////////////////////////////////////////////
		l=endl;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt3(X,Y,0);
		
		l=endl-0.1;
		y=ob.spiral_x(A,l);
		x=lrRot*ob.spiral_y(A,l);
		dist0=ob.xyddaa(0,0,y,x,&sita);//方位角
		x1=dist0*sin(fx0+sita);
		y1=dist0*cos(fx0+sita);
		X=sPt0.x+x1;
		Y=sPt0.y+y1;
		AcGePoint3d Pt4(X,Y,0);
		
		AcDbLine eLine(Pt3,Pt4);
		es=ent->intersectWith(&sLine,intType,points);
		es=ent->intersectWith(&eLine,intType,points);	*/
//////////////////////////////////////////////////////////////////////////
	
/*
    return es;
}

Acad::ErrorStatus
SPIRAL::dwgInFields(AcDbDwgFiler* filer)
{
    assertWriteEnabled();
    Acad::ErrorStatus es;
    if ((es = AcDbEntity::dwgInFields(filer)) != Acad::eOk) 
    {
        return es;
    }
    
////////////////
	filer->readDouble(&A);
	filer->readDouble(&R);
	filer->readPoint3d(&sPt0);
	filer->readDouble(&startl);
	filer->readDouble(&endl);
	filer->readInt16(&lrRot);
	filer->readDouble(&Buchang);
	filer->readDouble(&fx0);
///////////////////////
    return filer->filerStatus();
}*/

/*

Acad::ErrorStatus
SPIRAL::dwgOutFields(AcDbDwgFiler* filer) const
{
    assertReadEnabled();
    Acad::ErrorStatus es;

    if ((es = AcDbEntity::dwgOutFields(filer))
        != Acad::eOk)
    {
        return es;
    }
////////////////
	filer->writeDouble(A);
	filer->writeDouble(R);
	filer->writePoint3d(sPt0);
	filer->writeDouble(startl);
	filer->writeDouble(endl);
	filer->writeInt16(lrRot);
	filer->writeDouble(Buchang);
	filer->writeDouble(fx0);
///////////////////////
     return filer->filerStatus();
}


Acad::ErrorStatus
SPIRAL::dxfInFields(AcDbDxfFiler* filer)
{
    assertWriteEnabled();
    Acad::ErrorStatus es = Acad::eOk;
    struct resbuf rb;

    if ((AcDbEntity::dxfInFields(filer) != Acad::eOk)
        || !filer->atSubclassData(L"SPIRAL_PENG"))
    {
        return filer->filerStatus();
    }
//////////////////////////////////////////////////////////////////////////
	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		A= rb.resval.rreal;

	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		R= rb.resval.rreal;

	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfXCoord)
		sPt0= asPnt3d(rb.resval.rpoint);
	
	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		startl= rb.resval.rreal;

	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		endl= rb.resval.rreal;
	
	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfInt16)
		lrRot= rb.resval.rint;

	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		Buchang= rb.resval.rreal;
	
	es = filer->readItem(&rb);
	if  ( es == Acad::eOk&&rb.restype == AcDb:: kDxfReal)
		fx0= rb.resval.rreal;
//////////////////////////////////////////////////////////////////////////
    return es;
}

Acad::ErrorStatus
SPIRAL::dxfOutFields(AcDbDxfFiler* filer) const
{
    assertReadEnabled();
    Acad::ErrorStatus es;

    if ((es = AcDbEntity::dxfOutFields(filer))
        != Acad::eOk)
    {
        return es;
    }
    filer->writeItem(AcDb::kDxfSubclass, "SPIRAL_PENG");
//////////////////////////////////////////////////////////////////////////
	filer->writeDouble(AcDb::kDxfReal,A);
	filer->writeDouble(AcDb::kDxfReal,R);
	filer->writePoint3d(AcDb::kDxfXCoord,sPt0);
	filer->writeDouble(AcDb::kDxfReal,startl);
	filer->writeDouble(AcDb::kDxfReal,endl);
	filer->writeInt16(AcDb::kDxfInt16,lrRot);
	filer->writeDouble(AcDb::kDxfReal,Buchang);
	filer->writeDouble(AcDb::kDxfReal,fx0);
//////////////////////////////////////////////////////////////////////////
     return filer->filerStatus();
}

Acad::ErrorStatus
SPIRAL::transformBy(const AcGeMatrix3d& xform)
{
     return Acad::eOk;
}
///修剪
void SPIRAL::TrimARC(AcDbArc *pArc)
{
	int TriMode=0;//圆弧与缓和曲线走向一致
	AcGePoint3d FirstPt=getPoint(1-1);//0-base
	AcGePoint3d FirstNearPt;
	double dist1=ArcNearestPoint(FirstPt,pArc,FirstNearPt);
	AcGePoint3d EndPt=getPoint(logicalLength()-1);//0-base
	AcGePoint3d EndNearPt;
	double dist2=ArcNearestPoint(EndPt,pArc,EndNearPt);
//	AcDbLine *l1=new AcDbLine(FirstPt,pArc->center());
//	AcDbLine *l2=new AcDbLine(EndPt,pArc->center());
//	AddEntityToDbs(l1);
//	AddEntityToDbs(l2);
	////////////////
	if(dist1<dist2)
	{
		if(dist1>0.01){acutPrintf(L"dist1=%lf,R1=%lf\n",dist1,pArc->radius());return;}
		ads_point p1,p2;
		p1[X]=pArc->center().x;p1[Y]=pArc->center().y;p1[Z]=pArc->center().z;
		p2[X]=FirstPt.x;p2[Y]=FirstPt.y;p2[Z]=FirstPt.z;
		double jiao=ads_angle(p1,p2);
		if(TriMode==0)
		{
			if(lrRot<0)pArc->setEndAngle(jiao);
			else pArc->setStartAngle(jiao);		
		}
		else
		{
			if(lrRot<0)pArc->setStartAngle(jiao);
			else pArc->setEndAngle(jiao);	
		}			
	}
	else
	{
//		AcDbLine *L=new AcDbLine(getPoint(logicalLength()-3),EndPt);
////		double jiao=angleX(L)+lrRot*0.5*Pai;
//		delete L;
//		double jiao=FwjtoAngX(getFwj(logicalLength()-1))+lrRot*0.5*Pai;	
		if(dist2>0.01){acutPrintf(L"dist2=%lf,R2=%lf\n",dist2,pArc->radius());return;}
		ads_point p1,p2;
		p1[X]=pArc->center().x;p1[Y]=pArc->center().y;p1[Z]=pArc->center().z;
		p2[X]=EndPt.x;p2[Y]=EndPt.y;p2[Z]=EndPt.z;
		double jiao=ads_angle(p1,p2);		
		if(TriMode==0)
		{
			if(lrRot<0)pArc->setStartAngle(jiao);
			else pArc->setEndAngle(jiao);	
		}
		else
		{
			if(lrRot<0)pArc->setEndAngle(jiao);
			else pArc->setStartAngle(jiao);		
		}
	}
}

void SPIRAL::TrimLine(AcDbLine *pLine)
{
	int TriMode=0;//直线与缓和曲线走向一致
	AcGePoint3d ThirdPt=getPoint(3-1);//0-base
	AcGePoint3d FirstPt=getPoint(1-1);//0-base
	AcDbLine *pL=new AcDbLine(FirstPt,ThirdPt);
	double fwj1=fwj(pL);delete pL;
	double fwj2=fwj(pLine);
	pLine->assertWriteEnabled();
	if(fabs(fwj1-fwj2)<3.0)
	{
		if(TriMode==0)pLine->setEndPoint(FirstPt);//the same direct
		else pLine->setStartPoint(FirstPt);
	}
	else 
	{
		if(TriMode==0)pLine->setStartPoint(FirstPt);
		else pLine->setEndPoint(FirstPt);
	}
}

int SPIRAL::logicalLength()
{
	return (int)((endl-startl)/Buchang+1);
}

AcGePoint3d SPIRAL::getPoint(int i)//0-base
{
	double l;
	double x,y,x1,y1,sita,dist0;
	AcGePoint3d Pt;
	l=startl+i*Buchang;
	y=spiral_x(A,l);
	x=lrRot*spiral_y(A,l);
	dist0=xyddaa(0,0,y,x,&sita);//方位角
	x1=dist0*sin(fx0+sita);
	y1=dist0*cos(fx0+sita);
	Pt.x=sPt0.x+x1;
	Pt.y=sPt0.y+y1;
	Pt.z=0;
	return Pt;
}

AcGePoint3d SPIRAL::getPoint(double slc)//0-base
{
	double l;
	double x,y,x1,y1,sita,dist0;
	AcGePoint3d Pt;
	l=slc;
	y=spiral_x(A,l);
	x=lrRot*spiral_y(A,l);
	dist0=xyddaa(0,0,y,x,&sita);//方位角
	x1=dist0*sin(fx0+sita);
	y1=dist0*cos(fx0+sita);
	Pt.x=sPt0.x+x1;
	Pt.y=sPt0.y+y1;
	Pt.z=0;
	return Pt;
}

double SPIRAL::getFwj(int i)//0-base
{
	double l;
	double x,y,sita,dist0;
	AcGePoint3d Pt;
	l=startl+i*Buchang;
	y=spiral_x(A,l);
	x=lrRot*spiral_y(A,l);
	dist0=xyddaa(0,0,y,x,&sita);//方位角
	return sita;
}

void SPIRAL::GetItem(double &AA,double &RR,AcGePoint3d &ssPt0,double &fwj,double &sstartl,int &lr)
{
	huanhequxian();
	if(pObjAy.NUM==1)Revise(pObjAy.OBJ1);
	if(pObjAy.NUM==2){Revise(pObjAy.OBJ1);Revise(pObjAy.OBJ2);}
//	acedAlert(L"getitem");
	AA=A;
	RR=R;
	ssPt0=sPt0;
	fwj=fx0;
	sstartl=startl;
	lr=lrRot;
	acutPrintf(L"A=%lf,R=%lf,fx0=%lf,startl=%lf,lrRot=%d\n",A,R,fx0,startl,lrRot);
	
//	acedAlert(L"getitemend");	
}
*/


