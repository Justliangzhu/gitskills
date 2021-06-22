// GTZX.h: interface for the GTZX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
#define AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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
#include "BAS_DRAW_FUN_RAIL.h"
#include "triDTM_road.H"
//#include "GTZX2.h"
#include "GTZX_JYX.h"
#include "GTZX2_JYX.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 

//		自定义类

//   1、交点定义
//   MNJD 允许的最多交点数（含起、终点），NJD--实际交点数（含起、终点），
//   IJD--第I个交点（起点0，终点NJD-1），JDGH--交点冠号，
//   JDarray[i][6] --交点 序号、N、E、R、L1、L2；JDQXB[i][]--ZH、HZ、T1、T2、L
//   JDPXYINDATA[i][3][5]--交点线元
//   DLB[j]--断链表（含冠号）
//   PDB[k][]--坡度表

class GTZX_ROAD : public BAS_DRAW_FUN_RAIL ,public AcDbEntity ,public triDTM_road
{
public:

	bool IsdrawTZD;
	bool IsdrawLC;
	bool IsdrawXLM;
	bool IsdrawBPD;
	bool IsdrawJDLX;
	bool IsdrawJD;
	
	

	  bool isReFresh;

	  //////////////////////////////////////////////////////////////////////////
	  //里程坐标转换函数组
	  //坐标转现场里程
	  //里程坐标转换函数组
	  //坐标转现场里程
	  void BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);
	  bool TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC=3);
	  bool TrsNEtoProjml(double N, double E, double& Projml);
	  bool TrsNEtoCml(double N, double E, double& Cml);
	  bool TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ);
	  bool TrsCkmltoProjml(ACHAR Ckml[], double& Projml);
	  bool TrsCkmltoCml(ACHAR Ckml[], double& Cml);
	  bool TrsCmltoNE(double Cml, xlpoint& PZ);
	  bool TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC=3);
	  bool TrsCmltoProjml(double Cml, double& Projml);
	  bool TrsProjmltoNE(double Projml, xlpoint& PZ);
	  bool TrsProjmltoCml(double Projml, double& Cml);
	  bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC=3);

	  void ReadXLGuifan();
       GuiFanCanShu mGuiFanCS;
	  //////计算坡度等
	  //变坡点数组
      BPDArray m_BPDArray;
	  int m_LJorGM;
	  int m_Xsw;
	  int m_DesHXSW;
	  //CM数组
	  double m_CM[5*MAXBPDNUM];
	  //CM数组总数
	  int m_NCM;
	  //变坡点数组转CM 数组
	  void TrsBPDArraytoCM();
	  //标注变坡点
	  bool BZCM(AcGiWorldDraw* pWd);//标注边坡点
	  //设置变坡点数组
	  void SetBPDArray(int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd);
	  void SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd);
	 // 计算cml处的高程
	  double rdes_level(double cml,double &slope);
	 // 由CM数组计算cml处的高程
	  double DLEVEL(int icm, double cml);
	  //线路关系数据
	  XLRelate m_XLRelate;
	  //需要与当前线路求交点的线路名数组
	  ACHAR m_NeedInterXLNameArray[8][64];
	  int m_NeedInterXLNum;
	  //约束错误信息
	  CArray<ConstraintErr,ConstraintErr> m_ConstraintErrArray;
	  //设置线路约束信息
	  void SetXLRelate(XLRelate& Relate);
	  //检测当前线路与其所有相交线路的交角与高差约束信息
	  void CheckAllXLConstraint();
	  //标注违反相交线路约束信息
	  void DrawXLConstraint(AcGiWorldDraw* pWd);
	  //获取引出接入接坡数据
	  void GetJPData(CString JPXLName,bool isStart, double& Lever, double& Slope);
	  //刷新与当前线路相交的相关线路实体
	  void RefreshRelateXL();
	  void INSERT_STATION(ACHAR CKml[80],ACHAR StaName[80],ACHAR TKSta[80],AcGiWorldDraw* pWd);
	  int station_num;
	  struct
	  {
		  ACHAR CKml[80];//中心里程
		  ACHAR StaName[80];//车站名 
		  ACHAR TKSta[80];//插旗图块名
		  
	  }Station[100];
	  void Invert();
	  //plh04.14//给定统一里程，是否向前搜索，返加与附近断链处统一里程的差值，tylc>断链处lc...+
	  double DistToNearDL(double tylc,bool IsForwardTo);
	  //plh04.14
	  double LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo);
	  //plh04.14
	  int GetPreJD(double N,double E);
	  //plh04.14
	  int DistToNearJD(double tylc,bool IsForwardTo=true);
	  //plh04.14
	  //修改指定交点
	  void MODIFYJD(int JDNO,double N, double E, double L1, double R, double L2);
	  //plh04.14
	  //尾部增加交点
	  void ADDJD(double N,double E);

	  Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
	  Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);
	  //  由断链数据表 求某统一里程的现场里程和冠号
	  double   XLC( double TYLC,ACHAR *GH, int NDL);
	  double   XLC( double TYLC,ACHAR *GH, int NDL,int &iDL);
	  //  由断链数据表 求某现场里程的统一里程
	  double  TYLC(ACHAR ckml[80], int NDL );
	  
	  void  FormLCB(double dlc,double sml,double eml) ;
	  void AddTZDtoLCB(); //将曲线特征点加入里程表 
	  void AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT); //将桥隧站加入里程表
	  //输出到struct LCB里程标表
	  void VIEW_PM_ZDM(int k);
	  void VIEW_PM_PQX(int k);
	  int PM_PQX(double *pqx);
	  void outdata(CString str);
	  Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);
	  
	  void draw_DMX();
	  int PM_DMX(double *dmx);
	  double xl_length;
      double DV;//速度目标值
	  double RtjMin,RtjMax;//推荐半径
	  double ZXLmin1,ZXLmin2;//一般最小夹直线长，困难最小夹直线长
	  double Rmin1,Rmin2;//一般最小曲线半径，困难
	  CString TLDJ,ZXSM;//铁路等级
	  double XJJ;//标准线检举,0单线,>0二线在右側，<0二线在左側
	  double ZDMJG;//纵断面间距
	  ///方案组合数据
	  Adesk::Int16 pro;//本GTZX号码
	  CString mdbname;//本GTZX对应的mdb
	  Adesk::Int16 Zx[2];//本GTZX父方案Zx[0],子方案ZX[1],-1为空 直系亲属
	  int Pxout[20];//本GTZX引出方案,最多20个
	  int nout;//本GTZX引出方案个数
	  int Pxin[20];//本GTZX引入方案,最多20个
	  int nin;//本GTZX引入方案个数
	  double Sprjml;//本方案起点在父方案投影连续里程
	  double Eprjml;//本方案终点在子方案投影连续里程
	  double SelSml;//选择派生引入方案的控制里程,初始为终点连续里程
	  double SelEml;//选择派生引出方案的控制里程,初始为起点连续里程

    ACRX_DECLARE_MEMBERS(GTZX_ROAD);
	GTZX_ROAD();
	virtual ~GTZX_ROAD();

    GTZX_ROAD(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr);
	void Set(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr);
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
    Adesk::Boolean getpxy_DATA();

    void setTD(short int JDTDBZ0,short int draw_zybz0,short int qxbbz0);
	void  setINIT(int nlc,int nh,int Chinese_Engbz,double Texth);
	void  setJD_RL(int JDi ,double Ri,double L1i,double L2i);
    virtual int   getJD_RL(int JDi, double *Ri,double *L1i,double *L2i ,double *NN,double *EE);
    int get_pxy(int id,double *d0,double *d1,double *d2,double *d3,double *d4);  
	int savePM(ACHAR *fname);
	int saveDL(ACHAR *fname);
    int savePXY(ACHAR *fname);

    
	Adesk::Boolean   addXL(int JDi,double R,double L1,double L2 ,double NN ,double EE,double Nend ,double Eend );
    Adesk::Boolean   addJD(int JDi,double R,double L1,double L2 ,double NN ,double EE );
    Adesk::Boolean   DelJD(int JDi );
	
	int getDLM(int iDL,double  *BLC,double *DL, ACHAR *BGH,ACHAR *EGH);
	Adesk::Boolean   setDLM(int iDL,double BLC,double DL, ACHAR *BGH,ACHAR *EGH,int mode=1);
	Adesk::Boolean setDLM(int iDL,double BLC,double DL, CString BGH, CString EGH, int mode=1 );
    virtual Adesk::Boolean   addDLM(int iDL,double BLC,double DL, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(int iDL);
   
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
    virtual void GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);
    virtual void subList() const;
    Acad::ErrorStatus  subExplode(AcDbVoidPtrArray& entityset);
    //  virtual AcRxObject* clone() const;

	//
    virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		                                                AcDbIntArray& osnapModes,
														AcDbIntArray& geomIds) const;

    virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		                                       const AcGeVector3d& offset);

   virtual Acad::ErrorStatus subGetOsnapPoints(
           AcDb::OsnapMode   osnapMode,
           int               gsSelectionMark,
       const AcGePoint3d&    pickPoint,
       const AcGePoint3d&    lastPoint,
       const AcGeMatrix3d&   viewXform,
           AcGePoint3dArray& snapPoints,
          AcDbIntArray&     geomIds) const ;

	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);	
    Acad::ErrorStatus subGetTransformedCopy( const AcGeMatrix3d& mat,AcDbEntity*& ent) const;

	int mode_display;
	double JDarray[MAXJDNUM][6];  // 1->N,2->E,
	double array_pxy[MAXJDNUM][4][5];// QXLRL[100][3];
	double array[4*MAXJDNUM][10];//0：线元性质1-直线，2-圆，3-前缓， 4-后缓
	                      //8，9：线元起点x,y
	                      //6:线元起点里程
	                      //7:半径
	                      //4:左右转
	                      //5：起始切线方位角
	double QXB[MAXJDNUM][9]; 
	bool m_IfViewDmx;
