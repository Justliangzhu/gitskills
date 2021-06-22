// GTZDM.h: interface for the GTZDM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZDM_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_)
#define AFX_GTZDM_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
//  #include <dbapserv.h>
#include <adslib.h>
#include <dbxrecrd.h>

#include <string.h>
#include <dbents.h>
#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 
#include <dbjig.h>
#include <acgi.h>
#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN.H"
#include "JD_CENTER.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
//#define AppName  /*MSG1*/"GUADS"    


//		自定义类    
//   DLB[j]--断链表（含冠号）
//   PDB[k][]--坡度表  
class GTZDM_ROAD : public AcDbEntity,public BAS_DRAW_FUN//坡度类
{ 
public:
    ACRX_DECLARE_MEMBERS(GTZDM_ROAD);
	//*************************************成员变量************************************
	//**********1.设计标准变量**********
	double A_JXRshuMin;	//凹形极限= _T("4000");
	double A_YBRshu;	// 凹形一般= _T("6000");
	double MAXPD;		// 最大纵坡_T("3");
	double T_JXRShu;	// 凸形极限_T("11000");
	double T_YBRshuMin;	//凸形一般 _T("17000");
	double YBLShuMin;	//竖曲线长度一般 _T("250");
	double YBPCMin;		//坡长一般_T("400");
	double JXPCMin;		//坡长极限 _T("300");
	double JXLRShuMin;	//竖曲线长度极限 _T("100");
	double DV;			//车速度
	double XZPD, XZPD1;	//限制坡度
	double DFXYXC;		//到发线有效长
	double PDLmin;		//最小坡段长
	CString TLDJ, QYZL;	//铁路等级,牵引种类
	double PDDSC1, PDDSC2;	//坡度代数差一般，困难

	//**********2.设计变量**********
	//坡度相关变量
	double BPD_array[MAXBPDNUM][4];  //[][0]=//图上坐标
	double BPDYcor[MAXBPDNUM];//竖曲线曲中点高程  图上坐标
	double PDarray[MAXBPDNUM][4];//实际里程坐标
	Adesk::Int16 NBPD;	//变坡点数组大小
	int NCM;
	double CM[5 * MAXBPDNUM];
	//断链相关变量
	int NH, NLC; //NDL 断链点数（包括里程冠号改变点，如CK12+235=C1K12+235，为断链值是0的特殊断链点）
	// NLC--里程保留位数》=0  NH--高程保留位数》=0
	Adesk::Int16 DLNum;
	//起点为特殊断链，终点无断链，全线至少一个断链（起点）
	//里程冠号至多为8个字符
	CArray<DLBZ, DLBZ>DLArray;
	double  ZDMDLBZ[MAXDLNUM][2];  // 断链点处 DLBZ[i][0] 统一X坐标 
	//控制标高数组
	std::vector<GCKZPt> m_LevelCtrlPtArray;
	//接坡信息数组
	std::vector<JPInfor>m_JPInforArray_slope;
	// DLBZ[i][1]该断链点处 前100m=50—150m 当后链为整100  或
	//                     后100m=50—150m 当前链为整100 而后链不为整100 	
	int m_Frame_Index;	//图框号	20191023添加

	//**********3.绘图变量**********
	double HBVSCALE;	//横纵向比例
	double texth;		//字高
	double VSCALE, HSCALE, K0, X0;	//高程比例  水平比例  基点里程  基点对应图上X坐标	
	double PMY0, BMLCY0, DMLCY0, DMBGY0, PDY0, SJBGY0, H0, Y0;	// 起点   平面  百米标  地面里程 地面里程 坡度  坡度高程 基点高程  基点对应图上Y坐标
	double  EK, Lmin;	// 终点里程        竖曲线最短长度m 
	int GLbz;			//绘公路 =1 铁路=2
	double h_plan, h_ml, h_addml, h_addh, h_prof, h_geolo, h_earth;//栏高
	double QZZ;			//QZZ变坡点里程取整位，
	CString  Property;	//性质 -1 左幅 0全幅 1右幅
	int Xsw;			//Xsw坡度小数位数
	int DesHXSW;		//设计标高小数位数
	int LJorGM;			//路肩坡或轨面坡 0-路肩坡 1-轨面坡
	int TDFS;			//拖动方式 1:任意拖动  2:前坡度不变拖动  3:后坡度不变拖动
	int VerRadius;		//竖曲线步长 0 无限制 1 1米倍数 2 10米 2 100米
	int m_Is_JP;        //是否考虑接坡约束 0->不考虑 1->考虑
	         
	//**********99.暂时废弃变量**********
	//    double DLBZ[MAXDLNUM][2];  // 断链点处 DLBZ[i][0] 统一X坐标 
	// DLBZ[i][1]该断链点处 前100m=50—150m 当后链为整100  或
	// 后100m=50—150m 当前链为整100 而后链不为整100      
	// 现场[K0,H0]===纵面图上(X0,Y0)


	//*************************************成员函数************************************
	//**********1.构造、析构函数**********
	//构造函数
	GTZDM_ROAD();
	GTZDM_ROAD(int iNBPD, double iBPD_array[MAXBPDNUM][4], ACHAR *GH, int type = 1, int desh_xsw = 2);
	//析构函数
	virtual ~GTZDM_ROAD();

	//**********2.CAD sub系列函数**********
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;

	virtual void subList() const;
	virtual Acad::ErrorStatus  subExplode(AcDbVoidPtrArray& entityset)const;
	//  virtual AcRxObject* clone() const;
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

