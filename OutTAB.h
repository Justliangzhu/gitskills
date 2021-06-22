// OutTAB.h: interface for the OutTAB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTTAB_H__1F25AE06_0AB2_4BC4_88A9_2A2F1000F0CC__INCLUDED_)
#define AFX_OUTTAB_H__1F25AE06_0AB2_4BC4_88A9_2A2F1000F0CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "excel9.h"	// Added by ClassView
//#include "excel8.h"
#include "BAS_DRAW_FUN.h"

class OutTAB  : public BAS_DRAW_FUN
{
public:
	struct LJSJBdata
	{
		ACHAR ckml[80];
		double cml;
		double dmgc;
		double DeshNoVC,dh,DeshHasVC;
		double twh;
		double Zljwid,Yljwid;
		double Zljdh,Zcxddh,Mdh,Yljdh,Ycxddh;
		double SGtwh;
		double BL,BR;
		int ZDMXZ,YDMXZ;
	};

	struct TSFoutput
	{
	   ACHAR ckml[80];
	   bool IfBriTunEnd,IfBriTunStart;
       double cml,CutAreaSum,FillSoilArea,FillRockArea,dis;
	   double CutVolSum,CutVol[6],CutPer[6];
	   double FillVolSum;
	   double BZLY[6];//本桩利用
	   double TQ;//填缺
	   double BZWYF[6],BzwyfSoilSum,BzwyfRockSum;//本桩挖余方
	} *CrsData;

	struct KMTSFdata
	{
	  ACHAR KSml[80],KEml[80];
	  double KmCutVol[6],DisSum;
	  double KmFillVolSum,KmCutVolSum;
	} ;
	int TSF_Ncrs;

	struct LXTWdata
	{
		ACHAR ckml[80];
		double cml;
		double ZTWinf,RTWinf;
		double ZTWH,RTWH;//左、右坡脚至路肩填挖高
		double ZBPXC,RBPXC;//左、右边坡斜长
        double Wlen,Wjj,Warea;
	} *LXTW;
	int NLXTW;

	struct BGPSGdata
	{
		ACHAR ckml[80];
		double cml;
		double ZLJBYDesh,YLJBYDesh;//左、右路基边缘设计标高
		double ZPJDesh,YPJDesh;//左、右坡口、脚标高
		double ZPJWid,YPJWid;//左、右坡口、脚平面宽度
        double ZSLTWid,YSLTWid;//左、右碎落台宽度
		double ZGouToCenWid,YGouToCenWid;//左、右中心至路中心距离
		double ZGouDesh,YGouDesh;//左、右沟底标高
		double ZGouHt,YGouHt;//左、右沟  深
	} *BGPSG;
	int NBGPSG;

	struct YdByKm
	{
		double zarea;
		double yarea;
		ACHAR lcfw[40];
	} *pYdByKm;
	int NYdByKm;

	struct  QBTnode{
		double cml,Aqbt,Awyn;
		} ;
	
