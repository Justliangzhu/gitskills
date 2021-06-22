// HdmDes.h: interface for the HdmDes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDMDES_H__0AF46E78_FFC1_44CB_973A_5114CAB81AB9__INCLUDED_)
#define AFX_HDMDES_H__0AF46E78_FFC1_44CB_973A_5114CAB81AB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "hdmdmx.h"
#include "bri.h"
#include "tun.h"
#include "HdmDataBase.h"
#include "cross.h"
#include "OutTAB.h"


class HdmDes  : public HdmDmx
{
public:
	HdmDes();
	virtual ~HdmDes();
	bool If_end;
	//20200322梁柱添加汇总表数据
	double RoadLen, FillEarWok, CutEarWok, CutStnWok, C25, BrkStn, BrkPav, C15, GutLen, EarWok, M75, B, Steel, GreAre, HanGuan,YDAllArea;
 	struct RoadHdmdata
	{
		CString roadname;//道路名
		JD_CENTER *pm;
		GTZDM_ROAD *AllZM,*ZZM,*YZM;//整幅纵面,左幅纵面,右幅纵面
		int NAllBri,NZBri,NYBri;
		BRITAB *AllBri,*ZBri,*YBri;//整幅桥,左幅桥,右幅桥
		int NAllTun,NZTun,NYTun;
		TUNTAB *AllTun,*ZTun,*YTun;//整幅隧,左幅隧,右幅隧		
		int ZcgNm,YcgNm,ZjkNm,YjkNm;//超高加宽
		CgSZ *Zcg,*Ycg,*Zjk,*Yjk;
		
		int NCRS;
		HDMDMXdata *hdmdmx,*hdmBench;//横断面地面线，台阶	

		CRSDAT *hdmdes;
		HdmDataBase hdmbz;
		int NZLMK,NYLMK;
		LMKdata *ZLMK,*YLMK;
		int BxNum;
		BXLMKD *pBXArr;
		int XZ;//0-主线，1-匝道
		//起始终止点在主线上的投影里程
		double SmlOnZx,EmlOnZx;
		//起终路肩分离点在主线上的投影里程
		double LJFLSmlOnZx,LJFLEmlOnZx,LJFLSmlOnFLD,LJFLEmlOnFLD;
		//起终边坡分离点在主线上的投影里程
		double BPFLSmlOnZx,BPFLEmlOnZx,BPFLSmlOnFLD,BPFLEmlOnFLD;
		//横断面设计数据
		int LorR;
		int ZxOrYx;//在主线的左侧或右侧
		//算量新增2019-08-29
		double L_BPArea;//左侧边坡绿化面积-总量-出表时汇总使用
		double R_BPArea;//右侧边坡绿化面积-总量-出表时汇总使用
		double PSAreaT;//排水填方体积-总量-出表时汇总使用
		double PSAreaW;//排水挖方体积-总量-出表时汇总使用
		double DMAreaT;//断面填方体积-总量-出表时汇总使用
		double DMAreaW;//断面挖方体积-总量-出表时汇总使用
		double LMArea;//路面面积-总量-出表时汇总使用
		double CGLength;//侧沟长度-总量-出表时汇总使用
		double PSGLength;//排水沟长度-总量-出表时汇总使用
		double TGLength;//天沟长度-总量-出表时汇总使用
		double CGJQvolume;//侧沟浆砌体积-总量-出表时汇总使用
		double PSGJQvolume;//排水沟浆砌体积-总量-出表时汇总使用
		double TGJQvolume;//天沟浆砌体积-总量-出表时汇总使用
		//统计批量工程数量
		

		RoadHdmdata()
		{
			NAllBri = NZBri = NYBri = 0;
			NAllTun = NZTun = NYTun = 0;
		}
	} ;
    RoadHdmdata *road;
	RoadHdmdata *Zroad,*Yroad;    	
	int RoadSum,ZRoadSum,YRoadSum;
    double SCALE;
	AcGePoint3d StartPt;
	AcGePoint3d ZStartPt,YStartPt;
	int ZSorX,YSorX;//采点为于一线绘图原点的上或下
	double StartX,StartY;
    struct Maplist
	{
		int ncrs;
		double SX,SY;//图框内框的起始XY坐标
        int HDMNO[100];//记录落在MAP范围内的横断面编号,最多100		
	};
	int MAPSUM;
	struct BDData BDArr[100];
    int NBIDUAN;//鼻端个数
	int m_Isbuju_HDm;//是否布局出图
	int CurRoad;
	Maplist *MAP;
	bool Ifpage;
	double m_ljdist,m_ljdh;

	//20190822新增	改沟参数
	bool m_IsModifyGutter;		//是否是改沟模式
	CString m_GutterPath;		//改沟路径
	double m_GutterThick;		//水沟沟底厚

	//20190829新增	绘制占地桩参数
	bool m_IsDrawCrossEarthPile;	//是否绘制占地桩

	//20190910新增	图框图块名称	水沟图块名称
	ACHAR m_FrameBlockName[256];		
	ACHAR m_GutterBlockName[256];

	//20190911性质	指定桩号性质数组
	std::vector<CrossType> m_CrossTypeArray;

	//增加图框长宽参数
	double m_FrameWid, m_FrameHeight;

