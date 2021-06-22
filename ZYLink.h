// ZYLink.h: interface for the ZYLink class.
//
//////////////////////////////////////////////////////////////////////
#define Pai 3.14159265358979323846264338327
#if !defined(AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_)
#define AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SPIRAL.h"
#include "JD_CENTER.h"
//ֱ�ߺ�Բ����ͨ�������������Ӽ�����
class ZYLink:public BAS_DRAW_FUN 
{
public:
	//����ֱ�ߵ������˵����꾭γ�࣬Բ���ߵ�Բ�����ꡢ�뾶���������ߵ�ת����תΪ����
	void Set(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr);
public:
	//��������
	SPIRAL pSPIRAL;
	//�Ƿ�����޺��ʵĻ������ߣ�
	bool ERRFlag;
private:
	//����ת����1���ң�1
	int LeftOrRight;
	double dR,R,A;
	//�����������յ�����
	double BaseN,BaseE,EndN,EndE;
	//�����������յ㷽λ��
	double EndFwj,BaseFwj;
	//��������
	int count;
private:
	double func(double x);
	double diedai(double AA1, double AA2,int flag);
	double AtodR(double AA);			
public:
	//�ɰ뾶��Aֵ���㻺�����ߵ�ת�Ǻ�Բ�ĵ�ֱ�ߵľ���
	static double DistCenToQX(double r,double a,double &afa);
	ZYLink(AcDbLine *pLine,AcDbArc *pArc,int lr=0);
	ZYLink(double N1,double E1,double fwj,double cenN,double cenE,double r,int lr);
	ZYLink(double N1,double E1,double N2,double E2,double cenN,double cenE,double r,int lr);
	ZYLink();
	~ZYLink();
};

#endif // !defined(AFX_ZYLINK_H__137D7A83_84B5_4356_8ADF_E980209B83CD__INCLUDED_)
/*
bool drawmode;
bool ERRFlag;
void GetSpirItem(int i, double &AA, double &RR, AcGePoint3d &sPt, double &fwj, double &slc, int &lr,AcGePoint3d &ePt);
void getSpiralId(AcDbObjectId& Id1);
int count;
double jifen_sin(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s);
double jifen_cos(double afa1,int pianxiang,double Ls,double Rt,double start_s,double end_s);
double diedai(double AA1,double AA2,int flag);
double afa_s(double afa1,int pianxiang,double s,double Ls,double Rt);
ZYLink(AcDbLine *pL1,AcDbArc *pArc,int lr=0);
ZYLink(double X0,double Y0,double fwj,double CenX,double CenY,double RR,int lr=0);

ZYLink(AcDbLine *pLine,AcDbCircle *pCircle);
ZYLink();
virtual ~ZYLink();
SPIRAL *pSpiral;
//private:
AcDbObjectId pSpirId;
void CALCULATION(int lr=0);
void getlrRot(int lr=0);
double startFWJ;
double func(double x);
double dR;
double AtodR(double AA);
AcDbArc* Arc;
AcDbCircle* Cir;
AcDbLine* Line;
AcGePoint3d startPtL,endPtL,center,startPtCur,endPtCur;
double R,A;
	double lrRot;//��ƫΪ��1����ƫΪ����1*/
