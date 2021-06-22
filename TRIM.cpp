// TRIM.cpp: implementation of the TRIM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TRIM.h"
#include "PmPointer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "dbxline.h"
extern CStringArray TKLCAy;
#define TRIMVERSION 20040220
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void TRIM_ROAD::SetEndPoint(AcGePoint3d mPt)
{
	assertWriteEnabled();
	MidPt[NMidPt-1]=mPt;
}

TRIM_ROAD::TRIM_ROAD(double OuterHigh,double dh,double ddtk,double dspace,double scale)
{
	//	LPt=NULL;RPt=NULL;
	TUSCALE=scale;
	NMidPt=0;
	High_Outer=OuterHigh;
	dSPACE=dspace;
	dH=dh;
	High=High_Outer-2.0*(dH+dSPACE);
	dTK=ddtk;		
}

TRIM_ROAD::~TRIM_ROAD()
{
}

void TRIM_ROAD::AddFrame(AcGePoint3d mPt)
{
	assertWriteEnabled();
	MidPt[NMidPt++]=mPt;
}

void TRIM_ROAD::DelEndFrame()
{
	assertWriteEnabled();
	if(NMidPt>1)NMidPt--;
}

void TRIM_ROAD::CreateFrame()
{
	//	LPt=new AcGePoint3d[NMidPt];
	//	RPt=new AcGePoint3d[NMidPt];
	AcGePoint3d lpt,rpt;
	int i;
	for( i=1;i<NMidPt;i++)jiao[i]=angleX(MidPt[i-1],MidPt[i]);
	jiao[0]=jiao[1];
	for(i=0;i<NMidPt;i++)
	{
		LPt[i].x=MidPt[i].x+0.5*High*cos(jiao[i]+0.5*Pai);
		LPt[i].y=MidPt[i].y+0.5*High*sin(jiao[i]+0.5*Pai);
		LPt[i].z=0.0;

		RPt[i].x=MidPt[i].x+0.5*High*cos(jiao[i]-0.5*Pai);
		RPt[i].y=MidPt[i].y+0.5*High*sin(jiao[i]-0.5*Pai);
		RPt[i].z=0.0;

	}
	for(i=1;i<NMidPt-1;i++)//调整
	{
		lpt.x=MidPt[i].x+0.5*High*cos(jiao[i+1]+0.5*Pai);
		lpt.y=MidPt[i].y+0.5*High*sin(jiao[i+1]+0.5*Pai);
		lpt.z=0.0;

		rpt.x=MidPt[i].x+0.5*High*cos(jiao[i+1]-0.5*Pai);
		rpt.y=MidPt[i].y+0.5*High*sin(jiao[i+1]-0.5*Pai);
		rpt.z=0.0;

		AcGePoint3dArray points1,points2;

		AcDbLine LineL1(LPt[i-1],LPt[i]);
		AcDbLine LineL2(lpt,LPt[i+1]);
		double jiao1=angleX(&LineL1);
		double jiao2=angleX(&LineL2);
		if(lpt.distanceTo(LPt[i+1])>0.01&&LPt[i-1].distanceTo(LPt[i])>0.01
			&&fabs(fabs(jiao1-jiao2)-Pai)>0.0001&&fabs(jiao1-jiao2)>0.0001)//保证不平行
			//用于求交的两条直线不能重合
		{
			if(Acad::eOk==LineL1.intersectWith(&LineL2,AcDb::kExtendBoth,points1))
			{
				LPt[i]=points1[0];
			}
			else LPt[i]=lpt;
		}
		else LPt[i]=lpt;

		AcDbLine LineR1(RPt[i-1],RPt[i]);
		AcDbLine LineR2(rpt,RPt[i+1]);
		jiao1=angleX(&LineR1);
		jiao2=angleX(&LineR2);

		if(rpt.distanceTo(RPt[i+1])>0.01&&RPt[i-1].distanceTo(RPt[i])>0.01
			&&fabs(fabs(jiao1-jiao2)-Pai)>0.0001&&fabs(jiao1-jiao2)>0.0001)
		{
			if(Acad::eOk==LineR1.intersectWith(&LineR2,AcDb::kExtendBoth,points2))
			{
				RPt[i]=points2[0];
			}
			else RPt[i]=rpt;
		}
		else RPt[i]=rpt;
	}
}

