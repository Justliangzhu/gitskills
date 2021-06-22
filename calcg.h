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
	double JKfactor;//����(��Ӳ·��)��ȳ���7.50m  JKfactor=1 else JKfactor=0.5
	double ZXCGJBL;//ֱ�߶γ���ʱ���߽�����
	double CGHXZXJBL;///���Ͷ���С���߽�����1/330
	int ZXCGLenMode;//ֱ�߶γ���ʱ�ĳ���ȡ��0-��ȡ������1-1m�������� ,2-5m��������
	int SuphMode;
	int Vj;
	int NSUPH,NJK;
	double SuphData[10][2],JKData[10][2];
	

 //Fend[][0]---���ʣ�0--ֱ�ߣ�1--�������ߣ�2--�����ߣ�3--��������Ҫ����
  //            ����ı�ӵ�ţ�
  //Fend[][1]---CML�ֶ��յ��������
  //Fend[][2]---X�ֶ��յ�����
  //Fend[][3]---Y
  //Fend[][4]---FWJ�ֶ��յ�����·����ķ�λ��
  //Fend[][5]---������յ�Ĵ��x����
  //Fend[][6]---������յ�Ĵ��y����
  //Fend[][7]---������յ�ķ�λ��
  //Fend[][8]---������յ���������
  //Fend[][9]---�öεİ뾶��Aֵ

	//private:
	//��Ԫ����˵��
	//////////////////////////////////////////////////////////////////////////////////
	//��0����Ԫ��ʾ��·���                                                         //
	//0: NULL                                                                       //
	//1��2: N��E                                                                    //
	//3: ��·��㷽λ��a                                                            //
	//4: ��·������                                                               //
	//////////////////////////////////////////////////////////////////////////////////
	
	//0����Ԫ���� 1-ֱ�ߣ�2-Բ��3-ǰ����4-��, 5-��������R��->RС, 6-��������RС->R��
	
	//(R1>R2)/////////////////////////////////////////////////////////////////////////
	//1:			L		R    A         A              A                A        //
	//2:					L	 R         R              R1               R2       //
	//3:                                                  R2               R1       //
	//////////////////////////////////////////////////////////////////////////////////
	//4: ����ת
    //5: ��ʼ���߷�λ��
    //6: ��Ԫ�����̣�=ͳһ���=��·�������+���յ㵽��·�������߾��룩
	//7: �뾶 R
	//8��9����Ԫ������� N,E
	//���һ����ԪΪ�յ�
	//0:0

 
 CgSZ Zcg[1000],Ycg[1000],Zjk[1000],Yjk[1000];
 int ZcgNm,YcgNm,ZjkNm,YjkNm;

//���ļ�����

int indata();
int read_JDfile_new();
void ReadPMfile();
void ReadWKdir();
int ReadSGFile();
int Read_HdmFile();
//�ڴ�

void  freeroom();
//��������
double FindSuph(double R);
double FindJK(double R);
int FindIlmk(double cml,LMKdata lmk[],int nlmk);
//����
double AdjustLc(double lc);
void SetSuphData(int ZorY,struct CgSZ *Cg,int& cgnm,struct CgSZ *Jk,int& jknm);
double cal_B(double Zjdw , int suph_mode , double Cxdw , double Yljw , double Tljw);
void Cal_Suph(); //�����ݶ���ͼ���賬������
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
