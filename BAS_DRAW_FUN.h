// BAS_DRAW_FUN.h: interface for the BAS_DRAW_FUN class.
//
//////////////////////////////////////////////////////////////////////
// BAS_DRAW_FUN.h: interface for the BAS_DRAW_FUN class.
//
/////////////////////////////////////////////////////////////////////
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
#include "rxdefs.h"
#include "afxtempl.h"
#include "Struct.h"

#include <string>
#include <fstream>
using namespace std;


#if !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
#define AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_
#define pi 3.14159265358979323846264338327 
#define PI 3.14159265358979323846264338327
#define Pai 3.14159265358979323846264338327
//////////////////////////////////////////////////////////////////////////
#define SPIRALPRECISION 40
#define PRECISION 0.0001
#define MAXJDNUM 200
#define MAXDLNUM 200
#define MAXBPDNUM 300
#define Spoint(pt, x, y, z)  pt[X] = (x);  pt[Y] = (y);  pt[Z] = (z)
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseSettting.h"
//�������߾���
#define SPIRALSTEP 0.5
//�����
#define VERY_MAX 999999999999999999
//����ģʽ
#define KNOW_A1 1
#define KNOW_A2 2
#define KNOW_K_A1_A2 12

//����ת
#define LEFT -1
#define RIGHT 1
//��������
#define IsJiaoDian 0
#define IsCenter 1
//��ӹ�ϵ
#define NotRelation 0//�޹�
#define C_Link 1//C��
#define S_Link 2//S��
#define Luan_Link 3//����
	//����ת
#define LEFT -1
#define RIGHT 1
//�Ƿ��ͷ
#define HuiTou -1
#define NotHuiTou 1
//��������
#define IsJiaoDian 0
#define IsCenter 1
#define IsLine 2
#define IsArc 3
#define IsCircle 4
#define IsJdCenter 5

///////////////////////�������߰뾶���Ʊ仯��ʶ///////////////////////////
//2005.5.12 hhp
///////////////////////��·��Ԫ���յ㴦�������ʶ/////////////////////////
#define VerticalUp        8 	               		
#define ToFirstArea       1 
#define HorizontalRight   5
#define ToSecondArea      2
#define VerticalDown      6
#define ToThreeArea       3
#define HorizontalLeft    7
#define ToFourArea        4            
///////////////////////��·��Ԫ���յ㴦�������ʶ/////////////////////////
//2005.5.12 hhp
//���ݾ��ȶ���;
#define DataPrecision 0.00000001
//2005.5.12 hhp
/////////////////////////////////////////////////////////////////////////
//2005.5.12 hhp
///////////////////////�������߰뾶���Ʊ仯��ʶ///////////////////////////
#define LargeToSmall -2                         //�����ĳһ�뾶;
#define BigToSmall   -1                         //ĳһ��뾶��ĳСһ�뾶;
#define NoChange      0						    //�뾶�������ı���ֱ����Բ;
#define SmallToBig    1						    //ĳһС�뾶��ĳһ��뾶;
#define SmallToLarge  2						    //ĳһ�뾶�������;
///////////////////////������·��Ԫƫ���ʶ///////////////////////////////
#define DeflectRigth 1
#define NoneDeflect  0
#define DeflectLeft -1
///////////////////////������·��Ԫƫ���ʶ///////////////////////////////
//2005.5.14 hhp
///////////////////////������·��Ԫ���ͱ�ʶ///////////////////////////////
#define LineItem     1
#define CirOrArcItem 2
#define SprialItem   3

//20190816	������ɫ����
#define BYLAYER		0	//��ͼ��
#define RED			1	//��ɫ
#define YELLOW		2	//��ɫ
#define	GREEN		3	//��ɫ
#define CYANBLUE	4	//��ɫ
#define BLUE		5	//��ɫ
#define MAGENTA		6	//���
#define WHITE		7	//��ɫ/��ɫ

#include "SpiralY.h"
class SPIRAL;
//��Ԫ����
struct XianYuan
{
	enum
	{
		LINE = 1,//ֱ��
		ARC = 2,//Բ��
		PreSPIRAL = 3,//������ǰ����0->R 
		AftSPIRAL = 4,//�����ĺ󻺣�R->0
		BigToSmallSPIRAL = 5,//�������������ߣ���Բ-->СԲ
		SmallToBigSPIRAL = 6//�������������ߣ�СԲ-->��Բ
	};
};


struct STuKuang
{
	double StartTYLC,EndTYLC;//ͼ�����յ��ͳһ���
	CString StartXCLC,EndXCLC;//ͼ�����յ���ֳ����
	double StX,StY;//ͼ����·������
	double EdX,EdY;//ͼ����·������
	double CenX,CenY;//ͼ������
	double LtUpX,LtUpY;//���Ͻǵ�����
	double LtBmX,LtBmY;//���½ǵ�����
	double RtUpX,RtUpY;//���Ͻǵ�����
	double RtBmX,RtBmY;//���½ǵ�����

	//��תƽ�ƾ���
	AcGeMatrix3d xform;
	double newStX,newStY;//ͼ����·������
	double newEdX,newEdY;//ͼ����·������
	double newCenX,newCenY;//ͼ������
	double newLtUpX,newLtUpY;//���Ͻǵ�����
	double newLtBmX,newLtBmY;//���½ǵ�����
	double newRtUpX,newRtUpY;//���Ͻǵ�����
	double newRtBmX,newRtBmY;//���½ǵ�����
	ads_point inspt;

	STuKuang()
	{
		StartXCLC = L"";
		EndXCLC = L"";
		StartTYLC = 0.0; EndTYLC = 0.0;
		StX = 0.0; StY = 0.0;
		EdX = 0.0; EdY = 0.0;
		CenX = 0.0; CenY = 0.0;
		LtUpX = 0.0; LtUpY = 0.0;
		LtBmX = 0.0; LtBmY = 0.0;
		RtUpX = 0.0; RtUpY = 0.0;
		RtBmX = 0.0; RtBmY = 0.0;

		newStX = 0.0; newStY = 0.0;
		newEdX = 0.0; newEdY = 0.0;
		newCenX = 0.0; newCenY = 0.0;
		newLtUpX = 0.0; newLtUpY = 0.0;
		newLtBmX = 0.0; newLtBmY = 0.0;
		newRtUpX = 0.0; newRtUpY = 0.0;
		newRtBmX = 0.0;  newRtBmY = 0.0;
	}
};

struct ptt_node{
	ads_point pt0;
	ads_point pt;
	ads_point pt1;
	struct ptt_node *next;
};
struct arc_lst{
	ads_real R;
	ads_real xo;
	ads_real yo;
};
struct point_lk{
	ads_point pt;
				struct point_lk *next;
};
class BAS_DRAW_FUN 
{
public:
	struct DLBZ  //�ںŸı�㡢�����㡢���յ�Ĺںš���̱�
	{ 
		double BLC,ELC;//DL;  //  �����Ķ�ǰ��̺ͶϺ���̡�������ֵ(+ֵΪ����;-Ϊ����)
		double DL;
		CString BGH,EGH; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�
		double TYLC;
		Adesk::Int16 FLDNo;//����α��
		CString FLDName;//���������
	};

	struct Point
	{
		double x,y,z;
	};

	

	
	struct GLTZSTAdata//���������ֳ����
	{
		double FLDSmlOnZX,FLDEmlOnZX;//�������ʼ���,��ֹ���
		CString FLDname;
		double NoseSmlOnZX,NoseSmlOnFLD;//��������ͳһ���
		double NoseEmlOnZX,NoseEmlOnFLD;//��������ͳһ���	
		double StartR,EndR;
		///////////////////////////////////////////////////////////////////////////////////
	};
	
 	struct HDMDMXdata//��̱��
	{
		int  lcflag;//������ͣ�-1������꣩,1-9���ױ꣬0-������
		ACHAR flag[10];
    	int BTxz;//�������� 0-���ŷ��� -1-����ͷ,-2-����ͷ,-3-ȫ��ͷ -4-����ͷ,-5-����ͷ,-6-ȫ��ͷ
                                 // 1-����β, 2-����β, 3-ȫ��β  4-����β,5-����β,-6-ȫ��β
		double lc;//ͳһ���
///////////////////////////////////////////////////////////////////////////////////
		double EarH;//��׮�����
        int ZEarPtNum,YEarPtNum;
        CArray<Point,Point>ZEarPtAry;
		CArray<Point,Point>YEarPtAry;
	};
///////////////////////////////////////////////////////////////////////////////////////////////




   
 
  



