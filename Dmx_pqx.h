// DMX_PQX.h: interface for the DMX_PQX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DMX_PQX_H__3E774856_41C8_402E_AC17_E2555DB90DB2__INCLUDED_)
#define AFX_DMX_PQX_H__3E774856_41C8_402E_AC17_E2555DB90DB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define AppName  /*MSG1*/L"GUADS" 

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


class DMX_PQX_ROAD : public AcDbEntity,public BAS_DRAW_FUN//������-ƽ����
{
public:
	DMX_PQX_ROAD();//must have
	virtual ~DMX_PQX_ROAD();		
	

    double texth;
	double HBVScale;
	int m_Frame_Index;	//ͼ���	20190712���
	double xyddaa(double xb,double yb,double xe,double ye,double *f);
	Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	void set_HScale_ori();
    Adesk::Boolean  G_make2dline(AcGiWorldDraw* pWd,AcGePoint3dArray ptarray, int icolor);
	void   draw_plan (AcGiWorldDraw* pWd);//��ƽ��      
	
	double cmltosx (double cml);
	//num==NUM��Ԫ��,array[0]--slc,array[1]--l
	//array[2]--zhyflag,array[3]--lrflag
	//array[4]--a,array[5]--R
	ACRX_DECLARE_MEMBERS(DMX_PQX_ROAD);
	
private:
	double High;//����
	double x_ori,y_ori;//startpoint����
	double m_HScale, m_VScale;	//������ͼ�ݺ�����	//20190709	�޸Ĳ���˫�������
	Adesk::Int32 NUM;//��Ԫ��//dwgIN.dwgout.//NUM��1����
	AcDbObjectId PqxId;
};

#endif // !defined(AFX_DMX_PQX_H__3E774856_41C8_402E_AC17_E2555DB90DB2__INCLUDED_)
