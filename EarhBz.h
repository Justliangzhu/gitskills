// EarhBz.h: interface for the EarhBz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EARHBZ_H__888919E7_C403_4200_943F_A03A9EF93698__INCLUDED_)
#define AFX_EARHBZ_H__888919E7_C403_4200_943F_A03A9EF93698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dbmain.h"
#include "rxboiler.h"
#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
//#include <dbapserv.h>
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
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN.H"

class EarhBz_ROAD : public AcDbEntity,public BAS_DRAW_FUN 
{
public:
	
	EarhBz_ROAD();
	virtual ~EarhBz_ROAD();
	
	double texth;
	double HBVScale;
	int m_Frame_Index;	//图框号	20190712添加
    Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	void set_HScale_ori();
    Adesk::Boolean  G_make2dline(AcGiWorldDraw* pWd,AcGePoint3dArray ptarray, int icolor);
	
	double cmltosx (double cml);	   
	ACRX_DECLARE_MEMBERS(EarhBz_ROAD);
private:
	double High;//栏高
	double x_ori,y_ori;//startpoint坐标
	double m_HScale, m_VScale;	//地面线图纵横编比例	//20190709	修改采用双轴比例尺
	
	AcDbObjectId EarhId;
};

#endif // !defined(AFX_EARHBZ_H__888919E7_C403_4200_943F_A03A9EF93698__INCLUDED_)
