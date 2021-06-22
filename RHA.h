#pragma once

#ifdef PINGMIAN_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else

#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "malloc.h"
#include "time.h"
#include "BAS_CALC_FUN.H"
#include "BAS_DRAW_FUN.h"
#include "Struct.h"
#include <vector>
#include "PlaneDraftPrefer.h"
//#include "GetAllVertData.h"

using namespace std;

//交点属性
#define BXPX        0   //并行平行(交点不可拖动)
#define BX_NotPX    1   //并行不平行
#define RX_Start    2   //绕行始
#define RX_One      3   //绕行中
#define RX_End      4   //绕行止
#define S_Start     5   //S弯始
#define S_End       6   //S弯止 

//#define	BLJD        10  //基线交点
//单点绕行用一个绕行始表示

//线路名（包含线路属性）
#define JYJX        L"既有基线"
#define JY2X        L"既有二线"
#define JY3X        L"既有三线"
#define JY4X        L"既有四线"
#define SJJX        L"设计基线"
#define SJ2X        L"设计二线"
#define SJ3X        L"设计三线"        
#define SJ4X        L"设计四线"
#define LSX         L"临时线"
#define SGBX        L"施工便线"
//以上为线路名，同时线路还有四种属性：既有基线 既有二线 设计基线 设计二线，也用JYJX等表示

#define DefaultMdbNm L"C:\\未命名.MDB" //临时线默认数据库名

#define LEFTLINE -1
#define RIGHTLINE 1
#define MAINLINE 0

#define TUNMIDLINE 0
#define CONTINUELINE 1
#define RAILLEGEND	2

#define InOneRXD    1
#define ConTwoRXD   2
#define QHBX_AddBX  3
#define QHBX_AddRX  4
#define QBHR        5
#define QRHB        6

//-----------------------------------------------------------------------------

