// BlockComputerFun.h: interface for the BlockComputerFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLOCKCOMPUTERFUN_H__AD836B07_7FFF_4CC5_A003_11A5B36F08BE__INCLUDED_)
#define AFX_BLOCKCOMPUTERFUN_H__AD836B07_7FFF_4CC5_A003_11A5B36F08BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"
struct RouteDot 
{
	double DotX;
	double DotY;
	double CurRadius;
	double CurMiles;
	double OrientAngle;
};

struct RampItem 
{
	//匝道名称;
	CString ItemName;
	//匝道单元起点坐标,方位角,起点里程;
	double FDotX;
	double FDotY;
	double FDotOrient;
	double FDotSMiles;
	//匝道单元的起终半径;
	//FDotCurMiles;
	double ItemStartR;
	double ItemEndR;
	//匝道单元的类型,属性,半径变化趋势,单元偏向,匝道单元当前长度;
	double ItemType;
	double ItemPro;
	double RadForWard;
	double ItemDef;
	double ItemLen;	
};

class BlockComputerFun:public BAS_DRAW_FUN
{
public:
	BlockComputerFun();
	virtual ~BlockComputerFun();
	//2005.4.24 hhp
	double GetPreciseLength(double x1,double y1,double x2,double y2);
	double GetValueKByOrient(double ValueOrient);
	void GetProjectDotOfLine(double LineX1,double LineY1,double ValueK,
		double SelDotX,double SelDotY,double &ProjX,double &ProjY);
	//2005.4.25 hhp
	double ConvertAngleToOrient(double AngleOfLineAndXaxes);
	//2005.4.26 hhp
	void GetCrossDotOfLine(double ValueK1,double LineX1,double LineY1,
					   double ValueK2,double LineX2,double LineY2,
					   double &CrossX,double &CrossY);
	//2005.4.27 hhp
	double GetDistOfLineAndSelDot(double ValueK,double LineX,double LineY,
		double SelDotX,double SelDotY);
	//2005.5.5 hhp
	//根据指定圆的圆心坐标,半径值及线路的偏向
	//计算沿线路走向上通过指定两点的圆其圆心坐标及半径;
	BOOL  GetCenterCorAndRadius(double CurCenX,double CurCenY,double CurRadius,double PreDeflect,double &CurDeflect,
							   double FixDotX,double FixDotY,double MoveDotX,double MoveDotY,
							   double &ComCenX,double &ComCenY,double &ComRadius);
	double GetValueKByTwoPoint(double LineX1,double LineY1,double LineX2,double LineY2);
	double GetDistOfLineAndDot(double LineX1,double LineY1,double LineX2,double LineY2,double DotX,double DotY);
	int GetDotAtWhichSide(double LineX1,double LineY1,double LineX2,
		double LineY2,double DotX,double DotY);
	int DotAtMiddleOrOut(double LineX1,double LineY1,double LineX2,
		double LineY2,double DotX,double DotY);
	void ShowArcOnScreen(double CenX,double CenY,double ArcRadius,
		double ItemDeflect,double ArcDotX1,double ArcDotY1,
		double ArcDotX2,double ArcDotY2,AcDbObjectId &ArcId,AcDbArc * pArc = NULL);
	void GetTangentCorOfTwoCircle(double CenX1,double CenY1,double CenRadius1,
		double CenX2,double CenY2,double CenRadius2,
		double &TanX,double &TanY);
	double ConvertOrientToAngle(double ValueOrient);
	/////////////////////////////////////////////////
	int ComputerCorOfAnyDotOnItem(RouteDot &SDot,RouteDot &EDot,double CirLen,
		double ValueA,double ERadius,int ItemType,int ItemDeflect);
	int GetDeflectOfDotToLine(double LineX,double LineY,double LineOrient,
						  double SelDotX,double SelDotY);
	double GetAngleOfLineAndXaxes(double StartX,double StartY,double EndX,double EndY);
	double  GetValueOfMonWithNumPower(double ValuePar,long NumPower);
	long  GetValueOfFactorial(long ValueNum);
	double GetAbsXCorOfSprialCir(double ValueA,double CirLength);
	double GetAbsYCorOfSprialCir(double ValueA,double CirLength);
	int GetItemCurForward(double DotOrientAngle);
	BOOL GetValueAOfSprial(double StartX,double StartY,double StartR,double StartOrient,double EndX,double EndY,
		double &EndR,double &OrignX,double &OrignY,double &OrignOrient,double &CirLength,double &ValueA,
		int RadForWard,int Deflect);
	double GetValueAOfSprial(double StartX,double StartY,double StartOrient,double EndX,double EndY,
		double &EndR,double &CriLength);	
	int GetRadiusForward(double StartR,double EndR);
	void GetOrignCorOfSprial(double SprX,double SprY,double SprRad,
		double SprOrient,double ValueA,int Deflect,int RadForward,
		double &OrignX,double &OrignY,double &OrignOrient);
	void ShowSpiralCirOnScreen(double StartX,double StartY,double StartR,double StartOrient,
		double EndR,int ItemDeflect,double ValueA,double CirLen,
		AcDbObjectId & CreateEntId/*,AcDb2dPolyline *p2dPolyline = NULL*/);
	double GetValidLimValueAEnd(double DeflectAngle,double StartR);
	Adesk::Boolean  DrawSprialCirInWorldDraw(AcGiWorldDraw* pWd,double INDATA[5],double LO,xlpoint *P0,xlpoint *PZ,int icolor);	
	double GetCenCorAndSprialDotCor(double SEast,double SNorth,double SOrientAngle,
								double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,
								double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY);
	double GetDistOfCenterAndSelDot(double SEast,double SNorth,double SOrientAngle,
								double StartR,double EndR,double ValueA,double ItemDeflect,double CirDeflect,double InputRad,
								double SelDotX,double SelDotY,double &CenX,double &CenY,double &SprX,double &SprY);	

	double GetRadiusByStartDotAndSelDot(double StartX,double StartY,double StartOrient,double CurDeflect,
		double SelDotX,double SelDotY,double &GetCenX,double &GetCenY);
	double GetRadiusOfLineAndTwoDot(double LineX,double LineY,double LineOrient,double FixX,double FixY,
									double MoveX,double MoveY,double &CenX,double &CenY,double &TanX,double &TanY,BOOL BackOrnot);
	void ModifyStartDot(double &StartX,double &StartY,double ValueSide,double OffsetDist,
		double &StartOrient,double ValueDef,double ValueAngle);
	void BackTonotModifidOfSDot(double &StartX,double &StartY,double ValueSide,double OffsetDist,
		double &StartOrient,double ValueDef,double ValueAngle);
	BOOL GetComItemFromSelEntity(AcDbObjectId SelEntId,struct RampItem & ComItem,double CurDef = 0,BOOL FreeOrnot = false);
	void GetOtherParameterOfArc(double StartX,double StartY,double StartOA,double ArcRad,double ArcLen,double ArcDef,
		double &ArcCenX,double &ArcCenY,double & ArcEndX,double &ArcEndY);
	double ChangeDataPrecision(double InputData,int PreLength = 4);	
    void ComputerMilesAndCorsOfRoute(AcDbObjectId InputRouteId,
					   ads_point nearPoint,double &ItemType,double &ReturnProMils,xlpoint &RoutePoint);
	
};
#endif // !defined(AFX_BLOCKCOMPUTERFUN_H__AD836B07_7FFF_4CC5_A003_11A5B36F08BE__INCLUDED_)
