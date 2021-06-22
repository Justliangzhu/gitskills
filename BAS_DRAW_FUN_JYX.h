// BAS_DRAW_FUN_JYX.h: interface for the BAS_DRAW_FUN_JYX class.
//
//////////////////////////////////////////////////////////////////////
// BAS_DRAW_FUN_JYX.h: interface for the BAS_DRAW_FUN_JYX class.
//
//////////////////////////////////////////////////////////////////////
//#include "RAILDESIGN.H" 
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
#include <iostream>


#include "rxdefs.h"
#include "UltraCArray.h"
#include "StdAfx.h"
//#include "EXCEL9.h"
//#include "GTZX_JYX.h"
// #include "BAS_DRAW_FUN_JYX.h"

//#include "ol_errno.h"
//#include "STDIO.h"
//#include "malloc.h"

#include <dbregion.h>

#if !defined(AFX_BAS_DRAW_FUN_JYX_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
#define AFX_BAS_DRAW_FUN_JYX_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_
#define pi 3.14159265358979323846
#define PI 3.14159265358979323846
#define Pi 3.1415926535897932384626433832795
#define TRIMVERSION 20040220
#define MAXJDNUM 200
#define MAXBPDNUM 300
#define MAXDLNUM 200
#define PRECISION 1.0e-6
#define MAXGJD 200
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Spoint(pt, x, y, z)  pt[X] = (x);  pt[Y] = (y);  pt[Z] = (z)
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]


//class GTZX;
struct ptt_node_JYX{
	ads_point pt0;
	ads_point pt;
	ads_point pt1;
	struct ptt_node_JYX *next;
};
struct arc_lst_JYX{
	ads_real R;
	ads_real xo;
	ads_real yo;
};
struct point_lk_JYX{
	ads_point pt;
	struct point_lk_JYX *next;
};

class BAS_DRAW_FUN_JYX  
{
public:
	BAS_DRAW_FUN_JYX();
	virtual ~BAS_DRAW_FUN_JYX();

	double m_dHalfPI;

	void InitNameOfWay(CComboBox *m_Combo_WayName,int JYorGJ,CString m_Edit_FName);//初始化线路名

	int DrawZorY; //1 从左向右画 -1 从右向左画
	struct NoteStr
	{
		int rownum,colnum,ntext;
		int fitinf;//=0 not fit mode ; =1 fit mode;
		double texth,dv;//dv fit时 算位置时的除数
		CString text;//表格内只有一行文本 ntext=1
		CString text1;//否则两行文本 ntext=2
		CString text2;//三行文本 ntext=3
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
    GuiFanCanShu mGJGuiFanCS;
	struct ZDMError
	{
		int m_iBPDIndex;

		//错误类型:0-坡度代数差超限 1-竖缓重叠 2-坡度超限 3-高程超出高程控制点 4-坡长小于最小坡长
		int m_iErrorType;

		ACHAR m_ckml[80];
		double m_dTYLC;
	}; 


