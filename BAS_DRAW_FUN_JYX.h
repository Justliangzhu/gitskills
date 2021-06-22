// BAS_DRAW_FUN_JYX.h: interface for the BAS_DRAW_FUN_JYX class.
//
//////////////////////////////////////////////////////////////////////
// BAS_DRAW_FUN_JYX.h: interface for the BAS_DRAW_FUN_JYX class.
//
//////////////////////////////////////////////////////////////////////
//#include "RAILDESIGN.H" 
#include <stdlib.h>
#include <rxobject.h>
#include <rxregsvc.h>
#include <dbents.h>
#include <aced.h>
#include <dbsymtb.h>
#include <adslib.h>
#include <dbxrecrd.h>

#include <string.h>

#include <geassign.h>
#include <dbgroup.h>
#include <gearc2d.h >
#include <dbpl.h> 
#include <acutads.h> 
#include <dbapserv.h> 

#include <dbjig.h>
#include <acgi.h>
#include <math.h>
#include <assert.h>
//#include <iostream.h>
#include <iostream>


#include "rxdefs.h"
#include "UltraCArray.h"
#include "StdAfx.h"
//#include "EXCEL9.h"
//#include "GTZX_JYX.h"
// #include "BAS_DRAW_FUN_JYX.h"

//#include "ol_errno.h"
//#include "STDIO.h"
//#include "malloc.h"

#include <dbregion.h>

#if !defined(AFX_BAS_DRAW_FUN_JYX_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
#define AFX_BAS_DRAW_FUN_JYX_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_
#define pi 3.14159265358979323846
#define PI 3.14159265358979323846
#define Pi 3.1415926535897932384626433832795
#define TRIMVERSION 20040220
#define MAXJDNUM 200
#define MAXBPDNUM 300
#define MAXDLNUM 200
#define PRECISION 1.0e-6
#define MAXGJD 200
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define Spoint(pt, x, y, z)  pt[X] = (x);  pt[Y] = (y);  pt[Z] = (z)
#define Cpoint(d, s)   d[X] = s[X];  d[Y] = s[Y];  d[Z] = s[Z]


//class GTZX;
struct ptt_node_JYX{
	ads_point pt0;
	ads_point pt;
	ads_point pt1;
	struct ptt_node_JYX *next;
};
struct arc_lst_JYX{
	ads_real R;
	ads_real xo;
	ads_real yo;
};
struct point_lk_JYX{
	ads_point pt;
	struct point_lk_JYX *next;
};

class BAS_DRAW_FUN_JYX  
{
public:
	BAS_DRAW_FUN_JYX();
	virtual ~BAS_DRAW_FUN_JYX();

	double m_dHalfPI;

	void InitNameOfWay(CComboBox *m_Combo_WayName,int JYorGJ,CString m_Edit_FName);//��ʼ����·��

	int DrawZorY; //1 �������һ� -1 ��������
	struct NoteStr
	{
		int rownum,colnum,ntext;
		int fitinf;//=0 not fit mode ; =1 fit mode;
		double texth,dv;//dv fitʱ ��λ��ʱ�ĳ���
		CString text;//�����ֻ��һ���ı� ntext=1
		CString text1;//���������ı� ntext=2
		CString text2;//�����ı� ntext=3
	};

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
    GuiFanCanShu mGJGuiFanCS;
	struct ZDMError
	{
		int m_iBPDIndex;

		//��������:0-�¶ȴ������ 1-�����ص� 2-�¶ȳ��� 3-�̳߳����߳̿��Ƶ� 4-�³�С����С�³�
		int m_iErrorType;

		ACHAR m_ckml[80];
		double m_dTYLC;
	}; 