void TRIM_ROAD::CreateResBox(AcGePoint3d OriPt)
{
	resBOX[1].LDPt.set(OriPt.x,OriPt.y,0);
	resBOX[1].LTPt.set(OriPt.x,OriPt.y+High,0);
	double dist1=LPt[0].distanceTo(LPt[1]);
	double dist2=RPt[0].distanceTo(RPt[1]);
	resBOX[1].RTPt.set(OriPt.x+dist1,OriPt.y+High,0);
	resBOX[1].RDPt.set(OriPt.x+dist2,OriPt.y,0);
	resBOX[1].XZjiao=jiao[1];
	GetBoxCenter(resBOX[1]);
	//	DrawBox(resBOX[1]);
	double resBoxHigh=resBOX[1].LTPt.y;
	AcGePoint3d CenTpt,CenBpt;
	double offDt1,offDt2;
	offDt1 = dist1;
	offDt2 = dist2;

	for(int i=2;i<NMidPt;i++)
	{
		double B=jiao[i-1]-jiao[i];
		if(B<0)B+=2*pi;
		if(B>pi)B=2*pi-B;
		B=pi-B;
		B=0.5*B;//框顶边与公共边的夹角
		double dd=dTK+2*(dH+dSPACE)/tan(B);//展直后两框最小间距
		if(offDt1>offDt2)
		{
			resBOX[i].XZjiao=jiao[i];
			resBOX[i].LTPt.x=resBOX[i-1].RTPt.x+dd;
			resBOX[i].LTPt.y=resBoxHigh;
			resBOX[i].LTPt.z=0;
			AcGeMatrix3d xformR,xformT;
			AcGeVector3d norm(0,0,1);
			AcGeVector3d  moveBy(resBOX[i].LTPt.x-LPt[i-1].x,resBOX[i].LTPt.y-LPt[i-1].y,0);
			xformR.setToRotation(-1.0*jiao[i],norm,resBOX[i].LTPt);
			xformT.setToTranslation(moveBy);
			resBOX[i].LDPt = RPt[i-1]; 
			resBOX[i].LDPt=resBOX[i].LDPt.transformBy(xformT);
			resBOX[i].LDPt=resBOX[i].LDPt.transformBy(xformR);
			CenTpt.x = resBOX[i-1].RTPt.x+0.5*dd;//折图竖直线一点
			CenTpt.y = resBOX[i-1].RTPt.y;//折图竖直线一点
			CenBpt.x = CenTpt.x;
			CenBpt.y = CenTpt.y - High;			
		}
		else
		{
			resBOX[i].XZjiao=jiao[i];
			resBOX[i].LDPt.x=resBOX[i-1].RDPt.x+dd;
			resBOX[i].LDPt.y=OriPt.y;
			resBOX[i].LDPt.z=0;
			AcGeMatrix3d xformR,xformT;
			AcGeVector3d norm(0,0,1);
			AcGeVector3d  moveBy(resBOX[i].LDPt.x-RPt[i-1].x,resBOX[i].LDPt.y-RPt[i-1].y,0);
			xformR.setToRotation(-1.0*jiao[i],norm,resBOX[i].LDPt);
			xformT.setToTranslation(moveBy);
			resBOX[i].LTPt=LPt[i-1];
			resBOX[i].LTPt=resBOX[i].LTPt.transformBy(xformT);
			resBOX[i].LTPt=resBOX[i].LTPt.transformBy(xformR);

			CenTpt.x =resBOX[i-1].RDPt.x+0.5*dd;
			CenTpt.y = OriPt.y;
			CenBpt.x = CenTpt.x;
			CenBpt.y = CenTpt.y + High;			
		}
		dist1=LPt[i-1].distanceTo(LPt[i]);
		dist2=RPt[i-1].distanceTo(RPt[i]);
		resBOX[i].RDPt.set(resBOX[i].LDPt.x+dist2,OriPt.y,0);
		resBOX[i].RTPt.set(resBOX[i].LTPt.x+dist1,OriPt.y+High,0);
		GetBoxCenter(resBOX[i]);
		//		makeline(CenTpt,CenBpt,0);//折图竖直线
		//		DrawBox(resBOX[i]);

		offDt1 = resBOX[i].RTPt.distanceTo(resBOX[1].LTPt);
		offDt2 = resBOX[i].RDPt.distanceTo(resBOX[1].LDPt);


	}
	boxallIN.LDPt=resBOX[1].LDPt;
	boxallIN.LTPt=resBOX[1].LTPt;
	boxallIN.RTPt=resBOX[NMidPt-1].RTPt;
	boxallIN.RDPt=resBOX[NMidPt-1].RDPt;

	boxallIN.LDPt.y=resBOX[1].LDPt.y-dSPACE;
	boxallIN.LTPt.y=resBOX[1].LTPt.y+dSPACE;
	boxallIN.RTPt.y=resBOX[NMidPt-1].RTPt.y+dSPACE;
	boxallIN.RDPt.y=resBOX[NMidPt-1].RDPt.y-dSPACE;

	//	DrawBox(boxallIN);//内框
	boxallOUT.LDPt.x=boxallIN.LDPt.x-1.5*dH;
	boxallOUT.LDPt.y=boxallIN.LDPt.y-dH;
	boxallOUT.LTPt.x=boxallIN.LTPt.x-1.5*dH;
	boxallOUT.LTPt.y=boxallIN.LTPt.y+dH;
	boxallOUT.RDPt.x=boxallIN.RDPt.x+1.5*dH;
	boxallOUT.RDPt.y=boxallIN.RDPt.y-dH;
	boxallOUT.RTPt.x=boxallIN.RTPt.x+1.5*dH;
	boxallOUT.RTPt.y=boxallIN.RTPt.y+dH;
	/*
	boxallIN.LDPt.y=resBOX[1].LDPt.y-dSPACE;
	boxallIN.LTPt.y=resBOX[1].LTPt.y+dSPACE;
	boxallIN.RTPt.y=resBOX[NMidPt-1].RTPt.y+dSPACE;
	boxallIN.RDPt.y=resBOX[NMidPt-1].RDPt.y-dSPACE;*/

	//	DrawBox(boxallOUT);//外框

}

