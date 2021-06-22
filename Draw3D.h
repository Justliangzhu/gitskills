// Draw3D.h: interface for the CDraw3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_)
#define AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XLDataBase.h"
#include "JD_CENTER.h"

class CDraw3D : public BAS_DRAW_FUN
{
public:
	CDraw3D();
	virtual ~CDraw3D();
	double Create3DModel();
	void ReadHdmResultFile();
	void ReadACrossResultData(FILE *fpr,int jcross);
	int draw_fill_cut();
	int drawfillandcut(int i,int ZorY,double qdeltah,double hdeltah,double qtw,double htw,int qdmxz,int hdmxz,int qnpt,int hnpt,AcGePoint3dArray &qPtarr, AcGePoint3dArray &hPtarr);
    double FindBriAng(double cml);
	void DrawPartFillBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
		AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
	void DrawCutPartBetweenTwoMl(int ZorY,int qPtNum,int hPtNum,
		AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
	void CalTWZeroPt(double bml,double eml,double Bdeltah,double Edeltah,double &k,double &MidMl);
   void AddBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,double x,xlpoint &PZtmp);
   void DrawBriBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void AddSideBriBodyPt(AcGePoint3dArray &Ptarr,int LorR,xlpoint &PZtmp);
    void CreateBriPt(int jcross,int brixz,AcGePoint3dArray &Ptarr,int ZorY);
	void CreateTunPt(int jcross,AcGePoint3dArray &Ptarr,int ZorY);
   void AddSinTunBodyPt(AcGePoint3dArray &Ptarr,int LorR, xlpoint &PZtmp);
   void DrawTunBody(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void AddDouTunBodyPt(AcGePoint3dArray &Ptarr,int LorR,xlpoint &PZtmp);
   void DrawTunDoor(int ZorY,int scross, int ncross);
   void Draw_TunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr);
   void Draw_DouTunBZ(int ZorY,int scross,AcGePoint3dArray &OutTunArr);
   void DrawLM(int ZorY,AcGePoint3dArray &qPtarr,AcGePoint3dArray &hPtarr);
   void draw_circleBase(AcGePoint3d CenPt,double D,double H);
   void draw_BaseCape(AcGePoint3d Lpt,AcGePoint3d Rpt,double fwj);
   void draw_bridge_base(double ang,CString XZ);
   void GetCmlKKSY(int ibri);
   void CalDeshLmk(double cml, double &zmh, double &ymh,double &Zwd, double &Ywd);
   void draw_BrigeSlop();
   int Cal_ZPJD(int ZorY,int HorT,AcGePoint3d Cenpt,AcGePoint3d Spt,AcGePoint3d Ept,AcGePoint3dArray &ptarr);//����׶�½ŵ�
	int draw_Terrain();
	void CalBriSlopPt(int i,int ZorY,AcGePoint3dArray &ptarr);
	void CalOffsetByXLZX(double cml,int NLMK,LMKdata lmk[],double& OffsetByXLZX);
	void ReadLMK(CString LMKTABname);
	
   int KNum;
   struct brginf{
	   int ks;
	   double klen;
   } BrgArray[100];
   
   struct BriLSEml
   {
	   double sml;//��������ʼ���	
   } BrgLArr[1000];//���Ŷ�Ӧ�����±�
   int nBriL;

	JD_CENTER *pm;
	XLDataBase DBS;

	double SmlOnZx,EmlOnZx,SPXJJ;
	//����·�������������ϵ�ͶӰ���
	//	double LJFLSmlOnZx,LJFLEmlOnZx,LJFLSmlOnFLD,LJFLEmlOnFLD;
	//���ձ��·�����������ϵ�ͶӰ���
	double BPFLSmlOnZx,BPFLEmlOnZx,BPFLSmlOnFLD,BPFLEmlOnFLD;
	
	int NCRS;
	struct CRSDAT
	{
		ACHAR ckml[80];
		double cml;
		double dml;
        int DorS;//1-��·�棬2-˫·��
		double DuanLuoInf;
		double xjj;//�߼�࣬����Ϊ0,��������Ϊ-����Ϊ+
		double ZDesH,YDesH;
		double LZDK,RZDK;
		//DesH·�����Ƹ̺߳�EarH����̣߳�HDM�Ļ�ͼ���ݣ�//ָ����
		double RHFL[10],RHFR[10];  //����·���5��
		double zlmhd[3],ylmhd[3];
		int ZDMXZ,YDMXZ;//���ҷ���������
		double  Zt,Yt;
		double ZHc,YHc,Y_ZHc,Y_YHc;//���ڸ�
		int zptsum,rptsum,Lnpt,Rnpt;
		AcGePoint3dArray RPTL,RPTR;
		AcGePoint3dArray PTRPTL,PTRPTR;
		int Ljdnum,Rjdnum;
		//ָ����
		double FLDcml;//��ص�·ͶӰ���
		CString FLDname;//��ص�·��
		double Y_RHFL[10],Y_RHFR[10];  //�����·���5��
        double Y_zlmhd[3],Y_ylmhd[3];
		int Y_ZDMXZ,Y_YDMXZ;//���ҷ���������
		double  Y_Zt,Y_Yt;
		int Y_Lnpt,Y_Rnpt;
		AcGePoint3dArray Y_RPTL,Y_RPTR;
		AcGePoint3dArray Y_PTRPTL,Y_PTRPTR;
		int Y_Ljdnum,Y_Rjdnum;
		
		double Ldeltah,Rdeltah;//����·���������ߵĸ߲��
		double Y_Ldeltah,Y_Rdeltah;//����·���������ߵĸ߲��
		double newLYd,newRYd;//��·�����ĵĵ���߲㣬��·�����ĵĵ���߲�
		double  at,aw;
		double Ldh,Rdh;
		double DX;
		//		//���߻�һ��
		int LGouPtNum,RGouPtNum,LJSGouPtNum,RJSGouPtNum;////////wwq5.14
		int LGouProperty,RGouProperty;
		double Lgou[36],Rgou[36],LJSgou[36],RJSgou[36];//ˮ��7��/////wwq5.14
		AcGePoint3dArray PTLgou,PTRgou,PTLJSgou,PTRJSgou;
		//	
		//		//����
		int Y_LGouPtNum,Y_RGouPtNum,Y_LJSGouPtNum,Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty,Y_RGouProperty;
		double Y_Lgou[36],Y_Rgou[36],Y_LJSgou[36],Y_RJSgou[36];//ˮ��7��/////wwq5.14
        AcGePoint3dArray Y_PTLgou,Y_PTRgou,Y_PTLJSgou,Y_PTRJSgou;	
		
		int IfHasZlink,IfHasYlink;
		CRSDAT *zlink,*ylink;
		double ZDorS,YDorS;
		double ZCG_GDR,YCG_GDR;
		//���߻�һ��		
		double OffsetByXLZX;
	};	
	CRSDAT *hdmdes;

	int NZLMK,NYLMK;
	LMKdata *ZLMK,*YLMK;
};

#endif // !defined(AFX_DRAW3D_H__31A9C0D9_9522_413D_A893_249B0F39A4F4__INCLUDED_)
