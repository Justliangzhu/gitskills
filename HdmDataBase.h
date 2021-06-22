// HdmDataBase.h: interface for the HdmDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HDMDATABASE_H__D92896AF_63A8_403B_AF7C_6CEADC7D16BB__INCLUDED_)
#define AFX_HDMDATABASE_H__D92896AF_63A8_403B_AF7C_6CEADC7D16BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ado.h"
#include "bas_draw_fun.h"
class HdmDataBase : public BAS_DRAW_FUN  
{
public:
	HdmDataBase();
	virtual ~HdmDataBase();

	double YSXS[6];//ѹʵϵ��
	DLBZ *DLArray;
	int NDL;

	int NZDM,NYDM; //·���޿�
    dmdata *ZDM,*YDM;
	
	int NZPWD,NYPWD;
	pwddata *ZPWD,*YPWD;//·���������
	
	int NZHTHD,NYHTHD;
	hthddata *ZHTHD,*YHTHD;//��������
	
	int NZCRS,NYCRS;
	crswiddata *ZCRSWID,*YCRSWID;
	
	int NZCTKD,NYCTKD;//������
	ctkddata *ZCTKD,*YCTKD;
	
	int NZARCBP,NYARCBP;//���α���
    arcbpata *ZARCBP,*YARCBP;
	
	////////·���
	/*
	int NZLMK,NYLMK;
	struct LMKdata
	{ 
	char ckml[40];
	double halfwid;
	int mode[2];
	} *ZLMK,*YLMK;
	*/
	
	int NZLDBP,NYLDBP,NZLQBP,NYLQBP;
	LDBPdata ZLDBP[200],YLDBP[200],ZLQBP[200],YLQBP[200];
	
	/////////·ǵˮ��
	int NZLQSG,NYLQSG;
	LQSGdata *ZLQSG,*YLQSG;
	
	int NZLDSG,NYLDSG;
	LDSGdata *ZLDSG,*YLDSG;
	int NZTG,NYTG;
	TGdata *ZTG,*YTG;
	
	///////·������
	int NLDTL;
	LDTLdata *LDTL;

	int NWFFL;
	WFFLdata *WFFL;

	int NGLDMLZ;
	GLDMLZdata *GLDMLZ;

	int NGLDMLQBPTP;
	GLDMLQBPTPdata *GLDMLQBPTP;

	int NGLDMXJG;
	GLDMXJGdata *GLDMXJG;

	int NYD;
	YongDiFDB *FDB;

	int NYDLX;
    YongDiLXB * LXB;
	    
	int NDMX;
	DMXdata *DMX;


	int NZLMHD,NYLMHD;
	LMHdata *ZLMHD,*YLMHD;

	
	int NZGouPD,NYGouPD;
	GouPDdata *ZGouPD,*YGouPD;
  
//	struct Hdmresdata//�����ɹ����ݽṹ
//	{
//		CString GH;
//	    double dml;//�����
//		double dmbg;//������
//		double sjbg;//��Ʊ��
//        int Lnpt;//����Ƶ���
//		double RPTL[SJPT*2];
//		int Rnpt;//����Ƶ���
//		double RPTR[SJPT*2];
//		int Lsg_inf;//ˮ����Ϣ 1=ˮ����0=�칵
//		int Rsg_inf;//ˮ����Ϣ 1=ˮ����0=�칵
//		int Lgpt;//�󹵵���
//		double Lgou[SGPT*2];
//		int Rgpt;//�ҹ�����
//        double Rgou[SGPT*2];
//	};
   
