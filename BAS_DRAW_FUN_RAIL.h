// BAS_DRAW_FUN1.h: interface for the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
// BAS_DRAW_FUN1.h: interface for the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
#include "resource.h"
#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <dbents.h>
#include "stdafx.h"
#include <aced.h>
#include <dbsymtb.h>
#include <adslib.h>
#include <dbxrecrd.h>

#include <string.h>

#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 
#include <acutads.h> 
#include <dbapserv.h> 
#include "BlockComputerFun.h"
#include <dbjig.h>
#include <acgi.h>
#include <math.h>
#include <assert.h>
#include "Hhqx.h"
//#include <iostream.h>
#include "rxdefs.h"
#include "BlockComputerFun.h"
//#include "ol_errno.h"
//#include "STDIO.h"
//#include "malloc.h"
#include "UltraCArray.h"
#include "StdAfx.h"

#if !defined(AFX_BAS_DRAW_FUN_RAIL_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
#define AFX_BAS_DRAW_FUN_RAIL_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_
#define pi 3.14159265358979323846
#define PI 3.14159265358979323846
#define MAXJDNUM 200
#define MAXBPDNUM 300
#define MAXDLNUM 200
#define PRECISION 1.0e-6
//缓和曲线精度
#define SPIRALSTEP 0.5
//无穷大
#define VERY_MAX 999999999999999999
//计算模式
#define KNOW_A1 1
#define KNOW_A2 2
#define KNOW_K_A1_A2 12

//左右转
#define LEFT -1
#define RIGHT 1
//交点属性
#define IsJiaoDian 0
#define IsCenter 1
//后接关系
#define NotRelation 0//无关
#define C_Link 1//C型
#define S_Link 2//S型
#define Luan_Link 3//卵型
//左右转
#define LEFT -1
#define RIGHT 1
//是否回头
#define HuiTou -1
#define NotHuiTou 1
//交点属性
#define IsJiaoDian 0
#define IsCenter 1
#define IsLine 2
#define IsArc 3
#define IsCircle 4
#define IsJdCenter 5

///////////////////////缓和曲线半径趋势变化标识///////////////////////////
//2005.5.12 hhp
///////////////////////线路单元起终点处的走向标识/////////////////////////
#define VerticalUp        8 	               		
#define ToFirstArea       1 
#define HorizontalRight   5
#define ToSecondArea      2
#define VerticalDown      6
#define ToThreeArea       3
#define HorizontalLeft    7
#define ToFourArea        4   
///////////////////////缓和曲线半径趋势变化标识///////////////////////////
#define LargeToSmall -2                         //无穷大到某一半径;
#define BigToSmall   -1                         //某一大半径到某小一半径;
#define NoChange      0						    //半径不发生改变如直线与圆;
#define SmallToBig    1						    //某一小半径到某一大半径;
#define SmallToLarge  2						    //某一半径到无穷大;
#define DataPrecision 0.00000001
///////////////////////定义线路单元偏向标识///////////////////////////////
#define DeflectRigth 1
#define NoneDeflect  0
#define DeflectLeft -1
///////////////////////定义线路单元偏向标识///////////////////////////////
//2005.5.14 hhp
///////////////////////定义线路单元类型标识///////////////////////////////
#define LineItem     1
#define CirOrArcItem 2
#define SprialItem   3
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct XYarray
{
	double xyarr[10];
	AcDbObjectId eID;
};

class BAS_DRAW_FUN_RAIL  
{
public:


	typedef struct
	{
		ACHAR InterXLName[64];//相交线路名
		double JJlowLimt;
		double GCmin;
	}InterXL;//线路关系数据结构
	typedef struct
	{
		ACHAR CurXLNme[64];//当前线路名
		ACHAR OutXLName[64];//引出线路名
		ACHAR InXLName[64];//接入线路名
		int  SXLDCnum;
		int  EXLDCnum;
		InterXL InterXLNameArray[8];//交叉线路名数组
		int InterXLSum;
	}XLRelate;//线路关系数据结构

	typedef struct
	{
		ACHAR CurCkml[20];
		ACHAR InterCkml[20];
		double InterN;
		double interE;
		double CurFwj;
		double InterFwj;
		double JiaJiao;
		double CurDesH;
		double InterDesH;
		double DetaH;
		double MinAng;
		double MinDetaH;
	}ConstraintErr;//不满足交叉约束的错误信息

