// EarthLine.h: interface for the EarthLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EARTHLINE_H__BC9026E1_8E3D_11D4_AF18_006067772FF4__INCLUDED_)
#define AFX_EARTHLINE_H__BC9026E1_8E3D_11D4_AF18_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dbmain.h"
#include "BAS_DRAW_FUN.h"
//#define AppName  /*MSG1*/"RailHdm" 
class EarthLine_ROAD : public AcDbEntity,public BAS_DRAW_FUN
{

public:
	ACRX_DECLARE_MEMBERS(EarthLine_ROAD);
	EarthLine_ROAD();
	virtual ~EarthLine_ROAD();
    double offset;
	/*
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler* filer);
		Acad::ErrorStatus dwgOutFields(AcDbDwgFiler* filer) const;
		Acad::ErrorStatus dxfInFields(AcDbDxfFiler* filer);
		Acad::ErrorStatus dxfOutFields(AcDbDxfFiler* filer) const;*/
	

		// AcDbEntity overrides
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	void Draw(int iColor,double NoteTextH=4.0);
//	void DrawEarthLine(AcGiWorldDraw* pWd);
	
/*	virtual Acad::ErrorStatus
	getGripPoints(AcGePoint3dArray& gripPoints,
					  AcDbIntArray& osnapMasks,
					  AcDbIntArray& geomIds) const;

	virtual Acad::ErrorStatus
	moveGripPointsAt(const AcDbIntArray& indices,
					     const AcGeVector3d& offset);

	virtual Acad::ErrorStatus transformBy(const AcGeMatrix3d& xform);*/



//	    virtual Acad::ErrorStatus explode(AcDbVoidPtrArray& entitySet) const;

	virtual void subList() const;
// 指定一个x坐标，返回相对应的地面高程并计算点号,i// nf=0: 找不到，inf=1：正常结束
	int Cal_Yd(double x,double &Yd1,int &ptnum);

	int m_EarthPointNum;
	int m_BenchPointNum;
    AcGePoint3d *m_EarthPoints;
	AcGePoint3d *m_BenchPoints;
	AcGePoint3d bzpoint;
	ACHAR m_ckml[30];
	double m_cml,m_twh,Yd,EarH;
	ACHAR m_ZorY[10];
	double HTHD;
	AcGePoint3d m_OriPoint;
	double m_sfactor;	//绘图比例
	
};
int MAKE_ACDBOPENOBJECT_FUNCTION(EarthLine_ROAD);
#endif // !defined(AFX_EARTHLINE_H__BC9026E1_8E3D_11D4_AF18_006067772FF4__INCLUDED_)
