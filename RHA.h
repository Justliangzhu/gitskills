#pragma once

#ifdef PINGMIAN_MODULE
#define DLLIMPEXP __declspec(dllexport)
#else

#define DLLIMPEXP
#endif

//-----------------------------------------------------------------------------
#include "dbmain.h"
#include "malloc.h"
#include "time.h"
#include "BAS_CALC_FUN.H"
#include "BAS_DRAW_FUN.h"
#include "Struct.h"
#include <vector>
#include "PlaneDraftPrefer.h"
//#include "GetAllVertData.h"

using namespace std;

//��������
#define BXPX        0   //����ƽ��(���㲻���϶�)
#define BX_NotPX    1   //���в�ƽ��
#define RX_Start    2   //����ʼ
#define RX_One      3   //������
#define RX_End      4   //����ֹ
#define S_Start     5   //S��ʼ
#define S_End       6   //S��ֹ 

//#define	BLJD        10  //���߽���
//����������һ������ʼ��ʾ

//��·����������·���ԣ�
#define JYJX        L"���л���"
#define JY2X        L"���ж���"
#define JY3X        L"��������"
#define JY4X        L"��������"
#define SJJX        L"��ƻ���"
#define SJ2X        L"��ƶ���"
#define SJ3X        L"�������"        
#define SJ4X        L"�������"
#define LSX         L"��ʱ��"
#define SGBX        L"ʩ������"
//����Ϊ��·����ͬʱ��·�����������ԣ����л��� ���ж��� ��ƻ��� ��ƶ��ߣ�Ҳ��JYJX�ȱ�ʾ

#define DefaultMdbNm L"C:\\δ����.MDB" //��ʱ��Ĭ�����ݿ���

#define LEFTLINE -1
#define RIGHTLINE 1
#define MAINLINE 0

#define TUNMIDLINE 0
#define CONTINUELINE 1
#define RAILLEGEND	2

#define InOneRXD    1
#define ConTwoRXD   2
#define QHBX_AddBX  3
#define QHBX_AddRX  4
#define QBHR        5
#define QRHB        6

//-----------------------------------------------------------------------------

class DLLIMPEXP RHA : public BAS_CALC_FUN ,public BAS_DRAW_FUN ,public AcDbEntity {

public:
	ACRX_DECLARE_MEMBERS(RHA) ;

protected:
	static Adesk::UInt32 kCurrentVersionNumber ;

public:	
	int m_iDoc;//�ĵ������±�,ֻ��ȫ�ֱ����ã��Ծֲ���·��ʩ�����ߺ���ʱ�ߣ�û�� 818 �ñ����ǲ����Ѿ���¼��ȫ�������У�����Ҫ��RHA��¼;�������ٸñ���ȥ��ȡȫ������
	CString m_XLName;//��·��:���л��ߡ����ж��ߡ��������ߡ�����ƻ��ߡ���ƶ��ߡ���
	CString XLShuXing;   //1-���л��� 2-���ж��ߣ��磺�����������ߣ�3-��ƻ��� 4-��ƶ��� 
	//5-��ʱ�� 6-ʩ������
	//Old   ��·���ʣ����ߣ����� ����
	CString m_mdbName;//�Զ���ʵ���Ӧ��mdb

	double xl_length;//��·����

	vector<JDarrayStruct> JDarray;//��������

	//���߱�
	//CArray<QXBStruct,QXBStruct>QXB;
	vector<QXBStruct> QXB;
	CArray<OneJZXStruct , OneJZXStruct> lsJZXarray;//��ֱ������(�����ߺ������) 818 �Ժ����
	CArray<JZXStruct , JZXStruct> JZXArray;//��ֱ������//818 �����޸�ΪlsJZXarray



	// QXLRL[100][3];

	//double array[4*MAXJDName][10];//0����Ԫ����1-ֱ�ߣ�2-Բ��3-ǰ���� 4-��

	//DoubleArray3d array_pxy;//array_pxy[NJD][4][5] ��ʱ��Ԫ����//�ĳ�һά�������ͷ�
	vector<vector<double>> array;//array[4*NJD][10] ��Ԫ����
	Int16  tarray;//��Ԫ����

	vector<double> array_pxy;//array_pxy[NJD][4][5] ��ʱ��Ԫ����//�ĳ�һά�������ͷ�
	//int tarray_pxy;//array_pxy����Ŀ

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
	Int16 JD0; //��ʼ����� 818 û��Ҫ
	ACHAR JDGH[20];   //����ں�
	BOOL isGJD;//818 �Ƿ�Ľ� û��Ҫ��



	vector<DLTab> DLarray;//��������

	vector<BZLCStructTable>BZLCarray;
	
	//��¼�Ľ��α�ע�ĵ�����
	CArray<AcGePoint3d , AcGePoint3d > GJDNotePtAry;
	//ʵ����Ľ�����Ϣ����
	vector<GJDInf> GJDInforArray;//�����ߴ洢�������ö����飬����ߴ洢����ȫ�߷ֶ�����

	//vector<RXDInf> ZMRxdAry;//�������жα� ���ж��ߺ���ƶ����������ж����顣 ע�⣺������ֻ���������ʱ�� //818�����ж����Ƿ񴢴��˵����꣬Ӧ�ô�
	

