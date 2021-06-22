

// GTZX.h: interface for the GTZX class.
//
//////////////////////////////////////////////////////////////////////

//#if !defined(AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
//#define AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

#if !defined(AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)
#define AFX_GTZX_JYX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_

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
///

///
#include <dbjig.h>
#include <acgi.h>
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include <iostream>


#include "rxdefs.h"
#include "ol_errno.h"
#include "STDIO.h"
#include "malloc.h"
#include "time.h"
#include "BAS_DRAW_FUN_JYX.h"

#include "StdAfx.h"
// #include "triDTM.H"
//#include "GJDGTZX.h"

#define streq(s,t) (_tcscmp((s),(t))==0)
#define AppName  /*MSG1*/L"GUADS" 

//class GTZX2;

//		�Զ�����

//   1�����㶨��
//   MNJD �������ཻ�����������յ㣩��NJD--ʵ�ʽ������������յ㣩��
//   IJD--��I�����㣨���0���յ�NJD-1����JDGH--����ںţ�
//   JDarray[i][6] --���� ��š�N��E��R��L1��L2��JDQXB[i][]--ZH��HZ��T1��T2��L
//   JDPXYINDATA[i][3][5]--������Ԫ
//   DLB[j]--���������ںţ�
//   PDB[k][]--�¶ȱ�
class GTZX_JYX_ROAD : public BAS_DRAW_FUN_JYX ,public AcDbEntity 
{
public:

	bool IsdrawTZD;
	bool IsdrawLC;
	bool IsdrawXLM;
	bool IsdrawBPD;
	bool IsdrawJDLX;
	bool IsdrawJD;
	//////////////////////////////////////////////////////////////////////////
	//�������ת��������
	//����ת�ֳ����
	void BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);//��ע������
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


	//////�����¶ȵ�
	BPDArray m_BPDArray;
	double m_CM[5*MAXBPDNUM];
	int m_NCM;//CM��������
	void TrsBPDArraytoCM();//���µ�����תCM ����
	void SetBPDArray(int NPD, PDTAB* BPDArr, bool IsAdd);
	// ����cml���ĸ߳�
	double rdes_level(double cml,double &slope);
	// ��CM�������cml���ĸ߳�
	double DLEVEL(int icm, double cml);
	//��ע���µ�
	bool BZCM(AcGiWorldDraw* pWd);
	//////////////////////////////////////////////////////////////////////////

	GuiFanCanShu m_GuiFanCS;

	bool IsEXRXD;
	struct JZXStruct
	{
		xlpoint QJZXStartPt,QJZXEndPt;//ǰһ��ֱ�ߵ����յ�
		xlpoint HJZXStartPt,HJZXEndPt;//��һ��ֱ�ߵ����յ�
		int JDNum;//��ǰ�����
	};

	CArray<JZXStruct , JZXStruct> JZXArray;//��ֱ������

	CArray<struct GJDInf,struct GJDInf> GJDInfTab;

	int NGJD;

	BOOL IsReSetColor ;

	struct GJDInforStruct
	{
		double spt[2];//�������
		double ept[2];//�յ�����
		int GJDType;//�Ľ������ͣ�0-���иĽ� 1-S�� 2-���иĽ�   // -�� - ��ȫ����
		ACHAR BXLName[256];
		ACHAR EXLName[256];

		ACHAR gjdname[256];//�ⲿ�Ľ�����

		ACHAR GJDBLC[50];//������
		ACHAR GJDELC[50];//�յ����

	};

	struct LYD 
	{
		double sml;
		double eml;
	};
	//ʵ��



	struct SetLineWeightStruct
	{
		AcDb::LineWeight m_LineWeight ;
		int m_XLKuanInt;
	};

	//������ɫ���߿�
	void setColorLineW();

	//������ɫ
	void SetAllColor(int ColorIndex);

	//�뾶�뻺���Ĺ淶���
	void RTolCheckUp( BOOL IsGaiJian );

	AcDb::LineWeight TranLineWeightFromIntToEnum( int LineWeight);

	//���ñ�׼����
	void SetStandardXLKuanFormat( CArray< SetLineWeightStruct , SetLineWeightStruct & >&  LineWeightArrayToOut );


	bool JudgeGTZX2GJDArrayChanged(CArray<struct GJDAllInforStruct,struct GJDAllInforStruct>& p_OriGJDArr,CArray<GJDAllInforStruct,GJDAllInforStruct>& p_NewGJDArr,CArray<int,int>& p_ChangedIndexArr);

	
	//ʵ����Ľ�����Ϣ����
	CArray<GJDInforStruct,GJDInforStruct>GJDInforArray;

	int GJDInforArrayNum;

	//	BOOL isQXGJ; //�Ƿ������߸Ľ�
	//	UltraCArray_T<GJDStruct,GJDStruct&> TempGJDJDArray;

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

	Adesk::Boolean G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);
	Adesk::Boolean G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);
	//  �ɶ������ݱ� ��ĳͳһ��̵��ֳ���̺͹ں�
	double   XLC( double TYLC,ACHAR *GH, int NDL);
	double   XLC( double TYLC,ACHAR *GH, int NDL,int &iDL);
	CString  XLC( double TYLC);
	//  �ɶ������ݱ� ��ĳ�ֳ���̵�ͳһ���
	double  TYLC(ACHAR ckml[80], int NDL );

	void  FormLCB(double dlc,double sml,double eml) ;
	void AddTZDtoLCB(); //�����������������̱� 
	//�����struct LCB��̱��
	void VIEW_PM_ZDM(int k);
	void VIEW_PM_PQX(int k);
	int PM_PQX(double *pqx);
	void outdata(CString str);
	Adesk::Boolean G_maketext(AcGiWorldDraw* pWd,AcGePoint3d& Pt, ACHAR* ctext,double ang,double texth ,int icolor,int mode);

	CString YCXLName , JRXLName ;

	int AllColor; //������ɫ
	int GTZXJDToJDColor;  //ƽ�潻��������ɫ

	int XWArcColor; //��λԲ��

	int HuanHequxianColor; //

	int JiaZhiXianColorZuo,JiaZhiXianColorYou; 

	int mdbnameBoxColor,mdbnameTextColor;


	int QXYSColor;


	AcDb::LineWeight  LineWeightValue ; 
	/*
	ACHAR BXLSXName[50];//�����·��
	ACHAR EXLSXName[50];//�յ���·��
	*/

	//����Ӱ�췶Χ���
	void  DrawYXFW(AcGiWorldDraw* pWd, ACHAR GTZXLC[50],ACHAR GJDGTZXLC[50],int flag,ACHAR GJDName[255]);

	//������·��ʽ
	AcDb::LineWeight SetXLKuanFormat( int XLKuanInt );
//	draw_DMX();


	int PM_DMX(double *dmx,double ZDMJG,double ZJ,double sml,double eml);
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
	CString XLShuXing;   //��·���ʣ����ߣ����� ����

	Adesk::Int16 Zx[2];//��GTZX������Zx[0],�ӷ���ZX[1],-1Ϊ�� ֱϵ����
	int Pxout[20];//��GTZX��������,���20��
	int nout;//��GTZX������������
	int Pxin[20];//��GTZX���뷽��,���20��
	int nin;//��GTZX���뷽������
	double Sprjml;//����������ڸ�����ͶӰ�������
	double Eprjml;//�������յ����ӷ���ͶӰ�������
	double SelSml;//ѡ���������뷽���Ŀ������,��ʼΪ�յ��������
	double SelEml;//ѡ���������������Ŀ������,��ʼΪ����������

	ACRX_DECLARE_MEMBERS(GTZX_JYX_ROAD);
	GTZX_JYX_ROAD();
	virtual ~GTZX_JYX_ROAD();


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

	//	Adesk::Boolean  GTZX::setBZLC(int iDL,double BLC,double DL, ACHAR *GH, int mode );

	virtual Adesk::Boolean   addDLM(int iDL,double BLC,double DL, ACHAR *EGH  );
	virtual Adesk::Boolean   DelDLM(int iDL);


	//��ӱ�־���
	void  addBZLCToGTZX(ads_point PT ,ACHAR CurBZLC[30]);

	//ɾ����־���
	void DeleteBZLCToGTZX(ads_point PT);

	//���뽻���±�� �����zh��hz������ zhorhz=-1 zh; zhorhz=1 hz
	Adesk::Boolean  getZhHzPt(int iJD,int zhorhz,double pt[2]);

	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const;
	virtual Acad::ErrorStatus dxfInFields(AcDbDxfFiler*);
	virtual Acad::ErrorStatus dxfOutFields(AcDbDxfFiler*) const;
	virtual int GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);
	virtual void subList() const;
	Acad::ErrorStatus  explode(AcDbVoidPtrArray& entityset);
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

	virtual Adesk::Boolean MakeGJDsToOne();
	int GetCurJDno(double N,double E);

	int mode_display;
