// GTZDM.h: interface for the GTZDM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZDK_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_)
#define AFX_GTZDK_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
#include <adslib.h>
#include <dbxrecrd.h>
#include <string.h>
#include <dbents.h>
#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 
#include <dbjig.h>
#include <acgi.h>
#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN.H"
#include "JD_CENTER.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
//#define AppName  /*MSG1*/"GUADS"    


//		自定义类    
//   DLB[j]--断链表（含冠号）
//   PDB[k][]--坡度表  
struct ZDKDAT
{
	   double cml;
	   double LZDK,RZDK;
	   int IsInsertHdm;//是否为增加横断面
	   double Lpt[2];//占地界的x,y坐标
	   double Rpt[2];//占地界的x,y坐标
};

class GTZDK_ROAD : public AcDbEntity,public BAS_DRAW_FUN//坡度类
{ 
public:
   ACRX_DECLARE_MEMBERS(GTZDK_ROAD);

   int CS;
   double DFACTOR;
   double Scml,Ecml;
   Adesk::Int16 NZDK;//左右占地宽点数	
   ZDKDAT pZDK[10000];
   JD_CENTER *pLine;	
   bool ReadZDKFile(FILE* fpr);//读占地宽文件
   void CalZDKXY();//由占地宽计算占地界坐标
   void FastSort(struct ZDKDAT R[],int s1,int t1);
   int PARTITION(struct ZDKDAT R[],int l,int h);
   int ZDKDel(AcGePoint3d PT);
   int ZDKAdd(AcGePoint3d PT);
   int ZDKChange(AcGePoint3d PT);
   void DelEntInYD();
//    CZDKDATA ZX;
	GTZDK_ROAD();
	int textStyle();
	void AddZDK();
	void DelZDK();
	void ChangeZDK();
	virtual ~GTZDK_ROAD();
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	 Adesk::Boolean G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
 	 Adesk::Boolean G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);
    Adesk::Boolean G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);

	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;

    virtual void subList() const;
    virtual Acad::ErrorStatus  subExplode(AcDbVoidPtrArray& entityset)const;
    //  virtual AcRxObject* clone() const;
    virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		                                                AcDbIntArray& osnapModes,
														AcDbIntArray& geomIds) const;

    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		                                       const AcGeVector3d& offset);
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode   osnapMode,
		int               gsSelectionMark,
		const AcGePoint3d&    pickPoint,
		const AcGePoint3d&    lastPoint,
		const AcGeMatrix3d&   viewXform,
		AcGePoint3dArray& snapPoints,
		
		AcDbIntArray&     geomIds) const ;

   	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);	
     Adesk::Boolean  BZ_ZDK(AcGiWorldDraw* pWd);	 

};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZDK_ROAD);
#endif // !defined(AFX_GTZDK_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_)