	vector<FZZDM> FZZDMAry;
	vector<FZZDM> ZMRXDArray;//����������

	//ǰһ���������顢�������顢��־�������
	vector<JDarrayStruct> OldJDarray;
	vector<DLTab> OldDLarray;
	vector<BZLCStructTable> OldBZLCarray;

	vector<Switch> m_SwitchArray;//����
	//vector<QXBCmlStruct> QXBCmlAry;//ƽ�����߱������������������ 



	//CArray<OneJZXStruct , OneJZXStruct> JZXArray;//��ֱ������,���ڽ������С����ж�

	BOOL IsReSetColor;//�Ƿ�����������ɫ
	Int16 AllColor; //������ɫ
	Int16 GTZXJDToJDColor;  //ƽ�潻��������ɫ
	Int16 XWArcColor; //��λԲ��
	Int16 HuanHequxianColor; //
	Int16 JiaZhiXianColorZuo,JiaZhiXianColorYou;//���ֱ����ɫ���Ҽ�ֱ����ɫ 
	Int16 mdbnameBoxColor,mdbnameTextColor;//��·�����ο���ɫ����·����ɫ
	Int16 QXYSColor;//����Ҫ����ɫ

	PlaneDraftPrefer DrawParam;//��ͼ���� ע�⣺�ڻ����Զ���ʵ��ʱ�õ������� DrawParam.m_dLC��DrawParam.NLC �߿� ��ע�ֵĸ߶�
								                                   //818 ����ƽ���Զ���ʵ���
	
	/**/

	/*-------------------------�淶--------------------------*/
	double DV;//�ٶ�Ŀ��ֵ  818 ʲôʱ��ֵ
	//double T0min;//��С��ֱ�߳�
	//double Lrmin;//��СԲ���߳� 818 ��ʱ��bas_calc_fun

	vector<double> m_Rs;//���߰뾶ϵ��  R[20]
	vector<double> m_l0s;//�������߳������ã������߰뾶һһ��Ӧ��R[i]��L[i] L[20]

	double RtjMin, RtjMax;//�Ƽ��뾶��Сֵ�����ֵ
	//BOOL isJYGuiFanCS;

	double ZXLmin1,ZXLmin2;//һ����С��ֱ�߳���������С��ֱ�߳�
	double Rmin1,Rmin2;//һ����С���߰뾶������
	CString TLDJ,ZXSM;//��·�ȼ�
	double XJJ;//��׼�߼��,0����,>0�������҂ȣ�<0���������
	double ZDMJG;//�ݶ�����

	


	//���ӷ���ʱ��
	CString BfLinkXLName, AftLinkXLName;//ǰ��������·��
	Int16 LinkXLBfJD, LinkXLAftJD; //������·��ǰ�ӵ�ǰһ�������±꣬��ӵĺ�һ�������±�
	
	/*---------------------�϶�����---------------------*/
	int MoveJDIndx;//�϶�������±꣨�϶������߽����Բ�����յ㣩
	bool IsMove;//�Ƿ�е��϶�
	bool IsMoveDone;//�Ƿ�е��϶�����


	/*---------------------��ͼ����---------------------*/

	
	BOOL m_IsNoteTZD;//�Ƿ��ע������
	BOOL m_IsNoteML;//�Ƿ��ע���
	BOOL m_isNoteNYDL;//�Ƿ��ע��ҵ����
	Int16 mode_display;//�Ƿ���ʾ��·��=0 ��ʾ

	//double LW;//�߿�
	AcDb::LineWeight  LineWeightValue ;//�߿� 
	double texth;
	Int16 draw_zybz;	//draw_zybz=-1 �����Ĵ�׮�����߲���������࣬=1Ϊ�����Ҳ�  =0Ϊ������ƫ���  818�Ƿ��б�Ҫ�ֱ����Ҳ�
	Int16 qxbbz;//qxbbz == 1, ��ע����Ҫ�أ�!= 1 ����ע

	double BZLC_dlc;//100���ױ꣬1000����꣬500��
	//int NGC/*,NLC*/;//NH--�̱߳���λ�� NLC--��̱���λ��

	Int16 Chinese_Engbz_type;//818 ���ã�
	Int16 NGC;//818

	//���µ�����
	vector<PDTAB> m_BPDArray;
	//CM����
	vector<double> m_CM;




public:

	RHA () ;
	virtual ~RHA () ;

	//----- AcDbObject protocols
	//- Dwg Filing protocol
	virtual Acad::ErrorStatus dwgOutFields (AcDbDwgFiler *pFiler) const ;
	virtual Acad::ErrorStatus dwgInFields (AcDbDwgFiler *pFiler) ;
	Acad::ErrorStatus subExplode(AcDbVoidPtrArray& entitySet) const;

	//----- AcDbEntity protocols
	//- Graphics protocol
protected:
	virtual Adesk::Boolean subWorldDraw (AcGiWorldDraw *mode) ;
	virtual Adesk::UInt32 subSetAttributes (AcGiDrawableTraits *traits) ;
	Acad::ErrorStatus RHA::subErase(Adesk::Boolean erasing);