//plhadd	zh里程,hy里程,yh里程,hz里程,a,T1,T2,L,左偏为-1(右1)
	//[NJD][9]
	
	Adesk::Int16 NJD,tarray;
	Adesk::Int16 NDL,NH,NLC; //NDL 断链点数（包括里程冠号改变点，如CK12+235=C1K12+235，为断链值是0的特殊断链点）
               // NLC--里程保留位数》=0  NH--高程保留位数》=0
	Adesk::Int16 Chinese_Engbz_type;  
  Adesk::Int16 JDTDBZ ;// JDTDBZ 交点拖动标志 ，JDTDBZ==1为沿切线拖动  =2 为切圆 =其他整数为任意拖动
  Adesk::Int16 draw_zybz,qxbbz;	//draw_zybz=-1 曲线四大桩和曲线参数标在左侧，=1为标在右侧  =0为按曲线偏向标
                            	//qxbbz=1 要标曲线参数，其他不标
	Adesk::Int16 JD0; //起始交点号
	DLBZ DLB[30];  // 含起点
  DLBZ DLBTMP[30];
	double texth;
	ACHAR JDGH[20];
  //  double Design_V;
	int KNDD;
//    int BZTZD1 (double array[400][5],int trow, char *GCH,int NDL,double texth ,double LCB,int zybz,int Chinese_Engbz_type ,int NLC );  //  标注里程
//    int BZLC1(double array[400][5],int trow,char *GCH,int NDL,double texth,double dlc,double LCB,int zybz,int type) ; //  标注里程;

	struct TZDLCB//特征点里程表
	{
		int tzdflag;
		//特征点类型，起点(0),终点(1),ZH(2),HY(3),QZ(4),YH(5),HZ(7)
		double lc;//统一里程
		double x,y;//平面坐标
		ACHAR bz[80];
	}pTzdlcb[5*MAXJDNUM];
	int NTZD;//特征点数