	int ReadFile(bool IsForDes,CString mdbFname=L"");
	void ReadLmInfor();
	int SetBXDmx(CString mdbname,CString RoadName,double dmx[],double ZJ,double sml,double eml,JD_CENTER *pm);
	JD_CENTER *ReadPm(CString mdbname,CString RoadName);
	GTZDM_ROAD *ReadZm(CString mdbname,CString RoadName,CString XZ,JD_CENTER *pm);
	void ReadBri(CString mdbname,CString RoadName,CString XZ,RoadHdmdata &RoadData);
	void ReadTun(CString mdbname,CString RoadName,CString XZ,RoadHdmdata &RoadData);
	void ReadCG(CString mdbname,CString RoadName,CString CGTABname,RoadHdmdata &RoadData);
	void ReadJK(CString mdbname,CString RoadName,CString JKTABname,RoadHdmdata &RoadData);
	int ReadHdmdmx(CString RoadName,CString mdbname,RoadHdmdata &RoadData);
	void ReadHdmBench(CString RoadName,CString mdbname,RoadHdmdata &RoadData);//读取横断面台阶线
	void ReadLMK(CString mdbname,CString RoadName,CString LMKTABname,RoadHdmdata &RoadData);
	void InitRoadData(int i);
	void ReadZm(CString mdbname,CString RoadName,CString XZ,RoadHdmdata &RoadData);

	bool SetHdmLm();
    void WriteHdmDmxFile(int i);
	void WriteHdmBenchFile(int i); 
	void DesignHdm();

	int NCM;//CM数组总数
	double* CM;//纵断面数据数组，每个变坡点有五个属性。
						//依次为：0里程，1高程，2坡度，3坡长，4竖曲线半径
	double Cal_des_level(double cml); //用于纵面优化算设计高程
	double CalDesH(double cml,GTZDM_ROAD *ZM1,GTZDM_ROAD *AllZM);
	void CalLMHP(double cml,int cgNm,CgSZ *cg,double &LMHP,double &YLJHP);
	void CalLMK_JK(double cml,int NLMK,LMKdata lmk[],int ncg,CgSZ cg[],int njk, CgSZ jk[],double &half_fgdk,
					double &lydk,double &cxdk,double &yljk,double &tljk,double &tljhp,
					int &bkmode,int &sjbgmode);

    void CalLMKNoJK(double cml,int NLMK,LMKdata lmk[],double &half_fgdk,
					double &lydk,double &cxdk,double &yljk,double &tljk,
					double &lmhp,double &yljhp,double &tljhp,
					int &bkmode,int &sjbgmode,double& OffsetByXLZX);

    void CalLMPT(double cml,double DesH,int cgNm,CgSZ cg[],
					 int jkNm,CgSZ jk[],int NLMK,LMKdata *LMK,double RHF[10],int ZorY,int BxNum,BXLMKD *pBXArr,JD_CENTER *pm);

	void CalLMJK(double cml,int ncg, CgSZ cg[], double &addwid);			  
    int SetDmProperty(double cml,JD_CENTER *pm,int NBRI1,BRITAB Bri1[],int NBRI2,BRITAB Bri2[],
						  int NTUN1,TUNTAB Tun1[],int NTUN2,TUNTAB Tun2[],int ZorY,double &BTZDK,int qdmxz=0);

	void CalHdmBp();

	double FindIljxk(double cml,JD_CENTER *pm,int nljxk,dmdata ljxk[]);//宽
	void FindIljjk(double cml,JD_CENTER *pm,int nljjk,pwddata ljjk[],double &ltjk,double &lqjk);//宽
	//double FindGLTHD(double cml,JD_CENTER *pm,int nldtl,LDTLdata ldtl[]);
	int FindSG(double cml,JD_CENTER *pm,int nldsg,LDSGdata gou[],int &WallorGou,
				   double &PSG_JQHD,double &T2W,double &T1W,double &T2H,double &GH,double &GW,
				   double &HH,double &SP1,double &SP2,double &SP3,double &PSG_GDR);

