// DrawPM.h: interface for the CDrawPM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_)
#define AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "XLDataBase.h"
#include "JD_CENTER.h"
#include "HdmDes.h"
#include "Struct.h"

static CString	BM="ˮ׼��";
static CString	DaoXian="����";

class CDrawPM  : public BAS_DRAW_FUN
{
public:
	//********************************�ṹ��********************************
	struct CRSDAT
	{
		ACHAR ckml[80];
		double cml;
		double dml;
		int DorS;
		double DuanLuoInf;
		double xjj;//�߼�࣬����Ϊ0,��������Ϊ-����Ϊ+
		double ZDesH, YDesH;
		double LZDK, RZDK;
		//DesH·�����Ƹ̺߳�EarH����̣߳�HDM�Ļ�ͼ���ݣ�//ָ����
		double RHFL[10], RHFR[10];  //����·���4��
		double zlmhd[3], ylmhd[3];
		int ZDMXZ, YDMXZ;//���ҷ���������
		double  Zt, Yt;
		double ZHc, YHc, Y_ZHc, Y_YHc;//���ڸ�
		int zptsum, rptsum, Lnpt, Rnpt;
		AcGePoint3dArray RPTL, RPTR;
		AcGePoint3dArray PTRPTL, PTRPTR;
		int Ljdnum, Rjdnum;
		//ָ����
		double FLDcml;//��ص�·ͶӰ���
		CString FLDname;//��ص�·��
		double Y_RHFL[10], Y_RHFR[10];  //�����·���4��
		double Y_zlmhd[3], Y_ylmhd[3];
		int Y_ZDMXZ, Y_YDMXZ;//���ҷ���������
		double  Y_Zt, Y_Yt;
		int Y_Lnpt, Y_Rnpt;
		AcGePoint3dArray Y_RPTL, Y_RPTR;
		AcGePoint3dArray Y_PTRPTL, Y_PTRPTR;
		int Y_Ljdnum, Y_Rjdnum;

		double Ldeltah, Rdeltah;//����·���������ߵĸ߲��
		double Y_Ldeltah, Y_Rdeltah;//����·���������ߵĸ߲��
		double newLYd, newRYd;//��·�����ĵĵ���߲㣬��·�����ĵĵ���߲�
		double  at, aw;
		double Ldh, Rdh;
		double DX;
		//		//���߻�һ��
		int LGouPtNum, RGouPtNum, LJSGouPtNum, RJSGouPtNum;////////wwq5.14
		int LGouProperty, RGouProperty;
		double Lgou[36], Rgou[36], LJSgou[36], RJSgou[36];//ˮ��7��/////wwq5.14
		AcGePoint3dArray PTLgou, PTRgou, PTLJSgou, PTRJSgou;
		//	
		//		//����
		int Y_LGouPtNum, Y_RGouPtNum, Y_LJSGouPtNum, Y_RJSGouPtNum;////////wwq5.14
		int Y_LGouProperty, Y_RGouProperty;
		double Y_Lgou[36], Y_Rgou[36], Y_LJSgou[36], Y_RJSgou[36];//ˮ��7��/////wwq5.14
		AcGePoint3dArray Y_PTLgou, Y_PTRgou, Y_PTLJSgou, Y_PTRJSgou;
		double CenPt[2];//��·���ĵ�����
		double fwj;

		int IfHasZlink, IfHasYlink;
		CRSDAT* zlink, * ylink;
		double ZDorS, YDorS;//1-��·�棬2-˫·��ͬ�� 3-˫·�淴��
		double ZCG_GDR, YCG_GDR;
		int IsInsertHdm;
		double OffsetByXLZX;
		//���߻�һ��		
	};

	struct Hole
	{
		double qInpt[2];
		double Inpt[2];
		int nLpt, nRpt;
		AcGePoint3dArray LptArr;
		AcGePoint3dArray RptArr;
		AcDbPolyline* pline;
	} Ahole;

	struct BriBJX
	{
		double qpt[3];
		double hpt[3];
	} Abjx;

	struct ZDKDAT
	{
		double cml;
		double LZDK;
		double RZDK;
		int IsInsertHdm;
		//		double OutPt[2];//ռ�ؽ��������ɾ��ʵ��
	};