class DLLIMPEXP RHA : public BAS_CALC_FUN ,public BAS_DRAW_FUN ,public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(RHA) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:	
	int m_iDoc;//文档窗口下标,只对全局变量用，对局部线路（施工便线和临时线）没用 818 该变量是不是已经记录在全局数组中，不需要再RHA记录;尽量减少该变量去获取全局数组
	CString m_XLName;//线路名:既有基线、既有二线、既有三线……设计基线、设计二线……
	CString XLShuXing;   //1-既有基线 2-既有二线（如：既有三、四线）3-设计基线 4-设计二线 
	//5-临时线 6-施工便线
	//Old   线路性质：基线，左线 右线
	CString m_mdbName;//自定义实体对应的mdb

	double xl_length;//线路长度

	vector<JDarrayStruct> JDarray;//交点数据

	//曲线表
	//CArray<QXBStruct,QXBStruct>QXB;
	vector<QXBStruct> QXB;
	CArray<OneJZXStruct , OneJZXStruct> lsJZXarray;//夹直线数组(既有线和设计线) 818 以后改名
	CArray<JZXStruct , JZXStruct> JZXArray;//夹直线数组//818 将其修改为lsJZXarray



	// QXLRL[100][3];

	//double array[4*MAXJDName][10];//0：线元性质1-直线，2-圆，3-前缓， 4-后缓

	//DoubleArray3d array_pxy;//array_pxy[NJD][4][5] 临时线元数组//改成一维，方便释放
	vector<vector<double>> array;//array[4*NJD][10] 线元数组
	Int16  tarray;//线元个数

	vector<double> array_pxy;//array_pxy[NJD][4][5] 临时线元数组//改成一维，方便释放
	//int tarray_pxy;//array_pxy的数目

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
	Int16 JD0; //起始交点号 818 没必要
	ACHAR JDGH[20];   //交点冠号
	BOOL isGJD;//818 是否改建 没必要？



	vector<DLTab> DLarray;//断链数组

	vector<BZLCStructTable>BZLCarray;
	
	//记录改建段标注的点数组
	CArray<AcGePoint3d , AcGePoint3d > GJDNotePtAry;
	//实体里改建段信息数组
	vector<GJDInf> GJDInforArray;//既有线存储的是利用段数组，设计线存储的是全线分段数组

	//vector<RXDInf> ZMRxdAry;//纵面绕行段表 既有二线和设计二线纵面绕行段数组。 注意：该数组只在纵面设计时用 //818检查既有二线是否储存了点坐标，应该存
	

	vector<FZZDM> FZZDMAry;
	vector<FZZDM> ZMRXDArray;//横断面设计用

	//前一个交点数组、断链数组、标志里程数组
	vector<JDarrayStruct> OldJDarray;
	vector<DLTab> OldDLarray;
	vector<BZLCStructTable> OldBZLCarray;

	vector<Switch> m_SwitchArray;//道岔
	//vector<QXBCmlStruct> QXBCmlAry;//平面曲线表特征点连续里程数组 



	//CArray<OneJZXStruct , OneJZXStruct> JZXArray;//夹直线数组,用于解算绕行、并行段

	BOOL IsReSetColor;//是否重新设置颜色
	Int16 AllColor; //所有颜色
	Int16 GTZXJDToJDColor;  //平面交点连线颜色
	Int16 XWArcColor; //线位圆弧
	Int16 HuanHequxianColor; //
	Int16 JiaZhiXianColorZuo,JiaZhiXianColorYou;//左夹直线颜色，右夹直线颜色 
	Int16 mdbnameBoxColor,mdbnameTextColor;//线路名矩形框颜色，线路名颜色
	Int16 QXYSColor;//曲线要素颜色

	PlaneDraftPrefer DrawParam;//绘图参数 注意：在绘制自定义实体时用到两参数 DrawParam.m_dLC、DrawParam.NLC 线宽 标注字的高度
								                                   //818 增加平面自定义实体表？
	
	/**/

	/*-------------------------规范--------------------------*/
	double DV;//速度目标值  818 什么时候赋值
	//double T0min;//最小夹直线长
	//double Lrmin;//最小圆曲线长 818 暂时放bas_calc_fun

	vector<double> m_Rs;//曲线半径系列  R[20]
	vector<double> m_l0s;//缓和曲线长度配置，与曲线半径一一对应：R[i]配L[i] L[20]

	double RtjMin, RtjMax;//推荐半径最小值、最大值
	//BOOL isJYGuiFanCS;

	double ZXLmin1,ZXLmin2;//一般最小夹直线长，困难最小夹直线长
	double Rmin1,Rmin2;//一般最小曲线半径，困难
	CString TLDJ,ZXSM;//铁路等级
	double XJJ;//标准线检举,0单线,>0二线在右側，<0二线在左側
	double ZDMJG;//纵断面间距

	


	//连接方案时用
	CString BfLinkXLName, AftLinkXLName;//前后连接线路名
	Int16 LinkXLBfJD, LinkXLAftJD; //连接线路是前接的前一个交点下标，后接的后一个交点下标
	
	/*---------------------拖动交点---------------------*/
	int MoveJDIndx;//拖动交点的下标（拖动该曲线交点或圆曲线终点）
	bool IsMove;//是否夹点拖动
	bool IsMoveDone;//是否夹点拖动结束


	/*---------------------绘图变量---------------------*/

	
	BOOL m_IsNoteTZD;//是否标注特征点
	BOOL m_IsNoteML;//是否标注里程
	BOOL m_isNoteNYDL;//是否标注内业断链
	Int16 mode_display;//是否显示线路，=0 显示

	//double LW;//线宽
	AcDb::LineWeight  LineWeightValue ;//线宽 
	double texth;
	Int16 draw_zybz;	//draw_zybz=-1 曲线四大桩和曲线参数标在左侧，=1为标在右侧  =0为按曲线偏向标  818是否有必要分别左右侧
	Int16 qxbbz;//qxbbz == 1, 标注曲线要素；!= 1 不标注

	double BZLC_dlc;//100百米标，1000公里标，500标
	//int NGC/*,NLC*/;//NH--高程保留位数 NLC--里程保留位数

	Int16 Chinese_Engbz_type;//818 无用？
	Int16 NGC;//818

	//变坡点数组
	vector<PDTAB> m_BPDArray;
	//CM数组
	vector<double> m_CM;




public:

	RHA () ;
	virtual ~RHA () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;
	Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	Acad::ErrorStatus RHA::subErase(Adesk::Boolean erasing);

	//- Osnap points protocol