void TRIM_ROAD::CreateResBox1(AcGePoint3d OriPt)
{
	double afa;
	double l;
	for(int i=1;i<NMidPt;i++)
	{
		afa=angleX(resBOX[i].LDPt,resBOX[i].LTPt);
		l=dSPACE/cos(fabs(afa-0.5*pi));
		//		acutPrintf(L"afa=%lf,l=%lf  ",afa,l);

		resBOX1[i].LTPt.x=resBOX[i].LTPt.x+l*cos(afa);
		resBOX1[i].LTPt.y=resBOX[i].LTPt.y+l*sin(afa);

		resBOX1[i].LDPt.x=resBOX[i].LDPt.x-l*cos(afa);
		resBOX1[i].LDPt.y=resBOX[i].LDPt.y-l*sin(afa);

		afa=angleX(resBOX[i].RDPt,resBOX[i].RTPt);
		l=dSPACE/cos(fabs(afa-0.5*pi));
		//		acutPrintf(L"afa=%lf,l=%lf\n",afa,l);

		resBOX1[i].RTPt.x=resBOX[i].RTPt.x+l*cos(afa);
		resBOX1[i].RTPt.y=resBOX[i].RTPt.y+l*sin(afa);

		resBOX1[i].RDPt.x=resBOX[i].RDPt.x-l*cos(afa);
		resBOX1[i].RDPt.y=resBOX[i].RDPt.y-l*sin(afa);

	}
	/*
	resBOX1[1].LDPt.set(OriPt.x,OriPt.y,0);
	resBOX1[1].LTPt.set(OriPt.x,OriPt.y+High+2.0*dSPACE,0);
	double dist1=LPt[0].distanceTo(LPt[1]);
	double dist2=RPt[0].distanceTo(RPt[1]);
	resBOX1[1].RTPt.set(OriPt.x+dist1,OriPt.y+High+2.0*dSPACE,0);
	resBOX1[1].RDPt.set(OriPt.x+dist2,OriPt.y,0);
	resBOX1[1].XZjiao=jiao[1];
	GetBoxCenter(resBOX1[1]);
	//	DrawBox(resBOX1[1]);
	double resBOX1High=resBOX1[1].LTPt.y;
	AcGePoint3d CenTpt,CenBpt;
	double offDt1,offDt2;
	offDt1 = dist1;
	offDt2 = dist2;

	for(int i=2;i<NMidPt;i++)
	{
	double B=jiao[i-1]-jiao[i];
	if(B<0)B+=2*pi;
	if(B>pi)B=2*pi-B;
	B=pi-B;
	B=0.5*B;//框顶边与公共边的夹角
	double dd=dTK+2*(dH)/tan(B);//展直后两框最小间距
	if(offDt1>offDt2)
	{
	resBOX1[i].XZjiao=jiao[i];
	resBOX1[i].LTPt.x=resBOX1[i-1].RTPt.x+dd;
	resBOX1[i].LTPt.y=resBOX1High+2.0*dSPACE;
	resBOX1[i].LTPt.z=0;
	AcGeMatrix3d xformR,xformT;
	AcGeVector3d norm(0,0,1);
	AcGeVector3d  moveBy(resBOX1[i].LTPt.x-LPt[i-1].x,resBOX1[i].LTPt.y-LPt[i-1].y,0);
	xformR.setToRotation(-1.0*jiao[i],norm,resBOX1[i].LTPt);
	xformT.setToTranslation(moveBy);
	resBOX1[i].LDPt = RPt[i-1]; 
	resBOX1[i].LDPt=resBOX1[i].LDPt.transformBy(xformT);
	resBOX1[i].LDPt=resBOX1[i].LDPt.transformBy(xformR);
	CenTpt.x = resBOX1[i-1].RTPt.x+0.5*dd;//折图竖直线一点
	CenTpt.y = resBOX1[i-1].RTPt.y;//折图竖直线一点
	CenBpt.x = CenTpt.x;
	CenBpt.y = CenTpt.y - High-2.0*dSPACE;			
	}
	else
	{
	resBOX1[i].XZjiao=jiao[i];
	resBOX1[i].LDPt.x=resBOX1[i-1].RDPt.x+dd;
	resBOX1[i].LDPt.y=OriPt.y;
	resBOX1[i].LDPt.z=0;
	AcGeMatrix3d xformR,xformT;
	AcGeVector3d norm(0,0,1);
	AcGeVector3d  moveBy(resBOX1[i].LDPt.x-RPt[i-1].x,resBOX1[i].LDPt.y-RPt[i-1].y,0);
	xformR.setToRotation(-1.0*jiao[i],norm,resBOX1[i].LDPt);
	xformT.setToTranslation(moveBy);
	resBOX1[i].LTPt=LPt[i-1];
	resBOX1[i].LTPt=resBOX1[i].LTPt.transformBy(xformT);
	resBOX1[i].LTPt=resBOX1[i].LTPt.transformBy(xformR);

	CenTpt.x =resBOX1[i-1].RDPt.x+0.5*dd;
	CenTpt.y = OriPt.y;
	CenBpt.x = CenTpt.x;
	CenBpt.y = CenTpt.y + High+2.0*dSPACE;			
	}
	dist1=LPt[i-1].distanceTo(LPt[i]);
	dist2=RPt[i-1].distanceTo(RPt[i]);
	resBOX1[i].RDPt.set(resBOX1[i].LDPt.x+dist2,OriPt.y,0);
	resBOX1[i].RTPt.set(resBOX1[i].LTPt.x+dist1,OriPt.y+High+2.0*dSPACE,0);
	GetBoxCenter(resBOX1[i]);
	//		makeline(CenTpt,CenBpt,0);//折图竖直线
	//		DrawBox(resBOX1[i]);

	offDt1 = resBOX1[i].RTPt.distanceTo(resBOX1[1].LTPt);
	offDt2 = resBOX1[i].RDPt.distanceTo(resBOX1[1].LDPt);
	}*/

}

