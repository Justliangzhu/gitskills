// SPIRAL.h: interface for the SPIRAL class.
// 缓和曲线类
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIRAL_H__82E23572_051C_4B80_93BD_FD3650616891__INCLUDED_)
#define AFX_SPIRAL_H__82E23572_051C_4B80_93BD_FD3650616891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

//////////////////////////////////////////////////////////////////////////
// 缓和曲线类

class SPIRAL: /*public AcDbEntity,*/public BAS_DRAW_FUN
{
public:
//	ACRX_DECLARE_MEMBERS(SPIRAL);

	void Set(double startn,double starte,double startfwj,double a,
		double startr,double endr,int lr,int iColor=0);
	void SetByEnd(double endn,double ende,double endfwj,double a,
		double endr,double startr,int lr,int iColor=0);
	void Set2(double endn,double ende,double endfwj,double a,
		double startr,double endr,int lr,int iColor=0);

	double GetPtJiaoByX(double LToBasePt,double &sita);    // 传入到参照点的曲线长，返回该点与参照点连线与X轴夹角（局部坐标系下）
	double GetQXFwj(double LToBasePt);                     // 传入到参照点的曲线长，返回方位角
	bool   GetPoint(double LToBasePt,double &N,double &E); // 传入到参照点的曲线长，返回坐标
	void   DRAW(); // 绘制缓和曲线
	
private:
	void CALCULATION(); // 计算缓和曲线要素

public:
	double StartL, EndL;              // 起、终点距参照点的曲线距离
	double A, StartR, EndR;           // A值，起点半径，终点半径
	double BaseFwj, StartFwj, EndFwj; // 参照点的方位角，起点的方位角，终点的方位角
	double BaseN,BaseE, StartN,StartE, EndN,EndE; // 参照点、起终点的坐标 
	int    LeftOrRight;               // 左偏为-1，右偏为+1
	int    m_iColor;                  // 图形颜色
	AcDbObjectId SprialID;            // 图形ID
	
public:
	SPIRAL();
	SPIRAL(double startn,double starte,double startfwj,double a,
		double startr,double endr,int lr);
	virtual ~SPIRAL();
};
//MAKE_ACDBOPENOBJECT_FUNCTION(SPIRAL);

#endif // !defined(AFX_SPIRAL_H__82E23572_051C_4B80_93BD_FD3650616891__INCLUDED_)

//	void GetItem(double &AA,double &RR,AcGePoint3d &ssPt0,double &fwj,double &startl,int &lr);
//	double getFwj(int i);//0-base
//	AcGePoint3d getPoint(int i);
//	AcGePoint3d getPoint(double slc);//0-base
//	int logicalLength();
//	void TrimLine(AcDbLine*);
//	void TrimARC(AcDbArc*);
//	OBJARRAY pObjAy;
//	void worldDraw(int mode=0);
//	void Revise_EndPoint(AcGePoint3d ENDPOINT);
//	void AddReviseItem(AcDbObject*);
//	void Revise(AcDbObject*);
//	void ReviseLine(AcDbLine*);
//	void ReviseCircle(AcDbCircle*);
//	double fx0,A;///缓和曲线半径无穷大处切线处切线方向fwj
//	double startX0,startY0;//缓和曲线起点的坐标，不一定是直缓点
//	double endX0,endY0;
////private:
////	AcDbLine *Line;//缓和曲线半径无穷大处切线　
//	AcDbSpline *SPLINE;
//	AcGePoint3d sPt0;//直缓点坐标
//	double Buchang,startl,endl,R;
//	void huanhequxian();
//	double *pspir;
//	Adesk::Int16 NUM;
//	Adesk::Int16 lrRot;//左偏为-1,右偏为＋1；

/*
	virtual Adesk::Boolean worldDraw(AcGiWorldDraw* mode);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	
    virtual Acad::ErrorStatus	intersectWith(
        const AcDbEntity*   ent,
        AcDb::Intersect     intType,
        AcGePoint3dArray&      points,
        int                 thisGsMarker  = 0,
        int                 otherGsMarker = 0)
        const;	
	
    virtual Acad::ErrorStatus	intersectWith(
		const AcDbEntity*   ent,
		AcDb::Intersect     intType,
		const AcGePlane&    projPlane,
		AcGePoint3dArray&      points,
		int                 thisGsMarker  = 0,
		int                 otherGsMarker = 0)
		const;
	
	virtual Acad::ErrorStatus   transformBy(const AcGeMatrix3d& xform);*/
//	SPIRAL(double AA,double RR,double x,double y,double fwj,double sstartl,int lr);
//	SPIRAL(AcGePoint3d sPt,AcGePoint3d ePt,double sa,double ea,double R1,double R2,double L,int ZRflag);
//	SPIRAL(double AA,double RR,AcGePoint3d ssPt0,double fwj,double sstartl,int lr);
