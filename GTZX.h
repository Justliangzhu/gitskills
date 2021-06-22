// GTZX.h: interface for the GTZX class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
#define AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <aced.h>
#include <dbsymtb.h>
//#include <dbapserv.h>
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
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN_RAIL.h"
#include "triDTM_road.H"
//#include "GTZX2.h"
#include "GTZX_JYX.h"
#include "GTZX2_JYX.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 

//		�Զ�����

//   1�����㶨��
//   MNJD �������ཻ�����������յ㣩��NJD--ʵ�ʽ������������յ㣩��
//   IJD--��I�����㣨���0���յ�NJD-1����JDGH--����ںţ�
//   JDarray[i][6] --���� ��š�N��E��R��L1��L2��JDQXB[i][]--ZH��HZ��T1��T2��L
//   JDPXYINDATA[i][3][5]--������Ԫ
//   DLB[j]--���������ںţ�
//   PDB[k][]--�¶ȱ�

class GTZX_ROAD : public BAS_DRAW_FUN_RAIL ,public AcDbEntity ,public triDTM_road
{
public:

	bool IsdrawTZD;
	bool IsdrawLC;
	bool IsdrawXLM;
	bool IsdrawBPD;
	bool IsdrawJDLX;
	bool IsdrawJD;
	
	

	  bool isReFresh;