Adesk::Boolean  TRIM_ROAD::subWorldDraw(AcGiWorldDraw* pWd)
{ 
	//	assertWriteEnabled();
	if(NMidPt>1)
	{
		CreateFrame();
		AcGePoint3d Pt[2];

		pWd->subEntityTraits().setSelectionMarker(1);
		pWd->subEntityTraits().setColor(0);	
		pWd->geometry().polyline(NMidPt,LPt);
		pWd->geometry().polyline(NMidPt,RPt);
		for(int i=0;i<NMidPt;i++)
		{
			Pt[0]=LPt[i];
			Pt[1]=RPt[i];
			pWd->geometry().polyline(2,Pt);
		}
	}
	return ( pWd->regenType()==kAcGiSaveWorldDrawForProxy);
}

Acad::ErrorStatus TRIM_ROAD::subGetGripPoints(AcGePoint3dArray& gripPoints,
											  AcDbIntArray& osnapModes,
											  AcDbIntArray& geomIds)const
{   
	assertReadEnabled();
	//	acutPrintf(L"setgrip");
	// 将直线中点入夹点集，序号0——2*LineAyLen
	for(int i=0;i<NMidPt;i++)
	{
		gripPoints.append(MidPt[i]);//GetLineMidPt(pLine));
	}
	return Acad::eOk;
}