	struct AssistLineSTU //辅助线线元表 
	{
		//		double (*XYArray)[10];// 
		double XYArray[4*MAXJDNUM][10];
		int XYNum;//线元数
		ACHAR FZXName[256];//辅助线名
		ACHAR TypeOfFZX[256];//辅助线类型	
	};

	struct AssistLineManage // 辅助线汇总表
	{
		ACHAR FZXName[256];//辅助线名
		ACHAR TypeOfFZX[256];//辅助线类型	
	};

	struct MetroStation 
	{
		double CX;
		double CY;//车站中心点坐标
		int Num;//站台个数,最大3个
		//int Style;//0岛式站台//1平行相对式侧式站台//2平行错开式侧式站台//3上下重叠式侧式站台//4上下错开式侧式站台//5岛、侧混合式站台
		CString strStyle;//0岛式站台//1平行相对式侧式站台//2平行错开式侧式站台//3上下重叠式侧式站台//4上下错开式侧式站台//5岛、侧混合式站台

		double   X1[3];
		double   Y1[3];//站台中心点坐标
		double   fwj[3];//站台方位角,与线路走向一致
		double   Length[3];
		double   Width[3];//站台尺寸
		CString  StaName;//站名
		double   dis;//站台间距
		double   Disf;//距前一车站的距离
		double   Disb;//距下一车站的距离
		CString   Mileage;//现场里程
		double statylc;//统一里程
		double CuoDong1;//平行错动车站的错动量
		double CuoDong2;//平行错动车站的错动量

	};

	struct SwitchTab 
	{
		CString Name;//道岔名
		ACHAR ckml[80];//现场里程
		double cml;//在各自线路上的统一里程
		double CenX;
		double CenY;//道岔坐标
		//int LorR;//离左线或是右线更近
		double fwj;//所在正线的方位角
		int SwitchNum;//道岔号数
		int SwitchType;//道岔类型
		double cmlOfJX;//基线上的统一里程
		/////////////////////////////////////////增加标高 坡度
		double lever;//标高
		double slope;//坡度

	};
	struct GuiFanCanShu
	{
		ACHAR TieLuDengJi[24];//铁路等级1
		int SuDuMuBiao;//速度目标2
		short ZhXianShu;//正线数目3,1表示单线，2表示一次双线，3表示双线预留二线
		float XianJianJu;//线间距4
		int MinR1;//一般最小线半径5
		int MinR2;//困难最小线半径6
		float XianZhiPo;//限制坡度7下行
		float XianZhiPo1;//限制坡度7上行
		int DaoFaXian;//到发线8
		short GuiDao;//轨道类型：0-4：无逢特-无逢轻，5-9：有逢特-有逢轻
		short QianYin;//牵引种类：1-电力，2-内燃，3-蒸汽
		ACHAR JiChe[24];//机车类型11
		ACHAR BiSai[24];//闭塞12
		ACHAR KongZhi[24];//列车控制方式13
		ACHAR ZhiHei[24];//列车指挥方式14
		short RXiangShu;//采用的半径系列项数，最多20
		int R[20];//曲线半径系列
		int L[20];//缓和曲线长度配置，与曲线半径一一对应：R[i]配L[i]
		int TJianR1;//推荐半径小
		int TJianR2;//推荐半径大
		int MinZhi;//最小直线长度
		int MinYuan;//最小圆曲线长度
		int JiaKuan[20];//曲线加宽，与曲线半径一一对应：R[i]配JiaKuan [i]
		int MinPoChang[7];//最小坡段长度，对应于6种到发线长度
		int MaxPoCha[7];//最大坡差
		float ShuTiaoJ;//竖曲线条件
		int ShuR;//竖曲线半径
		bool ZheJian;//曲线折减
		int SuiDaoFJ[4];//隧道长度分级
		float SDXiShuD[5];//隧道折减系数（电力）
		float SDXiShuN[5];//隧道折减系数（内燃）
		float JiBiaoHou;//基床表层厚度
		float WuFJiaK;//路基面无缝加宽
		float LuJiM1;//单线路堤路基面
		float LuJiM2;//单线路堑路基面
		float LuJiM3;//双线路堤路基面
		float LuJiM4;//双线路堑路基面
		short QuXianFJi;//曲线半径分级数5
		int QuXianRFJ[5];//曲线半径分级
		float LuJiMJiaK[6];//路基面曲线加宽值
		ACHAR SheJiJieDuan[24];//设计阶段
	};
	/////////////////////////////////////////////09.10.17
	void RToS(double lc,int NLC,ACHAR ch[80]);

