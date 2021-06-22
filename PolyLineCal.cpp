// PolyLineCal.cpp: implementation of the PolyLineCal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PolyLineCal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PolyLineCal::PolyLineCal()
{
	Poly=NULL;
	Poly2d=NULL;
	Poly3d=NULL;
}

PolyLineCal::~PolyLineCal()
{
	NodePtArray.removeSubArray(0,NodePtArray.length()-1);
}

void PolyLineCal::SetNearPoint(AcGePoint3d pt)
{
	NearPT=pt;
}

void PolyLineCal::SetPoly(AcDbPolyline *pline)
{
	Poly=pline;
	Poly2d=NULL;
	Poly3d=NULL;
}

void PolyLineCal::Set2dPoly(AcDb2dPolyline *pline)
{
	Poly=NULL;
	Poly2d=pline;
	Poly3d=NULL;
}

void PolyLineCal::Set3dPoly(AcDb3dPolyline *pline)
{
	Poly=NULL;
	Poly2d=NULL;
	Poly3d=pline;
}

void PolyLineCal::UpdateData()
{
	NodePtArray.removeSubArray(0,NodePtArray.length()-1);
	if(Poly)
	{
		AcDbObjectId eId;
		//		if(Acad::eOk==acdbOpenObject(Poly,eId,AcDb::kForRead))
		{
			int num=Poly->numVerts();
			for(int i=0;i<num;i++)
			{
				AcGePoint2d Pt;
				Poly->getPointAt(i,Pt);
				NodePtArray.append(Pt);
			}
			//			Poly->close();
		}
	}
	else if(Poly2d)
	{
		AcDbObjectId eId,vertexObjId;
		if(Acad::eOk==acdbOpenObject(Poly2d,eId,AcDb::kForRead))
		{
			AcDbObjectIterator *pVertIter=Poly2d->vertexIterator();
			Poly2d->close();
			AcDb2dVertex *pVertex;
			AcGePoint3d location;
			for(int i=0;!pVertIter->done();i++,pVertIter->step())
			{
				vertexObjId=pVertIter->objectId();
				/*acdbOpenObject(pVertex,vertexObjId,AcDb::kForRead);*/
				if(acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				location=pVertex->position();
				pVertex->close();
				NodePtArray.append(AcGePoint2d(location.x,location.y));
			}
		}
	}
	else if(Poly3d)
	{
		AcDbObjectId eId,vertexObjId;
		if(Acad::eOk==acdbOpenObject(Poly3d,eId,AcDb::kForRead))
		{
			AcDbObjectIterator *pVertIter=Poly3d->vertexIterator();
			Poly3d->close();
			AcDb2dVertex *pVertex;
			AcGePoint3d location;
			for(int i=0;!pVertIter->done();i++,pVertIter->step())
			{
				vertexObjId=pVertIter->objectId();
				/*acdbOpenObject(pVertex,vertexObjId,AcDb::kForRead);*/
				if(acdbOpenObject(pVertex, vertexObjId, AcDb::kForRead)!=Acad::eOk)
				{
					ads_printf(L"打开实体失败！\n");
					return;
				}
				location=pVertex->position();
				pVertex->close();
				NodePtArray.append(AcGePoint2d(location.x,location.y));
			}
		}
	}
}

void PolyLineCal::GetBianTwoPoint(AcGePoint3d &StartPt,AcGePoint3d &EndPt)
{
	int i,num=NodePtArray.length();
	AcGePoint2d Pt1,Pt2;
	double Mindist=9999999,dist;
	for(i=0;i<num-1;i++)
	{
		Pt1=NodePtArray.at(i);
		Pt2=NodePtArray.at(i+1);
		double x,y,t;
		x=NearPT.x;
		y=NearPT.y;
		dist=BAS_DRAW_FUN::DistToLine(Pt1.x,Pt1.y,Pt2.x,Pt2.y,x,y,t);
		if(dist<Mindist)
		{
			Mindist=dist;
			StartPt.x=Pt1.x;
			StartPt.y=Pt1.y;
			EndPt.x=Pt2.x;
			EndPt.y=Pt2.y;
		}
	}
}
