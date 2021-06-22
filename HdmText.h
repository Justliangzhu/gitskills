// HdmText.h: interface for the HdmText class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDMTEXT_H__3542ABEB_94AA_11D4_AF18_006067772FF4__INCLUDED_)
#define AFX_HDMTEXT_H__3542ABEB_94AA_11D4_AF18_006067772FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once 
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"
//#define AppName  /*MSG1*/"RailHdm" 
class HdmText_ROAD  : public AcDbEntity,public BAS_DRAW_FUN
{
public:
	ACRX_DECLARE_MEMBERS(HdmText_ROAD);
	HdmText_ROAD();
	Acad::ErrorStatus subTransformBy(const AcGeMatrix3d& xform);
	virtual ~HdmText_ROAD();

	AcGePoint3d mpt;

	ACHAR m_ckml[30];//主线里程
	ACHAR FLD_ckml[30];//右线里程

	int DorS;
	double ZZ_twh,ZY_twh;//左右填挖高
	double YZ_twh,YY_twh;
	double XJJ;

	double m_fillarea;
	double m_cutarea;
	double DuoluoInf,offsetX,YoffsetX,BTinf,Y_BTinf,LZDK,RZDK,EarH;
	AcGePoint3dArray m_ZSlopPt,m_YSlopPt;
	double m_ZBP[30],m_YBP[30];

	/////////////////////////////////用于交互设计
	double Z_LFillArea ,MidFillArea , Y_RFillArea;
	double Z_LCutArea , MidCutArea , Y_RCutArea;
	double Z_LFillSSTArea,MidFillSSTArea, Y_RFillSSTArea;//填渗水土面积
	double Z_LQBTArea ,MidQBTArea ,Y_RQBTArea;
	double Z_LFillBetterSoilArea ,MidFillGLTArea ,Y_RFillBetterSoilArea;
	double Z_LHJarea ,MidHJarea ,Y_RHJarea;
	double Z_LSGJQarea ,MidGouJQarea ,Y_RSGJQarea;
	double  Z_LDQlength ,MidDQlength ,Y_RDQlength;//挡墙坡长
	double Z_LGrasslength ,MidGrasslength,Y_RGrasslength;//铺草坡长
	double  Z_LGouTarea , Y_RGouTarea;
	double Z_LGouWarea ,Y_RGouWarea;

    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	void NoteBP(AcGiWorldDraw* pWd);//标注边坡
	void note_side_slop(AcGePoint3d pt1,AcGePoint3d pt2,double m,ACHAR prestr[20],
                    double txth,AcGiWorldDraw* pWd);
	Adesk::Boolean Draw(int iColor);
	void note_side_slop_outdraw(AcGePoint3d pt1,AcGePoint3d pt2,double m,ACHAR prestr[20],
							 double txth, ACHAR* LayerName = NULL);
	void  OutDrawBP(double NoteTextH=4.0);//标注边坡

};
int MAKE_ACDBOPENOBJECT_FUNCTION(HdmText_ROAD);
#endif // !defined(AFX_HDMTEXT_H__3542ABEB_94AA_11D4_AF18_006067772FF4__INCLUDED_)
