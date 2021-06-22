// ZDMHT1.h: interface for the CZDMHT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZDMHT1_H__3F377F1B_33BD_4306_91AD_ECBB93267087__INCLUDED_)
#define AFX_ZDMHT1_H__3F377F1B_33BD_4306_91AD_ECBB93267087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BAS_DRAW_FUN.H"
#include "JD_CENTER.H"
#include "XLDataBase.h"
#include "ado.h"

class CZDMHT :public BAS_DRAW_FUN 
{
public:
	int WayorRamp;//0公路 1 匝道
	int DrawMode;//0 初步 1施工
	bool InfLmPt[8];//路面8点的高程标注
	bool infwgx;//是否绘制网格线
	bool isdrawctrh;//是否绘制网格线
	CString LmPtName[8];//路面8点的名称
	bool IsFirst;//是否第一副图
	double  X_scale, Y_scale;
	double  X_inori, Y_inori;//内框左下角
	double Xd_ori;//绘图起始x
	double  X_inmax, Y_inmax;//内框右上角
	double NTKLen, NTKHt;//内图框长,高
	double WTKLen, WTKHt;//外图框长,高
	double TopFW;//纵面图上部绘竖曲线范围
	double m_Scml, m_Ecml;//该幅纵面图起终连续里程
	double m_Sdml, m_Edml;//该幅纵面图起终现场里程
	CString m_OutName;
	int CurTNo;//当前页 页总数
	double X_inori_Buju, Y_inori_Buju;
	CString m_TKName;//图框名
	CString m_dzgkname;//地质概况文件名
	CString SGH,EGH;
	JD_CENTER *m_pm;
	double BREAK[1000][2];//[0]断轴里程，[1]断轴高程 添加终点位置断轴记录终点里程
	double SDAI[100][3];//处理断链，0 断链处留的长度 1断链处的屏幕坐标 已经+断链处留的长度
	int NBREAK,NSDAI;  // 2 断链处连续里程
	int WXPnum;
	XLDataBase *pDBS;
	XLDataBase *pZDBS;
	XLDataBase *pYDBS;
	XLDataBase *BxDBS[10];//最多10个边线地面线
	int NBxDmx;
	double m_Vertical_Curve_Y;	//20190711添加	为确保竖曲线标注能在
	double m_Frame_Center_X, m_Frame_Center_Y;	//20190718添加	记录绘图中心坐标
	bool m_IsNoteScale;	//是否标注比例尺（在图形左侧）	20190829
	DrawProfilePara m_VDrawPara;	//纵断面绘图参数
	int m_Isbuju_ZDM;
	CgSZ *ZCG,*YCG;
	int NZCG,NYCG;
	CgSZ *ZJK,*YJK;
	int NZJK,NYJK;
	LMKdata *ZLMK,*YLMK;
	int NZLMK,NYLMK;
	int BxNum;
	BXLMKD *pBXArr;
	bool m_bzym;
	
	int KNum;
	struct brginf{
		int ks;
		double klen;
	} BrgArray[100];

	 struct BriLSEml
	 {
		   double sml;//该梁的起始里程	
	  } BrgLArr[1000];//梁号对应数组下标
	  int nBriL;

	WXP wxp[20];
	//std::vector<GCKZPt> m_LevelCtrlPtArray;
	double GWBotH;
	CZDMHT();	
	virtual ~CZDMHT();	  
	void FormSdai();//生成Sdai数组同时生成里程桩数组
	
	void DrawZDM(double YbreakTextH=5.0,double DefTextH=5.0);//初始化 绘纵断面
	void draw_box();
	double mltosx(int mess, double cml) const;//mess=0 绘制断链前 1绘制断链后
	double ztosy(double cml,double mz);
	void  Y_break();//计算断轴
	double CalDmxH(double cml);//计算断轴根据地面线
	double des_level (double cml,XLDataBase *pDBStmp);//不考虑竖曲线计算设计高程
	void  axleSg(double Texth=5.0);//绘制施工断轴 
	void  axleCb();//绘制断轴 单线
	void  draw_des_slop(XLDataBase *pDBStmp,double DefTextH=5.0);//绘坡度线    
	void Draw_Des_VerCurve(int sno,int eno,XLDataBase *pDBStmp);
	void DrawVerCurLine(double sml,double eml,int Color,CString LayerName,double LWid=0.0,double Len=10,double Zoff=0,XLDataBase *pDBStmp=NULL);//绘sml到eml的坡度线 考虑竖曲线，断轴
	//Zoff表示离坡度线距离 负：在坡度线下
	double rdes_level (double cml,XLDataBase *pDBStmp);//考虑竖曲线计算设计高程
	double DLEVEL(int icm, double cml,XLDataBase *pDBStmp);
	void  draw_earthline();//绘地面线
	void  draw_bxearthline();//绘边线地面线 
	void draw_vertical_factors(int wxpi = -1);//绘制竖曲线
	void DrawLCZH_Sg(int wxpi);//绘制施工图里程桩号
	void DrawLCZH_Cb(int wxpi);//绘制初步图里程桩号
	void DrawBox_Desh(int wxpi);//绘制施工图设计高程
	void DrawBox_DMGC(int wxpi);//绘制地面高程
	void DrawBox_SLOP(int wxpi);//绘制栏目中坡度
	void DrawBox_PLAN(int wxpi);//绘制栏目中平面
	void DrawBox_Cg(int wxpi);//绘制栏目中超高
	void DrawBox_DZGK(int wxpi);//绘制栏目中地质概况
	void DrawTK();
	//【高程里程坐标系】转【CAD坐标系】
	AcGePoint2d Data_realtodraw(AcGePoint2d pt);
	//20190709	增加CAD绘图坐标系坐标与里程高程坐标系转换函数
	double Trs_Coor_System(double Ori_Para, int Mile_Level_Flag, int Trs_Flag) const;
	void DrawDZQ();//上大中桥
	void DrawTun();//上隧道
	void GetCmlKKSY(int ibri);
	int F_num(ACHAR *str, ACHAR c);
	void draw_dai();
	void WriteBlock();
	void DrawCulvet();//绘制小桥涵
	void draw_circle(double sx, double sy);
	void draw_culvert(double sx, double sy);
	void draw_boxcult(double sx, double sy);
	void draw_arc(double sx, double sy);
	void draw_cross(double sx, double sy);
    void GrdrawDesL(double xcml, double zori, int sbk);
	void GrdrawEarth(double xcml,double zori,int sbk);
	void  AddAxle(ads_point PT);//增加断轴
	void  DelAxle(ads_point PT);//删除断轴
	void  AjustAxle(ads_point PT);//调整断轴
	double sxtocml(double sx);
	void ReadLMK(CString LMKTABname);
	void DrawBox_TWH(int wxpi);
	void DrawScale();	//标注比例尺	在图形左侧	20190829
	//是否绘制该里程
	bool isBiaoZhuGaiLiCheng(double cml_Start,double cml_End,double cml,double LiChengBuChang = 20.0);
	//20190821新增	绘制延长线
	void DrawExtendLine(XLDataBase* pDBStmp, double PreLen, double AftLen);
	//20190829新增	检测是否是特征桩
	bool IsFeaturePile(double Cml);
	//20190910新增	绘制网格
	void Draw_Grid() const;
	void Draw_CtrH();
	void DrawZDMOutput();//纵面布局出图
	void TransFormTK_ZM(STuKuang *TK, double CenX, double CenY, double scale = 1.0);
};

#endif // !defined(AFX_ZDMHT1_H__3F377F1B_33BD_4306_91AD_ECBB93267087__INCLUDED_)