public:
	/*virtual Acad::ErrorStatus subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	int gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const ;*/
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,	
		const AcGePoint3d &pickPoint,	
		const AcGePoint3d &lastPoint,	
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d& insertionMat) const; 



	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	//查询技术标准中距n_o_dR最近的圆曲线半径及其缓长
	int RHA::SearchProximalRadius(double &n_o_fR);


	/*---------------------------------------------------*/


	//现场里程转投影里程
	bool TrsCkmltoProjml(const ACHAR ckml[], double& projml) const;

	//现场里程转投影里程
	bool TrsCkmltoProjml(const ACHAR gh[], double dml, double& projml) const;

	//连续里程转现场里程  eg. "K159+900"
	bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC) const;

	//连续里程转冠号和里程 eg. "K"  159900
	bool TrsProjmltoCkml(double projml, int NLC,  ACHAR GH[], double &XCLC) const;//818 和TrsProjmltoCkml同名，两个函数的参数顺序调整了

	//投影里程转坐标
	bool TrsProjmltoNE(double Projml,xlpoint& PZ) const;

	void RHA::TrsProjmltoNE(double Projml, double &N,  double &E) const;


	//现场里程转坐标
	void TrsCkmltoNE(ACHAR Ckml[], double &N, double &E);
	void TrsCkmltoPZ(ACHAR Ckml[], xlpoint& PZ) const; 
	void TrsCkmltoPZ(ACHAR gh[], double dml, xlpoint& PZ) const;

	//统一里程转坐标
	void TrsCmltoNE(double cml, double& N, double &E) const;
	void TrsCmltoPZ(double cml, xlpoint& PZ);

	//N、E坐标转连续里程
	void TrsNEtoCml(double N, double E, double& cml);


	//获得绕行段数目
	int GetRXDNum(vector<JDarrayStruct> &jdAry);
	int GetRXDNum() const;




	//给投影里程求该点最原始的参照线，返回参照线指针
	RHA* GetBasLine(double projml2x, double &projml1x) const;


	//得到指定绕行段的起终交点下标
	bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx);

	bool GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx, vector<JDarrayStruct> &jdArray);

	//获取所有绕行段起终交点号
	bool GetAllRxdSEJD(vector<JDarrayStruct> &jdAry, vector<int>& SJDArray, vector<int>& EJDArray);

	//定位参照线直线段上的百米标
    double LocatHmOnLine(double sProjml, double eProjml, bool directn, ACHAR CKml[]) const;

	//给定统一里程，是否向前寻找附近的整百米标，要求在直线上
	double LocateHundredMeterOnLine(double tylc1x, bool IsForwardTo, ACHAR ckml[]) const;

	//对某统一里程处的现场里程取整，返回取整后现场里程对应的统一里程
	double RHA::RoundMileage(double cml, int intPrec, bool directn) const;
	
	//跟最近断链的统一里程的距离（取绝对值）,dirctn=true,向前找断链；=false，向后找
	double DistToNearDL(double projml,const vector<DLTab> &DLAry, bool IsForwardTo=true) const;

	//函数功能: 寻找某统一里程到附近的断链的距离（dirctn=true,向前找断链，=false，向后找）
	int RHA::FindNearDLbyCml(double cml, const vector<DLTab> &tmpDLB, bool dirctn) const;

	//函数功能:寻找某一投影里程附近的断链下标,dirctn=true,向前找断链；=false，向后找
	int RHA::FindNearDL(double projml, const vector<DLTab> &DLAry, bool dirctn) const;

	//寻找某几何里程的前一个断链号
	int RHA::GetPreDL(double projml) const;

	//寻找距离某几何里程最近一个断链(跳过从参照线上复制过来的断链)
	int RHA::GetPreRealDL(double projml) const;

	//搜索夹直线重叠的交点下标
	bool SearchJZXCDJDIndex_OnJYJX(int iJD2,int isQorH,RHA* pJYX,int sJD_JY, int eJD_JY,int &iOverLapJD);

	//点到线路的距离
	double  DistFromPtToGTZX(double x, double y);

	//生成绕行段起点断后冠号
	ACHAR* CreatRXD_SPtEGH(CString xlName, int LorR, ACHAR *GH1x);

	bool GenRXDL(int iRXD, int jdix, DLTab &dl, bool dirctn=true);

	bool GenBXDL(int jdix, DLTab &dl);

	//  设置断链数据
	void setDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx=-1, ACHAR *RXDName=NULL, ACHAR *BLName=NULL);
	//void AddDL(double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx=-1, ACHAR *RXDName=NULL, ACHAR *BLName=NULL);

	Adesk::Boolean  BZLC(AcGiWorldDraw* pWd,double dLC,int zy);

	bool GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno, const vector<DLTab> &dlAry) const;

	bool RHA::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno)const;  

	void MarkiRXDLC(AcGiWorldDraw* pWd,double dLC,int zy, int sdl, int edl);

	//标注绕行段里程和断链
	void MarkRXDLC(AcGiWorldDraw* pWd);

	//标注绕行段的里程和断链
	void RHA::MarkRXDLC() const;

	//标注单个断链
	void MarkSglDL(AcGiWorldDraw* pWd, int iDL, int zy);
	//(炸开)标注单个断链
	void RHA::MarkSglDL(int iDL, int zy)  const;


	void GenDL();

	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);

	//void ModifyJDXZ();
	//  标注特征点里程
	Adesk::Boolean MarkTZD(AcGiWorldDraw* pWd, int zybz=1);  
	//  标注特征点里程
	Adesk::Boolean MarkTZD() const;  //  标注里程



	//绘直线（设置线宽）
	Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);


	void checkJDNum();//检查前后交点号是否相同，相同将按复曲线处理


	//计算断链坐标
	void CalDL_NE(int sdl=0, int edl=-1);
	//根据修改后的交点数据更新断链
	void CmpNewDLwithOld();

	void SaveData();

	void UpdateAllData(/*int sJD=-1, int insrtBxJdNum=0*/);

    //是否全线绕行
	bool IsAllLineRX() const;


	//设置颜色和线宽和其它绘图参数
	void setColorLineW();
	void SetAllColor(int ColorIndex);

	//绘制缓和曲线
	Adesk::Boolean G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double LO,struct xlpoint &P0,struct xlpoint &PZ, double ND);

	//绘制线路各个线元
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint &P0,struct xlpoint &PZ,bool ISShow,double T0mini);
	//标注交点
	void MarkJD(AcGiWorldDraw* pWd,vector<JDarrayStruct> &arrayd,vector<QXBStruct>& qxb,ACHAR *JDchr);


	//设置实体属性参数、规范参数、绘图参数
	void InitJBCS();

	//判断参照线信息是否不全，如果不全返回true
	bool RHA::IsJdBLInfEmpty(const JDarrayStruct &iJD) const;

	/*判断参照线信息是否正确，如果正确返回true*/
	bool RHA::IsJdBLInfRight(const JDarrayStruct &iJD, RHA* &pBLRHA, int &blJDNo) const;

	//判断某一交点的前一边是否有平行约束，如果有返回true
	bool RHA::IsBfSideRecordParallel(const vector<JDarrayStruct> &jdAry, int iJD) const;

	//由交点前一边或后一边的线间距,适用于单边平行交点
	void RHA::ModifyJDXYbyOneParall(const vector<JDarrayStruct> &jdAry1x, int iJD1x
		, vector<JDarrayStruct> &jdAry2x, int iJD2x, bool isQorH);

	//由交点前后边的线间距计算二线交点坐标
	void RHA::CalExJDByParallel(vector<JDarrayStruct> &jdAry, int jdix);

	//由交点前后边的线间距计算二线交点坐标
	void RHA::CalExJDByParallel(const RHA *pBfBLRHA, const RHA *pCurBLRHA, int bfJDNo, int curJDNo, double bfXJJ, double curXJJ, JDarrayStruct *curJD);


	//修正二线交点坐标，交点坐标有可能不给或不够精确，由绕并关系重新计算
	void ModifyExJDXY();

	//函数功能：求解并行段二线交点的交点坐标
	void RHA::CalEXJD(int jdn1,int njd2,double EXJD[2], const RHA *m_pm1);

	////计算某并行交点
	void RHA::CalOneBXJDdata(int iJD,const RHA *m_pm1);



	//源线路pSrcRHA按线间距平移得到新线路时，计算新线路的交点坐标，注意：单纯按照线路平移
	void RHA::GetJDXYwhenMoveXLbyXJJ(const RHA *pSrcRHA, double xjj);



 //   //创建夹直线数组
	//void CreatJZXArray();


	//将本线路的几何线位和其它线路比较，确定参照关系，形成分段数组
	//，isOnlyBaseOnSJJX = true，只参照设计基线（对设计二线用）；= false，参照所有线路
	void CalExReference(bool isOnlyBaseOnSJJX=false);

	//比较几何，确定参照关系
	void CalExReference(CStringArray& XLNameAry, vector<JDarrayStruct>& RefJDarray);
	//形成二线分段数组GJDInfAry
	//bool CalExFDArray();

	//判断投影里程是否位于断链点上
	Int16 JudgePtIsOnDL(double Projml) const;

	Int16 RHA::JudgePtIsOnDL(ACHAR ckml[]) const;

	//NE坐标转投影里程
	void TrsNEtoProjml(double N, double E, double& Projml) const;

	//NE坐标转现场里程
	void TrsNEtoCkml(double N, double E, ACHAR Ckml[]) const;


	//由投影里程求所处绕行段序号（0开始）
	Int16 GetiRxdByProjml(double projml) const;
	//由经距、纬距，求所处绕行段序号（0开始）
	Int16 GetiRXDFromNE(double N, double E);
	//由经距、纬距，求所处绕行段序号（0开始）
	Int16 GetiRXDFromNE(double N, double E, double &projml);

	
	//标注线间距
	void MarkXJJ(AcGiWorldDraw* pWd);

	bool moveJZXAt(ads_point ptbase,ads_point ptout, int QQorHq);

	//获取前个交点号
	int GetPreJD(double N, double E) const;

	//求出某点(N, E)在线路上投影点的前后交点坐标,即获取所在边 plh07.17 
	int GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);

	//由统一里程获取附近交点，IsForwardTo=true 向前搜索(小于该交点的曲中点)
	int DistToNearJD(double projml, bool IsForwardTo) const;

	//给定几何里程寻找该里程前一交点号(小于该交点的直缓点)
	int RHA::FindPreJD(double projml);


	//平面设计函数 818 放bas_fun??

	//平移夹直线后，计算前后两交点坐标（通过点）
	bool RHA::MovJZX(ads_point onPt, ads_point passPt);

	//平移夹直线后，计算前后两交点坐标（偏移距）
	bool RHA::MovJZX(ads_point onPt, double offset);

	//旋转夹直线
	bool RotJZX(ads_point onPt, ads_point basPt, double angl);

	void  FormLCB(double sCml, double eCml, double step, CArray<LCB, LCB> &lcAry); 

	//标注线间距：1 线上一点，2 线上一点，标注线长度=20mm*SCALE，。。。。
	void MarkXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len);

	////计算曲线要素（标注用）
	//void CalNoteQXYS();

	//计算线元
	Adesk::Boolean  getpxy_DATA(int moveJDix=-1);

	void DeleteiRXDDLB(int iRXD);//删除某个绕行段断链表

	void DeleteOneRXD(int iRXD);

	void CalT1T2L();//818

	//投影里程projml转统一里程cml
	double  CalnewcmlTocml(double newcml) const;

	//统一里程cml转投影里程projml
	double  CalcmlTonewcml(double kmlTocml) const;

	//现场里程转统一里程
	void TrsCkmltoCml(ACHAR ckml[], double &cml) const ;
	
	//现场里程转统一里程
	void RHA::TrsCkmltoCml(const ACHAR gh[], const double xlc, double &cml) const ;

	//绘交点连线和交点圆
	void RHA::DrawJDLineAndCir(AcGiWorldDraw *pWd);

	//绘线位
	void RHA::DrawXL(AcGiWorldDraw *pWd);
	

	Adesk::Boolean  MarkBiaoZhiLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type);  

	//由断链自动生成标志里程数组 isCreatNew =true 重新生成，=false 接着生成
	void GenBZLCbyDL(bool isCreatNew=true);

	//标志里程按投影里程排序
	void SortBZLCArray();

	//改建段数组按投影里程排序
	void SortGJDArray(vector<GJDInf>& gjdAry);

	//计算标志里程信息，并排序
	void CalBZLC();

	//增加标志里程
	void AddBZLC(ads_point PT ,ACHAR CurAddBZLC[50], CString iRxdXLName, bool isAuto=false);

	//删除一个标志里程
	void DeleteBZLC(ads_point PT, bool IsDelDL=false);

	// 在一个绕行段上，根据更新的断链数组（线路改动）确定要保留的原来的标志里程数组bzlcAry
	//dlAry 新的绕行段断链数组
	void CmpNewBZLCwithOld(vector<DLTab> &dlAry, vector<BZLCStructTable> &bzlcAry);

	//获得绕行段名
	CString GetRxdName(int i);
	//规范检测
	void checkpm();

	bool  DelDLM(double tylc);
	bool  AddDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH, double TYLC, Int16 iRxd);

	//计算S弯的转角、两交点坐标
	bool CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2
		,double JZXLen,double ZH_N,double ZH_E,double Fwj
		,double& afa,double &x1,double &y1,double &x2,double &y2);
	//计算S弯的转角
	bool Get_S_Afa(double R1,double L1,double R2,double L2
		,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2);

	//选择S弯
	bool SelSModel(double N, double E,int& SJD,int& S_JD1,int& S_JD2);

	//获得某绕行段所有用户设置的标志里程
	void GetiRxdBZLCArray(int iRXD ,  vector<BZLCStructTable> & bzlcAry );

	//删除绕行段标志里程
	void DeleteiRXDBZLC(int iRXD, bool isDelSE=false);

	//获得对应交点下标(JDi)的半径、缓长、N E坐标
	void getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );

	bool AddRXD(AcGePoint3d PT1, AcGePoint3d PT2, AcGePoint3d OutPt
		,CArray<JDarrayStruct,JDarrayStruct>& jdAry, ACHAR ckml[], BOOL isPreMove=FALSE);

	//全线绕行线路自动赋交点号、交点性质
	void GenJDNameJDXZ();

	//指定交点序号，修改其坐标、半径、缓长
	void MODIFYJD(int iJD,double N, double E, double l1, double R, double l2);

	//由绕行段编号获取绕行段线路名
	CString GetRXDXLNameByRXDIndex(int RXDIndex);

	//由绕行段线路名获取绕行段编号
	int GetRxdIndx(CString RXDXLName);

	//由【纵面】绕行段线路名获取【纵面】绕行段编号
	int RHA::GetZMRxdIndx(CString zmRxdName);

	//将特征点桩号加入里程标
	void RHA::AddTZDtoLCB(double Scml, double Ecml, CArray<LCB,LCB> &LCBArray);



	//将桥隧站加入里程表 
	void AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT, CArray<LCB, LCB> &pLCB);