	Acad::ErrorStatus subGetTransformedCopy(const AcGeMatrix3d& mat, AcDbEntity*& ent) const;
	/*
	virtual Acad::ErrorStatus getStretchPoints(AcGePoint3dArray& stretchPoints)const;
	virtual Acad::ErrorStatus moveStretchPointsAt(const AcDbIntArray& indices,
	const AcGeVector3d& offset);
	*/
	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);

	//**********3.坡度计算相关函数**********
	//****①变坡点处理****
	//删除变坡点
	Adesk::Boolean DelBPD(int BPDi);
	//增加变坡点
	Adesk::Boolean addBPD(int BPDi, double R, double XLC1, double HY);
	//设置变坡点
	void setBPD(int BPDi, double Ri, double XLC, double HY);
	//获取变坡点
	int getBPD(int BPDi, double *Ri, double *XLCi, double *HYi);
	//保存变坡点
	int saveBPD(ACHAR *fname);
	//调整变坡点的现场里程为整dlcr
	void AdjustBPD(int dlc);
	//调整坡度	mode=0顺延，1-不顺延
	void AdjustPD(int ipd, double pd, int mode);
	//****②计算高程****
	//计算设计高程（不考虑竖曲线）
	double rdes_levelWithoutVerCurve(double cml);
	//计算设计高程
	double rdes_level(double cml);
	//计算设计高程
	double DLEVEL(int icm, double cml);
	//****③其他计算函数****
	//找出最小坡段长
	void FindPDLenMin();
	//数组转换
	void BpdToCM();
	void PDdataToCM(int &NCM, double CM[]);
	//计算切线长
	double CalT(int icm);
	//检测竖曲线是否穿袖子，并修正曲线半径R
	int CheckVerR(int ii, double &R);
	//限制接坡起终坡度
	void LimitJPSlope_StEd();
	//**********4.平面计算相关函数**********
	//计算统一里程
	double TYLC(double dml);//szf
	//修改指定断链
	void setDLM(int iDL, double BLC, double EDL, CString BGH, CString EGH);
	void addDLM(int iDL, double BLC, double DL, CString EGH);
	void setDLB(DLBZ *dlb, int ndl);
	//删除指定断链
	void DelDLM(int iDL);
	//求现场里程
	CString XLC(double cml);
	//求现场里程
	double XLC(double TYLC, CString& GH);
	double XLC1(double TYLC);//szf
	//计算断链统一里程
	void GetDLTylc();

	//**********5.绘图相关函数**********
	//【CAD坐标系】转【高程里程坐标系】
	AcGePoint2d Data_drawtoreal(AcGePoint2d pt);
	//【高程里程坐标系】转【CAD坐标系】
	AcGePoint2d Data_realtodraw(AcGePoint2d pt);
	//设置初始绘图参数
	void setINIT(int nlc, int nh, int Chinese_Engbz, double Texth, double HBSCALE, double XX0 = 100, double YY0 = 100, double KK0 = 10000, double HH0 = 100);
	//获取基点、里程、高程、横纵比例尺信息
	int getK0H0(double *XX0, double *YY0, double *KK0, double *HH0, double *hscale);
	//整个数组从【CAD坐标系】转【高程里程坐标系】
	void ScreenToReal();
	//20190709	增加CAD绘图坐标系坐标与里程高程坐标系转换函数
	double Trs_Coor_System(double Ori_Para, int Mile_Level_Flag, int Trs_Flag) const;
	//画线、画圆函数
	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor);
	// 	G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, char* ctext,double ang,double texth ,int icolor,int mode);
	Adesk::Boolean G_makearc(AcGiWorldDraw* pWd, AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx);
	//标注变坡点（自定义实体）
	Adesk::Boolean BZBPD(AcGiWorldDraw* pWd);	
	//标注变坡点（CAD实体）
	Adesk::Boolean BZBPD1(double  BPD_array[MAXBPDNUM][4], int NBPD, double CSB[6], int NLC, int GLbz);
	//绘制竖曲线
	void DrawVerCurLine(double sml, double eml, int Color, AcGiWorldDraw* pWd);	
	//设置文字样式
	int textStyle();
	//绘制控制点标高
	void DrawLevelCtrlPt(AcGiWorldDraw* pWd);
	
	//**********6.其他函数**********
	//两个值比大小
	double CALMAX(double A, double B);
	double CALMIN(double A, double B);
    //规范检测
	void checkzm();
	void checkzm(CString mdbname, CString roadname, CString XZ, CStringArray &ErrMesArr);
	//坡度折减
	void pdzj(double D1, double D2, double *U1, double *U2);
	double pdzj_Tun(double BLC, double ELC);
	double GetTunZJXS(double tunlen, CString qyzl);
	//计算填挖表
	void CalTWB(int NCM, double CM[], double cml, double &DMGC, double &SJGC, double &TWH, double &SJPD, double &LJKD, double &SQXZJ, double &LJSGG, double &ZXTJ);
	//电力牵引的小曲线半径折减
	void LRADCON(double R, double BIP, double CIP, double *U3, double *U4);
	//初始化函数 
	void INIT();	
	
private:
	AcDbObjectId ZPId;	//???
	void VIEW_ZM_TypeHdm();	//???
    //int draw_ve();
    //int dr_cm(int i);
};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZDM_ROAD);
#endif // !defined(AFX_GTZDM_H__9AC3E47D_CCC0_49E4_A6C2_B394B3CA42AB__INCLUDED_)