	  //////////////////////////////////////////////////////////////////////////
	  //�������ת��������
	  //����ת�ֳ����
	  //�������ת��������
	  //����ת�ֳ����
	  void BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);
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

	  void ReadXLGuifan();
       GuiFanCanShu mGuiFanCS;
	  //////�����¶ȵ�
	  //���µ�����
      BPDArray m_BPDArray;
	  int m_LJorGM;
	  int m_Xsw;
	  int m_DesHXSW;
	  //CM����
	  double m_CM[5*MAXBPDNUM];
	  //CM��������
	  int m_NCM;
	  //���µ�����תCM ����
	  void TrsBPDArraytoCM();
	  //��ע���µ�
	  bool BZCM(AcGiWorldDraw* pWd);//��ע���µ�
	  //���ñ��µ�����
	  void SetBPDArray(int NPD, BAS_DRAW_FUN::PDTAB* BPDArr, bool IsAdd);
	  void SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd);
	 // ����cml���ĸ߳�
	  double rdes_level(double cml,double &slope);
	 // ��CM�������cml���ĸ߳�
	  double DLEVEL(int icm, double cml);
	  //��·��ϵ����
	  XLRelate m_XLRelate;
	  //��Ҫ�뵱ǰ��·�󽻵����·������
	  ACHAR m_NeedInterXLNameArray[8][64];
	  int m_NeedInterXLNum;
	  //Լ��������Ϣ
	  CArray<ConstraintErr,ConstraintErr> m_ConstraintErrArray;
	  //������·Լ����Ϣ
	  void SetXLRelate(XLRelate& Relate);
	  //��⵱ǰ��·���������ཻ��·�Ľ�����߲�Լ����Ϣ
	  void CheckAllXLConstraint();
	  //��עΥ���ཻ��·Լ����Ϣ
	  void DrawXLConstraint(AcGiWorldDraw* pWd);
	  //��ȡ���������������
	  void GetJPData(CString JPXLName,bool isStart, double& Lever, double& Slope);
	  //ˢ���뵱ǰ��·�ཻ�������·ʵ��
	  void RefreshRelateXL();
	  void INSERT_STATION(ACHAR CKml[80],ACHAR StaName[80],ACHAR TKSta[80],AcGiWorldDraw* pWd);
	  int station_num;
	  struct
	  {
		  ACHAR CKml[80];//�������
		  ACHAR StaName[80];//��վ�� 
		  ACHAR TKSta[80];//����ͼ����
		  
	  }Station[100];
	  void Invert();
	  //plh04.14//����ͳһ��̣��Ƿ���ǰ�����������븽��������ͳһ��̵Ĳ�ֵ��tylc>������lc...+
	  double DistToNearDL(double tylc,bool IsForwardTo);
	  //plh04.14
	  double LocateHundredMeterOnLine(double tylc1x,bool IsForwardTo);
	  //plh04.14
	  int GetPreJD(double N,double E);
	  //plh04.14
	  int DistToNearJD(double tylc,bool IsForwardTo=true);
	  //plh04.14
	  //�޸�ָ������
	  void MODIFYJD(int JDNO,double N, double E, double L1, double R, double L2);
	  //plh04.14
	  //β�����ӽ���
	  void ADDJD(double N,double E);

	  Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
	  Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);
	  //  �ɶ������ݱ� ��ĳͳһ��̵��ֳ���̺͹ں�
	  double   XLC( double TYLC,ACHAR *GH, int NDL);
	  double   XLC( double TYLC,ACHAR *GH, int NDL,int &iDL);
	  //  �ɶ������ݱ� ��ĳ�ֳ���̵�ͳһ���
	  double  TYLC(ACHAR ckml[80], int NDL );
	  
	  void  FormLCB(double dlc,double sml,double eml) ;
	  void AddTZDtoLCB(); //�����������������̱� 
	  void AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT); //������վ������̱�
	  //�����struct LCB��̱��
	  void VIEW_PM_ZDM(int k);
	  void VIEW_PM_PQX(int k);
	  int PM_PQX(double *pqx);
	  void outdata(CString str);
	  Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);
	  
	  void draw_DMX();
	  int PM_DMX(double *dmx);
	  double xl_length;
      double DV;//�ٶ�Ŀ��ֵ
	  double RtjMin,RtjMax;//�Ƽ��뾶
	  double ZXLmin1,ZXLmin2;//һ����С��ֱ�߳���������С��ֱ�߳�
	  double Rmin1,Rmin2;//һ����С���߰뾶������
	  CString TLDJ,ZXSM;//��·�ȼ�
	  double XJJ;//��׼�߼��,0����,>0�������҂ȣ�<0���������
	  double ZDMJG;//�ݶ�����
	  ///�����������
	  Adesk::Int16 pro;//��GTZX����
	  CString mdbname;//��GTZX��Ӧ��mdb
	  Adesk::Int16 Zx[2];//��GTZX������Zx[0],�ӷ���ZX[1],-1Ϊ�� ֱϵ����
	  int Pxout[20];//��GTZX��������,���20��
	  int nout;//��GTZX������������
	  int Pxin[20];//��GTZX���뷽��,���20��
	  int nin;//��GTZX���뷽������
	  double Sprjml;//����������ڸ�����ͶӰ�������
	  double Eprjml;//�������յ����ӷ���ͶӰ�������
	  double SelSml;//ѡ���������뷽���Ŀ������,��ʼΪ�յ��������
	  double SelEml;//ѡ���������������Ŀ������,��ʼΪ����������

    ACRX_DECLARE_MEMBERS(GTZX_ROAD);
	GTZX_ROAD();
	virtual ~GTZX_ROAD();

    GTZX_ROAD(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr);
	void Set(int iNJD,double iJDarray[MAXJDNUM][6],ACHAR *GH,ACHAR *JDchr);
    virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
    Adesk::Boolean getpxy_DATA();

    void setTD(short int JDTDBZ0,short int draw_zybz0,short int qxbbz0);
	void  setINIT(int nlc,int nh,int Chinese_Engbz,double Texth);
	void  setJD_RL(int JDi ,double Ri,double L1i,double L2i);
    virtual int   getJD_RL(int JDi, double *Ri,double *L1i,double *L2i ,double *NN,double *EE);
    int get_pxy(int id,double *d0,double *d1,double *d2,double *d3,double *d4);  
	int savePM(ACHAR *fname);
	int saveDL(ACHAR *fname);
    int savePXY(ACHAR *fname);

    
	Adesk::Boolean   addXL(int JDi,double R,double L1,double L2 ,double NN ,double EE,double Nend ,double Eend );
    Adesk::Boolean   addJD(int JDi,double R,double L1,double L2 ,double NN ,double EE );
    Adesk::Boolean   DelJD(int JDi );
	
	int getDLM(int iDL,double  *BLC,double *DL, ACHAR *BGH,ACHAR *EGH);
	Adesk::Boolean   setDLM(int iDL,double BLC,double DL, ACHAR *BGH,ACHAR *EGH,int mode=1);
	Adesk::Boolean setDLM(int iDL,double BLC,double DL, CString BGH, CString EGH, int mode=1 );
    virtual Adesk::Boolean   addDLM(int iDL,double BLC,double DL, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(int iDL);
   
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
    virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
    virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
    virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
    virtual void GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);
    virtual void subList() const;
    Acad::ErrorStatus  subExplode(AcDbVoidPtrArray& entityset);
    //  virtual AcRxObject* clone() const;

	//
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
          AcDbIntArray&     geomIds) const ;

	Acad::ErrorStatus subTranformBy(const AcGeMatrix3d &);	
    Acad::ErrorStatus subGetTransformedCopy( const AcGeMatrix3d& mat,AcDbEntity*& ent) const;

	int mode_display;
	double JDarray[MAXJDNUM][6];  // 1->N,2->E,
	double array_pxy[MAXJDNUM][4][5];// QXLRL[100][3];
	double array[4*MAXJDNUM][10];//0����Ԫ����1-ֱ�ߣ�2-Բ��3-ǰ���� 4-��
	                      //8��9����Ԫ���x,y
	                      //6:��Ԫ������
	                      //7:�뾶
	                      //4:����ת
	                      //5����ʼ���߷�λ��
	double QXB[MAXJDNUM][9]; 
	bool m_IfViewDmx;
