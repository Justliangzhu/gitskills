// JD_CENTER.h: interface for the JD_CENTER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JD_CENTER_H__B8F695BF_C625_4CE9_8E46_2167E1BFEEFC__INCLUDED_)
#define AFX_JD_CENTER_H__B8F695BF_C625_4CE9_8E46_2167E1BFEEFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.H"
#include "ZYLink.h"
#include "YYSLink.h"
#include "YYCLink.h"
#include "YYLuanLink.h"
#include "YYQXLink.h"
#include "JDLuanLink.h"
#include "FourJDLocationC_S.h"
#include "Norm3JD.h"
#include "ThreeJDCalA1A2FromT1T2R.h"
#include "ThreeJDCalThisAFromThatA_PreT_R.h"
#include "GTZX.h"
#include "GTZX2.h"
//////////////////////////////////////////////////////////////////////////
//2004.4.25 hhp

struct RouteItem
{
	double v0;//0：线元性质 1-直线，2-圆，3-前缓，4-后缓, 5-不完整缓R大->R小, 6-不完整缓R小->R大
	//(R1>R2)//////////////////////////////////////////////////////////////////////
	double v1;//1:			L		R    A         A              A                A        //
	double v2;//2:					L	 R         R              R1               R2       //
	double v3;//3:                                                  R2               R1       //
	//////////////////////////////////////////////////////////////////////////////////
	double v4;//4: 左右转
    double v5;//5: 起始切线方位角
    double v6;//6: 线元起点里程（=统一里程=线路起点的里程+参照点到线路起点的曲线距离）
	double v7;//7: 半径 R
	double v8;
	double v9;//8，9：线元起点坐标 N,E
	//最后一个线元为终点
	//0:0
};
//////////////////////////////////////////////////////////////////////////

#define AppName  /*MSG1*/L"ROAD" 
//基线自定义实体
class JD_CENTER : public BAS_DRAW_FUN , public AcDbEntity  
{
public:
	//****************************************成员变量*****************************************

	//***********************1.基本属性***********************
	ACHAR mdbname[256];//方案数据库名
	ACHAR RoadName[256];//道路名，主线，右分离段,左分离段等

	//***********************2.核心数据***********************
	double QXB[MAXJDNUM][9];	//曲线表
	Adesk::Int16 XYNum;			//线元数
	double(*XYArray)[10];		//线元表
	Adesk::Int16 JdCenNum;		//交点数
	CArray<JdCenter, JdCenter>JdCenArray;	//交点表	交点圆心数组,首尾为起终点
	Adesk::Int16 DLNum;			//断链数
	CArray<DLBZ, DLBZ>DLArray;	//断链表	起点为特殊断链，终点无断链，全线至少一个断链（起点）	里程冠号至多为8个字符
	ACHAR LCGH[20], JDGH[20];	//里程GH
	int *XYJDNO;				//对应的交点/圆心编号,夹直线对应后面交点/圆心
	double XLLength;			//线路终点统一里程
	double LenOfLine;			//线路长度

	//***********************3.属性数据***********************
	Adesk::Int16 FLDNo;	//分离段编号：1线为0，分离段>＝1
	Adesk::Int16 JD0;	//起始交点号
	Adesk::Int16 PreJD;
	double Ls1;
	//***********************4.规范数据***********************
	int DV;			//设计车速
	CString Grade;	//等级
	double ZDMJG;	//纵断面间距

	//***********************5.超高加宽数据***********************
	int NZLMK, NYLMK;		//左路面宽数量	右路面宽数量
	LMKdata *ZLMK, *YLMK;	//左路面宽数据	右路面宽数据
	int BxNum;				//边线数量
	BXLMKD *pBXArr;			//边线数据数组
	int ZjkNm, YjkNm;		//超高加宽
	CgSZ *Zjk, *Yjk;		//左加宽	右加宽
	CgSZ *Zcg, *Ycg;		//左超高	右超高
	int ZcgNm, YcgNm;		//左超高数量	右超高数量

	//***********************5.绘图数据***********************
	bool BzRAL;		//标注R,A,L
	double SCALE, texth, DLBlength;	//比例	字高	断链表长度
	bool SHOWJD;	//是否绘制交点号
	Adesk::Int16 NLC, DrawLeftOrRight;//小数位数	绘制在左右侧