	OutTAB();
	virtual ~OutTAB();
	void ExcelLoad();
	void OUT_BGPSGSJB();
	void FillBgpsg(CString RoadStr,CString DuanName,int Spage,int PageSum);
	void Cal_BGPSG(CString PrjMdb,CString RoadName,double Sdml,double Edml); //生成边沟排水沟数据表
	void OUT_YDBYKM();
	void OUT_ZZZB();
	void FillZzzb(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	void OUT_ZXQXZJ();
	void FillZxqxzj(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	void OUT_ZPSQX();
	void FillzpsqxByXZ(JD_CENTER *pm,CString PrjMdb,CString RoadName,double Sdml,double Edml,CString DuanName,int Spage,int PageSum,CString XZ,long &shnum);
	void Fillzpsqx(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	void OUT_ZZYDZB();
	void OUT_ZZYDZBMJ();
	void Fillzzydzb(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);

     void OUT_LJSJB();
	 bool ispage(ACHAR *str1,ACHAR *str2);
	 bool glzhuang(ACHAR *str);
	void Fillljdatab(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	double CalDesH(double cml,GTZDM_ROAD *ZM1,GTZDM_ROAD *AllZM);
	GTZDM_ROAD *ReadZm(CString mdbname,CString RoadName,CString XZ,JD_CENTER *pm);
	void Fillljsjb(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
    double CalDesHWithoutVerCurve(double cml,GTZDM_ROAD *ZM1,GTZDM_ROAD *AllZM);
	//void ReadACrossResultData(FILE *fpr,JD_CENTER *pm,LJSJBdata &ljdata,GTZDM *allzm,GTZDM *zzm,GTZDM *yzm);
	int ReadACrossResultData(FILE *fpr,JD_CENTER *pm,LJSJBdata &ljdata,GTZDM_ROAD *allzm,GTZDM_ROAD *zzm,GTZDM_ROAD *yzm,int ZorY,int &IsInserthdm);
//---------------------------------------------------
    int TranCmlToRow(JD_CENTER *pm,double fsml,ACHAR ckml[50][80],int nRow);
    float RowsToFloat(int Rows,_Worksheet wsMysheet);
	void DrawPm(JD_CENTER *pm,ACHAR CKSML[256],ACHAR CKEML[256],ACHAR ckml[50][80],int nRow,_Worksheet wsMysheet);
	void DrawText(long orient,float left,float top,float width,
					   float height,long fontsize,Shapes &mShapes,ACHAR *str);
	_Application m_app;
	Workbooks    m_workbooks;
	Worksheets   m_worksheets;
	_Workbook    m_workbook;
	_Worksheet   m_worksheet;
    Sheets       m_sheets;
	int FindWichPd(double cml,GTZDM_ROAD *zm);
	void cmltobmstr(JD_CENTER *pm,double cml, ACHAR bmstr[]);
	void DrawZm(JD_CENTER *pm,GTZDM_ROAD *zm,ACHAR CKSML[256],ACHAR CKEML[256],ACHAR ckml[50][80],int nRow,_Worksheet wsMysheet);

	void OUT_TSFJSB(); 
    bool Cal_TSFJSB(CString PrjMdb,CString RoadName,double Sdml,double Edml); 
	void FillLjtsfjs(CString RoadStr,CString DuanName,int Spage,int PageSum);

	void CalAllTSF(CString PrjMdb,CString RoadName,double Sdml,double Edml, double& FillVolSum, double& CutVolSum);//计算土石方总量
	void OUT_MGLTSFB();
	void Cal_MGLTSFB(CString PrjMdb,CString RoadName,double Sdml,double Edml,int &KMnum,KMTSFdata KMdata[500]); //生成每公里土石方数量计算表
	void Fillmgltsfsl(CString RoadStr,CString DuanName,int Spage,int PageSum,int KMnum,KMTSFdata KMdata[500]);

	void OUT_LXTWB(); 
	void FillLxtw(CString RoadStr,CString DuanName,int Spage,int PageSum);
	void Cal_LXTW(CString PrjMdb,CString RoadName,double Sdml,double Edml); //生成路线填挖数据表
	double Cal_LmWfLen(int NumPt,double EarPt[],double wd,int DesNpt,double DESPT[10]);
	double Cal_EarY(double x,int EarNpt,double EARPT[]);
	double Cal_DesY(double x,int DesNpt,double DESPT[]);

	void CalZX(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	void CalFX(CString PrjMdb,CString RoadName,CString RoadStr,double Sdml,double Edml,CString DuanName,int Spage,int PageSum);
	void CalPZXPT(JD_CENTER *pm,double cml,double jd,double dis,double pt[2],double &prjml);
	void CalPFXPT(JD_CENTER *pm,double cml,int ZorY,double distozx,double jd,double dis,double pt[2],double &prjml);
	double Callmhp(double ml);

	void OUT_QBTB();
	void OUT_WYNB();//挖淤泥表
	double Cal_QBTVol(double sml,double eml,int ncrs,QBTnode crs[],int IFQB,double &len);
	bool CheckIfInSta(double cml,int NSTA,	LCBdata STA[]);

	void CalNewZB(double offsetX,double earh,double Learpt[],int lnpt,double Rearpt[],int rnpt,
					  double &NewYd,double NewLearpt[],int &newLnpt,double NewRearpt[],int &newRnpt);

	int Gety(double x,int ptsum,double ptlist[],double &Yd , int &ptnum);
};

#endif // !defined(AFX_OUTTAB_H__1F25AE06_0AB2_4BC4_88A9_2A2F1000F0CC__INCLUDED_)