//	struct LCB//里程标表
//	{
//    	int lcflag;//里程类型，-1（公里标）,1-9百米标，0-断链标
//		double lc;//统一里程
////		double x,y;//平面坐标
////		char bz[80];
//	}pLCB[20000];
	struct LCB//里程标表
	{
		int lcflag;//里程类型，-1（公里标）,1-9百米标，0-断链标
		double lc;//统一里程
		//		double x,y;//平面坐标
		//		char bz[80];
	};
	CArray<LCB,LCB> pLCB;
	int NLCB;//桩数
	double StartCml,EndCml;
	ACHAR StartCKml[80],EndCKml[80];
	
	void FindZXLenMin();//找出最小夹直线长
	void checkpm();
	void  BZJD(AcGiWorldDraw* pWd,double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int trow,int JD0,ACHAR *JDchr );
   void  BZLC(AcGiWorldDraw* pWd,double dlc, int zybz,int type);
    void BZTZD(AcGiWorldDraw* pWd, double dlc, int zybz,int type) ; //标注里程
  
//private:
	int INIT();
	Adesk::Boolean  G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ, double ND);
    Adesk::Boolean  G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ );
    

    int BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC );  //  标注里程
    int BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type);  //  标注里程
    int BZJD1(double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int trow,int JD0, double texth,int draw_zybz,int qxbbz,int NLC);
    void draw_pm();
	//-----------------------------
    void VIEW_PM_TypeHdm();
 };
//ACRX_DXF_DEFINE_MEMBERS(GTZX, AcDbEntity,  0,  GTZX,"GUADS");
int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX_ROAD);
#endif // !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)





















