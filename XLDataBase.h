// XLDataBase.h: interface for the XLDataBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
#define AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ado.h"
#include "bas_draw_fun.h"
#include "Struct.h"
#include <vector>

class XLDataBase  : public BAS_DRAW_FUN
{
public:
	XLDataBase();
	virtual ~XLDataBase();
    
	ModifyRoadTab* pModifyRoad;
	int ModifyRoadSum;

	DoubleLineData *QXB2x;//二线曲线表
	int NQX2x;//二线曲线数目

	EXDLB *DLB2x;//二线断链表
	int NDL2x;//断链数

	YongDiLXB* YDLXB;//用地类型表
	int YDLXSum;//用地类型分段总数

	QXTAB *QXB;//曲线表
	int NQX;

	CTRLHT *pCtrlHt;
	int CRNUM;
	
	DXSTRUCT *DX;
	int DXNum;
	
	BMSTRUCT *BM;
	int BmNum;
	
	XLManager *XLMan;//线路管理
	int NXL;

	JdCenter *JdCenArray;
	int NJdCen;
	
	DLBZ *DLArray;
	int NDL;

	DLBZ *ZDLArray;
	int NZDL;
	
	ZQZJTAB *ZQZJB;

	TUNTAB *Tun;
	int TunNum;
	
	BRITAB *Bri;
	int BriNum;

	PDTAB *PDB;
	int NPD;

	JPInfor *JPB;
	int JPnum;

	TWTAB *TWB;//纵地面线表
	int NTW;

	Culvet *CUL;
	int NCU;
	
	double (*XYArray)[10];//  	
	int XYNum;//线元数

	struct DMXCS{
		double ZJ;//左负右正 0 为中线
		double sdml;
		double edml;//起终里程
		int Color;//绘该地面线的颜色
	} DmxCsArr[50];
	int NDMX;
	double Scml,Ecml;//纵断面图中绘制边线地面线起终里程
	double Hmin,Hmax;
	int Color;//纵断面图中绘制边线地面线的颜色
	double ZJ;

	RoadProperty RoadXZ;//道路性质
	ZdmDesStandard ZDMBZ;//纵断面设计标准
	CGJSCS CalCGCs;

	LMKdata *lmk;
	int NLMK;

	PPCDdata *ppcd;
	int NPPCD;

    CgSZ CG[500];
	int NCG;
	
	LCBdata *STA;
	int NSTA;

	//20190808	横断面数据
	//松方压实系数
	double YSXS[6];
	//填方分类
	std::vector<LDTLdata> m_FillType;
	//用地面积汇总分段
	std::vector<YongDiFDB> m_EarthSection;
	//挖方分类
	std::vector<WFFLdata> m_CutType;
	//关联断面路肩拉直
	std::vector<GLDMLZdata> m_GLDMLZ;
	//关联断面路堑边坡推平
	std::vector<GLDMLQBPTPdata> m_GLDMLQBPTP;
	//关联断面线间沟
	std::vector<GLDMXJGdata> m_GLDMXJG;
	//清表土厚度
	std::vector<hthddata> m_HTHD;
	//沟底坡
	std::vector<GouPDdata> m_GouPD;
	//超填宽度
	std::vector<ctkddata> m_CTKD;
	//弧形边坡
	std::vector<arcbpata> m_ARCBP;
	//挡墙
	std::vector<dqdata> m_DQ;
	//低填路堤自动填平
	std::vector<LDTPData> m_LDTP;
	//低挖路堑自动推平
	std::vector<LQTPData> m_LQTP;
	//低填路堤自动加宽土路肩
	std::vector<TLJJKData> m_TLJJK;
	//路面厚度
	std::vector<LMHdata> m_LMHD;
	//地面线内插宽
	std::vector<crswiddata> m_CRSWID;
	//横断面限制宽度
	std::vector<dmdata> m_DM;
	//用地加宽
	std::vector<pwddata> m_PWD;
	//路堤边坡
	std::vector<LDBPdata> m_LDBP;
	//路堑边坡
	std::vector<LDBPdata> m_LQBP;
	//排水沟
	std::vector<LDSGdata> m_LDSG;
	//侧沟
	std::vector<LQSGdata> m_LQSG;
	//天沟
	std::vector<TGdata> m_TG;

	//20190918	控制点标高
	std::vector<GCKZPt> m_LevelCtrlPtArray;

	std::vector<JPInfor>m_JPArray;
	//20191017	路面分层
	std::vector<RoadLayerData> m_RoadLayerArray;

	//20191030	道铁交叉信息
	std::vector<ConstraintErr> m_RoadNRailInterInforArray;
	
	CAdoConnection m_adoConnection;
	CString	m_strSQL;
	CString	m_strError;
	CAdoRecordSet Report_input;
	CStringArray m_strTableNameList;

	void Release();
	int  IsMdb2(CString MdbName, int &isEX);
	int ReadAllXZ(CString mdbFname,CString RoadName);//读取线路数据库中坡度表的数据
    int Read_XLDbs(CString mdbFname,CString TabFname);//读取线路数据库中的数据
	int Read_XLDbs_Railway(CString mdbFname, CString TabFname);	//20191029新增	读取铁路数据库中数据
//	int Write_XLDbs(CString mdbFname,CString TabFname);//写入线路数据库中的数据
	bool InitTableList(void);//取出mdb中的所有表名
	bool FindTable(CString strTempName);//查找表是否存在于mdb中

	void GetAllXLName(CStringArray *XLNameArray);
	bool GetiFLDDLInfo(int iFLD,int &sDLNo,int &eDLNo);
	int Read_EX_XLDbs(CString mdbFname,CString TabFname,int irxd);//读取线路数据库中的二线数据
	int Read_XLDbs(CString mdbFname,CString TabFname,CString RoadName);//读取线路数据库中的数据
	int Read_XLDbs(CString mdbFname,CString TabFname,CString RoadName,CString XZ);
	int Write_XLDbs(CString mdbFname,CString TabFname,CString RoadName);//写入线路数据库中的数据
    int Write_XLDbs(CString mdbFname,CString TabFname,CString RoadName,CString XZ);//写入线路数据库中的数据
	int ReadAllRoadName();
	int m_RoadSum;//道路数
	struct RoadData
	{
       CString RoadName;
	   CString BGH,EGH;
	   double BLC,ELC;
	} Road[50];//最多50条路

	CString RoadZ,RoadY;		

	double SuphData[10][2];
	int NuphData;
	double JKData[10][2];
	int NJK;

	bool PutRnameInArray(CString rname);//读取线路数据库中断链表的数据
	int SetRoadSEml(CString mdbFname,CString RoadName,int iRoad);
	int ReadAllRoadName(CString mdbFname);//读取线路数据库中断链表的数据
	void SortRoad();
	int DelAllRecords(CString mdbFname,CString TabFname);//删除表中所有记录
	int AttachMdb(CString HdmMdbname,CString RoadName,CString TabName);
};

#endif // !defined(AFX_XLDATABASE_H__DE0A0B2C_C97D_48EB_BA2B_CFE66E46A810__INCLUDED_)
