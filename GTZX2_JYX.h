// GTZX2.h: interface for the GTZX2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTZX2_JYX_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
#define AFX_GTZX2_JYX_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GTZX_JYX.h"
#include "BAS_DRAW_FUN_JYX.h"
#include "UltraCArray.h"
#include "StdAfx.h"

#define B_X 0
#define R_X 1
#define S_StartPoint 2
#define S_EndPoint 3
#define B_XNOTP_X 4//plh05.05
//#define B_X_Des 5 //���� �뾶����Ҫ����
#define R_X_OneR 6 //���е�
#define R_XStart 7
#define R_XEnd 8
#define MAXDLNUM2x 100
#define MAXJDNUM2x 100
#define LEFTLINE -1
#define RIGHTLINE 1
//DLֵ->����Ϊ��������Ϊ�����������BLC+DL=ELC,DL=ELC-BLC;
struct GuiFanCanShu
{
	ACHAR TieLuDengJi[24];//��·�ȼ�1
	int SuDuMuBiao;//�ٶ�Ŀ��2
	short ZhXianShu;//������Ŀ3,1��ʾ���ߣ�2��ʾһ��˫�ߣ�3��ʾ˫��Ԥ������
	float XianJianJu;//�߼��4
	int MinR1;//һ����С�߰뾶5
	int MinR2;//������С�߰뾶6
	float XianZhiPo;//�����¶�7����
	float XianZhiPo1;//�����¶�7����
	int DaoFaXian;//������8
	short GuiDao;//������ͣ�0-4���޷���-�޷��ᣬ5-9���з���-�з���
	short QianYin;//ǣ�����ࣺ1-������2-��ȼ��3-����
	ACHAR JiChe[24];//��������11
	ACHAR BiSai[24];//����12
	ACHAR KongZhi[24];//�г����Ʒ�ʽ13
	ACHAR ZhiHei[24];//�г�ָ�ӷ�ʽ14
	short RXiangShu;//���õİ뾶ϵ�����������20
	int R[20];//���߰뾶ϵ��
	int L[20];//�������߳������ã������߰뾶һһ��Ӧ��R[i]��L[i]
	int TJianR1;//�Ƽ��뾶С
	int TJianR2;//�Ƽ��뾶��
	int MinZhi;//��Сֱ�߳���
	int MinYuan;//��СԲ���߳���
	int JiaKuan[20];//���߼ӿ������߰뾶һһ��Ӧ��R[i]��JiaKuan [i]
	int MinPoChang[7];//��С�¶γ��ȣ���Ӧ��6�ֵ����߳���
	int MaxPoCha[7];//����²�
	float ShuTiaoJ;//����������
	int ShuR;//�����߰뾶
	bool ZheJian;//�����ۼ�
	int SuiDaoFJ[4];//������ȷּ�
	float SDXiShuD[5];//����ۼ�ϵ����������
	float SDXiShuN[5];//����ۼ�ϵ������ȼ��
	float JiBiaoHou;//���������
	float WuFJiaK;//·�����޷�ӿ�
	float LuJiM1;//����·��·����
	float LuJiM2;//����·ǵ·����
	float LuJiM3;//˫��·��·����
	float LuJiM4;//˫��·ǵ·����
	short QuXianFJi;//���߰뾶�ּ���5
	int QuXianRFJ[5];//���߰뾶�ּ�
	float LuJiMJiaK[6];//·�������߼ӿ�ֵ
	ACHAR SheJiJieDuan[24];//��ƽ׶�
};
class GTZX2_JYX_ROAD : public GTZX_JYX_ROAD
{		

public:

	//////////////////////////////////////////////////////////////////////////
	//�������ת��������
	//����ת�ֳ����
	bool TrsNEtoCkml(double N, double E, ACHAR Ckml[], int NLC=3);
	bool TrsNEtoProjml(double N, double E, double& Projml);
	bool TrsNEtoCml(double N, double E, double& Cml);
	bool TrsCkmltoNE(ACHAR Ckml[], xlpoint& PZ);
	bool TrsCkmltoProjml(ACHAR Ckml[], double& Projml);
	bool TrsCkmltoCml(ACHAR Ckml[], double& Cml, int iRXD=-1);
	bool TrsCmltoNE(double Cml, xlpoint& PZ);
	bool TrsCmltoCkml(double Cml, ACHAR Ckml[], int NLC=3);
	bool TrsCmltoProjml(double Cml, double& Projml);
	bool TrsProjmltoNE(double Projml, xlpoint& PZ);
	bool TrsProjmltoCml(double Projml, double& Cml);
	bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC=3);

	void SetiRXDBPDArray(int iRXD, int NPD, PDTAB* BPDArr, bool IsAdd);
	void Create2XGTBPDArray(APDTAB&BPDArray);
	void Create2XGTBPDArray();
	UltraCArray_T<BPDArray,BPDArray> AllRXDBPDArray;

	//�γɶ��߹�ͨ������
	void Create2XGTDLB();
	//��ע���µ�
	bool BZCM(AcGiWorldDraw* pWd);
	//���߹�ͨ��������
	int m_2XGTDLSum;
	//�����־���
	void SetBZLCVal();

	BOOL isJYGuiFanCS;
    GuiFanCanShu m_GuiFanCS;
	bool m_IsNeedCalGJDInf;//�Ƿ���Ҫ���¼����ͨ���߸Ľ�����Ϣ
	BOOL isGJD;
	double TYLC2x(CString XCLC2x,int iRXD=-1);//plh08.20//��iRXD��0��ָ�����жβ���
	//plh04.14
	EXDLB DLB2x[MAXDLNUM2x];
	GTZX_JYX_ROAD *m_pm1;//һ��ƽ��
	DoubleLineData JD2[MAXJDNUM2x];
	int LEFT_OR_RIGHT;//�����߱�� (LEFTLINE -1) (RIGHTLINE 1)
	bool calsflag;


	//	CArray<GJDInf,GJDInf> GJDInfTab;

	//	CArray<EXGJDFDInf,EXGJDFDInf>GJDFDInfTab; //�Ľ��ηֶ���Ϣ��
	//	int NGJDFDInfTab;//�Ľ��ηֶ���
	//	CArray<GJDInforStruct,GJDInforStruct>GJDInforArray2;


	int NGJD;
	int zory;//������߻�������� -1 ����,1 ����
	ACHAR xlname[256];
	int m_LJorGM;
	int m_Xsw;
	int m_DesHXSW;
	//	double  JDarray[MAXJDNUM][6];  // 1->N,2->E,
	//	CArray<JDarrayStruct,JDarrayStruct>JDarray;
	//	double QXB[MAXJDNUM][9];	
	//	CArray<BAS_DRAW_FUN::QXBStruct,BAS_DRAW_FUN::QXBStruct>QXB;