//	ACHAR m_AddJDGH[20];
	//	double  JDarray[MAXJDNUM][6];  // 1->N,2->E,
	//	double QXB[MAXJDNUM][9];	
	/*	typedef	struct JDarrayStruct
	{ //����ṹ����
	ACHAR JDNum[20];
	double N;
	double E;
	double l1;
	double R;
	double l2;
	}JDarrayStruct;*/

	CArray<struct JDarrayStruct,struct JDarrayStruct>JDarray;


	GTZX_JYX_ROAD(int iNJD,CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH,ACHAR *JDchr,BOOL isConnectNewGTZXToIn = FALSE , BOOL isJXflag = 0 , BOOL IsGaiJian = FALSE );


	//������·��ɫ
//	void SetXLColor( CString XLNameToIn );

	//1 ��������ָ��,�Ľ�����Ϣ��,��ͨ����Ϣ

//	GTZX( int iNJD , CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH , ACHAR *JDchr , 
//		 CArray<GJDInf,GJDInf>& GJDInfTabArrayToIn);
	//GTZX( int iNJD,CArray<JDarrayStruct,JDarrayStruct>& iJDarray,ACHAR *GH,ACHAR *JDchr,BOOL isConnectNewGTZX = FALSE);
	//2����ͨ�ߵ�����\�Ľ� ��Ϣ��
	// 3 xlPoint_Pz����
	//��4�ԸĽ����׺���

	//5��ʼ����ֹ���ꡡ�Ϳ�ʼ����ֹ������ţ������ӵģǣԣڣ�ָ��

	double array_pxy[MAXJDNUM][4][5];// QXLRL[100][3];

	void GetArrayCS(CArray<struct QXBStruct,struct QXBStruct>&array,ACHAR CurJD[20],int &firstNum,int &AllNum);

	//*****************  
	double array[4*MAXJDNUM][10];//0����Ԫ����1-ֱ�ߣ�2-Բ��3-ǰ���� 4-��
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
	/*

	*/


	//���߱�
	CArray<QXBStruct,QXBStruct>QXB;

	BOOL isConnectNewGTZX ;

	//ԭʼ�����߱�
	//	CArray<QXBStruct,QXBStruct>YSQXB;

	//�������߱�
	//	CArray<QXBStruct,QXBStruct>JYQXB;

	//zh���,hy���,yh���,hz���,a,T1,T2,L,��ƫΪ-1(��1),�����
	Adesk::Int16 JYQXBNum;
	//JYQXB 

	bool m_IfViewDmx;
	//plhadd	zh���,hy���,yh���,hz���,a,T1,T2,L,��ƫΪ-1(��1),  //�����
	//[NJD][9]
	void DrawBZTZD(AcGiWorldDraw* pWd,int zybz,ACHAR str[]);
	xlpoint  PZtmp;
	int NewADDJDXiaoBiao;  //���������ӵ��±�

	CArray<struct BZLCStructTable,struct BZLCStructTable>BZLCarray;
	Adesk::Int16 NBZLC;

	AcDbObjectId GJDeId; //�Ľ���id
	Adesk::Int16 NJD,tarray;  
	Adesk::Int16 NDL,NH,NLC; //NDL ����������������̹ںŸı�㣬��CK12+235=C1K12+235��Ϊ����ֵ��0����������㣩
	// NLC--��̱���λ����=0  NH--�̱߳���λ����=0
	Adesk::Int16 Chinese_Engbz_type;  
	Adesk::Int16 JDTDBZ ;// JDTDBZ �����϶���־ ��JDTDBZ==1Ϊ�������϶�  =2 Ϊ��Բ =��������Ϊ�����϶�
	Adesk::Int16 draw_zybz,qxbbz;	//draw_zybz=-1 �����Ĵ�׮�����߲���������࣬=1Ϊ�����Ҳ�  =0Ϊ������ƫ���
	//qxbbz=1 Ҫ�����߲�������������
	Adesk::Int16 JD0; //��ʼ�����

	DLBZ DLB[MAXDLNUM];  // �����
	DLBZ DLBTMP[MAXDLNUM];
	//	BZLCStructTable BZLC[30];
	double texth;
	ACHAR JDGH[20];

	//  double Design_V;
	int KNDD;
	//    int BZTZD1 (double array[400][5],int trow, ACHAR *GCH,int NDL,double texth ,double LCB,int zybz,int Chinese_Engbz_type ,int NLC );  //  ��ע���
	//    int BZLC1(double array[400][5],int trow,ACHAR *GCH,int NDL,double texth,double dlc,double LCB,int zybz,int type) ; //  ��ע���;

	struct TZDLCB//��������̱�
	{
		int tzdflag;
		//���������ͣ����(0),�յ�(1),ZH(2),HY(3),QZ(4),YH(5),HZ(7)
		double lc;//ͳһ���
		double x,y;//ƽ������
		ACHAR bz[80];
	}pTzdlcb[5*MAXJDNUM];
	int NTZD;//��������
	//struct LCB//��̱��
	//{
	//	int lcflag;//������ͣ�-1����,1-9���ױ꣬0-�����
	//	double lc;//ͳһ���
	//	//		double x,y;//ƽ������
	//	//		ACHAR bz[80];
	//}pLCB[30000];
	struct LCB//��̱��
	{
		int lcflag;//������ͣ�-1����,1-9���ױ꣬0-�����
		double lc;//ͳһ���
		ACHAR ckml[20];//�ֳ����
		//		double x,y;//ƽ������
		//		ACHAR bz[80];
	}*pLCB;
	int NLCB;//׮��
	double StartCml,EndCml;
	ACHAR StartCKml[80],EndCKml[80];

	//����������sml��eml��Ķ�����,����DLno��
	void GetDLBno(double sml,double eml,CArray<int,int&>&DLno);
	void FindZXLenMin();//�ҳ���С��ֱ�߳�
	void checkpm();

	void BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb,int njd);
	//	BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb ,int trow);
	Adesk::Boolean BZLC(AcGiWorldDraw* pWd,double dlc, int zybz,int type);

	Adesk::Boolean BZLC2(AcGiWorldDraw* pWd,double dlc, int zybz,int type);   //��־��� 
	void BZTZD(AcGiWorldDraw* pWd, double dlc, int zybz,int type) ; //  ��ע���

	int INIT( BOOL IsGaiJian );
	Adesk::Boolean G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ, double ND);
	Adesk::Boolean G_makearc(AcGiWorldDraw* pWd,AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ );

	int BZTZD1 (double array[4*MAXJDNUM][10],int trow, ACHAR *GCH, int NDL,double texth ,double LCB,int bz_zybz,int Chinese_Engbz_type ,int NLC );  //  ��ע���
	int BZLC1(double array[4*MAXJDNUM][10],int trow,ACHAR *GCH,int NDL,int NLC,double texth,double dlc,double LCB,int zybz,int type);  //  ��ע���
	int BZJD1(CArray<JDarrayStruct,JDarrayStruct>& arrayd,CArray<QXBStruct,QXBStruct>& qxb ,int trow, double texth,int draw_zybz,int qxbbz,int NLC);
	void draw_pm();
	//-----------------------------
	void VIEW_PM_TypeHdm();
	void CalAJDT1T2(int ijd,double &T1,double &T2);
	double CalcmlTonewcml(double kmlTocml);
	double CalnewcmlTocml(double newcml);
	Adesk::Boolean BZTZD2(AcGiWorldDraw* pWd, double dlc, int bz_zybz,int Chinese_Engbz_type);  //  ��ע���
	double TYLC(double dml);
	void SetBZLCVal();

	void OnReCalBZLC(CArray<BZLCStructTable,BZLCStructTable>& m_BZLC);
	void CalT1T2L();
	//	void MergeQXBandJYQXBToQXB();
	void DrawPMXL(AcGiWorldDraw* pWd);
	void DrawPMCSSZ(AcGiWorldDraw* pWd);
	Adesk::Boolean DrawGTZXPMXL(AcGiWorldDraw* pWd);
	int GetPreOrAftJD(double N, double E,bool IsForwardTo);
	double DistFromPtToGTZX(double x, double y);//����һ�㵽��·����

	//////////////////////////////////////////////////////////////////////////

	void CreatJZXArray();//������ֱ������
	bool IsJZXCD(int iJD2,GTZX_JYX_ROAD *pGTZX,int QorH,int iSJD1, int iEJD1,int &iJD1);//��ֱ���Ƿ��ص�
	int SearchGJDSptIndex(int sjdn, int ejdn ,GTZX_JYX_ROAD *pGTZX,int &iJD1);//��Ѱ�Ľ�����㽻���±�
	int SearchGJDEptIndex(int sjdn, int ejdn ,GTZX_JYX_ROAD *pGTZX,int &iJD1);//��Ѱ�Ľ����յ㽻���±�
	int CreatGJDInfTab(GTZX_JYX_ROAD *pGTZX);//�����Ľ��α�

	int CreatGJDInfTab(GTZX_JYX_ROAD *pGTZXGJ,int JDShuxingArray[]);//�����Ľ�����Ϣ��

	int CreatBXGJD(GTZX_JYX_ROAD* pGTZXGJ,int GJJDStart,int GJJDEnd,int JYJDStart,int JYJDEnd,double spt[2],double ept[2]);//�������иĽ���

	void SortGJDArray();//�Ľ�������

	void MergeGJDArray();//�ϲ��Ľ���




	void SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256]);