	////////////////////////////////////////////////
    void CheckIfXLintersect(double array1[4*MAXJDNUM][10],double JDarray1[MAXJDNUM][6],int NunJD1,double array2[4*MAXJDNUM][10],double JDarray2[MAXJDNUM][6],int NunJD2,AcGePoint2dArray* JDpt);
	bool IfTriangleXJ(AcGePoint3dArray Triangle1pt,  AcGePoint3dArray Triangle2pt);
	bool IfXJandCaljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd);
	bool checkIfXJ(double array[10],double array2[10],double depth);
	void ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);
	int GetXdataFromEntity(AcDbObjectId CurID,XYarray &xray);
	double angleX(AcGePoint3d sPtL,AcGePoint3d ePtL);//Line(sPtl,ePtl)
	double angleX(AcDbLine *pL);
	double distfwj(double xb,double yb,double xe,double ye,double *f);
	//double ConvertOrientToAngle(double ValueOrient);
	void CalXLjd(double array1[4*MAXJDNUM][10],int NunJD1,double array2[4*MAXJDNUM][10],int NunJD2,AcGePoint2dArray* JDpt);
	void movepoint(double& x1,double& y1, double& x2, double& y2,double dis);
	void getminandmax(double x1,double x2,double x3,double x4,double& min,double& max);
	int CalJD_Spiral_Spiral_diedai(double &l2_spiral1,double &l2_spiral2,double Spiral1_A,double Spiral1_Spt[3],double Spiral1_Len,double Spiral1_ZorY,
		double Spiral2_A,double Spiral2_Spt[3],double Spiral2_Len,double Spiral2_ZorY);
	//void GetOrignCorOfSprial(double SprX,double SprY,double SprRad,
	//	double SprOrient,double ValueA,int Deflect,int RadForward,
	//	double &OrignX,double &OrignY,double &OrignOrient);
	bool CheckIfOnArc(double array[10],double pt[3]);
	int GetXYonSomeXY(double array2[10],double jslen,double respt[3]);
	int CalJD_ZX_ZX(double Line1_array[10],double Line2_array[10],double ResPt[2]);
	int CalJD_ZX_ARC(double Line_array[10],double ARC_array[10],double ResPt[2],double ResPt1[2]);
	int CalJD_ZX_Spiral(double Line_array[10],double Spiral_array[10],double ResPt[3],double ResPt1[3]);
	int CalJD_ARC_Spiral(double Arc_array[10],double Spiral_array[10],double ResPt[3],double ResPt1[3]);
	int CalJD_Arc_Arc(double Arc1_array[10],double Arc2_array[10],double ResPt[3],double ResPt1[3]);
	int CalJD_Spiral_Spiral(double Spiral1_array[10],double Spiral2_array[10],double ResPt[3],double ResPt1[3]);
	int CalJD_XY1_XY2(double XY1[10],double XY2[10],double ResPt[3],double ResPt1[3]);
	//求三个交点的左右转，左-1，右+1
	int GetLeftOrRight3JD(double N1, double E1, double N0, double E0, 
		double N2, double E2);
	
	void GetQD(double CenN, double CenE, double R,
		double OutN, double OutE, int LRFlag,
		double &QDN, double &QDE);	
	void GetCenter3JD(double N1,double E1,double N0,double E0,double N2,double E2,double R,double Lo1,double Lo2,double &CenN,double &CenE);
	double FwjtoAngX(double ang);
	void StdJiao(double &jiao);	
	//plh04.14
	virtual void GetT0min_Lrmin(double DV,double &T0min,double &Lrmin,struct GuiFanCanShu mGuiFanCS);
	void getLineEndPt(AcGePoint3d sPt,double len,double fwj,AcGePoint3d &ePt);
	//求点到线的距离 
	double DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
		double &outY, double &distTo2PointOnLine);	