	void createBlocks();
	AcDbObjectId maketext1(AcGePoint3d lpos, AcGePoint3d rpos,ACHAR* ctext,double ang,double texth ,int icolor=0,ACHAR* textStyle=L"����",ACHAR *font=L"simplex.shx",ACHAR *bigfont=L"hzdx.shx",ACHAR *LayerName=NULL,int i=0);
	AcDbObjectId   makeline1(AcGePoint3d startPt,AcGePoint3d endPt);
	AcDbObjectId   makeline1(AcGePoint3d startPt,AcGePoint3d endPt,int icolor,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	AcDbObjectId make2dline1(AcGePoint3dArray ptarray,int icolor=0,double startWid=0,double EndWid=0,ACHAR *LayerName=NULL);
	AcDbObjectId maketext1(AcGePoint3d pos, ACHAR* ctext,double ang,double texth ,int icolor=0,int mode=4,ACHAR* textStyle=L"����",ACHAR *LayerName=NULL);

	struct Table //���ṹ
	{
		int nrow,ncol;
		double RowH[30],ColWid[8];
		AcGePoint2d LDpt; 
		int Nstr;
		NoteStr Str[30];
	};
	struct LTJCflag//���彻��ͼ��
	{
		AcDbObjectId blockId;
		CString blockname;
	} Ltjc[2];

	struct STATIONflag//��վͼ��
	{
		AcDbObjectId blockId;
		CString blockname;
	} Staflag[140];
	int SflgN;

	struct PJDflag//ƽ����ͼ��
	{
		AcDbObjectId blockId;
		CString blockname;
	} Pjdflag[8];
	int PjdN;

	void maketable(struct Table &tab,int icolor=0,double startWid=0,double EndWid=0);


	////��������귨���㲦�����ú���
	struct  CXZYDFD{
		CString GH;
		double dml;//������ͳ�Ʒֶ��յ����
		CString BZText;//��ע
	};

	struct  CYDLXFD{
		CString GH;
		double dml;//�õ����ͷֶ��յ����
		CString YDType;//�õ�����
		double cml;//��Ӧ�������
		int xh;//��Ӧ�õ��������
	};

	struct BCTAB  //���
	{
		CString NameOfWay;
		ACHAR ckml[80];
		double Level;
	}; 

	struct RAILSTU //����ṹ��
	{
		CString XLName;
		CString	FdKml;//�ֶ������
		CString	StruType;
		double cml;
		double StruHeight;
	};

	struct EXDLB
	{//���߶�����
		ACHAR XLName[50]; //��·��
		int no;//��ʾ�ڼ������ж�
		CString RXDName;//�Ծ����ж�����
		double BLC,ELC,TYLC;  //  �����Ķ�ǰ��̡��Ϻ���̡�������ͳһ���
		CString   BGH,EGH; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�	

		double x;
		double y;
	};








	struct SetLineAttriStruct
	{//��·��������
		CString XLName;  //��·��
		int ColorIndex; //��ɫ
		int LineW;  //�߿�
	};

	struct DoubleLineData//˫��ƽ������
	{
		Adesk::Int16 JDXZ;//�������� 0-���жν���,1-���жν���
		Adesk::Int16 jdno;//���߽����(�±�)
		CString BaseLineJDName;	//���߽�����	20191114����

		double x,y,Lo1,R,Lo2;
		Adesk::Int16 jdno1;//��Ӧһ�߽������
		double xjj;//�߼��//��������Ϊ������Ϊ��
		double qxjk;//���߼ӿ�
		double NYDL;//������ҵ����
		double ZXNYDL;//ֱ����ҵ����
		ACHAR ZHLC[80];//ֱ�����
		ACHAR JDLC[80];//����ͶӰ��һ���ϵ����//plh05.08
		bool ISXJJFirst;//�Ƿ�����߼�����
		bool ISStartEnd;//�Ƿ�Ϊ���ߵ����յ�
		bool ISShow;//�Ƿ���ʾ���û�

		ACHAR XLName[50]; //��·��

		ACHAR JDNum[40];
	};
	////////////////////////////////////////����plh

	typedef UltraCArray_T<DoubleLineData,DoubleLineData> RXDInfoArray;
	typedef UltraCArray_T<EXDLB,EXDLB> EXDLBArray;
	struct RXDInfo//���ж���Ϣ
	{
		RXDInfoArray m_arrJD;
		EXDLBArray m_arrDLB;
	};
	//////////////////////////////////////////////////////////////////////////



	struct xlpoint { double x,y,a,r,lc; };


	////���߽ṹ
	//struct  xlpointRorSSide
	//{
	//	BOOL IsSpanSide ;
	//	xlpoint JieRuPZ; //�����
	//};

	 struct DLBZ  //�ںŸı�㡢�����㡢���յ�Ĺںš���̱�
	{
		//ע�⣺���߶������еĶ�����ELC��һ����ֵ,��ʱҪͨ��BLC + DL ��� ELC
		double BLC,ELC,DL;  //  �����Ķ�ǰ��̺Ͷ���ֵ(+ֵΪ����;-Ϊ����)

		ACHAR   BGH[8],EGH[8]; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�
		ACHAR   GJDNum[255];   //�Ľ��ζ�����ֻ��ԸĽ��Σ�

		double N,E; //�������
	};

	struct QXTAB//���߱�
	{
		int jdno;//�����
		double x,y,zj,R,l1,l2,T1,T2,L,jzxl;
		CString BGH,EGH;//ZH,HZ׮�ں�
		double Bml,Eml;//�������
		double InDL;//		��ҵ����
	};


	//struct TWTAB//���ڱ�
	//{
	//	CString GH;//��׮�ں�
	//	double  ml;//��׮���
	//	double  DMGC,SJGC;//����߳�,��Ƹ߳�
	//	double SJPD;//����¶�
	//	double SQXZJ;//�������ݾ�
	//	double ZXTJ;//����̧��ֵ
	//	double ZXTWG;//�������ڸ�
	//	double LJSGG;//·��ʩ����
	//	double QXJK;//���߼ӿ�
	//	double LJSGK;//·��ʩ�����
	//	ACHAR CKml[80];
	//};


	struct TWTAB//���ڱ�
	{
		CString NameOfWay;//��·��
		CString GH;//��׮�ں�
		double  ml;//��׮���
		double  DMGC,SJGC;//����߳�,��Ƹ߳�
		double JYGMGC,SJGMGC;//����߳�
		double SJPD;//����¶�
		double SQXZJ;//�������ݾ�
		double ZXTJ;//����̧��ֵ
		double ZXTWG;//�������ڸ�
		double LJSGG;//·��ʩ����
		double ZQXJK,YQXJK;//���߼ӿ�
		double LJSGK;//·��ʩ�����
		double ZXJJ,YXJJ;//�����߼��
		double GTJXXJJ,GTEXXJJ;//��ƻ��ߣ���ƶ�������ڼ��л��ߵ��߼��
		double LJFLK;//·������
		CString FZ;//��ע
		int DMinf;//������Ϣ ��CalJK����
		/*
		DMinf = 0 //����·��
		1//���ߣ�II�ߣ�����
		2//���ߣ�II�ߣ�����
		3//���߲���
		4//���ߣ�II�ߣ�����,·��δ���루��һ�߼�������ʽ·����
		5//���ߣ�II�ߣ�����,·����루��һ�ߺ��������Ϸֱ����·����
		6//���ߣ�II�ߣ�����,·��δ���루��һ�߼�������ʽ·����
		7//���ߣ�II�ߣ�����,·����루��һ�ߺ��������Ϸֱ����·����
		*/

		ACHAR CKml[80];
	};

	struct HdmAreaData//�����ɹ�������ݽṹ
	{
		CString NameOfWay;//��·��
		CString GH;
		double dml;
		double BTinf;//��������
		double at;//�����
		double aglt;//���������
		double asst;//��ˮ�����
		double aqbt;// ��������
		double acut;//�ڷ����
		double aw[6];//���������
		double LZDK;//��ռ�ؿ�
		double RZDK;//��ռ�ؿ�
		double ahj;// ������� �ӹ̷����۹�
		double asgt;//ˮ�������
		double asgw;//ˮ�������
		double ajq;//�������     �����۹�
		double cpbpxc;//��Ƥ����б��
		double dqbpxc;//��ǽ����б��

	};

	struct ZYJSBZTAB//��Ҫ������׼��
	{
		CString TLDJ;//��·�ȼ�
		CString ZXSM;//������Ŀ
		CString GDLX;//�������
		CString QYZL;//ǣ������
		double SDMB;//�ٶ�Ŀ��
		double XJJ;//��׼�߼��
		double RtjMin,RtjMax;//�Ƽ��뾶С����
		double XZPD,XZPD1;//�����¶�,���������¶�
		double DFXYXC;//��������Ч��
		double Rmin1,Rmin2;//��С���߰뾶
	};
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////
	//���иĽ����ݽṹ
	struct BoLiangTable//�������߲�����������
	{
		ACHAR JDNum[20];//�����
		double ZHCml,HYCml,YHCml,HZCml;
		double alfa,R,l1,l2,KZH,KHY,KYH,KHZ;
		ACHAR GH[20];
		bool IsMeasure;//�Ƿ���ҵʵ��

		ACHAR XLName[36];
		// 		CString BGH,EGH;//ZH,HZ׮�ں�
		// 		double Bml,Eml;//�������
		// 		double InDL;//		��ҵ����
	};

	//����׮���ߵ����ݽṹ
	struct WaiYiZhuang 
	{
		ACHAR m_cGH[16];
		double m_dLC;

		ACHAR m_cCKml[16];
		double m_dCml;


		double m_dN;
		double m_dE;
		double m_dOffset;//���ƾ� ������
		int m_iZXOrQX;//-1-ֱ�߶���㣬1-ֱ�߶��յ㣬0-ֱ���� 3-ֱ�ߵ� 2-����
		ACHAR XLName[40];//��·��
	};

	struct ZMTaiZhang
	{
		ACHAR m_cGH[16];
		double m_dLC;
		double m_dCml;

		ACHAR XLName[40];//��·��

		ACHAR m_cCKml[16];

	};

	struct XingZhengQuHua
	{
		CString m_sSml;
		CString m_sEml;

		CString m_sName;

		double m_dSmlTYLC;
		double m_dEmlTYLC;
	};



	struct XLDStruct// ��·�ֶ���Ϣ
	{
		double Bcml,Ecml;
		double BpointToN,BpointToE,EpointToN,EpointToE;
		CString XLName;
	};

	//struct JDDataTable//�������߲�����������
	//{

	//	ACHAR jdno[20];//�����
	//	double x,y,R,l1,l2;

	//};

	struct OriginalMeasureTable//��������ԭʼ��������
	{
		ACHAR jdno[20];//�����
		int ZJPoint;
		//	ACHAR GH[20];
		ACHAR Landmark[255];
		double fj;

		double ZhiCeJv;
		double Youjiao;

		double Ej;
		double Es;
		double deE;
		double BLTerritoryPlus;
		double BLTerritoryMinus;
		ACHAR ShuXing[15];

		double LCcha;

		int Fuzhu;

	};

	typedef struct MulFDData  //�����߷ֶ���Ϣ��
	{
		ACHAR jdno[20];//�����
		double KZH ;
		double KHY ;  //HY���
		double KYH ;  //YH���
		double KHZ ;
	};

	typedef struct QLTKStruct // ����ͼ
	{
		AcDbObjectId eId ;
		AcGePoint3d ZXPt , YSPt ;
	};


	//struct MulCurveFDTable   //�����߷ֶ���Ϣ��
	//{
	//	ACHAR jdno[20];//�����
	//	double KHY;  //HY���
	//	double KYH;  //YH���
	//};

	//ֻ�Զ�д,ʵ�����������ṹ
	typedef struct BZLCStructTable  //��־��̱�
	{
		//	int Num;
		//ACHAR GH[8];
		ACHAR kml[30];
		double X;
		double Y;
		double K;
		double xyTocml;
		double kmlTocml;
		ACHAR   GJDNum[255];   //�Ľ��ζ�����ֻ��ԸĽ��Σ�

	};


	typedef	struct JDarrayStruct
	{ //����ṹ����

		ACHAR GJDNum[255];
		ACHAR JDNum[40];
		double N;
		double E;
		double l1;
		double R;
		double l2;
		BOOL isGJ;//True--�Ľ�//False--�ع�
	};


	typedef	struct QXBStruct
	{ //
		double KZH;
		double KHY;
		double KYH;
		double KHZ;
		double a;
		double l1;
		double R;
		double l2;

		double T1;
		double T2;
		double L;
		int LorR;
		ACHAR JDNum[20];
		BOOL isGJ;//True--�Ľ�//False--�ع�

		ACHAR GH[20]; // ��̹ں�

		//��ΰ2007-08-07���߱��е�CKZH���ַ���ԭ������Ϊ20��̫С���Ӵ�Ϊ40
		ACHAR XLName[255];
		ACHAR CKZH[40];
		ACHAR CKHY[40];
		ACHAR CKYH[40];
		ACHAR CKHZ[40];
	};

	// typedef UltraCArray_T<JDarrayStruct,JDarrayStruct>SGJJDarray2;

	struct GJDInf{
		ACHAR XLName[256];
		double spt[2];//�������
		double ept[2];//�յ�����
		int iSJD1,iEJD1;//�Ľ������յ���һ���ϵĽ����
		int iSJD2,iEJD2;//�Ľ������յ��ڶ����ϵĽ����
		int GJDType;//�Ľ������ͣ�0-���� 1-S�� 2-���л��߸Ľ� -1�����ö�

		int ID;//�ڲ���ʶ
		ACHAR gjdname[256];//�ⲿ�Ľ�����

		ACHAR BXLName[256];//��ʼ��·��
		ACHAR EXLName[256];//��ֹ��·��

		ACHAR GJDBLC[50];//������
		ACHAR GJDELC[50];//�յ����
	};//�Ľ�����Ϣ��
	//ʵ��


	struct SearchRange
	{
		ACHAR StartXLName[256];
		ACHAR EndXLName[256];
		double spt[2];//�������
		double ept[2];//�յ�����
	};

	typedef	struct ConnectLineTableStruct
	{ 
		ACHAR ToOutSGJJDNum[50];  //���� �Ľ��α��

		ACHAR ToInSGJJDNum[50];  //���� �Ľ��α��

		ACHAR BXLName[256];   
		ACHAR EXLName[256];
	};


	typedef struct Gtzx2BZLCStruct
	{
		int i ;
		CArray<BZLCStructTable,BZLCStructTable>BZLCarray;

	};

	typedef	struct SGBXTableStruct
	{ 
		ACHAR SGBXName[50];

		ACHAR ToInSGBXName[50];  //����
		ACHAR BXLName[256];   
		ACHAR EXLName[256];
	};

	typedef	struct JBJXTableStruct
	{ 
		ACHAR JBJXName[50];
	};



	//�Ľ��λ��ܱ�
	typedef	struct GJDAllInforStruct
	{ 
		ACHAR ToOutSGJJDNum[50];//���� ��·��

		ACHAR ToInSGJJDNum[50];//���� ��·��

		double BpointToN;
		double BpointToE;

		double EpointToN;
		double EpointToE;

		ACHAR BXLName[256];//�����·��
		ACHAR EXLName[256];//�յ���·��

		int GJDType; //�Ľ�������

		ACHAR GJDBLC[50];//����ֳ����
		ACHAR GJDELC[50];//�յ��ֳ����
		ACHAR GJXZ[50];

		double GJDSml;//���ͳһ���
		double GJDEml;//�յ�ͳһ���

	};





	//��ΰ��
	struct EXGJDFDInf{
		ACHAR XLName[256]; //��������·��
		double spt[2];//�������
		double ept[2];//�յ�����

	};//�Ľ��ηֶ���Ϣ��


	//ĳһ�Ľ��Ρ����β�����
	typedef	struct  OneGJDJDStruct  //  GJDToOneInforStruct
	{ //
		ACHAR SGJDJDNum[50];
		UltraCArray_T<JDarrayStruct,JDarrayStruct&>SGJJDarray;
	};




	struct ListZhFa{//������Ϸ�������
		int pro;     //����pro��ѡ��̷�Χ
		ACHAR SGH[10];//��ں�
		double Sdml;//���ֳ����
		ACHAR EGH[10];//�չں�
		double Edml;//���ֳ����
		ListZhFa *prior;
		ListZhFa *next;
	};


	struct EXDuanLuo
	{
		int no;//������
		double sml;//����������ͳһ���
		double eml;//�����յ����ͳһ���
	};


	struct STATIONSTRUCT
	{
		int iRXD;//���жα��
		//վ��������̹ںţ����������վ���ƣ�����ͼ����
		CString GH,NamSta,TKSta;
		ACHAR Kml[80];
		double XLC;
	};

	struct BMSTRUCT
	{
		int iRXD;
		//ˮ׼���š��̡߳���̹ںţ������������·���߾��룬����
		CString BianHao,GH;
		double GaoCheng,XLC,DistZX,N,E;

		CString CLMS , FZ ; //��������,��ע	

		double dTYLC;

		ACHAR cGTJXLC[20];//��ͨ��ƻ����ϵ��ֳ����

		bool bBXLYOrRX;//���С����ö�(true)�������иĽ���(false)


	};

	struct DXSTRUCT
	{
		int iRXD;
		//׮�ţ��̣߳����꣬��λ��
		CString ZhuangHao;
		double GaoCheng,N,E,FWJ;
		double RightAng,Dist,cml;//�ҽǣ����룬���
	};

	////��������귨���㲦�����ú���

	struct SZJDOrCD //�����е㣨�����þ��㣩�Ľṹ
	{
		ACHAR JDNum[20];
		int iZJDOrCD;//�þ��㻹�ǲ�㣬0��ʾ��㣬��0ʱΪ�þ�����
		CString csLCBZ;//�������
		double dN;//�����þ���N����
		double dE;//�����þ���E����
		double dNXL;//��������·�ϵ�N����
		double dEXL;//��������·�ϵ�E����
		double dLZJDtoCD;//�߳�
		double dYoujiao;//�ҽ�
		double dFwjHoushi;//���ӷ�λ��
		double dPianjiao;//ƫ��
		double dLC;//�����̣��þ���ʱΪ0.0;
		double dBoliang;//����
		CString csCDAttribute;//�������
		double dLCCha;//��̲�
	};


	//һ��
	typedef UltraCArray_T<SZJDOrCD,SZJDOrCD&>OneSZJDOrCD;


	//----------------------�ݶ���-------------------------------
	struct GMXTAB//�����߱�
	{
		CString NameOfWay;//��·��
		CString CAP;//��׮�ں�
		double  ML;//���
		double  H;//�߳�
		ACHAR CKml[80];
		double x;//��ͼ����
		double y;
	};

	//�����
	struct BranchTrack
	{
		CString StCap;//���ں� 
		CString EdCap;//�յ�ں� 
		double StDml;//������
		double EdDml;//�յ����
	};

	struct RoadBedTAB//������ȱ�
	{
		CString NameOfWay;//��·��
		CString GH;//��׮�ں�
		double  ml;//���
		double  H;//�������
		double  H1;//����ṹ�߶�
		double dml;
		ACHAR CKml[80];

		CString sRemark;//������Ƶ��ע
	};

	struct PDTAB//���µ��
	{
		CString NameOfWay;
		CString GH;//���µ�ں�
		double  ml;//���µ����
		double  Level;//��Ƹ߳�
		double Rshu,RLen;//�����߰뾶,����
		double degree,length;//�¶ȣ��³�
		CString Notes;//��ע
		ACHAR CKml[80];
		double cml;//������
	};

	typedef	CArray<PDTAB,PDTAB> BPDArray;

	typedef UltraCArray_T<PDTAB,PDTAB> APDTAB;
	typedef UltraCArray_T<APDTAB,APDTAB> MPDTAB;


	struct TUNTAB//�����
	{
		CString NameOfWay;//��·��
		int iRXD;//���жα�š���0��ʾ���л���
		CString TUNName;//����� 
		CString BGH;//�ں�
		CString EGH;//�ں�
		CString flag;//���췽ʽ
		double StDml;//������
		double StH;
		double EdDml;//�յ����
		double EdH;
		double CenDml;
		double CenH;
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJZT;
		long NumOfLine; //����
	};


	struct SMALLBRIDGETAB//С�ź���
	{
		CString NameOfWay;//��·��
		int iRXD;//���жα�š���0��ʾ���л���
		CString GH;//�ں�
		double ml;//�������
		ACHAR CKml[80];
		CString KKStyle;//�׿�ʽ��
		double dwL,dwR;//������
		double JJiao;//����
		double ToUse;//��;
		double InsertStyle;//���췽ʽ
		CString GJXZ;//�Ľ�����
		CString Notes;//��ע
		double LLevel,RLevel;//���Һ��ڱ��
		int LineNum;//������1-���ߣ�2-˫�ߣ�
	};

	struct BasicDataOfZDM//����������
	{
		CString Fname;
		CString WayName;
		double XScale;
		double YScale;
		CString MLDecimal;
		CString PDDeciamal;
		CString HDecimal;
		int TLDLLG;
		int TLDLZG;
		int SJGCLG;
		int SJGCZG;
		int SJPDLG;
		int SJPDZG;
		int JYGCLG;
		int JYGCZG;
		int LCLG;
		int LCZG;
		int QXLG;
		int QXZG;
	};


	typedef struct DhXJJ{
		int JDi;
		double xjj;
		double N;
		double E;
	};

	struct DMXTAB//�����߱�
	{
		CString NameOfWay;//��·��
		CString GH;//��׮�ں�
		double  ml;//���µ����
		double  Level;//��Ƹ߳�
		ACHAR CKml[80];
	};

	struct STATAB//��վ��
	{
		CString NameOfWay;
		CString StaName;//��վ�� 
		CString BGH,EGH,CGH;//�ں�
		double CenDml;//�������
		double StDml;//������
		double EdDml;//�յ����
		double CZLX;//��վ����
		CString CQTKM;//����ͼ����
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJXZ;//�Ľ�����
	};


	struct BRITAB//�����ű�
	{
		CString NameOfWay;//��·��
		int iRXD;//���жα�š���0��ʾ���л���
		int xs;//����1˫��2
		CString BriName;//���� 
		CString BGH,EGH,CGH;//�ں�
		double CenDml;//�������
		double CenH;//���ĸ̣߳�new��
		double StDml;//������
		double StH;//���߳�(new)
		double EdDml;//�յ����
		double EdH;//�յ�߳�(new)
		CString KKSY;//�׿�ʽ��
		long LineNum;//����
		CString SWHt;//һ����ˮλ�߳�
		double BriLen;
		ACHAR SKml[80],EKml[80],CKml[80];
		CString GJZT;

	};

	struct QXJKTAB//���߼ӿ��
	{
		double Rmin,Rmax,JK;// 		
	};

	struct JiYouXianZhanDiKuan//������ռ�ؿ�
	{
		CString GH;
		double ml;
		double LuJiBianJieKuan;//·���߽��
		double ZhanDiKuan;//ռ�ؿ�
	};


	struct YongDiLXB//�õ����ͱ�
	{
		ACHAR XLName[40];
		ACHAR GH[20];
		double dml;//�����
		ACHAR YDLX[40];
	};
	  static  BOOL IsOnSameLine( AcGePoint3d PT1 , AcGePoint3d PT2 , AcGePoint3d PT3 , AcGePoint3d PT4 );

	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	void dTodms(double d, int Nw, ACHAR *dms);
	void DrawRect(AcGePoint3d RightUp,AcGePoint3d LeftDownPt,int Color,double wide);//�����


	bool GetJdno1FromJdstr(CString JDstr, CArray<JDarrayStruct,JDarrayStruct>& JDarray,Adesk::Int16 &Jdno,int CountAllNum);

	void GetJdnoFromJdstr(CString JDstr, Adesk::Int16 &Jdno);

	AcDbObjectId CreateLayer(CString LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight=AcDb::kLnWtByLayer);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType,AcCmColor color,AcDb::LineWeight weight=AcDb::kLnWtByLayer);