	void createBlocks();
	AcDbObjectId maketext1(AcGePoint3d lpos, AcGePoint3d rpos,ACHAR* ctext,double ang,double texth ,int icolor=0,ACHAR* textStyle=L"宋体",ACHAR *font=L"simplex.shx",ACHAR *bigfont=L"hzdx.shx",ACHAR *LayerName=NULL,int i=0);
	AcDbObjectId   makeline1(AcGePoint3d startPt,AcGePoint3d endPt);
	AcDbObjectId   makeline1(AcGePoint3d startPt,AcGePoint3d endPt,int icolor,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	AcDbObjectId make2dline1(AcGePoint3dArray ptarray,int icolor=0,double startWid=0,double EndWid=0,ACHAR *LayerName=NULL);
	AcDbObjectId maketext1(AcGePoint3d pos, ACHAR* ctext,double ang,double texth ,int icolor=0,int mode=4,ACHAR* textStyle=L"宋体",ACHAR *LayerName=NULL);

	struct Table //表格结构
	{
		int nrow,ncol;
		double RowH[30],ColWid[8];
		AcGePoint2d LDpt; 
		int Nstr;
		NoteStr Str[30];
	};
	struct LTJCflag//立体交叉图块
	{
		AcDbObjectId blockId;
		CString blockname;
	} Ltjc[2];

	struct STATIONflag//车站图块
	{
		AcDbObjectId blockId;
		CString blockname;
	} Staflag[140];
	int SflgN;

	struct PJDflag//平交道图块
	{
		AcDbObjectId blockId;
		CString blockname;
	} Pjdflag[8];
	int PjdN;

	void maketable(struct Table &tab,int icolor=0,double startWid=0,double EndWid=0);


	////任意点坐标法计算拨量所用函数
	struct  CXZYDFD{
		CString GH;
		double dml;//行政区统计分段终点里程
		CString BZText;//备注
	};

	struct  CYDLXFD{
		CString GH;
		double dml;//用地类型分段终点里程
		CString YDType;//用地类型
		double cml;//对应连续里程
		int xh;//对应用地类型序号
	};

	struct BCTAB  //标尺
	{
		CString NameOfWay;
		ACHAR ckml[80];
		double Level;
	}; 

	struct RAILSTU //轨道结构表
	{
		CString XLName;
		CString	FdKml;//分段终里程
		CString	StruType;
		double cml;
		double StruHeight;
	};

	struct EXDLB
	{//二线断链表
		ACHAR XLName[50]; //线路名
		int no;//表示第几个绕行段
		CString RXDName;//对就绕行段名称
		double BLC,ELC,TYLC;  //  断链的断前里程、断后里程、断链处统一里程
		CString   BGH,EGH; //  断链的断前里程冠号和断后里程冠号	

		double x;
		double y;
	};








	struct SetLineAttriStruct
	{//线路属性数据
		CString XLName;  //线路名
		int ColorIndex; //颜色
		int LineW;  //线宽
	};

	struct DoubleLineData//双线平面数据
	{
		Adesk::Int16 JDXZ;//交点性质 0-并行段交点,1-绕行段交点
		Adesk::Int16 jdno;//二线交点号(下标)
		CString BaseLineJDName;	//基线交点名	20191114新增

		double x,y,Lo1,R,Lo2;
		Adesk::Int16 jdno1;//对应一线交点序号
		double xjj;//线间距//二线在左为－在右为＋
		double qxjk;//曲线加宽
		double NYDL;//曲线内业断链
		double ZXNYDL;//直线内业断链
		ACHAR ZHLC[80];//直缓里程
		ACHAR JDLC[80];//交点投影到一线上的里程//plh05.08
		bool ISXJJFirst;//是否根据线间距计算
		bool ISStartEnd;//是否为换边的起终点
		bool ISShow;//是否显示给用户

		ACHAR XLName[50]; //线路名

		ACHAR JDNum[40];
	};
	////////////////////////////////////////以上plh

	typedef UltraCArray_T<DoubleLineData,DoubleLineData> RXDInfoArray;
	typedef UltraCArray_T<EXDLB,EXDLB> EXDLBArray;
	struct RXDInfo//绕行段信息
	{
		RXDInfoArray m_arrJD;
		EXDLBArray m_arrDLB;
	};
	//////////////////////////////////////////////////////////////////////////



	struct xlpoint { double x,y,a,r,lc; };


	////点跨边结构
	//struct  xlpointRorSSide
	//{
	//	BOOL IsSpanSide ;
	//	xlpoint JieRuPZ; //接入边
	//};

	 struct DLBZ  //冠号改变点、断链点、起、终点的冠号、里程表
	{
		//注意：基线断链表中的断链的ELC不一定有值,此时要通过BLC + DL 算出 ELC
		double BLC,ELC,DL;  //  断链的断前里程和断链值(+值为长链;-为短链)

		ACHAR   BGH[8],EGH[8]; //  断链的断前里程冠号和断后里程冠号
		ACHAR   GJDNum[255];   //改建段段名（只针对改建段）

		double N,E; //大地坐标
	};

	struct QXTAB//曲线表
	{
		int jdno;//交点号
		double x,y,zj,R,l1,l2,T1,T2,L,jzxl;
		CString BGH,EGH;//ZH,HZ桩冠号
		double Bml,Eml;//起终里程
		double InDL;//		内业断链
	};


	//struct TWTAB//填挖表
	//{
	//	CString GH;//加桩冠号
	//	double  ml;//加桩里程
	//	double  DMGC,SJGC;//地面高程,设计高程
	//	double SJPD;//设计坡度
	//	double SQXZJ;//竖曲线纵距
	//	double ZXTJ;//中心抬降值
	//	double ZXTWG;//中心填挖高
	//	double LJSGG;//路肩施工高
	//	double QXJK;//曲线加宽
	//	double LJSGK;//路肩施工宽度
	//	ACHAR CKml[80];
	//};


	struct TWTAB//填挖表
	{
		CString NameOfWay;//线路名
		CString GH;//加桩冠号
		double  ml;//加桩里程
		double  DMGC,SJGC;//地面高程,设计高程
		double JYGMGC,SJGMGC;//轨面高程
		double SJPD;//设计坡度
		double SQXZJ;//竖曲线纵距
		double ZXTJ;//中心抬降值
		double ZXTWG;//中心填挖高
		double LJSGG;//路肩施工高
		double ZQXJK,YQXJK;//曲线加宽
		double LJSGK;//路肩施工宽度
		double ZXJJ,YXJJ;//左右线间距
		double GTJXXJJ,GTEXXJJ;//设计基线，设计二线相对于既有基线的线间距
		double LJFLK;//路肩分离宽
		CString FZ;//附注
		int DMinf;//断面信息 在CalJK计算
		/*
		DMinf = 0 //单线路基
		1//左线（II线）并行
		2//右线（II线）并行
		3//三线并行
		4//左线（II线）绕行,路肩未分离（按一线计算整体式路基）
		5//左线（II线）绕行,路肩分离（按一线和左线资料分别计算路基）
		6//右线（II线）绕行,路肩未分离（按一线计算整体式路基）
		7//右线（II线）绕行,路肩分离（按一线和右线资料分别计算路基）
		*/

		ACHAR CKml[80];
	};

	struct HdmAreaData//横断面成果面积数据结构
	{
		CString NameOfWay;//线路名
		CString GH;
		double dml;
		double BTinf;//断面性质
		double at;//填面积
		double aglt;//改良土面积
		double asst;//渗水土面积
		double aqbt;// 清表土面积
		double acut;//挖方面积
		double aw[6];//六类挖面积
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
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	//既有改建数据结构
	struct BoLiangTable//既有曲线拨量计算结果表
	{
		ACHAR JDNum[20];//交点号
		double ZHCml,HYCml,YHCml,HZCml;
		double alfa,R,l1,l2,KZH,KHY,KYH,KHZ;
		ACHAR GH[20];
		bool IsMeasure;//是否外业实测

		ACHAR XLName[36];
		// 		CString BGH,EGH;//ZH,HZ桩冠号
		// 		double Bml,Eml;//起终里程
		// 		double InDL;//		内业断链
	};

	//外移桩导线点数据结构
	struct WaiYiZhuang 
	{
		ACHAR m_cGH[16];
		double m_dLC;

		ACHAR m_cCKml[16];
		double m_dCml;


		double m_dN;
		double m_dE;
		double m_dOffset;//外移距 左负右正
		int m_iZXOrQX;//-1-直线段起点，1-直线段终点，0-直线中 3-直线点 2-曲线
		ACHAR XLName[40];//线路名
	};

	struct ZMTaiZhang
	{
		ACHAR m_cGH[16];
		double m_dLC;
		double m_dCml;

		ACHAR XLName[40];//线路名

		ACHAR m_cCKml[16];

	};

	struct XingZhengQuHua
	{
		CString m_sSml;
		CString m_sEml;

		CString m_sName;

		double m_dSmlTYLC;
		double m_dEmlTYLC;
	};



	struct XLDStruct// 线路分段信息
	{
		double Bcml,Ecml;
		double BpointToN,BpointToE,EpointToN,EpointToE;
		CString XLName;
	};

	//struct JDDataTable//既有曲线拨量计算结果表
	//{

	//	ACHAR jdno[20];//交点号
	//	double x,y,R,l1,l2;

	//};

	struct OriginalMeasureTable//既有曲线原始测量资料
	{
		ACHAR jdno[20];//交点号
		int ZJPoint;
		//	ACHAR GH[20];
		ACHAR Landmark[255];
		double fj;

		double ZhiCeJv;
		double Youjiao;

		double Ej;
		double Es;
		double deE;
		double BLTerritoryPlus;
		double BLTerritoryMinus;
		ACHAR ShuXing[15];

		double LCcha;

		int Fuzhu;

	};

	typedef struct MulFDData  //复曲线分段信息表
	{
		ACHAR jdno[20];//交点号
		double KZH ;
		double KHY ;  //HY里程
		double KYH ;  //YH里程
		double KHZ ;
	};

	typedef struct QLTKStruct // 曲率图
	{
		AcDbObjectId eId ;
		AcGePoint3d ZXPt , YSPt ;
	};


	//struct MulCurveFDTable   //复曲线分段信息表
	//{
	//	ACHAR jdno[20];//交点号
	//	double KHY;  //HY里程
	//	double KYH;  //YH里程
	//};

	//只对读写,实体操作不是这结构
	typedef struct BZLCStructTable  //标志里程表
	{
		//	int Num;
		//ACHAR GH[8];
		ACHAR kml[30];
		double X;
		double Y;
		double K;
		double xyTocml;
		double kmlTocml;
		ACHAR   GJDNum[255];   //改建段段名（只针对改建段）

	};


	typedef	struct JDarrayStruct
	{ //交点结构数组

		ACHAR GJDNum[255];
		ACHAR JDNum[40];
		double N;
		double E;
		double l1;
		double R;
		double l2;
		BOOL isGJ;//True--改建//False--重构
	};


	typedef	struct QXBStruct
	{ //
		double KZH;
		double KHY;
		double KYH;
		double KHZ;
		double a;
		double l1;
		double R;
		double l2;

		double T1;
		double T2;
		double L;
		int LorR;
		ACHAR JDNum[20];
		BOOL isGJ;//True--改建//False--重构

		ACHAR GH[20]; // 里程冠号

		//李伟2007-08-07曲线表中的CKZH等字符串原来长度为20，太小，加大为40
		ACHAR XLName[255];
		ACHAR CKZH[40];
		ACHAR CKHY[40];
		ACHAR CKYH[40];
		ACHAR CKHZ[40];
	};

	// typedef UltraCArray_T<JDarrayStruct,JDarrayStruct>SGJJDarray2;

	struct GJDInf{
		ACHAR XLName[256];
		double spt[2];//起点坐标
		double ept[2];//终点坐标
		int iSJD1,iEJD1;//改建段起终点在一线上的交点号
		int iSJD2,iEJD2;//改建段起终点在二线上的交点号
		int GJDType;//改建段类型，0-并行 1-S弯 2-绕行或者改建 -1　利用段

		int ID;//内部标识
		ACHAR gjdname[256];//外部改建段名

		ACHAR BXLName[256];//起始线路名
		ACHAR EXLName[256];//终止线路名

		ACHAR GJDBLC[50];//起点里程
		ACHAR GJDELC[50];//终点里程
	};//改建段信息表
	//实体


	struct SearchRange
	{
		ACHAR StartXLName[256];
		ACHAR EndXLName[256];
		double spt[2];//起点坐标
		double ept[2];//终点坐标
	};

	typedef	struct ConnectLineTableStruct
	{ 
		ACHAR ToOutSGJJDNum[50];  //对外 改建段编号

		ACHAR ToInSGJJDNum[50];  //对内 改建段编号

		ACHAR BXLName[256];   
		ACHAR EXLName[256];
	};


	typedef struct Gtzx2BZLCStruct
	{
		int i ;
		CArray<BZLCStructTable,BZLCStructTable>BZLCarray;

	};

	typedef	struct SGBXTableStruct
	{ 
		ACHAR SGBXName[50];

		ACHAR ToInSGBXName[50];  //对内
		ACHAR BXLName[256];   
		ACHAR EXLName[256];
	};

	typedef	struct JBJXTableStruct
	{ 
		ACHAR JBJXName[50];
	};



	//改建段汇总表
	typedef	struct GJDAllInforStruct
	{ 
		ACHAR ToOutSGJJDNum[50];//对外 线路名

		ACHAR ToInSGJJDNum[50];//对内 线路名

		double BpointToN;
		double BpointToE;

		double EpointToN;
		double EpointToE;

		ACHAR BXLName[256];//起点线路名
		ACHAR EXLName[256];//终点线路名

		int GJDType; //改建段类型

		ACHAR GJDBLC[50];//起点现场里程
		ACHAR GJDELC[50];//终点现场里程
		ACHAR GJXZ[50];

		double GJDSml;//起点统一里程
		double GJDEml;//终点统一里程

	};





	//李伟，
	struct EXGJDFDInf{
		ACHAR XLName[256]; //既有线线路名
		double spt[2];//起点坐标
		double ept[2];//终点坐标

	};//改建段分段信息表


	//某一改建段　几何参数表
	typedef	struct  OneGJDJDStruct  //  GJDToOneInforStruct
	{ //
		ACHAR SGJDJDNum[50];
		UltraCArray_T<JDarrayStruct,JDarrayStruct&>SGJJDarray;
	};




	struct ListZhFa{//有序组合方案链表
		int pro;     //方案pro入选里程范围
		ACHAR SGH[10];//起冠号
		double Sdml;//起现场里程
		ACHAR EGH[10];//终冠号
		double Edml;//终现场里程
		ListZhFa *prior;
		ListZhFa *next;
	};


	struct EXDuanLuo
	{
		int no;//段落编号
		double sml;//段落起点二线统一里程
		double eml;//段落终点二线统一里程
	};


	struct STATIONSTRUCT
	{
		int iRXD;//绕行段编号
		//站房中心里程冠号，里程数，车站名称，插旗图块名
		CString GH,NamSta,TKSta;
		ACHAR Kml[80];
		double XLC;
	};

	struct BMSTRUCT
	{
		int iRXD;
		//水准点编号、高程、里程冠号，里程数，到线路中线距离，坐标
		CString BianHao,GH;
		double GaoCheng,XLC,DistZX,N,E;

		CString CLMS , FZ ; //材料描述,附注	

		double dTYLC;

		ACHAR cGTJXLC[20];//贯通设计基线上的现场里程

		bool bBXLYOrRX;//并行、利用段(true)还是绕行改建段(false)


	};

	struct DXSTRUCT
	{
		int iRXD;
		//桩号，高程，坐标，方位角
		CString ZhuangHao;
		double GaoCheng,N,E,FWJ;
		double RightAng,Dist,cml;//右角，距离，里程
	};

	////任意点坐标法计算拨量所用函数

	struct SZJDOrCD //测量中点（测点或置镜点）的结构
	{
		ACHAR JDNum[20];
		int iZJDOrCD;//置镜点还是测点，0表示测点，非0时为置镜点点号
		CString csLCBZ;//输入里程
		double dN;//测点或置镜点N坐标
		double dE;//测点或置镜点E坐标
		double dNXL;//拨正后线路上的N坐标
		double dEXL;//拨正后线路上的E坐标
		double dLZJDtoCD;//边长
		double dYoujiao;//右角
		double dFwjHoushi;//后视方位角
		double dPianjiao;//偏角
		double dLC;//测点里程，置镜点时为0.0;
		double dBoliang;//拨量
		CString csCDAttribute;//测点属性
		double dLCCha;//里程差
	};


	//一个
	typedef UltraCArray_T<SZJDOrCD,SZJDOrCD&>OneSZJDOrCD;


	//----------------------纵断面-------------------------------
	struct GMXTAB//轨面线表
	{
		CString NameOfWay;//线路名
		CString CAP;//中桩冠号
		double  ML;//里程
		double  H;//高程
		ACHAR CKml[80];
		double x;//绘图坐标
		double y;
	};

	//道岔表
	struct BranchTrack
	{
		CString StCap;//起点冠号 
		CString EdCap;//终点冠号 
		double StDml;//起点里程
		double EdDml;//终点里程
	};

	struct RoadBedTAB//道床厚度表
	{
		CString NameOfWay;//线路名
		CString GH;//中桩冠号
		double  ml;//里程
		double  H;//道床厚度
		double  H1;//轨道结构高度
		double dml;
		ACHAR CKml[80];

		CString sRemark;//纵面控制点表备注
	};

	struct PDTAB//变坡点表
	{
		CString NameOfWay;
		CString GH;//变坡点冠号
		double  ml;//变坡点里程
		double  Level;//设计高程
		double Rshu,RLen;//竖曲线半径,长度
		double degree,length;//坡度，坡长
		CString Notes;//附注
		ACHAR CKml[80];
		double cml;//连续里
	};

	typedef	CArray<PDTAB,PDTAB> BPDArray;

	typedef UltraCArray_T<PDTAB,PDTAB> APDTAB;
	typedef UltraCArray_T<APDTAB,APDTAB> MPDTAB;


	struct TUNTAB//隧道表
	{
		CString NameOfWay;//线路名
		int iRXD;//绕行段编号。＜0表示并行或单线
		CString TUNName;//隧道名 
		CString BGH;//冠号
		CString EGH;//冠号
		CString flag;//插旗方式
		double StDml;//起点里程
		double StH;
		double EdDml;//终点里程
		double EdH;
		double CenDml;
		double CenH;
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJZT;
		long NumOfLine; //线数
	};


	struct SMALLBRIDGETAB//小桥涵表
	{
		CString NameOfWay;//线路名
		int iRXD;//绕行段编号。＜0表示并行或单线
		CString GH;//冠号
		double ml;//中心里程
		ACHAR CKml[80];
		CString KKStyle;//孔跨式样
		double dwL,dwR;//横向宽度
		double JJiao;//交角
		double ToUse;//用途
		double InsertStyle;//插旗方式
		CString GJXZ;//改建性质
		CString Notes;//附注
		double LLevel,RLevel;//左、右涵口标高
		int LineNum;//线数（1-单线，2-双线）
	};

	struct BasicDataOfZDM//基本参数表
	{
		CString Fname;
		CString WayName;
		double XScale;
		double YScale;
		CString MLDecimal;
		CString PDDeciamal;
		CString HDecimal;
		int TLDLLG;
		int TLDLZG;
		int SJGCLG;
		int SJGCZG;
		int SJPDLG;
		int SJPDZG;
		int JYGCLG;
		int JYGCZG;
		int LCLG;
		int LCZG;
		int QXLG;
		int QXZG;
	};


	typedef struct DhXJJ{
		int JDi;
		double xjj;
		double N;
		double E;
	};

	struct DMXTAB//地面线表
	{
		CString NameOfWay;//线路名
		CString GH;//中桩冠号
		double  ml;//变坡点里程
		double  Level;//设计高程
		ACHAR CKml[80];
	};

	struct STATAB//车站表
	{
		CString NameOfWay;
		CString StaName;//车站名 
		CString BGH,EGH,CGH;//冠号
		double CenDml;//中心里程
		double StDml;//起点里程
		double EdDml;//终点里程
		double CZLX;//车站类型
		CString CQTKM;//插旗图块名
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJXZ;//改建性质
	};


	struct BRITAB//大中桥表
	{
		CString NameOfWay;//线路名
		int iRXD;//绕行段编号。＜0表示并行或单线
		int xs;//单线1双线2
		CString BriName;//桥名 
		CString BGH,EGH,CGH;//冠号
		double CenDml;//中心里程
		double CenH;//中心高程（new）
		double StDml;//起点里程
		double StH;//起点高程(new)
		double EdDml;//终点里程
		double EdH;//终点高程(new)
		CString KKSY;//孔跨式样
		long LineNum;//线数
		CString SWHt;//一百年水位高程
		double BriLen;
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJZT;

	};

	struct QXJKTAB//曲线加宽表
	{
		double Rmin,Rmax,JK;// 		
	};

	struct JiYouXianZhanDiKuan//既有线占地宽
	{
		CString GH;
		double ml;
		double LuJiBianJieKuan;//路基边界宽
		double ZhanDiKuan;//占地宽
	};


	struct YongDiLXB//用地类型表
	{
		ACHAR XLName[40];
		ACHAR GH[20];
		double dml;//里程数
		ACHAR YDLX[40];
	};
	  static  BOOL IsOnSameLine( AcGePoint3d PT1 , AcGePoint3d PT2 , AcGePoint3d PT3 , AcGePoint3d PT4 );

	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	void dTodms(double d, int Nw, ACHAR *dms);
	void DrawRect(AcGePoint3d RightUp,AcGePoint3d LeftDownPt,int Color,double wide);//绘矩形


	bool GetJdno1FromJdstr(CString JDstr, CArray<JDarrayStruct,JDarrayStruct>& JDarray,Adesk::Int16 &Jdno,int CountAllNum);

	void GetJdnoFromJdstr(CString JDstr, Adesk::Int16 &Jdno);

	AcDbObjectId CreateLayer(CString LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight=AcDb::kLnWtByLayer);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight=AcDb::kLnWtByLayer);

	void MoveEntities(AcDbObjectIdArray *EntityIds,double fwj,double dist);

	double fwj(AcGePoint3d sPt,AcGePoint3d ePt);

	void makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType);

