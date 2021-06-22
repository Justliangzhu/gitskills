// TRIM.h: interface for the TRIM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRIM_H__7F5327FF_A6ED_4591_8CE6_CDFCED2614B3__INCLUDED_)
#define AFX_TRIM_H__7F5327FF_A6ED_4591_8CE6_CDFCED2614B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"
#define AppName  /*MSG1*/L"GUADS" 
#include "XLDataBase.h"
#include "GTZX.h"
class TRIM_ROAD :public BAS_DRAW_FUN,public AcDbEntity
{
public:
	double TUSCALE;
	void CreateLC();
	double dTK;//外图框间距
	double dSPACE;//空白区宽度
	
	struct Box
	{
		AcGePoint3d LDPt,LTPt,RTPt,RDPt,Cen;
		double XZjiao;
	}resBOX[200];
	Box resBOX1[200];
	void GetBoxCenter(Box &box);
	
	void DrawBox(Box box);
	Box boxallIN;//内框
	Box boxallOUT;//外框
	
	
	ACRX_DECLARE_MEMBERS(TRIM_ROAD);
	TRIM_ROAD(double OuterHigh=297,double dh=10,double ddtk=80,double dspace=50,double scale=1.0);
	virtual ~TRIM_ROAD();
//	void AddFrame();
	void SetEndPoint(AcGePoint3d mPt);
	void AddFrame(AcGePoint3d mPt);
	void DelEndFrame();
	double High_Outer;//图纸高度
	double High;//图纸内框高度
	double dH;//内外框间距
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd=NULL);
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		AcDbIntArray& osnapModes,
		AcDbIntArray& geomIds) const;
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		const AcGeVector3d& offset);
	virtual Acad::ErrorStatus  subExplode(AcDbVoidPtrArray& entityset) const;
	
//private:
	Adesk::Int32 NMidPt;//==NUM_FRAME+1;
	double jiao[200];//1->199
	AcGePoint3d MidPt[200];

	struct TKLC
	{
		CString sml,eml;
	}tklc[200];//1~199

	AcGePoint3d LPt[200];
	AcGePoint3d RPt[200];

	void CreateFrame();
	void CreateResBox(AcGePoint3d OriPt);
	void CreateResBox1(AcGePoint3d OriPt);
		
};
void MAKE_ACDBENTITY_FUNCTION(TRIM_ROAD);
#endif // !defined(AFX_TRIM_H__7F5327FF_A6ED_4591_8CE6_CDFCED2614B3__INCLUDED_)
