// calcg.h: interface for the calcg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALCG_H__C6B976E9_48DA_471C_8323_553F786568D7__INCLUDED_)
#define AFX_CALCG_H__C6B976E9_48DA_471C_8323_553F786568D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JD_CENTER.h"
#include "XLDataBase.h"

class calcg  : public BAS_DRAW_FUN
{
public:	

    JD_CENTER *pm;
   	int NZLMK,NYLMK;
	LMKdata *ZLMK,*YLMK;
	double JKfactor;//车道(加硬路肩)宽度超过7.50m  JKfactor=1 else JKfactor=0.5
	double ZXCGJBL;//直线段超高时超高渐变率
	double CGHXZXJBL;///缓和段最小超高渐变率1/330
	int ZXCGLenMode;//直线段超高时的长度取整0-不取整数，1-1m的整倍数 ,2-5m的整倍数
	int SuphMode;
	int Vj;
	int NSUPH,NJK;
	double SuphData[10][2],JKData[10][2];
	

 //Fend[][0]---性质（0--直线，1--缓和曲线，2--公切线，3--缓和曲线要计算
  //            方向改变加点号）
  //Fend[][1]---CML分段终点连续里程
  //Fend[][2]---X分段终点坐标
  //Fend[][3]---Y
  //Fend[][4]---FWJ分段终点沿线路方向的方位角
  //Fend[][5]---计算参照点的大地x坐标
  //Fend[][6]---计算参照点的大地y坐标
  //Fend[][7]---计算参照点的方位角
  //Fend[][8]---计算参照点的连续里程
  //Fend[][9]---该段的半径或A值

	//private:
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

 
 CgSZ Zcg[1000],Ycg[1000],Zjk[1000],Yjk[1000];
 int ZcgNm,YcgNm,ZjkNm,YjkNm;

//读文件函数

int indata();
int read_JDfile_new();
void ReadPMfile();
void ReadWKdir();
int ReadSGFile();
int Read_HdmFile();
//内存

void  freeroom();
//检索函数
double FindSuph(double R);
double FindJK(double R);
int FindIlmk(double cml,LMKdata lmk[],int nlmk);
//其它
double AdjustLc(double lc);
void SetSuphData(int ZorY,struct CgSZ *Cg,int& cgnm,struct CgSZ *Jk,int& jknm);
double cal_B(double Zjdw , int suph_mode , double Cxdw , double Yljw , double Tljw);
void Cal_Suph(); //生成纵断面图所需超高数据
void SetCirCg(int ZorR, int i, double Supi, struct CgSZ *Cg,int& cgnm);
void SetFirstCirCg(int ZorR, int i, double Ig, struct CgSZ *Cg,int& cgnm);
void SetSpiralCg(int ZorR, int qorh, double B, double Supi,int i, double Ig, double IgOpp, struct CgSZ *Cg,int& cgnm);
void CalZYLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm);
void CalYZLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm);
void CalYZYLinkOnZX(int i,double Ig,double IgOpp,double B,int ZorR, struct CgSZ *Cg,int &cgnm,struct CgSZ *Jk,int &jknm);
void JKCalYZYLinkOnZX(int i,int ZorR,struct CgSZ *Jk,int &jknm);
calcg(JD_CENTER *pline,double zxcgjbl,double cghhlen,int mode,int suphmode);
void AdjustCGByZX1(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[]);
void AdjustCGByZX(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[]);
void AdjustCGByZX_FX(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,double fldlen,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[]);
void AdjustCGByZX_FX1(JD_CENTER *zxpm,JD_CENTER *pm,double ZXdml,double ZDdml,double fldlen,int &NZCG,CgSZ ZCG[],int &NYCG,CgSZ YCG[]);
void CheckIfMlOver(CgSZ *Cg,int cgnm);
	virtual ~calcg();
};

#endif // !defined(AFX_CALCG_H__C6B976E9_48DA_471C_8323_553F786568D7__INCLUDED_)