	int FindTG(double cml,JD_CENTER *pm,int ntg,TGdata gou[],double &BJ,double &GW,double &GS,double &SP1,double &SP2,double &TG_JQHD,double &TG_GDR);
	int FindWichBP(double cml,JD_CENTER *pm,int nltbp,LDBPdata ltbp[],double tm[],double th[],double tptw[],double tpti[]);//左路堤边坡
    void Cal_D(double cml,JD_CENTER *pm,int nlzsg,LQSGdata lzsg[],double *D,double *GK,double *GS,double *GP1,double *GP2,double *JQHD,double *GDR);
	int FindWichHD(double cml,JD_CENTER *pm,int NLMHD,LMHdata lmhd[],double HD[3]);
	double FindWichHTHD(double cml,JD_CENTER *pm,int nhthd, hthddata hthd[]);
	void FindWichHTHD(double cml,JD_CENTER *pm,int nhthd, hthddata hthd[],double &returnhthd,int &xz);
    int CrosNew(//输入
					int iroad,//道路号
					int MR,int iCross,
					double Yd,int EarPtSum,double EarPtList[],
					double RHF[10],
					double lmhd[3],
					double InZJ,
					double PJPT[2],
					//输出
					double &deltah,
					double& FillORCut,double& TWH,
					int& DesPtSum,double DesPtList[],
					//填土面积，填石面积，清表土面积,挖方面积
					double& FillArea,double &FillGLTArea,double& QBTArea,double& CutArea,
					//用地宽
					double& ZDK,
					//排水沟性质(1-水沟，0-坡脚墙)，点数,坐标，沟中心x,y,填面积（包括排水沟和天沟），挖面积（包括排水沟和天沟）
					int &IfSetGouBotH,double &GouBotH,int &GouProperty, int &GouPtNum, double gou[],double &GouH,double &GouX,double &GouTarea,double &GouWarea,
					//截水沟点数,坐标
					int &JSGouPtNum, double JSgou[],
					//三沟浆砌面积,挡墙长度,铺草长度
					double &AllGouJQArea,double &DQlength,double &Grasslength,
					double &CG_GDR);
     double CalGouJQArea(double GH,double GW,double SP1,double SP2,double JQHD);
	 void  CalDesArea(int TorW,int DesPtSum, double DesPtList[],int EarPtSum,double EarPtList[],double lmhd[3],double& FillArea,double& CutArea);
	 int CalNewDesline(int TorW,int esPtSum,double DesPtList[],double lmhd[3],int &NewPtNum,double NewDesPtList[]);
	 int Gety(double x,int ptsum,double ptlist[],double &Yd , int &ptnum);
	 int DesBPnew(int NA,double StartDesPt[2],double WM[30],double PH[30],double PTW[30],double PTI[30],
					 int EarPtNum2,int EarPtSum,double EarPtList[],double ljxk,double BPpt[50],int& BPptSum,
					 double ZJ,int bpjs);
	 void BPptToDESpt(int BPptSum,double BPpt[],int &DesPtSum,
						 double DesPtList[],double LTJK,double &BJX,
						 double &BJY,double &ZDK,double &DQlength,double &GrassLength);
	 void CalDesAllArea(int nDespt,double DesPoint[],int NearPt,double earpt[],
						double lmhd[3],double hthd,double glthd,double hjwid,
				        double ZJ,int TorW,
		                double StartDesPt[],double TM[],double TH[],double TPTW[],double TPTI[],int NBPJS,
		                double &FillArea,double &CutArea,double &QBTArea,double &FillGLTArea,double &HJarea);
	 int DesLTGou(double GC,double desh,double HPDK,double HPDI,double GDK,double GS,double SP1,double SP2,double DesPtList[],int DesPtSum,double EarthPt[],int EarPtSum,int &GouPtNum,double GouPt[],double PSG_JQHD);
	 void FormNewDmx(int EarPtSum,double EarPtList[],int DesPtSum,double DesPtList[],int &NewDmxPtNum,double NewDmxPtList[]);
     void CalGouArea(int EarPtSum,double EarPtList[],int npt,double gou[],double& SGFillArea,double& SGCutArea);	
     int  DesLZ(int& npt, double gou[],double GS,double GK,double SP1,double SP2,double DesH_LZ,double LUZLMK,double WM[30],double PH[30],double ZPTW[30],double ZPTI[30],int EarPtNum2,int EarPtSum,double EarPtList[],double LJXK,double DesPtList[],int& DesPtSum,double& FillORCut,double ZJ,double DQinf,double WallType,double WallH,int BPJS,double &DQlength,double &Grasslength);
     double GetDmhp(double sx, double ex, double EarthPt[], int EarPtSum);
     int DesTGou(double TG_BJ,double TG_GW,double TG_GS,double TG_SP1,double TG_SP2,double TG_GDR,
					double DesPtList[],int DesPtSum,double EarPtList[],int EarPtSum,
					int &JSGouPtNum,double JSgou[]);
	 int CALXY(int N1,int NP,int NA,double CC,double C1,double C2,double xh,
		   double *XZ,double *YZ,double PCROS[]);
	 void cal_polygon_area(int npt,double ptlist[],int EarPtSum,double EarPtList[],double &FillArea,double &CutArea);
     int CALPTJD(int N1,int NP,double sptx,double spty,double eptx,double epty,double *XZ,double *YZ,double PCROS[]);
     double CalGLTArea(int TorW,int DesPtSum,double DesPtList[],int EarPtSum,double EarPtList[],double lmhd,double glthd);
     int CalGouOutPt(double offsetX,AcGePoint3d PrePJD,AcGePoint3d PJD,double EarthPt[],int EarPtSum,AcGePoint3d &pt);
	 int CalGouAllPt(int &GouPtNum,double GouPt[],double HPDI,double GS,double GDK,double SP1,double SP2,AcGePoint3d pt,AcGePoint3d PrePJD,AcGePoint3d PJD);//计算水沟各点
	 void CalNewZB(double offsetX,double earh,double Learpt[],int lnpt,double Rearpt[],int rnpt,
			  double &NewYd,double NewLearpt[],int &newLnpt,double NewRearpt[],int &newRnpt);
	 double FindWichDQ(double cml,JD_CENTER *pm,int NDQ,dqdata DQ[]);
     void InitHdmData(int iroad,int j);//设置初值
	 void WriteHdmResFile(int iroad,int jcross,FILE* fpw);
	 void Write3dFile(int iroad, int jcross, FILE* fpw);	//20200516新增	写出左线3d文件
	 void Write3dFile_Roadbed(double MP[3], double Azimuth, xlpoint RoadPZ, int iroad, int jcross, FILE* fpw);	//20200517新增
	 void Write3dFile_Bridge(double MP[3], double Azimuth, int iroad, int jcross, FILE* fpw);	//20200518新增
	 void Write3dFile_Tunnel(double MP[3], double Azimuth, int iroad, int jcross, FILE* fpw);	//20200518新增
	 void WriteFile();
	 int Cal_GouStartPt(double GouStartPt[2],double DesPtList[],int DesPtSum,double ZSDH);
	 int DesLTGou(int ifset,double goubot,double TGK,double ZSDK,double TGH,double GS,double GDK,double ZSDH,double SP1,double SP2,double SP3,double gou[14],double DesPtList[],int DesPtSum,double EarthPt[],int EarPtSum,double lzbp,double &gouH,double &gouX,double &BJX,double PSG_JQHD,double GDR);
	 int DrawCross();
	 void DrawIRoadHdm(int iroad,double sml,double eml);
	 void Cal_CrossOriPt(int iroad,AcGePoint3d StartPt);
	 void CalPjqPt(int zory,AcGePoint3d PJD,AcGePoint3d PrePJD,int &pjqptnum,double PjqPt[]);//---wwq
	 void TGhasJQ(int zory,AcGePoint3d m_JSGouPoints[4],double TG_JQHD,double SP1,double SP2,double GS,double GDR,int &JqPtNum,AcGePoint3d JqPt[],int m_EarthPointNum,AcGePoint3d m_EarthPoints[]);
	 void CGhasJQ(int zory,AcGePoint3d m_DesignPoints[],double CG_JQHD,double SP1,double SP2,double GS,int &JqPtNum,AcGePoint3d JqPt[]);
	 void cal_JQpt(int zory,double jqhd,double SP1,double SP2,double H, double GDR,double gouBotPt[6],int &JqPtNum,AcGePoint3d JqPt[]);
	 void cal_JQpt_CG(int zory,double jqhd,double SP1,double SP2,double H, double gouBotPt[6],int &JqPtNum,AcGePoint3d JqPt[]);
	 void SGhasJQ(int zory,int SGptNum,AcGePoint3d m_SGouPoints[],double SG_JQHD,double SP1,double SP2,double GS,double GDR,int &JqPtNum,AcGePoint3d JqPt[]);
	 void SetDZoomCorner(CROSS pCross,int iroad,int jcross);
	 void SetSZoomCorner(CROSS pCross,int iroad,int jcross);
	 int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	 Adesk::Boolean setCROSSAutoDesignData(CROSS &pCross,int iroad,int k,double Earh,double newLearpt[],int newNumL,double newRearpt[],int newNumR,double newLBenchpt[],int newBenchNumL,double newRBenchpt[],int newBenchNumR);
	 Adesk::Boolean setCROSSAutoDesignData(CROSS &pCross,RoadHdmdata &ZDroad,int k,CRSDAT *LinkRoad,double Earh,double newLearpt[],int newNumL,double newRearpt[],int newNumR);
	 Adesk::Boolean Y_setCROSSAutoDesignData(CROSS &pCross,int k,double Earh);