////plh
	bool GetTangent(double CenX,double CenY,double R,double  &OutX,double  &OutY,double  &TX,double &TY);
	double PXJ(double fwj1, double fwj2);
	double fwj(double x0,double y0,double x1,double y1);
	double AFA(double fwj1,double fwj2);
	double radiusfree(AcDbLine *Line1, AcDbLine *Line2,AcGePoint3d pk);
	
	void GetPerPoint(double outX,double outY,double x1,double y1,double x2,double y2,double &perX,double &perY);
	
	//plh04.14
	bool GetCircleFromT_P_R(double x1,double y1,double x2,double y2,double R,double &cenX,double &cenY);

	//plh04.14//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
	double GetRFromT1T2(double &x1, double &y1, double &x0,double &y0,double &x2,double &y2,GuiFanCanShu& mGuiFanCS, bool IsStandard=false, double DV=350,int INTRadius=10,int JUDGET012=0);
	//plh04.14
	double GetRFromT_P_P(double x1,double y1,double x2,double y2,double &xs,double &ys,double &x0,double &y0,double &xe,double &ye,double &cenX,double &cenY,AcDbObjectIdArray &eraseId, GuiFanCanShu& mGuiFanCS, bool IsStandard=true, double DV=350);
	//传入切线方向，返回半径,前一个切点，交点，后一个切点
	struct EXDLB{//二线断链表
		int no;//表示第几个绕行段
		CString RXDName;//对就绕行段名称
		double BLC,ELC,TYLC;  //  断链的断前里程、断后里程、断链处统一里程
		CString   BGH,EGH; //  断链的断前里程冠号和断后里程冠号	
	};
	
	struct DoubleLineData//双线平面数据
	{
		Adesk::Int16 JDXZ;//交点性质 0-并行段交点,1-绕行段交点
		Adesk::Int16 jdno;//二线交点号
		double x,y,Lo1,R,Lo2;
		Adesk::Int16 jdno1;//对应一线交点序号
		double xjj;//线间距//二线在左为－在右为＋
		double qxjk;//曲线加宽
		double NYDL;//内业断链
		ACHAR ZHLC[80];//直缓里程
		ACHAR JDLC[80];//交点投影到一线上的里程//plh05.08
		bool ISXJJFirst;//是否根据线间距计算
		bool ISStartEnd;//是否为换边的起终点
		bool ISShow;//是否显示给用户
	};
////////////////////////////////////////以上plh


	struct xlpoint { double x,y,a,r,lc; };
  struct DLBZ  //冠号改变点、断链点、起、终点的冠号、里程表
	{ 
  	double BLC,DL;  //  断链的断前里程和断链值(+值为长链;-为断链)
	  ACHAR   BGH[8],EGH[8]; //  断链的断前里程冠号和断后里程冠号
	};

	struct STATAB//车站表
	{
		CString StaName;//车站名 
		CString BGH,EGH,CGH;//冠号
		double CenDml;//中心里程
		double StDml;//起点里程
		double EdDml;//终点里程
		double CZLX;//车站类型
		//double CQTKM;//插旗图块名
		ACHAR TKSta[256];
		ACHAR SKml[80],EKml[80],CKml[80];
	};
	
	struct TUNTAB//隧道表
	{
    CString TunName;//桥名 
		CString BGH;//冠号
		CString EGH;//冠号
		CString flag;//插旗方式
		double StDml;//起点里程
		double EdDml;//终点里程
		double TunCost;
		ACHAR SKml[80],EKml[80];
		CString Note;//附注
		int xs;//线数
	};

	struct SMALLBRIDGETAB//小桥涵表
	{
		int iRXD;//绕行段编号。＜0表示并行或单线
		CString GH;//冠号
		double ml;//中心里程
		CString KKStyle;//孔跨式样
		double dwL,dwR;//横向宽度
		double JJiao;//交角
		double ToUse;//用途
		double InsertStyle;//插旗方式
		CString Notes;//附注
	};
	
	struct PDTAB//变坡点表
	{
		CString GH;//变坡点冠号
		double  ml;//变坡点里程
		double  Level;//设计高程
		double Rshu,RLen;//竖曲线半径,长度
		double degree,length;//坡度，坡长
		CString Notes;//附注
		ACHAR CKml[80];
		double cml;//变坡点连续里程
	};

	typedef	CArray<PDTAB,PDTAB> BPDArray;

	struct DMXTAB//地面线表
	{
        CString GH;//中桩冠号
		double  ml;//变坡点里程
		double  Level;//设计高程
		ACHAR CKml[80];
	};

	struct QXTAB//曲线表
	{
		int jdno;//交点号
		double x,y,zj,R,l1,l2,T1,T2,L,jzxl;
		CString BGH,EGH;//ZH,HZ桩冠号
		double Bml,Eml;//起终里程
		double InDL;//		内业断链
	};

   
	struct TWTAB//填挖表
	{
        CString GH;//加桩冠号
		double  ml;//加桩里程
		double  DMGC,SJGC;//地面高程,设计高程
		double SJPD;//设计坡度
		double SQXZJ;//竖曲线纵距
		double ZXTJ;//中心抬降值
		double ZXTWG;//中心填挖高
		double LJSGG;//路肩施工高
		double QXJK;//曲线加宽
		double LJSGK;//路肩施工宽度
		ACHAR CKml[80];
	};
	
	struct HdmAreaData//横断面成果面积数据结构
	{
		CString GH;
		double dml;
		double BTinf;//断面性质
		double at;//填面积
		double aglt;//改良土面积
		double asst;//渗水土面积
		double aqbt;// 清表土面积
	    double aw[6];//挖面积
		double LZDK;//左占地宽
		double RZDK;//右占地宽
		double ahj;// 护肩面积 加固防护污工
		double asgt;//水沟填面积
		double asgw;//水沟挖面积
		double ajq;//浆砌面积     附属污工
		double cpbpxc;//草皮边坡斜长
		double dqbpxc;//挡墙边坡斜长
		
	};

	struct ZYJSBZTAB//主要技术标准表
	{
        CString TLDJ;//铁路等级
		CString ZXSM;//正线数目
		CString GDLX;//轨道类型
        CString QYZL;//牵引种类
        double SDMB;//速度目标
		double XJJ;//标准线间距
		double RtjMin,RtjMax;//推荐半径小（大）
		double XZPD,XZPD1;//限制坡度,下行限制坡度
		double DFXYXC;//到发线有效长
		double Rmin1,Rmin2;//最小曲线半径
	};