	void MoveEntities(AcDbObjectIdArray *EntityIds,double fwj,double dist);

	double fwj(AcGePoint3d sPt,AcGePoint3d ePt);

	void makeXJJFlag(AcGePoint3d Pt1,AcGePoint3d Pt2,double len,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType);

	AcDbObjectId CreateGroup(ACHAR *GroupName,AcDbObjectIdArray IdAr);

	void AdjustAngle(double &j);
	void dlayer(CString layername);
	void RotateEntities(AcDbObjectIdArray *EntityIds,AcGePoint3d Pos,double jiao);
	void LCChr(CString GH,CString LC,int NLC,CString &LCStr);
	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,	int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	//	AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,double lw,ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL,double LTScale=1.0);

	AcDbObjectId makepolyline(AcGePoint3dArray PtArray,bool isClosed,int icolor,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	void getMinMaxXY(ads_point p1,ads_point p2,ads_point p3,ads_point p4,double &xmin,double &xmax,double &ymin,double &ymax);
	void mZOOM(double x1,double y1,double x2,double y2);

	void RToS(double lc,int NLC,ACHAR ch[80]);

	AcDbObjectId CreateBlockFromDwgFile(ACHAR *Path,ACHAR *Name);

	static double angleX(AcGePoint3d sPtL,AcGePoint3d ePtL);//Line(sPtl,ePtl)
	static double angleX(AcDbLine *pL);

	static void makeDX(AcGePoint2dArray PtAy,double R,int icolor,AcDb::LineWeight lw,ACHAR *LayerName, ACHAR *LineType=NULL,bool dxflag=false);//����
	static AcDbObjectId CreateLayer(ACHAR *LayerName);
	static AcDbObjectId   AddEntityToDbs(AcDbEntity *pEntity);

	static AcDbObjectId   AddEntityToDbs(AcDbEntity *pEntity,int mode,bool ispapespace=false);

	static AcDbObjectId  makecircle(AcGePoint3d& startPt,double R,int icolor);
	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt);