	 void  SetZxXJJ();//设置主线线间距
	 double  SearchLJFLml(int iroad,double startml,double endml,int &j,bool FrontORBack,int zory,double& sml1,double &zsml1);//true:向前,false:向后
	 void Cal_FLD_Fendml();
	 void SearchBPFLml(RoadHdmdata &ZXroad,RoadHdmdata &ZDroad,double startml,double endml,int &j,bool FrontORBack,int zory,double& sml1,double &zsml1);//true:向前,false:向后
	 void Cal_HdmProperty(double cml,int &duanluo,int &DorS,int &iroad,int &ZxOrYx);
	 void DesACrossOnSingleLine(int i,int j,int zory=0);//只有一个中线的整体式路面
	 void DesZXCrossOnDoubleLM(int j,RoadHdmdata &ZDroad,int zory);
     void CalTWinf(double Earh,double earpt[],int npt,double w,double BG,double &TWinf,double &deltah);
	 int CalBPJD(double XJGK,double XJJ,double Z_RStartPt[2],double Z_RBPpt[],int Z_RBPptSum,
					double Y_LStartPt[2],double Y_LBPpt[],int Y_LBPptSum,
					double ZYd,double YYd,
					double BPJD[]);
	 void DesFLdHdm_ZTtoYT(double XJJ,int iroad,double cml,double projml,
							  double Z_RHFR[],double Y_RHFL[],
							  double LNewYd,int Z_NewNumR,double Z_NewRearpt[],
							  double RNewYd,int Y_NewNumL,double Y_NewLearpt[],
							  double &Z_RTW,double &Y_LTW,
							  double Z_Rlmhd[3],double Y_Llmhd[3],
							  int &Z_Rnpt,double Z_RDesPoint[],
							  int &Y_Lnpt,double Y_LDesPoint[],
							  double &MidFillArea,double &MidCutArea,
							  double &MidQBTArea,double &MidFillGLTArea,double &MidHJarea,
							  double &MidGouJQarea,
							  double &MidDQLength,double &MidCrassLength ,bool &IfXJGou
							  );//设计左填至右填
     void DesFLdHdm_ZWtoYW(double XJJ,int iroad,
							  double cml,double projml,
							  double Z_RHFR[],double Y_RHFL[],
							  double LNewYd,int Z_NewNumR,double Z_NewRearpt[],
							  double RNewYd,int Y_NewNumL,double Y_NewLearpt[],
							  double &Z_RTW,double &Y_LTW,
							  double Z_Rlmhd[3],double Y_Llmhd[3],
							  int &Z_Rnpt,double Z_RDesPoint[],int &Y_Lnpt,double Y_LDesPoint[],
							  double &MidFillArea,double &MidCutArea,
							  double &MidQBTArea,double &MidFillGLTArea,double &MidHJarea,double &MidGouJQarea,
							  double &MidDQLength,double &MidCrassLength,bool&IfXJGou
							  );//设计左挖至右挖
	 void DesFLdHdm_ZTtoYW(double XJJ,int iroad,
							  double cml,double projml,
							  double Z_RHFR[],double Y_RHFL[],
							  double LNewYd,int Z_NewNumR,double Z_NewRearpt[],
							  double RNewYd,int Y_NewNumL,double Y_NewLearpt[],
							  double &Z_RTW,double &Y_LTW,
							  double Z_Rlmhd[3],double Y_Llmhd[3],
							  int &Z_Rnpt,double Z_RDesPoint[],int &Y_Lnpt,double Y_LDesPoint[],
							  double &MidFillArea,double &MidCutArea,
							  double &MidQBTArea,double &MidFillGLTArea,double &MidHJarea,double &MidGouJQarea,
							  double &MidDQLength,double &MidCrassLength,bool&IfXJGou
							  );//设计左填至右挖