	//////////////////////////////////////////////////////////////////////////





	//////////////////////////////////////////////////////////////////////////

    struct DMXdata
	{
		CString GhK;
		double rcd;//���
		double m_dmgc;//����߳�
		long NumL; //��������
		double Learpt[200];//������
		long NumR; //�ҵ������
		double Rearpt[200];//�ҵ����
	};


	struct BDData//�Ƕ��������
	{
		CString XL1;
		CString XL2;
		int InOrOut;//0:����������ӷ����1:������̼�С����
		int ZDStOrEd;//0:�ѵ���㴦�Ƕ�,1:�ѵ��յ㴦�Ƕ�
		int Lmhp;//1 ͬ�� -1 ����
		double C1;//1��ƫ��ֵ
		double C2;//2��ƫ��ֵ
		double Z1;//1�����ζ˺�Ĺ��ɶγ���
		double Z2;//2�����ζ˺�Ĺ��ɶγ���
        double YljRNose;//Ӳ·��Ƕ˰뾶
		double TljRNose;//��·��Ƕ˰뾶
		double Yljdml1;//Ӳ·��Ƕ���1�ߵ��ֳ����
		double Yljdml2;//Ӳ·��Ƕ���2�ߵ��ֳ����
		double Tljdml1;//��·��Ƕ���1�ߵ��ֳ����
		double Tljdml2;//��·��Ƕ���2�ߵ��ֳ����
		double FldDml1;//�������������
		double FldDml2;//�������ѵ����
		double YCSpt[2];
		double YCEpt[2];
		double TCSpt[2];
		double TCEpt[2];
		double YNosePt[2];
		double TNosePt[2];
		double YLjPt1[2];//1��·�紦����
		double YLjPt2[2];//2��·�紦����
	};

//////////////////////////////////////////////////////////////////////////////////
	struct CRSDAT
	{
		bool IfLink;//�Ƿ��������
		double glcml;//�������
		CString glRoadName;
        
		ACHAR ckml[80];
		double cml;
		double dml;
		int DorS;//1-��·�棬2-˫·��
		int DuanLuoInf;//��������ڶ�����Ϣ0-���ߣ�1-˫�߲��У�2-˫������·�����˫·�� 3-˫�����б��·��뵥��·��
		double xjj;//�߼�࣬����Ϊ0,��������Ϊ-����Ϊ+
		double ZDesH,YDesH;
		double LZDK,RZDK;
		double ZZJ,YZJ;//����֧��
		double ZPJPT[2],YPJPT[2];
		//DesH·�����Ƹ̺߳�EarH����̣߳�HDM�Ļ�ͼ���ݣ�//ָ����
		double BriZZDK,BriYZDK,TunZZDK,TunYZDK;
		double RHFL[10],RHFR[10];  //����·���5��
		double zlmhd[3],ylmhd[3];
		int ZDMXZ,YDMXZ;//���ҷ���������
		double  Zt,Yt;
		double ZHc,YHc,Y_ZHc,Y_YHc;//���ڸ�
		int zptsum,rptsum,Lnpt,Rnpt;
		AcGePoint3dArray RPTL,RPTR;
		int Ljdnum,Rjdnum;
		//ָ����
		double FLDcml;//��ص�·ͶӰ���
		CString FLDname;//��ص�·��
		double Y_BriZZDK,Y_BriYZDK,Y_TunZZDK,Y_TunYZDK;
		double Y_RHFL[10],Y_RHFR[10];  //�����·���5��
        double Y_zlmhd[3],Y_ylmhd[3];
		int Y_ZDMXZ,Y_YDMXZ;//���ҷ���������
		double  Y_Zt,Y_Yt;
		int Y_Lnpt,Y_Rnpt;
		AcGePoint3dArray Y_RPTL,Y_RPTR;
		int Y_Ljdnum,Y_Rjdnum;
		
		double Ldeltah,Rdeltah;//����·���������ߵĸ߲��
		double Y_Ldeltah,Y_Rdeltah;//����·���������ߵĸ߲��
		double newLYd,newRYd;//��·�����ĵĵ���߲㣬��·�����ĵĵ���߲㣨�ָ�Ϊ��·���ߵĵ���̣߳�
		double  at,aw;
        double offsetX,YoffsetX;//·��������һ��ƫ������������
		double LmH,YljH;//·���,Ӳ·���
		double Xo,Yo;//��ͼ���յ�
		double hi,wi;//
		double ZXo,ZYo,ZX1,ZY1,tX0,tY0;//׷��ԭ��,tX0 tY0�����ע�͵�
		double Xmin, Xmax, Ymin, Ymax;//��ͼ���յ�,
		double Ldh,Rdh;
		double qxjk,Yqxjk;//���߼ӿ�,��-��+,˫·��ʱ�����������߼ӿ�
		//		double LJFLK;
        int mflag;//�Ƿ��޸�
		double DX;
		//	
		//		
		//	//	double LPjqPt[]
		//
		//		//���߻�һ��
		int LGouPtNum,RGouPtNum,LJSGouPtNum,RJSGouPtNum;////////wwq5.14
		int LGouProperty, RGouProperty;//0-�½�ǽ��1-ˮ����1-��ˮ��,2-·��·�繵,-1-��ʼ����ˮ����2-�๵
		int LIfSetGouH,RIfSetGouH;
		double LGouH,RGouH;
		double Lgou[36],Rgou[36],LJSgou[36],RJSgou[36];//ˮ��7��/////wwq5.14	 
		//	
		//		//����
		int Y_LGouPtNum,Y_RGouPtNum,Y_LJSGouPtNum,Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty,Y_RGouProperty;
		double Y_Lgou[36],Y_Rgou[36],Y_LJSgou[36],Y_RJSgou[36];//ˮ��7��/////wwq5.14
		
		double FillArea ;//�����
		double CutArea;  //�����
		double FillBetterSoilArea; //����������
		double QBTArea ;//��������
		double WYNArea ;//���������
		double GouTarea;//ˮ�������
		double GouWarea;//ˮ�������
		double WFFLarea[6];//�����ڷ�����ȷ���������////wwq
		double HJarea ;//�������
		double SGJQarea;//�������
		double DQlength ;//��ǽ�³�
		double Grasslength ;//�̲��³�
		//		int DMinf;
		bool m_IfXJGou;		
		///////////////////////////////���ڽ������
		double Z_LFillArea ,MidFillArea , Y_RFillArea;
		double Z_LCutArea , MidCutArea , Y_RCutArea;
		double Z_LFillSSTArea,MidFillSSTArea, Y_RFillSSTArea;//����ˮ�����
		double Z_LQBTArea ,MidQBTArea ,Y_RQBTArea;
		double Z_LFillBetterSoilArea ,MidFillGLTArea ,Y_RFillBetterSoilArea;
		double Z_LHJarea ,MidHJarea ,Y_RHJarea;
		double Z_LSGJQarea ,MidGouJQarea ,Y_RSGJQarea;
		double  Z_LDQlength ,MidDQlength ,Y_RDQlength;//��ǽ�³�
		double Z_LGrasslength ,MidGrasslength,Y_RGrasslength;//�̲��³�
		double  Z_LGouTarea , Y_RGouTarea;
		double Z_LGouWarea ,Y_RGouWarea;

		double ZCG_GDR,YCG_GDR;//�๵��R;
		
		AcDbObjectId LgouId,RgouId;
		bool IfHasDes;
        int IfHasZlink,IfHasYlink;
		CRSDAT *zlink,*ylink;
		double ZDorS,YDorS;
		double ZHTHD,YHTHD;//��������
		int ZHTXZ,YHTXZ;
		bool IfFldSEHdm;//�Ƿ��������ն���
		int IsInsertHdm;//�Ƿ�Ϊ���ڲ������
		int IfSetDQ;//0�޵�ǽ��-1-����е�ǽ, 1-�Ҳ��е�ǽ��2-���඼��
		//		int xlxz;//��·����
		//		int r_bh;//���жα��
		double OffsetByXLZX;//·�������������·���ߵ�ƫ����

		CRSDAT()
		{
			offsetX = 0.0;
		}
	};
    



	struct ZQZJTAB//ֱ��ת�Ǳ�
	{
		CString JDNO;
		CString JDZH;//����׮��
		double JDN,JDE;//��������
		double JDJJ;//���һ������ļ��
		double FWJ;//��ǰ�������һ����������ߵķ�λ��
		double JIAZX;//��ǰ�������һ������ļ�ֱ�߳�
		double AFA;//��ǰ�����ת��
		double T1,T2;//ǰ�У�����
		double R1,R2,R3;//ǰһ�����㣬��ǰ���㣬��һ������İ뾶
		double A1,A2;//ǰ������
		double Ls1,Lc,Ls2,L;//ǰ�������߳���Բ���߳����󻺺����߳��������ܳ�
		double OutDist;//���

		CString ZHZH;//��һ�����������׮��
		double ZHN,ZHE;//��һ���������������
		
		CString HYZH;//��һ���������յ㼰Բ�������׮��
		double HYN,HYE;//��һ���������յ㼰Բ�����������
		
		CString YoZH;//Բ�����е�׮��
		double YoN,YoE;//Բ�����е�����

		CString YHZH;//�ڶ�����������㼰Բ�����յ�׮��
		double YHN,YHE;//�ڶ�����������㼰Բ�����յ�����
		
		CString HZZH;//�ڶ����������յ�׮��
		double HZN,HZE;//�ڶ����������յ�����

		CString Note;
	};			 
	struct XLManager
	{	
		int No;//��·���
		CString Name;//��·����
		int PreNo,NextNo;//ǰ������·���
	};

	struct JdCenter
	{
		//��������
		Adesk::Int16 JDXZ;
		double N,E,R,A1,A2,Ls1,Ls2;
		double JDN,JDE;//��������
		double CenN,CenE;//Բ������
		//ǰ��ߵķ�λ�ǣ���ǰ����Ϊ���
		int CirXYNo;
		double PreFwj,AftFwj;
		double T1,T2,L;//ǰ�У���ǰ�����߳�
		double JdNo;//�����
		//����ת
		Adesk::Int16 LeftOrRight;
		//�Ƿ��ͷ
//		bool IsOrNotHuiTou;
		Adesk::Int16 IsOrNotHuiTou;//0 ����ͷ 1 ˳����ת��ͷ 2 ˳����ת��ͷ 3 ������ת��ͷ 4 ������ת��ͷ
		//��ӹ�ϵ �޹� S�� C�� ����
		Adesk::Int16 LinkAfter;

		Adesk::Int16 FLDNo;//����α��

		double afa;//�����ת��
		double DistToAfterJD;//���һ������ľ���
		//20190712	����Ĭ�Ϲ��캯��
		JdCenter()
		{
			JDXZ = 0;
			N = E = R = A1 = A2 = Ls1 = Ls2 = 0.0;
			JDN = JDE = CenE = CenN = 0.0;
			CirXYNo = 0;
			PreFwj = AftFwj = 0.0;
			T1 = T2 = L = 0.0;
			JdNo = 0.0;
			LeftOrRight = 0;
			IsOrNotHuiTou = 0;
			LinkAfter = FLDNo = 0;
			afa = 0.0;
			DistToAfterJD = 0.0;
		}
	};

	struct DUANLUO//����ṹ��
	{
//		int StartJDNo,EndJDNo;//���ս����
//		int StartDLNo,EndDLNo;//���ն�����
		double StartTYLC1x,EndTYLC1x;//���յ�ͶӰ��һ���ϵ����
//		GTZX2 *iGtzx2;//�ö����Ӧ��GTZX2����
	};
	

	
	struct ModifyRoadTab
	{
		CString RoadName;//��·��
		CString RailName;//�ཻ��·��
		CString SCkml;//������
		CString RailCkml;//���洦��·���
		double Raillevel;//���洦��·�߳�
		CString ECkml;//��ֹ���
		double RoadDml;//���洦��·���
		double RoadLen;//��·����
		double InterAng;//�����
		double DetaH;//�߲�
		CString UpDown;//��·�ڵ�ǰ��·���·�
		CString KKSY;//�׿�ʽ��
		CString RandType;//��·���͵ȼ�
		double LjWide;//·�����
		double LmWide;//·���
		double LmThick;//·����
		CString LmType;//·���������
		double LmArea;//·�湤�����
		double FillVol;//�
		double CutVol;//�ڷ�
		double MasonryVol;//�ع�����
		double LandArea;//ռ�����
		CString LandAreaStr;//ռ������ַ���
		CString Note;//��ע

		ModifyRoadTab()
		{
			RoadName = L"";
			RailName = L"";
			SCkml = L"";
			RailCkml = L"";
			Raillevel = 0.0;
			ECkml = L"";
			RoadDml = 0.0;
			RoadLen = 0.0;
			InterAng = 0.0;
			DetaH = 0.0;
			UpDown = L"";
			KKSY = L"";
			RandType = L"";
			LjWide = 0.0;
			LmWide = 0.0;
			LmThick = 0.0;
			LmType = L"";
			LmArea = 0.0;
			FillVol = 0.0;
			CutVol = 0.0;
			MasonryVol = 0.0;
			LandArea = 0.0;
			LandAreaStr = L"";
			Note = L"";
		}

	};

	struct EXDLB{//���߶�����
		int iDuanLuo;//�����//plh04.29
		int no;//��ʾ�ڼ������ж�
		CString RXDName;//�Ծ����ж�����
		double BLC,ELC,TYLC;  //  �����Ķ�ǰ��̡��Ϻ���̡�������ͳһ���
		CString   BGH,EGH; //  �����Ķ�ǰ��̹ںźͶϺ���̹ں�	
	};
	
	struct DoubleLineData//˫��ƽ������
	{
		int iDuanLuo;//�����//plh04.29
		int JDXZ;//�������� 0-���жν���,1-���жν���
		int jdno;//���߽����
		double x,y,Lo1,R,Lo2;
		int jdno1;//��Ӧһ�߽������
		double xjj;//�߼��//��������Ϊ������Ϊ��
		double qxjk;//���߼ӿ�
		double NYDL;//��ҵ����
		ACHAR ZHLC[80];//ֱ�����
		ACHAR JDLC[80];//����ͶӰ��һ���ϵ����//plh05.08
		bool ISXJJFirst;//�Ƿ�����߼�����
		bool ISStartEnd;//�Ƿ�Ϊ���յ�
		bool ISShow;//�Ƿ���ʾ���û�
	};
////////////////////////////////////////����plh


	struct xlpoint { double x,y,a,r,lc; };
	AcGePoint3d resetjd_centerid(AcGePoint3d  Point, AcDbObject* pEntity, AcDbObjectId& lineId);


   
	struct PDTAB//���µ��
	{
		CString GH;//���µ�ں�
		double  ml;//���µ����
		double cml;
		double  Level;//��Ƹ߳�
		double Rshu, RLen;//�����߰뾶,����
		double degree, length;//�¶ȣ��³�
		CString Notes;//��ע
		ACHAR CKml[80];
	};

	typedef	CArray<PDTAB,PDTAB> BPDArray;

	struct DMXTAB//�����߱�
	{
        CString GH;//��׮�ں�
		double  ml;//���µ����
		double  Level;//��Ƹ߳�
		ACHAR CKml[80];
	};

	struct QXTAB//���߱�
	{
		int jdno;//�����
		double x,y,zj,R,l1,l2,T1,T2,L,jzxl;
		CString BGH,EGH;//ZH,HZ׮�ں�
		double Bml,Eml;//�������
		double InDL;//		��ҵ����
	};

   
	struct TWTAB//�ݵ����߱�
	{
        CString GH;//��׮�ں�
		double  ml;//��׮���
		double cml;
		double  DMGC;//����߳�,��Ƹ߳�	
		ACHAR CKml[80];
	};


