// YYLuanLink.h: interface for the YYLuanLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_)
#define AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "SPIRAL.h"
//���ͼ���ģ��
class YYLuanLink : public BAS_DRAW_FUN 
{
public:
	//�������߶���
	SPIRAL pSPIRAL;
	//�����������յ������
	double PtOnCir1N,PtOnCir1E,PtOnCir2N,PtOnCir2E;
	
	//������Բ������Ͱ뾶������ת
	virtual void Set(double n1,double e1,double r1,
			double n2,double e2,double r2,
			int lr);
private:
	double func(double x);
	double diedai(double A1, double A2,int flag);//flag+1,����-1�ݼ�
	double AtoCC(double AA);
	int count;
	double iDistO1O2;
public:
//private:
	//////////////////////////////////////////////////////////////////////////
	//��������
	double CenN1,CenE1,R1;//ǰһ��Բ
	double CenN2,CenE2,R2;//��һ��Բ
	int LeftOrRight;//����ת����Ϊ-��Ϊ+
//private:
	//////////////////////////////////////////////////////////////////////////
	//�ڲ�����
	double A,R;//��
	//Բ�ľ�
	double DistO1O2;
	//�Ƿ����
	bool ERRFlag;
//	//��R1<R2����ԲҪ������Exflag=-1;
//	static int Exflag;
	//����ģʽ #define KNOW_A1 1 #define KNOW_A2 2 #define KNOW_K_A1_A2 12	
	double Ls;//��
	double startL,endL;
	double Rmax,Rmin;
	double fwjO1O2,fwjO2O1;
	
public:
	static double DistThisCenToOtherGQX(double r1,double r2,double a,double &afa);
	YYLuanLink();
	~YYLuanLink();
	YYLuanLink(AcDbArc *pArc1,AcDbArc *pArc2,int lr);
	YYLuanLink(double n1,double e1,double r1,
			double n2,double e2,double r2,
			int lr);

};

#endif // !defined(AFX_YYLUANLINK_H__5A1A3EAB_5C22_4650_82BA_6A320BC645E0__INCLUDED_)