public:

	GTZX_JYX_ROAD *CreateGtzx();
	//���������жζ��������һ�߶�����ϲ�Ϊһ��������	
	int CreateGtzxDLB();
	//�޸Ľ������ʣ�������������������н������ʱ�Ϊ���С�
	void ModifyJDXZ(int iRXD);

	//�������ж����յ������
	void Modify_RXD_SE_Coords(DoubleLineData p_JD2Arr[],int p_iJDIndex,DoubleLineData *p_JD);




	int GetiRxdNoFromTYLC1x(double lc1x);

	/////////////////////////////////////////////////////////
	void ModifyRXDName(double N, double E);
	void out();
	GTZX2_JYX_ROAD(AcDbObjectId Id1x, double xjj , BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD(GTZX_JYX_ROAD *gtzx, double xjj , BOOL IsGaiJian = FALSE );

	GTZX2_JYX_ROAD ( GTZX2_JYX_ROAD *gtzx2 , double xjj , BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD(GTZX2_JYX_ROAD *pm2,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[] , BOOL IsGaiJian = FALSE );
	//S�����
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
	void Insert2XDLB(int iRXD,EXDLB *ExDlb, int DlNum); //���룲�߶���
	void Update2XDLB(int iRXD,EXDLB *ExDlb, int DlNum);//���£��߶���
	void GetDLTYLC();
	void SetDLB(EXDLB *Dlb,int ndl);
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
	//	bool AddRXD(double (*jdarray)[6],double (*qxb)[9],int njd,double N,double E,ACHAR EGH[]);//plh04.11.25
	//bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[], BOOL PreMove = FALSE );//plh04.11.25

	//	void AddRXD(AcGePoint3d PT1,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove = FALSE ) ;//������������

	int getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );

	//	void AddRXD(AcGePoint3d PT1,double (*jdarray)[6],int &njd);
	// void AddRXD(AcGePoint3d PT1,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd);  //������������
	void AddRXD(AcGePoint3d PT1,AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove =FALSE ); //������������

	void AddRXD(AcGePoint3d PT1,AcGePoint3d PT2,AcGePoint3d OutPt,CArray<JDarrayStruct,JDarrayStruct>& jdarray,int &njd,BOOL isPreMove=FALSE );//������������

	bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[],ACHAR JDGH[] ) ; //plh04.11.25

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

	void GetiRxdBZLCArray(int iRXD , CArray<BAS_DRAW_FUN_JYX::BZLCStructTable,BAS_DRAW_FUN_JYX::BZLCStructTable> & iRxdBZLCArrayToOut );

	//ɾ��ָ�����ж�
	void DeleteiRXDDLB(int iRXD);

	//��ȡ���жε�����
	int GetRXDNum();

	//�õ�ָ�����жε����ս������

	void GetJDXHofiRXD(int iRXD,int &firstJDXH,int &endJDXH);

	//�����������жε�EXDLB
	void Create2XDLB(int iRXD,ACHAR EGH[]=NULL,bool IsUpdate=true);//plh04.11.25

	//�����������жε�EXDLB
	void Create2XDLB();

	void Generate2XDLB(int p_iRXD,CArray<EXDLB,EXDLB> &p_arrDLBNew);

	double T0min;//��С��ֱ�߳�
	double Lrmin;//��СԲ���߳�
	void GetT0min_Lrmin();
	void UpdateData(bool IsCreateFirst=true);
	void UpdateDataRXD(int iRXD);//���¹�ͨ����ĳ���ж�����


	void UpdateDataSeveralRXD(CArray<int,int> &p_arrChangedRXD,UltraCArray_T<RXDInfo,RXDInfo> &p_arrNewRXD);








	void InsertDateRXD(int iRXD);//����һ�����жκ�ֻ���¹�ͨ���߸����ж�����
	//	GTZX2(GTZX *pm1,double[][2],int njd2,ACHAR jdgh[],int jdo);//һ�����ݣ�һ�߽���ź��߼�࣬���߽���ںţ���ʼ������
	GTZX2_JYX_ROAD(GTZX_JYX_ROAD *pm1,int njd2,DoubleLineData jd2[],int jdo,ACHAR jdgh[], BOOL IsGaiJian = FALSE );
	GTZX2_JYX_ROAD();
	virtual ~GTZX2_JYX_ROAD();
	void CalJDdata();//�Զ�����뾶������
	void CalJDdata1();//������뾶������
	void CalNYDL();
	bool CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
			double& BanJing2,double& HuanChang2);
	double CalS(double R,double Lh,double T0,double xjj1,double xjj2,ACHAR ZHLC[80],double &x1,double &y1,double &x2,double &y2);
	static double Lh_Rtop(double Lh,double R);
	ACRX_DECLARE_MEMBERS(GTZX2_JYX_ROAD);
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw* mode);
	Adesk::Boolean G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,bool ISShow=true);
	//	void BZJD(AcGiWorldDraw* pWd,double arrayd[500][6],double qxb[500][9],int tarray,int JD0,ACHAR *JDchr);
	void BZJD(AcGiWorldDraw* pWd,CArray<JDarrayStruct,JDarrayStruct>& JDarray,CArray<QXBStruct,QXBStruct>& qxb,ACHAR *JDchr);

	//����һ����̣������߼�࣬�����϶�Ӧ������̼���,1-��ʾ�ɹ���0-ʧ��
	//  int CalXjj(double ml1,double &xjj,double &projml,double resPt[2]);
	void CalEXJD(int jdn1,int njd2,double EXJD[2]);//����һ�߽���ţ����߶�Ӧ����ţ����ز��жζ��߽�������
	int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	//   int Judge2XLorR(xlpoint PZ);
	void SetJDdata();//���ɶ���JDarray
	static double GetAfaFromR_Lh_T0_dBJ(double R,double Lh,double T0,double dbj,double &T);
	double CalTFromR_Lr_Lh(double R,double Lr,double Lh); 
	double CalT0andLjiaFromR_Lr_Lh_d(double R,double Lr,double Lh,double dbj,double &T0);		
	void BZLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type,double (*xyarray)[10],int &tary,EXDLB *iExDlb,int ndl , GTZX_JYX_ROAD *pSubGtzxToIn);


	CArray<Gtzx2BZLCStruct,Gtzx2BZLCStruct>AllBLZCArray ;

	void BZTZD(AcGiWorldDraw* pWd,int draw_zybz,double (*xyarray)[10],int nxy,bool IsOnlyB_S=true);//��ע���������

	Adesk::Boolean BZTZD2(AcGiWorldDraw* pWd,double dlc,int zybz,int type);  //  ��ע���

	virtual Adesk::Boolean addDLM(double tylc,double BLC,double ELC, ACHAR *EGH , BOOL IsSingleModifyR =FALSE );
	virtual Adesk::Boolean   DelDLM(double tylc);
	//plh04.14	
	virtual Acad::ErrorStatus dwgInFields(AcDbDwgFiler*);
	virtual Acad::ErrorStatus dwgOutFields(AcDbDwgFiler*) const; 
	virtual Acad::ErrorStatus subGetGripPoints(AcGePoint3dArray& gripPoints,
		AcDbIntArray& osnapModes,
		AcDbIntArray& geomIds) const;

	virtual Acad::ErrorStatus subMoveGripPointsAt(const AcDbIntArray& indices, 
		const AcGeVector3d& offset);

	//�����Ƿ�����������иĽ�����,-1Ϊû��
	int GetGJDNum(double N,double E);
	//������ͨ��ƶ�����N��E�����ȡ�õ����ڸĽ��ε������ţ����öη���-1
	int GetGT2xGJDIndex(double N,double E);
	//����ȱʡ�Ľ�����
	void SetDefaultGjdName(double N,double E,int GJDType,ACHAR GjdName[256]);
	//�б�����Ľ�������иĽ����Ƿ���ڰ�����ϵ
	bool IsIncludeByOtherGjd(double spt[2],double ept[2]);
	bool IsJdInGTZX(int iJD);
	int SearchGJDEptIndex(int sjdn, int ejdn);
	int SearchGJDSptIndex(int sjdn, int ejdn);
	int CreatGJDInfTab();	
	CString CalGJDLC(double N,double E);//�����ע���ж�
	void BZSTR(AcGiWorldDraw* pWd,xlpoint PZ,CString str1,CString str2); 
	void BZGJD(AcGiWorldDraw* pWd);//��ע���иĽ���
	void BZiGJDLC(AcGiWorldDraw* pWd,int iGJD);//�����ע���ж�
	Adesk::Boolean  getZhHzPt(int iJD,int zhorhz,double pt[2]);

	void DrawXL(AcGiWorldDraw* pWd , ACHAR CurXLName[256]);

	void CheckJD2IsAvailable();
	// ����IRXD�Ļ���ָ��
	GTZX_JYX_ROAD * GetiRxdtoGTZXplinp(int irxd );

	void SetBZLCArray( BZLCStructTable *BZLCTableToIn , int AddBZLCNumToIn );

	void Insert2xBZLC(int iRXD,CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable>& BZLCArrayToIn );

	void GetiRXDBZLCinfo(int iRXD, int &SBLZCNum, int &EBLZCNum);

	void addBZLCToGTZX2(ads_point PT ,ACHAR CurAddBZLC[30] ,CString iRxdXLName );

	int CreatEXGJDInfTab(GTZX_JYX_ROAD* pGTZXJYJX, GTZX_JYX_ROAD* pGTZXJYYX, GTZX_JYX_ROAD* pGTZXJYZX, CArray<EXGJDFDInf,EXGJDFDInf&> &EXGJDFDInfTab);
	GTZX2_JYX_ROAD *m_pm2;
	/*
	virtual Acad::ErrorStatus getOsnapPoints(
	AcDb::OsnapMode   osnapMode,
	int               gsSelectionMark,
	const AcGePoint3d&    pickPoint,
	const AcGePoint3d&    lastPoint,
	const AcGeMatrix3d&   viewXform,
	AcGePoint3dArray& snapPoints,
	AcDbIntArray&     geomIds) const ;
	*/

	//�γɹ�ͨ���2�߷ֶ���Ϣ����
	//	void FormSectionInforForGTZX2();

	//�õ���ͨ���2�ߵ��ֳ���̣�����NE����
	CString GetGTX2Xclc(double N,double E);

	//��������̼����ֳ����
	CString GetGTX2XclcFromcml(double cml);

	//�������߱����ֳ����
	void CalCKQXBToKml(CArray<QXBStruct,QXBStruct>& CKQXB);


	//��ͨ���2�߱�ע��㣯�յ�
	void DrawBandEPointsForGTZX2(AcGiWorldDraw* pWd );


	////�������жα�ŵõ����ж�ƽ��ָ�� zj 07.7.4
	//GTZX* GetiRxdtoGTZXplinp(int RXDno);
 
	//����2��ͳһ��̵õ����Զα�� zj 07.7.5
	int GetiRxdNoFromTYLC2x(double lc2x);

	//�������ж����õ����Զα�� zj 07.7.15
	int GetiRxdNoFromRXDName(CString RXDName);

	//������һ��,������������յ�Ľ���� zj 07.7.16
	void GetiRXDJDArray(int iRXD, double jday[][6], int &njd,int &SJDNum,int &EJDNum);

	CString GetRXDGTXXclc(double N,double E);

	void DrawBZLC(AcGiWorldDraw* pWd);

	void TrsBPDArraytoCM();
	void DrawOneBZLC( AcGiWorldDraw* pWd ,GTZX_JYX_ROAD * pSubGtzxToIn ,int CurNum );

	//�õ���ͨ���2�߸Ľ��ֶ�����
	void GetGTXSJ2gjdInforArray(GTZX_JYX_ROAD *pGT1x , CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2);


	//ϸ���Ľ��ηֶ�����
	void GetGTXSJ2Allgjdinfor(CArray<GJDInforStruct,GJDInforStruct&>& GJDInforArrayForGTXSJ2ToIn , CArray<GJDInf,GJDInf&>& GJDInforArrayForGTXSJ2ToOut);

	//�õ�ϸ����ĸĽ���ָ��(���߸�ʽ)
	GTZX_JYX_ROAD  * GetOneGTXSJ2TopLinep(GJDInforStruct CurGJDInforToIn );

	//�ж�Pt(N,E)���Ƿ��ڸĽ����ϣ��͸Ľ�������:-1-���� 0-���иĽ� 1-S��Ľ� 2-���иĽ�
	int GetPtGJDType(double N, double E);

	void GetiRxdStartEndMlByNum(int irxd,double &ProjSml,double &ProjEml);

	//��ȡ��ͨ��ƶ��������жΣ�������ת��Ϊ��ͨ��ƻ���ָ��
	GTZX_JYX_ROAD * GetiRxdtoGTZXplinp_GTX(int irxd);

	//�����ͨ����ߵ��߼��
	double CalGTXXJJ(double cmlOnGT1X, GTZX_JYX_ROAD *pJYJX, GTZX_JYX_ROAD* pGTJX, GTZX2_JYX_ROAD* pGTEX,double& XJJ1, double&XJJ2);


	bool AddMSRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray,CArray<QXBStruct,QXBStruct>& qxb,int njd,double N,double E,ACHAR EGH[]);

	//�����жα�Ż�ȡ���ж���·��
	CString GetRXDXLNameByRXDIndex(int RXDIndex);

	int GetRXDIndexByRXDXLName(CString RXDXLName);

	void GetiRXD1xTYLC(int iRXD,double &startLc,double &endLc);

	//��־iRxd��־���
	Adesk::Boolean  BZLC2(AcGiWorldDraw* pWd,GTZX_JYX_ROAD * pSubGtzx ,int zybz , int type )  ;

	//1.�ж��Ƿ���ӳ���������
	// JDNum -- ���߽����
	BOOL JudgeCurJDIsOnXL( int JDNum , BOOL IsBefore );

	//�������߰��ױ������
	void ModifyExNEForHundredMeter( double & N, double & E , BOOL IsForWard );

	//�������ж��߰��ױ������
	void ModifyAllExNEForHundredMeter( );

	int  GetGTX2XJD(double N, double E ,BOOL IsForWard);
	void  GetGTX2XclcFromcml(double cml,double &dml,ACHAR gh[8]);


	void DeleteiRXDBZLC(int iRXD);

	//������������
	void CalNEForKml2x();

	//����̼�������
	void CalKmlForNE2x();

	//  ɾ��һ����־���
	void DeleteBZLCToGTZX2(ads_point PT);

	Adesk::Boolean BZRxdZD(AcGiWorldDraw* pWd,double dlc,int zybz,int type , GTZX_JYX_ROAD * pSubGtzx );  //  ��ע���

	void SetDefaultiRxdBZLC(int iRXD , CArray<BAS_DRAW_FUN_JYX::BZLCStructTable ,BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCarrayToOut );

	void SetBZLCArray( CArray<BAS_DRAW_FUN_JYX::BZLCStructTable , BAS_DRAW_FUN_JYX::BZLCStructTable> & BZLCArrayToIn );

	bool AddRXD(CArray<JDarrayStruct,JDarrayStruct>& jdarray , ACHAR EGH[],bool IsRX = true, bool IsHBQX = false);

	void GetNEToGtzx2xkml( CString kml2x , double & NToOut , double & EToOut );

	void DeliRxdBZLCArray(int iRXD);

	//////////////////////////////////////////////////////////////////////////
	//���ɹ�ͨ����߸Ľ���
	void CreateLYDArray(double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	//���������������߼�ֱ���ص����ֵĽ����±��
	bool SearchJZXCDJDIndex_OnJYJX(GTZX_JYX_ROAD* pJYJX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1);

	bool SearchJZXCDJDIndex_OnJYEX(GTZX2_JYX_ROAD* pJYEX,int iJD2,int QorH,int iSJD1, int iEJD1,int &iJD1);

	//������ֱ������
	void Create2XZJXArray();

	//��λ���öεİ��ױ�
	void LocateHundredMeterOnLine_LYD(double Pt[2],bool IsBefore);

	//�������иĽ�������
	void CreateRXGJD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>& RXGJDArray);

	void CreateGJDInGT2XRXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateGJDInGT2XBXD(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateGJDPt_By_LYDPt(double Spt[2],double Ept[2],CArray<GJDInf,GJDInf>&  LYDArray,CArray<GJDInf,GJDInf>& GJDArray);

	void CreateAllGJD(CArray<GJDInf,GJDInf>& BXGJDArray,CArray<GJDInf,GJDInf>& RXGJDArray,CArray<GJDInf,GJDInf>& AllGJDArray);

	void RefreshGTZX2DLArray(double Sml, double Eml,int OriNDL, EXDLB OriEXDLB[], int NewNDL, EXDLB NewEXDLB[],int& RefreshNDL, EXDLB RefreshEXDLB[]);

	void RefreshGTZX2GJDArray(double Sml, double Eml, CArray<GJDInf,GJDInf>& OriGJDArray, CArray<GJDInf,GJDInf>& NewGJDArray, CArray<GJDInf,GJDInf>& RefreshGJDArray);
	
	void RefreshExFDArray(double RefreshSml,double RefreshEml,int iRXD);

	void ModifyiRXDDLB2xByGJDArray(int iRXD);
	
	void SortGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray);

	void MergeGTZX2GJDArray(CArray<GJDInf,GJDInf>& AllGJDArray);

	void CreateLYDArray_OnJYJX(GTZX_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	void CreateLYDArray_OnJYEX(GTZX2_JYX_ROAD* pGTZXJY,double Spt[2], double Ept[2], CArray<GJDInf,GJDInf>& LYDInfArray);

	void CreateLYDAndBXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& LYDArray,CArray<GJDInf,GJDInf>& BXGJDArray);

	void CreateGTJXGJDArray(double spt[2],double ept[2],CArray<GJDInf,GJDInf>& AllGJDArray,CArray<GJDInf,GJDInf>& LYDArray);
	//////////////////////////////////////////////////////////////////////////

	//���ֳ���̻�ȡ��Ӧ�ĸĽ��Σ������λ�ڲ��ж�ʱ����-1
	int GetRXDIndexByXCLC2X(CString XCLC2x);

	//���ֳ���̻�ȡͶӰ��PZ
	int GetPZFromXCLC2X(CString XCLC2x,struct xlpoint& PZ);
	
	double CalZXNYDL(double sml,double eml);//�������յ���̼���ֱ�߶���ҵ���� zhongjing

	bool Get_S_Afa(double R1,double L1,double R2,double L2,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2);

	bool CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2,double JZXLen,double ZH_N,double ZH_E,double Fwj,double& afa,double &x1,double &y1,double &x2,double &y2);

	bool SelSModel(double N, double E,int& SJD, int& S_JD1,int& S_JD2);//ѡ��S��

	void ModifyDLB2xByGJDArray();//�ɹ�ͨ��ƶ��ߵĸĽ�����Ϣ���Ӷ��߶�������

	void CalGT2xDLValue();//�����ͨ���ߵĶ������ļ���ͳһ���

	void GetGT2XDLArray(double Sml,double Eml,int& DLSum, EXDLB DLBArray[MAXDLNUM]);//��ȡͶӰ�����Sml-Eml��Χ�ڵĶ���,������Sml��Eml

	int JudgePtIsOnDL(double Cml);//���뼸��ͳһ��̣��жϸĵ��Ƿ��ڶ�����

	int JudgePtIsOnRXD(double N,double E);//�ж�ĳ���Ƿ������ж���

	void CalGJDStartEndXCLC(GTZX_JYX_ROAD::GJDInforStruct& GJD);//����Ľ������յ��ڼ������ϵ��ֳ����

	void setColorLineW();//������ɫ�߿�

	void InfluenceOfGJDModifyToRXD(CArray<GJDAllInforStruct,GJDAllInforStruct> &p_arrOriBasicSectionInfo,CArray<int,int> &p_arrInfluRXD);

	void CalCKQXBToKml();

	void FormLCB2X(double sml,double eml,double Step);
	void FormSectionInfor();//�γɷֶ���Ϣ����

	typedef struct
	{
		double Sml;
		double Eml;
		int RXDIndex;

	}Sec;

	//----------------------------------------------------------------------------
	typedef UltraCArray_T<GJDAllInforStruct,GJDAllInforStruct> GJDArrToOneRXD;
	typedef struct  
	{
		GJDArrToOneRXD GJDArr;
		int iRXDIndex;
	}GJDArr_RXD;

	UltraCArray_T <GJDArr_RXD,GJDArr_RXD> m_arrOldGJDArrToRXD;
	UltraCArray_T <GJDArr_RXD,GJDArr_RXD> m_arrNewGJDArrToRXD;
	//------------------------------------------------------------------------------


	void GetJDIndexArrayByPts(ads_point p_sptDwg,ads_point p_eptDwg,CArray<int,int>& p_arrJDIndex,CArray<DoubleLineData,DoubleLineData>& p_arrJD);


	//-----------------------------------------------------------------------------


	//-----------------------------------------------------------------------------


};

int MAKE_ACDBOPENOBJECT_FUNCTION(GTZX2_JYX_ROAD);
#endif // !defined(AFX_GTZX2_H__E55358F8_0D01_4A12_9C8C_EA644FDC9116__INCLUDED_)
