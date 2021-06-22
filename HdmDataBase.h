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

	double YSXS[6];//压实系数
	DLBZ *DLArray;
	int NDL;

	int NZDM,NYDM; //路基限宽
    dmdata *ZDM,*YDM;
	
	int NZPWD,NYPWD;
	pwddata *ZPWD,*YPWD;//路基本体外宽
	
	int NZHTHD,NYHTHD;
	hthddata *ZHTHD,*YHTHD;//清表土厚度
	
	int NZCRS,NYCRS;
	crswiddata *ZCRSWID,*YCRSWID;
	
	int NZCTKD,NYCTKD;//超填宽度
	ctkddata *ZCTKD,*YCTKD;
	
	int NZARCBP,NYARCBP;//弧形边坡
    arcbpata *ZARCBP,*YARCBP;
	
	////////路面宽
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
	
	/////////路堑水沟
	int NZLQSG,NYLQSG;
	LQSGdata *ZLQSG,*YLQSG;
	
	int NZLDSG,NYLDSG;
	LDSGdata *ZLDSG,*YLDSG;
	int NZTG,NYTG;
	TGdata *ZTG,*YTG;
	
	///////路堤填料
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
  
//	struct Hdmresdata//横断面成果数据结构
//	{
//		CString GH;
//	    double dml;//里程数
//		double dmbg;//地面标高
//		double sjbg;//设计标高
//        int Lnpt;//左设计点数
//		double RPTL[SJPT*2];
//		int Rnpt;//右设计点数
//		double RPTR[SJPT*2];
//		int Lsg_inf;//水沟信息 1=水沟、0=天沟
//		int Rsg_inf;//水沟信息 1=水沟、0=天沟
//		int Lgpt;//左沟点数
//		double Lgou[SGPT*2];
//		int Rgpt;//右沟点数
//        double Rgou[SGPT*2];
//	};
   
   struct HdmAreaData//横断面成果面积数据结构
   {
	   CString GH;
	   double dml;
       int BTinf;//断面性质
	   double at;//填面积
       double aglt;//改良土面积
	   double asst;//渗水土面积
	   double aqbt;// 清表土面积
	   double aw[6];//挖面积
	   double LZDK;//左占地宽
	   double RZDK;//右占地宽
	   double ahj;// 护肩面积
	   double asgt;//水沟填面积
	   double asgw;//水沟挖面积
	   double ajq;//浆砌面积
	   double cpbpxc;//草皮边坡斜长
	   double dqbpxc;//挡墙边坡斜长

   };
   struct GuiFanCanShu
   {
	   char TieLuDengJi[24];//铁路等级1
	   int SuDuMuBiao;//速度目标2
	   short ZhXianShu;//正线数目3,1表示单线，2表示一次双线，3表示双线预留二线
	   float XianJianJu;//线间距4
	   int MinR1;//一般最小线半径5
	   int MinR2;//困难最小线半径6
	   float XianZhiPo;//限制坡度7上行
	   float XianZhiPo1;//限制坡度7下行
	   int DaoFaXian;//到发线8
	   short GuiDao;//轨道类型：0-4：无逢特-无逢轻，5-9：有逢特-有逢轻
	   short QianYin;//牵引种类：1-电力，2-内燃，3-蒸汽
	   char JiChe[24];//机车类型11
	   char BiSai[24];//闭塞12
	   char KongZhi[24];//列车控制方式13
	   char ZhiHei[24];//列车指挥方式14
	   short RXiangShu;//采用的半径系列项数，最多20
	   int R[20];//曲线半径系列
	   int L[20];//缓和曲线长度配置，与曲线半径一一对应：R[i]配L[i]
	   int TJianR1;//推荐半径小
	   int TJianR2;//推荐半径大
	   int MinZhi;//最小直线长度
	   int MinYuan;//最小圆曲线长度
	   int JiaKuan[20];//曲线加宽，与曲线半径一一对应：R[i]配JiaKuan [i]
	   int MinPoChang[7];//最小坡段长度，对应于6种到发线长度,0位是用户选择的数据
	   int MaxPoCha[7];//最大坡差
	   float ShuTiaoJ;//竖曲线条件
	   int ShuR;//竖曲线半径
	   bool ZheJian;//曲线折减
	   int SuiDaoFJ[4];//隧道长度分级
	   float SDXiShuD[5];//隧道折减系数（电力）
	   float SDXiShuN[5];//隧道折减系数（内燃）
	   float JiBiaoHou;//基床表层厚度
	   float WuFJiaK;//路基面无缝加宽
	   float LuJiM1;//单线路堤路基面
	   float LuJiM2;//单线路堑路基面
	   float LuJiM3;//双线路堤路基面
	   float LuJiM4;//双线路堑路基面
	   short QuXianFJi;//曲线半径分级数5
	   int QuXianRFJ[5];//曲线半径分级
	   float LuJiMJiaK[6];//路基面曲线加宽值
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
	
  	int m_RoadSum;//道路数
	struct RoadData
	{
       CString RoadName;
	   CString BGH,EGH;
	   double BLC,ELC;
	} Road[50];//最多50条路
	
	int NZDQ,NYDQ;
    dqdata *ZDQ,*YDQ;

	int NZLDTP,NYLDTP;
    LDTPData *ZLDTP,*YLDTP;//低填路堤填平数据
	
	int NZLQTP,NYLQTP;
    LQTPData *ZLQTP,*YLQTP;//低挖路堑推平数据
	
	int NZTLJJK,NYTLJJK;
    TLJJKData *ZTLJJK,*YTLJJK;//土路肩加宽数据

	std::vector<RoadLayerData> m_RoadLayerArray_L, m_RoadLayerArray_R;	//20191017	路面分层表

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
	
	bool InitTableList(void);//取出mdb中的所有表名
	bool FindTable(CString strTempName);//查找表是否存在于mdb中
	int AttachMdb(CString HdmMdbname,CString roadname,CString TabName);
	bool ConnectMdb();
	void deleteDataFrmMdb(CString tabName,CString RoadName);

	//int Attach_ZYJSMdb(CString TabName);
	void ReadDLB();
	int GetRoadSEml(CString mdbFname,CString RoadName,double &Sxlc,double &Exlc);
	bool Read_JBiaoTuFromGuiFan();
	int Read_AllHdmDataFromDbs(CString mdbFname,CString roadname);

	int ReadAllRoadName(CString mdbFname);//读取线路数据库中断链表的数据
	bool PutRnameInArray(CString rname);//读取线路数据库中断链表的数据
	int SetRoadSEml(CString mdbFname,CString RoadName,int iRoad);
	void SortRoad();
	void WriteGouPDB(CString HdmMdbname,CString RoadName,CString bzFrame);
};

#endif // !defined(AFX_HDMDATABASE_H__D92896AF_63A8_403B_AF7C_6CEADC7D16BB__INCLUDED_)