Acad::ErrorStatus TRIM_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset)
{  
	//	acutPrintf(L"moveGripPointsAt");
	assertWriteEnabled();
	AcGePoint3d startPt,endPt,centerPt;
	double dx=0,dy=0;
	for ( int i=0;i<indices.length();i++)
	{ 
		int j=indices[i];
		MidPt[j]+=offset;
		bool IsNear=false;
		for(int k=0;k<NMidPt;k++)
		{
			if(k==j)continue;
			if(MidPt[j].distanceTo(MidPt[k])<0.0001)
			{
				IsNear=true;
				break;
			}
		}
		if(IsNear)MidPt[j]-=offset;
	}
	return Acad::eOk;
}

Acad::ErrorStatus
	TRIM_ROAD::dwgInFields(AcDbDwgFiler* filer)
{
	assertWriteEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbEntity::dwgInFields(filer)) != Acad::eOk) 
	{
		return es;
	} 
	Adesk::Int32 version;
	filer->readInt32(&version);
	if(version!=TRIMVERSION)
	{
		AfxMessageBox(L"版本不正确!");
		return filer->filerStatus();
	}

	filer->readDouble(&TUSCALE);
	filer->readDouble(&High_Outer);
	filer->readDouble(&dSPACE);
	filer->readDouble(&dH);
	filer->readDouble(&dTK);	
	filer->readDouble(&High);
	filer->readInt32(&NMidPt);
	for(int i=0;i<NMidPt;i++)filer->readPoint3d(&MidPt[i]);
	///////////////////////
	return filer->filerStatus();
}

Acad::ErrorStatus
	TRIM_ROAD::dwgOutFields(AcDbDwgFiler* filer) const
{	
	//	acutPrintf(L"out");
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbEntity::dwgOutFields(filer))
		!= Acad::eOk)
	{
		return es;
	}

	Adesk::Int32 version=TRIMVERSION;
	filer->writeInt32(version);

	filer->writeDouble(TUSCALE);
	filer->writeDouble(High_Outer);
	filer->writeDouble(dSPACE);
	filer->writeDouble(dH);
	filer->writeDouble(dTK);	
	filer->writeDouble(High);
	filer->writeInt32(NMidPt);
	for(int i=0;i<NMidPt;i++)filer->writePoint3d(MidPt[i]);
	return filer->filerStatus();
}


