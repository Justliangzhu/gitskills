// DrawPM.h: interface for the CDrawPM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_)
#define AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XLDataBase.h"
#include "JD_CENTER.h"
#include "HdmDes.h"
#include "Struct.h"

static CString	BM="水准点";
static CString	DaoXian="导线";

class CDrawPM  : public BAS_DRAW_FUN
{
public:
	//********************************结构体********************************
	struct CRSDAT
	{
		ACHAR ckml[80];
		double cml;
		double dml;
		int DorS;
		double DuanLuoInf;
		double xjj;//线间距，单线为0,二线在左为-，右为+
		double ZDesH, YDesH;
		double LZDK, RZDK;
		//DesH路肩的设计高程和EarH地面高程（HDM的绘图数据）//指左线
		double RHFL[10], RHFR[10];  //主线路面点4个
		double zlmhd[3], ylmhd[3];
		int ZDMXZ, YDMXZ;//左右幅断面性质
		double  Zt, Yt;
		double ZHc, YHc, Y_ZHc, Y_YHc;//填挖高
		int zptsum, rptsum, Lnpt, Rnpt;
		AcGePoint3dArray RPTL, RPTR;
		AcGePoint3dArray PTRPTL, PTRPTR;
		int Ljdnum, Rjdnum;
		//指右线
		double FLDcml;//相关道路投影里程
		CString FLDname;//相关道路名
		double Y_RHFL[10], Y_RHFR[10];  //分离段路面点4个
		double Y_zlmhd[3], Y_ylmhd[3];
		int Y_ZDMXZ, Y_YDMXZ;//左右幅断面性质
		double  Y_Zt, Y_Yt;
		int Y_Lnpt, Y_Rnpt;
		AcGePoint3dArray Y_RPTL, Y_RPTR;
		AcGePoint3dArray Y_PTRPTL, Y_PTRPTR;
		int Y_Ljdnum, Y_Rjdnum;

		double Ldeltah, Rdeltah;//左右路肩点与地面线的高层差
		double Y_Ldeltah, Y_Rdeltah;//左右路肩点与地面线的高层差
		double newLYd, newRYd;//左路基中心的地面高层，右路基中心的地面高层
		double  at, aw;
		double Ldh, Rdh;
		double DX;
		//		//左线或一线
		int LGouPtNum, RGouPtNum, LJSGouPtNum, RJSGouPtNum;////////wwq5.14
		int LGouProperty, RGouProperty;
		double Lgou[36], Rgou[36], LJSgou[36], RJSgou[36];//水沟7点/////wwq5.14
		AcGePoint3dArray PTLgou, PTRgou, PTLJSgou, PTRJSgou;
		//	
		//		//右线
		int Y_LGouPtNum, Y_RGouPtNum, Y_LJSGouPtNum, Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty, Y_RGouProperty;
		double Y_Lgou[36], Y_Rgou[36], Y_LJSgou[36], Y_RJSgou[36];//水沟7点/////wwq5.14
		AcGePoint3dArray Y_PTLgou, Y_PTRgou, Y_PTLJSgou, Y_PTRJSgou;
		double CenPt[2];//线路中心点坐标
		double fwj;

		int IfHasZlink, IfHasYlink;
		CRSDAT* zlink, * ylink;
		double ZDorS, YDorS;//1-单路面，2-双路面同坡 3-双路面反坡
		double ZCG_GDR, YCG_GDR;
		int IsInsertHdm;
		double OffsetByXLZX;
		//左线或一线		
	};

	struct Hole
	{
		double qInpt[2];
		double Inpt[2];
		int nLpt, nRpt;
		AcGePoint3dArray LptArr;
		AcGePoint3dArray RptArr;
		AcDbPolyline* pline;
	} Ahole;

	struct BriBJX
	{
		double qpt[3];
		double hpt[3];
	} Abjx;

	struct ZDKDAT
	{
		double cml;
		double LZDK;
		double RZDK;
		int IsInsertHdm;
		//		double OutPt[2];//占地界外点用于删除实体
	};