	static AcDbObjectId   makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	static AcDbObjectId   makeline2(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	//????

	bool CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,ads_point &pt1,ads_point &pt2,ads_point &pt3,ads_point &pt4);
	bool CalFrame(double d12,double d23,double d34,double d41,AcGePoint3d P1,AcGePoint3d P2,AcGePoint3d P3,AcGePoint3d P4,AcGePoint3d &Pt1,AcGePoint3d &Pt2,AcGePoint3d &Pt3,AcGePoint3d &Pt4);	
	//�ı������ź�����P1��P2��P3��P4���ı����ĸ�����,Pt1,Pt2,Pt3,Pt4Ϊ���ź���ĸ�����
	//d12Ϊ��P1P2��ƽ�ƾ���......������Ϊ���������Ϊ����

	void ZoomTriangleByPoint_O(double dist,AcGePoint3d O,AcGePoint3d &P1,AcGePoint3d &P2);
	//���������Σ����ж���O���䣬ֻ����O���ԱߵĴ���ƽ��dist��dist>0����Ŵ�dist��dist<0������Сdist,���ź�������Ϊ��OPt1Pt2;

	double distptol(AcGePoint3d pt, AcDbLine *Line);
	//�㵽ֱ�ߵľ���
	double distptol(AcGePoint3d O, AcGePoint3d P1,AcGePoint3d P2);
	//��O������P1P2����ֱ�ߵľ���

	//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor);
	static AcDbObjectId   maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor=2,int mode=1,ACHAR *layername=NULL,ACHAR* textStyle=L"����");
	static AcDbObjectId   makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx);
	//	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,ACHAR *layer=NULL);
	static AcDbObjectId   drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND);
	static AcDbObjectId   GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT,AcGePoint3d& pT1,AcGePoint3d& pT2);
	int  getPoint(ACHAR* pMessage, AcGePoint3d& pT);
	void  makeGroup(AcDbObjectId groupId);
	AcDbEntity*  selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
	static double  Get_INDATA_L(double INDATA[5],int *jj);
	static int xlzbxy(struct xlpoint *P0,struct xlpoint *PZ,double l,double INDATA[5],int display);
	static int  xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ);
	int getmodel(double x1,double y1,double x2,double y2,double x3,double y3,double L1,double L2,double R);
	struct resbuf *entdxf( struct resbuf *entdata,int gcode);
	static double xyddaa(double xb,double yb,double xe,double ye,double *f);
	void removeAllButLines(AcDbObjectId groupId);
	void groups();
	static double spiral_x(double A,double l);
	static double spiral_y(double A,double l);
	static double rad(double d_ms);
	static double dms_rad(double rad);
	int getBPMTL(double a,double R,double L1,double L2,double *BPMTL);
	static double deg(double d_ms);
	double spiral_p(double A,double L);
	double spiral_xm(double A,double L);
	static double dms(double ddd);
	void printList(struct resbuf* pBuf);
	void iterate(AcDbObjectId plineId);
	AcDbObjectId createPl_xlpm(int trow,double arrayd[MAXJDNUM][6]);
	static ACHAR*  LCchr(ACHAR  *GH,double lc,int NLC,int mode);
	void calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt,double *tanl, double *R, double *xc,double *yc,double *ang,double *startfw,double *endfw,double *pj);
	void caljd(double x1,double y1,double x2,double y2,double x_1,double y_1,double x_2,double y_2,double *xjd,double *yjd);
	void calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd);
	double Get_Rshu(double design_V,double deltaI, struct GuiFanCanShu mGuiFanCS);
	//   double get_l0(double R);

	double get_l0(double SR,double Design_V,BOOL isJYGuiFanCS, GuiFanCanShu mGuiFanCS);
	double get_R(double SR,double Design_V,BOOL isJYGuiFanCS, GuiFanCanShu mGuiFanCS);

	static AcDbObjectId LoadTextStyle();

	static long factorialfun(int n);		
	static double TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] );
	double TYLC1(double XLC,int NDL,DLBZ DLBTMP[30]);
	bool TYLC1(ACHAR ckml[80], int NDL,DLBZ DLBTMP[30] , double& TLC);
	double XLC1( double TYLC,ACHAR *GH, int NDL,DLBZ DLBTMP[30]);
	///
	double GetSide(double Xo,double Yo,double x1,double y1,double x,double y);
	double ZXJ(double s2,double s1);

	double PROJ_ML(double array2[4*MAXJDNUM][10],int trow2,double xout , double yout , double xon , double yon);
	double XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int trow2);
	static	bool split_ckml(ACHAR str[], double *ml, ACHAR  *ck);
	static void split_ckml(ACHAR ckml[80],double &XLC,ACHAR GH[10]);

	static	int  F_num(ACHAR *str, ACHAR c);
	void putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place);
	void atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4]);
	void NoteBG(AcGePoint3d Opt,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,ACHAR *layername=NULL);
	void DrawBGTri(AcGePoint3d Opt,double sfactor,int iColor,ACHAR *layername=NULL);
	void dlayer(ACHAR lname[]);//ɾ����
	double get_l0(double Design_V,int KNDD,double SR, GuiFanCanShu mGuiFanCS);
	int crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point & pp);
	static void dmltockml(CString GH,double dml,ACHAR ckml[80]);
	void NullInStrto$(CString& text);
	void InStr$toNull();
	int ReadWorkdir();
	bool LoadArx(ACHAR ArxFname[256],ACHAR cmd[80]);
	double get_xjj(double Design_V, GuiFanCanShu mGuiFanCS);
	double get_XJJK(double Design_V,double SR,bool IsJYGuifan, GuiFanCanShu mGuiFanCS);//�߼��ӿ�
	double Get_QXJK(CString TLDJ,double design_V,double SR, GuiFanCanShu mGuiFanCS);//�淶����·���ӿ�
	static int INTERS(double *x,double *y,double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);
	int CalXYjd(double Spt[3],double Ept[3],double array2[4*MAXJDNUM][10],int trow2,int kk, double ResPt[2],double &ProjMl);
	int IfOnXY(double X1,double Y1,double X2,double Y2,double X3,double Y3,double X4,double Y4);//�Ƿ�����Ԫ�н���
	double Search_EXPt(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3],double jiao,double ResPt[2]);
	int Search_XY(double array2[4*MAXJDNUM][10],int trow2,double Pt1[3]);//��λpt1��������Ԫ
	static	double Circle_XY_to_ML(double x , double y,double array2[4*MAXJDNUM][10],int kk);
	//����һ����̣�һ����Ԫ������������Ԫ���ݣ����߼��,����ͶӰ������̼�ͶӰ��
	int CalXjj(double ml1,double array1[4*MAXJDNUM][10],int tarray1,double array2[4*MAXJDNUM][10],int tarray2,double &xjj,double &projml,double resPt[2]);
	//����һ����һ��PZ,������Ԫ����
	int Judge2XLorR(xlpoint PZ,double array2[4*MAXJDNUM][10],int tarray2);

	//	BOOL isJYGuiFanCS;
	//GuiFanCanShu mGuiFanCS;

	int FindWichDLFromFile(ACHAR GH[10],double XLC);


	static void SortArray( CArray<double,double> &a);
	static	void DrawLinePolyline(AcGePoint3dArray ptarr,int color,double wide);
	//���������������ת����-1����+1
	int GetLeftOrRight3JD(double N1, double E1, double N0, double E0, 
		double N2, double E2);

	void GetQD(double CenN, double CenE, double R,
		double OutN, double OutE, int LRFlag,
		double &QDN, double &QDE);	
	void GetCenter3JD(double N1,double E1,double N0,double E0,double N2,double E2,double R,double Lo1,double Lo2,double &CenN,double &CenE);
	double FwjtoAngX(double ang);
	void StdJiao(double &jiao);	
	//plh04.14
	virtual void GetT0min_Lrmin(double DV,double &T0min,double &Lrmin, GuiFanCanShu mGuiFanCS);
	void getLineEndPt(AcGePoint3d sPt,double len,double fwj,AcGePoint3d &ePt);
	//��㵽�ߵľ��� 
	double DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
		double &outY, double &distTo2PointOnLine);	
	////plh
	bool GetTangent(double CenX,double CenY,double R,double  &OutX,double  &OutY,double  &TX,double &TY);
	double PXJ(double fwj1, double fwj2);
	double fwj(double x0,double y0,double x1,double y1);
	double AFA(double fwj1,double fwj2);
	double radiusfree(AcDbLine *Line1, AcDbLine *Line2,AcGePoint3d pk);

	void GetPerPoint(double outX,double outY,double x1,double y1,double x2,double y2,double &perX,double &perY);

	//plh04.14
	bool GetCircleFromT_P_R(double x1,double y1,double x2,double y2,double R,double &cenX,double &cenY);

	//plh04.14//����һ�����㣬ֱʼx1,y1,����x0,y0,��ֹx2,y2,�Ƿ����ذ뾶ϵ�У����ذ뾶
	double GetRFromT1T2(double&L0, double &x1, double &y1, double &x0,double &y0,double &x2,double &y2, BOOL isJYGuiFanCS , GuiFanCanShu& mGuiFanCS, bool IsStandard=false , double DV=350 ,int INTRadius =10 , int JUDGET012=0, BOOL isPreMove= FALSE );

	//plh04.14
	double GetRFromT_P_P(double x1,double y1,double x2,double y2,double &xs,double &ys,double &x0,double &y0,double &xe,double &ye,double &cenX,double &cenY,AcDbObjectIdArray &eraseId ,bool isJYGuiFanCS, GuiFanCanShu& mGuiFanCS, bool IsStandard=true, double DV=350 );
	//�������߷��򣬷��ذ뾶,ǰһ���е㣬���㣬��һ���е�


	//����Ϊ�������������ߵ��г����㣬����ת�ǣ�Բ���߰뾶����һ���Ͷ���ʼ�뾶����һ���Ͷγ����ڶ����Ͷ���ֹ�뾶���ڶ����Ͷγ������г�
	void BAS_DRAW_FUN_JYX::get_T_Len(double zj,double R,double RB,double L1,double RE,double L2,double &T1,double &T2,double &L);
	void BAS_DRAW_FUN_JYX::Get_HHQX_T1T2(double A,double RB,double RE,double &TB,double &TE);
	double BAS_DRAW_FUN_JYX::Adjust_L_JDLuan(double aa,double R,double RB,double Lo1,double RE,double Lo2,double T,int qorh);