	struct ZYJSBZTAB//��Ҫ������׼��
	{
        CString TLDJ;//��·�ȼ�
		CString ZXSM;//������Ŀ
		CString GDLX;//�������
        double SDMB;//�ٶ�Ŀ��
		double XJJ;//��׼�߼��
		double RtjMin,RtjMax;//�Ƽ��뾶С����
		double XZPD;//�����¶�
		double DFXYXC;//��������Ч��
		double Rmin1,Rmin2;//��С���߰뾶
	};

	struct Culvet//��Ӧ������
	{
		CString QHType;//�ź�����
		double CenDml;//�������
		double cml;//��Ӧ���������
		CString KKMS;//�׿����� 
		CString QHMS;//�ź�����
		double ht;
        double ang;//����
	} ;

	struct  RoadProperty
	{
		CString Grade;//�ȼ�
		CString TerrainType;//���
		CString DXLB;//�������
		int DV;//��Ƴ���
	};

	struct  ZdmDesStandard
	{
		CString MAXPD;//�������
		CString TRshu1;//͹�����߰뾶һ����Сֵ
		CString TRshu2;//͹�����߰뾶������Сֵ
		CString ARshu1;//�������߰뾶һ����Сֵ
		CString ARshu2;//�������߰뾶������Сֵ
		CString Lshu1;//�����߳���һ����Сֵ
		CString Lshu2;//�����߳��ȼ�����Сֵ
		CString PCMin1;//�³�һ����Сֵ
		CString PCMin2;//�³�������Сֵ
	};
	


	struct CGJSCS
	{
		CString	m_HxCgJbl;//���ͳ��߽�����
	    CString	m_ZxCgJbl;//ֱ�߳��߽�����
        int CgMode;//0 1 2 3 4 5;
		int JkMode;//0 ���� 1������
	    int CgLenMode;//0-��ȡ��;1-1��������";"2-5��������
		CGJSCS()
		{
			m_HxCgJbl.Empty();
			m_ZxCgJbl.Empty();
			CgMode = 0;
			JkMode = 0;
			CgLenMode = 0;
		}
  	};
	
	struct NoteStr
	{
		int rownum,colnum,ntext;
		int fitinf;//=0 not fit mode ; =1 fit mode;
		double texth,dv;//dv fitʱ ��λ��ʱ�ĳ���
		CString text;//�����ֻ��һ���ı� ntext=1
		CString text1;//���������ı� ntext=2
		CString text2;//�����ı� ntext=3
	};
	
    struct Table //���ṹ
	{
		int nrow,ncol;
		double RowH[14],ColWid[8];
		AcGePoint2d LDpt; 
		int Nstr;
        NoteStr Str[30];
	};
	
	struct BMSTRUCT
	{
		//ˮ׼���š��̡߳���̹ںţ������������·���߾��룬����
		CString BianHao;
		double GaoCheng,N,E;
	};
	
	struct DXSTRUCT
	{
		//׮�ţ��̣߳����꣬��λ��
		CString BianHao;
		double GaoCheng,N,E;
	};
	
    struct CTRLHT//���Ʊ��
	{
		double dml;
		double Ht;
		CString Att;//���� �����Ʊ�ߡ������Ʊ�� �ؾ����			
	};
	struct XYarray
	{
		double xyarr[10];
		AcDbObjectId eID;
	};
//	CArray<XYarray,XYarray>pXYArr;
	struct BXLMKD
	{
		double *ptarr;
		int bxinf,ptnum;//��������
		double bxsml,bxeml;
		int LorR;//���߲߱�
		AcDbObjectId eID;
	};
	struct GLROAD
	{
		CString glxl;
		double glsml;
		double glmml;
		double gleml;//
		int torf;//1ͬ���� -1������
		double sml;//��sml~eml��Χ���й�����·����Ӧ�����Ϊglsml~gleml
        double mml;		
		double eml;//��Ӧ������·glxl
		int qorh;
		int glxlxz;//������·���� 0-���� 1-�ѵ�
	};

	struct PPCDdata//���³�������
	{ 
		double sdml,edml;//"�ֶ��������"
		double max_hp;//������
	};
	