	//- Osnap points protocol
public:
	/*virtual Acad::ErrorStatus subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	int gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const ;*/
	virtual Acad::ErrorStatus subGetOsnapPoints(
		AcDb::OsnapMode osnapMode,
		Adesk::GsMarker gsSelectionMark,	
		const AcGePoint3d &pickPoint,	
		const AcGePoint3d &lastPoint,	
		const AcGeMatrix3d &viewXform,
		AcGePoint3dArray &snapPoints,
		AcDbIntArray &geomIds,
		const AcGeMatrix3d& insertionMat) const; 



	//- Grip points protocol
	virtual Acad::ErrorStatus subGetGripPoints (AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds) const ;
	virtual Acad::ErrorStatus subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) ;

	//��ѯ������׼�о�n_o_dR�����Բ���߰뾶���仺��
	int RHA::SearchProximalRadius(double &n_o_fR);


	/*---------------------------------------------------*/


	//�ֳ����תͶӰ���
	bool TrsCkmltoProjml(const ACHAR ckml[], double& projml) const;

	//�ֳ����תͶӰ���
	bool TrsCkmltoProjml(const ACHAR gh[], double dml, double& projml) const;

	//�������ת�ֳ����  eg. "K159+900"
	bool TrsProjmltoCkml(double Projml, ACHAR Ckml[], int NLC) const;

	//�������ת�ںź���� eg. "K"  159900
	bool TrsProjmltoCkml(double projml, int NLC,  ACHAR GH[], double &XCLC) const;//818 ��TrsProjmltoCkmlͬ�������������Ĳ���˳�������

	//ͶӰ���ת����
	bool TrsProjmltoNE(double Projml,xlpoint& PZ) const;

	void RHA::TrsProjmltoNE(double Projml, double &N,  double &E) const;


	//�ֳ����ת����
	void TrsCkmltoNE(ACHAR Ckml[], double &N, double &E);
	void TrsCkmltoPZ(ACHAR Ckml[], xlpoint& PZ) const; 
	void TrsCkmltoPZ(ACHAR gh[], double dml, xlpoint& PZ) const;

	//ͳһ���ת����
	void TrsCmltoNE(double cml, double& N, double &E) const;
	void TrsCmltoPZ(double cml, xlpoint& PZ);

	//N��E����ת�������
	void TrsNEtoCml(double N, double E, double& cml);


	//������ж���Ŀ
	int GetRXDNum(vector<JDarrayStruct> &jdAry);
	int GetRXDNum() const;




	//��ͶӰ�����õ���ԭʼ�Ĳ����ߣ����ز�����ָ��
	RHA* GetBasLine(double projml2x, double &projml1x) const;


	//�õ�ָ�����жε����ս����±�
	bool RHA::GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx);

	bool GetRxdSEJD(int iRXD, int &SJDIx, int &EJDIx, vector<JDarrayStruct> &jdArray);

	//��ȡ�������ж����ս����
	bool GetAllRxdSEJD(vector<JDarrayStruct> &jdAry, vector<int>& SJDArray, vector<int>& EJDArray);

	//��λ������ֱ�߶��ϵİ��ױ�
    double LocatHmOnLine(double sProjml, double eProjml, bool directn, ACHAR CKml[]) const;

	//����ͳһ��̣��Ƿ���ǰѰ�Ҹ����������ױ꣬Ҫ����ֱ����
	double LocateHundredMeterOnLine(double tylc1x, bool IsForwardTo, ACHAR ckml[]) const;

	//��ĳͳһ��̴����ֳ����ȡ��������ȡ�����ֳ���̶�Ӧ��ͳһ���
	double RHA::RoundMileage(double cml, int intPrec, bool directn) const;
	
	//�����������ͳһ��̵ľ��루ȡ����ֵ��,dirctn=true,��ǰ�Ҷ�����=false�������
	double DistToNearDL(double projml,const vector<DLTab> &DLAry, bool IsForwardTo=true) const;

	//��������: Ѱ��ĳͳһ��̵������Ķ����ľ��루dirctn=true,��ǰ�Ҷ�����=false������ң�
	int RHA::FindNearDLbyCml(double cml, const vector<DLTab> &tmpDLB, bool dirctn) const;

	//��������:Ѱ��ĳһͶӰ��̸����Ķ����±�,dirctn=true,��ǰ�Ҷ�����=false�������
	int RHA::FindNearDL(double projml, const vector<DLTab> &DLAry, bool dirctn) const;

	//Ѱ��ĳ������̵�ǰһ��������
	int RHA::GetPreDL(double projml) const;

	//Ѱ�Ҿ���ĳ����������һ������(�����Ӳ������ϸ��ƹ����Ķ���)
	int RHA::GetPreRealDL(double projml) const;

	//������ֱ���ص��Ľ����±�
	bool SearchJZXCDJDIndex_OnJYJX(int iJD2,int isQorH,RHA* pJYX,int sJD_JY, int eJD_JY,int &iOverLapJD);

	//�㵽��·�ľ���
	double  DistFromPtToGTZX(double x, double y);

	//�������ж����Ϻ�ں�
	ACHAR* CreatRXD_SPtEGH(CString xlName, int LorR, ACHAR *GH1x);

	bool GenRXDL(int iRXD, int jdix, DLTab &dl, bool dirctn=true);

	bool GenBXDL(int jdix, DLTab &dl);

	//  ���ö�������
	void setDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx=-1, ACHAR *RXDName=NULL, ACHAR *BLName=NULL);
	//void AddDL(double BLC,double ELC, ACHAR *BGH,ACHAR *EGH,double TYLC, Int16 RXDIndx=-1, ACHAR *RXDName=NULL, ACHAR *BLName=NULL);

	Adesk::Boolean  BZLC(AcGiWorldDraw* pWd,double dLC,int zy);

	bool GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno, const vector<DLTab> &dlAry) const;

	bool RHA::GetiRXDDLinfo(int iRXD, int &sDLno, int &eDLno)const;  

	void MarkiRXDLC(AcGiWorldDraw* pWd,double dLC,int zy, int sdl, int edl);

	//��ע���ж���̺Ͷ���
	void MarkRXDLC(AcGiWorldDraw* pWd);

	//��ע���жε���̺Ͷ���
	void RHA::MarkRXDLC() const;

	//��ע��������
	void MarkSglDL(AcGiWorldDraw* pWd, int iDL, int zy);
	//(ը��)��ע��������
	void RHA::MarkSglDL(int iDL, int zy)  const;


	void GenDL();

	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor);

	//void ModifyJDXZ();
	//  ��ע���������
	Adesk::Boolean MarkTZD(AcGiWorldDraw* pWd, int zybz=1);  
	//  ��ע���������
	Adesk::Boolean MarkTZD() const;  //  ��ע���



	//��ֱ�ߣ������߿�
	Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd,AcGePoint3d& startPt,AcGePoint3d& endPt, int icolor,AcDb::LineWeight lw);


	void checkJDNum();//���ǰ�󽻵���Ƿ���ͬ����ͬ���������ߴ���


	//�����������
	void CalDL_NE(int sdl=0, int edl=-1);
	//�����޸ĺ�Ľ������ݸ��¶���
	void CmpNewDLwithOld();

	void SaveData();

	void UpdateAllData(/*int sJD=-1, int insrtBxJdNum=0*/);

    //�Ƿ�ȫ������
	bool IsAllLineRX() const;


	//������ɫ���߿��������ͼ����
	void setColorLineW();
	void SetAllColor(int ColorIndex);

	//���ƻ�������
	Adesk::Boolean G_drawspi(AcGiWorldDraw* pWd,double INDATA[5],double LO,struct xlpoint &P0,struct xlpoint &PZ, double ND);

	//������·������Ԫ
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd,double INDATA[5],struct xlpoint &P0,struct xlpoint &PZ,bool ISShow,double T0mini);
	//��ע����
	void MarkJD(AcGiWorldDraw* pWd,vector<JDarrayStruct> &arrayd,vector<QXBStruct>& qxb,ACHAR *JDchr);


	//����ʵ�����Բ������淶��������ͼ����
	void InitJBCS();

	//�жϲ�������Ϣ�Ƿ�ȫ�������ȫ����true
	bool RHA::IsJdBLInfEmpty(const JDarrayStruct &iJD) const;

	/*�жϲ�������Ϣ�Ƿ���ȷ�������ȷ����true*/
	bool RHA::IsJdBLInfRight(const JDarrayStruct &iJD, RHA* &pBLRHA, int &blJDNo) const;

	//�ж�ĳһ�����ǰһ���Ƿ���ƽ��Լ��������з���true
	bool RHA::IsBfSideRecordParallel(const vector<JDarrayStruct> &jdAry, int iJD) const;

	//�ɽ���ǰһ�߻��һ�ߵ��߼��,�����ڵ���ƽ�н���
	void RHA::ModifyJDXYbyOneParall(const vector<JDarrayStruct> &jdAry1x, int iJD1x
		, vector<JDarrayStruct> &jdAry2x, int iJD2x, bool isQorH);

	//�ɽ���ǰ��ߵ��߼�������߽�������
	void RHA::CalExJDByParallel(vector<JDarrayStruct> &jdAry, int jdix);

	//�ɽ���ǰ��ߵ��߼�������߽�������
	void RHA::CalExJDByParallel(const RHA *pBfBLRHA, const RHA *pCurBLRHA, int bfJDNo, int curJDNo, double bfXJJ, double curXJJ, JDarrayStruct *curJD);


	//�������߽������꣬���������п��ܲ����򲻹���ȷ�����Ʋ���ϵ���¼���
	void ModifyExJDXY();

	//�������ܣ���Ⲣ�жζ��߽���Ľ�������
	void RHA::CalEXJD(int jdn1,int njd2,double EXJD[2], const RHA *m_pm1);

	////����ĳ���н���
	void RHA::CalOneBXJDdata(int iJD,const RHA *m_pm1);



	//Դ��·pSrcRHA���߼��ƽ�Ƶõ�����·ʱ����������·�Ľ������꣬ע�⣺����������·ƽ��
	void RHA::GetJDXYwhenMoveXLbyXJJ(const RHA *pSrcRHA, double xjj);



 //   //������ֱ������
	//void CreatJZXArray();


	//������·�ļ�����λ��������·�Ƚϣ�ȷ�����չ�ϵ���γɷֶ�����
	//��isOnlyBaseOnSJJX = true��ֻ������ƻ��ߣ�����ƶ����ã���= false������������·
	void CalExReference(bool isOnlyBaseOnSJJX=false);

	//�Ƚϼ��Σ�ȷ�����չ�ϵ
	void CalExReference(CStringArray& XLNameAry, vector<JDarrayStruct>& RefJDarray);
	//�γɶ��߷ֶ�����GJDInfAry
	//bool CalExFDArray();

	//�ж�ͶӰ����Ƿ�λ�ڶ�������
	Int16 JudgePtIsOnDL(double Projml) const;

	Int16 RHA::JudgePtIsOnDL(ACHAR ckml[]) const;

	//NE����תͶӰ���
	void TrsNEtoProjml(double N, double E, double& Projml) const;

	//NE����ת�ֳ����
	void TrsNEtoCkml(double N, double E, ACHAR Ckml[]) const;


	//��ͶӰ������������ж���ţ�0��ʼ��
	Int16 GetiRxdByProjml(double projml) const;
	//�ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
	Int16 GetiRXDFromNE(double N, double E);
	//�ɾ��ࡢγ�࣬���������ж���ţ�0��ʼ��
	Int16 GetiRXDFromNE(double N, double E, double &projml);

	
	//��ע�߼��
	void MarkXJJ(AcGiWorldDraw* pWd);

	bool moveJZXAt(ads_point ptbase,ads_point ptout, int QQorHq);

	//��ȡǰ�������
	int GetPreJD(double N, double E) const;

	//���ĳ��(N, E)����·��ͶӰ���ǰ�󽻵�����,����ȡ���ڱ� plh07.17 
	int GetBian(double N, double E, double &N1, double &E1, double &N2, double &E2);

	//��ͳһ��̻�ȡ�������㣬IsForwardTo=true ��ǰ����(С�ڸý�������е�)
	int DistToNearJD(double projml, bool IsForwardTo) const;

	//�����������Ѱ�Ҹ����ǰһ�����(С�ڸý����ֱ����)
	int RHA::FindPreJD(double projml);


	//ƽ����ƺ��� 818 ��bas_fun??

	//ƽ�Ƽ�ֱ�ߺ󣬼���ǰ�����������꣨ͨ���㣩
	bool RHA::MovJZX(ads_point onPt, ads_point passPt);

	//ƽ�Ƽ�ֱ�ߺ󣬼���ǰ�����������꣨ƫ�ƾࣩ
	bool RHA::MovJZX(ads_point onPt, double offset);

	//��ת��ֱ��
	bool RotJZX(ads_point onPt, ads_point basPt, double angl);

	void  FormLCB(double sCml, double eCml, double step, CArray<LCB, LCB> &lcAry); 

	//��ע�߼�ࣺ1 ����һ�㣬2 ����һ�㣬��ע�߳���=20mm*SCALE����������
	void MarkXJJFlag(AcGiWorldDraw* pWd,AcGePoint3d Pt1,AcGePoint3d Pt2,double len);

	////��������Ҫ�أ���ע�ã�
	//void CalNoteQXYS();

	//������Ԫ
	Adesk::Boolean  getpxy_DATA(int moveJDix=-1);

	void DeleteiRXDDLB(int iRXD);//ɾ��ĳ�����жζ�����

	void DeleteOneRXD(int iRXD);

	void CalT1T2L();//818

	//ͶӰ���projmlתͳһ���cml
	double  CalnewcmlTocml(double newcml) const;

	//ͳһ���cmlתͶӰ���projml
	double  CalcmlTonewcml(double kmlTocml) const;

	//�ֳ����תͳһ���
	void TrsCkmltoCml(ACHAR ckml[], double &cml) const ;
	
	//�ֳ����תͳһ���
	void RHA::TrsCkmltoCml(const ACHAR gh[], const double xlc, double &cml) const ;

	//�潻�����ߺͽ���Բ
	void RHA::DrawJDLineAndCir(AcGiWorldDraw *pWd);

	//����λ
	void RHA::DrawXL(AcGiWorldDraw *pWd);
	

	Adesk::Boolean  MarkBiaoZhiLC(AcGiWorldDraw* pWd,double dlc,int zybz,int type);  

	//�ɶ����Զ����ɱ�־������� isCreatNew =true �������ɣ�=false ��������
	void GenBZLCbyDL(bool isCreatNew=true);

	//��־��̰�ͶӰ�������
	void SortBZLCArray();

	//�Ľ������鰴ͶӰ�������
	void SortGJDArray(vector<GJDInf>& gjdAry);

	//�����־�����Ϣ��������
	void CalBZLC();

	//���ӱ�־���
	void AddBZLC(ads_point PT ,ACHAR CurAddBZLC[50], CString iRxdXLName, bool isAuto=false);

	//ɾ��һ����־���
	void DeleteBZLC(ads_point PT, bool IsDelDL=false);

	// ��һ�����ж��ϣ����ݸ��µĶ������飨��·�Ķ���ȷ��Ҫ������ԭ���ı�־�������bzlcAry
	//dlAry �µ����жζ�������
	void CmpNewBZLCwithOld(vector<DLTab> &dlAry, vector<BZLCStructTable> &bzlcAry);

	//������ж���
	CString GetRxdName(int i);
	//�淶���
	void checkpm();

	bool  DelDLM(double tylc);
	bool  AddDLM(int iDL,double BLC,double ELC, ACHAR *BGH,ACHAR *EGH, double TYLC, Int16 iRxd);

	//����S���ת�ǡ�����������
	bool CalS(double R1,double L01,double R2,double L02,double xjj1,double xjj2
		,double JZXLen,double ZH_N,double ZH_E,double Fwj
		,double& afa,double &x1,double &y1,double &x2,double &y2);
	//����S���ת��
	bool Get_S_Afa(double R1,double L1,double R2,double L2
		,double DetaXJJ,double JZXLen,double& Alfa,double& T1,double& T2);

	//ѡ��S��
	bool SelSModel(double N, double E,int& SJD,int& S_JD1,int& S_JD2);

	//���ĳ���ж������û����õı�־���
	void GetiRxdBZLCArray(int iRXD ,  vector<BZLCStructTable> & bzlcAry );

	//ɾ�����жα�־���
	void DeleteiRXDBZLC(int iRXD, bool isDelSE=false);

	//��ö�Ӧ�����±�(JDi)�İ뾶��������N E����
	void getJD_RL(int JDi , double *Ri,double *L1i,double *L2i,double *NN,double *EE );

	bool AddRXD(AcGePoint3d PT1, AcGePoint3d PT2, AcGePoint3d OutPt
		,CArray<JDarrayStruct,JDarrayStruct>& jdAry, ACHAR ckml[], BOOL isPreMove=FALSE);

	//ȫ��������·�Զ�������š���������
	void GenJDNameJDXZ();

	//ָ��������ţ��޸������ꡢ�뾶������
	void MODIFYJD(int iJD,double N, double E, double l1, double R, double l2);

	//�����жα�Ż�ȡ���ж���·��
	CString GetRXDXLNameByRXDIndex(int RXDIndex);

	//�����ж���·����ȡ���жα��
	int GetRxdIndx(CString RXDXLName);

	//�ɡ����桿���ж���·����ȡ�����桿���жα��
	int RHA::GetZMRxdIndx(CString zmRxdName);

	//��������׮�ż�����̱�
	void RHA::AddTZDtoLCB(double Scml, double Ecml, CArray<LCB,LCB> &LCBArray);



	//������վ������̱� 
	void AddBriTunStatoLCB(int NBTS,double BTS[][2],ACHAR BorT, CArray<LCB, LCB> &pLCB);