   struct HdmAreaData//�����ɹ�������ݽṹ
   {
	   CString GH;
	   double dml;
       int BTinf;//��������
	   double at;//�����
       double aglt;//���������
	   double asst;//��ˮ�����
	   double aqbt;// ��������
	   double aw[6];//�����
	   double LZDK;//��ռ�ؿ�
	   double RZDK;//��ռ�ؿ�
	   double ahj;// �������
	   double asgt;//ˮ�������
	   double asgw;//ˮ�������
	   double ajq;//�������
	   double cpbpxc;//��Ƥ����б��
	   double dqbpxc;//��ǽ����б��

   };
   struct GuiFanCanShu
   {
	   char TieLuDengJi[24];//��·�ȼ�1
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
	   char JiChe[24];//��������11
	   char BiSai[24];//����12
	   char KongZhi[24];//�г����Ʒ�ʽ13
	   char ZhiHei[24];//�г�ָ�ӷ�ʽ14
	   short RXiangShu;//���õİ뾶ϵ�����������20
	   int R[20];//���߰뾶ϵ��
	   int L[20];//�������߳������ã������߰뾶һһ��Ӧ��R[i]��L[i]
	   int TJianR1;//�Ƽ��뾶С
	   int TJianR2;//�Ƽ��뾶��
	   int MinZhi;//��Сֱ�߳���
	   int MinYuan;//��СԲ���߳���
	   int JiaKuan[20];//���߼ӿ������߰뾶һһ��Ӧ��R[i]��JiaKuan [i]
	   int MinPoChang[7];//��С�¶γ��ȣ���Ӧ��6�ֵ����߳���,0λ���û�ѡ�������
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
   };
   
   struct QuXianZheJiang
   {
	   float ZhJiangVal[8][8];
   };	
   //int NJ_S;
   CFile guifanFile;
   GuiFanCanShu mGuiFanCS;

 
//   Hdmresdata* Res;
//   int NRES;
//   HdmAreaData* Area;
//   int NArea;
	
  	int m_RoadSum;//��·��
	struct RoadData
	{
       CString RoadName;
	   CString BGH,EGH;
	   double BLC,ELC;
	} Road[50];//���50��·
	
	int NZDQ,NYDQ;
    dqdata *ZDQ,*YDQ;

	int NZLDTP,NYLDTP;
    LDTPData *ZLDTP,*YLDTP;//����·����ƽ����
	
	int NZLQTP,NYLQTP;
    LQTPData *ZLQTP,*YLQTP;//����·ǵ��ƽ����
	
	int NZTLJJK,NYTLJJK;
    TLJJKData *ZTLJJK,*YTLJJK;//��·��ӿ�����

	std::vector<RoadLayerData> m_RoadLayerArray_L, m_RoadLayerArray_R;	//20191017	·��ֲ��

	public:
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
	CStringArray m_strTableNameList;
	CString mdbFname;
	CString RoadName;
	
	void Write_DmxDb();
	void Write_HdmDesresDb();
	void Write_HdmAreaDb();
	int Read_HdmDbs(CString mdbFname,CString roadname,CString TabName);
	void  WriteHdmMdb(CString HdmMdbname,CString RoadName,int xh);
	
	bool InitTableList(void);//ȡ��mdb�е����б���
	bool FindTable(CString strTempName);//���ұ��Ƿ������mdb��
	int AttachMdb(CString HdmMdbname,CString roadname,CString TabName);
	bool ConnectMdb();
	void deleteDataFrmMdb(CString tabName,CString RoadName);

	//int Attach_ZYJSMdb(CString TabName);
	void ReadDLB();
	int GetRoadSEml(CString mdbFname,CString RoadName,double &Sxlc,double &Exlc);
	bool Read_JBiaoTuFromGuiFan();
	int Read_AllHdmDataFromDbs(CString mdbFname,CString roadname);

	int ReadAllRoadName(CString mdbFname);//��ȡ��·���ݿ��ж����������
	bool PutRnameInArray(CString rname);//��ȡ��·���ݿ��ж����������
	int SetRoadSEml(CString mdbFname,CString RoadName,int iRoad);
	void SortRoad();
	void WriteGouPDB(CString HdmMdbname,CString RoadName,CString bzFrame);
};

#endif // !defined(AFX_HDMDATABASE_H__D92896AF_63A8_403B_AF7C_6CEADC7D16BB__INCLUDED_)
