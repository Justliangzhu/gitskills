// BAS_DRAW_FUN.h: interface for the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
// BAS_DRAW_FUN.h: interface for the BAS_DRAW_FUN class.
//
/////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <dbents.h>
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

#include <dbjig.h>
#include <acgi.h>
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "afxtempl.h"
#include "Struct.h"

#include <string>
#include <fstream>
using namespace std;


#if !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
#define AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_
#define pi 3.14159265358979323846264338327 
#define PI 3.14159265358979323846264338327
#define Pai 3.14159265358979323846264338327
//////////////////////////////////////////////////////////////////////////
#define SPIRALPRECISION 40
#define PRECISION 0.0001
#define MAXJDNUM 200
#define MAXDLNUM 200
#define MAXBPDNUM 300
#define Spoint(pt, x, y, z)  pt[X] = (x);  pt[Y] = (y);  pt[Z] = (z)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseSettting.h"
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
///////////////////////线路单元起终点处的走向标识/////////////////////////
//2005.5.12 hhp
//数据精度定义;
#define DataPrecision 0.00000001
//2005.5.12 hhp
/////////////////////////////////////////////////////////////////////////
//2005.5.12 hhp
///////////////////////缓和曲线半径趋势变化标识///////////////////////////
#define LargeToSmall -2                         //无穷大到某一半径;
#define BigToSmall   -1                         //某一大半径到某小一半径;
#define NoChange      0						    //半径不发生改变如直线与圆;
#define SmallToBig    1						    //某一小半径到某一大半径;
#define SmallToLarge  2						    //某一半径到无穷大;
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

//20190816	增加颜色定义
#define BYLAYER		0	//随图层
#define RED			1	//红色
#define YELLOW		2	//黄色
#define	GREEN		3	//绿色
#define CYANBLUE	4	//青色
#define BLUE		5	//蓝色
#define MAGENTA		6	//洋红
#define WHITE		7	//白色/黑色

#include "SpiralY.h"
class SPIRAL;
//线元类型
struct XianYuan
{
	enum
	{
		LINE = 1,//直线
		ARC = 2,//圆弧
		PreSPIRAL = 3,//完整的前缓：0->R 
		AftSPIRAL = 4,//完整的后缓：R->0
		BigToSmallSPIRAL = 5,//不完整缓和曲线，大圆-->小圆
		SmallToBigSPIRAL = 6//不完整缓和曲线，小圆-->大圆
	};
};


struct STuKuang
{
	double StartTYLC,EndTYLC;//图框起终点的统一里程
	CString StartXCLC,EndXCLC;//图框起终点的现场里程
	double StX,StY;//图框线路起坐标
	double EdX,EdY;//图框线路终坐标
	double CenX,CenY;//图框中心
	double LtUpX,LtUpY;//左上角点坐标
	double LtBmX,LtBmY;//左下角点坐标
	double RtUpX,RtUpY;//右上角点坐标
	double RtBmX,RtBmY;//右下角点坐标

	//旋转平移矩阵
	AcGeMatrix3d xform;
	double newStX,newStY;//图框线路起坐标
	double newEdX,newEdY;//图框线路终坐标
	double newCenX,newCenY;//图框中心
	double newLtUpX,newLtUpY;//左上角点坐标
	double newLtBmX,newLtBmY;//左下角点坐标
	double newRtUpX,newRtUpY;//右上角点坐标
	double newRtBmX,newRtBmY;//右下角点坐标
	ads_point inspt;

	STuKuang()
	{
		StartXCLC = L"";
		EndXCLC = L"";
		StartTYLC = 0.0; EndTYLC = 0.0;
		StX = 0.0; StY = 0.0;
		EdX = 0.0; EdY = 0.0;
		CenX = 0.0; CenY = 0.0;
		LtUpX = 0.0; LtUpY = 0.0;
		LtBmX = 0.0; LtBmY = 0.0;
		RtUpX = 0.0; RtUpY = 0.0;
		RtBmX = 0.0; RtBmY = 0.0;

		newStX = 0.0; newStY = 0.0;
		newEdX = 0.0; newEdY = 0.0;
		newCenX = 0.0; newCenY = 0.0;
		newLtUpX = 0.0; newLtUpY = 0.0;
		newLtBmX = 0.0; newLtBmY = 0.0;
		newRtUpX = 0.0; newRtUpY = 0.0;
		newRtBmX = 0.0;  newRtBmY = 0.0;
	}
};

struct ptt_node{
	ads_point pt0;
	ads_point pt;
	ads_point pt1;
	struct ptt_node *next;
};
struct arc_lst{
	ads_real R;
	ads_real xo;
	ads_real yo;
};
struct point_lk{
	ads_point pt;
				struct point_lk *next;
};
class BAS_DRAW_FUN 
{
public:
	struct DLBZ  //冠号改变点、断链点、起、终点的冠号、里程表
	{ 
		double BLC,ELC;//DL;  //  断链的断前里程和断后里程、、断链值(+值为长链;-为断链)
		double DL;
		CString BGH,EGH; //  断链的断前里程冠号和断后里程冠号
		double TYLC;
		Adesk::Int16 FLDNo;//分离段编号
		CString FLDName;//分离段名称
	};

	struct Point
	{
		double x,y,z;
	};

	

	
	struct GLTZSTAdata//关联特征现场里程
	{
		double FLDSmlOnZX,FLDEmlOnZX;//分离段起始里程,终止里程
		CString FLDname;
		double NoseSmlOnZX,NoseSmlOnFLD;//关联断面统一里程
		double NoseEmlOnZX,NoseEmlOnFLD;//关联断面统一里程	
		double StartR,EndR;
		///////////////////////////////////////////////////////////////////////////////////
	};
	
 	struct HDMDMXdata//里程标表
	{
		int  lcflag;//里程类型，-1（公里标）,1-9百米标，0-断链标
		ACHAR flag[10];
    	int BTxz;//桥隧性质 0-非桥非隧 -1-左桥头,-2-右桥头,-3-全桥头 -4-左隧头,-5-右隧头,-6-全隧头
                                 // 1-左桥尾, 2-右桥尾, 3-全桥尾  4-左隧尾,5-右隧尾,-6-全隧尾
		double lc;//统一里程
///////////////////////////////////////////////////////////////////////////////////
		double EarH;//中桩地面高
        int ZEarPtNum,YEarPtNum;
        CArray<Point,Point>ZEarPtAry;
		CArray<Point,Point>YEarPtAry;
	};
///////////////////////////////////////////////////////////////////////////////////////////////




   
 
  



	//////////////////////////////////////////////////////////////////////////





	//////////////////////////////////////////////////////////////////////////

    struct DMXdata
	{
		CString GhK;
		double rcd;//里程
		double m_dmgc;//地面高程
		long NumL; //左地面点数
		double Learpt[200];//左地面点
		long NumR; //右地面点数
		double Rearpt[200];//右地面点
	};


