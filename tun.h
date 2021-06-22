// Tun.h: interface for the Tun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUN_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
#define AFX_TUN_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define AppName  /*MSG1*/L"GUADS" 
#include "gtzdm.h"
#include "dbmain.h"
#include "dmx.h"


class TUN_ROAD : //自定义地面线类
	public GTZDM_ROAD
{
public:
	ACRX_DECLARE_MEMBERS(TUN_ROAD);
	TUN_ROAD();
	TUN_ROAD(int nTun,double array[][2],GTZDM_ROAD *zm,int IfAutoName,int zdmno);//array[0]--lc,array[1]--level
	TUN_ROAD(int nTun,TUNTAB tun[],GTZDM_ROAD *zm,int IfAutoName,int zdmno);
	virtual ~TUN_ROAD();

	GTZDM_ROAD *zmLine;
    struct Tundata
	 {
		 double sml,eml,cml;
		 double sdml,edml,cdml;
		 ACHAR SKml[80],EKml[80],CKml[80];
		 double spt[2],ept[2],cpt[2];
		 CString TunName;//隧道名 
		 CString XZ;
		 double ZZDK,YZDK;

		 Tundata() 
		 {
			 _tcscpy(SKml,L"");
			 _tcscpy(EKml,L"");
			 _tcscpy(CKml,L"");
		 }
	 } Tun[300];//最多500座
	Adesk::Int32 NTun;//地面线点数//dwgIN.dwgout.
	AcDbObjectId TunLayerId;
	int IfChangeTunName;
	int Zdmno;//纵断面图号

	void DrawVLine(AcGiWorldDraw* pWd,double pt[2],double len);
	void DrawHLine(AcGiWorldDraw* pWd,double sml,double eml,double Fpt[2],double Tpt[2],double hd);
	Adesk::Boolean BZTun(AcGiWorldDraw* pWd,int iTun,double texth);
	Adesk::Boolean DelTun(AcGePoint2d PT);
	Adesk::Boolean AddTun(AcGePoint2d SPT,AcGePoint2d EPT);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,AcDbIntArray& osnapModes,AcDbIntArray& geomIds)const;
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset);
	void CheckTun();//检查隧道范围有无交插，合并处理

	};
int MAKE_ACDBOPENOBJECT_FUNCTION(TUN_ROAD);//?
#endif // !defined(AFX_Tun_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