//	void SetDefaultGjdName(int GJDType,ACHAR GJDName[256]);
	void CalGJDQZZB(int iJD1,int iJD2,int QDorZD, GTZX_JYX_ROAD *pGTZX, double &PtX, double &PtY);//����Ľ������յ�����
	void AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int IEJD1,int iEJD2,int GJDType);//��ӸĽ�����Ϣ
	void AddGJDInf(double spt[2],double ept[2],int iSJD1,int iSJD2,int IEJD1,int iEJD2,int GJDType,CString xln);//��ӸĽ�����Ϣ
	int GetGJDType(int sno1,int sno2,int eno1,int eno2,GTZX_JYX_ROAD *pGTZX);//��ȡ�Ľ�������
	bool isZXPX(double SPt1[2],double EPt1[2],double SPt2[2],double EPt2[2]);

	//���ù�ͨ���ֳ�������߱�
	void SetCKQXB(CArray<QXBStruct,QXBStruct>& CKQXBToIn);

	/////////////////////////////////////////////////////////////


	double GTZXFit(GTZX_JYX_ROAD *pGTZX,double &XMove,double &YMove,double &DegRotate);//����·ƥ�䣬Ѱ����ѵ�ƽ����������ת�Ƕ�
	double CalJDPC(CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray,double XMove,double YMove,double DegRotate);//���������������ж�Ӧ��������ƽ����

	void powell(double p[], double xi[], int n, int np, double ftol, int& iter, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	void eraseall(double pbar[], double prr[], double pr[]);
	void linmin(double p[], double xi[], int n, double& fret,CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double brent(double ax, double bx, double cx, double tol, double& xmin, CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	void mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc, CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double f1dim(double x , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	int sgn(double x);
	double func2(double x[], int n , CArray<AcGePoint3d,AcGePoint3d> &JYJDArray,CArray<AcGePoint3d,AcGePoint3d> &CLJDArray);
	double pcom[51], xicom[51];
	int ncom;
//	bool CreatJX(CArray<QXBStruct,QXBStruct> &QXBJYDC, GTZX *pGTZXZZJX);
//	GTZX_JYX* CreatJX(CArray<QXBStruct,QXBStruct> &QXBFromCL);

	//��ʶ��\��
	void DrawBandEPoints(AcGiWorldDraw* pWd );

	//��¼�Ľ��α�ע�ĵ�����
	CArray<AcGePoint3d , AcGePoint3d > MidPointArray;

	int MidPointArrayNum;

	void SearchGJD(double PickSPt[2], double PickEPt[2], CArray<GJDInf,GJDInf> &XGGJDInfArray, GTZX_JYX_ROAD *pGTZX,CString xln);

	void FormSectionInfor();

	CArray<GJDAllInforStruct,GJDAllInforStruct> BasicSectionInfor;//�����ֶ���Ϣ

	int xlpoint_pz_GTX(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ); 
	
	GTZX_JYX_ROAD * Get_GTX_RXGJD_GTZX(struct GJDAllInforStruct RXDBasicSectionInfor);

	CString GetGTXXclc(double N,double E);

	//�����ͨ��ƻ������߱����ֳ����
	void CalCKQXBToKmlForGTZX( CArray<QXBStruct,QXBStruct>& CKQXB );

	//�ж�Pt(N,E)���Ƿ��ڸĽ����ϣ��͸Ľ�������:-1-���� 0-���иĽ� 1-S��Ľ� 2-���иĽ�
	int GetPtGJDType(double N, double E);

	int NBRI;
	double Bri[200][2];

	int NTUN;
	double Tun[200][2];

	int NSTA;
	double Sta[100][2];

	void  AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT); //������վ������̱� 

	bool GetPtGJDInf(double dml, int &GJDType, CString &GJDStartJYXLName, CString &GJDEndJYXLName);

	//������������
	void CalNEForKml();

	//����̼�������
	void CalKmlForNE();

	//�������������ֳ����
	CString ModifyXCLCOnDL(double cml);

	//��ͶӰ��̼����ͨ��ƻ����ϵĲ����ںŵ��ֳ����dml
	double DmlGTX( double TYLC,ACHAR *GH, int NDL,struct DLBZ DLBTMP[MAXDLNUM]);

	double DmlGTX(double TYLC,ACHAR *GH);

	void SortBZLCArray();
	
	AcGePoint3d m_XLNameNotePt;

	double CalnewcmlTocml(double newcml , double & K );

	void getGJDInfo(double GJD_Spt[2],double GJD_Ept[2],CArray<GJDInf,GJDInf>& OneGJDArray);

	int GetPZFromXCLC1X(CString XCLC1x,struct xlpoint& PZ);
	
	void CalCKQXBToKmlForGTZX(); 

	double SearchNearDLOnGTX(double ProjCml,bool IsForwardTo,int& DLIndex);//������ͨ��ƻ�����ͶӰ���ΪProjCml�ĵ��������Ķ����ľ���

	void GetJYDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM]);//��ȡͶӰ�����Sml-Eml��Χ�ڵĶ���

	void GetGTXDLArray(double Sml,double Eml,int& DLSum, DLBZ DLBArray[MAXDLNUM]);//��ȡͶӰ�����Sml-Eml��Χ�ڵĶ���,������Sml��Eml

	int GetGT1xGJDIndex(double N,double E);//��ȡ��ͨ������ĳ��ĸĽ��������ţ�-1Ϊ���ö�

	int JudgePtIsOnDL(double Cml);//���뼸��ͳһ��̣��жϸĵ��Ƿ��ڶ�����


	void GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,CArray<int,int>& p_arrJDIndex,CArray<JDarrayStruct,JDarrayStruct>& p_arrJD);

	void CalXlcByProjml(double cml, ACHAR kml[40]);

};




#endif // !defined(AFX_GTZX_H__148D287E_47E1_4023_8291_68868AA7316B__INCLUDED_)


