

// GTZX.h: interface for the GTZX class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
//#define AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

#if !defined(AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
#define AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

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
///

///
#include <dbjig.h>
#include <acgi.h>
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include <iostream>


#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN_JYX.h"

#include "StdAfx.h"
// #include "triDTM.H"
//#include "GJDGTZX.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 

//class GTZX2;

//		自定义类

//   1、交点定义
//   MNJD 允许的最多交点数（含起、终点），NJD--实际交点数（含起、终点），
//   IJD--第I个交点（起点0，终点NJD-1），JDGH--交点冠号，
//   JDarray[i][6] --交点 序号、N、E、R、L1、L2；JDQXB[i][]--ZH、HZ、T1、T2、L
//   JDPXYINDATA[i][3][5]--交点线元
//   DLB[j]--断链表（含冠号）
//   PDB[k][]--坡度表
class GTZX_JYX_ROAD : public BAS_DRAW_FUN_JYX ,public AcDbEntity 
{
public:

	bool IsdrawTZD;
	bool IsdrawLC;
	bool IsdrawXLM;
	bool IsdrawBPD;
	bool IsdrawJDLX;
	bool IsdrawJD;
	//////////////////////////////////////////////////////////////////////////
	//里程坐标转换函数组
	//坐标转现场里程
	void BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);//标注竖曲线
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


	//////计算坡度等
	BPDArray m_BPDArray;
	double m_CM[5*MAXBPDNUM];
	int m_NCM;//CM数组总数
	void TrsBPDArraytoCM();//变坡点数组转CM 数组
	void SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd);
	// 计算cml处的高程
	double rdes_level(double cml,double &slope);
	// 由CM数组计算cml处的高程
	double DLEVEL(int icm, double cml);
	//标注变坡点
	bool BZCM(AcGiWorldDraw* pWd);
	//////////////////////////////////////////////////////////////////////////

	GuiFanCanShu m_GuiFanCS;

	bool IsEXRXD;
	struct JZXStruct
	{
		xlpoint QJZXStartPt,QJZXEndPt;//前一夹直线的起终点
		xlpoint HJZXStartPt,HJZXEndPt;//后一夹直线的起终点
		int JDNum;//当前交点号
	};

	CArray<JZXStruct , JZXStruct> JZXArray;//夹直线数组

	CArray<struct GJDInf,struct GJDInf> GJDInfTab;

	int NGJD;

	BOOL IsReSetColor ;

	struct GJDInforStruct
	{
		double spt[2];//起点坐标
		double ept[2];//终点坐标
		int GJDType;//改建段类型，0-并行改建 1-S弯 2-绕行改建   // -１ - 完全利用
		ACHAR BXLName[256];
		ACHAR EXLName[256];

		ACHAR gjdname[256];//外部改建段名

		ACHAR GJDBLC[50];//起点里程
		ACHAR GJDELC[50];//终点里程

	};

	struct LYD 
	{
		double sml;
		double eml;
	};
	//实体



	struct SetLineWeightStruct
	{
		AcDb::LineWeight m_LineWeight ;
		int m_XLKuanInt;
	};

	//设置颜色和线宽
	void setColorLineW();

	//设置颜色
	void SetAllColor(int ColorIndex);

	//半径与缓长的规范检测
	void RTolCheckUp( BOOL IsGaiJian );

	AcDb::LineWeight TranLineWeightFromIntToEnum( int LineWeight);

	//设置标准数据
	void SetStandardXLKuanFormat( CArray< SetLineWeightStruct , SetLineWeightStruct & >&  LineWeightArrayToOut );


	bool JudgeGTZX2GJDArrayChanged(CArray<struct GJDAllInforStruct,struct GJDAllInforStruct>& p_OriGJDArr,CArray<GJDAllInforStruct,GJDAllInforStruct>& p_NewGJDArr,CArray<int,int>& p_ChangedIndexArr);

	
	//实体里改建段信息数组
	CArray<GJDInforStruct,GJDInforStruct>GJDInforArray;

	int GJDInforArrayNum;

	//	BOOL isQXGJ; //是否是曲线改建
	//	UltraCArray_T<GJDStruct,GJDStruct&> TempGJDJDArray;

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

	Adesk::Boolean G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
	Adesk::Boolean G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);
	//  由断链数据表 求某统一里程的现场里程和冠号
	double   XLC( double TYLC,ACHAR *GH, int NDL);
	double   XLC( double TYLC,ACHAR *GH, int NDL,int &iDL);
	CString  XLC( double TYLC);
	//  由断链数据表 求某现场里程的统一里程
	double  TYLC(ACHAR ckml[80], int NDL );

	void  FormLCB(double dlc,double sml,double eml) ;
	void AddTZDtoLCB(); //将曲线特征点加入里程表 
	//输出到struct LCB里程标表
	void VIEW_PM_ZDM(int k);
	void VIEW_PM_PQX(int k);
	int PM_PQX(double *pqx);
	void outdata(CString str);
	Adesk::Boolean G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);

	CString YCXLName , JRXLName ;

	int AllColor; //所有颜色
	int GTZXJDToJDColor;  //平面交点连线颜色

	int XWArcColor; //线位圆弧

	int HuanHequxianColor; //

	int JiaZhiXianColorZuo,JiaZhiXianColorYou; 

	int mdbnameBoxColor,mdbnameTextColor;


	int QXYSColor;


	AcDb::LineWeight  LineWeightValue ; 
	/*
	ACHAR BXLSXName[50];//起点线路名
	ACHAR EXLSXName[50];//终点线路名
	*/

	//绘制影响范围里程
	void  DrawYXFW(AcGiWorldDraw* pWd, ACHAR GTZXLC[50],ACHAR GJDGTZXLC[50],int flag,ACHAR GJDName[255]);

	//设置线路格式
	AcDb::LineWeight SetXLKuanFormat( int XLKuanInt );