	//***********************6.铁路相关数据***********************
	ConstraintErr m_InterSectData;	//与铁路交叉数据
	CString m_RelRailName;	//相关铁路的线路名
	bool m_IsJYX;	//20191031	是否是既有线
	double RtjMin;

	//***********************7.其他数据***********************
	CArray<LCBdata, LCBdata>pLCB;
	int NLCB;//桩数
	ZQZJTAB *ZQZJB;//直曲转角表
	int m_NCM;	//CM数组总数
	double m_CM[5 * MAXBPDNUM];	//纵断面	CM数组
	int RINum;
	CArray<RouteItem, RouteItem>RIAarray;
	double(*InterArray)[10];
	struct Ptstr
	{
		double pt[2];
	};
	bool IfCalXY;
	bool IfDelJZX;
	//***********************99.废弃数据***********************
	//	int Chinese_Engbz_type;

	//线元数组说明
	//////////////////////////////////////////////////////////////////////////////////
	//第0个线元表示线路起点                                                         //
	//0: NULL                                                                       //
	//1，2: N，E                                                                    //
	//3: 线路起点方位角a                                                            //
	//4: 线路起点里程                                                               //
	//////////////////////////////////////////////////////////////////////////////////

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
	//0:0
	
	//****************************************成员函数*****************************************
	//***********************1.构造、析构函数***********************
	JD_CENTER(JD_CENTER *pJC);
	JD_CENTER();
	JD_CENTER(double xy[][10], int xyn);
	virtual ~JD_CENTER();
	//***********************2.CAD相关函数***********************
	ACRX_DECLARE_MEMBERS(JD_CENTER);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;

	virtual void subList() const;
	virtual Acad::ErrorStatus explode(AcDbVoidPtrArray& entitySet);
	//  virtual AcRxObject* clone() const;

