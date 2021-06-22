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
	int hdmno;//��hdmdes�����еı��
	double BTinf;
	double cml;
	int m_DesignPointNum;
    AcGePoint3d *m_DesignPoints;
	double m_Xcdh,m_Yljh,m_Tljh,m_FillORCut;
    double m_TM[30],m_TH[30],m_TPTW[30],m_TPTI[30],m_WM[30],m_PH[30],m_ZPTW[30],m_ZPTI[30],m_ZJ,m_XW,m_WW[2];
	double RHF[10],LMHP[3];
	int m_nLTBPJS,m_nLZBPJS;
	double LnewYd,RnewYd;//·�����ĵĵ���߳�
	int JqPtNum,XGJqPtNum;
	AcGePoint3d JqPt[29],XGJqPt[29];//�߹����У��߼乵����
	double GK,GS,GP1,GP2,CG_JQHD,CG_GDR;//�߹���Ϣ
	double BPLen,BPH;//���³����
	double m_ArcBpR[4];//0-·����·�磬1-·���½ţ�3-·ǵ�¶���2-·ǵ�½�
	double m_sfactor;

	int m_GouPropty;//ˮ������ 0-·��ˮ�� 1-·ǵˮ�� 2-��ƽˮ������ƽˮ����·�������õ�·ǵ����·�繵
	int DesignLineColor,lmhdcolor;

	bool m_IsModifyGutter;
	double m_GutterThick;

	//20191009	����·��ˮ�����칵�Ĳ���������ʵ��ˮ����������
	int m_GutterType;
	//·��ˮ������	��ˮ�߿�	���µ���	��ˮ�ߺ�	ˮ����	ˮ����	���µ���	�ڱ���	�����	��ˮ��������	�������	���װ뾶
	double m_LDG_T2W, m_LDG_T1W, m_LDG_T2H, m_LDG_GH, m_LDG_GW, m_LDG_HH, m_LDG_SP1, m_LDG_SP2, m_LDG_SP3, m_LDG_JQHD, m_LDG_GDR;
	//��ˮ�����칵������	�߾�	����	����	������	������	�������	���װ뾶
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

   //���๵�Ƿ����
   void CheckLateralGutterExist();
   //���ò๵����
   void SetLateralGutterPara(double ParaGK, double ParaGS, double ParaGP1, double ParaGP2, double ParaCG_JQHD, double ParaCG_GDR);
   //����·�̹�����
   void SetLDGPara(double T2W, double T1W, double T2H, double GH, double GW, double HH, double SP1, double SP2, double SP3, double JQHD, double GDR);
   //�����칵����
   void SetTGPara(double BJ, double GW, double GS, double SP1, double SP2, double JQHD, double GDR);

   void NoteRoadCrossSlope(double NoteTextH=4.0);
};
int MAKE_ACDBOPENOBJECT_FUNCTION(DesignLine_ROAD);
#endif // !defined(AFX_DESIGNLINE_H__2F740861_8F0C_11D4_AF18_006067772FF4__INCLUDED_)