	BAS_DRAW_FUN();
	virtual ~BAS_DRAW_FUN();
	//***********************CAD��ͼ��غ���***********************
	//����ͼ��
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType);
	static AcDbObjectId CreateLayer(ACHAR *LayerName);
	AcDbObjectId CreateLayer(ACHAR *LayerName, ACHAR *LineType, AcCmColor color, AcDb::LineWeight weight = AcDb::kLnWtByLayer);
	//ɾ��ͼ��
	void dlayer(ACHAR lname[]);
	//���õ�ǰͼ��
	static void SetCurLayer(ACHAR *LayerName);
	//�޸�ͼ����ɫ
	static void SetLayerColor(ACHAR* LayerName, int ColorIndex);
	//���Ʊ��
	void maketable(struct Table& tab, int icolor = 0, double startWid = 0, double EndWid = 0, ACHAR* LayerName = NULL);
	//���뵽���ݿ�
	static AcDbObjectId AddEntityToDbs(AcDbEntity *pEntity, int mode = 0, int ShiftSpace = 0);
	//����Բ
	static AcDbObjectId  makecircle(AcGePoint3d& startPt, double R, int icolor, ACHAR *LayerName = NULL);
	bool makecircle(AcGePoint3d& centerPt, double R, AcDbObjectId& circleId, int iColor);
	//����Բ��
	static AcDbObjectId   makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx);
	AcDbObjectId  makearc(AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx, int color, int fh = 1, ACHAR *LayerName = NULL);
	Adesk::Boolean  G_makearc(AcGiWorldDraw* pWd, AcGePoint3d startPt, AcGePoint3d endPt, double R, int npx);
	bool makearc(AcGePoint3d startPt, double dFwj, double R, double Len, int LorR, AcDbObjectId& arcId, int iColor);

	static AcDbObjectId CreateHatch(AcGePoint3dArray Pts, ACHAR* patName, bool bAssociative,int ColorIndex,bool iscir=false);
	//����ֱ��
	static AcDbObjectId   makeline(AcGePoint3d& startPt, AcGePoint3d& endPt);
	static AcDbObjectId   makeline1(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor);
	Adesk::Boolean  G_makeline(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor);
	Adesk::Boolean  G_makelineW(AcGiWorldDraw* pWd, AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor, AcDb::LineWeight lw);
	static AcDbObjectId makeline(AcGePoint3d& startPt, AcGePoint3d& endPt, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	bool makeline(AcGePoint3d startPt, double dFwj, double Len, AcDbObjectId& lineId, int iColor);
	bool makeline(AcGePoint3d startPt, AcGePoint3d endPt, AcDbObjectId& lineId, int iColor);
	//���ƶ����
	AcDbObjectId CreateDLPolyLine(AcGePoint3dArray points, int ColorIndex, double SetoffX, double SetoffY, int ClassGrade = 1);
	AcDbObjectId Create3dPolyLine(AcGePoint3dArray points, int ColorIndex,double SetoffX, double SetoffY,int ClassGrade=1);
	AcDbObjectId   makepolylineTD(AcGePoint3dArray& PtAy, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = L"RAILL", double LTScale = 1.0, double Offset = 0.0)  const;	//20190707���
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy, int icolor = 0, AcDb::LineWeight lw = AcDb::kLnWtByLayer, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	static AcDbObjectId   makepolyline(AcGePoint2dArray PtAy, int icolor = 0, double lw = 1.0, ACHAR *LayerName = NULL, ACHAR *LineType = NULL, double LTScale = 1.0);
	AcDbObjectId makepolyline(AcGePoint3dArray PtArray, bool isClosed, int icolor = 0, AcDb::LineWeight lw = AcDb::kLnWtByLayer, ACHAR *LayerName = NULL, ACHAR *LineType = NULL);
	static AcDbObjectId  make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, ACHAR *layer = NULL, double ConstWide = 1.0);
	AcDbObjectId  make2dline(AcGePoint3dArray ptarray, int icolor, double startWid, double EndWid, AcDbObjectId LayerId);
	bool makepolyline(AcGePoint2dArray& vertices, AcDbObjectId& polylineId, int iColor);
	//��������
	static AcDbObjectId   maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor = 0, int mode = 3, ACHAR* textStyle = L"����", ACHAR *LayerName = NULL);
	static AcDbObjectId MakeMText(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, double Width,
		int icolor = 0, AcDbMText::AttachmentPoint mode = AcDbMText::AttachmentPoint::kMiddleCenter, ACHAR* textStyle = L"����", ACHAR *LayerName = NULL, double LineSpacingFactor = 1.0);
	Adesk::Boolean  G_maketext(AcGiWorldDraw* pWd, AcGePoint3d& Pt, ACHAR* ctext, double ang, double texth, int icolor, int mode = 1);
	AcDbObjectId H_maketext(AcGePoint3d& pos, ACHAR* ctext, double ang, double texth, int icolor, int mode, ACHAR* textStyle, AcDbObjectId layerId);
	AcDbObjectId maketext(AcGePoint3d lpos, AcGePoint3d rpos, ACHAR* ctext, double ang, double texth, int icolor = 0, ACHAR* textStyle = L"����", ACHAR *font = L"simplex.shx", ACHAR *bigfont = L"hzdx.shx", ACHAR *LayerName = NULL);
	//�������
	AcDbObjectId  drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND);
	AcDbObjectId  drawspi(double INDATA[5], double l, struct xlpoint *P0, struct xlpoint *PZ, int bz, double ND, int color, double Lwid, ACHAR *LayerName);
	Adesk::Boolean  G_drawspi(AcGiWorldDraw* pWd, double INDATA[5], double LO, xlpoint *P0, xlpoint *PZ, double ND, int icolor = -1);
	//����Ԫ
	AcDbObjectId  GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz);
	AcDbObjectId  GUXWXY(double INDATA[5], struct xlpoint *P0, struct xlpoint *PZ, int bz, int color, double Lwid, ACHAR *LayerName);
	Adesk::Boolean  G_WXY(AcGiWorldDraw* pWd, double INDATA[5], xlpoint *P0, xlpoint *PZ, int icolor = -1);
	bool DrawXY(double XYItem[10], AcDbObjectId& XYId, int iColor);
	//arrayd������ƶ����
	AcDbObjectId createPl_xlpm(int trow, double arrayd[100][6]);

	AcGePoint2d GetOrthPt(AcGePoint2d pt0, AcGePoint2d pt1, AcGePoint2d pt2);
	//���Ʊ��
	void NoteBG(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR *layername = NULL, int Dir = 1);
	void DrawBGTri(AcGePoint3d Opt, double sfactor, int iColor, ACHAR *layername = NULL, int Dir = 1);
	void NoteBGWithLine(AcGePoint3d Opt, double texth, ACHAR Str[], int iColor, int itextColor, double sfactor, int mode, ACHAR* layername = NULL);
	//����solid
	AcDbObjectId  makesolid(AcGePoint3d Pt1, AcGePoint3d Pt2, AcGePoint3d Pt3, int icolor, ACHAR *LayerName);
	//���Ƶ���
	static void makeDX(AcGePoint2dArray PtAy, double R, int icolor, double lw, ACHAR *LayerName, ACHAR *LineType = NULL, bool dxflag = false);//����
	//��dwg�л�ͼ
	bool DrawFromDwgFile(ACHAR *PathName, AcGePoint3d& Pt, ACHAR* Layer, double tuscale, double rotation, bool IsModel);  //pt ��ת���㣨������꣩ 
	//�߿�ת��
	AcDb::LineWeight TranLineWeightFromIntToEnum(int LineWeight) const;

	//***********************CAD������غ���***********************
	//��ȡ�û�ʰȡ�㺯��
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT);
	int  getPoint(AcGePoint3d& pF, ACHAR* pMessage, AcGePoint3d& pT, AcGePoint3d& pT1, AcGePoint3d& pT2);
	int  getPoint(ACHAR* pMessage, AcGePoint3d& pT);
	//��ȡ�麯����
	void  makeGroup(AcDbObjectId groupId);
	//ѡ��ʵ�庯��
	AcDbEntity*  selectEntity(AcDbObjectId& eId, AcDb::OpenMode openMode);
	// ��ĳʵ������ݽ�������в���������������
	struct resbuf *entdxf(struct resbuf *entdata, int gcode);
	//�Ƴ�һ��Group�����е�ʵ�壿
	void removeAllButLines(AcDbObjectId groupId);
	//������
	void groups();
	//������
	void printList(struct resbuf* pBuf);
	//������������ȡʵ��Ķ���
	void iterate(AcDbObjectId plineId);
	//�����������麯��	//20190707���
	AcDbObjectId BAS_DRAW_FUN::CreateGroup(ACHAR *GroupName, AcDbObjectIdArray IdAr) const;	
	//����������3dface
	void CreateAFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color);
	void CreateAFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, AcGePoint3d &p3, Adesk::UInt16 color, ACHAR *LayerName = NULL);
	void CreateFace(AcGePoint3d &p0, AcGePoint3d &p1, AcGePoint3d &p2, Adesk::UInt16 color, ACHAR *LayerName = NULL);
	void CreateFace(double p0[3], double p1[3], double p2[3], Adesk::UInt16 color, ACHAR *LayerName = NULL);
	//������Ԫ��չ����
	void SetXYXdata(AcDbObjectId CurID, AcDbObjectId AftID, double xydata[10]);
	//��ʼ��������
	void InitResBuf(struct  resbuf   *pTemp);
	//����չ���������Ԫ����
	void FormXYarray(AcDbObjectId CurID, CArray<XYarray, XYarray>&pXYArr);
	//���ö˲���������
	void SetDBXdata(AcDbObjectId CurID, CString mdbname, CString roadname, double cml, CString HxBh, double curx = -9999, double cury = -9999, double prex = -9999, double prey = -9999, int xsw = 2);
	//��ȡ�˲���������
	static bool GetDBXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname, double &cml, CString &HxBh, double &curx, double &cury, double &prex, double &prey, int&xsw);
	static void GetDBHXEntity(AcDbObjectIdArray& IdArr, AcArray<CString>& layerArray, CString mdbname, CString roadname, CString HxBh);
	//������·��չ����
	void SetXLXdata(AcDbObjectId CurID, CString mdbname, CString roadname);
	//��ȡ��·��չ����
	bool GetXLXdata(AcDbObjectId CurID, CString &mdbname, CString &roadname);
	//��ʵ���ȡ��չ����
	int GetXdataFromEntity(AcDbObjectId CurID, XYarray &xray);
	//ѡȡԲ��Բ��
	int  GetCircle(ads_name e, ads_point vertex);
	void GetArcPt(double spt[2], double ept[2], double R, int npt, double pt[]);//����ˮ��
	//20190713	����ZOOM����
	static void Zoom(double Left, double Top, double Right, double Bottom);
	//��dwgͼ�д�����
	static AcDbObjectId CreateBlockFromDwgFile(ACHAR *Path, ACHAR *Name);
	//��ȡ��·������ʵ��
	static void GetJD_CENTEREntity(AcDbObjectIdArray& IdArr);
	//��ȡ����ʵ��
	static Acad::ErrorStatus getAllEntity(AcDbDatabase *pDb, AcDbObjectIdArray& IdArr,
		const AcArray<CString>& layerNameArr);
	
	//������
	static struct ptt_node *getArcpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_Arcpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *getplinept(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_plinept(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *getbrkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct ptt_node *get_brkpt(struct resbuf *Entlist, ads_point p0, ads_point p1);
	static struct point_lk *findroot(ads_point ps, ads_point pe, struct arc_lst *pxyr, ads_real bulge,
		ads_point ps1, ads_point ps2);
	static struct arc_lst *findxyr(ads_point ps, ads_point pe, ads_real bulge);
	static int getrbpoint(ads_point pt, struct resbuf *ebuf, int code);
	static struct resbuf *get_resbuf(int code, struct resbuf *p);

	//20190718	����һЩCAD��غ���
	//����ִ�еĶ���׽ģʽ
	static void Set_OSMODE(short Mode_Type);
	//ɾ��ָ����Χ�ڵ�ʵ��
	static void DeleteEntityRectangle(ads_point PtLeftButtom, ads_point PtRightUp);

	//***********************��·���μ�����غ���***********************
	//��ȡ��Ԫ��������Ԫ����
	static double  Get_INDATA_L(double INDATA[5], int *jj);
	//�������Ԫ��INDATA[5]����ָ�����ȣ�l��������
	static int xlzbxy(struct xlpoint *P0, struct xlpoint *PZ, double l, double INDATA[5], int display);
	//����·��һ�����Ϣ�������������̣�
	static int  xlpoint_pz(double array2[][10], int trow2, struct xlpoint *PZ);
	//�����Ԫģ�ͣ�
	int getmodel(double x1, double y1, double x2, double y2, double x3, double y3, double L1, double L2, double R);
	//����������뷽λ��
	static double xyddaa(double xb, double yb, double xe, double ye, double *f);
	//���㻺�����ߵ�x��y
	static double spiral_x(double A, double l);
	static double spiral_y(double A, double l);
	//��������Ҫ��
	int getBPMTL(double a, double R, double L1, double L2, double *BPMTL);
	//���㻺������p
	double spiral_p(double A, double L);
	//���㻺������m
	double spiral_xm(double A, double L);
	//����������ֱ������Ԫ�Ľ���
	int CalXYjd(double Spt[3], double Ept[3], double array2[400][10], int trow2, int kk, double ResPt[2], double &ProjMl);
	//�Ƿ�����Ԫ�н���
	int IfOnXY(double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	//������Ԫ���顢�����ꡢ��Ƕȣ���ͶӰ��
	double Search_EXPt(double array2[400][10], int trow2, double Pt1[3], double jiao, double ResPt[2]);
	//��λpt1��������Ԫ
	int Search_XY(double array2[400][10], int trow2, double Pt1[3]);
	//������Բ�ϵ�����
	double Circle_XY_to_ML(double x, double y, double array2[400][10], int kk);
	double Circle_XY_to_ML(double x, double y, double array2[10]);
	//����һ����̣�һ����Ԫ������������Ԫ���ݣ����߼��,����ͶӰ������̼�ͶӰ��
	int CalXjj(double ml1, double array1[400][10], int tarray1, double array2[400][10], int tarray2, double &xjj, double &projml, double resPt[2]);
	//���������㽻��
	void calptan(double x1, double y1, double x2, double y2, double x3, double y3, double xt, double yt, double *tanl, double *R, double *xc, double *yc, double *ang, double *startfw, double *endfw);
	void caljd(double x1, double y1, double x2, double y2, double x_1, double y_1, double x_2, double y_2, double *xjd, double *yjd);
	void calzxjd(double x1, double y1, double x2, double y2, double a2, double b2, double c2, double *xjd, double *yjd);
	//����һ����һ��PZ,������Ԫ����
	int Judge2XLorR(xlpoint PZ, double array2[400][10], int tarray2);
	//������Ԫ�볤�ȼ�������
	int GetXYonSomeXY(double array2[10], double jslen, double respt[3]);
	//��������Ԫ�Ľ���
	int CalJD_XY1_XY2(double XY1[10], double XY2[10], double ResPt[3], double ResPt1[3]);
	//����Ƿ���Բ����
	bool CheckIfOnArc(double array[10], double pt[3]);
	//������Ԫָ����������ͷ�λ��
	void CalXYXY(double XY[10], double len, double &x, double &y, double &fwj);
	//������Ԫ
	void ReverseXY(double XY[10]);
	//���������·���Ҳ�
	int JudgePTLorR(double N, double E, double &dist, double array[4 * MAXJDNUM][10], int tarray);
	//����ָ������ں���Ԫ��
	int FindiXY(double cml, double array2[][10], int trow2);
	//����Ϊ�������������ߵ��г����㣬����ת�ǣ�Բ���߰뾶����һ���Ͷ���ʼ�뾶����һ���Ͷγ����ڶ����Ͷ���ֹ�뾶���ڶ����Ͷγ������г�
	void get_T_Len(double zj, double R, double RB, double L1, double RE, double L2, double &T1, double &T2, double &L);
	//���㻺����������
	void Get_HHQX_T1T2(double A, double RB, double RE, double &TB, double &TE);
	//�����·�Ƿ��ཻ
	void CheckIfXLintersect(double array1[4 * MAXJDNUM][10], double JDarray1[MAXJDNUM][6], int NunJD1, double array2[4 * MAXJDNUM][10], double JDarray2[MAXJDNUM][6], int NunJD2, AcGePoint2dArray* JDpt);
	//�Ƿ��������ཻ
	bool IfTriangleXJ(AcGePoint3dArray Triangle1pt, AcGePoint3dArray Triangle2pt);
	//������·����
	void CalXLjd(double array1[4 * MAXJDNUM][10], int Sxynum, int Exynum, double array2[4 * MAXJDNUM][10], int Sxynum2, int Exynum2, AcGePoint2dArray* JDpt);
	//����Ƿ��ཻ
	bool checkIfXJ(double array[10], double array2[10], double depth);

	//������Ԫ����
	int CalJD_ZX_ZX(double Line1_array[10], double Line2_array[10], double ResPt[2]);
	int CalJD_ZX_ARC(double Line_array[10], double ARC_array[10], double ResPt[2], double ResPt1[2]);
	int CalJD_ZX_Spiral(double Line_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_ARC_Spiral(double Arc_array[10], double Spiral_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_Arc_Arc(double Arc1_array[10], double Arc2_array[10], double ResPt[3], double ResPt1[3]);
	int CalJD_Spiral_Spiral(double Spiral1_array[10], double Spiral2_array[10], double ResPt[3], double ResPt1[3]);

	int CalJD_ARC_Spiral_diedai(double &l2_arc, double &l2_spiral, double Arc_R, double Arc_Spt[3], double Arc_Len, double Arc_ZorY,
		double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY);
	int CalJD_line_Spiral_diedai(double &l2_line, double &l2_spiral, double line_Spt[3], double line_Len,
		double Spiral_A, double Spiral_Spt[3], double Spiral_Len, double Spiral_ZorY);
	int CalJD_ARC_ARC_diedai(double &l2_arc1, double &l2_arc2, double Arc1_R, double Arc1_Spt[3], double Arc1_Len, double Arc1_ZorY,
		double Arc2_R, double Arc2_Spt[3], double Arc2_Len, double Arc2_ZorY);
	int CalJD_Spiral_Spiral_diedai(double &l2_spiral1, double &l2_spiral2, double Spiral1_A, double Spiral1_Spt[3], double Spiral1_Len, double Spiral1_ZorY,
		double Spiral2_A, double Spiral2_Spt[3], double Spiral2_Len, double Spiral2_ZorY);

    
	//***********************�Ƕȼ��㺯��***********************
    //���ȷ��루double����ת�����ȡ�
	static double rad(double d_ms);
    //�����ȡ�ת���ȷ��루double����
	static double dms_rad(double rad);
    //���ȷ��루double����ת��������
	static double deg(double d_ms);
    //��������ת���ȷ��루double����
	static double dms(double ddd);
	//��������ת���ȷ��루�ַ�����������С��λ�����ƣ�
	double dms(double ddd, int Nw, ACHAR* DMS_Str);
	//��������ת���ȷ��루�ַ������룩��
	void atodmss(double alpha, int &d, ACHAR mm[4], ACHAR ss[4]);
	//���ȷ��롿ת���ȷ��루�ַ�������
	void dTodms(double dd, int Nw, ACHAR *dms);
	//��ⷽλ��
	void CheckFwj(double &fwj);
	double FormFwj(double& dFwj);
	static void StdJiao(double &jiao);
	//����������X��ľ���
	double angleX(AcGePoint3d sPtL, AcGePoint3d ePtL);//Line(sPtl,ePtl)
	double angleX(AcDbLine *pL);
	//�������㷽λ�Ǻ;���
	double distfwj(double xb, double yb, double xe, double ye, double *f);//��Y��н�f,��������֮��
	//����λ�ǡ�ת����X��нǡ�
	double FwjtoAngX(double ang);
	//���㷽λ��
	double fwj(AcGePoint3d sPt, AcGePoint3d ePt);//��Y��н�
	double fwj(AcDbLine *pL);//��Y��н�
	//����ת���
	double PXJ(double fwj1, double fwj2);
	//���㷽λ��
	double fwj(double x0, double y0, double x1, double y1);
	//����ת���
	double AFA(double fwj1, double fwj2);


	//***********************���μ��㺯��***********************
	//����߲ຯ��
	double GetSide(double Xo, double Yo, double x1, double y1, double x, double y);
	//��ֱ�߶���ֱ�߶εĽ��� ���ѿ������꣩
	int crossll(ads_point p1, ads_point p2, ads_point p3, ads_point p4, ads_point pp);
	//����ֱ�߽���
	static int INTERS(double *x, double *y, double X1, double Y1, double X2, double Y2, double X3, double Y3, double X4, double Y4);
	//������뺯��
	static double caldistance(ads_point pt1, ads_point pt2);
	//������
	void CalLineExtension(int n, double l, double &xval, double &yval);
	//�ж�������trino���Ƿ������PT �� ����true �� ����false 
	bool CheckPtInTri(AcGePoint3d PPT, AcGePoint3d ppt0, AcGePoint3d ppt1, AcGePoint3d ppt2);
	//�������Ҳ�
	double chkd(double pt11[3], double pt22[3], double pt33[3]);
	double CCW2(double pt1[2], double pt2[2], double pt3[2]);
	//����ֱ�߷�λ�Ƿֱ�ΪFwj1��Fwj2��������н�
	double CalJiaJiao(double Fwj1, double Fwj2);
	//�ɷ�λ���������
	static AcGeVector3d calfwjvec(double fwj);
	//��������ȡ����
	int GetLeftOrRight3JD(double N1, double E1, double N0, double E0, double N2, double E2);
	void GetCenter3JD(double N1, double E1, double N0, double E0, double N2, double E2, double R, double Lo1, double Lo2, double &CenN, double &CenE);
	void GetQD(double CenN, double CenE, double R,
		double OutN, double OutE, int LRFlag,
		double &QDN, double &QDE);
	//���㻡��
	double GetArcLengthBy2PointOnCircleAndLR(double CenN, double CenE, double R,
		double N1, double E1, double N2, double E2, int lr);
	//������Բ
	double getArc3Pt(AcDbArc *pArc, AcGePoint3d &ptb, AcGePoint3d &pt, AcGePoint3d &pte);//ƽ��
	AcDbArc *getArcFrom3Pt(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte);//ƽ��
	void getArcCenFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2, AcGePoint3d &cen);//ƽ��//���ذ뾶��INPUT��Բ��������
	AcDbArc* getArc(double x1, double y1, double x0, double y0, double x2, double y2);//ƽ��//���ذ뾶��INPUT��Բ��������
	double getArc3Pt(AcDbArc *pArc, double &x1, double &y1, double &x0, double &y0, double &x2, double &y2);//ƽ��
	int getLRRot(AcGePoint3d ptb, AcGePoint3d pt, AcGePoint3d pte);
	double getArcFrom3Pt(double x1, double y1, double x0, double y0, double x2, double y2);//ƽ��//���ذ뾶��INPUT��Բ��������
	//�㵽ֱ�߾���
	double distptol(AcGePoint3d pt, AcDbLine *Line);
	//�㵽��Ԫ�ϵ������
	double CircleNearestPoint(AcGePoint3d Pt, AcDbCircle *Circle, AcGePoint3d &NearestPt);
	double ArcNearestPoint(AcGePoint3d Pt, AcDbArc *Arc, AcGePoint3d &NearestPt);
	static double LineNearestPoint(AcGePoint3d Pt, AcDbLine *Line, AcGePoint3d &NearestPt);
	//��㵽�ߵľ��� 
	static double DistToLine(double inX1, double inY1, double inX2, double inY2, double &outX,
		double &outY, double &distTo2PointOnLine);
	//����������Բ���߰뾶
	double radiusfree(AcDbLine *Line1, AcDbLine *Line2, AcGePoint3d pk);
	//��������
	bool GetTangent(double CenX, double CenY, double R, double  &OutX, double  &OutY, double  &TX, double &TY);
	//��ȡֱ���յ�
	void getLineEndPt(AcGePoint3d sPt, double len, double fwj, AcGePoint3d &ePt);
	//���㴹ֱ��
	void GetPerPoint(double outX, double outY, double x1, double y1, double x2, double y2, double &perX, double &perY);
	//plh04.14	TPRȷ��Բ
	bool GetCircleFromT_P_R(double x1, double y1, double x2, double y2, double R, double &cenX, double &cenY);
	//plh04.14//����һ�����㣬ֱʼx1,y1,����x0,y0,��ֹx2,y2,�Ƿ����ذ뾶ϵ�У����ذ뾶
	double GetRFromT1T2(double &x1, double &y1, double &x0, double &y0, double &x2, double &y2, bool IsStandard = false, double DV = 350, int INTRadius = 10, int JUDGET012 = 0);
	//plh04.14//�������߷��򣬷��ذ뾶,ǰһ���е㣬���㣬��һ���е�
	double GetRFromT_P_P(double x1, double y1, double x2, double y2, double &xs, double &ys, double &x0, double &y0, double &xe, double &ye, double &cenX, double &cenY, AcDbObjectIdArray &eraseId, bool IsStandard = true, double DV = 350);
	

	//***********************ͨ�ü��㺯��***********************
	//�׳�
	static double factorialfun(int n);
	//Ѱ���¶�
	int FindiPD(double cml, double CM[], int NCM);
	//���ط���
	static int SIGN(double a);
	//������
	virtual double func(double x);
	//���������ַ����ƽ�ָ�
	void mnbrak(double& ax, double& bx, double& cx, double& fa, double& fb, double& fc);
	double golden(double ax, double bx, double cx, double tol, double& xmin);

	//***********************ͨ�ò�������***********************
    //ת��С��λ��
	static void RToS(double lc, int NLC, ACHAR ch[]);
	//��עʱ����ֵתΪ����ͬʱȥ����Ч����
	CString DoubleToStr(double xz);
	//Ѱ��ָ���ַ�c���ַ����е�λ��str
	int F_num(ACHAR *str, ACHAR c);
	//������ַ����м��ϡ�+��	put "+" to mileage (e.g. CK153500.0 --> CK153+500.0)
	void putplus(ACHAR str[], ACHAR in_ch, ACHAR s_ch, int place);
	//�ո���$֮���໥ת��
	void NullInStrto$(CString& text);
	static void InStr$toNull();
	static CString InStr$toNull(CString text);

	//��ȡ����Ŀ¼
	int ReadRailWorkdir(ACHAR workdir[256], ACHAR pro[256]);
	static int ReadWorkdir();
	//�����ݿ��ȫ·���õ�������
	CString GetProByMdbName(CString MdbName);
	//��ȡ��ǰ����MDB��
	static CString GetCurProMdbPath();

	//������
	void CalArrowPt(AcGePoint3d pt, double fwj, double RePt[2][2]);

	//ƽ����ͼ�ù��ߺ���
	bool BAS_DRAW_FUN::CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &pt1, ads_point &pt2, ads_point &pt3, ads_point &pt4);
	bool CalFrame(double d12, double d23, double d34, double d41, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, AcGePoint3d &Pt1, AcGePoint3d &Pt2, AcGePoint3d &Pt3, AcGePoint3d &Pt4);
	void ZoomTriangleByPoint_O(double dist, AcGePoint3d O, AcGePoint3d &P1, AcGePoint3d &P2);
	double distptol(AcGePoint3d O, AcGePoint3d P1, AcGePoint3d P2);
	//��ʼ�����Ƶ�·����
	void IniMdfRoadData(CString MdfRoadNmae, ModifyRoadTab& ModifyRoad);
	//��ȡ�ı��ļ�������
	int CountLines(ACHAR *filename);

	
	//***********************��·�����غ���***********************
	//�ںź�dml��ϳ��ֳ����
	static CString LCchr(CString GH, double lc, int NLC);
	static ACHAR*  LCchr(ACHAR  *GH, double lc, int NLC, int mode);
	//ͳһ���ת�ֳ���̣������ںż��㣩
	double XLC1(double TYLC, int NDL, DLBZ DLBTMP[30]);
	//ͳһ���ת�ֳ���̣����ںż��㣩
	double XLC1(double TYLC, ACHAR* GH, int NDL, DLBZ DLBTMP[30]);
	//�ֳ����תͳһ��̣����ںż��㣩
	double TYLC1(ACHAR  ckml[], int NDL, DLBZ DLBTMP[30]);
	//�ֳ����תͳһ��̣������ںż��㣩
	double TYLC1(double  dml, int NDL, DLBZ DLBTMP[30]);
	//����ת���
	static double ZXJ(double s2, double s1);
	//����ͶӰ���
	double PROJ_ML(double array2[][10], int trow2, double xout, double yout, double xon, double yon);
	double XY_to_ML(double x, double y, double array2[400][10], int trow2);

	//���ֳ����
	void split_ckml(CString str, double *ml, CString &ck);
	void split_ckml(ACHAR str[], double *ml, ACHAR  *ck);
	//dmlתCkml
	void dmltockml(CString GH, double dml, ACHAR ckml[80]);


	//***********************�淶������غ���***********************
	//����������
	double Get_Rshu(double design_V, double deltaI);
	//���㻺������
	double get_l0(double SR, double Design_V);
	double get_l0(double Design_V, int KNDD, double SR);
	//double get_l0(double R);
	//����Բ���߰뾶
	double get_R(double SR,double Design_V);
	//�����߼��
	double get_xjj(double Design_V);
	//�����߼��ӿ�
	double get_XJJK(double Design_V, double SR);
	//�淶����·���ӿ�
	double Get_QXJK(CString TLDJ, double design_V, double SR);
	//�����ݶ������
	void SetZdmMinPC(int DV, CString &MAXPD, CString &YBPCMin, CString &JXPCMin, CString &T_YBRshuMin, CString &T_JXRShu,
		CString &A_YBRshuMin, CString &A_JXRShu, CString &YBLRshuMin, CString &JXLRShuMin);
	//��ȡ��·�ȼ�������ٶ�
	void ReadRoadDV(int &DV, CString &grade);
	//��������ٶ�����·���
	static void SetLMKByDV(CString grade, int DV, double &fgdk, double &lydk, double &cxdk,
		double &lmhp, double &yljk, double &yljhp, double &tljk,
		double &tljhp, int &bkmode, int &sjbgmode);
	//���㳬��
	void FindSuphI(int DV, double MAXCG, double Ig, int &NSuphI, double SuphI[10][2]);
	//����ӿ�
	void FindJK(CString JKLB, int &NJK, double JK[10][2]);
	//ֱ�߳��߽�����
	double FindZXCGJBL(int DV, int mode);
	//����·���ӿ�
	void CalLMK_JK(double cml, int NLMK, LMKdata lmk[], int njk, CgSZ jk[], double &half_fgdk,
		double &lydk, double &cxdk, double &yljk, double &tljk, double &tljhp,
		int &bkmode, int &sjbgmode);
	//���ݳ���ȡ���ѵ�����ֵ
	void FindZD_SuphI(int DV, int &NSuphI, double SuphI[10][2]);
	//��ȡ��С�������߳�
	void GetLoMin(double DV, double &Lmin);
	//��ȡ��С���߳�
	void GetCirLenMin(double DV, double &Lmin);
	//����Ƿ���ͬƽ��
	bool checkIfSamePm(double cml, double DV, double array2[][10], int trow2);
	//����Ƿ���ͬ����
	bool checkIfSameZm(double cml, double DV, double CM[], int NCM);
	//��ȡ��СT����СL	plh04.14
	virtual void GetT0min_Lrmin(double DV, double &T0min, double &Lrmin);
	//���㲻ͬ��������СԲ���߰뾶	20190724
	double Cal_Road_MinR(double DesignSpeed);

	//***********************�����غ���***********************
	void fit(double x[], double y[], int ndata, double sig[], int mwt, double& a,
		double& b, double& siga, double& sigb, double& chi2, double& q);
	double gammq(double a, double x);
	void gser(double& gamser, double& a, double& x, double& gln);
	void gcf(double& gammcf, double& a, double& x, double& gln);
	double gammln(double xx);
	void medfit(double x[], double y[], int ndata, double& a, double& b, double& abdev);
	double rofunc(double b, double xt[], double yt[], int ndatat, double &abdevt);
	void  sort(int n, double ra[]);
	int sgn(double number);

	//***********************ͼ����غ���***********************
	////TRIM//////////////////////////////////////////////////////////////////////	
	static void DelBoxObject(ads_point p0, ads_point p1, ads_point p2, ads_point p3);
	static int AutoEntBK(ads_point p0, ads_point p1, ads_point p2, ads_point p3, int ITK = 1234);
	static int ZDKAutoEntBK(ads_point qLpt, ads_point hLpt, ads_point hRpt, ads_point qRpt, int ITK);
	static int AutoEntBK1(ads_point p0, ads_point p1);
	static void fline(ads_point pt1, ads_point pt2, ads_point result, double delta);
	static void CalFrame(double dist, AcGePoint3d P1, AcGePoint3d P2, AcGePoint3d P3, AcGePoint3d P4, ads_point &p1, ads_point &p2, ads_point &p3, ads_point &p4);
	static void AutoEntTransform(AcGeMatrix3d xformr, AcGeMatrix3d xformt, ads_point p1, ads_point p2, ads_point p3, ads_point p4);
	////CAL TU KUANG//////////////////////////////////////////////////////////////////////	
	static void getMinMaxXY(ads_point p1, ads_point p2, ads_point p3, ads_point p4, double &xmin, double &xmax, double &ymin, double &ymax);
	static double cal_h(ads_point pt1, ads_point pt2, ads_point pt3);
	static double FAngle(double s1, double s2);
	static double chk(double pt1[3], double pt2[3], double pt3[3]);
	//////////////////////////////////////////////////////////////////////////	

	// ����Ԫ(����)ģʽ�� 2005.07.14 ldf
	bool Msf2Elem(AcDbObjectId Id1, AcDbObjectId Id2, int &iLinkModel, int LeftOrRight, double A[2], double XYArray[2][10], int& iXYNum);
	// ������Բ��ϵ���Զ�ȷ�����ӷ�ʽ 2005.07.14 ldf	
	int  AutoModel(AcGePoint3d Center1, AcGePoint3d Center2, double R1, double R2);
	//����Ԫ(��չ)ģʽ������
	bool Msf3Elem(AcDbObjectId sId, AcDbObjectId cId, AcDbObjectId eId, int& iQLinkModel, int& iHLinkModel, int LeftOrRight, double A[4],
		double XYArray[5][10], int& iXYNum);
	//����Ԫ(����)ģʽ������(�̶���㼰���Ӳ���A)
	bool Msf2ElemFitA(AcDbObjectId Id1, AcDbObjectId Id2, AcGePoint3d BasePt, double dFwj, int iLinkModel, int LeftOrRight, double A[2],
		double XYArray[2][10], int& iXYNum);
	//�������ϲ�����
	void MergeBX(BXLMKD &oldbx, BXLMKD &newbx, int inf, BXLMKD &resbx);
	//�������������̲߳�������Ҫ,���¼����¶�\�³�
	void NewDegree(vector<PDTAB> &Slope);
	//������̼���߳�
	double MLToH_Bas(double cml, int ncm, double cm[], double& degree);
	double VerticleArc(int iSlope, double cml, double cm[]);
	struct ConstraintErr
	{
		double CurDml;
		ACHAR InterCkml[20];
		double InterN;
		double interE;
		double CurFwj;
		double InterFwj;
		double JiaJiao;
		double CurDesH;
		double InterDesH;
		double DetaH;
		double MinAng;
		double MinDetaH;
		ConstraintErr()
		{
			CurDml = 0.0;
			_tcscpy(InterCkml, L"");
			InterN = 0.0;
			interE = 0.0;
			CurFwj = 0.0;
			InterFwj = 0.0;
			JiaJiao = 0.0;
			CurDesH = 0.0;
			InterDesH = 0.0;
			DetaH = 0.0;
			DetaH = 0.0;
			MinAng = 0.0;
			MinDetaH = 0.0;
		}

	};//�����㽻��Լ���Ĵ�����Ϣ
} ;

#endif // !defined(AFX_BAS_DRAW_FUN1_H__F676E28D_A3DF_437B_B1D4_BB9824BE6095__INCLUDED_)
