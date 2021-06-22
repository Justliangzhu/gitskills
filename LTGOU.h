// LTGOU.h: interface for the LTGOU class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LTGOU_H__2F740862_8F0C_11D4_AF18_006067772FF4__INCLUDED_)
#define AFX_LTGOU_H__2F740862_8F0C_11D4_AF18_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <acdb.h>		// acdb definitions
#include <adslib.h>		// ads defs
#include <dbsymtb.h>
#include <adeskabb.h>
#include <dbpl.h>		// AcDbPolyline
#include <gepnt2d.h>
#include <gepnt3d.h>
#include <acgi.h>
#include "bas_draw_fun.h"

#include "EarthLine.h"
#include "JSGOU.h"
#include "HdmText.h"


#define AppName  /*MSG1*/L"RailHdm" 
class LTGOU_ROAD : public AcDbEntity,public BAS_DRAW_FUN
{
public:
	ACRX_DECLARE_MEMBERS(LTGOU_ROAD);
	LTGOU_ROAD();
	virtual ~LTGOU_ROAD();

	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
    virtual void subList() const;
	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	int Cal_Yd(double x,double &Yd,int &ptnum);
	Adesk::Boolean LTGOU_ROAD::Draw(int iColor);

	int m_LTGouPointNum,m_ZorY;
    AcGePoint3d m_LTGouPoints[18],m_GouBotPoint,m_OriGouBotPt,m_PJDpt;		
	double m_GouBotH,m_GouBotX,m_XtoPJ;
	double m_cml;
	int pjqptnum;//�½ŵ���
	AcGePoint3d PjqPt[7];//�½�ǽ
	int JqPtNum;//���е���
	AcGePoint3d JqPt[29];//����
	Adesk::Int16 GouProperty;// 1-ˮ��,0-�½�ǽ	3-�޸Ĺ���·��ˮ��
	//���µ���,���µ�����,�������,�ڲ��£�����£��������
	//double HPDK,HPDI,GH,GW,SP1,SP2,PSG_JQHD;
	//������   ֲ������ ������        ֲ������           ����������
    double m_T2W,m_T1W,m_T2H,m_GH,m_GW,m_HH,m_SP1,m_SP2,m_SP3,PSG_JQHD,m_PSG_GDR;
	EarthLine_ROAD * l_earthline;
	EarthLine_ROAD * r_earthline;
	JSGOU_ROAD * JSGou;
	HdmText_ROAD * Note;
    bool ifMoveGripPoints;
	Adesk::Int16 m_IfSetBotH;
	
	Acad::ErrorStatus  subGetOsnapPoints(
										 AcDb::OsnapMode osnapMode,
										 int gsSelectionMark,
										 const AcGePoint3d & pickPoint,
										 const AcGePoint3d & lastPoint,
										 const AcGeMatrix3d& viewXform,
										 AcGePoint3dArray & snapPoints,
										 AcDbIntArray& geomIds)const;
	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
									   AcDbIntArray& osnapModes,
									   AcDbIntArray& geomIds)const;
	Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices,
										  const AcGeVector3d& offset);
	Acad::ErrorStatus dwgInFields(AcDbDwgFiler * filer);
	Acad::ErrorStatus dwgOutFields(AcDbDwgFiler * filer) const;
	
	//���ò๵����
	void SetGutterPara(double T2W, double T1W, double T2H, double GH, double GW, double HH, double SP1, double SP2, double SP3, double JQHD, double GDR);
	//ɾ��ˮ��	20190930����
	void DeleteGutter();
};
int MAKE_ACDBOPENOBJECT_FUNCTION(LTGOU_ROAD);
#endif // !defined(AFX_LTGOU_H__2F740862_8F0C_11D4_AF18_006067772FF4__INCLUDED_)