	AcDbObjectId CreateGroup(ACHAR *GroupName,AcDbObjectIdArray IdAr);

	void AdjustAngle(double &j);
	void dlayer(CString layername);
	void RotateEntities(AcDbObjectIdArray *EntityIds,AcGePoint3d Pos,double jiao);
	void LCChr(CString GH,CString LC,int NLC,CString &LCStr);
	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,	int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	//	AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL,double LTScale=1.0);

	AcDbObjectId makepolyline(AcGePoint3dArray PtArray,bool isClosed,int icolor,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	void getMinMaxXY(ads_point p1,ads_point p2,ads_point p3,ads_point p4,double &xmin,double &xmax,double &ymin,double &ymax);
	void mZOOM(double x1,double y1,double x2,double y2);

	void RToS(double lc,int NLC,ACHAR ch[80]);

	AcDbObjectId CreateBlockFromDwgFile(ACHAR *Path,ACHAR *Name);

	static double angleX(AcGePoint3d sPtL,AcGePoint3d ePtL);//Line(sPtl,ePtl)
	static double angleX(AcDbLine *pL);

	static void makeDX(AcGePoint2dArray PtAy,double R,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType=NULL,bool dxflag=false);//导线
	static AcDbObjectId CreateLayer(ACHAR *LayerName);
	static AcDbObjectId   AddEntityToDbs(AcDbEntity *pEntity);

	static AcDbObjectId   AddEntityToDbs(AcDbEntity *pEntity,int mode,bool ispapespace=false);

	static AcDbObjectId  makecircle(AcGePoint3d& startPt,double R,int icolor);
	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt);