	struct BDData//鼻端相关数据
	{
		CString XL1;
		CString XL2;
		int InOrOut;//0:主线里程增加方向或1:主线里程减小方向
		int ZDStOrEd;//0:匝道起点处鼻端,1:匝道终点处鼻端
		int Lmhp;//1 同坡 -1 反坡
		double C1;//1线偏置值
		double C2;//2线偏置值
		double Z1;//1线砌形端后的过渡段长度
		double Z2;//2线砌形端后的过渡段长度
        double YljRNose;//硬路肩鼻端半径
		double TljRNose;//土路肩鼻端半径
		double Yljdml1;//硬路肩鼻端在1线的现场里程
		double Yljdml2;//硬路肩鼻端在2线的现场里程
		double Tljdml1;//土路肩鼻端在1线的现场里程
		double Tljdml2;//土路肩鼻端在2线的现场里程
		double FldDml1;//分流点主线里程
		double FldDml2;//分流点匝道里程
		double YCSpt[2];
		double YCEpt[2];
		double TCSpt[2];
		double TCEpt[2];
		double YNosePt[2];
		double TNosePt[2];
		double YLjPt1[2];//1线路肩处坐标
		double YLjPt2[2];//2线路肩处坐标
	};

//////////////////////////////////////////////////////////////////////////////////
	struct CRSDAT
	{
		bool IfLink;//是否关联断面
		double glcml;//关联里程
		CString glRoadName;
        
		ACHAR ckml[80];
		double cml;
		double dml;
		int DorS;//1-单路面，2-双路面
		int DuanLuoInf;//横断面所在段落信息0-单线，1-双线并行，2-双线绕行路肩分离双路面 3-双线绕行边坡分离单线路面
		double xjj;//线间距，单线为0,二线在左为-，右为+
		double ZDesH,YDesH;
		double LZDK,RZDK;
		double ZZJ,YZJ;//左右支距
		double ZPJPT[2],YPJPT[2];
		//DesH路肩的设计高程和EarH地面高程（HDM的绘图数据）//指左线
		double BriZZDK,BriYZDK,TunZZDK,TunYZDK;
		double RHFL[10],RHFR[10];  //主线路面点5个
		double zlmhd[3],ylmhd[3];
		int ZDMXZ,YDMXZ;//左右幅断面性质
		double  Zt,Yt;
		double ZHc,YHc,Y_ZHc,Y_YHc;//填挖高
		int zptsum,rptsum,Lnpt,Rnpt;
		AcGePoint3dArray RPTL,RPTR;
		int Ljdnum,Rjdnum;
		//指右线
		double FLDcml;//相关道路投影里程
		CString FLDname;//相关道路名
		double Y_BriZZDK,Y_BriYZDK,Y_TunZZDK,Y_TunYZDK;
		double Y_RHFL[10],Y_RHFR[10];  //分离段路面点5个
        double Y_zlmhd[3],Y_ylmhd[3];
		int Y_ZDMXZ,Y_YDMXZ;//左右幅断面性质
		double  Y_Zt,Y_Yt;
		int Y_Lnpt,Y_Rnpt;
		AcGePoint3dArray Y_RPTL,Y_RPTR;
		int Y_Ljdnum,Y_Rjdnum;
		
		double Ldeltah,Rdeltah;//左右路肩点与地面线的高层差
		double Y_Ldeltah,Y_Rdeltah;//左右路肩点与地面线的高层差
		double newLYd,newRYd;//左路基中心的地面高层，右路基中心的地面高层（现改为线路中线的地面高程）
		double  at,aw;
        double offsetX,YoffsetX;//路基中心与一线偏移量，左负右正
		double LmH,YljH;//路面厚,硬路肩厚
		double Xo,Yo;//绘图参照点
		double hi,wi;//
		double ZXo,ZYo,ZX1,ZY1,tX0,tY0;//追踪原点,tX0 tY0桥隧道注释点
		double Xmin, Xmax, Ymin, Ymax;//绘图参照点,
		double Ldh,Rdh;
		double qxjk,Yqxjk;//曲线加宽,左-右+,双路面时还有右线曲线加宽
		//		double LJFLK;
        int mflag;//是否修改
		double DX;
		//	
		//		
		//	//	double LPjqPt[]
		//
		//		//左线或一线
		int LGouPtNum,RGouPtNum,LJSGouPtNum,RJSGouPtNum;////////wwq5.14
		int LGouProperty, RGouProperty;//0-坡脚墙，1-水沟；1-排水沟,2-路堤路肩沟,-1-初始化无水沟，2-侧沟
		int LIfSetGouH,RIfSetGouH;
		double LGouH,RGouH;
		double Lgou[36],Rgou[36],LJSgou[36],RJSgou[36];//水沟7点/////wwq5.14	 
		//	
		//		//右线
		int Y_LGouPtNum,Y_RGouPtNum,Y_LJSGouPtNum,Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty,Y_RGouProperty;
		double Y_Lgou[36],Y_Rgou[36],Y_LJSgou[36],Y_RJSgou[36];//水沟7点/////wwq5.14
		
		double FillArea ;//填面积
		double CutArea;  //挖面积
		double FillBetterSoilArea; //填改良土面积
		double QBTArea ;//清表土面积
		double WYNArea ;//挖淤泥面积
		double GouTarea;//水沟填面积
		double GouWarea;//水沟挖面积
		double WFFLarea[6];//沟按挖方分类确定的挖面积////wwq
		double HJarea ;//护肩面积
		double SGJQarea;//浆砌面积
		double DQlength ;//挡墙坡长
		double Grasslength ;//铺草坡长
		//		int DMinf;
		bool m_IfXJGou;		
		///////////////////////////////用于交互设计
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

		double ZCG_GDR,YCG_GDR;//侧沟底R;
		
		AcDbObjectId LgouId,RgouId;
		bool IfHasDes;
        int IfHasZlink,IfHasYlink;
		CRSDAT *zlink,*ylink;
		double ZDorS,YDorS;
		double ZHTHD,YHTHD;//清表土厚度
		int ZHTXZ,YHTXZ;
		bool IfFldSEHdm;//是否分离段起终断面
		int IsInsertHdm;//是否为后期补充断面
		int IfSetDQ;//0无挡墙，-1-左侧有挡墙, 1-右侧有挡墙，2-两侧都有
		//		int xlxz;//线路性质
		//		int r_bh;//绕行段编号
		double OffsetByXLZX;//路基中线相对于线路中线的偏移量

		CRSDAT()
		{
			offsetX = 0.0;
		}
	};
    



	struct ZQZJTAB//直曲转角表
	{
		CString JDNO;
		CString JDZH;//交点桩号
		double JDN,JDE;//交点坐标
		double JDJJ;//与后一个交点的间距
		double FWJ;//当前交点与后一个交点的连线的方位角
		double JIAZX;//当前交点与后一个交点的夹直线长
		double AFA;//当前交点的转角
		double T1,T2;//前切，后切
		double R1,R2,R3;//前一个交点，当前交点，后一个交点的半径
		double A1,A2;//前缓，后缓
		double Ls1,Lc,Ls2,L;//前缓和曲线长、圆曲线长、后缓和曲线长、曲线总长
		double OutDist;//外距