//	draw_DMX();


	int PM_DMX(double *dmx,double ZDMJG,double ZJ,double sml,double eml);
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
	CString XLShuXing;   //线路性质：基线，左线 右线

	Adesk::Int16 Zx[2];//本GTZX父方案Zx[0],子方案ZX[1],-1为空 直系亲属
	int Pxout[20];//本GTZX引出方案,最多20个
	int nout;//本GTZX引出方案个数
	int Pxin[20];//本GTZX引入方案,最多20个
	int nin;//本GTZX引入方案个数
	double Sprjml;//本方案起点在父方案投影连续里程
	double Eprjml;//本方案终点在子方案投影连续里程
	double SelSml;//选择派生引入方案的控制里程,初始为终点连续里程
	double SelEml;//选择派生引出方案的控制里程,初始为起点连续里程

	ACRX_DECLARE_MEMBERS(GTZX_JYX_ROAD);
	GTZX_JYX_ROAD();
	virtual ~GTZX_JYX_ROAD();


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

	//	Adesk::Boolean  GTZX::setBZLC(int iDL,double BLC,double DL, ACHAR *GH, int mode );

	virtual Adesk::Boolean   addDLM(int iDL,double BLC,double DL, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(int iDL);


	//添加标志里程
	void  addBZLCToGTZX(ads_point PT ,ACHAR CurBZLC[30]);

	//删除标志里程
	void DeleteBZLCToGTZX(ads_point PT);

	//传入交点下标号 计算出zh或hz点坐标 zhorhz=-1 zh; zhorhz=1 hz
	Adesk::Boolean  getZhHzPt(int iJD,int zhorhz,double pt[2]);

	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	virtual int GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);
	virtual void subList() const;
	Acad::ErrorStatus  explode(AcDbVoidPtrArray& entityset);
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

	virtual Adesk::Boolean MakeGJDsToOne();
	int GetCurJDno(double N,double E);

	int mode_display;
//	ACHAR m_AddJDGH[20];
	//	double  JDarray[MAXJDNUM][6];  // 1->N,2->E,
	//	double QXB[MAXJDNUM][9];	
	/*	typedef	struct JDarrayStruct
	{ //交点结构数组
	ACHAR JDNum[20];
	double N;
	double E;
	double l1;
	double R;
	double l2;
	}JDarrayStruct;*/

	CArray<struct JDarrayStruct,struct JDarrayStruct>JDarray;


	GTZX_JYX_ROAD(int iNJD,CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH,ACHAR *JDchr,BOOL isConnectNewGTZXToIn = FALSE , BOOL isJXflag = 0 , BOOL IsGaiJian = FALSE );


	//设置线路颜色
//	void SetXLColor( CString XLNameToIn );

	//1 传入三个指针,改建段信息表,贯通线信息

