// BRI.h: interface for the BRI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BRI_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
#define AFX_BRI_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define AppName  /*MSG1*/L"GUADS" 
#include "gtzdm.h"
#include "dmx.h"
#include "dbmain.h"


class BRI_ROAD : //�Զ����������
	public GTZDM_ROAD
{
public:
	ACRX_DECLARE_MEMBERS(BRI_ROAD);
	BRI_ROAD();
	BRI_ROAD(int nbri,double array[][2],GTZDM_ROAD *zm,int IfAutoName,int zdmno);//array[0]--lc,array[1]--level
	BRI_ROAD(int nbri,BRITAB bri[],GTZDM_ROAD *zm,int IfAutoName,int zdmno);

	virtual ~BRI_ROAD();

	GTZDM_ROAD *zmLine;
	int BriNum;
    struct Bridata
	 {
		 double sml,eml,cml;
		 double sdml,edml,cdml;
		 ACHAR SKml[80],EKml[80],CKml[80];
		 double spt[2],ept[2],cpt[2];
		 CString BriName;//���� 
		 CString KKSY;//�׿�ʽ��
		 CString SWHt;//һ����ˮλ�߳�
		 CString XZ;
		 double HeadAng;
		 double TailAng;
		 double ZZDK,YZDK;

		 Bridata()
		 {
			 _tcscpy(SKml,L"");
			 _tcscpy(EKml,L"");
			 _tcscpy(CKml,L"");
		 }
//		 int flag;//0 �ش���, 1����, 2����, 3С��
	 } Bri[500];//���500��
	Adesk::Int32 NBRI;//�����ߵ���//dwgIN.dwgout.
	AcDbObjectId BriLayerId;
	int IfChangeBriName;
	int Zdmno;//�ݶ���ͼ��

	void DrawVLine(AcGiWorldDraw* pWd,double pt[2],double len);
	void DrawHLine(AcGiWorldDraw* pWd,double sml,double eml,double Fpt[2],double Tpt[2],double hd);
	Adesk::Boolean DelBri(AcGePoint2d PT);
	Adesk::Boolean BZBRI(AcGiWorldDraw* pWd,int iBri,double texth);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,AcDbIntArray& osnapModes,AcDbIntArray& geomIds)const;
    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices,const AcGeVector3d& offset);
	Adesk::Boolean AddBri(AcGePoint2d SPT,AcGePoint2d EPT);
	void DRAWSW(AcGiWorldDraw* pWd,int i);
	void CheckBri();//���������Χ���޽��壬�ϲ�����

	};
int MAKE_ACDBOPENOBJECT_FUNCTION(BRI_ROAD);//?
#endif // !defined(AFX_BRI_H__0D5185A7_02B7_456B_8B03_AC291FD92043__INCLUDED_)
