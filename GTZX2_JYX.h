// GTZX2.h: interface for the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX2_JYX_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
#define AFX_GTZX2_JYX_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GTZX_JYX.h"
#include "BAS_DRAW_FUN_JYX.h"
#include "UltraCArray.h"
#include "StdAfx.h"

#define B_X 0
#define R_X 1
#define S_StartPoint 2
#define S_EndPoint 3
#define B_XNOTP_X 4//plh05.05
//#define B_X_Des 5 //并行 半径缓长要输入
#define R_X_OneR 6 //绕行点
#define R_XStart 7
#define R_XEnd 8
#define MAXDLNUM2x 100
#define MAXJDNUM2x 100
#define LEFTLINE -1
#define RIGHTLINE 1
//DL值->长链为负，短链为正，运算规则：BLC+DL=ELC,DL=ELC-BLC;
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
class GTZX2_JYX_ROAD : public GTZX_JYX_ROAD
{		

public:

	//////////////////////////////////////////////////////////////////////////
	//里程坐标转换函数组
	//坐标转现场里程
	bool TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC=3);
	bool TrsNEtoProjml(double N, double E, double& Projml);
	bool TrsNEtoCml(double N, double E, double& Cml);
	bool TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ);
	bool TrsCkmltoProjml(ACHAR Ckml[], double& Projml);
	bool TrsCkmltoCml(ACHAR Ckml[], double& Cml, int iRXD=-1);
	bool TrsCmltoNE(double Cml, xlpoint& PZ);
	bool TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC=3);
	bool TrsCmltoProjml(double Cml, double& Projml);
	bool TrsProjmltoNE(double Projml, xlpoint& PZ);
	bool TrsProjmltoCml(double Projml, double& Cml);
	bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC=3);

	void SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd);
	void Create2XGTBPDArray(APDTAB&BPDArray);
	void Create2XGTBPDArray();
	UltraCArray_T<BPDArray,BPDArray> AllRXDBPDArray;

	//形成二线贯通断链表
	void Create2XGTDLB();
	//标注变坡点
	bool BZCM(AcGiWorldDraw* pWd);
	//二线贯通断链个数
	int m_2XGTDLSum;
	//计算标志里程
	void SetBZLCVal();

	BOOL isJYGuiFanCS;
    GuiFanCanShu m_GuiFanCS;
	bool m_IsNeedCalGJDInf;//是否需要从新计算贯通二线改建段信息
	BOOL isGJD;
	double TYLC2x(CString XCLC2x,int iRXD=-1);//plh08.20//若iRXD＞0在指定绕行段查找
	//plh04.14
	EXDLB DLB2x[MAXDLNUM2x];
	GTZX_JYX_ROAD *m_pm1;//一线平面
	DoubleLineData JD2[MAXJDNUM2x];
	int LEFT_OR_RIGHT;//左右线标记 (LEFTLINE -1) (RIGHTLINE 1)
	bool calsflag;


	//	CArray<GJDInf,GJDInf> GJDInfTab;

	//	CArray<EXGJDFDInf,EXGJDFDInf>GJDFDInfTab; //改建段分段信息表
	//	int NGJDFDInfTab;//改建段分段数
	//	CArray<GJDInforStruct,GJDInforStruct>GJDInforArray2;


	int NGJD;
	int zory;//设计左线或设计右线 -1 左线,1 右线
	ACHAR xlname[256];
	int m_LJorGM;
	int m_Xsw;
	int m_DesHXSW;
	//	double  JDarray[MAXJDNUM][6];  // 1->N,2->E,
	//	CArray<JDarrayStruct,JDarrayStruct>JDarray;
	//	double QXB[MAXJDNUM][9];	
	//	CArray<BAS_DRAW_FUN::QXBStruct,BAS_DRAW_FUN::QXBStruct>QXB;
