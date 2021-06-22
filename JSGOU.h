// JSGOU.h: interface for the JSGOU class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JSGOU_H__0CD4C5EF_DD61_4CA0_A179_D08BB989817C__INCLUDED_)
#define AFX_JSGOU_H__0CD4C5EF_DD61_4CA0_A179_D08BB989817C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <acdb.h>		// acdb definitions
#include <adslib.h>		// ads defs
#include <dbsymtb.h>
#include <adeskabb.h>
#include <dbpl.h>		// AcDbPolyline
#include <gepnt2d.h>
#include <gepnt3d.h>
#include <acgi.h>
#include "bas_draw_fun.h"
//#define AppName  /*MSG1*/"RailHdm" 
class JSGOU_ROAD : public AcDbEntity,public BAS_DRAW_FUN
{
public:
	ACRX_DECLARE_MEMBERS(JSGOU_ROAD);
	JSGOU_ROAD();
	virtual ~JSGOU_ROAD();
    
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual void subList() const;
	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	int Cal_Yd(double x,double &Yd,int &ptnum);
	Adesk::Boolean JSGOU_ROAD::Draw(int iColor);

	int JSGouPtNum;
	int m_ZorY;
	AcGePoint3d m_JSGouPoints[18],m_GouBotPoint,m_OriGouBotPt;
    double m_GouBotH,m_GouBotX,m_XtoPJ;
	ACHAR m_ckml[80];
	double BJ,GW,GS,SP1,SP2,TG_JQHD,TG_GDR;
	int JqPtNum;//浆切点数
	AcGePoint3d JqPt[29];
	AcGePoint3d m_JqPt[29];

	//设置侧沟参数
	void SetGutterPara(double ParaBJ, double ParaGW, double ParaGS, double ParaSP1, double ParaSP2, double ParaJQHD, double ParaGDR);

	//20190928新增	删除天沟
	void DeleteGutter();

	//20190928新增	检测天沟是否存在
	void CheckNSetGutterStatus();
};
int MAKE_ACDBOPENOBJECT_FUNCTION(JSGOU_ROAD);
#endif // !defined(AFX_JSGOU_H__0CD4C5EF_DD61_4CA0_A179_D08BB989817C__INCLUDED_)
