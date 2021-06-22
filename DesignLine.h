// DesignLine.h: interface for the DesignLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DESIGNLINE_H__2F740861_8F0C_11D4_AF18_006067772FF4__INCLUDED_)
#define AFX_DESIGNLINE_H__2F740861_8F0C_11D4_AF18_006067772FF4__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.h"
#include "LTGou.h"
#include "EarthLine.h"
#include "JSGOU.h"
#include "HdmText.h"
#define AppName  /*MSG1*/L"RailHdm" 
class DesignLine_ROAD : public BAS_DRAW_FUN,public AcDbEntity
{
public:
	ACRX_DECLARE_MEMBERS(DesignLine_ROAD);
	DesignLine_ROAD();
	virtual ~DesignLine_ROAD();

	int m_ZorY;
	bool ifMoveGripPoints;
	//double DorS;
	ACHAR RoadName[256];
	int hdmno;//在hdmdes数组中的编号
	double BTinf;
	double cml;
	int m_DesignPointNum;
    AcGePoint3d *m_DesignPoints;
	double m_Xcdh,m_Yljh,m_Tljh,m_FillORCut;
    double m_TM[30],m_TH[30],m_TPTW[30],m_TPTI[30],m_WM[30],m_PH[30],m_ZPTW[30],m_ZPTI[30],m_ZJ,m_XW,m_WW[2];
	double RHF[10],LMHP[3];
	int m_nLTBPJS,m_nLZBPJS;
	double LnewYd,RnewYd;//路面中心的地面高程
	int JqPtNum,XGJqPtNum;
	AcGePoint3d JqPt[29],XGJqPt[29];//边沟浆切，线间沟浆切
	double GK,GS,GP1,GP2,CG_JQHD,CG_GDR;//边沟信息
	double BPLen,BPH;//边坡长与高
	double m_ArcBpR[4];//0-路堤土路肩，1-路堤坡脚，3-路堑坡顶，2-路堑坡脚
	double m_sfactor;

	int m_GouPropty;//水沟属性 0-路基水沟 1-路堑水沟 2-推平水沟，填平水沟，路基上设置的路堑沟，路肩沟
	int DesignLineColor,lmhdcolor;

	bool m_IsModifyGutter;
	double m_GutterThick;

	//20191009	新增路堤水沟和天沟的参数，用以实现水沟参数传递
	int m_GutterType;
	//路堤水沟参数	挡水堰宽	护坡道宽	挡水堰厚	水沟厚	水沟宽	护坡道厚	内边坡	外边坡	挡水堰外侧边坡	浆砌厚度	沟底半径
	double m_LDG_T2W, m_LDG_T1W, m_LDG_T2H, m_LDG_GH, m_LDG_GW, m_LDG_HH, m_LDG_SP1, m_LDG_SP2, m_LDG_SP3, m_LDG_JQHD, m_LDG_GDR;
	//截水沟（天沟）参数	边距	沟宽	沟深	内坡率	外坡率	浆砌厚度	沟底半径
	double m_TG_BJ, m_TG_GW, m_TG_GS, m_TG_SP1, m_TG_SP2, m_TG_JQHD, m_TG_GDR;
	
	EarthLine_ROAD *l_earthline,*r_earthline;
	LTGOU_ROAD *LTGou;
	JSGOU_ROAD *JSGou;
	HdmText_ROAD *Note;
	AcDbObjectId gouId;

	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);	
	Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
											   AcDbIntArray& osnapModes,
											   AcDbIntArray& geomIds)const;
    Acad::ErrorStatus DesignLine_ROAD::subMoveGripPointsAt(const AcDbIntArray& indices,
												  const AcGeVector3d& offset);
	Acad::ErrorStatus subGetTransformedCopy( const AcGeMatrix3d& mat,AcDbEntity*& ent) const;

	void SetBPStr(double FillORCut, int ptnum, AcGePoint3d DesPoints[],double bp[30],AcGePoint3dArray &SlopPt,int ZorY);
   // void SetBPStr(double FillORCut, int ptnum, AcGePoint3d DesPoints[],double bp[3],AcGePoint3dArray &SlopPt,int ZorY);
    virtual void subList() const;
//	int Cal_Yd(double x,double &Yd1,int &ptnum);
	int Cal_Yd(double x,double &Yd,int &ptnum,AcGePoint3dArray &despts);
    int Set_LM(int mode);
	int Set_BP(int mode);
	void Draw(double NoteTextH=4.0);
	void BZDesh(AcGiWorldDraw* pWd,AcGePoint3d notept,double desh);
	void CalBP();
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler *filer) const;
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler * filer);
		Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		int gsSelectionMark,
		const AcGePoint3d & pickPoint,
		const AcGePoint3d & lastPoint,
		const AcGeMatrix3d& viewXform,
		AcGePoint3dArray & snapPoints,
		AcDbIntArray& geomIds) const ;
   bool DesArcBp(double R,int JiaoDianNum,AcGePoint3d arcpt[31],char TorW);	
   bool DesArcTFBpWithLTgou(double R,int JiaoDianNum,AcGePoint3d arcpt[31],char TorW);			 
   int DesTopArcBp(double R,AcGePoint3d arcpt[31],char TorW);	
   int CalDmxJD(double Xo,double Yo,double R,double jd[2]);	 
   void CGhasJQ(int zory,AcGePoint3d m_DesignPoints[],double CG_JQHD,double CG_GDR,int &JqPtSum,AcGePoint3d JqPtArray[]);
   void cal_JQpt(int zory,int LQGorLDG,int GouPtSum,AcGePoint3d GouPtArray[],double JQHD,int &JqPtSum,AcGePoint3d JqPtArray[]);

   //检测侧沟是否存在
   void CheckLateralGutterExist();
   //设置侧沟参数
   void SetLateralGutterPara(double ParaGK, double ParaGS, double ParaGP1, double ParaGP2, double ParaCG_JQHD, double ParaCG_GDR);
   //设置路堤沟参数
   void SetLDGPara(double T2W, double T1W, double T2H, double GH, double GW, double HH, double SP1, double SP2, double SP3, double JQHD, double GDR);
   //设置天沟参数
   void SetTGPara(double BJ, double GW, double GS, double SP1, double SP2, double JQHD, double GDR);

   void NoteRoadCrossSlope(double NoteTextH=4.0);
};
int MAKE_ACDBOPENOBJECT_FUNCTION(DesignLine_ROAD);
#endif // !defined(AFX_DESIGNLINE_H__2F740861_8F0C_11D4_AF18_006067772FF4__INCLUDED_)