Acad::ErrorStatus
	TRIM_ROAD::dxfInFields(AcDbDxfFiler* pFiler)
{
	assertWriteEnabled();
	Acad::ErrorStatus es = Acad::eOk;
	struct resbuf inbuf;

	if ((AcDbEntity::dxfInFields(pFiler) != Acad::eOk)
		|| !pFiler->atSubclassData(L"MYROAD_PENG"))
	{
		return pFiler->filerStatus();
	}
	//////////////////////////////////////////////////////////////////////////
	//1:
	Adesk::Int32 version;
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32)
		{
			version  = inbuf.resval.rlong;
		}
	} 
	if(version!=TRIMVERSION)
	{
		AfxMessageBox(L"版本不正确!");
		return pFiler->filerStatus();
	}
	//2:	
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal )
		{	TUSCALE = inbuf.resval.rreal;	}
		else		{		}
	}
	//3:	
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+1 )
		{	High_Outer = inbuf.resval.rreal;	}
		else		{		}
	}
	//4:
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+2 )
		{	dSPACE = inbuf.resval.rreal;		}
		else		{		}
	}
	//5:	
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+3 )
		{	dH = inbuf.resval.rreal;		}
		else		{		}
	}
	//6:
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+4 )
		{	dTK = inbuf.resval.rreal;		}
		else		{		}
	}
	//7:
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfReal+5 )
		{	High = inbuf.resval.rreal;		}
		else		{		}
	}

	//8:
	es = pFiler->readItem(&inbuf);
	if  ( es == Acad::eOk)
	{
		if ( inbuf.restype == AcDb:: kDxfInt32)
		{
			NMidPt  = inbuf.resval.rint;
		}
		else		{		}
	} 

	//9:  
	for(int i=0;i<NMidPt;i++)
	{
		es = pFiler->readItem(&inbuf);
		if  ( es == Acad::eOk)
		{
			if (inbuf.restype != AcDb::kDxfXCoord) 
				throw AcDb::kDxfXCoord;
			MidPt[i] = asPnt3d(inbuf.resval.rpoint);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return es;
}

Acad::ErrorStatus
	TRIM_ROAD::dxfOutFields(AcDbDxfFiler* filer) const
{
	assertReadEnabled();
	Acad::ErrorStatus es;

	if ((es = AcDbEntity::dxfOutFields(filer))
		!= Acad::eOk)
	{
		return es;
	}
	filer->writeItem(AcDb::kDxfSubclass, "MYROAD_PENG");

	//////////////////////////////////////////////////////////////////////////
	//1:
	Adesk::Int32 version=TRIMVERSION;
	filer->writeInt32(AcDb::kDxfInt32, version);
	//2:	
	filer->writeDouble(AcDb::kDxfReal, TUSCALE);
	//3:	
	filer->writeDouble(AcDb::kDxfReal+1, High_Outer);
	//4:
	filer->writeDouble(AcDb::kDxfReal+2, dSPACE);
	//5:	
	filer->writeDouble(AcDb::kDxfReal+3, dH);
	//6:
	filer->writeDouble(AcDb::kDxfReal+4, dTK);
	//7:
	filer->writeDouble(AcDb::kDxfReal+5, High);
	//8:
	filer->writeInt32(AcDb::kDxfInt32, NMidPt);
	//9:  
	for(int i=0;i<NMidPt;i++)filer->writePoint3d(AcDb::kDxfXCoord, MidPt[i]);
	//////////////////////////////////////////////////////////////////////////
	return filer->filerStatus();
}

Acad::ErrorStatus TRIM_ROAD::subExplode(AcDbVoidPtrArray& entitySet)const 
{   
	assertReadEnabled();
	AcDbObjectId entId;
	AcGePoint3dArray  pArr;
	int i=0,j=0;

	pArr.setLogicalLength(NMidPt*2);
	for (j = 0; j < NMidPt; j++){pArr[i].set(LPt[j].x,LPt[j].y,0);i++;}
	for (j = NMidPt-1; j >= 0; j--){pArr[i].set(RPt[j].x,RPt[j].y,0);i++;}

	AcDb2dPolyline *pPline = new AcDb2dPolyline(AcDb::k2dSimplePoly,pArr,0.0, Adesk::kTrue);
	pPline->setColorIndex(0);
	entitySet.append(pPline);
	return Acad::eOk;	
}

void TRIM_ROAD::GetBoxCenter(Box &box)
{
	double x=box.LDPt.x+box.LTPt.x+box.RDPt.x+box.RTPt.x;
	double y=box.LDPt.y+box.LTPt.y+box.RDPt.y+box.RTPt.y;
	box.Cen.set(x/4.0,y/4.0,0);
}

void TRIM_ROAD::DrawBox(Box resBOX)
{
	AcGePoint3dArray PtArray;
	PtArray.append(resBOX.LDPt);
	PtArray.append(resBOX.LTPt);
	PtArray.append(resBOX.RTPt);
	PtArray.append(resBOX.RDPt);
	makepolyline(PtArray,true,0);//,AcDb::LineWeight lw,char *LayerName, char *LineType);
	//	
	//	makeline(resBOX.LDPt,resBOX.LTPt);
	//	makeline(resBOX.LTPt,resBOX.RTPt);
	//	makeline(resBOX.RTPt,resBOX.RDPt);
	//	makeline(resBOX.RDPt,resBOX.LDPt);
}

void TRIM_ROAD::CreateLC()
{
	TKLCAy.RemoveAll();
	XLDataBase ob;
	ACHAR work[256],pro[256];
	CString SCPRO=L"";
	if(ReadRailWorkdir(work,pro)>0)
	{
		SCPRO=work;
		SCPRO+="\\DATA\\";
		SCPRO+=pro;
		SCPRO+=".mdb";
		//AfxMessageBox(SCPRO);
		bool isSJX=PmPointer::SJXorJYX(SCPRO,6,false);
		if (isSJX)
		{
			if((1!=ob.Read_XLDbs(SCPRO,L"曲线表"))||(1!=ob.Read_XLDbs(SCPRO,L"断链表")))
			{
				TKLCAy.Add(L"NO");
				ob.Release();
				return;			
			}
			double JDarray[MAXJDNUM][6];
			int i;
			for(i=0;i<ob.NQX;i++)
			{
				JDarray[i][0]=ob.QXB[i].jdno;
				JDarray[i][1]=ob.QXB[i].x;
				JDarray[i][2]=ob.QXB[i].y;
				JDarray[i][3]=ob.QXB[i].l1;
				JDarray[i][4]=ob.QXB[i].R;
				JDarray[i][5]=ob.QXB[i].l2;
			}
			GTZX_ROAD gtzx(ob.NQX,JDarray,L"BK",L"JD");
			if (&gtzx==NULL)
			{
				return;
			}
			for(i=0;i<ob.NDL;i++)
			{
				_tcscpy(gtzx.DLB[i].BGH , ob.DLArray[i].BGH);
				_tcscpy(gtzx.DLB[i].EGH , ob.DLArray[i].EGH);
				gtzx.DLB[i].BLC=ob.DLArray[i].BLC;			
				gtzx.DLB[i].DL=ob.DLArray[i].DL;
			}
			gtzx.NDL=ob.NDL;

			gtzx.getpxy_DATA();

			double xout,yout,tylc,xclc,zdtylc,zdxclc;

			CString GH,xlc,xlcchr;
			ACHAR gh[40], Ckml[20];
			zdxclc=ob.DLArray[ob.NDL-1].BLC;
			xlcchr = LCchr(ob.DLArray[ob.NDL-1].BGH, zdxclc, 3);
			_tcscpy(Ckml, xlcchr);
			zdtylc=TYLC1(Ckml,ob.NDL,ob.DLArray);
			xlcchr=L"";
			xout=MidPt[0].y;
			yout=MidPt[0].x;
			tylc=PROJ_ML(gtzx.array,gtzx.tarray,xout,yout,xout,yout);
			if(tylc<0||tylc>zdtylc)tklc[0].eml=L" ";
			else
			{
				xclc=XLC1(tylc,gh,ob.NDL,ob.DLArray);
				xlcchr = LCchr(gh, xclc, 0);
				tklc[0].eml=xlcchr;
			}				
			//		acutPrintf(L"\n%d:%lf\n",0,tylc);
			for(i=1;i<NMidPt;i++)
			{
				xlcchr=L"";
				xout=MidPt[i].y;
				yout=MidPt[i].x;
				tylc=PROJ_ML(gtzx.array,gtzx.tarray,xout,yout,xout,yout);
				if(tylc<0||tylc>zdtylc)tklc[i].eml=" ";
				else
				{

					xclc=XLC1(tylc,gh,ob.NDL,ob.DLArray);
					xlcchr = LCchr(gh, xclc, 0);
					tklc[i].eml=xlcchr;	
				}			
				tklc[i].sml=tklc[i-1].eml;			
				//			acutPrintf(L"\n%d:%lf\n",i,xclc);
			}
		} 
		else
		{
			ACHAR  ckml[160];
			double cml;
			GTZX_JYX_ROAD *gtzx=PmPointer::GetGTSJJX(SCPRO);
			if (gtzx==NULL)
			{
				return;
			}
			gtzx->TrsNEtoCkml(MidPt[0].y,MidPt[0].x,ckml);
			gtzx->TrsCkmltoCml(ckml,cml);
			ACHAR zdckml[160];
			double zdcml;
			_stprintf(zdckml,L"%s%.3f",gtzx->DLB[gtzx->NDL-1].BGH,gtzx->DLB[gtzx->NDL-1].BLC);
			gtzx->TrsCkmltoCml(zdckml,zdcml);
			if(cml<0||cml>zdcml)
				tklc[0].eml="";
			else
				tklc[0].eml.Format(L"%s",ckml);
			for (int i=1;i<NMidPt;i++)
			{
				gtzx->TrsNEtoCkml(MidPt[i].y,MidPt[i].x,ckml);
				gtzx->TrsCkmltoCml(ckml,cml);
				if(cml<0||cml>zdcml)
					tklc[i].eml="";
				else
					tklc[i].eml.Format(L"%s",ckml);
				tklc[i].sml=tklc[i-1].eml;	
			}

		}

		tklc[0].sml=L"YES";
		for(int i=0;i<NMidPt;i++)
		{
			TKLCAy.Add(tklc[i].sml);
			TKLCAy.Add(tklc[i].eml);
		}		
	}
	else 
	{
		TKLCAy.Add(L"NO");
	}
	ob.Release();
}
