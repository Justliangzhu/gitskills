// YYCLink.h: interface for the YYCLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_)
#define AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "spiral.h"
//C�ͼ���ģʽ
class YYCLink : public BAS_DRAW_FUN  
{
public:
	//������������
	SPIRAL pSPIRAL1,pSPIRAL2;
	//���������յ�����
	double PtOnCir1N,PtOnCir1E,PtOnCir2N,PtOnCir2E;
	//����Բ��ʵ���ָ�롢������ʼ��Aֵ������ת
	virtual void Set(AcDbArc *pArc1,AcDbArc *pArc2,double a1,double a2,int lr);

	//������Բ��Բ������Ͱ뾶��������ʼ��Aֵ������ת
	virtual void Set(double n1,double e1,double r1,
			double n2,double e2,double r2,
			double a1,double a2,int lr);
//private:
	//ѭ������
	int count;
	//////////////////////////////////////////////////////////////////////////
	//��������
	double CenN1,CenE1,R1;//ǰһ��Բ
	double CenN2,CenE2,R2;//��һ��Բ
	double A1,A2;//ǰ������
	int LeftOrRight;//����ת����Ϊ-��Ϊ+
//private:
	//////////////////////////////////////////////////////////////////////////
	//�ڲ�����
	int IsSModel;//�Ƿ�ΪS�� ��SΪ-1;;CΪ+1;;
	//Բ�ľ�
	double DistO1O2;
	//�Ƿ����
	bool ERRFlag;
	//����ģʽ #define KNOW_A1 1 #define KNOW_A2 2 #define KNOW_K_A1_A2 12	
	int K_A_mode;
	double K12;// A1,A2��ֵ
	double Lo1,Lo2;//ǰ������
	double GQ_N,GQ_E;//���������߹��е�

private:
	double func(double x);
	virtual double GetDistO1ToO2(double a1,double a2);
	double diedai(double A21, double A22);//����A2-CC
	double AtoCC(double a);
public:
	void SetSLink(bool IsS=true);
	YYCLink();
	YYCLink(double n1,double e1,double r1,
			double n2,double e2,double r2,
			double a1,double a2,int lr);
	~YYCLink();	
	
//	YYCLink(double CenX1,double CenY1,double RR1,
//				 double CenX2,double CenY2,double RR2,
//				 double AA1,double AA2,double lr);
//
//	SPIRAL pSPIRAL1,pSPIRAL2;
//
//	bool ERRFlag;
//	void GetSpirItem(int i,double &AA, double &RR, double &x,double &y, double &fwj, double &slc, double &lr);
//	YYCLink(double x11,double y11,double x10,double y10,double x12,double y12,double x21,double y21,double x20,double y20,double x22,double y22,double lr,double Ak);
//	void KeyBoardInput();	
//	void CALCULATION(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2,double lr=0,double Ak=0);
//	YYCLink (AcDbEntity *pEnt1,AcDbEntity *pEnt2);
//	YYCLink(AcDbArc *pArc1,AcDbArc *pArc2);
//	YYCLink(AcDbCircle *pCir1,AcDbCircle *pCir2);
//	YYCLink(AcDbArc *pArc1,AcDbCircle *pCir2);
//	YYCLink(AcDbCircle *pCir1,AcDbArc *pArc2);
//	YYCLink();
//	virtual ~YYCLink();
//	YYCLink(AcGePoint3d Cen1,double r1,AcGePoint3d Cen2,double r2);
//	double diedai(double A1, double A2);//S�ͣ�@A2��CC����,������
//	double AtoCC(double AA);
//	double func(double x);
////private:
//	double PtOnCir1X,PtOnCir1Y,PtOnCir2X,PtOnCir2Y;
//	double fwj_O1P1,fwj_O2P2;
//	AcDbCircle *Cir1,*Cir2;
//	AcDbArc *Arc1,*Arc2;
//	AcDbObjectId spirID1,spirID2;
//	AcGePoint3d GQ;
//	void getlrRot();
//	int K_A_mode;
//	void Create();
//	AcGePoint3dArray points;//�����㣻
//	int LRflag;
//	int count;
//	double Xm1,Xm2;//�д���
//	AcGePoint3d pCen1,pCen2,Pt1,Pt2;//Բ��1��Բ��2����C1���е㣬��C2���е㣻
//	double K12,A1,A2,R1,R2,R,dC1C2,D1,D2,L1,L2,Bata1,Bata2,Fai,Afa1,Afa2;
//	//L1��L2Ϊ�鹫��ֱ������Բ֮�仺��,D1,D2Ϊ�鹫��ֱ������Բ֮�ࡣ

	
};

#endif // !defined(AFX_YYCLINK_H__C257A622_A213_4909_9178_9806409603F0__INCLUDED_)