	virtual Acad::ErrorStatus getOffsetCurves(double             offsetDist,
		AcDbVoidPtrArray&           offsetCurves)  const;

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
		AcDbIntArray&     geomIds) const;

	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);
	Acad::ErrorStatus subGetTransformedCopy(const AcGeMatrix3d& mat, AcDbEntity*& ent) const;

	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* pWd);
	//***********************3.通用函数***********************
	//设置交点冠号
	void SetJDGH(CString str);
	//设置里程冠号
	void SetLCGH(CString str);
	//设置是绘制交点连线
	void SetShowJD(BOOL IsShow);
	//设置道路名
	void setRoadName(CString rname);
	//设置数据库名
	void setMdbName(CString MdBname);
	//获取指定点处线路切线边的两端点/交点坐标
	void GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);
	//获取指定点前面的交点坐标
	int GetPreJD(double N, double E);
	//获取指定里程处的线元编号
	int GetXYNoFromTYLC(double tylc);
	//获取交点数据
	int getJD_RL(int JDi, double *Ri, double *L1i, double *L2i, double *NN, double *EE);
	//计算转向
	int FindWichZorY(double cml);
	//计算直线与道路的交点
	void CalJD_ZX_XL(double Spt[2], double fwj, double PtOnXL[2], double &cml);

	//***********************4.几何相关函数***********************
	//计算所有的交点
	void CalJiaoDian();
	void CalJiaoDian_New();
	//计算线元表
	void CalXYArray();
	void CalXYArray_OnlyJD();
	//初步计算所有交点的切长
	void Cal_CurJD_QXSE(int JDno, double& sprojml, double& eprojml);
	//交点转圆心
	void TranCenter();
	//计算线元交点
	void CalXYJD(double jd[2], double Spt[2], double SFwj, double Ept[2], double EFwj);
	//计算回头曲线
	int CalIfHuiTOU(double sfwj, double efwj, double zx);
	//线元转交点
	void XYArrayToJD();
	//设置线元
	void SetXYArray(double xyarr[3][10]);
	//增加线元
	void AddXYArray(double xyarr[3][10]);
	//解算鼻端圆心坐标
	void CalNoseCen(JD_CENTER *pm2, double sml1, int QorH, double C1, double C2, int YTinf,
		double Rnose,
		double &CmlonZX, double &CmlOnYX, double NosePt[2]);
	// 按W3,W4解算鼻端圆心坐标
	void CalNoseCen(JD_CENTER *pm2, double sml1, int QorH, double W3, double W4,
		double &CmlonZX, double &CmlOnYX, double NosePt[2]); 
	//?
	void UpdateDataRIArray();
	//调整缓长？
	int Adjust_L_JDLuan(double aa, double R, double RB, double Lo1, double RE, double Lo2, double T, int qorh);
	//计算所有交点切线？
	void Cal_AllJD_T();
	//替换线元？
	void ReplaceItem(double xyarr[3][10]);

	//***********************5.里程（断链）相关函数***********************
	//获取指定里程处后面相邻断链的编号
	int GetAfterDL(double tylc);
	//在指定位置增加一个断链
	void addDLM(int iDL, double BLC, double DL, CString EGH);
	//删除指定断链
	void DelDLM(int iDL);
	//传入断链表
	void setDLB(DLBZ *dlb, int ndl);
	//计算断链处的统一里程
	void GetDLTylc();
	//查找与指定里程最近的断链
	double DistToNearDL(double tylc, bool IsForwardTo = true);
	//查找与指定里程最近的整百米桩
	double LocateHundredMeterOnLine(double tylc1x, bool IsForwardTo = true);
	//修改指定断链
	void setDLM(int iDL, double BLC, double EDL, CString BGH, CString EGH);
	//void setDLM(int iDL,double BLC,double ELC,char *BGH,char *EGH);

	//***********************6.里程、几何转换函数***********************
	//统一里程转现场里程
	CString GetXCLCStr(double tylc);
	//由坐标投影统一里程
	double PROJ_ML(double n, double e);
	//求现场里程
	CString XLC(double cml);
	//求现场里程
	double XLC(double TYLC, CString& GH);
	double XLC1(double TYLC);
	//里程转坐标及线路切线方位角
	int xlpoint_pz(struct xlpoint *PZ);
	//现场里程转统一里程
	//	double TYLC(CString ckml);
	double TYLC(double dml);
	double TYLC(CString ckml);
	//封装【坐标】、【现场里程】、【投影里程】间的相互转换函数		20190715	ljx
	//①【坐标】与【投影里程】相互转换
	void Trs_ProjMl_To_NE(double ProjMl, double& N, double& E);
	double Trs_NE_To_ProjMl(double N, double E);
	//②【投影里程】与【现场里程】相互转换
	double Trs_Ckml_To_ProjMl(CString Ckml);
	CString Trs_ProjMl_To_Ckml(double ProjMl);
	void Trs_ProjMl_To_Ckml(double ProjMl, ACHAR Ckml[]);
	//③【坐标】与【现场里程】相互转换
	void Trs_Ckml_To_NE(CString Ckml, double& N, double& E);
	CString Trs_NE_To_Ckml(double N, double E);
	void Trs_NE_To_Ckml(double N, double E, ACHAR Ckml[]);
	void Trs_CkmlPt_To_NE(CString Ckml, double Dist,double& N, double& E);

	//***********************7.超高、加宽相关函数***********************
	//计算超高
	void CalCG(double projml, double& CG);
	//读取路面宽函数
	void ReadLMK(CString LMKTABname);
	//采用文件计算路面宽？
	void CalLMKbyFile(double cml, int ZorY, double &half_fgdk, double &lydk, double &cxdk, double &yljk, double &tljk);
	//读取加宽
	void ReadJK();
	//读取超高
	void ReadCG();

	//***********************8.绘图函数***********************
	//标注特征点
	void INSertLC(double cml);
	void BZTZD(AcGiWorldDraw* pWd, int bz_zybz, int textdir = 1);//-1，倒字头  //  标注里程
	//标注里程
	void BZLC(AcGiWorldDraw* pWd, double dlc, int zybz, int type, int textdir = 1);//字头朝向为-1时倒向  
	void BZTZD(double Scml, double Ecml, int inf, int textdir, double texth, int xsw);//-1，倒字头  //  标注里程
	//标注里程
	void DrawSEML(int zybz, int textdir, double texth, int xsw);
	void BZLC(double Scml, double Ecml, double dlc, int zybz, int type, int textdir, double texth, int xsw, double factor, int inf);//字头朝向为-1时倒向
	//构建图框结构数组,计算四个顶点坐标
	void TransFormTK(STuKuang *TK, int TkNum, double CenX, double CenY, double scale = 1.0);
	//构建图框结构数组，计算该图框范围内线路的起终里程
	void CalTK(double StartLc, double EndLc, double TKLen, double Frame_Height, double Frame_Width, STuKuang *TK, int &Num, double MoveCenterX = 0.0, double MoveCenterY = 0.0);
	//设置图框	20190909
	void SetTK(STuKuang& Frame, double Frame_Height, double Frame_Width, double CenterX, double CenterY);
	//绘线位、标注
	void Draw(AcGiWorldDraw* pWd = NULL);
	//绘宽度？
	void DrawKD(double sml, double sw, double eml, double ew, int mode, int icolor = 1, double interval = 2.0);
	//绘线元？
	void ShowXY(AcGiWorldDraw* pWd);
	//绘制起终点、交点信息函数	20190706	为适配标准改路模板	ljx
	void Draw_PI_Infor(double Scml, double Ecml, double texth, int Decimals, bool IsNoteRAL, bool IsGutter = false);
	//获取特征桩字符串
	void GetQxTZDStr(int i, ACHAR chr[]);
	//绘相交？
	void DrawInterSect(AcGiWorldDraw* pWd);
	//绘制自定义实体文字
	Adesk::Boolean  G_maketextNew(AcGiWorldDraw* pWd, AcGePoint3d& Pt, ACHAR* ctext, double ang, double texth, int icolor, int mode = 1);

	//***********************9.铁路相关函数***********************
	//计算公路与铁路的交点
	bool Caljd();
	
	//***********************10.里程表计算函数***********************
	//生成里程表
	void FormLCB(double dlc, double sml, double eml);
	//将曲线特征点加入里程表 
	void  AddTZDtoLCB(); 
	//将桥隧加入里程表 
	void  AddBritoLCB(int NBTS, double BTS[][3]); 
	//将桥隧加入里程表 
	void  AddTuntoLCB(int NBTS, double BTS[][3]); 
	//***********************11.高程计算函数***********************
	//变坡点数组转CM 数组
	void TrsBPDArraytoCM(int NBPD, double BPD_array[][4]);
	// 计算cml处的高程
	double rdes_level(double cml, double &slope);
	// 由CM数组计算cml处的高程
	double DLEVEL(int icm, double cml);
	
	//***********************12.其他函数***********************
	//生成直曲转角表
	void CreateZQZJB();
	//数模内插计算地面线
	int PM_DMX(double *dmx, double ZJ, double sml, double eml);
	//克隆一个设计线实体
	bool CloneJC(JD_CENTER *pJC);
	//线路倒置
	void Reverse();
	//打印控制点及线元信息，用于调程序
	void OutData();
	//圆心转成交点交保存到指定文件中
	void TranJD(CString fname = "");
	//合并两条线路，(n1,e1)指源线路上指定点，(n2,e2)线路二上指定点，
	void AppendXL(JD_CENTER *OtherXL, double n1, double e1, double n2, double e2);
	//过圆外一点做切线
	double GetFwjByJd_Center_A_LR(double jdn, double jde, double cn, double ce, double R, double A, int lr);
	//传入交点圆心数据
	void SetJdCenterArray(JdCenter *JCAy, int num);
	//?
	void CalPZXPT(double cml, double jd, double dis, double pt[2], double &prjml);
	int GetInOrOut(int ZDStOrEd, JD_CENTER *pm2);
	bool CalPXXJD(double cml, double angle, double dis, double pt[2], double &prjml, double &newWD);
};
int MAKE_ACDBOPENOBJECT_FUNCTION(JD_CENTER);
#endif // !defined(AFX_JD_CENTER_H__B8F695BF_C625_4CE9_8E46_2167E1BFEEFC__INCLUDED_)