	//********************************��Ա����********************************
	//****************ͼ�λ��Ʋ���****************
	double DFACTOR;		//��ͼ����
	int XSW; //С��λ��	0 1 2 3
	double LcSpacer;//��̱�ע��� 100��500��1000��
	double BGarowcml;
	double ZdkZlen, ZdkYlen;		//ռ�ؿ����	ռ�ؿ��ҿ�
	int ZdkMode;	//ռ�ؿ��עģʽ
	bool BzRAL;//��עR\A\L
	bool drawfldzdk;//true ���Ʒ�������β�λռ��  false ������
	double Sdml, Edml;//��ͼ�����ֳ����
	double Scml, Ecml;//��ͼ�����������
	double ZxWid;	//���߿��
	double YdxWid;	//�õ��߿��
	double texth;	//�ָ�
	int m_ZDKLineColor;	//ռ�ؿ�����ɫ
	bool m_IsGutter;	//�Ƿ��ǸĹ�ģʽ

	//****************�������****************
	XLDataBase DBS;		//���ݿ��д
	JD_CENTER *pm;		//ƽ��ָ��
	CRSDAT* hdmdes;
	HdmDes hdm;
	CArray<Hole, Hole>HoleArray;
	int HoleNum;
	int NCRS;
	int Count;
	CArray<BriBJX, BriBJX>BriBjArr;
	int NBriBj;
	ZDKDAT* pZDK;
	int NZDK;
	bool ifsgjk;
	double SmlOnZx, EmlOnZx, SPXJJ;
	//����·�������������ϵ�ͶӰ���
	//	double LJFLSmlOnZx,LJFLEmlOnZx,LJFLSmlOnFLD,LJFLEmlOnFLD;
	//���ձ��·�����������ϵ�ͶӰ���
	double BPFLSmlOnZx, BPFLEmlOnZx, BPFLSmlOnFLD, BPFLEmlOnFLD;
	int NZLMK, NYLMK;
	LMKdata* ZLMK, * YLMK;
	int BxNum;
	BXLMKD* pBXArr;
	GTZDM_ROAD* zdm, * Zzdm, * Yzdm;
	GLROAD ZGLR, YGLR;
	DrawHorizontalPara m_DrawPara;	//��ͼ����	20191231���
	
	//********************************��Ա����********************************
	//****************���졢��������****************
	CDrawPM();
	CDrawPM(JD_CENTER* pz, double sdml, double edml);
	virtual ~CDrawPM();
	//****************���㺯��****************
	void ReadHdmResultFile();	//��ȡ��������ļ�
	void ReadACrossResultData(FILE* fpr, int jcross);	//��ȡ�����������ļ�
	void ReadLMK(CString LMKTABname);		//��ȡ·�����Ϣ�����ļ��ж�ȡ��
	int FindIlmk(double cml, LMKdata lmk[], int nlmk);		//����·�����Ϣ
	double GetLMK(double cml, LMKdata lmk[], int nlmk);		//��ȡ·������㣩
	void CalZdkFld(int& FldSum, double SEml[50][3]);		//���㣨��ȡ����ռ�ؿ�����
	void CalOffsetByXLZX(double cml, int NLMK, LMKdata lmk[], double& OffsetByXLZX);	//����·�������������·���ߵ�ƫ����
	void formZPHole(int i, int ZorY, int qdmxz, int hdmxz, int qoppxz, int hoppxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr);		//���ɶ��ļ���
	void CalTWZeroPt(double bml, double eml, double Bdeltah, double Edeltah, double& k, double& MidMl);	//�����������
	int CalBjPt(double cml, double ang, int mark, ads_point pt1, ads_point pt2, double& x, double& y);	//����߽��
	void CheckZdkInf(int FldSum, double SEml[50][3], ZDKDAT& ZDK);	//���ռ�ؿ���Ϣ
	void FormHole();
	void CalBriSlopPt(int i, int ZorY, AcGePoint3dArray& ptarr);
	int Cal_ZPJD(int ZorY, int HorT, AcGePoint3d Cenpt, AcGePoint3d Spt, AcGePoint3d Ept, AcGePoint3dArray& ptarr);//����׶�½ŵ�
	void formZorYHole(int i, int ZorY, double qdeltah, double hdeltah, double qtw, double htw, int qdmxz, int hdmxz, int qoppxz, int hoppxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, double Ht);
	void CalYLMHt(double N, double E, double& prjml, double& Ht, double& lmhp);
	void CalTLMHt(double NY, double EY, double NT, double ET, double& prjml, double& Ht, double& Tljhp);
	double FindBriAng(double cml);
	void CalLmht(double N, double E, double& prjml, double& ZJ, double& Desh, double& lmhp, double& Lmbg);
	void CalLMKbyFile(double cml, int ZorY, double& half_fgdk, double& lydk, double& cxdk, double& yljk, double& tljk);
	int CalLMBG(double ZJ, double RHF[10], double& Lmbg, double& lmhp);
	int CalLMBGOnNose(double N, double E, HdmDes::RoadHdmdata& ZX, HdmDes::RoadHdmdata& ZD, double& Lmbg, double& lmhp);
	int CalLMBGOnNose(double N, double E, HdmDes::RoadHdmdata& ZX, HdmDes::RoadHdmdata& ZD, double& Lmbg, double& lmhp, double& ZJ, double& Desh);
	void CalLMKNoJK(double cml, int NLMK, LMKdata lmk[], double& half_fgdk,
		double& lydk, double& cxdk, double& yljk, double& tljk,
		double& lmhp, double& yljhp, double& tljhp,
		int& bkmode, int& sjbgmode);