	static AcDbObjectId   makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	static AcDbObjectId   makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	//????

	bool CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,ads_point &pt1,ads_point &pt2,ads_point &pt3,ads_point &pt4);
	bool CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,AcGePoint3d &Pt1,AcGePoint3d &Pt2,AcGePoint3d &Pt3,AcGePoint3d &Pt4);	
	//四边形缩放函数，P1，P2，P3，P4，四边形四个顶点,Pt1,Pt2,Pt3,Pt4为缩放后的四个顶点
	//d12为边P1P2的平移距离......向内缩为－，向外放为＋；

	void ZoomTriangleByPoint_O(double dist,AcGePoint3d O,AcGePoint3d &P1,AcGePoint3d &P2);
	//缩放三角形，其中顶点O不变，只沿着O到对边的垂线平移dist，dist>0向外放大dist，dist<0向内缩小dist,缩放后三角形为）OPt1Pt2;

	double distptol(AcGePoint3d pt, AcDbLine *Line);
	//点到直线的距离
	double distptol(AcGePoint3d O, AcGePoint3d P1,AcGePoint3d P2);
	//点O到两点P1P2构成直线的距离

	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor);
	static AcDbObjectId   maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor=2,int mode=1,ACHAR *layername=NULL,ACHAR* textStyle=L"宋体");
	static AcDbObjectId   makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	//	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR *layer=NULL);
	static AcDbObjectId   drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND);
	static AcDbObjectId   GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT,AcGePoint3d& pT1,AcGePoint3d& pT2);
	int  getPoint(ACHAR* pMessage, AcGePoint3d& pT);
	void  makeGroup(AcDbObjectId groupId);
	AcDbEntity*  selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
	static double  Get_INDATA_L(double INDATA[5],int *jj);
	static int xlzbxy(struct xlpoint *P0,struct xlpoint *PZ,double l,double INDATA[5],int display);
	static int  xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ);
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
	void printList(struct resbuf* pBuf);
	void iterate(AcDbObjectId plineId);
	AcDbObjectId createPl_xlpm(int trow,double arrayd[MAXJDNUM][6]);
	static ACHAR*  LCchr(ACHAR  *GH,double lc,int NLC,int mode);
	void calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt,double *tanl, double *R, double *xc,double *yc,double *ang,double *startfw,double *endfw,double *pj);
	void caljd(double x1,double y1,double x2,double y2,double x_1,double y_1,double x_2,double y_2,double *xjd,double *yjd);
	void calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd);
	double Get_Rshu(double design_V,double deltaI, struct GuiFanCanShu mGuiFanCS);
	//   double get_l0(double R);

	double get_l0(double SR,double Design_V,BOOL isJYGuiFanCS, GuiFanCanShu mGuiFanCS);
	double get_R(double SR,double Design_V,BOOL isJYGuiFanCS, GuiFanCanShu mGuiFanCS);

	static AcDbObjectId LoadTextStyle();

	static long factorialfun(int n);		
	static double TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] );
	double TYLC1(double XLC,int NDL,DLBZ DLBTMP[30]);
	bool TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] , double& TLC);
	double XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30]);
	///
	double GetSide(double Xo,double Yo,double x1,double y1,double x,double y);
	double ZXJ(double s2,double s1);

	double PROJ_ML(double array2[4*MAXJDNUM][10],int trow2,double xout , double yout , double xon , double yon);
	double XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int trow2);
	static	bool split_ckml(ACHAR str[], double *ml, ACHAR  *ck);
	static void split_ckml(ACHAR ckml[80],double &XLC,ACHAR GH[10]);

	static	int  F_num(ACHAR *str, ACHAR c);
	void putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place);
	void atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4]);
	void NoteBG(AcGePoint3d Opt,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,ACHAR *layername=NULL);
	void DrawBGTri(AcGePoint3d Opt,double sfactor,int iColor,ACHAR *layername=NULL);
	void dlayer(ACHAR lname[]);//删除层
	double get_l0(double Design_V,int KNDD,double SR, GuiFanCanShu mGuiFanCS);
	int crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point & pp);
	static void dmltockml(CString GH,double dml,ACHAR ckml[80]);
	void NullInStrto$(CString& text);
	void InStr$toNull();
	int ReadWorkdir();
	bool LoadArx(ACHAR ArxFname[256],ACHAR cmd[80]);
	double get_xjj(double Design_V, GuiFanCanShu mGuiFanCS);
	double get_XJJK(double Design_V,double SR,bool IsJYGuifan, GuiFanCanShu mGuiFanCS);//线间距加宽
	double Get_QXJK(CString TLDJ,double design_V,double SR, GuiFanCanShu mGuiFanCS);//规范曲线路基加宽
	static int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	int CalXYjd(double Spt[3],double Ept[3],double array2[4*MAXJDNUM][10],int trow2,int kk, double ResPt[2],double &ProjMl);
	int IfOnXY(double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);//是否与线元有交点
	double Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2]);
	int Search_XY(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3]);//定位pt1点所在线元
	static	double Circle_XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int kk);
	//输入一线里程，一线线元参数，二线线元数据，求线间距,二线投影连续里程及投影点
	int CalXjj(double ml1,double array1[4*MAXJDNUM][10],int tarray1,double array2[4*MAXJDNUM][10],int tarray2,double &xjj,double &projml,double resPt[2]);
	//输入一线上一点PZ,二线线元数据
	int Judge2XLorR(xlpoint PZ,double array2[4*MAXJDNUM][10],int tarray2);

	//	BOOL isJYGuiFanCS;
	//GuiFanCanShu mGuiFanCS;

	int FindWichDLFromFile(ACHAR GH[10],double XLC);


	static void SortArray( CArray<double,double> &a);
	static	void DrawLinePolyline(AcGePoint3dArray ptarr,int color,double wide);
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
	virtual void GetT0min_Lrmin(double DV,double &T0min,double &Lrmin, GuiFanCanShu mGuiFanCS);
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
	double GetRFromT1T2(double&L0, double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, BOOL isJYGuiFanCS , GuiFanCanShu& mGuiFanCS, bool IsStandard=false , double DV=350 ,int INTRadius =10 , int JUDGET012=0, BOOL isPreMove= FALSE );

	//plh04.14
	double GetRFromT_P_P(double x1,double y1,double x2,double y2,double &xs,double &ys,double &x0,double &y0,double &xe,double &ye,double &cenX,double &cenY,AcDbObjectIdArray &eraseId ,bool isJYGuiFanCS, GuiFanCanShu& mGuiFanCS, bool IsStandard=true, double DV=350 );
	//传入切线方向，返回半径,前一个切点，交点，后一个切点


	//两端为不完整缓和曲线的切长计算，输入转角，圆曲线半径，第一缓和段起始半径，第一缓和段长，第二缓和段终止半径，第二缓和段长，求切长
	void BAS_DRAW_FUN_JYX::get_T_Len(double zj,double R,double RB,double L1,double RE,double L2,double &T1,double &T2,double &L);
	void BAS_DRAW_FUN_JYX::Get_HHQX_T1T2(double A,double RB,double RE,double &TB,double &TE);
	double BAS_DRAW_FUN_JYX::Adjust_L_JDLuan(double aa,double R,double RB,double Lo1,double RE,double Lo2,double T,int qorh);