	//********************************成员变量********************************
	//****************图形绘制参数****************
	double DFACTOR;		//绘图比例
	int XSW; //小数位数	0 1 2 3
	double LcSpacer;//里程标注间距 100，500，1000；
	double BGarowcml;
	double ZdkZlen, ZdkYlen;		//占地宽左宽	占地宽右宽
	int ZdkMode;	//占地宽标注模式
	bool BzRAL;//标注R\A\L
	bool drawfldzdk;//true 绘制分离段棋形部位占地  false 不绘制
	double Sdml, Edml;//绘图起终现场里程
	double Scml, Ecml;//绘图起终连续里程
	double ZxWid;	//中线宽度
	double YdxWid;	//用地线宽度
	double texth;	//字高
	int m_ZDKLineColor;	//占地宽线颜色
	bool m_IsGutter;	//是否是改沟模式

	//****************计算参数****************
	XLDataBase DBS;		//数据库读写
	JD_CENTER *pm;		//平面指针
	CRSDAT* hdmdes;
	HdmDes hdm;
	CArray<Hole, Hole>HoleArray;
	int HoleNum;
	int NCRS;
	int Count;
	CArray<BriBJX, BriBJX>BriBjArr;
	int NBriBj;
	ZDKDAT* pZDK;
	int NZDK;
	bool ifsgjk;
	double SmlOnZx, EmlOnZx, SPXJJ;
	//起终路肩分离点在主线上的投影里程
	//	double LJFLSmlOnZx,LJFLEmlOnZx,LJFLSmlOnFLD,LJFLEmlOnFLD;
	//起终边坡分离点在主线上的投影里程
	double BPFLSmlOnZx, BPFLEmlOnZx, BPFLSmlOnFLD, BPFLEmlOnFLD;
	int NZLMK, NYLMK;
	LMKdata* ZLMK, * YLMK;
	int BxNum;
	BXLMKD* pBXArr;
	GTZDM_ROAD* zdm, * Zzdm, * Yzdm;
	GLROAD ZGLR, YGLR;
	DrawHorizontalPara m_DrawPara;	//绘图参数	20191231添加
	
	//********************************成员函数********************************
	//****************构造、析构函数****************
	CDrawPM();
	CDrawPM(JD_CENTER* pz, double sdml, double edml);
	virtual ~CDrawPM();
	//****************计算函数****************
	void ReadHdmResultFile();	//读取横断面结果文件
	void ReadACrossResultData(FILE* fpr, int jcross);	//读取单个断面结果文件
	void ReadLMK(CString LMKTABname);		//读取路面宽信息（从文件中读取）
	int FindIlmk(double cml, LMKdata lmk[], int nlmk);		//查找路面宽信息
	double GetLMK(double cml, LMKdata lmk[], int nlmk);		//获取路面宽（计算）
	void CalZdkFld(int& FldSum, double SEml[50][3]);		//计算（读取？）占地宽分离段
	void CalOffsetByXLZX(double cml, int NLMK, LMKdata lmk[], double& OffsetByXLZX);	//计算路基中线相对于线路中线的偏移量
	void formZPHole(int i, int ZorY, int qdmxz, int hdmxz, int qoppxz, int hoppxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr);		//生成洞文件？
	void CalTWZeroPt(double bml, double eml, double Bdeltah, double Edeltah, double& k, double& MidMl);	//计算填挖零点
	int CalBjPt(double cml, double ang, int mark, ads_point pt1, ads_point pt2, double& x, double& y);	//计算边界点
	void CheckZdkInf(int FldSum, double SEml[50][3], ZDKDAT& ZDK);	//检测占地宽信息
	void FormHole();
	void CalBriSlopPt(int i, int ZorY, AcGePoint3dArray& ptarr);
	int Cal_ZPJD(int ZorY, int HorT, AcGePoint3d Cenpt, AcGePoint3d Spt, AcGePoint3d Ept, AcGePoint3dArray& ptarr);//计算锥坡脚点
	void formZorYHole(int i, int ZorY, double qdeltah, double hdeltah, double qtw, double htw, int qdmxz, int hdmxz, int qoppxz, int hoppxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, double Ht);
	void CalYLMHt(double N, double E, double& prjml, double& Ht, double& lmhp);
	void CalTLMHt(double NY, double EY, double NT, double ET, double& prjml, double& Ht, double& Tljhp);
	double FindBriAng(double cml);
	void CalLmht(double N, double E, double& prjml, double& ZJ, double& Desh, double& lmhp, double& Lmbg);
	void CalLMKbyFile(double cml, int ZorY, double& half_fgdk, double& lydk, double& cxdk, double& yljk, double& tljk);
	int CalLMBG(double ZJ, double RHF[10], double& Lmbg, double& lmhp);
	int CalLMBGOnNose(double N, double E, HdmDes::RoadHdmdata& ZX, HdmDes::RoadHdmdata& ZD, double& Lmbg, double& lmhp);
	int CalLMBGOnNose(double N, double E, HdmDes::RoadHdmdata& ZX, HdmDes::RoadHdmdata& ZD, double& Lmbg, double& lmhp, double& ZJ, double& Desh);
	void CalLMKNoJK(double cml, int NLMK, LMKdata lmk[], double& half_fgdk,
		double& lydk, double& cxdk, double& yljk, double& tljk,
		double& lmhp, double& yljhp, double& tljhp,
		int& bkmode, int& sjbgmode);