public:
	RHA(const  vector<JDarrayStruct> &jd2, CString pGH, CString JDName
		, CString  xlName=L"",CString mdbname=L"", CString  xlShuXing=L"");

	RHA(CArray<JDarrayStruct,JDarrayStruct> &jd2, CString pGH, CString JDName
		, CString  xlName=L"",CString mdbname=L"", CString  xlShuXing=L"", BOOL isSetDrawParam=TRUE);

    RHA(int pNJD, JDarrayStruct jd2[], CString pGH, CString JDName
		, CString  xlName=L"", CString mdbname=L"", CString  xlShuXing=L"");

	RHA(CString xlName,  CString xLShuXing, const RHA *pSrcRHA, double xjj);

	void RHA::CreateGTJXGJDArray(/*double spt[2],double ept[2],*/vector<GJDInf>& AllGJDArray);

	//��������·��pGTZXJY�����öηֶ�
	void CreatLYDArrayOnJYX(RHA* pGTZXJY/*,double Spt[2], double Ept[2]*/, vector<GJDInf>& LYDInfArray);

	void RHA::CreateRXGJD(/*double Spt[2],double Ept[2],*/vector<GJDInf>& RXGJDArray);

	void RHA::CreatBXGJD(/*double GJD_Spt[2],double GJD_Ept[2],*/vector<GJDInf> &LYDRXDArray, vector<GJDInf>& OneGJDArray);

	void RHA::SetDefaultGjdName(CString blName, double N, double E, int GJDType, ACHAR GjdName[256]);

	void RHA::SortSections(vector<GJDInf> &LYDArray);

	//�ϲ��Ľ�������
	void MergeGTZX2GJDArray(vector<GJDInf>& AllGJDArray);
	//������ֱ������
	void CreatJZXArray();

	/////////����Ϊ��ƽ��ͼ����

	//���ɴ�ͳһ���lc1��ͳһ���lc2����·���ߣ�ȱʡFALSE,CONTINUOUS
	void DrawXLZXByXY(double Scml, double Ecml, const vector<vector<double>>  XYArray, int XYNum, double ZxWid, int linetype, ACHAR* LayerName, int LRMFlag)  const;

	//����DrawXLZXByXY�����Ƹ����Ľ���
	void MakeXLZX(double lc1,double lc2,int linetype,int LRMFlag,bool IsDrawRXGJ=true)  const;

	void RHA::BZJdLineAndJdCir() const;

	void RHA::BZJD(CString XLName,int LMRFlag) const;

	void RHA::MarkiRXDLC(double dLC,int zy, int sdl, int edl )  const;

	//���Ʒ�����һ�������̱�ע �򡰡���
	void RHA::DrawOneFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const;
	
	//����һ�η�����
	void RHA::DrawFQDBZ(double StartLC, double EndLC, double JianJu, int LMRFlag) const;

	//��ȡ�Ľ��λ��ܱ��γɼ��л��ߡ����ж��ߵ����ö����� GJDINFARRAY
	void RHA::CreatLYDArrayOfJYX();

	//�������η�����
	void RHA::BZJYXFQD() const;
	
	//�ж�һ���Ƿ������ö��ϣ������ö��Ϸ���true
	bool RHA::IsPtOnLYD(double projml) const;
	
	//������ߵ����ߵ����������ҵ����
	double  RHA::CalCurvNYDL(int iJD) const;

	/////////�ݶ���
	//������������ڲ�������߸̣߳����ص���������
	void RHA::PM_DMX(const CArray<LCB, LCB> &lcAry, double offst, vector<GMXTAB> &dmxAry);

	//��ȡ�������ֳ���̷�Χ�ڵĶ�������
	void RHA::GetBasLineDLAry(CString xlName, ACHAR sCkml[MaxCkmlStrNum], ACHAR eCkml[MaxCkmlStrNum], vector<DLTab> &dlAry);

	//���ɶ��߹�ͨ������
	void RHA::Create2XGTDLB();
	//������ж��ϴӲ������Ͽ��������Ķ������������Ϊ-3��
	void RHA::EarseDLofBLonBXD();

	//ͳһ���ת�ֳ����
	void  RHA::TrsCmltoCkml(double cml, ACHAR ckml[], int nLC=3) const ;

	//ͳһ���ת�ֳ����
	void  RHA::TrsCmltoCkml(double cml, ACHAR gh[], double &xlc) const ;

	//ͳһ���תͶӰ���
	double  RHA::TrsCmltoProjml(double cml) const ;

	//ͶӰ���תͳһ���
	double  RHA::TrsProjmltoCml(double projml) const ;

	//��������̻�ȡ��������ڵĸĽ�������
	int RHA::GetGJDTypeByCml(double Cml);
	int RHA::GetGJDTypeByProjml(double Projml);

	//��������̻�ȡ�Ľ�����Ϣ���Ľ�������GJDType�����ղ�����·��BXLName��EXLName
	void GetPtGJDInfByCml(double Cml, int& GJDType, CString& BXLName, CString& EXLName);

	//��������̻�ȡ�Ľ�����Ϣ���Ľ�������GJDType�����ղ�����·��BXLName��EXLName
	void RHA::GetPtGJDInfByCml(double N, double E, int& GJDType, CString& BXLName, CString& EXLName);


	//�ж�ĳ�ֳ���������жλ��ǲ��ж��ϣ�����Ĳ����У�
	bool RHA::JudgeMLonBXDorRXD(ACHAR ckml[], CString &blName, CString &rxdName);

	//�ɻ�����������ж��Ƿ������ж��ϣ��������жα�ţ��ڲ��жη���-1
	int RHA::GetiRxdNoFromCml_1X(double Cml_1X);

	//���Ƶ���������
	// �����ֳ����  ������  �յ����  �ų�  ������ʽ	����  ���жα�ţ���1����0��ʾΪ���л��ߣ�����(1-���ߣ�2-˫��)

	void RHA::INSERT_BIG_MID_BRIDGE(CString xclc0,  CString xclc0_start
		,  CString xclc0_end,  double lenBri,  double HBri,  CString StyBri
		,  CString NamBri	 ,  int LineNum) const;

	//��������ͶӰ������ɾֲ���������
	void RHA::CreateLocalDLarray(double sProjml, double eProjml, vector<DLTab> &dlAry) const;

	void RHA::MarkBridge() const;

	void RHA::INSERT_TUNNEL(CString qdxclc,CString zdxclc,CString NamTun,int LineNum,int Style) const;


	void RHA::MarkTunnel() const;

	void RHA::INSERT_STATION(CString xclc0,CString NamSta,CString TKSta,bool GJXZ) const;

	void RHA::DrawStaFQDBZ(double NCross, double ECross, double Fwj, double Size, int LMRFlag) const;//���Ʒ����α�ע������

	void RHA::MarkStation() const;

	void RHA::MarkBM() const;
	//����ˮ׼��  
	void RHA::INSERT_BM(double X, double Y, CString GH, double XLC, double DD, CString BianHao, double LEV) const;

	//���Ƶ���
	void RHA::MarkDX() const;

	//���Ƶ���
	void RHA::INSERT_DX(int NUM, double *NX, double *EY, CStringArray *BianHao, double *GaoCheng, double *FWJ) const;

	//  ��ע�߼��
	void RHA::BZXJJ() const ;

	//���߼��  
	void makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType,double jiao=-100) const;

	//���������С�ź��Ľӳ�
	void RHA::CalSmallBriLen(vector<SMALLBRIDGETAB> &smallBriAry) const;

	//����С�ź� IsDrawAll-�Ƿ�ȫ�����ƻ���ֻ��������
	void RHA::plhINSBR(bool IsDrawAll) const;

	void RHA::INSERT_SMALL_BRIDGE(ACHAR gh[], double xclc0, CString NameCross, int LineNum, bool GJXZ
		, int LMRFlag, int iRXD, double LTW, double RTW, double LLevel
		, double RLevel, double jiao, int InsertStyle, double dCulvertUse) const;

		//�溭��:�������꣬����ҿ���ߡ��Ҹߣ�����·ǰ��������ת��λ�ǣ�����
	void RHA::DrawCulvert(AcGePoint3d Pt0,double LTW,double RTW,double FWJ,ACHAR layname[80],bool IsAddWid) const;

	//��С�ź�
	void RHA::DrawSmallBridge(AcGePoint3d sPt, AcGePoint3d ePt, ACHAR layname[100], int LineNum, int mode=0, double LWid=3.0, double RWid=3.0, int LMRFlag=MAINLINE, double lc0=0.0, double Jiao=0.0) const;
	
	//����λ�䶯�󣬼��ĳ���ж����յ�����Ƿ���Ȼ�ʺϣ�����������¼���ö����ļ������
	bool RHA::CheckRxdDL(int iRXD, bool isSDLorEDL);

	//������λ�ı仯���¼������жζ����������ܱ���ԭ�ж������ֳ���̣�
	void RHA::UpdateRxdDL(int iRXD);

	//�������жα�־���
	void RHA::UpdateRxdBZLC(int iRXD, 	vector<BZLCStructTable> &bzlcArray);

	//��ĳ������ʼ������֮������ж�����ͶӰ���
	void RHA::CalDLprojml(int iStart);

	// ����ĳ���жεĶ����ͱ�־���
	void RHA::UpdateSglRxdLC(int iRXD, bool isCalXYarray=true);

	// �޸����ж�ʱ��������̣���������־��̣�
	void RHA::UpdateLC(int iRXD);


	//Ѱ��ĳͶӰ��̸����ı�־����±꣬���û�ҵ�����-100 
	int  RHA::FindNearBZLC(double projml, bool dirctn);

	//�����û����õĽ����϶���ʽ�͸ý�������ԣ��õ��ý����ʵ���϶���ʽ 
	int  RHA::GetJDMoveType(int jdMoveType, int iJD) const;

	//�����ӽ���ʱ���ж�ʰȡ�����ڷ�Χ�Ƿ��������ӽ���
	bool RHA::JudgeIfCanAddJD(double N, double E, int &iRxd);

	//��ɾ������ʱ���ж�ʰȡ�����ڷ�Χ�Ƿ�����ɾ������
	bool RHA::JudgeIfCanDelJD(double N, double E, int &iJD, int &iRxd);

	//��������Ҫ��
	void RHA::setJD_RL(int JDi ,double Ri,double L1i,double L2i );

	//����ͬ��Բ�뾶
	bool RHA::CalTXY(double XianJianJu,double JiaKuan,double BanJing1,double HuanChang1,
		double& BanJing2,double& HuanChang2);

	//����ĳ���н���
	//void RHA::CalOneBXJDdata(int iJD)


	//�Զ�����ĳ�����жε����ն���
	bool RHA::GeniRxdDL(int indx, DLTab rxdDL[2]);

	//�ж���ʰȡ������·��ʼ�߻�����ֹ��
	bool RHA::IsPtonSLorEL(AcGePoint3d pt, bool &isSLorEL) const;

	//���ܣ�������·ʱ������·��Ŀ����·�ϵ����յ�����
	bool RHA::GetSptEptWhenInXL(vector<JDarrayStruct> &tmpJDAry, const vector<QXBStruct> &tmpQXBAry);

	bool RHA::CombineJD(vector<JDarrayStruct> &jdAry);

	//����ָ���������������ж�
	int RHA::FindiRXD(int iJD);

	//��ע������
	void RHA::BZSHUQX(AcGiWorldDraw* pWd,int num,int LorR);

	bool RHA::BZCM(AcGiWorldDraw* pWd);

	//�γ�CM����
	void RHA::TrsBPDArraytoCM();

	//���ɸĽ�������
	void CalGJDArray();

	/*---------------------------------------------------*/
	//ȫ�����귨	���Ʋ�����ر���������	ljx	17.10.18
	//��Ա����
	vector<SSurPt> m_MeasurePoints;		//���ṹ��vector���洢�����еĲ��
	bool m_IsDrawRectification;			//�Ƿ���Ʋ���

	//��Ա����
	void DrawCircle(AcGiWorldDraw *mode);														//��СԲ����
	void CalRectification(vector<double>& Rectification);										//���㲦��
	void DrawRectificationText(vector<double>& Rectification, AcGiWorldDraw* mode);				//����������
} ;

#ifdef PINGMIAN_MODULE
ACDB_REGISTER_OBJECT_ENTRY_AUTO(RHA)
#endif

