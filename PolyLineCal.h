// PolyLineCal.h: interface for the PolyLineCal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLYLINECAL_H__150BD707_1988_4557_8ACF_765EDAD69356__INCLUDED_)
#define AFX_POLYLINECAL_H__150BD707_1988_4557_8ACF_765EDAD69356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

class PolyLineCal : public BAS_DRAW_FUN  
{
public:
	void SetNearPoint(AcGePoint3d pt);
	void SetPoly(AcDbPolyline *pline);
	void Set2dPoly(AcDb2dPolyline *pline);
	void Set3dPoly(AcDb3dPolyline *pline);
	void UpdateData();
	void GetBianTwoPoint(AcGePoint3d &StartPt,AcGePoint3d &EndPt);
public:
	PolyLineCal();
	virtual ~PolyLineCal();
private:
	AcDbPolyline *Poly;
	AcDb2dPolyline *Poly2d;
	AcDb3dPolyline *Poly3d;
	AcGePoint3d NearPT;
	AcGePoint2dArray NodePtArray;
};

#endif // !defined(AFX_POLYLINECAL_H__150BD707_1988_4557_8ACF_765EDAD69356__INCLUDED_)
