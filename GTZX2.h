// GTZX2.h: interface for the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
#define AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GTZX.h"
#define B_X 0
#define R_X 1
#define S_StartPoint 2
#define S_EndPoint 3
#define B_XNOTP_X 4//plh05.05
#define MAXDLNUM2x 100
#define MAXJDNUM2x 100
#define LEFTLINE -1
#define RIGHTLINE 1
//DL值->长链为负，短链为正，运算规则：BLC+DL=ELC,DL=ELC-BLC;
class GTZX2_ROAD : public GTZX_ROAD
{	
public:

	//////////////////////////////////////////////////////////////////////////
	//里程坐标转换函数组
	//坐标转现场里程
	 bool isReFresh;
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
    void SetiRXDBPDArray(int iRXD, int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd);
	void SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd);
	void Create2XGTBPDArray();
	UltraCArray_T<BPDArray,BPDArray> AllRXDBPDArray;

	//将下标为iRXD绕行段创建为一GTZX指针
	GTZX_ROAD* GetiRXDGTZX(int iRXD);
	//形成二线贯通断链表
	void Create2XGTDLB();
	//标注变坡点
	bool BZCM(AcGiWorldDraw* pWd);//标注边坡点
	//二线贯通断链个数
	int m_2XGTDLSum;
	//检测与其它线路的交叉约束
	void CheckAllXLConstraint();
	double TYLC2x(CString XCLC2x,int iRXD=-1);//plh08.20//若iRXD＞0在指定绕行段查找
	//plh04.14
	EXDLB DLB2x[MAXDLNUM2x];
	GTZX_ROAD *m_pm1;//一线平面
	DoubleLineData JD2[MAXJDNUM2x];
	int LEFT_OR_RIGHT;//左右线标记 (LEFTLINE -1) (RIGHTLINE 1)	

public:

	//修改交点性质：把绕行起终里程内所有交点性质变为绕行　
	void ModifyJDXZ(int iRXD);
	int GetiRxdNoFromTYLC1x(double lc1x);

/////////////////////////////////////////////////////////
	void ModifyRXDName(double N, double E);
	void out();
	GTZX2_ROAD(AcDbObjectId Id1x, double xjj);
	GTZX2_ROAD(GTZX_ROAD *gtzx, double xjj);

	void CalExJDByParrel(int jdn1, double xjj1, double xjj2, DoubleLineData *CurJD);
		
	int FindACHAR(ACHAR *str,ACHAR At);
		
	void ModifyNEbyXJJ(double &N, double &E, double xjj);
		
	void ModifyNEbyXJJ(ACHAR JDLC[],double xjj,double &N,double &E);
		
	void ModifyNEOnBian(DoubleLineData *CurJD);
		
	void ModifyOriExJDXY();
		
	//标记二线交点是否位于段落内(是否显示到屏幕上)
	void TagEXJDIsShow();
	
//////////////////////////////////////////////////////////
	//plh04.14
	void GetiRXDJDArray(int iRXD,double jday[MAXJDNUM][6],int &njd);
	//plh04.14
	void AddDoubleLineJD(DoubleLineData ExJD);//尾部增加一个交点
	//plh04.14
	void ModifyDoubleLineJD(int iJD,DoubleLineData ExJD);//修改指定交点
	//plh04.14
	int GetiRXDFromNE(double N,double E);
	//plh04.14
	void makeXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len);
	//plh04.14	
	void BZXJJ(AcGiWorldDraw* pWd);
	void Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum);
	void GetDLTYLC();
	void SetDLB(EXDLB *Dlb,int ndl);
	void SetDLB(BAS_DRAW_FUN::EXDLB *Dlb, int ndl);
	void DelS(double N,double E);
	double DistToJD(double N,double E,int iJD);
	void ModifyOneSWJDdata(int iJD,double N,double E,double XJJ,double JIAO);
	void CalOneBXJDdata(int iJD);
	bool AddS(double N1, double E1, double N2, double E2,double R1,double Lo1,double R2,double Lo2);
	int FindiRXD(int iJD);
	void DeleteOneRXD(int iRXD);
	void DeleteOneRXDByXLPoint(double N,double E);
	void InsertOneRXD(DoubleLineData *OneRXDJDB,int njd,ACHAR EGH[]);//plh04.11.25