public:
	/*计算两点之间的距离Dis和两点构成的直线与x的夹角dAngleByX */
	double DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend);

	/*把计算出来的角度转换为0-2pi范围内的方位角*/
	void FormFwj(double& dFwj);

	/*给定两个两个方位角得出转向角*/
	double ZxAngle(double dLineAngle2,double dLineAngle1);	

	/*求点到直线距离*/
	double DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*从一点向直线作垂线求垂足*/
	void ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*检测给定的三个点的位置是否按顺时针顺序排列,顺时针返回一个正值否则为负值*/
	static double CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y);

	/*检查点(dNout,dEout)是否在给定的圆弧上*/
	bool CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout);

	int JudgePTLorR(double N,double E,double &dist,double array[4*MAXJDNUM][10],int tarray);
	static double CCW2(double pt1[2],double pt2[2],double pt3[2]) ;

	CString ReverseFind(CString InputCstring);

	/*两直线是否重叠*/
	bool IsZXCD(double SPt1X,double SPt1Y,double EPt1X,double EPt1Y,double SPt2X,double SPt2Y,double EPt2X,double EPt2Y);

	//写颜色文件
	static void WriteColorFile( CString SelFileName ,CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArray );

	//得到数组i之前满足条件的个数
	int GetBeforeNum(int i  , CArray<JDarrayStruct , JDarrayStruct >& JDNumArray);

	//设置默认规范参数
	int SetDefultGuifan(GuiFanCanShu &guifan);

	//横断面设计中要用到的函数，从横断面设计模块中取出合并至该类  李伟2007-08-08
	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId);

	//直线拟合
	void fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
		double& b, double& siga, double& sigb, double& chi2, double& q);
	double gammq(double a, double x);
	void gser(double& gamser, double& a, double& x, double& gln);
	void gcf(double& gammcf, double& a, double& x, double& gln);
	double gammln(double xx);

	void NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId);

	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor=0,double startWid=0,double EndWid=0,ACHAR LayerName[]=L"0");

	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,AcDbObjectId LayerId);
