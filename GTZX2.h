// GTZX2.h: interface for the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
#define AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GTZX.h"
#define B_X 0
#define R_X 1
#define S_StartPoint 2
#define S_EndPoint 3
#define B_XNOTP_X 4//plh05.05
#define MAXDLNUM2x 100
#define MAXJDNUM2x 100
#define LEFTLINE -1
#define RIGHTLINE 1
//DLֵ->����Ϊ��������Ϊ�����������BLC+DL=ELC,DL=ELC-BLC;
class GTZX2_ROAD : public GTZX_ROAD
{	
public:

	//////////////////////////////////////////////////////////////////////////
	//�������ת��������
	//����ת�ֳ����
	 bool isReFresh;
	bool TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC=3);
	bool TrsNEtoProjml(double N, double E, double& Projml);
	bool TrsNEtoCml(double N, double E, double& Cml);
	bool TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ);
	bool TrsCkmltoProjml(ACHAR Ckml[], double& Projml);
	bool TrsCkmltoCml(ACHAR Ckml[], double& Cml);
	bool TrsCmltoNE(double Cml, xlpoint& PZ);
	bool TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC=3);
	bool TrsCmltoProjml(double Cml, double& Projml);
	bool TrsProjmltoNE(double Projml, xlpoint& PZ);
	bool TrsProjmltoCml(double Projml, double& Cml);
	bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC=3);
    void SetiRXDBPDArray(int iRXD, int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd);
	void SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd);
	void Create2XGTBPDArray();
	UltraCArray_T<BPDArray,BPDArray> AllRXDBPDArray;

	//���±�ΪiRXD���жδ���ΪһGTZXָ��
	GTZX_ROAD* GetiRXDGTZX(int iRXD);
	//�γɶ��߹�ͨ������
	void Create2XGTDLB();
	//��ע���µ�
	bool BZCM(AcGiWorldDraw* pWd);//��ע���µ�
	//���߹�ͨ��������
	int m_2XGTDLSum;
	//�����������·�Ľ���Լ��
	void CheckAllXLConstraint();
	double TYLC2x(CString XCLC2x,int iRXD=-1);//plh08.20//��iRXD��0��ָ�����жβ���
	//plh04.14
	EXDLB DLB2x[MAXDLNUM2x];
	GTZX_ROAD *m_pm1;//һ��ƽ��
	DoubleLineData JD2[MAXJDNUM2x];
	int LEFT_OR_RIGHT;//�����߱�� (LEFTLINE -1) (RIGHTLINE 1)	

public:

	//�޸Ľ������ʣ�������������������н������ʱ�Ϊ���С�
	void ModifyJDXZ(int iRXD);
	int GetiRxdNoFromTYLC1x(double lc1x);

/////////////////////////////////////////////////////////
	void ModifyRXDName(double N, double E);
	void out();
	GTZX2_ROAD(AcDbObjectId Id1x, double xjj);
	GTZX2_ROAD(GTZX_ROAD *gtzx, double xjj);

	void CalExJDByParrel(int jdn1, double xjj1, double xjj2, DoubleLineData *CurJD);
		
	int FindACHAR(ACHAR *str,ACHAR At);
		
	void ModifyNEbyXJJ(double &N, double &E, double xjj);
		
	void ModifyNEbyXJJ(ACHAR JDLC[],double xjj,double &N,double &E);
		
	void ModifyNEOnBian(DoubleLineData *CurJD);
		
	void ModifyOriExJDXY();
		
	//��Ƕ��߽����Ƿ�λ�ڶ�����(�Ƿ���ʾ����Ļ��)
	void TagEXJDIsShow();
	
//////////////////////////////////////////////////////////
	//plh04.14
	void GetiRXDJDArray(int iRXD,double jday[MAXJDNUM][6],int &njd);
	//plh04.14
	void AddDoubleLineJD(DoubleLineData ExJD);//β������һ������
	//plh04.14
	void ModifyDoubleLineJD(int iJD,DoubleLineData ExJD);//�޸�ָ������
	//plh04.14
	int GetiRXDFromNE(double N,double E);
	//plh04.14
	void makeXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len);
	//plh04.14	
	void BZXJJ(AcGiWorldDraw* pWd);
	void Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum);
	void GetDLTYLC();
	void SetDLB(EXDLB *Dlb,int ndl);
	void SetDLB(BAS_DRAW_FUN::EXDLB *Dlb, int ndl);
	void DelS(double N,double E);
	double DistToJD(double N,double E,int iJD);
	void ModifyOneSWJDdata(int iJD,double N,double E,double XJJ,double JIAO);
	void CalOneBXJDdata(int iJD);
	bool AddS(double N1, double E1, double N2, double E2,double R1,double Lo1,double R2,double Lo2);
	int FindiRXD(int iJD);
	void DeleteOneRXD(int iRXD);
	void DeleteOneRXDByXLPoint(double N,double E);
	void InsertOneRXD(DoubleLineData *OneRXDJDB,int njd,ACHAR EGH[]);//plh04.11.25