		CString ZHZH;//第一缓和曲线起点桩号
		double ZHN,ZHE;//第一缓和曲线起点坐标
		
		CString HYZH;//第一缓和曲线终点及圆曲线起点桩号
		double HYN,HYE;//第一缓和曲线终点及圆曲线起点坐标
		
		CString YoZH;//圆曲线中点桩号
		double YoN,YoE;//圆曲线中点坐标

		CString YHZH;//第二缓和曲线起点及圆曲线终点桩号
		double YHN,YHE;//第二缓和曲线起点及圆曲线终点坐标
		
		CString HZZH;//第二缓和曲线终点桩号
		double HZN,HZE;//第二缓和曲线终点坐标

		CString Note;
	};			 
	struct XLManager
	{	
		int No;//线路编号
		CString Name;//线路名称
		int PreNo,NextNo;//前、后线路编号
	};

	struct JdCenter
	{
		//交点属性
		Adesk::Int16 JDXZ;
		double N,E,R,A1,A2,Ls1,Ls2;
		double JDN,JDE;//交点坐标
		double CenN,CenE;//圆心坐标
		//前后边的方位角，当前交点为起点
		int CirXYNo;
		double PreFwj,AftFwj;
		double T1,T2,L;//前切，后前，曲线长
		double JdNo;//交点号
		//左右转
		Adesk::Int16 LeftOrRight;
		//是否回头
//		bool IsOrNotHuiTou;
		Adesk::Int16 IsOrNotHuiTou;//0 不回头 1 顺向左转回头 2 顺向右转回头 3 逆向左转回头 4 逆向右转回头
		//后接关系 无关 S型 C型 卵型
		Adesk::Int16 LinkAfter;

		Adesk::Int16 FLDNo;//分离段编号

		double afa;//交点的转角
		double DistToAfterJD;//与后一个交点的距离
		//20190712	增加默认构造函数
		JdCenter()
		{
			JDXZ = 0;
			N = E = R = A1 = A2 = Ls1 = Ls2 = 0.0;
			JDN = JDE = CenE = CenN = 0.0;
			CirXYNo = 0;
			PreFwj = AftFwj = 0.0;
			T1 = T2 = L = 0.0;
			JdNo = 0.0;
			LeftOrRight = 0;
			IsOrNotHuiTou = 0;
			LinkAfter = FLDNo = 0;
			afa = 0.0;
			DistToAfterJD = 0.0;
		}
	};

	struct DUANLUO//段落结构体
	{
//		int StartJDNo,EndJDNo;//起终交点号
//		int StartDLNo,EndDLNo;//起终断链号
		double StartTYLC1x,EndTYLC1x;//起终点投影到一线上的里程
//		GTZX2 *iGtzx2;//该段落对应的GTZX2对象
	};
	

	
	struct ModifyRoadTab
	{
		CString RoadName;//道路名
		CString RailName;//相交铁路名
		CString SCkml;//起点里程
		CString RailCkml;//交叉处铁路里程
		double Raillevel;//交叉处铁路高程
		CString ECkml;//终止里程
		double RoadDml;//交叉处公路里程
		double RoadLen;//道路长度
		double InterAng;//交叉角
		double DetaH;//高差
		CString UpDown;//铁路在当前道路上下方
		CString KKSY;//孔跨式样
		CString RandType;//道路类型等级
		double LjWide;//路基面宽
		double LmWide;//路面宽
		double LmThick;//路面厚度
		CString LmType;//路面材料类型
		double LmArea;//路面工程面积
		double FillVol;//填方
		double CutVol;//挖方
		double MasonryVol;//圬工数量
		double LandArea;//占地面积
		CString LandAreaStr;//占地面积字符串
		CString Note;//附注

		ModifyRoadTab()
		{
			RoadName = L"";
			RailName = L"";
			SCkml = L"";
			RailCkml = L"";
			Raillevel = 0.0;
			ECkml = L"";
			RoadDml = 0.0;
			RoadLen = 0.0;
			InterAng = 0.0;
			DetaH = 0.0;
			UpDown = L"";
			KKSY = L"";
			RandType = L"";
			LjWide = 0.0;
			LmWide = 0.0;
			LmThick = 0.0;
			LmType = L"";
			LmArea = 0.0;
			FillVol = 0.0;
			CutVol = 0.0;
			MasonryVol = 0.0;
			LandArea = 0.0;
			LandAreaStr = L"";
			Note = L"";
		}

	};

	struct EXDLB{//二线断链表
		int iDuanLuo;//段落号//plh04.29
		int no;//表示第几个绕行段
		CString RXDName;//对就绕行段名称
		double BLC,ELC,TYLC;  //  断链的断前里程、断后里程、断链处统一里程
		CString   BGH,EGH; //  断链的断前里程冠号和断后里程冠号	
	};
	
	struct DoubleLineData//双线平面数据
	{
		int iDuanLuo;//段落号//plh04.29
		int JDXZ;//交点性质 0-并行段交点,1-绕行段交点
		int jdno;//二线交点号
		double x,y,Lo1,R,Lo2;
		int jdno1;//对应一线交点序号
		double xjj;//线间距//二线在左为－在右为＋
		double qxjk;//曲线加宽
		double NYDL;//内业断链
		ACHAR ZHLC[80];//直缓里程
		ACHAR JDLC[80];//交点投影到一线上的里程//plh05.08
		bool ISXJJFirst;//是否根据线间距计算
		bool ISStartEnd;//是否为起终点
		bool ISShow;//是否显示给用户
	};
////////////////////////////////////////以上plh


	struct xlpoint { double x,y,a,r,lc; };
	AcGePoint3d resetjd_centerid(AcGePoint3d  Point, AcDbObject* pEntity, AcDbObjectId& lineId);


   
	struct PDTAB//变坡点表
	{
		CString GH;//变坡点冠号
		double  ml;//变坡点里程
		double cml;
		double  Level;//设计高程
		double Rshu, RLen;//竖曲线半径,长度
		double degree, length;//坡度，坡长
		CString Notes;//附注
		ACHAR CKml[80];
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

   
	struct TWTAB//纵地面线表
	{
        CString GH;//加桩冠号
		double  ml;//加桩里程
		double cml;
		double  DMGC;//地面高程,设计高程	
		ACHAR CKml[80];
	};


	struct ZYJSBZTAB//主要技术标准表
	{
        CString TLDJ;//铁路等级
		CString ZXSM;//正线数目
		CString GDLX;//轨道类型
        double SDMB;//速度目标
		double XJJ;//标准线间距
		double RtjMin,RtjMax;//推荐半径小（大）
		double XZPD;//限制坡度
		double DFXYXC;//到发线有效长
		double Rmin1,Rmin2;//最小曲线半径
	};

	struct Culvet//对应涵洞表
	{
		CString QHType;//桥涵类型
		double CenDml;//中心里程
		double cml;//对应的连续里程
		CString KKMS;//孔跨描述 
		CString QHMS;//桥涵描述
		double ht;
        double ang;//交角
	} ;