	 void DesZXHdm_ON_doubleLM(double XJJ,RoadHdmdata &ZDroad,
							  double cml,double projml,
							  double Z_RHFR[],double Y_RHFL[],
							  double LNewYd,int Z_NewNumR,double Z_NewRearpt[],
							  double RNewYd,int Y_NewNumL,double Y_NewLearpt[],
							  double &Z_RTW,double &Y_LTW,
							  double Z_Rlmhd[3],double Y_Llmhd[3],
							  int &Z_Rnpt,double Z_RDesPoint[],
							  int &Y_Lnpt,double Y_LDesPoint[],
							  double &MidFillArea,double &MidCutArea,
							  double &MidQBTArea,double &MidFillGLTArea,double &MidHJarea,
							  double &MidGouJQarea,
							  double &MidDQLength,double &MidCrassLength ,bool &IfXJGou
							  );//设计左填至右填
							  	 
       void Design_ZX_Hdm();
	   int  Design_FLD_Hdm();//设计绕行段横断面
       double Cal_deltah(double earpt[],int npt,double x,double desh,double Earh);
	   void BiaoZhuQS(int iroad,double cml,AcGePoint3d pos);//桥隧处不画断面时，标注释
	   void Draw_ZX_Hdm(double sml,double eml);
	   void CalFLDDrawStartPt(int iroad,AcGePoint3d &pt);
	   void Draw_FLD_Hdm(int iroad);
	   int GetIRoadByName(CString FLDname);
	   void WriteDetailedAreaFile(int iroad,int jcross,FILE * fpw);

	   void ShowCross();
	   bool ReadHdmResultFile(int i,CString fname=L"");
	   void ReadACrossResultData(FILE *fpr,int iroad , int jcross);
	   bool ReadHdmAreaFile(int i,CString fname=L"");
	   void ReadACrossAreaData(FILE * fpr,int iroad,int jcross);
	   
	   void SelectDM(AcDbObjectId DesId,int selectmode,int &ii0,int &ii1,int &kk);
	   void passCS(DesignLine_ROAD * desline,int bpjs,double m_M[],double m_H[],double m_PTW[]);
	   void ModifySomeBP(AcDbObjectId DesId,int ii0,int ii1,int bpjs,double m_M[],double m_H[],double m_PTW[]);
	   void ChangeBp(int selectmode,AcDbObjectId DesId,double BPJS,double m_M[],double  m_H[],double m_PTW[]);
	   Adesk::Boolean SDM_R_setCROSSAutoDesignData(int iroad,int k,double Earh,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou, EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);	  
	   Adesk::Boolean SDM_L_setCROSSAutoDesignData(int iroad,int k,double Earh,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou, EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);
	   void SDM_SingleLMAutoDesign(int iroad,int j,DesignLine_ROAD *designline,LTGOU_ROAD *LTGou,JSGOU_ROAD * JSGou,EarthLine_ROAD * l_earthline,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);
	   void SDM_AutoDesign(DesignLine_ROAD *designline,EarthLine_ROAD * l_earthline,LTGOU_ROAD * LTGou,JSGOU_ROAD* JSGou,EarthLine_ROAD * r_earthline,HdmText_ROAD *Note);
	   int SDM_CrosNew(//输入
					int iroad,//道路号
					int MR,int iCross,
					double Yd,int EarPtSum,double EarPtList[],
					double RHF[10],
					int nLTBPJS,
					double TM[30],double TH[30],double TPTW[30],double TPTI[30],
					int nLZBPJS,
					double WM[30],double PH[30],double ZPTW[30],double ZPTI[30],
					double PSG_JQHD,double T2W,double T1W,double T2H,double GH,double GW,double HH,double SP1,double SP2,double SP3,double PSG_GDR,
					int IfSetGouBotH,double GouBotH,
					double ParaGK, double ParaGS, double ParaGP1, double ParaGP2, double ParaCG_JQHD, double ParaCG_GDR,	//20190927新增	侧沟参数
					double ParaTG_BJ, double ParaTG_GW, double ParaTG_GS, double ParaTG_SP1, double ParaTG_SP2, double ParaTG_JQHD, double ParaTG_GDR,	//20190928新增	天沟参数
					//输出
					double lmhd[3],
					double &deltah,double& FillORCut,double& TWH,
					int& DesPtSum,double DesPtList[],
					//填土面积，填石面积，清表土面积,挖方面积
					double& FillArea,double &FillGLTArea,double& QBTArea,double& CutArea,
					//用地宽
					double& ZDK,
					//排水沟性质(1-水沟，0-坡脚墙)，点数,坐标，沟中心x,y,填面积（包括排水沟和天沟），挖面积（包括排水沟和天沟）
					int &GouProperty, int &GouPtNum, double gou[],double &GouH,double &GouX,double &GouTarea,double &GouWarea,
					//截水沟点数,坐标
					int &JSGouPtNum, double JSgou[],
					//三沟浆砌面积,挡墙长度,铺草长度
					double &AllGouJQArea,double &DQlength,double &Grasslength);
					