public:
	RHA(const  vector<JDarrayStruct> &jd2, CString pGH, CString JDName
		, CString  xlName=L"",CString mdbname=L"", CString  xlShuXing=L"");

	RHA(CArray<JDarrayStruct,JDarrayStruct> &jd2, CString pGH, CString JDName
		, CString  xlName=L"",CString mdbname=L"", CString  xlShuXing=L"", BOOL isSetDrawParam=TRUE);

    RHA(int pNJD, JDarrayStruct jd2[], CString pGH, CString JDName
		, CString  xlName=L"", CString mdbname=L"", CString  xlShuXing=L"");

	RHA(CString xlName,  CString xLShuXing, const RHA *pSrcRHA, double xjj);

	void RHA::CreateGTJXGJDArray(/*double spt[2],double ept[2],*/vector<GJDInf>& AllGJDArray);

	//创建本线路与pGTZXJY的利用段分段
	void CreatLYDArrayOnJYX(RHA* pGTZXJY/*,double Spt[2], double Ept[2]*/, vector<GJDInf>& LYDInfArray);

	void RHA::CreateRXGJD(/*double Spt[2],double Ept[2],*/vector<GJDInf>& RXGJDArray);

	void RHA::CreatBXGJD(/*double GJD_Spt[2],double GJD_Ept[2],*/vector<GJDInf> &LYDRXDArray, vector<GJDInf>& OneGJDArray);

	void RHA::SetDefaultGjdName(CString blName, double N, double E, int GJDType, ACHAR GjdName[256]);

	void RHA::SortSections(vector<GJDInf> &LYDArray);

	//合并改建段数组
	void MergeGTZX2GJDArray(vector<GJDInf>& AllGJDArray);
	//创建夹直线数组
	void CreatJZXArray();

	/////////以下为出平面图函数

	//生成从统一里程lc1到统一里程lc2的线路中线，缺省FALSE,CONTINUOUS
	void DrawXLZXByXY(double Scml, double Ecml, const vector<vector<double>>  XYArray, int XYNum, double ZxWid, int linetype, ACHAR* LayerName, int LRMFlag)  const;

	//调用DrawXLZXByXY，绘制各个改建段
	void MakeXLZX(double lc1,double lc2,int linetype,int LRMFlag,bool IsDrawRXGJ=true)  const;

	void RHA::BZJdLineAndJdCir() const;

	void RHA::BZJD(CString XLName,int LMRFlag) const;

	void RHA::MarkiRXDLC(double dLC,int zy, int sdl, int edl )  const;

	//绘制废弃段一个点的里程标注 打“×”
	void RHA::DrawOneFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const;
	
	//绘制一段废弃段
	void RHA::DrawFQDBZ(double StartLC, double EndLC, double JianJu, int LMRFlag) const;

	//读取改建段汇总表，形成既有基线、既有二线的利用段数组 GJDINFARRAY
	void RHA::CreatLYDArrayOfJYX();

	//绘制整段废弃段
	void RHA::BZJYXFQD() const;
	
	//判断一点是否在利用段上，在利用段上返回true
	bool RHA::IsPtOnLYD(double projml) const;
	
	//计算二线的曲线单个交点的内业断链
	double  RHA::CalCurvNYDL(int iJD) const;

	/////////纵断面
	//根据里程数组内插出地面线高程，返回地面线数组
	void RHA::PM_DMX(const CArray<LCB, LCB> &lcAry, double offst, vector<GMXTAB> &dmxAry);

	//获取基线上现场里程范围内的断链数组
	void RHA::GetBasLineDLAry(CString xlName, ACHAR sCkml[MaxCkmlStrNum], ACHAR eCkml[MaxCkmlStrNum], vector<DLTab> &dlAry);

	//生成二线贯通断链表
	void RHA::Create2XGTDLB();
	//清除并行段上从参照线上拷贝过来的断链（断链序号为-3）
	void RHA::EarseDLofBLonBXD();

	//统一里程转现场里程
	void  RHA::TrsCmltoCkml(double cml, ACHAR ckml[], int nLC=3) const ;

	//统一里程转现场里程
	void  RHA::TrsCmltoCkml(double cml, ACHAR gh[], double &xlc) const ;

	//统一里程转投影里程
	double  RHA::TrsCmltoProjml(double cml) const ;

	//投影里程转统一里程
	double  RHA::TrsProjmltoCml(double projml) const ;

	//由连续里程获取该里程所在的改建段类型
	int RHA::GetGJDTypeByCml(double Cml);
	int RHA::GetGJDTypeByProjml(double Projml);

	//由连续里程获取改建段信息：改建段类型GJDType，起终参照线路名BXLName，EXLName
	void GetPtGJDInfByCml(double Cml, int& GJDType, CString& BXLName, CString& EXLName);

	//由连续里程获取改建段信息：改建段类型GJDType，起终参照线路名BXLName，EXLName
	void RHA::GetPtGJDInfByCml(double N, double E, int& GJDType, CString& BXLName, CString& EXLName);


	//判断某现场里程在绕行段还是并行段上（纵面的并绕行）
	bool RHA::JudgeMLonBXDorRXD(ACHAR ckml[], CString &blName, CString &rxdName);

	//由基线连续里程判断是否在绕行段上，返回绕行段编号，在并行段返回-1
	int RHA::GetiRxdNoFromCml_1X(double Cml_1X);

	//绘制单个大中桥
	// 中心现场里程  起点里程  终点里程  桥长  桥梁样式	桥名  绕行段编号（－1、＜0表示为并行或单线）线数(1-单线，2-双线)

	void RHA::INSERT_BIG_MID_BRIDGE(CString xclc0,  CString xclc0_start
		,  CString xclc0_end,  double lenBri,  double HBri,  CString StyBri
		,  CString NamBri	 ,  int LineNum) const;

	//根据起终投影里程生成局部断链数组
	void RHA::CreateLocalDLarray(double sProjml, double eProjml, vector<DLTab> &dlAry) const;

	void RHA::MarkBridge() const;

	void RHA::INSERT_TUNNEL(CString qdxclc,CString zdxclc,CString NamTun,int LineNum,int Style) const;


	void RHA::MarkTunnel() const;

	void RHA::INSERT_STATION(CString xclc0,CString NamSta,CString TKSta,bool GJXZ) const;

	void RHA::DrawStaFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const;//绘制废弃段标注“×”

	void RHA::MarkStation() const;

	void RHA::MarkBM() const;
	//绘制水准点  
	void RHA::INSERT_BM(double X, double Y, CString GH, double XLC, double DD, CString BianHao, double LEV) const;

	//绘制导线
	void RHA::MarkDX() const;

	//绘制导线
	void RHA::INSERT_DX(int NUM, double *NX, double *EY, CStringArray *BianHao, double *GaoCheng, double *FWJ) const;

	//  标注线间距
	void RHA::BZXJJ() const ;

	//绘线间距  
	void makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType,double jiao=-100) const;

	//处理既有线小桥涵的接长
	void RHA::CalSmallBriLen(vector<SMALLBRIDGETAB> &smallBriAry) const;

	//绘制小桥涵 IsDrawAll-是否全部绘制还是只绘立交涵
	void RHA::plhINSBR(bool IsDrawAll) const;

	void RHA::INSERT_SMALL_BRIDGE(ACHAR gh[], double xclc0, CString NameCross, int LineNum, bool GJXZ
		, int LMRFlag, int iRXD, double LTW, double RTW, double LLevel
		, double RLevel, double jiao, int InsertStyle, double dCulvertUse) const;

		//绘涵洞:中心坐标，左宽，右宽，左高、右高，沿线路前进方向左转方位角，层名
	void RHA::DrawCulvert(AcGePoint3d Pt0,double LTW,double RTW,double FWJ,ACHAR layname[80],bool IsAddWid) const;

	//绘小桥涵
	void RHA::DrawSmallBridge(AcGePoint3d sPt, AcGePoint3d ePt, ACHAR layname[100], int LineNum, int mode=0, double LWid=3.0, double RWid=3.0, int LMRFlag=MAINLINE, double lc0=0.0, double Jiao=0.0) const;
	
	//在线位变动后，检查某绕行段起终点断链是否仍然适合，如果合适重新计算该断链的几何里程
	bool RHA::CheckRxdDL(int iRXD, bool isSDLorEDL);

	//根据线位的变化重新计算绕行段断链（尽可能保留原有断链的现场里程）
	void RHA::UpdateRxdDL(int iRXD);

	//更新绕行段标志里程
	void RHA::UpdateRxdBZLC(int iRXD, 	vector<BZLCStructTable> &bzlcArray);

	//从某断链开始，计算之后的所有断链的投影里程
	void RHA::CalDLprojml(int iStart);

	// 更新某绕行段的断链和标志里程
	void RHA::UpdateSglRxdLC(int iRXD, bool isCalXYarray=true);

	// 修改绕行段时，更新里程（断链、标志里程）
	void RHA::UpdateLC(int iRXD);


	//寻找某投影里程附近的标志里程下标，如果没找到返回-100 
	int  RHA::FindNearBZLC(double projml, bool dirctn);

	//根据用户设置的交点拖动方式和该交点的属性，得到该交点的实际拖动方式 
	int  RHA::GetJDMoveType(int jdMoveType, int iJD) const;

	//在增加交点时，判断拾取点所在范围是否允许增加交点
	bool RHA::JudgeIfCanAddJD(double N, double E, int &iRxd);

	//在删除交点时，判断拾取点所在范围是否允许删除交点
	bool RHA::JudgeIfCanDelJD(double N, double E, int &iJD, int &iRxd);

	//设置曲线要素
	void RHA::setJD_RL(int JDi ,double Ri,double L1i,double L2i );

	//计算同心圆半径
	bool RHA::CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
		double& BanJing2,double& HuanChang2);

	//计算某并行交点
	//void RHA::CalOneBXJDdata(int iJD)


	//自动产生某个绕行段的起终断链
	bool RHA::GeniRxdDL(int indx, DLTab rxdDL[2]);

	//判断在拾取点在线路起始边还是终止边
	bool RHA::IsPtonSLorEL(AcGePoint3d pt, bool &isSLorEL) const;

	//功能：插入线路时计算线路在目标线路上的起终点坐标
	bool RHA::GetSptEptWhenInXL(vector<JDarrayStruct> &tmpJDAry, const vector<QXBStruct> &tmpQXBAry);

	bool RHA::CombineJD(vector<JDarrayStruct> &jdAry);

	//返回指定交点所处的绕行段
	int RHA::FindiRXD(int iJD);

	//标注竖曲线
	void RHA::BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);

	bool RHA::BZCM(AcGiWorldDraw* pWd);

	//形成CM数组
	void RHA::TrsBPDArraytoCM();

	//生成改建段数组
	void CalGJDArray();

	/*---------------------------------------------------*/
	//全线坐标法	绘制拨量相关变量、函数	ljx	17.10.18
	//成员变量
	vector<SSurPt> m_MeasurePoints;		//测点结构体vector，存储了所有的测点
	bool m_IsDrawRectification;			//是否绘制拨量

	//成员函数
	void DrawCircle(AcGiWorldDraw *mode);														//画小圆函数
	void CalRectification(vector<double>& Rectification);										//计算拨量
	void DrawRectificationText(vector<double>& Rectification, AcGiWorldDraw* mode);				//画拨量文字
} ;

#ifdef PINGMIAN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(RHA)
#endif