	struct  RoadProperty
	{
		CString Grade;//等级
		CString TerrainType;//类别
		CString DXLB;//地形类别
		int DV;//设计车速
	};

	struct  ZdmDesStandard
	{
		CString MAXPD;//最大纵坡
		CString TRshu1;//凸竖曲线半径一般最小值
		CString TRshu2;//凸竖曲线半径极限最小值
		CString ARshu1;//凹竖曲线半径一般最小值
		CString ARshu2;//凹竖曲线半径极限最小值
		CString Lshu1;//竖曲线长度一般最小值
		CString Lshu2;//竖曲线长度极限最小值
		CString PCMin1;//坡长一般最小值
		CString PCMin2;//坡长极限最小值
	};
	


	struct CGJSCS
	{
		CString	m_HxCgJbl;//缓和超高渐变率
	    CString	m_ZxCgJbl;//直线超高渐变率
        int CgMode;//0 1 2 3 4 5;
		int JkMode;//0 线性 1抛物线
	    int CgLenMode;//0-不取整;1-1米整倍数";"2-5米整倍数
		CGJSCS()
		{
			m_HxCgJbl.Empty();
			m_ZxCgJbl.Empty();
			CgMode = 0;
			JkMode = 0;
			CgLenMode = 0;
		}
  	};
	
	struct NoteStr
	{
		int rownum,colnum,ntext;
		int fitinf;//=0 not fit mode ; =1 fit mode;
		double texth,dv;//dv fit时 算位置时的除数
		CString text;//表格内只有一行文本 ntext=1
		CString text1;//否则两行文本 ntext=2
		CString text2;//三行文本 ntext=3
	};
	
    struct Table //表格结构
	{
		int nrow,ncol;
		double RowH[14],ColWid[8];
		AcGePoint2d LDpt; 
		int Nstr;
        NoteStr Str[30];
	};
	
	struct BMSTRUCT
	{
		//水准点编号、高程、里程冠号，里程数，到线路中线距离，坐标
		CString BianHao;
		double GaoCheng,N,E;
	};
	
	struct DXSTRUCT
	{
		//桩号，高程，坐标，方位角
		CString BianHao;
		double GaoCheng,N,E;
	};
	
    struct CTRLHT//控制标高
	{
		double dml;
		double Ht;
		CString Att;//属性 上限制标高、下限制标高 必经标高			
	};
	struct XYarray
	{
		double xyarr[10];
		AcDbObjectId eID;
	};
//	CArray<XYarray,XYarray>pXYArr;
	struct BXLMKD
	{
		double *ptarr;
		int bxinf,ptnum;//边线类型
		double bxsml,bxeml;
		int LorR;//边线边侧
		AcDbObjectId eID;
	};
	struct GLROAD
	{
		CString glxl;
		double glsml;
		double glmml;
		double gleml;//
		int torf;//1同向坡 -1反向坡
		double sml;//在sml~eml范围内有关联道路，对应的里程为glsml~gleml
        double mml;		
		double eml;//对应关联道路glxl
		int qorh;
		int glxlxz;//关联道路性质 0-主线 1-匝道
	};

	struct PPCDdata//爬坡车道数据
	{ 
		double sdml,edml;//"分段起终里程"
		double max_hp;//最大横坡
	};
	