//plhadd	zh���,hy���,yh���,hz���,a,T1,T2,L,��ƫΪ-1(��1)
	//[NJD][9]
	
	Adesk::Int16 NJD,tarray;
	Adesk::Int16 NDL,NH,NLC; //NDL ����������������̹ںŸı�㣬��CK12+235=C1K12+235��Ϊ����ֵ��0����������㣩
               // NLC--��̱���λ����=0  NH--�̱߳���λ����=0
	Adesk::Int16 Chinese_Engbz_type;  
  Adesk::Int16 JDTDBZ ;// JDTDBZ �����϶���־ ��JDTDBZ==1Ϊ�������϶�  =2 Ϊ��Բ =��������Ϊ�����϶�
  Adesk::Int16 draw_zybz,qxbbz;	//draw_zybz=-1 �����Ĵ�׮�����߲���������࣬=1Ϊ�����Ҳ�  =0Ϊ������ƫ���
                            	//qxbbz=1 Ҫ�����߲�������������
	Adesk::Int16 JD0; //��ʼ�����
	DLBZ DLB[30];  // �����
  DLBZ DLBTMP[30];
	double texth;
	ACHAR JDGH[20];
  //  double Design_V;
	int KNDD;
//    int BZTZD1 (double array[400][5],int trow, char *GCH,int NDL,double texth ,double LCB,int zybz,int Chinese_Engbz_type ,int NLC );  //  ��ע���
//    int BZLC1(double array[400][5],int trow,char *GCH,int NDL,double texth,double dlc,double LCB,int zybz,int type) ; //  ��ע���;

	struct TZDLCB//��������̱�
	{
		int tzdflag;
		//���������ͣ����(0),�յ�(1),ZH(2),HY(3),QZ(4),YH(5),HZ(7)
		double lc;//ͳһ���
		double x,y;//ƽ������
		ACHAR bz[80];
	}pTzdlcb[5*MAXJDNUM];
	int NTZD;//��������
//	struct LCB//��̱��
//	{
//    	int lcflag;//������ͣ�-1������꣩,1-9���ױ꣬0-������
//		double lc;//ͳһ���
////		double x,y;//ƽ������
////		char bz[80];
//	}pLCB[20000];
	struct LCB//��̱��
	{
		int lcflag;//������ͣ�-1������꣩,1-9���ױ꣬0-������
		double lc;//ͳһ���
		//		double x,y;//ƽ������
		//		char bz[80];
	};
	CArray<LCB,LCB> pLCB;
	int NLCB;//׮��
	double StartCml,EndCml;
	ACHAR StartCKml[80],EndCKml[80];
	
	void FindZXLenMin();//�ҳ���С��ֱ�߳�
	void checkpm();
	void  BZJD(AcGiWorldDraw* pWd,double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int trow,int JD0,ACHAR *JDchr );
   void  BZLC(AcGiWorldDraw* pWd,double dlc, int zybz,int type);
    void BZTZD(AcGiWorldDraw* pWd, double dlc, int zybz,int type) ; //��ע���
  
//private:
	int INIT();
	Adesk::Boolean  G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ, double ND);
    Adesk::Boolean  G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ );
    

    int BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC );  //  ��ע���
    int BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type);  //  ��ע���
    int BZJD1(double arrayd[MAXJDNUM][6],double qxb[MAXJDNUM][9],int trow,int JD0, double texth,int draw_zybz,int qxbbz,int NLC);
    void draw_pm();
	//-----------------------------
    void VIEW_PM_TypeHdm();
 };
//ACRX_DXF_DEFINE_MEMBERS(GTZX, AcDbEntity,  0,  GTZX,"GUADS");
int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX_ROAD);
#endif // !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)





