//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	AcDbObjectId  makesolid(AcGePoint3d Pt1,AcGePoint3d Pt2,AcGePoint3d Pt3,int icolor,ACHAR *LayerName);
	int AutoEntBK(ads_point p0, ads_point p1);
	int getrbpoint(ads_point pt,struct resbuf *ebuf,int code);
	struct resbuf *get_resbuf(int code,struct resbuf *p);
	struct arc_lst_JYX *findxyr(ads_point, ads_point, ads_real);
	struct point_lk_JYX *findroot(ads_point, ads_point, struct arc_lst_JYX *, ads_real bulge,
		ads_point, ads_point);
	struct ptt_node_JYX *get_brkpt(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *getbrkpt(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *get_plinept(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *getplinept(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *get_Arcpt(struct resbuf *,ads_point, ads_point);
	struct ptt_node_JYX *getArcpt(struct resbuf *,ads_point, ads_point);
	void atodmss1(double alpha, int &d, ACHAR mm[4], ACHAR ss[12],int Pres);



	//读颜色文件
	static BOOL ReadColorFile( CString SelFileName , CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut );

	//设置默认颜色参数
	static void SetDefaultColor(CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut );

	static double DmlToCml(double dml,DLBZ DLBArray[MAXDLNUM],int DlSum);

	AcDbObjectId GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz,int color,double Lwid,ACHAR *LayerName);

	AcDbObjectId drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND,int color,double Lwid,ACHAR *LayerName);

	static AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	static AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	static AcDbObjectId makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx,int color,int fh=1,ACHAR *LayerName=NULL);
	static void GetAllRXD(CString MDBName,CString XLName,CArray<CString,CString>&AllRXDName);

	static bool GetArea(const AcDbObjectIdArray& curveIds1,const AcDbObjectIdArray& curveIds2,double & ReArea);

	AcDbObjectId FromePLineBySEMl(double Scml, double Ecml, double (*XYArray)[10], int XYNum);

	CString GetProByMdbName(CString mdbPath);


//	static int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ , GTZX *ZuoXianpGtzxToIn,GTZX * JiXianpGtzxToIn,GTZX * YouXianpGtzxToIn , CArray<GJDInf,GJDInf>& GJDInfTabArrayToIn) ;

	//virtual int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2, DLBZ DLBtoGT ,int NDLtoGT , CArray<JDarrayStruct,JDarrayStruct>& JDarraytoGT ,
	//    struct xlpoint *PZ , GTZX *ZuoXianpGtzxToIn ,
	//	GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn , GTZX * GTXGtzxToIn ,
	//	CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn );
	//virtual int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ ,  GTZX *ZuoXianpGtzxToIn ,
	//	GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn ,CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn) ;
} ;



#endif // !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