	BAS_DRAW_FUN();
	virtual ~BAS_DRAW_FUN();
	//***********************CAD绘图相关函数***********************
	//创建图层
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId CreateLayer(ACHAR *LayerName);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType, AcCmColor color, AcDb::LineWeight weight = AcDb::kLnWtByLayer);
	//删除图层
	void dlayer(ACHAR lname[]);
	//设置当前图层
	static void SetCurLayer(ACHAR *LayerName);
	//修改图层颜色
	static void SetLayerColor(ACHAR* LayerName, int ColorIndex);
	//绘制表格？
	void maketable(struct Table& tab, int icolor = 0, double startWid = 0, double EndWid = 0, ACHAR* LayerName = NULL);
	//加入到数据库
	static AcDbObjectId AddEntityToDbs(AcDbEntity *pEntity, int mode = 0, int ShiftSpace = 0);
	//绘制圆
	static AcDbObjectId  makecircle(AcGePoint3d& startPt, double R, int icolor, ACHAR *LayerName = NULL);
	bool makecircle(AcGePoint3d& centerPt, double R, AcDbObjectId& circleId, int iColor);
	//绘制圆弧
	static AcDbObjectId   makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx);
	AcDbObjectId  makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx, int color, int fh = 1, ACHAR *LayerName = NULL);
	Adesk::Boolean  G_makearc(AcGiWorldDraw* pWd, AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx);
	bool makearc(AcGePoint3d startPt, double dFwj, double R, double Len, int LorR, AcDbObjectId& arcId, int iColor);

	static AcDbObjectId CreateHatch(AcGePoint3dArray Pts, ACHAR* patName, bool bAssociative,int ColorIndex,bool iscir=false);
	//绘制直线
	static AcDbObjectId   makeline(AcGePoint3d& startPt, AcGePoint3d& endPt);
	static AcDbObjectId   makeline1(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor);
	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor);
	Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor, AcDb::LineWeight lw);
	static AcDbObjectId makeline(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	bool makeline(AcGePoint3d startPt, double dFwj, double Len, AcDbObjectId& lineId, int iColor);
	bool makeline(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId& lineId, int iColor);
	//绘制多段线
	AcDbObjectId CreateDLPolyLine(AcGePoint3dArray points, int ColorIndex, double SetoffX, double SetoffY, int ClassGrade = 1);
	AcDbObjectId Create3dPolyLine(AcGePoint3dArray points, int ColorIndex,double SetoffX, double SetoffY,int ClassGrade=1);
	AcDbObjectId   makepolylineTD(AcGePoint3dArray& PtAy, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = L"RAILL", double LTScale = 1.0, double Offset = 0.0)  const;	//20190707添加
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy, int icolor = 0, AcDb::LineWeight lw = AcDb::kLnWtByLayer, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = NULL, double LTScale = 1.0);
	AcDbObjectId makepolyline(AcGePoint3dArray PtArray, bool isClosed, int icolor = 0, AcDb::LineWeight lw = AcDb::kLnWtByLayer, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	static AcDbObjectId  make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, ACHAR *layer = NULL, double ConstWide = 1.0);
	AcDbObjectId  make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, AcDbObjectId LayerId);
	bool makepolyline(AcGePoint2dArray& vertices, AcDbObjectId& polylineId, int iColor);
	//绘制文字
	static AcDbObjectId   maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor = 0, int mode = 3, ACHAR* textStyle = L"宋体", ACHAR *LayerName = NULL);
	static AcDbObjectId MakeMText(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, double Width,
		int icolor = 0, AcDbMText::AttachmentPoint mode = AcDbMText::AttachmentPoint::kMiddleCenter, ACHAR* textStyle = L"宋体", ACHAR *LayerName = NULL, double LineSpacingFactor = 1.0);
	Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd, AcGePoint3d& Pt, ACHAR* ctext, double ang, double texth, int icolor, int mode = 1);
	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor, int mode, ACHAR* textStyle, AcDbObjectId layerId);
	AcDbObjectId maketext(AcGePoint3d lpos, AcGePoint3d rpos, ACHAR* ctext, double ang, double texth, int icolor = 0, ACHAR* textStyle = L"宋体", ACHAR *font = L"simplex.shx", ACHAR *bigfont = L"hzdx.shx", ACHAR *LayerName = NULL);
	//绘回旋线
	AcDbObjectId  drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND);
	AcDbObjectId  drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND, int color, double Lwid, ACHAR *LayerName);
	Adesk::Boolean  G_drawspi(AcGiWorldDraw* pWd, double INDATA[5], double LO, xlpoint *P0, xlpoint *PZ, double ND, int icolor = -1);
	//绘线元
	AcDbObjectId  GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz);
	AcDbObjectId  GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz, int color, double Lwid, ACHAR *LayerName);
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd, double INDATA[5], xlpoint *P0, xlpoint *PZ, int icolor = -1);
	bool DrawXY(double XYItem[10], AcDbObjectId& XYId, int iColor);
	//arrayd数组绘制多段线
	AcDbObjectId createPl_xlpm(int trow, double arrayd[100][6]);

	AcGePoint2d GetOrthPt(AcGePoint2d pt0, AcGePoint2d pt1, AcGePoint2d pt2);
	//绘制标高
	void NoteBG(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR *layername = NULL, int Dir = 1);
	void DrawBGTri(AcGePoint3d Opt, double sfactor, int iColor, ACHAR *layername = NULL, int Dir = 1);
	void NoteBGWithLine(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR* layername = NULL);
	//绘制solid
	AcDbObjectId  makesolid(AcGePoint3d Pt1, AcGePoint3d Pt2, AcGePoint3d Pt3, int icolor, ACHAR *LayerName);
	//绘制导线
	static void makeDX(AcGePoint2dArray PtAy, double R, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType = NULL, bool dxflag = false);//导线
	//从dwg中绘图
	bool DrawFromDwgFile(ACHAR *PathName, AcGePoint3d& Pt, ACHAR* Layer, double tuscale, double rotation, bool IsModel);  //pt 旋转基点（大地坐标） 
	//线宽转换
	AcDb::LineWeight TranLineWeightFromIntToEnum(int LineWeight) const;

	//***********************CAD操作相关函数***********************
	//获取用户拾取点函数
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT, AcGePoint3d& pT1, AcGePoint3d& pT2);
	int  getPoint(ACHAR* pMessage, AcGePoint3d& pT);
	//获取组函数？
	void  makeGroup(AcDbObjectId groupId);
	//选择实体函数
	AcDbEntity*  selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
	// 在某实体的数据结果缓表中查给定的类型码的组
	struct resbuf *entdxf(struct resbuf *entdata, int gcode);
	//移除一个Group中所有的实体？
	void removeAllButLines(AcDbObjectId groupId);
	//【？】
	void groups();
	//【？】
	void printList(struct resbuf* pBuf);
	//【？】迭代读取实体的顶点
	void iterate(AcDbObjectId plineId);
	//【？】创建组函数	//20190707添加
	AcDbObjectId BAS_DRAW_FUN::CreateGroup(ACHAR *GroupName, AcDbObjectIdArray IdAr) const;	
	//【？】创建3dface
	void CreateAFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color);
	void CreateAFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, AcGePoint3d &p3, Adesk::UInt16 color, ACHAR *LayerName = NULL);
	void CreateFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, Adesk::UInt16 color, ACHAR *LayerName = NULL);
	void CreateFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color, ACHAR *LayerName = NULL);
	//设置线元拓展数据
	void SetXYXdata(AcDbObjectId CurID, AcDbObjectId AftID, double xydata[10]);
	//初始化缓冲区
	void InitResBuf(struct  resbuf   *pTemp);
	//从拓展数据组成线元数组
	void FormXYarray(AcDbObjectId CurID, CArray<XYarray, XYarray>&pXYArr);
	//设置端部横线数据
	void SetDBXdata(AcDbObjectId CurID, CString mdbname, CString roadname, double cml, CString HxBh, double curx = -9999, double cury = -9999, double prex = -9999, double prey = -9999, int xsw = 2);
	//获取端部横线数据
	static bool GetDBXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname, double &cml, CString &HxBh, double &curx, double &cury, double &prex, double &prey, int&xsw);
	static void GetDBHXEntity(AcDbObjectIdArray& IdArr, AcArray<CString>& layerArray, CString mdbname, CString roadname, CString HxBh);
	//设置线路拓展数据
	void SetXLXdata(AcDbObjectId CurID, CString mdbname, CString roadname);
	//获取线路拓展数据
	bool GetXLXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname);
	//从实体获取拓展数据
	int GetXdataFromEntity(AcDbObjectId CurID, XYarray &xray);
	//选取圆、圆弧
	int  GetCircle(ads_name e, ads_point vertex);
	void GetArcPt(double spt[2], double ept[2], double R, int npt, double pt[]);//碟形水沟
	//20190713	新增ZOOM函数
	static void Zoom(double Left, double Top, double Right, double Bottom);
	//从dwg图中创建块
	static AcDbObjectId CreateBlockFromDwgFile(ACHAR *Path, ACHAR *Name);
	//获取道路中心线实体
	static void GetJD_CENTEREntity(AcDbObjectIdArray& IdArr);
	//获取所有实体
	static Acad::ErrorStatus getAllEntity(AcDbDatabase *pDb, AcDbObjectIdArray& IdArr,
		const AcArray<CString>& layerNameArr);
	
	//【？】
	static struct ptt_node *getArcpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_Arcpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_plinept(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct point_lk *findroot(ads_point ps, ads_point pe, struct arc_lst *pxyr, ads_real bulge,
		ads_point ps1, ads_point ps2);
	static struct arc_lst *findxyr(ads_point ps, ads_point pe, ads_real bulge);
	static int getrbpoint(ads_point pt, struct resbuf *ebuf, int code);
	static struct resbuf *get_resbuf(int code, struct resbuf *p);

	//20190718	增加一些CAD相关函数
	//设置执行的对象捕捉模式
	static void Set_OSMODE(short Mode_Type);
	//删除指定范围内的实体
	static void DeleteEntityRectangle(ads_point PtLeftButtom, ads_point PtRightUp);

	//***********************线路几何计算相关函数***********************
	//获取线元长度与线元类型
	static double  Get_INDATA_L(double INDATA[5], int *jj);
	//求给定线元（INDATA[5]）上指定长度（l）的坐标
	static int xlzbxy(struct xlpoint *P0, struct xlpoint *PZ, double l, double INDATA[5], int display);
	//求线路上一点的信息（需给定连续里程）
	static int  xlpoint_pz(double array2[][10], int trow2, struct xlpoint *PZ);
	//求解线元模型？
	int getmodel(double x1, double y1, double x2, double y2, double x3, double y3, double L1, double L2, double R);
	//求两点距离与方位角
	static double xyddaa(double xb, double yb, double xe, double ye, double *f);
	//计算缓和曲线的x、y
	static double spiral_x(double A, double l);
	static double spiral_y(double A, double l);
	//计算曲线要素
	int getBPMTL(double a, double R, double L1, double L2, double *BPMTL);
	//计算缓和曲线p
	double spiral_p(double A, double L);
	//计算缓和曲线m
	double spiral_xm(double A, double L);
	//【？】计算直线与线元的交点
	int CalXYjd(double Spt[3], double Ept[3], double array2[400][10], int trow2, int kk, double ResPt[2], double &ProjMl);
	//是否与线元有交点
	int IfOnXY(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	//给定线元数组、点坐标、点角度，求投影点
	double Search_EXPt(double array2[400][10], int trow2, double Pt1[3], double jiao, double ResPt[2]);
	//定位pt1点所在线元
	int Search_XY(double array2[400][10], int trow2, double Pt1[3]);
	//【？】圆上点的里程
	double Circle_XY_to_ML(double x, double y, double array2[400][10], int kk);
	double Circle_XY_to_ML(double x, double y, double array2[10]);
	//输入一线里程，一线线元参数，二线线元数据，求线间距,二线投影连续里程及投影点
	int CalXjj(double ml1, double array1[400][10], int tarray1, double array2[400][10], int tarray2, double &xjj, double &projml, double resPt[2]);
	//【？】计算交点
	void calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt, double *tanl, double *R, double *xc, double *yc, double *ang, double *startfw, double *endfw);
	void caljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd);
	void calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd);
	//输入一线上一点PZ,二线线元数据
	int Judge2XLorR(xlpoint PZ, double array2[400][10], int tarray2);
	//根据线元与长度计算坐标
	int GetXYonSomeXY(double array2[10], double jslen, double respt[3]);
	//计算两线元的交点
	int CalJD_XY1_XY2(double XY1[10], double XY2[10], double ResPt[3], double ResPt1[3]);
	//检测是否在圆弧上
	bool CheckIfOnArc(double array[10], double pt[3]);
	//计算线元指定长度坐标和方位角
	void CalXYXY(double XY[10], double len, double &x, double &y, double &fwj);
	//反向线元
	void ReverseXY(double XY[10]);
	//计算点在线路左右侧
	int JudgePTLorR(double N, double E, double &dist, double array[4 * MAXJDNUM][10], int tarray);
	//查找指定里程在何线元上
	int FindiXY(double cml, double array2[][10], int trow2);
	//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
	void get_T_Len(double zj, double R, double RB, double L1, double RE, double L2, double &T1, double &T2, double &L);
	//计算缓和曲线切线
	void Get_HHQX_T1T2(double A, double RB, double RE, double &TB, double &TE);
	//检测线路是否相交
	void CheckIfXLintersect(double array1[4 * MAXJDNUM][10], double JDarray1[MAXJDNUM][6], int NunJD1, double array2[4 * MAXJDNUM][10], double JDarray2[MAXJDNUM][6], int NunJD2, AcGePoint2dArray* JDpt);
	//是否三角形相交
	bool IfTriangleXJ(AcGePoint3dArray Triangle1pt, AcGePoint3dArray Triangle2pt);
	//计算线路交点
	void CalXLjd(double array1[4 * MAXJDNUM][10], int Sxynum, int Exynum, double array2[4 * MAXJDNUM][10], int Sxynum2, int Exynum2, AcGePoint2dArray* JDpt);
	//检测是否相交
	bool checkIfXJ(double array[10], double array2[10], double depth);

	//计算线元交点
	int CalJD_ZX_ZX(double Line1_array[10], double Line2_array[10], double ResPt[2]);
	int CalJD_ZX_ARC(double Line_array[10], double ARC_array[10], double ResPt[2], double ResPt1[2]);
	int CalJD_ZX_Spiral(double Line_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_ARC_Spiral(double Arc_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_Arc_Arc(double Arc1_array[10], double Arc2_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_Spiral_Spiral(double Spiral1_array[10], double Spiral2_array[10], double ResPt[3], double ResPt1[3]);

	int CalJD_ARC_Spiral_diedai(double &l2_arc, double &l2_spiral, double Arc_R, double Arc_Spt[3], double Arc_Len, double Arc_ZorY,
		double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY);
	int CalJD_line_Spiral_diedai(double &l2_line, double &l2_spiral, double line_Spt[3], double line_Len,
		double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY);
	int CalJD_ARC_ARC_diedai(double &l2_arc1, double &l2_arc2, double Arc1_R, double Arc1_Spt[3], double Arc1_Len, double Arc1_ZorY,
		double Arc2_R, double Arc2_Spt[3], double Arc2_Len, double Arc2_ZorY);
	int CalJD_Spiral_Spiral_diedai(double &l2_spiral1, double &l2_spiral2, double Spiral1_A, double Spiral1_Spt[3], double Spiral1_Len, double Spiral1_ZorY,
		double Spiral2_A, double Spiral2_Spt[3], double Spiral2_Len, double Spiral2_ZorY);

    
	//***********************角度计算函数***********************
    //【度分秒（double）】转【弧度】
	static double rad(double d_ms);
    //【弧度】转【度分秒（double）】
	static double dms_rad(double rad);
    //【度分秒（double）】转【度数】
	static double deg(double d_ms);
    //【度数】转【度分秒（double）】
	static double dms(double ddd);
	//【度数】转【度分秒（字符串）】（带小数位数控制）
	double dms(double ddd, int Nw, ACHAR* DMS_Str);
	//【度数】转【度分秒（字符串分离）】
	void atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4]);
	//【度分秒】转【度分秒（字符串）】
	void dTodms(double dd, int Nw, ACHAR *dms);
	//检测方位角
	void CheckFwj(double &fwj);
	double FormFwj(double& dFwj);
	static void StdJiao(double &jiao);
	//两点连线与X轴的距离
	double angleX(AcGePoint3d sPtL, AcGePoint3d ePtL);//Line(sPtl,ePtl)
	double angleX(AcDbLine *pL);
	//计算两点方位角和距离
	double distfwj(double xb, double yb, double xe, double ye, double *f);//与Y轴夹角f,返回两点之距
	//【方位角】转【与X轴夹角】
	double FwjtoAngX(double ang);
	//计算方位角
	double fwj(AcGePoint3d sPt, AcGePoint3d ePt);//与Y轴夹角
	double fwj(AcDbLine *pL);//与Y轴夹角
	//计算转向角
	double PXJ(double fwj1, double fwj2);
	//计算方位角
	double fwj(double x0, double y0, double x1, double y1);
	//计算转向角
	double AFA(double fwj1, double fwj2);


	//***********************几何计算函数***********************
	//计算边侧函数
	double GetSide(double Xo, double Yo, double x1, double y1, double x, double y);
	//求直线段与直线段的交点 （笛卡儿坐标）
	int crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point pp);
	//计算直线交点
	static int INTERS(double *x, double *y, double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	//计算距离函数
	static double caldistance(ads_point pt1, ads_point pt2);
	//【？】
	void CalLineExtension(int n, double l, double &xval, double &yval);
	//判断三角形trino内是否包含点PT 是 返回true 否 返回false 
	bool CheckPtInTri(AcGePoint3d PPT, AcGePoint3d ppt0, AcGePoint3d ppt1, AcGePoint3d ppt2);
	//计算左右侧
	double chkd(double pt11[3], double pt22[3], double pt33[3]);
	double CCW2(double pt1[2], double pt2[2], double pt3[2]);
	//两条直线方位角分别为Fwj1，Fwj2。计算其夹角
	double CalJiaJiao(double Fwj1, double Fwj2);
	//由方位角求解向量
	static AcGeVector3d calfwjvec(double fwj);
	//【？】获取交点
	int GetLeftOrRight3JD(double N1, double E1, double N0, double E0, double N2, double E2);
	void GetCenter3JD(double N1, double E1, double N0, double E0, double N2, double E2, double R, double Lo1, double Lo2, double &CenN, double &CenE);
	void GetQD(double CenN, double CenE, double R,
		double OutN, double OutE, int LRFlag,
		double &QDN, double &QDE);
	//计算弧长
	double GetArcLengthBy2PointOnCircleAndLR(double CenN, double CenE, double R,
		double N1, double E1, double N2, double E2, int lr);
	//三点算圆
	double getArc3Pt(AcDbArc *pArc, AcGePoint3d &ptb, AcGePoint3d &pt, AcGePoint3d &pte);//平面
	AcDbArc *getArcFrom3Pt(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte);//平面
	void getArcCenFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2, AcGePoint3d &cen);//平面//返回半径，INPUT：圆弧上三点
	AcDbArc* getArc(double x1, double y1, double x0, double y0, double x2, double y2);//平面//返回半径，INPUT：圆弧上三点
	double getArc3Pt(AcDbArc *pArc, double &x1, double &y1, double &x0, double &y0, double &x2, double &y2);//平面
	int getLRRot(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte);
	double getArcFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2);//平面//返回半径，INPUT：圆弧上三点
	//点到直线距离
	double distptol(AcGePoint3d pt, AcDbLine *Line);
	//点到线元上的最近点
	double CircleNearestPoint(AcGePoint3d Pt, AcDbCircle *Circle, AcGePoint3d &NearestPt);
	double ArcNearestPoint(AcGePoint3d Pt, AcDbArc *Arc, AcGePoint3d &NearestPt);
	static double LineNearestPoint(AcGePoint3d Pt, AcDbLine *Line, AcGePoint3d &NearestPt);
	//求点到线的距离 
	static double DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
		double &outY, double &distTo2PointOnLine);
	//【？】计算圆曲线半径
	double radiusfree(AcDbLine *Line1, AcDbLine *Line2, AcGePoint3d pk);
	//计算切线
	bool GetTangent(double CenX, double CenY, double R, double  &OutX, double  &OutY, double  &TX, double &TY);
	//获取直线终点
	void getLineEndPt(AcGePoint3d sPt, double len, double fwj, AcGePoint3d &ePt);
	//计算垂直点
	void GetPerPoint(double outX, double outY, double x1, double y1, double x2, double y2, double &perX, double &perY);
	//plh04.14	TPR确定圆
	bool GetCircleFromT_P_R(double x1, double y1, double x2, double y2, double R, double &cenX, double &cenY);
	//plh04.14//给定一个交点，直始x1,y1,交点x0,y0,终止x2,y2,是否遵守半径系列，返回半径
	double GetRFromT1T2(double &x1, double &y1, double &x0, double &y0, double &x2, double &y2, bool IsStandard = false, double DV = 350, int INTRadius = 10, int JUDGET012 = 0);
	//plh04.14//传入切线方向，返回半径,前一个切点，交点，后一个切点
	double GetRFromT_P_P(double x1, double y1, double x2, double y2, double &xs, double &ys, double &x0, double &y0, double &xe, double &ye, double &cenX, double &cenY, AcDbObjectIdArray &eraseId, bool IsStandard = true, double DV = 350);
	

	//***********************通用计算函数***********************
	//阶乘
	static double factorialfun(int n);
	//寻找坡度
	int FindiPD(double cml, double CM[], int NCM);
	//返回符号
	static int SIGN(double a);
	//【？】
	virtual double func(double x);
	//【？】二分法，黄金分割
	void mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc);
	double golden(double ax, double bx, double cx, double tol, double& xmin);

	//***********************通用操作函数***********************
    //转换小数位数
	static void RToS(double lc, int NLC, ACHAR ch[]);
	//标注时将数值转为串，同时去掉无效的零
	CString DoubleToStr(double xz);
	//寻找指定字符c在字符串中的位置str
	int F_num(ACHAR *str, ACHAR c);
	//在里程字符串中加上“+”	put "+" to mileage (e.g. CK153500.0 --> CK153+500.0)
	void putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place);
	//空格与$之间相互转换
	void NullInStrto$(CString& text);
	static void InStr$toNull();
	static CString InStr$toNull(CString text);

	//读取工作目录
	int ReadRailWorkdir(ACHAR workdir[256], ACHAR pro[256]);
	static int ReadWorkdir();
	//由数据库的全路径得到方案名
	CString GetProByMdbName(CString MdbName);
	//获取当前方案MDB名
	static CString GetCurProMdbPath();

	//【？】
	void CalArrowPt(AcGePoint3d pt, double fwj, double RePt[2][2]);

	//平面切图用工具函数
	bool BAS_DRAW_FUN::CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &pt1, ads_point &pt2, ads_point &pt3, ads_point &pt4);
	bool CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, AcGePoint3d &Pt1, AcGePoint3d &Pt2, AcGePoint3d &Pt3, AcGePoint3d &Pt4);
	void ZoomTriangleByPoint_O(double dist, AcGePoint3d O, AcGePoint3d &P1, AcGePoint3d &P2);
	double distptol(AcGePoint3d O, AcGePoint3d P1, AcGePoint3d P2);
	//初始化改移道路数据
	void IniMdfRoadData(CString MdfRoadNmae, ModifyRoadTab& ModifyRoad);
	//读取文本文件的行数
	int CountLines(ACHAR *filename);

	
	//***********************线路里程相关函数***********************
	//冠号和dml组合成现场里程
	static CString LCchr(CString GH, double lc, int NLC);
	static ACHAR*  LCchr(ACHAR  *GH, double lc, int NLC, int mode);
	//统一里程转现场里程（不带冠号计算）
	double XLC1(double TYLC, int NDL, DLBZ DLBTMP[30]);
	//统一里程转现场里程（带冠号计算）
	double XLC1(double TYLC, ACHAR* GH, int NDL, DLBZ DLBTMP[30]);
	//现场里程转统一里程（带冠号计算）
	double TYLC1(ACHAR  ckml[], int NDL, DLBZ DLBTMP[30]);
	//现场里程转统一里程（不带冠号计算）
	double TYLC1(double  dml, int NDL, DLBZ DLBTMP[30]);
	//计算转向角
	static double ZXJ(double s2, double s1);
	//计算投影里程
	double PROJ_ML(double array2[][10], int trow2, double xout, double yout, double xon, double yon);
	double XY_to_ML(double x, double y, double array2[400][10], int trow2);

	//拆开现场里程
	void split_ckml(CString str, double *ml, CString &ck);
	void split_ckml(ACHAR str[], double *ml, ACHAR  *ck);
	//dml转Ckml
	void dmltockml(CString GH, double dml, ACHAR ckml[80]);


	//***********************规范计算相关函数***********************
	//计算竖曲线
	double Get_Rshu(double design_V, double deltaI);
	//计算缓和曲线
	double get_l0(double SR, double Design_V);
	double get_l0(double Design_V, int KNDD, double SR);
	//double get_l0(double R);
	//计算圆曲线半径
	double get_R(double SR,double Design_V);
	//计算线间距
	double get_xjj(double Design_V);
	//计算线间距加宽
	double get_XJJK(double Design_V, double SR);
	//规范曲线路基加宽
	double Get_QXJK(CString TLDJ, double design_V, double SR);
	//设置纵断面参数
	void SetZdmMinPC(int DV, CString &MAXPD, CString &YBPCMin, CString &JXPCMin, CString &T_YBRshuMin, CString &T_JXRShu,
		CString &A_YBRshuMin, CString &A_JXRShu, CString &YBLRshuMin, CString &JXLRShuMin);
	//读取道路等级与设计速度
	void ReadRoadDV(int &DV, CString &grade);
	//根据设计速度设置路面宽
	static void SetLMKByDV(CString grade, int DV, double &fgdk, double &lydk, double &cxdk,
		double &lmhp, double &yljk, double &yljhp, double &tljk,
		double &tljhp, int &bkmode, int &sjbgmode);
	//计算超高
	void FindSuphI(int DV, double MAXCG, double Ig, int &NSuphI, double SuphI[10][2]);
	//计算加宽
	void FindJK(CString JKLB, int &NJK, double JK[10][2]);
	//直线超高渐变率
	double FindZXCGJBL(int DV, int mode);
	//计算路面宽加宽
	void CalLMK_JK(double cml, int NLMK, LMKdata lmk[], int njk, CgSZ jk[], double &half_fgdk,
		double &lydk, double &cxdk, double &yljk, double &tljk, double &tljhp,
		int &bkmode, int &sjbgmode);
	//依据车速取出匝道超高值
	void FindZD_SuphI(int DV, int &NSuphI, double SuphI[10][2]);
	//获取最小缓和曲线长
	void GetLoMin(double DV, double &Lmin);
	//获取最小曲线长
	void GetCirLenMin(double DV, double &Lmin);
	//检测是否相同平面
	bool checkIfSamePm(double cml, double DV, double array2[][10], int trow2);
	//检测是否相同纵面
	bool checkIfSameZm(double cml, double DV, double CM[], int NCM);
	//获取最小T和最小L	plh04.14
	virtual void GetT0min_Lrmin(double DV, double &T0min, double &Lrmin);
	//计算不同车速下最小圆曲线半径	20190724
	double Cal_Road_MinR(double DesignSpeed);

	//***********************拟合相关函数***********************
	void fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
		double& b, double& siga, double& sigb, double& chi2, double& q);
	double gammq(double a, double x);
	void gser(double& gamser, double& a, double& x, double& gln);
	void gcf(double& gammcf, double& a, double& x, double& gln);
	double gammln(double xx);
	void medfit(double x[], double y[], int ndata, double& a, double& b, double& abdev);
	double rofunc(double b, double xt[], double yt[], int ndatat, double &abdevt);
	void  sort(int n, double ra[]);
	int sgn(double number);

	//***********************图框相关函数***********************
	////TRIM//////////////////////////////////////////////////////////////////////	
	static void DelBoxObject(ads_point p0, ads_point p1, ads_point p2, ads_point p3);
	static int AutoEntBK(ads_point p0, ads_point p1, ads_point p2, ads_point p3, int ITK = 1234);
	static int ZDKAutoEntBK(ads_point qLpt, ads_point hLpt, ads_point hRpt, ads_point qRpt, int ITK);
	static int AutoEntBK1(ads_point p0, ads_point p1);
	static void fline(ads_point pt1, ads_point pt2, ads_point result, double delta);
	static void CalFrame(double dist, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &p1, ads_point &p2, ads_point &p3, ads_point &p4);
	static void AutoEntTransform(AcGeMatrix3d xformr, AcGeMatrix3d xformt, ads_point p1, ads_point p2, ads_point p3, ads_point p4);
	////CAL TU KUANG//////////////////////////////////////////////////////////////////////	
	static void getMinMaxXY(ads_point p1, ads_point p2, ads_point p3, ads_point p4, double &xmin, double &xmax, double &ymin, double &ymax);
	static double cal_h(ads_point pt1, ads_point pt2, ads_point pt3);
	static double FAngle(double s1, double s2);
	static double chk(double pt1[3], double pt2[3], double pt3[3]);
	//////////////////////////////////////////////////////////////////////////	

	// 两单元(基本)模式法 2005.07.14 ldf
	bool Msf2Elem(AcDbObjectId Id1, AcDbObjectId Id2, int &iLinkModel, int LeftOrRight, double A[2], double XYArray[2][10], int& iXYNum);
	// 依据两圆关系，自动确定连接方式 2005.07.14 ldf	
	int  AutoModel(AcGePoint3d Center1, AcGePoint3d Center2, double R1, double R2);
	//三单元(扩展)模式法解算
	bool Msf3Elem(AcDbObjectId sId, AcDbObjectId cId, AcDbObjectId eId, int& iQLinkModel, int& iHLinkModel, int LeftOrRight, double A[4],
		double XYArray[5][10], int& iXYNum);
	//两单元(基本)模式法解算(固定起点及连接参数A)
	bool Msf2ElemFitA(AcDbObjectId Id1, AcDbObjectId Id2, AcGePoint3d BasePt, double dFwj, int iLinkModel, int LeftOrRight, double A[2],
		double XYArray[2][10], int& iXYNum);
	//【？】合并边线
	void MergeBX(BXLMKD &oldbx, BXLMKD &newbx, int inf, BXLMKD &resbx);
	//由于修正里程与高程操作的需要,重新计算坡度\坡长
	void NewDegree(vector<PDTAB> &Slope);
	//根据里程计算高程
	double MLToH_Bas(double cml, int ncm, double cm[], double& degree);
	double VerticleArc(int iSlope, double cml, double cm[]);
	struct ConstraintErr
	{
		double CurDml;
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
		ConstraintErr()
		{
			CurDml = 0.0;
			_tcscpy(InterCkml, L"");
			InterN = 0.0;
			interE = 0.0;
			CurFwj = 0.0;
			InterFwj = 0.0;
			JiaJiao = 0.0;
			CurDesH = 0.0;
			InterDesH = 0.0;
			DetaH = 0.0;
			DetaH = 0.0;
			MinAng = 0.0;
			MinDetaH = 0.0;
		}

	};//不满足交叉约束的错误信息
} ;

#endif // !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