   void DRAWHDMT();
       void TrimEarthLine(int iroad,int j,double ZK,double YK);
	   void Calhiwi(int iroad, int j, double& hi, double& wi, double& Ymin, double& Ymax);
	   void DrawHdmMap(int iroad,double sml,double eml,double zk,double yk,CString TKname,int lnum,int spage,bool ifpage, bool IsShowGuide = true, double TabTextH=5.0,double NoteTextH=4.0);
	   void CreateMapList(int iroad,double sml,double eml,double zk,double yk,int lnum);
	   void DrawAHdmMap(int iroad,int i,Maplist MAP[],double zk,double yk,double TabTextH=5.0,double NoteTextH=4.0);//绘横断面图
	   void OutDwgFile();
	   void WriteHdmZdkFile(int iroad,int jcross,FILE* fpw);
	   void WriteHdmQBTFile(int iroad,int jcross,FILE* fpw);
	   int  FindLTTL(double cml,JD_CENTER *pm,int nldtl,LDTLdata ldtl[]);
	   void FindWFFL(double cml,JD_CENTER *pm,int nwffl,WFFLdata wffl[],double wfsj[6]);
	   void FindGLDMLZ(double cml,JD_CENTER *pm,int ngldmlz,GLDMLZdata gldmlz[],double& dX,double& dH);
	   void FindGLDMLQBPTP(double cml,JD_CENTER *pm,int ngldmlqbptp,GLDMLQBPTPdata gldmlqbptp[],double& dX);
	   void FindGLDMXJG(double cml,JD_CENTER *pm,int ngldmxjg,GLDMXJGdata gldmxjg[],
		   double& GouWide,double& GouHigh,double& NBP,double& WBP,double& JQHD,double& GDR);
	   int FindGouBotH(double cml,JD_CENTER *pm,int NGouPD, GouPDdata GouPD[],double &GouBotH);

	
   void DesZXLinkHdm_ZTtoYW(int i,int j,int iroad,int jcross,
								 double ZX_lmhd[3],
								 double &ZX_TW,double &ZX_TWH,
								 int &ZX_npt,double ZX_DesPoint[],
								 double &MidFillArea,double &MidCutArea,double &ZX_ZDK
								 );//设计左挖至右挖
   void DesLinkCross(int i,int j,int ZxOrYx);								 

   void ModifySomeSG(CString roadname,double Sdml,double Edml,int ZorY,double SGouH,double EGouH);
   void CalNoseCen(JD_CENTER *pm1,JD_CENTER *pm2,double sml1,int QorH,int iroad1,int irod2,double Rnose,
	   double &CmlonZX,double &CmlOnYX,double NosePt[2]);
   int FindIDM(int iroad,double glcml);
   int CalLinkHdmJd(int i,double cml1,double bpc1,int LorR1,int iroad,double cml2,double bpc2,int LorR2,double &ZJ1,double &ZJ2);
   void DesLinkCrossOnDoubleLineFLD(int i,int j);

  void CalLinkHdmSENo(int i,int jcross,int LorR,int desptsum,double DesPt[],int iroad,double &glcml,double &ZJ1,double &ZJ2); 
  int InsertHdm(int izx,int jcross,int iroad,int LorR,double ZJ2,double glcml);
  void SetDmxData(HDMDMXdata &dmx1 ,HDMDMXdata &dmx2);	 
  
  void  SetFLDXJJ();//设置主线线间距和各道路起终点在主线上的投影
  bool CheckPtOnZX(AcGePoint3d InterPt1,AcGePoint3d Spt,AcGePoint3d Ept);
  void CalLinkHdm_TWPJD(int i,int j,int LorR1,int iroad,int jcross);//设计左挖至右挖

//
//	void CalLMK(double cml,int NLMK,LMKdata lmk[],double &half_fgdk,
//					double &lydk,double &cxdk,double &yljk,double &tljk,double &tljhp,
//					int &bkmode,int &sjbgmode);
   bool FindWichBDDml(CString xl1,CString xl2,int qorh,double &xl1dml,double &xl2dml);
   bool FindWichNose(CString xl1,CString xl2,int qorh,double &YljNose);
   bool FindWichHP(CString xl1,CString xl2,int qorh,int &lmhp);
   bool FindWichLinkRoad(CString xl2,int qorh,int &lmhp,CString &mdbname1,CString &roadname1,double &dml1,double &dml2,double &flddml1,double &flddml2);
   bool ReadBDFile();
   bool FindWichNoseAndProperty(CString xl1,CString xl2,double cmlonzd,JD_CENTER *zdpm,int &qorh,double &YljNose,int &TorF,double &FldDmlOnZD,double &FldDmlOnZX);
   void CalZDLMX_JD(JD_CENTER *zxpm,double cmlonzx,JD_CENTER *zdpm,double cmlonzd,double xjj,int LorR,double dis,double pt[2],double &wd);
   void CalBDZDDesH(RoadHdmdata &ZXroad,double zxcml,double &CmlOnFLD,double xjj,RoadHdmdata &ZDroad,double RHFL[10],double RHFR[10],double ZX_RHFL[10],double ZX_RHFR[10]);
   void CalGDPt(double cmlonzd,RoadHdmdata &ZDroad,int izx,double ZX_xjj,double pt[2],double &zxcml);
   double  CalTPtoFPML(double bddmlonzd,int qorh,RoadHdmdata &ZDroad,double xjj);
   double CalZDFPDesH(double x,double y,RoadHdmdata &ZDroad,int izx);
   bool FindWichGLROAD(JD_CENTER *curpm,double cursml,double cureml, GLROAD &ZGLR,GLROAD &YGLR);
   int ReadRoaDFile(JD_CENTER *pm,GLROAD &ZGLR,GLROAD &YGLR);
   bool FindAndReadLinkRoad(CString CurrXL);
   int ReadARoadFile(CString mdbFname, RoadHdmdata &RoadData);
   void InitRoadData(RoadHdmdata &RoadData);
   void SearchBPFLml(RoadHdmdata &ZDroad,double startml,double endml,int &j,bool FrontORBack,int zory,double& sml1,double &zsml1);//true:向前,false:向后
   int CrosNew1(//输入
	   RoadHdmdata &ZDroad,//道路号
	   int MR,double cml,
	   double Yd,int EarPtSum,double EarPtList[],
	   double RHF[10],
	   double lmhd[3],
	   double InZJ,
	   double PJPT[2],
	   //输出
	   double &deltah,
	   double& FillORCut,double& TWH,
	   int& DesPtSum,double DesPtList[],
	   //填土面积，填石面积，清表土面积,挖方面积
	   double& FillArea,double &FillGLTArea,double& QBTArea,double& CutArea,
	   //用地宽
	   double& ZDK,
	   //排水沟性质(1-水沟，0-坡脚墙)，点数,坐标，沟中心x,y,填面积（包括排水沟和天沟），挖面积（包括排水沟和天沟）
	   int &IfSetGouBotH,double &GouBotH,int &GouProperty, int &GouPtNum, double gou[],double &GouH,double &GouX,double &GouTarea,double &GouWarea,
	   //截水沟点数,坐标
	   int &JSGouPtNum, double JSgou[],
	   //三沟浆砌面积,挡墙长度,铺草长度
	   double &AllGouJQArea,double &DQlength,double &Grasslength);	      
   void CalFgdHp(double cml, int NLMK, LMKdata lmk[], double& fgdhp, double& fgdk);
   void CalYljHp(double cml, int NLMK, LMKdata lmk[], double lmhp, double& yljhp);
   int CalLinkRoadInf(double cml, int zory, int& iroad);
   void Design_ZD_Hdm();
   int CalZXxjj(double zxcml, JD_CENTER* zxpm, JD_CENTER* zdpm, double& xjj, double& zdcml);
   void InitHdmData(CRSDAT& hdm);//设置初值
   void CalLinkZDHdm(int i, int jcross, int LorR, int desptsum, double DesPt[], RoadHdmdata& ZDroad, double& glcml, double& ZJ1, double& ZJ2);
   void DesZXHdmLinkZD(int i, int j, int zory, RoadHdmdata& ZDroad);
   void CalLinkHdm_TWPJD(int i, int j, int LorR1, RoadHdmdata& ZDroad, int jcross);//设计左挖至右挖
   int InsertHdm(int izx, int jcross, RoadHdmdata& ZDroad, int LorR, double ZJ2, double glcml);
   void DesACrossOnSingleLine(RoadHdmdata& ZDroad, int j, int zory = 0);//只有一个中线的整体式路面
   int FindIDM(RoadHdmdata& ZDroad, double glcml);