public:
	/*��������֮��ľ���Dis�����㹹�ɵ�ֱ����x�ļн�dAngleByX */
	double DisAndAngleByX(double &dAngleByX,double dXstart,double dYstart,double dXend,double dYend);

	/*�Ѽ�������ĽǶ�ת��Ϊ0-2pi��Χ�ڵķ�λ��*/
	void FormFwj(double& dFwj);

	/*��������������λ�ǵó�ת���*/
	double ZxAngle(double dLineAngle2,double dLineAngle1);	

	/*��㵽ֱ�߾���*/
	double DistanceFromPttoLine(double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*��һ����ֱ������������*/
	void ChuizuToZx(double &dXchuizu,double &dYchuizu,double dXout,double dYout,double dXstart,double dYstart,double dXend,double dYend);

	/*���������������λ���Ƿ�˳ʱ��˳������,˳ʱ�뷵��һ����ֵ����Ϊ��ֵ*/
	static double CCW2(double dPt1X,double dPt1Y,double dPt2X,double dPt2Y,double dPt3X,double dPt3Y);

	/*����(dNout,dEout)�Ƿ��ڸ�����Բ����*/
	bool CheckIfOnArc(double dNcircle,double dEcircle,double dAngleStart,double dAngleEnd,double dNout,double dEout);

	int JudgePTLorR(double N,double E,double &dist,double array[4*MAXJDNUM][10],int tarray);
	static double CCW2(double pt1[2],double pt2[2],double pt3[2]) ;

	CString ReverseFind(CString InputCstring);

	/*��ֱ���Ƿ��ص�*/
	bool IsZXCD(double SPt1X,double SPt1Y,double EPt1X,double EPt1Y,double SPt2X,double SPt2Y,double EPt2X,double EPt2Y);

	//д��ɫ�ļ�
	static void WriteColorFile( CString SelFileName ,CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArray );

	//�õ�����i֮ǰ���������ĸ���
	int GetBeforeNum(int i  , CArray<JDarrayStruct , JDarrayStruct >& JDNumArray);

	//����Ĭ�Ϲ淶����
	int SetDefultGuifan(GuiFanCanShu &guifan);

	//����������Ҫ�õ��ĺ������Ӻ�������ģ����ȡ���ϲ�������  ��ΰ2007-08-08
	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext,double ang,double texth ,int icolor,int mode,ACHAR* textStyle,AcDbObjectId layerId);

	//ֱ�����
	void fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
		double& b, double& siga, double& sigb, double& chi2, double& q);
	double gammq(double a, double x);
	void gser(double& gamser, double& a, double& x, double& gln);
	void gcf(double& gammcf, double& a, double& x, double& gln);
	double gammln(double xx);

	void NoteDist(AcGePoint3d Spt,AcGePoint3d Ept,double texth,ACHAR Str[],int iColor,int itextColor,double sfactor,int mode,AcDbObjectId LayerId);

	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor=0,double startWid=0,double EndWid=0,ACHAR LayerName[]=L"0");

	AcDbObjectId  make2dline(AcGePoint3dArray ptarray,int icolor,double startWid,double EndWid,AcDbObjectId LayerId);