//	GTZX( int iNJD , CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH , ACHAR *JDchr , 
//		 CArray<GJDInf,GJDInf>& GJDInfTabArrayToIn);
	//GTZX( int iNJD,CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH,ACHAR *JDchr,BOOL isConnectNewGTZX = FALSE);
	//2　贯通线的利用\改建 信息表
	// 3 xlPoint_Pz（）
	//　4对改建百米函数

	//5开始＼终止坐标　和开始＼终止断链编号，生成子的ＧＴＺＸ指针

	double array_pxy[MAXJDNUM][4][5];// QXLRL[100][3];

	void GetArrayCS(CArray<struct QXBStruct,struct QXBStruct>&array,ACHAR CurJD[20],int &firstNum,int &AllNum);

	//*****************  
	double array[4*MAXJDNUM][10];//0：线元性质1-直线，2-圆，3-前缓， 4-后缓
	//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大

	//(R1>R2)/////////////////////////////////////////////////////////////////////////
	//1:			L		R    A         A              A                A        //
	//2:					L	 R         R              R1               R2       //
	//3:                                                  R2               R1       //
	//////////////////////////////////////////////////////////////////////////////////
	//4: 左右转
	//5: 起始切线方位角
	//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
	//7: 半径 R
	//8，9：线元起点坐标 N,E
	//最后一个线元为终点
	/*

	*/


	//曲线表
	CArray<QXBStruct,QXBStruct>QXB;

	BOOL isConnectNewGTZX ;

	//原始的曲线表
	//	CArray<QXBStruct,QXBStruct>YSQXB;

	//既有曲线表
	//	CArray<QXBStruct,QXBStruct>JYQXB;

	//zh里程,hy里程,yh里程,hz里程,a,T1,T2,L,左偏为-1(右1),交点号
	Adesk::Int16 JYQXBNum;
	//JYQXB 

	bool m_IfViewDmx;
	//plhadd	zh里程,hy里程,yh里程,hz里程,a,T1,T2,L,左偏为-1(右1),  //交点号
	//[NJD][9]
	void DrawBZTZD(AcGiWorldDraw* pWd,int zybz,ACHAR str[]);
	xlpoint  PZtmp;
	int NewADDJDXiaoBiao;  //单交点增加的下标

	CArray<struct BZLCStructTable,struct BZLCStructTable>BZLCarray;
	Adesk::Int16 NBZLC;

	AcDbObjectId GJDeId; //改建段id
	Adesk::Int16 NJD,tarray;  
	Adesk::Int16 NDL,NH,NLC; //NDL 断链点数（包括里程冠号改变点，如CK12+235=C1K12+235，为断链值是0的特殊断链点）
	// NLC--里程保留位数》=0  NH--高程保留位数》=0
	Adesk::Int16 Chinese_Engbz_type;  
	Adesk::Int16 JDTDBZ ;// JDTDBZ 交点拖动标志 ，JDTDBZ==1为沿切线拖动  =2 为切圆 =其他整数为任意拖动
	Adesk::Int16 draw_zybz,qxbbz;	//draw_zybz=-1 曲线四大桩和曲线参数标在左侧，=1为标在右侧  =0为按曲线偏向标
	//qxbbz=1 要标曲线参数，其他不标
	Adesk::Int16 JD0; //起始交点号

	DLBZ DLB[MAXDLNUM];  // 含起点
	DLBZ DLBTMP[MAXDLNUM];
	//	BZLCStructTable BZLC[30];
	double texth;
	ACHAR JDGH[20];

	//  double Design_V;
	int KNDD;
	//    int BZTZD1 (double array[400][5],int trow, ACHAR *GCH,int NDL,double texth ,double LCB,int zybz,int Chinese_Engbz_type ,int NLC );  //  标注里程
	//    int BZLC1(double array[400][5],int trow,ACHAR *GCH,int NDL,double texth,double dlc,double LCB,int zybz,int type) ; //  标注里程;

	struct TZDLCB//特征点里程表
	{
		int tzdflag;
		//特征点类型，起点(0),终点(1),ZH(2),HY(3),QZ(4),YH(5),HZ(7)
		double lc;//统一里程
		double x,y;//平面坐标
		ACHAR bz[80];
	}pTzdlcb[5*MAXJDNUM];
	int NTZD;//特征点数
	//struct LCB//里程标表
	//{
	//	int lcflag;//里程类型，-1断链,1-9百米标，0-公里标
	//	double lc;//统一里程
	//	//		double x,y;//平面坐标
	//	//		ACHAR bz[80];
	//}pLCB[30000];
	struct LCB//里程标表
	{
		int lcflag;//里程类型，-1断链,1-9百米标，0-公里标
		double lc;//统一里程
		ACHAR ckml[20];//现场里程
		//		double x,y;//平面坐标
		//		ACHAR bz[80];
	}*pLCB;
	int NLCB;//桩数
	double StartCml,EndCml;
	ACHAR StartCKml[80],EndCKml[80];

	//获得连续里程sml和eml间的断链号,放在DLno中
	void GetDLBno(double sml,double eml,CArray<int,int&>&DLno);
	void FindZXLenMin();//找出最小夹直线长
	void checkpm();

	void BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb,int njd);
	//	BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb ,int trow);
	Adesk::Boolean BZLC(AcGiWorldDraw* pWd,double dlc, int zybz,int type);

	Adesk::Boolean BZLC2(AcGiWorldDraw* pWd,double dlc, int zybz,int type);   //标志里程 
	void BZTZD(AcGiWorldDraw* pWd, double dlc, int zybz,int type) ; //  标注里程

	int INIT( BOOL IsGaiJian );
	Adesk::Boolean G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ, double ND);
	Adesk::Boolean G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ );

	int BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC );  //  标注里程
	int BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type);  //  标注里程
	int BZJD1(CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb ,int trow, double texth,int draw_zybz,int qxbbz,int NLC);
	void draw_pm();
	//-----------------------------
	void VIEW_PM_TypeHdm();
	void CalAJDT1T2(int ijd,double &T1,double &T2);
	double CalcmlTonewcml(double kmlTocml);
	double CalnewcmlTocml(double newcml);
	Adesk::Boolean BZTZD2(AcGiWorldDraw* pWd, double dlc, int bz_zybz,int Chinese_Engbz_type);  //  标注里程
	double TYLC(double dml);
	void SetBZLCVal();

	void OnReCalBZLC(CArray<BZLCStructTable,BZLCStructTable>& m_BZLC);
	void CalT1T2L();
	//	void MergeQXBandJYQXBToQXB();
	void DrawPMXL(AcGiWorldDraw* pWd);
	void DrawPMCSSZ(AcGiWorldDraw* pWd);
	Adesk::Boolean DrawGTZXPMXL(AcGiWorldDraw* pWd);
	int GetPreOrAftJD(double N, double E,bool IsForwardTo);
	double DistFromPtToGTZX(double x, double y);//线外一点到线路距离

	//////////////////////////////////////////////////////////////////////////

	void CreatJZXArray();//创建夹直线数组
	bool IsJZXCD(int iJD2,GTZX_JYX_ROAD *pGTZX,int QorH,int iSJD1, int iEJD1,int &iJD1);//夹直线是否重叠
	int SearchGJDSptIndex(int sjdn, int ejdn ,GTZX_JYX_ROAD *pGTZX,int &iJD1);//搜寻改建段起点交点下标
	int SearchGJDEptIndex(int sjdn, int ejdn ,GTZX_JYX_ROAD *pGTZX,int &iJD1);//搜寻改建段终点交点下标
	int CreatGJDInfTab(GTZX_JYX_ROAD *pGTZX);//创建改建段表

	int CreatGJDInfTab(GTZX_JYX_ROAD *pGTZXGJ,int JDShuxingArray[]);//创建改建段信息表

	int CreatBXGJD(GTZX_JYX_ROAD* pGTZXGJ,int GJJDStart,int GJJDEnd,int JYJDStart,int JYJDEnd,double spt[2],double ept[2]);//创建并行改建段

	void SortGJDArray();//改建段排序

	void MergeGJDArray();//合并改建段




	void SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256]);
