// SPIRAL.h: interface for the SPIRAL class.
// ����������
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPIRAL_H__82E23572_051C_4B80_93BD_FD3650616891__INCLUDED_)
#define AFX_SPIRAL_H__82E23572_051C_4B80_93BD_FD3650616891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"

//////////////////////////////////////////////////////////////////////////
// ����������

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

	double GetPtJiaoByX(double LToBasePt,double &sita);    // ���뵽���յ�����߳������ظõ�����յ�������X��нǣ��ֲ�����ϵ�£�
	double GetQXFwj(double LToBasePt);                     // ���뵽���յ�����߳������ط�λ��
	bool   GetPoint(double LToBasePt,double &N,double &E); // ���뵽���յ�����߳�����������
	void   DRAW(); // ���ƻ�������
	
private:
	void CALCULATION(); // ���㻺������Ҫ��

public:
	double StartL, EndL;              // ���յ����յ�����߾���
	double A, StartR, EndR;           // Aֵ�����뾶���յ�뾶
	double BaseFwj, StartFwj, EndFwj; // ���յ�ķ�λ�ǣ����ķ�λ�ǣ��յ�ķ�λ��
	double BaseN,BaseE, StartN,StartE, EndN,EndE; // ���յ㡢���յ������ 
	int    LeftOrRight;               // ��ƫΪ-1����ƫΪ+1
	int    m_iColor;                  // ͼ����ɫ
	AcDbObjectId SprialID;            // ͼ��ID
	
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
//	double fx0,A;///�������߰뾶��������ߴ����߷���fwj
//	double startX0,startY0;//���������������꣬��һ����ֱ����
//	double endX0,endY0;
////private:
////	AcDbLine *Line;//�������߰뾶��������ߡ�
//	AcDbSpline *SPLINE;
//	AcGePoint3d sPt0;//ֱ��������
//	double Buchang,startl,endl,R;
//	void huanhequxian();
//	double *pspir;
//	Adesk::Int16 NUM;
//	Adesk::Int16 lrRot;//��ƫΪ-1,��ƫΪ��1��

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
