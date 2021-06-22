// DMX_LCB.h: interface for the DMX_LCB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMX_LCB_H__EFE35394_BE76_496A_99E4_05808F2B3446__INCLUDED_)
#define AFX_DMX_LCB_H__EFE35394_BE76_496A_99E4_05808F2B3446__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BAS_DRAW_FUN.h"
#include "dbmain.h"

//#define AppName  /*MSG1*/"GUADS" 
class DMX_LCB_ROAD : 
	public AcDbEntity,
	public BAS_DRAW_FUN  
{
public:
	void Modify_DATA();
	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
	Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int );
	Adesk::Boolean  BZLC(AcGiWorldDraw* pWd);
	Adesk::Boolean  BZDL(AcGiWorldDraw* pWd);  
	ACRX_DECLARE_MEMBERS(DMX_LCB_ROAD);
	DMX_LCB_ROAD();
	virtual ~DMX_LCB_ROAD();
	void set_ori_scale();
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);	
	virtual Adesk::Boolean  subWorldDraw(AcGiWorldDraw* pWd);
		
private:
	AcDbObjectId LcbId;
	double HBVScale;
	Adesk::Int16 tarray,NLC,NDL;
	double length;
	double startlc;
	double texth;
	double Boxh;
	double x_ori,y_ori;
	DLBZ DLB[MAXDLNUM];
	double array[4*MAXJDNUM][10];
	double m_HScale, m_VScale;//地面线图纵横编比例	20190709	修改采用双轴比例尺
};

#endif // !defined(AFX_DMX_LCB_H__EFE35394_BE76_496A_99E4_05808F2B3446__INCLUDED_)