/*
	struct ListZhFa{//有序组合方案链表
		int pro;     //方案pro入选里程范围
		char SGH[10];//起冠号
		double Sdml;//起现场里程
		char EGH[10];//终冠号
		double Edml;//终现场里程
        ListZhFa *prior;
        ListZhFa *next;
	};*/

	
	struct BRITAB//大中桥表
	{
        CString BriName;//桥名 
		CString BGH,EGH,CGH;//冠号
		double CenDml;//中心里程
		double StDml;//起点里程
		double EdDml;//终点里程
		CString KKSY;//孔跨式样
		CString SWHt;//一百年水位高程
		double BriLen;
		double BriCost;
		ACHAR SKml[80],EKml[80],CKml[80];
		int xs;//线数 1-单线 2-双线
		CString Note;//附注
	};

	struct GCKZPt 
	{
		ACHAR GH[8] ;
		double XCLc;//现场里程
		double Cml;//统一里程
		double DesHSX;//高程上限
		double DesHXX;//高程下限
		int Style;//控制点性质（0-必经点，1-只有上限点，2-只有下限点，3-即有上限点又有下限点）
// 		int FrontStyle;//前一个高程控制点的性质，如果在该控制点前有必经点为1，无必经点为0；
	};

	BAS_DRAW_FUN_RAIL();
	virtual ~BAS_DRAW_FUN_RAIL();
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId CreateLayer(ACHAR *LayerName);
	static AcDbObjectId   AddEntityToDbs(AcDbEntity *pEntity);
    static AcDbObjectId  makecircle(AcGePoint3d& startPt,double R,int icolor);
    static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt);
    static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor);
	static AcDbObjectId  makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL,double LTScale=1.0);

    static AcDbObjectId   maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor=2,int mode=1,ACHAR *layername=NULL,ACHAR* textStyle=L"宋体");
    static AcDbObjectId   makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR *layer=NULL);
    static AcDbObjectId   drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND);
    static AcDbObjectId   GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz);
    int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT);
    int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT,AcGePoint3d& pT1,AcGePoint3d& pT2);
    int  getPoint(ACHAR* pMessage, AcGePoint3d& pT);
    void  makeGroup(AcDbObjectId groupId);
    AcDbEntity*  selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
    static double  Get_INDATA_L(double INDATA[5],int *jj);
    static int xlzbxy(struct xlpoint *P0,struct xlpoint *PZ,double l,double INDATA[5],int display);
    static int  xlpoint_pz(double array2[][10],int trow2,struct xlpoint *PZ);
	int  xlpoint_pz(double array2[][10],int trow2,BAS_DRAW_FUN:: xlpoint *PZ1); 
    int getmodel(double x1,double y1,double x2,double y2,double x3,double y3,double L1,double L2,double R);
    struct resbuf *entdxf( struct resbuf *entdata,int gcode);
    static double xyddaa(double xb,double yb,double xe,double ye,double *f);
    void removeAllButLines(AcDbObjectId groupId);
    void groups();
    static double spiral_x(double A,double l);
    static double spiral_y(double A,double l);
    static double rad(double d_ms);
    static double dms_rad(double rad);
    int getBPMTL(double a,double R,double L1,double L2,double *BPMTL);
    static double deg(double d_ms);
    double spiral_p(double A,double L);
    double spiral_xm(double A,double L);
    static double dms(double ddd);
	double dms(double ddd, int Nw, ACHAR* DMS_Str);
    void printList(struct resbuf* pBuf);
	void iterate(AcDbObjectId plineId);
    AcDbObjectId createPl_xlpm(int trow,double arrayd[MAXJDNUM][6]);
    static ACHAR*  LCchr(ACHAR  *GH,double lc,int NLC,int mode);
    void calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt,double *tanl, double *R, double *xc,double *yc,double *ang,double *startfw,double *endfw);
    void caljd(double x1,double y1,double x2,double y2,double x_1,double y_1,double x_2,double y_2,double *xjd,double *yjd);
    void calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd);
	double Get_Rshu(double design_V,double deltaI, GuiFanCanShu mGuiFanCS);
 //   double get_l0(double R);
	double get_l0(double SR,double Design_V,GuiFanCanShu mGuiFanCS);
	double get_R(double SR,double Design_V, GuiFanCanShu mGuiFanCS);
    static long factorialfun(int n);		
    double TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] );
	static double XLC1byt2y( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30]);
    static double XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30]);
	///
	double GetSide(double Xo,double Yo,double x1,double y1,double x,double y);
	double ZXJ(double s2,double s1);
	double CalJiaJiao(double Fwj1, double Fwj2);
	double PROJ_ML(double array2[][10],int trow2,double xout , double yout , double xon , double yon);
	double XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int trow2);
	void split_ckml(ACHAR str[], double *ml, ACHAR  *ck);
	int  F_num(ACHAR *str, ACHAR c);
	void putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place);
	void atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4]);
	void NoteBG(AcGePoint3d Opt,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,ACHAR *layername=NULL);
	void DrawBGTri(AcGePoint3d Opt,double sfactor,int iColor,ACHAR *layername=NULL);
	void dlayer(ACHAR lname[]);//删除层
	double get_l0(double Design_V,int KNDD,double SR,GuiFanCanShu mGuiFanCS);
	int crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point pp);
	void dmltockml(CString GH,double dml,ACHAR ckml[80]);
	void NullInStrto$(CString& text);
	void InStr$toNull();
	int ReadWorkdir();
	double get_xjj(double Design_V, GuiFanCanShu mGuiFanCS);
	double get_XJJK(double Design_V,double SR, GuiFanCanShu mGuiFanCS);//线间距加宽
	double Get_QXJK(CString TLDJ,double design_V,double SR, GuiFanCanShu mGuiFanCS);//规范曲线路基加宽
	static int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	int CalXYjd(double Spt[3],double Ept[3],double array2[4*MAXJDNUM][10],int trow2,int kk, double ResPt[2],double &ProjMl);
	int IfOnXY(double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);//是否与线元有交点
	double Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2]);
	int Search_XY(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3]);//定位pt1点所在线元
	double Circle_XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int kk);
	//输入一线里程，一线线元参数，二线线元数据，求线间距,二线投影连续里程及投影点
	int CalXjj(double ml1,double array1[4*MAXJDNUM][10],int tarray1,double array2[4*MAXJDNUM][10],int tarray2,double &xjj,double &projml,double resPt[2]);
	//输入一线上一点PZ,二线线元数据
	int Judge2XLorR(xlpoint PZ,double array2[4*MAXJDNUM][10],int tarray2);
	int ReadGuifanFile(CString ProName, GuiFanCanShu& mGuiFanCS);

	int FindWichDLFromFile(ACHAR GH[10],double XLC);
	/////////////////////////////////////////////////////////引出接入 道岔
	double BAS_DRAW_FUN_RAIL::SwitchNumToAngle(int Num);
	//求接出点位置
	BOOL GetSingleSwitchCoordinate(double SingleXY[10],AcGePoint3d &Center,AcGePoint3d &End1,AcGePoint3d &End2,AcGePoint3d &End3);
	int JudgePTLorR(double N,double E,double &dist,double array[4*MAXJDNUM][10],int tarray);
	double CCW2(double pt1[2],double pt2[2],double pt3[2]);
	int FitSwitch(double angle);//根据角度（弧度）转道岔号数
	void InitSwitchNum(CComboBox &Swith);
	static CString GetProByMdbName(CString mdbPath);//由数据库完整路径获取方案名
	bool LoadArx(ACHAR ArxFname[256],ACHAR cmd[80]);

} ;



#endif // !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