//	bool AddRXD(GTZX *newGtzx,double N,double E);
	bool AddRXD(double (*jdarray)[6],double (*qxb)[9],int njd,double N,double E,ACHAR EGH[]);//plh04.11.25
		
	int getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );
		
	void AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd);
	void INIT();

	double XCLC(double TYLC,ACHAR *GH,EXDLB *ExDlb, int ndl);

	//由交点数组，交点个数，曲线表,返回线元数组，...	
	void CalXYArrayFromJdArray(double (*jdarray)[6], int njd,double (*qxb)[9],double (*xyarray)[10],int &tary);
	
	void BZiRXDLC(AcGiWorldDraw* pWd,int iRXD);//逐个标注绕行段
	void BZRXD(AcGiWorldDraw* pWd);

	//给定统一里程，是否向前寻找附近的整百米标，要求在直线上,	
	double LocateHundredMeterOnLine(double tylc,bool IsForwardTo=true);

	//查寻某绕行段的起终断链编号
	void GetiRXDDLinfo(int iRXD,int &sDLno,int &eDLno);

	//删除指定绕行段
	void DeleteiRXDDLB(int iRXD);

	//获取绕行段的总数
	int GetRXDNum();
	
	//得到指定绕行段的起终交点序号
	void GetJDXHofiRXD(int iRXD,int &firstJDXH,int &endJDXH);

	//计算批定绕行段的EXDLB
	void Create2XDLB(int iRXD,ACHAR EGH[]=NULL);//plh04.11.25

	//计算所有绕行段的EXDLB
	void Create2XDLB();
	
	double T0min;//最小夹直线长
	double Lrmin;//最小圆曲线长
	void GetT0min_Lrmin();
	void UpdateData(bool IsTheFirst = true);
//	GTZX2(GTZX *pm1,double[][2],int njd2,ACHAR jdgh[],int jdo);//一线数据，一线交点号和线间距，二线交点冠号，起始交点编号
	GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[]);
	GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,BAS_DRAW_FUN::DoubleLineData jd2[],int jdo,ACHAR jdgh[]);
	GTZX2_ROAD();
	virtual ~GTZX2_ROAD();
	void CalJDdata(BOOL full = FALSE,int qdi = 0,int zdi = 0);//自动计算半径、缓长
	void CalJDdata1();//不计算半径、缓长
	void CalNYDL();
	double CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2);
	static double Lh_Rtop(double Lh,double R);
	ACRX_DECLARE_MEMBERS(GTZX2_ROAD);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow=true);
	void BZJD(AcGiWorldDraw* pWd,double arrayd[500][6],double qxb[500][9],int tarray,int JD0,ACHAR *JDchr);
	//输入一线里程，计算线间距，二线上对应连续里程及点,1-表示成功，0-失败
	//  int CalXjj(double ml1,double &xjj,double &projml,double resPt[2]);
	void CalEXJD(int jdn1,int njd2,double EXJD[2]);//输入一线交点号，二线对应交点号，返回并行段二线交点坐标
	int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	//   int Judge2XLorR(xlpoint PZ);
	void SetJDdata();//生成二线JDarray
	static double GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dbj,double &T);
	double CalTFromR_Lr_Lh(double R,double Lr,double Lh); 
	double CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0);		
	void BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl);
	void BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S=true);//标注特征点里程
  virtual Adesk::Boolean addDLM(double tylc,double BLC,double ELC, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(double tylc);
//plh04.14	
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const; 
  virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		                                                AcDbIntArray& osnapModes,
														AcDbIntArray& geomIds) const;

  virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		                                       const AcGeVector3d& offset);
};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX2_ROAD);
#endif // !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