	void line0(double x1, double y1, double x2, double y2, int color, ACHAR* layer);	//绘制线条

	//****************绘线路函数****************
	void Draw_PI_Infor();	//绘制交点信息	20190706添加	
	void Draw_Center_Line();//绘制线路中心线	20190707添加	
	void DrawJDLine();		//绘制交点连线
	void BZLC(int inf);		//标注里程	0线位图 1 平面图
	void BZTZD(int inf);	//标注特征桩	0线位图 1 平面图
	void DrawDZQ();			//绘制大中桥
	void DrawTun();			//绘制隧道
	void DrawDX();			//绘制导线
	void DrawBM();			//绘制水准点
	int DrawCulvet();		//绘制小桥涵
	void DrawXLZX();		//绘制线路中线（暂时未用）
	void DrawSeML();		//标注起终点
	void DrawLsxLine(ads_point bpt, double cita, double lsxang);	//绘制小桥涵？
	void INSERT_DX(double XX, double YY, CString BianHao, double LEV);		//插入导线
	void INSERT_BM(double XX, double YY, CString BianHao, double LEV);		//插入水准点
	void DrawBGDATA();//绘制标高数据图
	//****************绘用地图****************
	void DrawZDK();		//绘制占地宽
	void BZ_ZDK();		//标注占地宽
	//****************绘平面总体图****************
	void DrawPartFillBetweenTwoMl(double sml, double eml, int ZorY, int qPtNum, int hPtNum,
		AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& flag);		//画填方
	void DrawCutPartBetweenTwoMl(double sml, double eml, int ZorY, int qPtNum, int hPtNum,
		AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& flag);		//画挖方
	int draw_fill_cut();	//绘制填挖
	void DrawZTT();			//绘制整体图
	void drawGouArrow(ads_point pt, double angle, int inf);	//绘制箭头
	void DRAW_GOU(int qPtNum, int hPtNum, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, int ZorY);
	void DRAW_GOU(AcGePoint3d qpt, AcGePoint3d hpt, double qfwj, double hfwj, int qPtNum, int hPtNum, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, int ZorY);
	void DRAW_ALLGOU();	//绘制水沟？
	int drawfillandcut(int i, int ZorY, double qdeltah, double hdeltah, double qtw, double htw, int qdmxz, int hdmxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& tflag, bool& wflag);	//绘制填挖
	void DrawAllSlop(int i);	//绘制所有坡度
	void Draw_BridgeSlop(ads_point bspt, ads_point rpt, ads_point spt);	//绘制桥梁坡度？
	void  DRAW_TW_GOU(CRSDAT Qhdm, CRSDAT Hhdm);
};

#endif // !defined(AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_)





