//	static AcDbObjectId   makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,AcDb::LineWeight lw=AcDb::kLnWtByLayer,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	AcDbObjectId  makesolid(AcGePoint3d Pt1,AcGePoint3d Pt2,AcGePoint3d Pt3,int icolor,ACHAR *LayerName);
	int AutoEntBK(ads_point p0, ads_point p1);
	int getrbpoint(ads_point pt,struct resbuf *ebuf,int code);
	struct resbuf *get_resbuf(int code,struct resbuf *p);
	struct arc_lst_JYX *findxyr(ads_point, ads_point, ads_real);
	struct point_lk_JYX *findroot(ads_point, ads_point, struct arc_lst_JYX *, ads_real bulge,
		ads_point, ads_point);
	struct ptt_node_JYX *get_brkpt(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *getbrkpt(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *get_plinept(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *getplinept(struct resbuf *, ads_point, ads_point);
	struct ptt_node_JYX *get_Arcpt(struct resbuf *,ads_point, ads_point);
	struct ptt_node_JYX *getArcpt(struct resbuf *,ads_point, ads_point);
	void atodmss1(double alpha, int &d, ACHAR mm[4], ACHAR ss[12],int Pres);



	//����ɫ�ļ�
	static BOOL ReadColorFile( CString SelFileName , CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut );

	//����Ĭ����ɫ����
	static void SetDefaultColor(CArray<SetLineAttriStruct,SetLineAttriStruct>& SetLineAttrArrayToOut );

	static double DmlToCml(double dml,DLBZ DLBArray[MAXDLNUM],int DlSum);

	AcDbObjectId GUXWXY(double INDATA[5],struct xlpoint *P0,struct xlpoint *PZ,int bz,int color,double Lwid,ACHAR *LayerName);

	AcDbObjectId drawspi(double INDATA[5],double l,struct xlpoint *P0,struct xlpoint *PZ,int bz,double ND,int color,double Lwid,ACHAR *LayerName);

	static AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor,AcDb::LineWeight lw,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);
	static AcDbObjectId makeline(AcGePoint3d& startPt,AcGePoint3d& endPt,int icolor=0,double lw=1.0,ACHAR *LayerName=NULL, ACHAR *LineType=NULL);

	static AcDbObjectId makearc(AcGePoint3d startPt,AcGePoint3d endPt, double R,int npx,int color,int fh=1,ACHAR *LayerName=NULL);
	static void GetAllRXD(CString MDBName,CString XLName,CArray<CString,CString>&AllRXDName);

	static bool GetArea(const AcDbObjectIdArray& curveIds1,const AcDbObjectIdArray& curveIds2,double & ReArea);

	AcDbObjectId FromePLineBySEMl(double Scml, double Ecml, double (*XYArray)[10], int XYNum);

	CString GetProByMdbName(CString mdbPath);


//	static int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ , GTZX *ZuoXianpGtzxToIn,GTZX * JiXianpGtzxToIn,GTZX * YouXianpGtzxToIn , CArray<GJDInf,GJDInf>& GJDInfTabArrayToIn) ;

	//virtual int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2, DLBZ DLBtoGT ,int NDLtoGT , CArray<JDarrayStruct,JDarrayStruct>& JDarraytoGT ,
	//    struct xlpoint *PZ , GTZX *ZuoXianpGtzxToIn ,
	//	GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn , GTZX * GTXGtzxToIn ,
	//	CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn );
	//virtual int xlpoint_pz(double array2[4*MAXJDNUM][10],int trow2,struct xlpoint *PZ ,  GTZX *ZuoXianpGtzxToIn ,
	//	GTZX * JiXianpGtzxToIn , GTZX * YouXianpGtzxToIn ,CArray<GJDAllInforStruct,GJDAllInforStruct>& BasicSectionInforToIn) ;
} ;



#endif // !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