	void line0(double x1, double y1, double x2, double y2, int color, ACHAR* layer);	//��������

	//****************����·����****************
	void Draw_PI_Infor();	//���ƽ�����Ϣ	20190706���	
	void Draw_Center_Line();//������·������	20190707���	
	void DrawJDLine();		//���ƽ�������
	void BZLC(int inf);		//��ע���	0��λͼ 1 ƽ��ͼ
	void BZTZD(int inf);	//��ע����׮	0��λͼ 1 ƽ��ͼ
	void DrawDZQ();			//���ƴ�����
	void DrawTun();			//�������
	void DrawDX();			//���Ƶ���
	void DrawBM();			//����ˮ׼��
	int DrawCulvet();		//����С�ź�
	void DrawXLZX();		//������·���ߣ���ʱδ�ã�
	void DrawSeML();		//��ע���յ�
	void DrawLsxLine(ads_point bpt, double cita, double lsxang);	//����С�ź���
	void INSERT_DX(double XX, double YY, CString BianHao, double LEV);		//���뵼��
	void INSERT_BM(double XX, double YY, CString BianHao, double LEV);		//����ˮ׼��
	void DrawBGDATA();//���Ʊ������ͼ
	//****************���õ�ͼ****************
	void DrawZDK();		//����ռ�ؿ�
	void BZ_ZDK();		//��עռ�ؿ�
	//****************��ƽ������ͼ****************
	void DrawPartFillBetweenTwoMl(double sml, double eml, int ZorY, int qPtNum, int hPtNum,
		AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& flag);		//���
	void DrawCutPartBetweenTwoMl(double sml, double eml, int ZorY, int qPtNum, int hPtNum,
		AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& flag);		//���ڷ�
	int draw_fill_cut();	//��������
	void DrawZTT();			//��������ͼ
	void drawGouArrow(ads_point pt, double angle, int inf);	//���Ƽ�ͷ
	void DRAW_GOU(int qPtNum, int hPtNum, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, int ZorY);
	void DRAW_GOU(AcGePoint3d qpt, AcGePoint3d hpt, double qfwj, double hfwj, int qPtNum, int hPtNum, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, int ZorY);
	void DRAW_ALLGOU();	//����ˮ����
	int drawfillandcut(int i, int ZorY, double qdeltah, double hdeltah, double qtw, double htw, int qdmxz, int hdmxz, int qnpt, int hnpt, AcGePoint3dArray& qPtarr, AcGePoint3dArray& hPtarr, bool& tflag, bool& wflag);	//��������
	void DrawAllSlop(int i);	//���������¶�
	void Draw_BridgeSlop(ads_point bspt, ads_point rpt, ads_point spt);	//���������¶ȣ�
	void  DRAW_TW_GOU(CRSDAT Qhdm, CRSDAT Hhdm);
};

#endif // !defined(AFX_DRAWPM_H__86B33287_1904_4A6E_B6FC_8697F8935EC1__INCLUDED_)





