   void Draw_ZD_Hdm(double sml, double eml);
   void SetSZoomCorner(CROSS pCross, RoadHdmdata& ZDroad, int jcross);
   void CalLMKbyFile(double cml, int ZorY, JD_CENTER* pm, BXLMKD* pBXArr, int BxNum, double& half_fgdk, double& lydk, double& cxdk, double& yljk, double& tljk);
   JD_CENTER* GetPm(CString XL);
   void SetHdmLm(RoadHdmdata& CurXL);
   bool FindAndReadLinkRoad_for3d(CString CurrXL);
   void SetLinkHdmData(CRSDAT* link, RoadHdmdata& ZDroad, int jcross);
   //弧形边坡
   int DesArcBp(double R, int EarPtNum2, int EarPtSum, double EarPtList[], double DesPtList[], int& DesPtSum);
   int CalDmxJD(int EarPtNum2, int EarPtSum, double EarPtList[], double Xo, double Yo, double R, double jd[2]);
   double xyddaa(double xb, double yb, double xe, double ye, double* f);

   double FindWichCTK(double cml, JD_CENTER* pm, int NCTKD, ctkddata CTKD[], double& ctwid);
   void FindWichArcBp(double cml, JD_CENTER* pm, int narcbp, arcbpata  arcbp[], double ArcBpR[4]);
   int InsertFLDStartOrEndHdm(int iroad, int StartOrEnd);
   void Cal_HdmDQinf(int iroad, int jcross);


   //计算iRoad号线路连续里程为cml的横断面路面点数据
   void CalHDmLm(int iRoad, double cml, double RHFL[10], double RHFR[10]);

   //是否自动推平，填平	
   bool IsAutoTP(int LDorLQ, int nEarthPtsum, double EarthPtList[], double YLJPt[2], double dYLJPL, double dGK, double JD[2], double& dDist);

   //自动填平路堑设计
   int AutoTPLQDes(double TPdist, int& DesPtSum, double DesPtList[], int EarthPtSum, double EarthPtList[], double GK, double GS, double GP1, double GP2, double GDR, double LQBPPL, double SLPTPL, double& GouH, double& GouX, int& LDSGPtSum, double LDSGPt[]);

   //自动推平路堤设计
   int AutoTPLDDes(double TPdist, int& DesPtSum, double DesPtList[], int EarthPtSum, double EarthPtList[], double& GK, double& GS, double& GP1, double& GP2, double GDR, double SLPTPL, double& GouH, double& GouX, int& LDSGPtSum, double LDSGPt[]);

   //自动开挖台阶设计
   int AutoCutBench(double BenchStartPt[2], int& BenchPtSum, double BenchPtList[], int EarthPtSum, double EarthPtList[], double LMPtList[8], double LMHD[3], double& CutBenchArea);

   //自动开挖台阶
   int HdmDes::AutoCutBench(bool IsPJToLJCen, double BenchStartPt[2], int& BenchPtSum, double BenchPtList[], int EarthPtSum, double EarthPtList[], double StartX, double EndX, double& CutBenchArea);

   //自动设计填方路堑水沟
   int AutoDesTFLQSGHdm(int& DesPtSum, double DesPtList[], int EarthPtSum, double EarthPtList[], double GK, double GS, double GP1, double GP2, double GDR, double SLPTPL, double& GouH, double& GouX, int& LDSGPtSum, double LDSGPt[]);

   //数模内插下标为iRoad道路上下标为iCross横断面地面线
   bool InsertHdmDmxByDTM(int iRoad, int iCross, double cml);

   //数模内插下标为iRoad道路上下标为iCross横断面地面线(匝道)
   bool InsertHdmDmxByDTM(RoadHdmdata& ZDroad, int iCross, double cml);

