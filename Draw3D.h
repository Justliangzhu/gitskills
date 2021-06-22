// Draw3D.h: interface for the CDraw3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_)
#define AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XLDataBase.h"
#include "JD_CENTER.h"

class CDraw3D : public BAS_DRAW_FUN
{
public:
	CDraw3D();
	virtual ~CDraw3D();
	double Create3DModel();
	void ReadHdmResultFile();
	void ReadACrossResultData(FILE *fpr,int jcross);
	int draw_fill_cut();
	int drawfillandcut(int i,int ZorY,double qdeltah,double hdeltah,double qtw,double htw,int qdmxz,int hdmxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr);
    double FindBriAng(double cml);
	void DrawPartFillBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
		AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
	void DrawCutPartBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
		AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
	void CalTWZeroPt(double bml,double eml,double Bdeltah,double Edeltah,double &k,double &MidMl);
   void AddBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,double x,xlpoint &PZtmp);
   void DrawBriBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void AddSideBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,xlpoint &PZtmp);
    void CreateBriPt(int jcross,int brixz,AcGePoint3dArray &Ptarr,int ZorY);
	void CreateTunPt(int jcross,AcGePoint3dArray &Ptarr,int ZorY);
   void AddSinTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp);
   void DrawTunBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void AddDouTunBodyPt(AcGePoint3dArray &Ptarr,int LorR,xlpoint &PZtmp);
   void DrawTunDoor(int ZorY,int scross, int ncross);
   void Draw_TunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr);
   void Draw_DouTunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr);
   void DrawLM(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void draw_circleBase(AcGePoint3d CenPt,double D,double H);
   void draw_BaseCape(AcGePoint3d Lpt,AcGePoint3d Rpt,double fwj);
   void draw_bridge_base(double ang,CString XZ);
   void GetCmlKKSY(int ibri);
   void CalDeshLmk(double cml, double &zmh, double &ymh,double &Zwd, double &Ywd);
   void draw_BrigeSlop();
   int Cal_ZPJD(int ZorY,int HorT,AcGePoint3d Cenpt,AcGePoint3d Spt,AcGePoint3d Ept,AcGePoint3dArray &ptarr);//计算锥坡脚点
	int draw_Terrain();
	void CalBriSlopPt(int i,int ZorY,AcGePoint3dArray &ptarr);
	void CalOffsetByXLZX(double cml,int NLMK,LMKdata lmk[],double& OffsetByXLZX);
	void ReadLMK(CString LMKTABname);
	
   int KNum;
   struct brginf{
	   int ks;
	   double klen;
   } BrgArray[100];
   
   struct BriLSEml
   {
	   double sml;//该梁的起始里程	
   } BrgLArr[1000];//梁号对应数组下标
   int nBriL;

	JD_CENTER *pm;
	XLDataBase DBS;

	double SmlOnZx,EmlOnZx,SPXJJ;
	//起终路肩分离点在主线上的投影里程
	//	double LJFLSmlOnZx,LJFLEmlOnZx,LJFLSmlOnFLD,LJFLEmlOnFLD;
	//起终边坡分离点在主线上的投影里程
	double BPFLSmlOnZx,BPFLEmlOnZx,BPFLSmlOnFLD,BPFLEmlOnFLD;
	
	int NCRS;
	struct CRSDAT
	{
		ACHAR ckml[80];
		double cml;
		double dml;
        int DorS;//1-单路面，2-双路面
		double DuanLuoInf;
		double xjj;//线间距，单线为0,二线在左为-，右为+
		double ZDesH,YDesH;
		double LZDK,RZDK;
		//DesH路肩的设计高程和EarH地面高程（HDM的绘图数据）//指左线
		double RHFL[10],RHFR[10];  //主线路面点5个
		double zlmhd[3],ylmhd[3];
		int ZDMXZ,YDMXZ;//左右幅断面性质
		double  Zt,Yt;
		double ZHc,YHc,Y_ZHc,Y_YHc;//填挖高
		int zptsum,rptsum,Lnpt,Rnpt;
		AcGePoint3dArray RPTL,RPTR;
		AcGePoint3dArray PTRPTL,PTRPTR;
		int Ljdnum,Rjdnum;
		//指右线
		double FLDcml;//相关道路投影里程
		CString FLDname;//相关道路名
		double Y_RHFL[10],Y_RHFR[10];  //分离段路面点5个
        double Y_zlmhd[3],Y_ylmhd[3];
		int Y_ZDMXZ,Y_YDMXZ;//左右幅断面性质
		double  Y_Zt,Y_Yt;
		int Y_Lnpt,Y_Rnpt;
		AcGePoint3dArray Y_RPTL,Y_RPTR;
		AcGePoint3dArray Y_PTRPTL,Y_PTRPTR;
		int Y_Ljdnum,Y_Rjdnum;
		
		double Ldeltah,Rdeltah;//左右路肩点与地面线的高层差
		double Y_Ldeltah,Y_Rdeltah;//左右路肩点与地面线的高层差
		double newLYd,newRYd;//左路基中心的地面高层，右路基中心的地面高层
		double  at,aw;
		double Ldh,Rdh;
		double DX;
		//		//左线或一线
		int LGouPtNum,RGouPtNum,LJSGouPtNum,RJSGouPtNum;////////wwq5.14
		int LGouProperty,RGouProperty;
		double Lgou[36],Rgou[36],LJSgou[36],RJSgou[36];//水沟7点/////wwq5.14
		AcGePoint3dArray PTLgou,PTRgou,PTLJSgou,PTRJSgou;
		//	
		//		//右线
		int Y_LGouPtNum,Y_RGouPtNum,Y_LJSGouPtNum,Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty,Y_RGouProperty;
		double Y_Lgou[36],Y_Rgou[36],Y_LJSgou[36],Y_RJSgou[36];//水沟7点/////wwq5.14
        AcGePoint3dArray Y_PTLgou,Y_PTRgou,Y_PTLJSgou,Y_PTRJSgou;	
		
		int IfHasZlink,IfHasYlink;
		CRSDAT *zlink,*ylink;
		double ZDorS,YDorS;
		double ZCG_GDR,YCG_GDR;
		//左线或一线		
		double OffsetByXLZX;
	};	
	CRSDAT *hdmdes;

	int NZLMK,NYLMK;
	LMKdata *ZLMK,*YLMK;
};

#endif // !defined(AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_)