//	bool AddRXD(GTZX *newGtzx,double N,double E);
	bool AddRXD(double (*jdarray)[6],double (*qxb)[9],int njd,double N,double E,ACHAR EGH[]);//plh04.11.25
		
	int getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );
		
	void AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd);
	void INIT();

	double XCLC(double TYLC,ACHAR *GH,EXDLB *ExDlb, int ndl);

	//�ɽ������飬������������߱�,������Ԫ���飬...	
	void CalXYArrayFromJdArray(double (*jdarray)[6], int njd,double (*qxb)[9],double (*xyarray)[10],int &tary);
	
	void BZiRXDLC(AcGiWorldDraw* pWd,int iRXD);//�����ע���ж�
	void BZRXD(AcGiWorldDraw* pWd);

	//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ����,	
	double LocateHundredMeterOnLine(double tylc,bool IsForwardTo=true);

	//��Ѱĳ���жε����ն������
	void GetiRXDDLinfo(int iRXD,int &sDLno,int &eDLno);

	//ɾ��ָ�����ж�
	void DeleteiRXDDLB(int iRXD);

	//��ȡ���жε�����
	int GetRXDNum();
	
	//�õ�ָ�����жε����ս������
	void GetJDXHofiRXD(int iRXD,int &firstJDXH,int &endJDXH);

	//�����������жε�EXDLB
	void Create2XDLB(int iRXD,ACHAR EGH[]=NULL);//plh04.11.25

	//�����������жε�EXDLB
	void Create2XDLB();
	
	double T0min;//��С��ֱ�߳�
	double Lrmin;//��СԲ���߳�
	void GetT0min_Lrmin();
	void UpdateData(bool IsTheFirst = true);
//	GTZX2(GTZX *pm1,double[][2],int njd2,ACHAR jdgh[],int jdo);//һ�����ݣ�һ�߽���ź��߼�࣬���߽���ںţ���ʼ������
	GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[]);
	GTZX2_ROAD(GTZX_ROAD *pm1,int njd2,BAS_DRAW_FUN::DoubleLineData jd2[],int jdo,ACHAR jdgh[]);
	GTZX2_ROAD();
	virtual ~GTZX2_ROAD();
	void CalJDdata(BOOL full = FALSE,int qdi = 0,int zdi = 0);//�Զ�����뾶������
	void CalJDdata1();//������뾶������
	void CalNYDL();
	double CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2);
	static double Lh_Rtop(double Lh,double R);
	ACRX_DECLARE_MEMBERS(GTZX2_ROAD);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow=true);
	void BZJD(AcGiWorldDraw* pWd,double arrayd[500][6],double qxb[500][9],int tarray,int JD0,ACHAR *JDchr);
	//����һ����̣������߼�࣬�����϶�Ӧ������̼���,1-��ʾ�ɹ���0-ʧ��
	//  int CalXjj(double ml1,double &xjj,double &projml,double resPt[2]);
	void CalEXJD(int jdn1,int njd2,double EXJD[2]);//����һ�߽���ţ����߶�Ӧ����ţ����ز��жζ��߽�������
	int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	//   int Judge2XLorR(xlpoint PZ);
	void SetJDdata();//���ɶ���JDarray
	static double GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dbj,double &T);
	double CalTFromR_Lr_Lh(double R,double Lr,double Lh); 
	double CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0);		
	void BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl);
	void BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S=true);//��ע���������
  virtual Adesk::Boolean addDLM(double tylc,double BLC,double ELC, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(double tylc);
//plh04.14	
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const; 
  virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		                                                AcDbIntArray& osnapModes,
														AcDbIntArray& geomIds) const;

  virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		                                       const AcGeVector3d& offset);
};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX2_ROAD);
#endif // !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