   //线间沟设计
   void XJGDes(int& DesPtSum, double DesPtList[], double GK, double GS, double GP1, double GP2, double GDR, double& GouH, double& GouX, int& XJGPtSum, double XJGPtArray[]);

   double CalDesHOnFGDBY(double cml, double DesHOnXLZX, int cgNm, CgSZ cg[],
	   int jkNm, CgSZ jk[], int NLMK, LMKdata* LMK, int ZorY,
	   int BxNum, BXLMKD* pBXArr, JD_CENTER* pm);

   void CalFGDBYDesH(double cml, int iRoad, double& DesH, int LOrR);

   void CalOffsetByXLZX(double cml, int NLMK, LMKdata lmk[], double& OffsetByXLZX);

   void SGhasJQ(int zory, int GouPtSum, AcGePoint3d GouPtArray[], double JQHD, int& JqPtSum, AcGePoint3d JqPtArray[]);

   void cal_JQpt(int zory, int LQGorLDG, int GouPtSum, AcGePoint3d GouPtArray[], double JQHD, int& JqPtSum, AcGePoint3d JqPtArray[]);

   void CGhasJQ(int zory, AcGePoint3d m_DesignPoints[], double CG_JQHD, double CG_GDR, int& JqPtSum, AcGePoint3d JqPtArray[]);

   double CalQBTArea(double PJZJ, double hthd, int NearPt, double earpt[]);

   bool FindIsAutoLDTP(double cml, JD_CENTER* pm, int nLDTP, LDTPData LDTP[]);

   bool FindIsAutoLQTP(double cml, JD_CENTER* pm, int nLQTP, LQTPData LQTP[]);

   bool FindIsAutoJKTLJ(double cml, JD_CENTER* pm, int nJKTLJ, TLJJKData JKTLJ[]);

   void InitHdmBench(HDMDMXdata hdmBench[], int NDMX);

   void CreateBenchPtArray(int DesPtSum, double DesPtList[], int EarthPtSum, double EarthPtList[], CArray<Point, Point>& BenchPtArray, double& CutBenchArea);

   //20190811	新增	全自动设计用
   void DesignHdm_AutoDesign();
   //绘制横断面网格线
   void DrawHDMwgx(double EarH, double DesH, double Ymin, double Ymax, AcGePoint3d* m_DesignPoints);

   //绘制改沟横断面	
   void DrawHdmMap_Gutter(int iroad, double sml, double eml, double zk, double yk, CString TKname, int lnum);
   void DrawAHdmMap_Gutter(int iroad, int i, Maplist MAP[], double zk, double yk);//绘横断面图	改沟
   //用于计算水沟填挖与断面整体填挖，边坡绿化面积，水沟长度，路面面积
   void CalTorWforPSandDM(int iroad);
   //计算路面面积
   void CalLMArea(int iroad, double& LMAreaL, double& LMAreaR);
   //计算断面左右边坡绿化面积
   void CalBPArea(int iroad, double& BPAreaL, double& BPAreaR);
   //计算水沟长度
   void CalLengthofPS(int iroad, double& CGLength, double& PSGLength, double& TGLength);
   //水沟浆砌体积计算
   void CalJQvolumeofPS(int iroad, double& CGJQvolume, double& PSGJQvolume, double& TGJQvolume);
   //水沟填挖方计算-是计算体积，不是面积
   void CalVolumeforPSandLJ(int iroad, double& PSVolT, double& PSVolW, double& DMVolT, double& DMVolW);
   //绘制工程数量表格
   void DrawGCSL();
   //绘制图框
   void DrawTK(double OriX, double OriY, double DFACTOR);
   //绘制文字
   void DrawWord(double OriX, double OriY, double DFACTOR);
   //标注文字
   void DrawData(int iroad, double OriX, double OriY, double DFACTOR);

   //输出工程数量表到Excel	20190915新增
  //20200303梁柱
   void WriteQuantityToExcel(int iroad,bool One_excel=false , Worksheets m_ExcelWorksheets=NULL,int sheet_num=0);
   void WriteQuantityToExcel_all(int iroad, bool One_excel = false, Worksheets m_ExcelWorksheets = NULL);
   bool MergeCellInExcel(_Worksheet wsMysheet, Range rgMyRge, long sLine, long eLine, CString sCol, CString eCol);	//合并单元格

   //计算排版	20190903新增
   void CalMapList(int iroad, double StartMl, double EndMl, double LEarthWid, double REarthWid, int ColNum);
   //绘制网格线
   void DrawCrossNet(double DesH, double Xmin, double Xmax, double Ymin, double Ymax, AcGePoint3d* m_DesignPoints, double& CenterY);

   bool CheckLateralGutterExist(CRSDAT& OneCross, int LorR);

   //计算超高	20190906新增
   void CalSup(double cml, int ncg, CgSZ cg[], double& LMHP, double& YLJHP);

   //修正横断面数组	20190911新增
   void ModifyCrossArray(int iroad);
   //横断面面布局出图
   void DrawHDMOutput(double CenX,double CenY, ACHAR s_ckml[], ACHAR e_ckml[], CString TKname);
   void TransFormTK_HDM(STuKuang *TK, double CenX, double CenY, double scale = 1.0);

   int SimpDesCrossForOpt();//横断面简化设计（优化）
   int Cal_TSFJSBForOpt(int StartIndex, int EndIndex, double &FillSUM, double &CutSUM);
	//删除横断面水沟
   void DeleteGutter(AcDbObjectId EntID);
   public:
	   // 是否绘制横断面网格
	   BOOL m_IsDrawHengDuanMianWangGe;
} ;	

#endif // !defined(AFX_HDMDES_H__0AF46E78_FFC1_44CB_973A_5114CAB81AB9__INCLUDED_)