public:

	GTZX_JYX_ROAD *CreateGtzx();
	//将二线绕行段断链表和其一线断链表合并为一个断链表	
	int CreateGtzxDLB();
	//修改交点性质：把绕行起终里程内所有交点性质变为绕行　
	void ModifyJDXZ(int iRXD);

	//修正绕行段起终点的坐标
	void Modify_RXD_SE_Coords(DoubleLineData p_JD2Arr[],int p_iJDIndex,DoubleLineData *p_JD);




	int GetiRxdNoFromTYLC1x(double lc1x);

	/////////////////////////////////////////////////////////
	void ModifyRXDName(double N, double E);
	void out();
	GTZX2_JYX_ROAD(AcDbObjectId Id1x, double xjj , BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD(GTZX_JYX_ROAD *gtzx, double xjj , BOOL IsGaiJian = FALSE );

	GTZX2_JYX_ROAD ( GTZX2_JYX_ROAD *gtzx2 , double xjj , BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD(GTZX2_JYX_ROAD *pm2,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[] , BOOL IsGaiJian = FALSE );
	//S弯计算
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
	void Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum); //插入２线断链
	void Update2XDLB(int iRXD,EXDLB *ExDlb, int DlNum);//更新２线断链
	void GetDLTYLC();
	void SetDLB(EXDLB *Dlb,int ndl);
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
	//	bool AddRXD(double (*jdarray)[6],double (*qxb)[9],int njd,double N,double E,ACHAR EGH[]);//plh04.11.25
	//bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[], BOOL PreMove = FALSE );//plh04.11.25

	//	void AddRXD(AcGePoint3d PT1,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove = FALSE ) ;//返回三个交点

	int getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );

	//	void AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd);
	// void AddRXD(AcGePoint3d PT1,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd);  //返回三个交点
	void AddRXD(AcGePoint3d PT1,AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove =FALSE ); //返回三个交点

	void AddRXD(AcGePoint3d PT1,AcGePoint3d PT2,AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove=FALSE );//返回三个交点

	bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[],ACHAR JDGH[] ) ; //plh04.11.25

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

	void GetiRxdBZLCArray(int iRXD , CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable> & iRxdBZLCArrayToOut );

	//删除指定绕行段
	void DeleteiRXDDLB(int iRXD);

	//获取绕行段的总数
	int GetRXDNum();

	//得到指定绕行段的起终交点序号

	void GetJDXHofiRXD(int iRXD,int &firstJDXH,int &endJDXH);

	//计算批定绕行段的EXDLB
	void Create2XDLB(int iRXD,ACHAR EGH[]=NULL,bool IsUpdate=true);//plh04.11.25

	//计算所有绕行段的EXDLB
	void Create2XDLB();

	void Generate2XDLB(int p_iRXD,CArray<EXDLB,EXDLB> &p_arrDLBNew);

	double T0min;//最小夹直线长
	double Lrmin;//最小圆曲线长
	void GetT0min_Lrmin();
	void UpdateData(bool IsCreateFirst=true);
	void UpdateDataRXD(int iRXD);//更新贯通二线某绕行段数据


	void UpdateDataSeveralRXD(CArray<int,int> &p_arrChangedRXD,UltraCArray_T<RXDInfo,RXDInfo> &p_arrNewRXD);








	void InsertDateRXD(int iRXD);//插入一个绕行段后只更新贯通二线该绕行段数据
	//	GTZX2(GTZX *pm1,double[][2],int njd2,ACHAR jdgh[],int jdo);//一线数据，一线交点号和线间距，二线交点冠号，起始交点编号
	GTZX2_JYX_ROAD(GTZX_JYX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[], BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD();
	virtual ~GTZX2_JYX_ROAD();
	void CalJDdata();//自动计算半径、缓长
	void CalJDdata1();//不计算半径、缓长
	void CalNYDL();
	bool CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
			double& BanJing2,double& HuanChang2);
	double CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2);
	static double Lh_Rtop(double Lh,double R);
	ACRX_DECLARE_MEMBERS(GTZX2_JYX_ROAD);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow=true);
	//	void BZJD(AcGiWorldDraw* pWd,double arrayd[500][6],double qxb[500][9],int tarray,int JD0,ACHAR *JDchr);
	void BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& JDarray,CArray<QXBStruct,QXBStruct>& qxb,ACHAR *JDchr);

	//输入一线里程，计算线间距，二线上对应连续里程及点,1-表示成功，0-失败
	//  int CalXjj(double ml1,double &xjj,double &projml,double resPt[2]);
	void CalEXJD(int jdn1,int njd2,double EXJD[2]);//输入一线交点号，二线对应交点号，返回并行段二线交点坐标
	int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	//   int Judge2XLorR(xlpoint PZ);
	void SetJDdata();//生成二线JDarray
	static double GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dbj,double &T);
	double CalTFromR_Lr_Lh(double R,double Lr,double Lh); 
	double CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0);		
	void BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl , GTZX_JYX_ROAD *pSubGtzxToIn);


	CArray<Gtzx2BZLCStruct,Gtzx2BZLCStruct>AllBLZCArray ;

	void BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S=true);//标注特征点里程

	Adesk::Boolean BZTZD2(AcGiWorldDraw* pWd,double dlc,int zybz,int type);  //  标注里程

	virtual Adesk::Boolean addDLM(double tylc,double BLC,double ELC, ACHAR *EGH , BOOL IsSingleModifyR =FALSE );
	virtual Adesk::Boolean   DelDLM(double tylc);
	//plh04.14	
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const; 
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		AcDbIntArray& osnapModes,
		AcDbIntArray& geomIds) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		const AcGeVector3d& offset);

	//搜索是否给定点在已有改建段上,-1为没有
	int GetGJDNum(double N,double E);
	//给定贯通设计二线上N，E坐标获取该点所在改建段的索引号，利用段返回-1
	int GetGT2xGJDIndex(double N,double E);
	//设置缺省改建段名
	void SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256]);
	//判别给定改建段与既有改建段是否存在包含关系
	bool IsIncludeByOtherGjd(double spt[2],double ept[2]);
	bool IsJdInGTZX(int iJD);
	int SearchGJDEptIndex(int sjdn, int ejdn);
	int SearchGJDSptIndex(int sjdn, int ejdn);
	int CreatGJDInfTab();	
	CString CalGJDLC(double N,double E);//逐个标注绕行段
	void BZSTR(AcGiWorldDraw* pWd,xlpoint PZ,CString str1,CString str2); 
	void BZGJD(AcGiWorldDraw* pWd);//标注所有改建段
	void BZiGJDLC(AcGiWorldDraw* pWd,int iGJD);//逐个标注绕行段
	Adesk::Boolean  getZhHzPt(int iJD,int zhorhz,double pt[2]);

	void DrawXL(AcGiWorldDraw* pWd , ACHAR CurXLName[256]);

	void CheckJD2IsAvailable();
	// 创建IRXD的基线指针
	GTZX_JYX_ROAD * GetiRxdtoGTZXplinp(int irxd );

	void SetBZLCArray( BZLCStructTable *BZLCTableToIn , int AddBZLCNumToIn );

	void Insert2xBZLC(int iRXD,CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable>& BZLCArrayToIn );

	void GetiRXDBZLCinfo(int iRXD, int &SBLZCNum, int &EBLZCNum);

	void addBZLCToGTZX2(ads_point PT ,ACHAR CurAddBZLC[30] ,CString iRxdXLName );

	int CreatEXGJDInfTab(GTZX_JYX_ROAD* pGTZXJYJX, GTZX_JYX_ROAD* pGTZXJYYX, GTZX_JYX_ROAD* pGTZXJYZX, CArray<EXGJDFDInf,EXGJDFDInf&> &EXGJDFDInfTab);
	GTZX2_JYX_ROAD *m_pm2;
	/*
	virtual Acad::ErrorStatus getOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray&     geomIds) const ;
	*/

	//形成贯通设计2线分段信息数组
	//	void FormSectionInforForGTZX2();

	//得到贯通设计2线的现场里程，输入NE坐标
	CString GetGTX2Xclc(double N,double E);

	//由连续里程计算现场里程
	CString GetGTX2XclcFromcml(double cml);

	//计算曲线表中现场里程
	void CalCKQXBToKml(CArray<QXBStruct,QXBStruct>& CKQXB);


	//贯通设计2线标注起点／终点
	void DrawBandEPointsForGTZX2(AcGiWorldDraw* pWd );


	////根据绕行段编号得到绕行段平面指针 zj 07.7.4
	//GTZX* GetiRxdtoGTZXplinp(int RXDno);
 
	//根据2线统一里程得到饶性段编号 zj 07.7.5
	int GetiRxdNoFromTYLC2x(double lc2x);

	//根据绕行段名得到饶性段编号 zj 07.7.15
	int GetiRxdNoFromRXDName(CString RXDName);

	//重载了一下,增加了输出起终点的交点号 zj 07.7.16
	void GetiRXDJDArray(int iRXD, double jday[][6], int &njd,int &SJDNum,int &EJDNum);

	CString GetRXDGTXXclc(double N,double E);

	void DrawBZLC(AcGiWorldDraw* pWd);

	void TrsBPDArraytoCM();
	void DrawOneBZLC( AcGiWorldDraw* pWd ,GTZX_JYX_ROAD * pSubGtzxToIn ,int CurNum );

	//得到贯通设计2线改建分段数组
	void GetGTXSJ2gjdInforArray(GTZX_JYX_ROAD *pGT1x , CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2);


	//细化改建段分段数组
	void GetGTXSJ2Allgjdinfor(CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2ToIn , CArray<GJDInf,GJDInf&>& GJDInforArrayForGTXSJ2ToOut);

	//得到细化后的改建段指针(基线格式)
	GTZX_JYX_ROAD  * GetOneGTXSJ2TopLinep(GJDInforStruct CurGJDInforToIn );

	//判断Pt(N,E)点是否在改建段上，和改建段类型:-1-利用 0-并行改建 1-S弯改建 2-绕行改建
	int GetPtGJDType(double N, double E);

	void GetiRxdStartEndMlByNum(int irxd,double &ProjSml,double &ProjEml);

	//获取贯通设计二线中绕行段，并将其转换为贯通设计基线指针
	GTZX_JYX_ROAD * GetiRxdtoGTZXplinp_GTX(int irxd);

	//计算贯通设计线的线间距
	double CalGTXXJJ(double cmlOnGT1X, GTZX_JYX_ROAD *pJYJX, GTZX_JYX_ROAD* pGTJX, GTZX2_JYX_ROAD* pGTEX,double& XJJ1, double&XJJ2);


	bool AddMSRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[]);

	//由绕行段编号获取绕行段线路名
	CString GetRXDXLNameByRXDIndex(int RXDIndex);

	int GetRXDIndexByRXDXLName(CString RXDXLName);

	void GetiRXD1xTYLC(int iRXD,double &startLc,double &endLc);

	//标志iRxd标志里程
	Adesk::Boolean  BZLC2(AcGiWorldDraw* pWd,GTZX_JYX_ROAD * pSubGtzx ,int zybz , int type )  ;

	//1.判断是否从延长线上引出
	// JDNum -- 二线交点号
	BOOL JudgeCurJDIsOnXL( int JDNum , BOOL IsBefore );

	//修正二线百米标的坐标
	void ModifyExNEForHundredMeter( double & N, double & E , BOOL IsForWard );

	//修正所有二线百米标的坐标
	void ModifyAllExNEForHundredMeter( );

	int  GetGTX2XJD(double N, double E ,BOOL IsForWard);
	void  GetGTX2XclcFromcml(double cml,double &dml,ACHAR gh[8]);


	void DeleteiRXDBZLC(int iRXD);

	//由坐标计算里程
	void CalNEForKml2x();

	//由里程计算坐标
	void CalKmlForNE2x();

	//  删除一个标志里程
	void DeleteBZLCToGTZX2(ads_point PT);

	Adesk::Boolean BZRxdZD(AcGiWorldDraw* pWd,double dlc,int zybz,int type , GTZX_JYX_ROAD * pSubGtzx );  //  标注里程

	void SetDefaultiRxdBZLC(int iRXD , CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCarrayToOut );

	void SetBZLCArray( CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCArrayToIn );

	bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray , ACHAR EGH[],bool IsRX = true, bool IsHBQX = false);

	void GetNEToGtzx2xkml( CString kml2x , double & NToOut , double & EToOut );

	void DeliRxdBZLCArray(int iRXD);

	//////////////////////////////////////////////////////////////////////////
	//生成贯通设计线改建段
	void CreateLYDArray(double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	//搜索设计线与既有线夹直线重叠部分的交点下标号
	bool SearchJZXCDJDIndex_OnJYJX(GTZX_JYX_ROAD* pJYJX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1);

	bool SearchJZXCDJDIndex_OnJYEX(GTZX2_JYX_ROAD* pJYEX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1);

	//创建夹直线数组
	void Create2XZJXArray();

	//定位利用段的百米标
	void LocateHundredMeterOnLine_LYD(double Pt[2],bool IsBefore);

	//创建绕行改建段数组
	void CreateRXGJD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>& RXGJDArray);

	void CreateGJDInGT2XRXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateGJDInGT2XBXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateGJDPt_By_LYDPt(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateAllGJD(CArray<GJDInf,GJDInf>& BXGJDArray,CArray<GJDInf,GJDInf>& RXGJDArray,CArray<GJDInf,GJDInf>& AllGJDArray);

	void RefreshGTZX2DLArray(double Sml, double Eml,int OriNDL, EXDLB OriEXDLB[], int NewNDL, EXDLB NewEXDLB[],int& RefreshNDL, EXDLB RefreshEXDLB[]);

	void RefreshGTZX2GJDArray(double Sml, double Eml, CArray<GJDInf,GJDInf>& OriGJDArray, CArray<GJDInf,GJDInf>& NewGJDArray, CArray<GJDInf,GJDInf>& RefreshGJDArray);
	
	void RefreshExFDArray(double RefreshSml,double RefreshEml,int iRXD);

	void ModifyiRXDDLB2xByGJDArray(int iRXD);
	
	void SortGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray);

	void MergeGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray);

	void CreateLYDArray_OnJYJX(GTZX_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	void CreateLYDArray_OnJYEX(GTZX2_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	void CreateLYDAndBXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& LYDArray,CArray<GJDInf,GJDInf>& BXGJDArray);

	void CreateGTJXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& AllGJDArray,CArray<GJDInf,GJDInf>& LYDArray);
	//////////////////////////////////////////////////////////////////////////

	//由现场里程获取对应的改建段，该里程位于并行段时返回-1
	int GetRXDIndexByXCLC2X(CString XCLC2x);

	//由现场里程获取投影点PZ
	int GetPZFromXCLC2X(CString XCLC2x,struct xlpoint& PZ);
	
	double CalZXNYDL(double sml,double eml);//根据起终点里程计算直线段内业断链 zhongjing

	bool Get_S_Afa(double R1,double L1,double R2,double L2,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2);

	bool CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2,double JZXLen,double ZH_N,double ZH_E,double Fwj,double& afa,double &x1,double &y1,double &x2,double &y2);

	bool SelSModel(double N, double E,int& SJD, int& S_JD1,int& S_JD2);//选择S弯

	void ModifyDLB2xByGJDArray();//由贯通设计二线的改建段信息增加二线断链数据

	void CalGT2xDLValue();//计算贯通二线的断链处的几何统一里程

	void GetGT2XDLArray(double Sml,double Eml,int& DLSum, EXDLB DLBArray[MAXDLNUM]);//获取投影里程在Sml-Eml范围内的断链,不包括Sml，Eml

	int JudgePtIsOnDL(double Cml);//传入几何统一里程，判断改点是否在断链上

	int JudgePtIsOnRXD(double N,double E);//判断某点是否在绕行段上

	void CalGJDStartEndXCLC(GTZX_JYX_ROAD::GJDInforStruct& GJD);//计算改建段起终点在既有线上的现场里程

	void setColorLineW();//设置颜色线宽

	void InfluenceOfGJDModifyToRXD(CArray<GJDAllInforStruct,GJDAllInforStruct> &p_arrOriBasicSectionInfo,CArray<int,int> &p_arrInfluRXD);

	void CalCKQXBToKml();

	void FormLCB2X(double sml,double eml,double Step);
	void FormSectionInfor();//形成分段信息数组

	typedef struct
	{
		double Sml;
		double Eml;
		int RXDIndex;

	}Sec;

	//----------------------------------------------------------------------------
	typedef UltraCArray_T<GJDAllInforStruct,GJDAllInforStruct> GJDArrToOneRXD;
	typedef struct  
	{
		GJDArrToOneRXD GJDArr;
		int iRXDIndex;
	}GJDArr_RXD;

	UltraCArray_T <GJDArr_RXD,GJDArr_RXD> m_arrOldGJDArrToRXD;
	UltraCArray_T <GJDArr_RXD,GJDArr_RXD> m_arrNewGJDArrToRXD;
	//------------------------------------------------------------------------------


	void GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,CArray<int,int>& p_arrJDIndex,CArray<DoubleLineData,DoubleLineData>& p_arrJD);


	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------


};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX2_JYX_ROAD);
#endif // !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