//	void SetDefaultGjdName(int GJDType,ACHAR GJDName[256]);
	void CalGJDQZZB(int iJD1,int iJD2,int QDorZD, GTZX_JYX_ROAD *pGTZX, double &PtX, double &PtY);//计算改建段起终点坐标
	void AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int IEJD1,int iEJD2,int GJDType);//添加改建段信息
	void AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int IEJD1,int iEJD2,int GJDType,CString xln);//添加改建段信息
	int GetGJDType(int sno1,int sno2,int eno1,int eno2,GTZX_JYX_ROAD *pGTZX);//获取改建段类型
	bool isZXPX(double SPt1[2],double EPt1[2],double SPt2[2],double EPt2[2]);

	//设置贯通线现场里程曲线表
	void SetCKQXB(CArray<QXBStruct,QXBStruct>& CKQXBToIn);

	/////////////////////////////////////////////////////////////


	double GTZXFit(GTZX_JYX_ROAD *pGTZX,double &XMove,double &YMove,double &DegRotate);//两线路匹配，寻求最佳的平移向量和旋转角度
	double CalJDPC(CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray,double XMove,double YMove,double DegRotate);//计算两交点数组中对应交点距离的平方和

	void powell(double p[], double xi[], int n, int np, double ftol, int& iter, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	void eraseall(double pbar[], double prr[], double pr[]);
	void linmin(double p[], double xi[], int n, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double brent(double ax, double bx, double cx, double tol, double& xmin, CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	void mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc, CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double f1dim(double x , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	int sgn(double x);
	double func2(double x[], int n , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double pcom[51], xicom[51];
	int ncom;
//	bool CreatJX(CArray<QXBStruct,QXBStruct> &QXBJYDC, GTZX *pGTZXZZJX);
//	GTZX_JYX* CreatJX(CArray<QXBStruct,QXBStruct> &QXBFromCL);

	//标识起\终
	void DrawBandEPoints(AcGiWorldDraw* pWd );

	//记录改建段标注的点数组
	CArray<AcGePoint3d , AcGePoint3d > MidPointArray;

	int MidPointArrayNum;

	void SearchGJD(double PickSPt[2], double PickEPt[2], CArray<GJDInf,GJDInf> &XGGJDInfArray, GTZX_JYX_ROAD *pGTZX,CString xln);

	void FormSectionInfor();

	CArray<GJDAllInforStruct,GJDAllInforStruct> BasicSectionInfor;//基本分段信息

	int xlpoint_pz_GTX(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ); 
	
	GTZX_JYX_ROAD * Get_GTX_RXGJD_GTZX(struct GJDAllInforStruct RXDBasicSectionInfor);

	CString GetGTXXclc(double N,double E);

	//计算贯通设计基线曲线表中现场里程
	void CalCKQXBToKmlForGTZX( CArray<QXBStruct,QXBStruct>& CKQXB );

	//判断Pt(N,E)点是否在改建段上，和改建段类型:-1-利用 0-并行改建 1-S弯改建 2-绕行改建
	int GetPtGJDType(double N, double E);

	int NBRI;
	double Bri[200][2];

	int NTUN;
	double Tun[200][2];

	int NSTA;
	double Sta[100][2];

	void  AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT); //将桥隧站加入里程表 

	bool GetPtGJDInf(double dml, int &GJDType, CString &GJDStartJYXLName, CString &GJDEndJYXLName);

	//由坐标计算里程
	void CalNEForKml();

	//由里程计算坐标
	void CalKmlForNE();

	//修正断链处的现场里程
	CString ModifyXCLCOnDL(double cml);

	//由投影里程计算贯通设计基线上的不带冠号的现场里程dml
	double DmlGTX( double TYLC,ACHAR *GH, int NDL,struct DLBZ DLBTMP[MAXDLNUM]);

	double DmlGTX(double TYLC,ACHAR *GH);

	void SortBZLCArray();
	
	AcGePoint3d m_XLNameNotePt;

	double CalnewcmlTocml(double newcml , double & K );

	void getGJDInfo(double GJD_Spt[2],double GJD_Ept[2],CArray<GJDInf,GJDInf>& OneGJDArray);

	int GetPZFromXCLC1X(CString XCLC1x,struct xlpoint& PZ);
	
	void CalCKQXBToKmlForGTZX(); 

	double SearchNearDLOnGTX(double ProjCml,bool IsForwardTo,int& DLIndex);//搜索贯通设计基线上投影里程为ProjCml的点距离最近的断链的距离

	void GetJYDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM]);//获取投影里程在Sml-Eml范围内的断链

	void GetGTXDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM]);//获取投影里程在Sml-Eml范围内的断链,不包括Sml，Eml

	int GetGT1xGJDIndex(double N,double E);//获取贯通基线上某点的改建段索引号，-1为利用段

	int JudgePtIsOnDL(double Cml);//传入几何统一里程，判断改点是否在断链上


	void GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,CArray<int,int>& p_arrJDIndex,CArray<JDarrayStruct,JDarrayStruct>& p_arrJD);

	void CalXlcByProjml(double cml, ACHAR kml[40]);

};




#endif // !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)


